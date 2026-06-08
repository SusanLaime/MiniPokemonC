#include "src/include/scanner.h"
#include "src/include/ll1.h"
#include "src/include/parser.h"
#include "src/include/visualizer.h"
#include <stdio.h>
#include <stdlib.h>

void banner(const char* text) {
    printf("\n======================================================================\n");
    printf("  %s\n", text);
    printf("======================================================================\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: ./ejecutar <archivo_fuente>\n");
        return 1;
    }

    banner("MiniPokemonLang — Pipeline Léxico + Sintáctico (C)");

    // Fase 1: Scanner
    printf("\n\n>>> FASE 1: ANÁLISIS LÉXICO\n");
    ScannerResult* result = escanear_archivo(argv[1]);
    
    if (result->num_tokens == 0 && result->num_errores == 0) {
        free(result);
        return 1;
    }

    imprimir_tokens(result);
    
    // Fase 2: Agrupación de Sentencias
    printf("\n\n>>> FASE 2: AGRUPACIÓN DE SENTENCIAS SIMBÓLICAS\n");
    ListaSentencias* sentencias = agrupar_sentencias(result);
    imprimir_sentencias(sentencias);

    printf("\n  Tokens reconocidos  : %d\n", result->num_tokens);
    printf("  Errores léxicos     : %d\n", result->num_errores);

    imprimir_errores(result);

    if (result->num_errores > 0) {
        printf("\n  ✗ El parser NO se ejecutará hasta corregir los errores léxicos.\n");
        banner("FIN — Pipeline detenido por errores léxicos");
        liberar_scanner_result(result);
        free(result);
        free(sentencias);
        return 1;
    }

    printf("\n  ✓ Sin errores léxicos — continuando al parser...\n");

    // Fase 3: Cálculos LL1
    printf("\n\n>>> FASE 3: CÁLCULOS LL(1)  (FIRST, FOLLOW, SELECT)\n");
    calcular_first();
    calcular_follow();
    calcular_select();
    
    graficar_first_follow();
    graficar_select();
    graficar_matriz_predictiva();
    
    printf("  ✓ Tablas LL(1) generadas exitosamente en images/tablas/\n");

    // Fase 4: Parser
    printf("\n\n>>> FASE 4: ANÁLISIS POR SENTENCIA + ÁRBOL SINTÁCTICO\n");
    ParserResult* p_res = parsear_sentencias(sentencias);
    imprimir_resultados_parser(p_res);

    printf("  ✓ Árboles generados exitosamente en images/arboles/\n");

    liberar_scanner_result(result);
    free(result);
    free(sentencias);
    free(p_res);
    return 0;
}
