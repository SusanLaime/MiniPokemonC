#ifndef ARBOL_H
#define ARBOL_H

#include "gramatica.h"

typedef struct NodoArbol {
    char* valor;    // e.g. "decl_var" or "VAROOM"
    char* lexema;   // null for non-terminals, string for terminals
    struct NodoArbol** hijos;
    int num_hijos;
    int capacidad_hijos;
} NodoArbol;

NodoArbol* crear_nodo(const char* valor, const char* lexema);
void agregar_hijo(NodoArbol* padre, NodoArbol* hijo);
void liberar_arbol(NodoArbol* nodo);

// Produccion Emitida from parser
typedef struct {
    const char* no_terminal;
    Regla* regla;
} ProduccionEmitida;

NodoArbol* construir_cst(const char* start_symbol, ProduccionEmitida* producciones, int num_producciones);
NodoArbol* convertir_a_ast(NodoArbol* cst);

#endif // ARBOL_H
