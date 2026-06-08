#include "src/include/scanner.h"
#include <stdio.h>
#include <string.h>

ListaSentencias* agrupar_sentencias(const ScannerResult* result) {
    ListaSentencias* lista = (ListaSentencias*)malloc(sizeof(ListaSentencias));
    lista->num_sentencias = 0;

    if (!result || result->num_errores > 0) {
        return lista;
    }

    Sentencia acumulador;
    acumulador.num_tokens = 0;
    acumulador.texto[0] = '\0';
    int previous_line = -1;

    for (int i = 0; i < result->num_tokens; i++) {
        const Token* token = &result->tokens[i];
        
        if (token->tipo == TOKEN_EOF || token->tipo == TOKEN_ERROR) {
            continue;
        }

        // Si hay un salto de más de 1 línea y el acumulador tiene datos, cerramos la sentencia.
        if (previous_line != -1 && (token->linea - previous_line) > 1) {
            if (acumulador.num_tokens > 0 && lista->num_sentencias < MAX_SENTENCES) {
                lista->sentencias[lista->num_sentencias++] = acumulador;
            }
            acumulador.num_tokens = 0;
            acumulador.texto[0] = '\0';
        }

        if (acumulador.num_tokens < MAX_TOKENS_PER_SENTENCE) {
            acumulador.tokens[acumulador.num_tokens++] = *token;
            if (strlen(acumulador.texto) > 0) strcat(acumulador.texto, " ");
            strcat(acumulador.texto, token->valor);
        }
        
        previous_line = token->linea;
    }

    // Agregar la última sentencia que quede pendiente
    if (acumulador.num_tokens > 0) {
        if (lista->num_sentencias < MAX_SENTENCES) {
            lista->sentencias[lista->num_sentencias++] = acumulador;
        }
    }

    return lista;
}

void imprimir_sentencias(const ListaSentencias* lista) {
    printf("\n======================================================================\n");
    printf("SENTENCIAS SIMBÓLICAS  (una por línea de código)\n");
    printf("======================================================================\n");
    if (lista->num_sentencias == 0) {
        printf("  (sin sentencias)\n");
    }
    for (int i = 0; i < lista->num_sentencias; i++) {
        printf("  [%3d]  ", i + 1);
        const Sentencia* s = &lista->sentencias[i];
        for (int j = 0; j < s->num_tokens; j++) {
            printf("%s ", tipo_token_str(s->tokens[j].tipo));
        }
        printf("\n");
    }
    printf("======================================================================\n");
}
