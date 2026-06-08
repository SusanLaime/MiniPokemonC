#include "src/include/parser.h"
#include <stdio.h>
#include <string.h>

Traza traza_actual;

static void array_to_str(const char** arr, int n, char* out) {
    out[0] = '\0';
    for (int i = 0; i < n; i++) {
        strcat(out, arr[i]);
        if (i < n - 1) strcat(out, " ");
    }
}

static int parsear_predictivo(const Sentencia* s, ResultadoLinea* res) {
    const char* pila[MAX_TOKENS_PER_SENTENCE * 2];
    int top_pila = 0;
    
    const char* entrada[MAX_TOKENS_PER_SENTENCE + 1];
    int len_entrada = 0;
    
    for (int i = 0; i < s->num_tokens; i++) {
        entrada[len_entrada++] = tipo_token_str(s->tokens[i].tipo);
    }
    entrada[len_entrada++] = "$";
    
    pila[top_pila++] = "$";
    pila[top_pila++] = START_SYMBOL;
    
    int paso = 1;
    res->num_producciones = 0;
    traza_actual.num_pasos = 0;
    
    while (top_pila > 0) {
        const char* tope = pila[top_pila - 1];
        const char* lookahead = entrada[0];
        
        char pila_str[1024];
        char entrada_str[1024];
        array_to_str(pila, top_pila, pila_str);
        array_to_str(entrada, len_entrada, entrada_str);
        
        TrazaPaso* tp = &traza_actual.pasos[traza_actual.num_pasos++];
        tp->paso = paso;
        strcpy(tp->pila_str, pila_str);
        strcpy(tp->entrada_str, entrada_str);
        
        if (strcmp(tope, "$") == 0 && strcmp(lookahead, "$") == 0) {
            strcpy(tp->accion, "ACEPTAR");
            break;
        }
        
        if (es_terminal(tope) || strcmp(tope, "$") == 0) {
            if (strcmp(tope, lookahead) == 0) {
                sprintf(tp->accion, "Match %s", lookahead);
                top_pila--;
                for (int i = 0; i < len_entrada - 1; i++) {
                    entrada[i] = entrada[i + 1];
                }
                len_entrada--;
            } else {
                return 0; // Error Sintáctico
            }
        } else {
            Regla* regla = buscar_en_matriz(tope, lookahead);
            if (!regla) return 0; // Error Sintáctico
            
            char prod_str[256] = "";
            for (int i = 0; i < regla->num_simbolos; i++) {
                strcat(prod_str, regla->simbolos[i].simbolo);
                if (i < regla->num_simbolos - 1) strcat(prod_str, " ");
            }
            sprintf(tp->accion, "%s -&gt; %s", tope, prod_str);
            
            top_pila--;
            res->producciones[res->num_producciones].no_terminal = tope;
            res->producciones[res->num_producciones].regla = regla;
            res->num_producciones++;
            
            if (!(regla->num_simbolos == 1 && strcmp(regla->simbolos[0].simbolo, "epsilon") == 0)) {
                for (int i = regla->num_simbolos - 1; i >= 0; i--) {
                    pila[top_pila++] = regla->simbolos[i].simbolo;
                }
            }
        }
        paso++;
    }
    return 1;
}

ParserResult* parsear_sentencias(const ListaSentencias* sentencias) {
    ParserResult* p_res = (ParserResult*)malloc(sizeof(ParserResult));
    p_res->num_resultados = 0;
    
    for (int i = 0; i < sentencias->num_sentencias; i++) {
        const Sentencia* s = &sentencias->sentencias[i];
        ResultadoLinea res;
        res.numero = i + 1;
        
        char s_str[1024] = "";
        for (int j = 0; j < s->num_tokens; j++) {
            strcat(s_str, tipo_token_str(s->tokens[j].tipo));
            if (j < s->num_tokens - 1) strcat(s_str, " ");
        }
        strcpy(res.sentencia_str, s_str);
        
        if (s->num_tokens > 0) {
            const char* prim = tipo_token_str(s->tokens[0].tipo);
            if (strcmp(prim, "VAROOM") == 0) strcpy(res.regla_aplicada, "decl_var");
            else if (strcmp(prim, "INCINEROAR") == 0) strcpy(res.regla_aplicada, "condicional");
            else if (strcmp(prim, "PRINPLUP") == 0) strcpy(res.regla_aplicada, "impresion");
            else if (strcmp(prim, "GARDEVOIR") == 0) strcpy(res.regla_aplicada, "bloque");
            else if (strcmp(prim, "INDEEDEE") == 0) strcpy(res.regla_aplicada, "asignacion");
            else strcpy(res.regla_aplicada, "desconocida");
        } else {
            strcpy(res.regla_aplicada, "vacía");
        }
        
        if (parsear_predictivo(s, &res)) {
            res.valida = 1;
            strcpy(res.mensaje, "Sentencia válida.");
            graficar_traza(&traza_actual, res.numero, s->texto);
            NodoArbol* cst = construir_cst("programa", res.producciones, res.num_producciones);
            
            // Asignar lexemas a los nodos terminales
            int token_idx = 0;
            void asignar_lexemas(NodoArbol* nodo) {
                if (!nodo) return;
                if (nodo->num_hijos == 0 && strcmp(nodo->valor, "epsilon") != 0) {
                    if (token_idx < s->num_tokens) {
                        nodo->lexema = strdup(s->tokens[token_idx].valor);
                        token_idx++;
                    }
                } else {
                    for (int i = 0; i < nodo->num_hijos; i++) {
                        asignar_lexemas(nodo->hijos[i]);
                    }
                }
            }
            asignar_lexemas(cst);
            
            graficar_cst(cst, res.numero, s->texto);
            NodoArbol* ast = convertir_a_ast(cst);
            graficar_ast(ast, res.numero, s->texto);
        } else {
            res.valida = 0;
            strcpy(res.mensaje, "Error sintáctico (tabla predictiva LL1)");
        }
        p_res->resultados[p_res->num_resultados++] = res;
    }
    return p_res;
}

void imprimir_resultados_parser(const ParserResult* result) {
    printf("\n======================================================================\n");
    printf("ANÁLISIS SINTÁCTICO — RESULTADO POR SENTENCIA\n");
    printf("======================================================================\n");
    
    int validas = 0;
    for (int i = 0; i < result->num_resultados; i++) {
        const ResultadoLinea* r = &result->resultados[i];
        if (r->valida) validas++;
        const char* estado = r->valida ? "✓" : "✗";
        printf("  [%3d] %s  %s\n", r->numero, estado, r->sentencia_str);
        printf("        -> Regla: %s\n", r->regla_aplicada);
        printf("        -> %s\n\n", r->mensaje);
    }
    
    int errores = result->num_resultados - validas;
    printf("======================================================================\n");
    printf("RESUMEN DEL PARSER\n");
    printf("======================================================================\n");
    printf("  Sentencias analizadas : %d\n", result->num_resultados);
    printf("  Válidas               : %d\n", validas);
    printf("  Con errores           : %d\n", errores);
    printf("  Estado                : %s\n", errores == 0 ? "✓ ÉXITO" : "✗ CON ERRORES SINTÁCTICOS");
    printf("======================================================================\n");
}
