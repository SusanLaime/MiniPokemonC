#include "src/include/tokens.h"
#include <string.h>

const char* tipo_token_str(TipoToken tipo) {
    switch (tipo) {
        case TOKEN_VAROOM: return "VAROOM";
        case TOKEN_PRINPLUP: return "PRINPLUP";
        case TOKEN_INCINEROAR: return "INCINEROAR";
        case TOKEN_ELEKID: return "ELEKID";
        case TOKEN_WAILMER: return "WAILMER";
        case TOKEN_FORRETRESS: return "FORRETRESS";
        case TOKEN_BRAIXEN: return "BRAIXEN";
        case TOKEN_TINGLU: return "TINGLU";
        case TOKEN_FLOATZEL: return "FLOATZEL";
        case TOKEN_INDEEDEE: return "INDEEDEE";
        case TOKEN_NUMEL: return "NUMEL";
        case TOKEN_QUAQUAVAL: return "QUAQUAVAL";
        case TOKEN_SEEL: return "SEEL";
        case TOKEN_GREEDENT: return "GREEDENT";
        case TOKEN_LEDIAN: return "LEDIAN";
        case TOKEN_PLUSLE: return "PLUSLE";
        case TOKEN_MINUN: return "MINUN";
        case TOKEN_PORYGON: return "PORYGON";
        case TOKEN_DITTO: return "DITTO";
        case TOKEN_ZACIAN: return "ZACIAN";
        case TOKEN_ZAMAZENTA: return "ZAMAZENTA";
        case TOKEN_GARDEVOIR: return "GARDEVOIR";
        case TOKEN_GALLADE: return "GALLADE";
        case TOKEN_PANCHAM: return "PANCHAM";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_EPSILON: return "epsilon";
        default: return "UNKNOWN";
    }
}
