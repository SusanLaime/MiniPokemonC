#include "../include/arbol.h"
#include "../include/gramatica.h"
#include <stdlib.h>
#include <string.h>

NodoArbol* crear_nodo(const char* valor, const char* lexema) {
    NodoArbol* n = (NodoArbol*)malloc(sizeof(NodoArbol));
    n->valor = strdup(valor);
    n->lexema = lexema ? strdup(lexema) : NULL;
    n->hijos = NULL;
    n->num_hijos = 0;
    n->capacidad_hijos = 0;
    return n;
}

void agregar_hijo(NodoArbol* padre, NodoArbol* hijo) {
    if (padre->num_hijos == padre->capacidad_hijos) {
        padre->capacidad_hijos = padre->capacidad_hijos == 0 ? 4 : padre->capacidad_hijos * 2;
        padre->hijos = (NodoArbol**)realloc(padre->hijos, padre->capacidad_hijos * sizeof(NodoArbol*));
    }
    padre->hijos[padre->num_hijos++] = hijo;
}

void liberar_arbol(NodoArbol* nodo) {
    if (!nodo) return;
    for (int i = 0; i < nodo->num_hijos; i++) {
        liberar_arbol(nodo->hijos[i]);
    }
    free(nodo->hijos);
    free(nodo->valor);
    if (nodo->lexema) free(nodo->lexema);
    free(nodo);
}

static NodoArbol* hoja_izquierda(NodoArbol* nodo) {
    if (nodo->num_hijos == 0 && es_no_terminal(nodo->valor) && strcmp(nodo->valor, "epsilon") != 0) {
        return nodo;
    }
    for (int i = 0; i < nodo->num_hijos; i++) {
        NodoArbol* res = hoja_izquierda(nodo->hijos[i]);
        if (res) return res;
    }
    return NULL;
}

NodoArbol* construir_cst(const char* start_symbol, ProduccionEmitida* producciones, int num_producciones) {
    NodoArbol* raiz = crear_nodo(start_symbol, NULL);
    for (int i = 0; i < num_producciones; i++) {
        NodoArbol* obj = hoja_izquierda(raiz);
        if (!obj) break;
        Regla* r = producciones[i].regla;
        for (int j = 0; j < r->num_simbolos; j++) {
            agregar_hijo(obj, crear_nodo(r->simbolos[j].simbolo, NULL));
        }
    }
    return raiz;
}

// Convert CST to AST
// Rules based on prompt: "El gráfico de los áboles debe ser de distinto color y forma para los no terminales y terminales en el caso del árbol conreto."
// "Para el árbol abstracto, no solo debe escribir su terminal sino también el símbolo como SEEL que es "=" debe implimirlo para que sea más explicito la sentencia."
// We'll remove punctuation tokens and epsilon nodes, and collapse unary rules.
static int is_punctuation(const char* val) {
    return strcmp(val, "PANCHAM") == 0 || strcmp(val, "ZACIAN") == 0 ||
           strcmp(val, "ZAMAZENTA") == 0 || strcmp(val, "GARDEVOIR") == 0 ||
           strcmp(val, "GALLADE") == 0;
}

static const char* map_lexema(const char* val) {
    if (strcmp(val, "SEEL") == 0) return "=";
    if (strcmp(val, "QUAQUAVAL") == 0) return "==";
    if (strcmp(val, "GREEDENT") == 0) return ">";
    if (strcmp(val, "LEDIAN") == 0) return "<";
    if (strcmp(val, "PLUSLE") == 0) return "+";
    if (strcmp(val, "MINUN") == 0) return "-";
    if (strcmp(val, "PORYGON") == 0) return "*";
    if (strcmp(val, "DITTO") == 0) return "/";
    return val;
}

NodoArbol* convertir_a_ast(NodoArbol* cst);

static void extract_expr_nodes(NodoArbol* cst, NodoArbol*** list, int* count, int* cap) {
    if (!cst || strcmp(cst->valor, "epsilon") == 0) return;
    if (strcmp(cst->valor, "termino") == 0) {
        NodoArbol* term = convertir_a_ast(cst->hijos[0]);
        if (term) {
            if (*count == *cap) { *cap = (*cap==0)?4:(*cap*2); *list = realloc(*list, *cap * sizeof(NodoArbol*)); }
            (*list)[(*count)++] = term;
        }
        return;
    }
    if (strcmp(cst->valor, "op_arit") == 0) {
        NodoArbol* op = crear_nodo(cst->hijos[0]->valor, map_lexema(cst->hijos[0]->valor));
        if (*count == *cap) { *cap = (*cap==0)?4:(*cap*2); *list = realloc(*list, *cap * sizeof(NodoArbol*)); }
        (*list)[(*count)++] = op;
        return;
    }
    for (int i=0; i<cst->num_hijos; i++) extract_expr_nodes(cst->hijos[i], list, count, cap);
}

static void extract_seq_nodes(NodoArbol* cst, NodoArbol*** list, int* count, int* cap) {
    if (!cst || strcmp(cst->valor, "epsilon") == 0) return;
    if (strcmp(cst->valor, "sentencia") == 0) {
        NodoArbol* s = convertir_a_ast(cst->hijos[0]);
        if (s) {
            if (*count == *cap) { *cap = (*cap==0)?4:(*cap*2); *list = realloc(*list, *cap * sizeof(NodoArbol*)); }
            (*list)[(*count)++] = s;
        }
        return;
    }
    for (int i=0; i<cst->num_hijos; i++) extract_seq_nodes(cst->hijos[i], list, count, cap);
}

