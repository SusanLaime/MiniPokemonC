#include "../include/visualizer.h"
#include <stdio.h>
#include <stdlib.h>

void graficar_first_follow() {
    FILE* f = fopen("images/tablas/first_follow.dot", "w");
    if (!f) return;
    fprintf(f, "digraph G {\n");
    fprintf(f, "  node [shape=plaintext];\n");
    fprintf(f, "  struct1 [label=<\n");
    fprintf(f, "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n");
    fprintf(f, "      <TR><TD BGCOLOR=\"lightblue\"><B>No Terminal</B></TD><TD BGCOLOR=\"lightblue\"><B>FIRST</B></TD><TD BGCOLOR=\"lightblue\"><B>FOLLOW</B></TD></TR>\n");
    
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        fprintf(f, "      <TR><TD>%s</TD><TD>", NON_TERMINALS[i]);
        for(int j=0; j<LL1_FIRST[i].conjunto.num_elementos; j++) {
            fprintf(f, "%s ", LL1_FIRST[i].conjunto.elementos[j]);
        }
        fprintf(f, "</TD><TD>");
        for(int j=0; j<LL1_FOLLOW[i].conjunto.num_elementos; j++) {
            fprintf(f, "%s ", LL1_FOLLOW[i].conjunto.elementos[j]);
        }
        fprintf(f, "</TD></TR>\n");
    }
    
    fprintf(f, "    </TABLE>\n");
    fprintf(f, "  >];\n");
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng images/tablas/first_follow.dot -o images/tablas/first_follow.png");
}

void graficar_select() {
    FILE* f = fopen("images/tablas/select.dot", "w");
    if (!f) return;
    fprintf(f, "digraph G {\n");
    fprintf(f, "  node [shape=plaintext];\n");
    fprintf(f, "  struct1 [label=<\n");
    fprintf(f, "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n");
    fprintf(f, "      <TR><TD BGCOLOR=\"lightblue\"><B>Producción</B></TD><TD BGCOLOR=\"lightblue\"><B>SELECT</B></TD></TR>\n");
    
    for (int i = 0; i < NUM_SELECT_ENTRIES; i++) {
        fprintf(f, "      <TR><TD>%s -&gt; ", LL1_SELECT[i].no_terminal);
        // Find rule
        for (int p=0; p<NUM_GRAMMAR_RULES; p++) {
            if(strcmp(GRAMMAR[p].no_terminal, LL1_SELECT[i].no_terminal) == 0) {
                Regla* r = &GRAMMAR[p].reglas[LL1_SELECT[i].regla_idx];
                for(int s=0; s<r->num_simbolos; s++) {
                    fprintf(f, "%s ", r->simbolos[s].simbolo);
                }
                break;
            }
        }
        fprintf(f, "</TD><TD>");
        for(int j=0; j<LL1_SELECT[i].conjunto.num_elementos; j++) {
            fprintf(f, "%s ", LL1_SELECT[i].conjunto.elementos[j]);
        }
        fprintf(f, "</TD></TR>\n");
    }
    
    fprintf(f, "    </TABLE>\n");
    fprintf(f, "  >];\n");
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng images/tablas/select.dot -o images/tablas/select.png");
}

