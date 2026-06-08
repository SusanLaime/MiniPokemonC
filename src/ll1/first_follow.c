#include "../include/ll1.h"
#include <string.h>
#include <stdlib.h>

MapaConjunto LL1_FIRST[MAX_NT];
MapaConjunto LL1_FOLLOW[MAX_NT];

static int agregar_a_conjunto(ConjuntoString* conjunto, const char* elemento) {
    for (int i = 0; i < conjunto->num_elementos; i++) {
        if (strcmp(conjunto->elementos[i], elemento) == 0) return 0; // Already exists
    }
    conjunto->elementos[conjunto->num_elementos++] = elemento;
    return 1; // Added
}

static ConjuntoString* buscar_first(const char* nt) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(LL1_FIRST[i].no_terminal, nt) == 0) return &LL1_FIRST[i].conjunto;
    }
    return NULL;
}

static ConjuntoString* buscar_follow(const char* nt) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(LL1_FOLLOW[i].no_terminal, nt) == 0) return &LL1_FOLLOW[i].conjunto;
    }
    return NULL;
}

ConjuntoString obtener_first_de_secuencia(Simbolo* secuencia, int len) {
    ConjuntoString res;
    res.num_elementos = 0;
    
    if (len == 0) {
        agregar_a_conjunto(&res, "epsilon");
        return res;
    }

    for (int i = 0; i < len; i++) {
        Simbolo sim = secuencia[i];
        if (sim.es_terminal) {
            agregar_a_conjunto(&res, sim.simbolo);
            break;
        } else {
            ConjuntoString* f = buscar_first(sim.simbolo);
            int tiene_eps = 0;
            for (int j = 0; j < f->num_elementos; j++) {
                if (strcmp(f->elementos[j], "epsilon") == 0) tiene_eps = 1;
                else agregar_a_conjunto(&res, f->elementos[j]);
            }
            if (!tiene_eps) break;
            if (i == len - 1) agregar_a_conjunto(&res, "epsilon");
        }
    }
    return res;
}

void calcular_first() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        LL1_FIRST[i].no_terminal = NON_TERMINALS[i];
        LL1_FIRST[i].conjunto.num_elementos = 0;
    }

    int cambios = 1;
    while (cambios) {
        cambios = 0;
        for (int i = 0; i < NUM_GRAMMAR_RULES; i++) {
            ProduccionesNT* pnt = &GRAMMAR[i];
            ConjuntoString* target_first = buscar_first(pnt->no_terminal);
            for (int j = 0; j < pnt->num_reglas; j++) {
                Regla* r = &pnt->reglas[j];
                ConjuntoString seq_first = obtener_first_de_secuencia(r->simbolos, r->num_simbolos);
                for (int k = 0; k < seq_first.num_elementos; k++) {
                    if (agregar_a_conjunto(target_first, seq_first.elementos[k])) {
                        cambios = 1;
                    }
                }
            }
        }
    }
}

void calcular_follow() {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        LL1_FOLLOW[i].no_terminal = NON_TERMINALS[i];
        LL1_FOLLOW[i].conjunto.num_elementos = 0;
    }

    ConjuntoString* start_follow = buscar_follow(START_SYMBOL);
    agregar_a_conjunto(start_follow, "$");

    int cambios = 1;
    while (cambios) {
        cambios = 0;
        for (int i = 0; i < NUM_GRAMMAR_RULES; i++) {
            ProduccionesNT* pnt = &GRAMMAR[i];
            ConjuntoString* head_follow = buscar_follow(pnt->no_terminal);
            
            for (int j = 0; j < pnt->num_reglas; j++) {
                Regla* r = &pnt->reglas[j];
                for (int idx = 0; idx < r->num_simbolos; idx++) {
                    Simbolo sim = r->simbolos[idx];
                    if (sim.es_terminal) continue;

                    ConjuntoString* target_follow = buscar_follow(sim.simbolo);
                    
                    int len_rest = r->num_simbolos - idx - 1;
                    ConjuntoString rest_first = obtener_first_de_secuencia(&r->simbolos[idx + 1], len_rest);
                    
                    int tiene_eps = 0;
                    for (int k = 0; k < rest_first.num_elementos; k++) {
                        if (strcmp(rest_first.elementos[k], "epsilon") == 0) tiene_eps = 1;
                        else {
                            if (agregar_a_conjunto(target_follow, rest_first.elementos[k])) cambios = 1;
                        }
                    }

                    if (tiene_eps || len_rest == 0) {
                        for (int k = 0; k < head_follow->num_elementos; k++) {
                            if (agregar_a_conjunto(target_follow, head_follow->elementos[k])) cambios = 1;
                        }
                    }
                }
            }
        }
    }
}
