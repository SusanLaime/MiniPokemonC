# Documentación del Proyecto: MiniPokemon a C

Este proyecto implementa un compilador LL(1) para el lenguaje MiniPokemonLang escrito completamente en lenguaje C. 
Aplica estrictas prácticas de código limpio (modularización en múltiples archivos de cabecera e implementación) y genera visualizaciones gráficas haciendo uso de herramientas externas (`dot` / Graphviz).

## Estructura del Proyecto

```
MiniPokemonC/
├── images/
│   ├── arboles/           (Contendrá los grafos .png y .dot de los CST y AST)
│   └── tablas/            (Contendrá los grafos .png y .dot de FIRST, FOLLOW, SELECT, Matrix y Trazas)
├── obj/                   (Archivos objeto de compilación intermedia)
├── src/
│   ├── include/           (Archivos de cabecera estructurados modularmente)
│   │   ├── arbol.h
│   │   ├── gramatica.h
│   │   ├── ll1.h
│   │   ├── parser.h
│   │   ├── scanner.h
│   │   ├── tokens.h
│   │   └── visualizer.h
│   ├── visualizer/        (Código encargado de generar archivos Graphviz)
│   │   ├── arbolGraph.c
│   │   └── tablasGraph.c
│   ├── ll1/               (Cálculo de componentes LL1 y estructuras base)
│   │   ├── arbol.c
│   │   ├── first_follow.c
│   │   ├── gramatica.c
│   │   └── select.c
│   ├── parser.c           (Analizador sintáctico predictivo)
│   ├── scannerConfig.c    (Agrupador lógico de sentencias)
│   ├── scannerPokemon.c   (Analizador léxico)
│   └── tokens.c           (Mapeo de strings para tokens)
├── codigoParaRevisar.txt  (Archivo fuente principal con las pruebas)
├── ejecutar.c             (Orquestador y punto de entrada)
└── Makefile               (Instrucciones de compilación)
```

## Dependencias
Para compilar: `gcc` (o `make`).
Para visualizar los gráficos generados: `graphviz` (`dot` debe estar instalado en el sistema y accesible desde PATH).

## Compilación y Ejecución
1. Sitúese en el directorio `MiniPokemonC`.
2. Ejecute el comando `make clean && make`. Esto generará el ejecutable `ejecutar`.
3. Inicie el programa pasando como argumento el archivo a revisar: `./ejecutar codigoParaRevisar.txt`
4. Revise las carpetas `images/arboles` e `images/tablas` para ver las imágenes y archivos Graphviz generados automáticamente para las trazas y árboles de cada sentencia.

## Límites del Proyecto y Mejoras Futuras

### Errores Actuales y Limitaciones
* **Gramática Estricta (LL1):** El compilador está estrictamente sujeto a su tabla predictiva y no tolera recursividad por la izquierda, lo cual obliga a utilizar producciones auxiliares (`_rest`) que pueden hacer que el CST se vuelva redundante.
* **Separación de Sentencias:** Actualmente, el escáner asume que cada sentencia estructural completa debe estar separada de la siguiente por al menos **una línea completamente en blanco**. Si el usuario junta varias sentencias en un bloque monolítico sin salto de línea extra, el compilador las tratará como una sola macro-sentencia, lo cual desbordaría el buffer de parsing.
* **Ausencia de Análisis Semántico:** El proyecto actualmente finaliza en la creación del AST (Abstract Syntax Tree). No verifica tipos, ni evalúa resultados, ni previene uso de variables no declaradas.
* **Buffer Limitado:** Se establecen límites estáticos de memoria (100 sentencias, 1000 nodos) utilizando C arrays nativos. Códigos masivos producirán segmentation faults en el estado actual.

### Mejoras Urgentes (Roadmap)
1. **Asignación de Memoria Dinámica:** Reemplazar el buffer estático (`MAX_TOKENS_PER_SENTENCE`, `MAX_PRODUCCIONES_EMITIDAS`) por listas doblemente enlazadas y uso extensivo de `realloc()` para procesar scripts de MiniPokemonLang de longitud arbitraria sin riesgo de stack overflow.
2. **Evaluador (Intérprete):** Agregar un módulo semántico (`eval.c`) que lea el AST validado y comience a computar el valor de las variables para tener un resultado de salida visible.
3. **Manejo de Errores con Pánico:** Implementar el modo pánico en el parser LL(1) para recuperar la compilación después de un error sintáctico, sin truncar bruscamente el análisis en la primera línea defectuosa.

## Reporte Histórico de Errores durante la Implementación

Durante la fase de migración de Python a C, los problemas más recurrentes fueron los siguientes:

1. **Colapso de RAM y Fugas de Memoria (Segfaults):**
   * **Problema:** En C, construir las tablas LL(1) (First, Follow) usando conjuntos anidados resultó en repetidos _segmentation faults_ porque los punteros sobrescribían memoria local del stack.
   * **Solución:** Se diseñó el sistema de estructuras anidadas globales estáticas y se depuró utilizando GDB para aislar los índices de los arreglos. Se limpió el uso de cadenas crudas asignando memorias fijas temporales.

2. **Diferencias Semánticas de Regex (Scanner):**
   * **Problema:** El módulo `re` de Python procesa los saltos de línea y tokens por grupo mucho mejor que `regcomp` nativo de C. Esto llevó a que palabras juntas fuesen mal identificadas, o a que bloques (como `forretress`) se dividieran erróneamente en múltiples oraciones a causa de un `;` interno.
   * **Solución:** Primero se intentó contar anidamientos de paréntesis y llaves, pero la solución final más estable y escalable fue requerir saltos de línea dobles (`token->linea - previous_line > 1`) como delimitadores de sentencias absolutas, asegurando oraciones cohesivas al momento de parsear.

3. **Árbol Abstracto Sintáctico (AST) Primitivo:**
   * **Problema:** La primera traducción solo borraba nodos epsilon, sin elevar los operadores (como `+`, `=`) a raíces reales del árbol (transformación *bottom-up*), haciendo que el diagrama visual del AST fuese ilegible o incorrecto.
   * **Solución:** Se reescribió `convertir_a_ast` desde cero empleando algoritmos explícitos en C para extraer el operador, fijarlo como raíz, y colgar el hijo izquierdo y el hijo derecho de forma manual. 

4. **Corrupción de Tablas Graphviz (HTML):**
   * **Problema:** Al inyectar el código original de C como título de las tablas del parser (por ejemplo `x > y`), el carácter `>` cerraba el XML interno de Graphviz antes de tiempo, corrompiendo por completo los PDFs/PNGs.
   * **Solución:** Se escribió una función personalizada `escape_html()` en C que preprocesa todos los títulos de las sentencias convirtiéndolos en secuencias seguras como `&gt;` y `&lt;`.
