#include "src/include/scanner.h"
#include <stdio.h>
int main() {
    ScannerResult* res = escanear_archivo("../codigoParaRevisar.txt");
    imprimir_tokens(res);
    return 0;
}
