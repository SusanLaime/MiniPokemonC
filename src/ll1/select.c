#include "../include/ll1.h"
#include <string.h>

SelectEntry LL1_SELECT[MAX_SELECT_ENTRIES];
int NUM_SELECT_ENTRIES = 0;

MatrixEntry LL1_MATRIX[MAX_MATRIX_ENTRIES];
int NUM_MATRIX_ENTRIES = 0;

static void agregar_a_conjunto(ConjuntoString* conjunto, const char* elemento) {
    for (int i = 0; i < conjunto->num_elementos; i++) {
        if (strcmp(conjunto->elementos[i], elemento) == 0) return;
    }
    conjunto->elementos[conjunto->num_elementos++] = elemento;
}

static ConjuntoString* buscar_follow(const char* nt) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(LL1_FOLLOW[i].no_terminal, nt) == 0) return &LL1_FOLLOW[i].conjunto;
    }
    return NULL;
}

void calcular_select() {
    NUM_SELECT_ENTRIES = 0;
    NUM_MATRIX_ENTRIES = 0;

    for (int i = 0; i < NUM_GRAMMAR_RULES; i++) {
        ProduccionesNT* pnt = &GRAMMAR[i];
        for (int j = 0; j < pnt->num_reglas; j++) {
            Regla* r = &pnt->reglas[j];
            
            SelectEntry se;
            se.no_terminal = pnt->no_terminal;
            se.regla_idx = j;
            se.conjunto.num_elementos = 0;
            
            ConjuntoString first_alpha = obtener_first_de_secuencia(r->simbolos, r->num_simbolos);
            int tiene_eps = 0;
            for (int k = 0; k < first_alpha.num_elementos; k++) {
                if (strcmp(first_alpha.elementos[k], "epsilon") == 0) tiene_eps = 1;
                else agregar_a_conjunto(&se.conjunto, first_alpha.elementos[k]);
            }

            if (tiene_eps) {
                ConjuntoString* head_follow = buscar_follow(pnt->no_terminal);
                for (int k = 0; k < head_follow->num_elementos; k++) {
                    agregar_a_conjunto(&se.conjunto, head_follow->elementos[k]);
                }
            }

            LL1_SELECT[NUM_SELECT_ENTRIES++] = se;

            // Llenar matriz predictiva
            for (int k = 0; k < se.conjunto.num_elementos; k++) {
                MatrixEntry me;
                me.no_terminal = pnt->no_terminal;
                me.terminal = se.conjunto.elementos[k];
                me.regla = r;
                LL1_MATRIX[NUM_MATRIX_ENTRIES++] = me;
            }
        }
    }
}

Regla* buscar_en_matriz(const char* nt, const char* t) {
    for (int i = 0; i < NUM_MATRIX_ENTRIES; i++) {
        if (strcmp(LL1_MATRIX[i].no_terminal, nt) == 0 && strcmp(LL1_MATRIX[i].terminal, t) == 0) {
            return LL1_MATRIX[i].regla;
        }
    }
    return NULL;
}
