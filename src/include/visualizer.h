#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "ll1.h"
#include "parser.h"
#include "arbol.h"

// Tablas
void graficar_first_follow();
void graficar_select();
void graficar_matriz_predictiva();
void graficar_traza(const Traza* traza, int num_sentencia, const char* texto_original);

// Arboles
void graficar_arbol(const NodoArbol* raiz, const char* titulo, const char* filename_dot, const char* filename_png, int is_ast);
void graficar_cst(const NodoArbol* raiz, int num_sentencia, const char* texto_original);
void graficar_ast(const NodoArbol* raiz, int num_sentencia, const char* texto_original);

#endif // VISUALIZER_H
