#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_VAROOM,
    TOKEN_PRINPLUP,
    TOKEN_INCINEROAR,
    TOKEN_ELEKID,
    TOKEN_WAILMER,
    TOKEN_FORRETRESS,
    TOKEN_BRAIXEN,
    TOKEN_TINGLU,
    TOKEN_FLOATZEL,
    TOKEN_INDEEDEE,
    TOKEN_NUMEL,
    TOKEN_QUAQUAVAL,
    TOKEN_SEEL,
    TOKEN_GREEDENT,
    TOKEN_LEDIAN,
    TOKEN_PLUSLE,
    TOKEN_MINUN,
    TOKEN_PORYGON,
    TOKEN_DITTO,
    TOKEN_ZACIAN,
    TOKEN_ZAMAZENTA,
    TOKEN_GARDEVOIR,
    TOKEN_GALLADE,
    TOKEN_PANCHAM,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_EPSILON, // Used for grammar
    TOKEN_UNKNOWN
} TipoToken;

const char* tipo_token_str(TipoToken tipo);

typedef struct {
    TipoToken tipo;
    char* valor;
    int linea;
    int columna;
} Token;

typedef struct {
    int linea;
    int columna;
    char* contenido;
    char* motivo;
} ErrorLexico;

#endif // TOKENS_H