NodoArbol* convertir_a_ast(NodoArbol* cst) {
    if (!cst) return NULL;
    if (strcmp(cst->valor, "epsilon") == 0) return NULL;
    
    if (strcmp(cst->valor, "programa") == 0 || strcmp(cst->valor, "sentencias_bloque") == 0 || strcmp(cst->valor, "sentencia_rest") == 0) {
        NodoArbol** list = NULL;
        int count = 0, cap = 0;
        extract_seq_nodes(cst, &list, &count, &cap);
        if (count == 0) return NULL;
        if (count == 1) {
            NodoArbol* ast = list[0];
            free(list);
            return ast;
        }
        NodoArbol* op = crear_nodo("seq", "seq");
        for (int i=0; i<count; i++) agregar_hijo(op, list[i]);
        free(list);
        return op;
    }

    if (strcmp(cst->valor, "sentencia") == 0) return convertir_a_ast(cst->hijos[0]);

    if (strcmp(cst->valor, "expr") == 0) {
        NodoArbol** list = NULL;
        int count = 0, cap = 0;
        extract_expr_nodes(cst, &list, &count, &cap);
        if (count == 0) return NULL;
        NodoArbol* ast = list[0];
        for (int i=1; i<count-1; i+=2) {
            NodoArbol* op = list[i];
            NodoArbol* rhs = list[i+1];
            agregar_hijo(op, ast);
            agregar_hijo(op, rhs);
            ast = op;
        }
        free(list);
        return ast;
    }

    if (strcmp(cst->valor, "condicion") == 0) {
        NodoArbol* op = crear_nodo(cst->hijos[1]->hijos[0]->valor, map_lexema(cst->hijos[1]->hijos[0]->valor));
        agregar_hijo(op, convertir_a_ast(cst->hijos[0]));
        agregar_hijo(op, convertir_a_ast(cst->hijos[2]));
        return op;
    }

    if (strcmp(cst->valor, "asignacion") == 0) {
        NodoArbol* op = crear_nodo("SEEL", "=");
        agregar_hijo(op, convertir_a_ast(cst->hijos[0]));
        agregar_hijo(op, convertir_a_ast(cst->hijos[2]));
        return op;
    }

    if (strcmp(cst->valor, "decl_var") == 0) {
        NodoArbol* op = crear_nodo("SEEL", "=");
        agregar_hijo(op, convertir_a_ast(cst->hijos[1]));
        agregar_hijo(op, convertir_a_ast(cst->hijos[3]));
        return op;
    }

    if (strcmp(cst->valor, "impresion") == 0) {
        NodoArbol* op = crear_nodo("PRINPLUP", "PRINPLUP");
        agregar_hijo(op, convertir_a_ast(cst->hijos[2]));
        return op;
    }

    if (strcmp(cst->valor, "bucle_while") == 0) {
        NodoArbol* op = crear_nodo("WAILMER", "WAILMER");
        agregar_hijo(op, convertir_a_ast(cst->hijos[2]));
        agregar_hijo(op, convertir_a_ast(cst->hijos[4]));
        return op;
    }

    if (strcmp(cst->valor, "bucle_for") == 0) {
        NodoArbol* op = crear_nodo("FORRETRESS", "FORRETRESS");
        NodoArbol* init = crear_nodo("SEEL", "=");
        agregar_hijo(init, convertir_a_ast(cst->hijos[3]));
        agregar_hijo(init, convertir_a_ast(cst->hijos[5]));
        agregar_hijo(op, init);
        agregar_hijo(op, convertir_a_ast(cst->hijos[7]));
        NodoArbol* inc = crear_nodo("SEEL", "=");
        agregar_hijo(inc, convertir_a_ast(cst->hijos[9]));
        agregar_hijo(inc, convertir_a_ast(cst->hijos[11]));
        agregar_hijo(op, inc);
        agregar_hijo(op, convertir_a_ast(cst->hijos[13]));
        return op;
    }

    if (strcmp(cst->valor, "condicional") == 0) {
        NodoArbol* op = crear_nodo("INCINEROAR", "INCINEROAR");
        agregar_hijo(op, convertir_a_ast(cst->hijos[2]));
        agregar_hijo(op, convertir_a_ast(cst->hijos[4]));
        NodoArbol* e = convertir_a_ast(cst->hijos[5]);
        if (e) agregar_hijo(op, e);
        return op;
    }

    if (strcmp(cst->valor, "else_opt") == 0) {
        if (cst->num_hijos == 1) return NULL;
        NodoArbol* op = crear_nodo("ELEKID", "ELEKID");
        agregar_hijo(op, convertir_a_ast(cst->hijos[1]));
        return op;
    }

    if (strcmp(cst->valor, "sentencia_control") == 0) {
        return crear_nodo(cst->hijos[0]->valor, map_lexema(cst->hijos[0]->valor));
    }

    if (strcmp(cst->valor, "bloque") == 0) {
        NodoArbol* op = crear_nodo("bloque", "bloque");
        NodoArbol* seq = convertir_a_ast(cst->hijos[1]);
        if (seq) {
            if (strcmp(seq->valor, "seq") == 0) {
                for (int i=0; i<seq->num_hijos; i++) agregar_hijo(op, seq->hijos[i]);
                seq->num_hijos = 0;
                liberar_arbol(seq);
            } else {
                agregar_hijo(op, seq);
            }
        }
        return op;
    }

    if (es_terminal(cst->valor)) {
        if (is_punctuation(cst->valor)) return NULL;
        const char* lex = cst->lexema ? cst->lexema : map_lexema(cst->valor);
        return crear_nodo(cst->valor, lex);
    }

    return NULL;
}
