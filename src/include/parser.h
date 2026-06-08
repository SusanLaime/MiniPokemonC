#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ll1.h"
#include "arbol.h"

#define MAX_PRODUCCIONES_EMITIDAS 1000

typedef struct {
    int numero;
    char sentencia_str[1024];
    int valida;
    char regla_aplicada[50];
    char mensaje[256];
    
    ProduccionEmitida producciones[MAX_PRODUCCIONES_EMITIDAS];
    int num_producciones;
} ResultadoLinea;

typedef struct {
    ResultadoLinea resultados[MAX_SENTENCES];
    int num_resultados;
} ParserResult;

ParserResult* parsear_sentencias(const ListaSentencias* sentencias);
void imprimir_resultados_parser(const ParserResult* result);

// Struct para la traza (usado por el visualizador)
#define MAX_TRAZA_PASOS 1000
typedef struct {
    int paso;
    char pila_str[1024];
    char entrada_str[1024];
    char accion[256];
} TrazaPaso;

typedef struct {
    TrazaPaso pasos[MAX_TRAZA_PASOS];
    int num_pasos;
} Traza;

// Guarda la traza del último parseo exitoso para poder graficarla
extern Traza traza_actual;

#endif // PARSER_H
