#ifndef LL1_H
#define LL1_H

#include "gramatica.h"

#define MAX_SET_SIZE 50

typedef struct {
    const char* elementos[MAX_SET_SIZE];
    int num_elementos;
} ConjuntoString;

// Matrix for FIRST and FOLLOW mapping a NonTerminal name to a set of Terminal names
typedef struct {
    const char* no_terminal;
    ConjuntoString conjunto;
} MapaConjunto;

extern MapaConjunto LL1_FIRST[MAX_NT];
extern MapaConjunto LL1_FOLLOW[MAX_NT];

void calcular_first();
void calcular_follow();
ConjuntoString obtener_first_de_secuencia(Simbolo* secuencia, int len);

typedef struct {
    const char* no_terminal;
    int regla_idx; // Index of the rule in GRAMMAR[no_terminal]
    ConjuntoString conjunto; // Terminals that select this rule
} SelectEntry;

#define MAX_SELECT_ENTRIES 100
extern SelectEntry LL1_SELECT[MAX_SELECT_ENTRIES];
extern int NUM_SELECT_ENTRIES;

void calcular_select();

// Predictive Matrix: LL1_MATRIX[NT_name][Terminal_name] -> Regla*
typedef struct {
    const char* no_terminal;
    const char* terminal;
    Regla* regla;
} MatrixEntry;

#define MAX_MATRIX_ENTRIES 500
extern MatrixEntry LL1_MATRIX[MAX_MATRIX_ENTRIES];
extern int NUM_MATRIX_ENTRIES;

Regla* buscar_en_matriz(const char* nt, const char* t);

#endif // LL1_H
