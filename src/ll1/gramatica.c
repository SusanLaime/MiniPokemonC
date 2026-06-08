#include "../include/gramatica.h"
#include <string.h>

const char* START_SYMBOL = "programa";

const char* NON_TERMINALS[] = {
    "programa", "sentencia", "sentencia_rest", "decl_var", "asignacion",
    "impresion", "condicional", "else_opt", "bloque", "sentencias_bloque",
    "condicion", "op_rel", "expr", "expr_rest", "termino", "op_arit",
    "bucle_while", "bucle_for", "sentencia_control"
};
int NUM_NON_TERMINALS = 19;

const char* TERMINALS[] = {
    "VAROOM", "INDEEDEE", "SEEL", "PANCHAM", "PRINPLUP", "ZACIAN", "ZAMAZENTA",
    "INCINEROAR", "ELEKID", "WAILMER", "FORRETRESS", "BRAIXEN", "TINGLU",
    "FLOATZEL", "GARDEVOIR", "GALLADE", "NUMEL", "PLUSLE", "MINUN", "PORYGON",
    "DITTO", "QUAQUAVAL", "GREEDENT", "LEDIAN"
};
int NUM_TERMINALS = 24;

ProduccionesNT GRAMMAR[] = {
    {"programa", {
        { {{"sentencia", 0}, {"sentencia_rest", 0}}, 2 }
    }, 1},
    {"sentencia_rest", {
        { {{"sentencia", 0}, {"sentencia_rest", 0}}, 2 },
        { {{"epsilon", 1}}, 1 }
    }, 2},
    {"sentencia", {
        { {{"decl_var", 0}}, 1 },
        { {{"asignacion", 0}}, 1 },
        { {{"impresion", 0}}, 1 },
        { {{"condicional", 0}}, 1 },
        { {{"bloque", 0}}, 1 },
        { {{"bucle_while", 0}}, 1 },
        { {{"bucle_for", 0}}, 1 },
        { {{"sentencia_control", 0}}, 1 }
    }, 8},
    {"decl_var", {
        { {{"VAROOM", 1}, {"INDEEDEE", 1}, {"SEEL", 1}, {"expr", 0}, {"PANCHAM", 1}}, 5 }
    }, 1},
    {"asignacion", {
        { {{"INDEEDEE", 1}, {"SEEL", 1}, {"expr", 0}, {"PANCHAM", 1}}, 4 }
    }, 1},
    {"impresion", {
        { {{"PRINPLUP", 1}, {"ZACIAN", 1}, {"expr", 0}, {"ZAMAZENTA", 1}, {"PANCHAM", 1}}, 5 }
    }, 1},
    {"condicional", {
        { {{"INCINEROAR", 1}, {"ZACIAN", 1}, {"condicion", 0}, {"ZAMAZENTA", 1}, {"bloque", 0}, {"else_opt", 0}}, 6 }
    }, 1},
    {"else_opt", {
        { {{"ELEKID", 1}, {"bloque", 0}}, 2 },
        { {{"epsilon", 1}}, 1 }
    }, 2},
    {"bloque", {
        { {{"GARDEVOIR", 1}, {"sentencias_bloque", 0}, {"GALLADE", 1}}, 3 }
    }, 1},
    {"sentencias_bloque", {
        { {{"sentencia", 0}, {"sentencias_bloque", 0}}, 2 },
        { {{"epsilon", 1}}, 1 }
    }, 2},
    {"bucle_while", {
        { {{"WAILMER", 1}, {"ZACIAN", 1}, {"condicion", 0}, {"ZAMAZENTA", 1}, {"bloque", 0}}, 5 }
    }, 1},
    {"bucle_for", {
        { {{"FORRETRESS", 1}, {"ZACIAN", 1}, {"VAROOM", 1}, {"INDEEDEE", 1}, {"SEEL", 1}, {"expr", 0}, {"PANCHAM", 1}, {"condicion", 0}, {"PANCHAM", 1}, {"INDEEDEE", 1}, {"SEEL", 1}, {"expr", 0}, {"ZAMAZENTA", 1}, {"bloque", 0}}, 14 }
    }, 1},
    {"sentencia_control", {
        { {{"BRAIXEN", 1}, {"PANCHAM", 1}}, 2 },
        { {{"TINGLU", 1}, {"PANCHAM", 1}}, 2 }
    }, 2},
    {"condicion", {
        { {{"expr", 0}, {"op_rel", 0}, {"expr", 0}}, 3 }
    }, 1},
    {"op_rel", {
        { {{"QUAQUAVAL", 1}}, 1 },
        { {{"GREEDENT", 1}}, 1 },
        { {{"LEDIAN", 1}}, 1 }
    }, 3},
    {"expr", {
        { {{"termino", 0}, {"expr_rest", 0}}, 2 }
    }, 1},
    {"expr_rest", {
        { {{"op_arit", 0}, {"termino", 0}, {"expr_rest", 0}}, 3 },
        { {{"epsilon", 1}}, 1 }
    }, 2},
    {"termino", {
        { {{"NUMEL", 1}}, 1 },
        { {{"INDEEDEE", 1}}, 1 }
    }, 2},
    {"op_arit", {
        { {{"PLUSLE", 1}}, 1 },
        { {{"MINUN", 1}}, 1 },
        { {{"PORYGON", 1}}, 1 },
        { {{"DITTO", 1}}, 1 }
    }, 4}
};
int NUM_GRAMMAR_RULES = 19;

int es_no_terminal(const char* simbolo) {
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        if (strcmp(simbolo, NON_TERMINALS[i]) == 0) return 1;
    }
    return 0;
}

int es_terminal(const char* simbolo) {
    if (strcmp(simbolo, "epsilon") == 0) return 1;
    for (int i = 0; i < NUM_TERMINALS; i++) {
        if (strcmp(simbolo, TERMINALS[i]) == 0) return 1;
    }
    return 0;
}
