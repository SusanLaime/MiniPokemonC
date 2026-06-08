#include "../include/visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int node_id_counter = 0;

static void generar_nodos_dot(FILE* f, const NodoArbol* nodo, int parent_id, int is_ast) {
    int current_id = node_id_counter++;
    
    const char* shape = "ellipse";
    const char* style = "filled";
    char color[32] = "#b3d4ff"; // Azul claro por defecto para no terminales en CST y todo en AST
    char fontcolor[32] = "black";

    if (!is_ast) {
        if (strcmp(nodo->valor, "epsilon") == 0) {
            shape = "box";
            strcpy(color, "#cccccc");
            strcpy(fontcolor, "dimgrey");
        } else if (es_terminal(nodo->valor)) {
            shape = "box";
            style = "\"rounded,filled\"";
            strcpy(color, "#a9dfbf");
        }
    }
    
    char label[256];
    if (!is_ast && nodo->lexema && strcmp(nodo->valor, "epsilon") != 0) {
        sprintf(label, "%s\\n(%s)", nodo->valor, nodo->lexema);
    } else if (is_ast && nodo->lexema) {
        strcpy(label, nodo->lexema);
    } else {
        strcpy(label, nodo->valor);
    }
    
    fprintf(f, "  node%d [label=\"%s\", shape=\"%s\", style=%s, fillcolor=\"%s\", fontcolor=\"%s\"];\n", 
            current_id, label, shape, style, color, fontcolor);
            
    if (parent_id != -1) {
        fprintf(f, "  node%d -> node%d;\n", parent_id, current_id);
    }
    
    for (int i = 0; i < nodo->num_hijos; i++) {
        generar_nodos_dot(f, nodo->hijos[i], current_id, is_ast);
    }
}

void graficar_arbol(const NodoArbol* raiz, const char* titulo, const char* filename_dot, const char* filename_png, int is_ast) {
    if (!raiz) return;
    
    FILE* f = fopen(filename_dot, "w");
    if (!f) return;
    
    fprintf(f, "digraph G {\n");
    fprintf(f, "  bgcolor=\"#f8f9fa\";\n");
    fprintf(f, "  labelloc=\"t\";\n");
    fprintf(f, "  labeljust=\"l\";\n");
    fprintf(f, "  label=\"%s\";\n", titulo);
    
    node_id_counter = 0;
    generar_nodos_dot(f, raiz, -1, is_ast);
    
    fprintf(f, "}\n");
    fclose(f);
    
    char cmd[512];
    sprintf(cmd, "dot -Tpng %s -o %s", filename_dot, filename_png);
    system(cmd);
}

void graficar_cst(const NodoArbol* raiz, int num_sentencia, const char* texto_original) {
    char dot_file[256], png_file[256], title[512];
    sprintf(dot_file, "images/arboles/cst_%d.dot", num_sentencia);
    sprintf(png_file, "images/arboles/cst_%d.png", num_sentencia);
    sprintf(title, "CST — %s", texto_original);
    
    graficar_arbol(raiz, title, dot_file, png_file, 0);
}

void graficar_ast(const NodoArbol* raiz, int num_sentencia, const char* texto_original) {
    char dot_file[256], png_file[256], title[512];
    sprintf(dot_file, "images/arboles/ast_%d.dot", num_sentencia);
    sprintf(png_file, "images/arboles/ast_%d.png", num_sentencia);
    sprintf(title, "AST — %s", texto_original);
    
    graficar_arbol(raiz, title, dot_file, png_file, 1);
}
