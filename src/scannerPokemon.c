#include "src/include/scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

// Helper functions removed

ScannerResult* escanear_archivo(const char* ruta_archivo) {
    ScannerResult* result = (ScannerResult*)malloc(sizeof(ScannerResult));
    result->num_tokens = 0;
    result->num_errores = 0;

    FILE* f = fopen(ruta_archivo, "r");
    if (!f) {
        printf("ERROR: No existe '%s'\n", ruta_archivo);
        return result;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* codigo = (char*)malloc(fsize + 1);
    fread(codigo, 1, fsize, f);
    codigo[fsize] = 0;
    fclose(f);

    int pos = 0;
    int linea = 1;
    int columna = 1;

    while (pos < fsize) {
        // Ignorar espacios y comentarios
        if (codigo[pos] == ' ' || codigo[pos] == '\t' || codigo[pos] == '\r') {
            pos++;
            columna++;
            continue;
        } else if (codigo[pos] == '\n') {
            pos++;
            linea++;
            columna = 1;
            continue;
        } else if (strncmp(&codigo[pos], "//", 2) == 0) {
            while (pos < fsize && codigo[pos] != '\n') {
                pos++;
            }
            continue;
        }

        int match_len = 0;
        TipoToken t_tipo = TOKEN_UNKNOWN;

        // Regex definitions matching Python's TOKENS_REGEX
        struct {
            const char* pattern;
            TipoToken tipo;
        } regex_rules[] = {
            {"^\\bvaroom\\b", TOKEN_VAROOM},
            {"^\\bprinplup\\b", TOKEN_PRINPLUP},
            {"^\\bincineroar\\b", TOKEN_INCINEROAR},
            {"^\\belekid\\b", TOKEN_ELEKID},
            {"^\\bwailmer\\b", TOKEN_WAILMER},
            {"^\\bforretress\\b", TOKEN_FORRETRESS},
            {"^\\bbraixen\\b", TOKEN_BRAIXEN},
            {"^\\bting-lu\\b", TOKEN_TINGLU},
            {"^\\bfloat\\b", TOKEN_FLOATZEL},
            {"^==", TOKEN_QUAQUAVAL},
            {"^=", TOKEN_SEEL},
            {"^>", TOKEN_GREEDENT},
            {"^<", TOKEN_LEDIAN},
            {"^\\+", TOKEN_PLUSLE},
            {"^-", TOKEN_MINUN},
            {"^\\*", TOKEN_PORYGON},
            {"^/", TOKEN_DITTO},
            {"^\\(", TOKEN_ZACIAN},
            {"^\\)", TOKEN_ZAMAZENTA},
            {"^\\{", TOKEN_GARDEVOIR},
            {"^\\}", TOKEN_GALLADE},
            {"^;", TOKEN_PANCHAM},
            {"^[0-9]+(\\.[0-9]+)?", TOKEN_NUMEL},
            {"^[a-zA-Z_][a-zA-Z0-9_]*", TOKEN_INDEEDEE}
        };

        for (int i = 0; i < sizeof(regex_rules)/sizeof(regex_rules[0]); i++) {
            regex_t regex;
            if (regcomp(&regex, regex_rules[i].pattern, REG_EXTENDED) != 0) {
                continue;
            }
            regmatch_t pmatch[1];
            if (regexec(&regex, &codigo[pos], 1, pmatch, 0) == 0 && pmatch[0].rm_so == 0) {
                match_len = pmatch[0].rm_eo;
                t_tipo = regex_rules[i].tipo;
                regfree(&regex);
                break;
            }
            regfree(&regex);
        }

        if (t_tipo != TOKEN_UNKNOWN) {
            Token t;
            t.tipo = t_tipo;
            t.valor = strndup(&codigo[pos], match_len);
            t.linea = linea;
            t.columna = columna;
            if (result->num_tokens < MAX_TOKENS) {
                result->tokens[result->num_tokens++] = t;
            }
            pos += match_len;
            columna += match_len;
        } else {
            ErrorLexico e;
            e.linea = linea;
            e.columna = columna;
            e.contenido = strndup(&codigo[pos], 1);
            e.motivo = strdup("Carácter no reconocido");
            if (result->num_errores < MAX_ERRORES) {
                result->errores[result->num_errores++] = e;
            }
            pos++;
            columna++;
        }
    }

    Token eof;
    eof.tipo = TOKEN_EOF;
    eof.valor = strdup("");
    eof.linea = linea;
    eof.columna = columna;
    if (result->num_tokens < MAX_TOKENS) {
        result->tokens[result->num_tokens++] = eof;
    }

    free(codigo);
    return result;
}

void imprimir_tokens(const ScannerResult* result) {
    printf("\n======================================================================\n");
    printf("TABLA DE TOKENS\n");
    printf("======================================================================\n");
    for (int i = 0; i < result->num_tokens; i++) {
        const Token* t = &result->tokens[i];
        printf("Token(%-12s | valor='%s' | línea=%d, col=%d)\n", 
            tipo_token_str(t->tipo), t->valor, t->linea, t->columna);
    }
    printf("======================================================================\n");
}

void imprimir_errores(const ScannerResult* result) {
    if (result->num_errores == 0) {
        printf("\n✓ Sin errores léxicos\n\n");
        return;
    }
    printf("\n======================================================================\n");
    printf("ERRORES LÉXICOS\n");
    printf("======================================================================\n");
    for (int i = 0; i < result->num_errores; i++) {
        const ErrorLexico* e = &result->errores[i];
        printf("[ERROR] Línea %d, Columna %d -> '%s' : %s\n", 
            e->linea, e->columna, e->contenido, e->motivo);
    }
    printf("======================================================================\n");
}

void liberar_scanner_result(ScannerResult* result) {
    for (int i = 0; i < result->num_tokens; i++) {
        free(result->tokens[i].valor);
    }
    for (int i = 0; i < result->num_errores; i++) {
        free(result->errores[i].contenido);
        free(result->errores[i].motivo);
    }
    result->num_tokens = 0;
    result->num_errores = 0;
}