void graficar_matriz_predictiva() {
    FILE* f = fopen("images/tablas/matriz_predictiva.dot", "w");
    if (!f) return;
    fprintf(f, "digraph G {\n");
    fprintf(f, "  node [shape=plaintext];\n");
    fprintf(f, "  struct1 [label=<\n");
    fprintf(f, "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n");
    
    fprintf(f, "      <TR><TD BGCOLOR=\"lightblue\"><B>M[NT, T]</B></TD>");
    for (int i = 0; i < NUM_TERMINALS; i++) {
        fprintf(f, "<TD BGCOLOR=\"lightblue\"><B>%s</B></TD>", TERMINALS[i]);
    }
    fprintf(f, "<TD BGCOLOR=\"lightblue\"><B>$</B></TD></TR>\n");
    
    for (int i = 0; i < NUM_NON_TERMINALS; i++) {
        fprintf(f, "      <TR><TD><B>%s</B></TD>", NON_TERMINALS[i]);
        for (int j = 0; j < NUM_TERMINALS + 1; j++) {
            const char* term = (j == NUM_TERMINALS) ? "$" : TERMINALS[j];
            Regla* r = buscar_en_matriz(NON_TERMINALS[i], term);
            if (r) {
                fprintf(f, "<TD>");
                for (int s = 0; s < r->num_simbolos; s++) {
                    fprintf(f, "%s ", r->simbolos[s].simbolo);
                }
                fprintf(f, "</TD>");
            } else {
                fprintf(f, "<TD></TD>"); // Empty
            }
        }
        fprintf(f, "</TR>\n");
    }
    
    fprintf(f, "    </TABLE>\n");
    fprintf(f, "  >];\n");
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng images/tablas/matriz_predictiva.dot -o images/tablas/matriz_predictiva.png");
}

static void escape_html(const char* src, char* dest) {
    while (*src) {
        if (*src == '<') {
            strcat(dest, "&lt;");
            dest += 4;
        } else if (*src == '>') {
            strcat(dest, "&gt;");
            dest += 4;
        } else if (*src == '&') {
            strcat(dest, "&amp;");
            dest += 5;
        } else {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

void graficar_traza(const Traza* traza, int num_sentencia, const char* texto_original) {
    if (traza->num_pasos == 0) return;
    
    char dot_file[256], png_file[256];
    sprintf(dot_file, "images/tablas/traza_%d.dot", num_sentencia);
    sprintf(png_file, "images/tablas/traza_%d.png", num_sentencia);
    
    FILE* f = fopen(dot_file, "w");
    if (!f) return;
    
    char texto_escapado[1024] = "";
    if (texto_original) {
        escape_html(texto_original, texto_escapado);
    }

    fprintf(f, "digraph G {\n");
    fprintf(f, "  node [shape=plaintext];\n");
    fprintf(f, "  struct1 [label=<\n");
    fprintf(f, "    <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n");
    fprintf(f, "      <TR><TD BGCOLOR=\"lightgray\" COLSPAN=\"4\"><B>Traza LL(1) - Sentencia %d: %s</B></TD></TR>\n", num_sentencia, texto_escapado);
    fprintf(f, "      <TR><TD BGCOLOR=\"#1F618D\"><FONT COLOR=\"white\"><B>Paso</B></FONT></TD>");
    fprintf(f, "<TD BGCOLOR=\"#1F618D\"><FONT COLOR=\"white\"><B>Pila</B></FONT></TD>");
    fprintf(f, "<TD BGCOLOR=\"#1F618D\"><FONT COLOR=\"white\"><B>Entrada</B></FONT></TD>");
    fprintf(f, "<TD BGCOLOR=\"#1F618D\"><FONT COLOR=\"white\"><B>Acción</B></FONT></TD></TR>\n");
    
    for (int i = 0; i < traza->num_pasos; i++) {
        const char* bgcolor = (i % 2 == 0) ? "#EBF5FB" : "#D6EAF8";
        fprintf(f, "      <TR><TD BGCOLOR=\"%s\">%d</TD>", bgcolor, traza->pasos[i].paso);
        fprintf(f, "<TD BGCOLOR=\"%s\">%s</TD>", bgcolor, traza->pasos[i].pila_str);
        fprintf(f, "<TD BGCOLOR=\"%s\">%s</TD>", bgcolor, traza->pasos[i].entrada_str);
        fprintf(f, "<TD BGCOLOR=\"%s\">%s</TD></TR>\n", bgcolor, traza->pasos[i].accion);
    }
    
    fprintf(f, "    </TABLE>\n");
    fprintf(f, "  >];\n");
    fprintf(f, "}\n");
    fclose(f);
    
    char cmd[512];
    sprintf(cmd, "dot -Tpng %s -o %s", dot_file, png_file);
    system(cmd);
}
