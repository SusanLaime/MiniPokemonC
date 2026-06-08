#ifndef SCANNER_H
#define SCANNER_H

#include "tokens.h"

// Max number of tokens/errors we expect for a single run
#define MAX_TOKENS 1000
#define MAX_ERRORES 50

typedef struct {
    Token tokens[MAX_TOKENS];
    int num_tokens;
    ErrorLexico errores[MAX_ERRORES];
    int num_errores;
} ScannerResult;

// scannerPokemon.c exports
ScannerResult* escanear_archivo(const char* ruta_archivo);
void imprimir_tokens(const ScannerResult* result);
void imprimir_errores(const ScannerResult* result);
void liberar_scanner_result(ScannerResult* result);

// scannerConfig.c exports
// A 'sentence' is an array of pointers to tokens, ending with a NULL pointer.
// We represent the list of sentences as a 2D array of token types.
#define MAX_SENTENCES 100
#define MAX_TOKENS_PER_SENTENCE 100

typedef struct {
    Token tokens[MAX_TOKENS_PER_SENTENCE];
    int num_tokens;
    char texto[512];
} Sentencia;

typedef struct {
    Sentencia sentencias[MAX_SENTENCES];
    int num_sentencias;
} ListaSentencias;

ListaSentencias* agrupar_sentencias(const ScannerResult* result);
void imprimir_sentencias(const ListaSentencias* lista);

#endif // SCANNER_H
