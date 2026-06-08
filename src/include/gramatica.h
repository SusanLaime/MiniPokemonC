#ifndef GRAMATICA_H
#define GRAMATICA_H

#include "tokens.h"

// We use string representation for NonTerminals to match the python code
#define MAX_PROD_LENGTH 20
#define MAX_REGLAS_PER_NT 10
#define MAX_NT 30

typedef struct {
    const char* simbolo; // Terminal string (e.g. "VAROOM") or NonTerminal string (e.g. "decl_var")
    int es_terminal;
} Simbolo;

typedef struct {
    Simbolo simbolos[MAX_PROD_LENGTH];
    int num_simbolos;
} Regla;

typedef struct {
    const char* no_terminal;
    Regla reglas[MAX_REGLAS_PER_NT];
    int num_reglas;
} ProduccionesNT;

extern const char* START_SYMBOL;
extern const char* NON_TERMINALS[];
extern int NUM_NON_TERMINALS;
extern const char* TERMINALS[];
extern int NUM_TERMINALS;

extern ProduccionesNT GRAMMAR[];
extern int NUM_GRAMMAR_RULES;

// Helper functions to work with grammar
int es_no_terminal(const char* simbolo);
int es_terminal(const char* simbolo);

#endif // GRAMATICA_H
