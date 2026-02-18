#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include "parser.h"
#include "queries.h"
#include "validacao.h"
#include "utils.h"

#include "aeroportos.h"
#include "aeronaves.h"
#include "voos.h"
#include "passageiros.h"
#include "reservas.h"
#include "mainUtils.h"
#include "main.h"
#include "arpAux.h"
#include "arnvAux.h"
#include "pssgAux.h"
#include "resvAux.h"
#include "voosAux.h"

static void trim_newline(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

static void init_aux_structures(Program *program) {
    //q1
    GHashTable *auxHashQ1 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroportoQ1);
    query1_aux(program, auxHashQ1);
    set_aeroporto_hash(program, auxHashQ1);

    //q2
    GHashTable *auxHashQ2 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeronaveAux);
    query2_aux(program, auxHashQ2);
    GPtrArray *sorted2 = sort_aeronaves(auxHashQ2);
    set_aeronaves_sorted_array(program, sorted2);

    //q4
    GPtrArray *reservasAux_array = sort_reservas_by_departure(program);
    GPtrArray *auxHashQ4 = semanasInicia(reservasAux_array);
    reservas_por_semana(auxHashQ4, reservasAux_array);
    top10_passageiros_por_semana(auxHashQ4);
    set_semanas_array(program, auxHashQ4);
 
    //q5
    GHashTable *auxHashQ5 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_airlineAux);
    query5_aux(program, auxHashQ5);
    GPtrArray *sorted5 = sort_airlines(auxHashQ5);
    set_airline_aux_array(program, sorted5);

    //q6
    GHashTable *auxHashQ6 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_nationalitys);
    query6_aux(program, auxHashQ6);
    set_nationalitys_hash(program, auxHashQ6);
}

static void print_last_output_file(int command_number, int query_number) {
    char *output_file = g_strdup_printf(OUTPUT_NAME "/" OUTPUT_FILE_NAME, command_number);
    FILE *f = fopen(output_file, "r");
    if (!f) {
        g_free(output_file);
        printf("Output: (sem conteúdo)\n");
        return;
    }
    char buffer[4096];
    size_t n = fread(buffer, 1, sizeof(buffer) - 1, f);
    buffer[n] = '\0';
    fclose(f);

    if (n == 0 || (n == 1 && buffer[0] == '\n')) {
       switch(query_number) {
            case 1: printf("Output: Q1: Aeroporto nao encontrado\n"); break;
            case 2: printf("Output: Q2: Nenhuma aeronave encontrada\n"); break;
            case 3: printf("Output: Q3: Nenhum aeroporto com partidas nesse período\n"); break;
            case 4: printf("Output: Q4: Nenhum passageiro viagou nesse período\n"); break;
            case 5: printf("Output: Q5: Nenhuma companhia com atraso médio\n"); break;
            case 6: printf("Output: Q6: Nacionalidade não encontrada entre os passageiros existentes.\n"); break;
            default: printf("Output: (sem conteúdo)\n"); break;
       }
    } else {
        printf("Output: %s", buffer);
    }
    
    g_free(output_file);
}

int main(void)
{
    // Criar pasta de output caso não exista
    g_mkdir_with_parents(OUTPUT_NAME, 0700);

    char dataset_path[512];

    while (1) {
        printf("Introduza o caminho dos ficheiros de dados: ");
        if (!fgets(dataset_path, sizeof(dataset_path), stdin)) {
            fprintf(stderr, "Erro a ler input.\n");
            return 1;
        }
        trim_newline(dataset_path);
        if (dataset_path[0] == '\0') {
            printf("Caminho vazio. Tente novamente.\n");
            continue;
        }

        // Preparar argumentos para initialize_program (usa arguments[1])
        char *arguments[3] = { (char *)"programa-interativo", dataset_path, NULL };
        Program *program = initialize_program(arguments);
        if (program == NULL) {
            printf("Falha a carregar dataset. Verifique o caminho e ficheiros.\n");
            continue; 
        }

        printf("Dataset carregado...\n");

        // Construir estruturas auxiliares usadas pelas queries
        init_aux_structures(program);

        int command_number = 1; // para nomes dos ficheiros de output
        int change_dataset = 0; // flag para distinguir entre sair e mudar dataset

        while (1) {
            printf("\nQue query deseja executar?\n");
            printf("0: Sair\n");
            printf("1: Query 1 - Listar o resumo de um aeroporto\n");
            printf("2: Query 2 - Top N aeronaves com mais voos realizados\n");
            printf("3: Query 3 - Listar o aeroporto com mais partidas entre 2 datas\n");
            printf("4: Query 4 - O passageiro que esteve mais tempo no top 10 de passageiros que mais gastaram em viagens durante um período\n");
            printf("5: Query 5 - Top N companhias aéreas com mais tempo de atraso médio por voo\n");
            printf("6: Query 6 - O aeroporto de destino mais comum para passageiros de uma determinada nacionalidade\n");
            printf("7: Mudar caminho do dataset\n");
            printf("Escolha: ");
            
            char line[256];
            if (!fgets(line, sizeof(line), stdin)) {
                printf("Erro a ler input. A sair...\n");
                break;
            }
            trim_newline(line);
            if (line[0] == '\0') {
                printf("Entrada vazia.\n");
                continue;
            }

            char *endptr = NULL;
            int qnum = (int)strtol(line, &endptr, 10);
            if (qnum == 0) {
                printf("A sair...\n");
                change_dataset = 0; // flag para sair completamente
                break;
            }
            if (qnum == 7) {
                // Mudar dataset
                printf("A descarregar dataset atual...\n");
                change_dataset = 1;
                break;
            }
            if (qnum < 1 || qnum > 6) {
                printf("Query inválida. Escolha entre 1 e 6.\n");
                continue;
            }

            int has_S = 0;
            while (1) {
                printf("Formato 'S'? (s/N): ");
                char fmt[16];
                if (!fgets(fmt, sizeof(fmt), stdin)) break;
                trim_newline(fmt);
                
                if (strcmp(fmt, "sim") == 0 || strcmp(fmt, "s") == 0 || strcmp(fmt, "S") == 0) {
                    has_S = 1;
                    break;
                } else if (strcmp(fmt, "nao") == 0 || strcmp(fmt, "n") == 0 || strcmp(fmt, "N") == 0) {
                    has_S = 0;
                    break;
                } else {
                    printf("Resposta inválida. Por favor, responda com \"s\" ou \"n\".\n");
                }
            }

            char args[256];
            args[0] = '\0';
            
            switch (qnum) {
                case 1:
                    while (1) {
                        printf("Intruduza o código do aeroporto (ex: 'AIV'): ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] != '\0') break;
                        printf("O código não pode ser vazio.\n");
                    }
                    break;
                case 2:
                    while (1) {
                        printf("Introduza um numero N e opcionalmente fabricante (ex: '10' ou '10 Boeing'): ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] == '\0') {
                            printf("O input não pode ser vazio.\n");
                            continue;
                        }
                        int temp_n;
                        if (sscanf(args, "%d", &temp_n) == 1) break;
                        printf("N tem de ser um inteiro\n");
                    }
                    break;
                case 3:
                     while (1) {
                        printf("Introduza a data inicial e final (YYYY-MM-DD YYYY-MM-DD): ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] == '\0') {
                            printf("O input não pode ser vazio.\n");
                            continue;
                        }
                        int y1, m1, d1, y2, m2, d2;
                        if (sscanf(args, "%d-%d-%d %d-%d-%d", &y1, &m1, &d1, &y2, &m2, &d2) == 6) break;
                        printf("Datas tem de ser validas\n");
                    }
                    break;
                case 4:
                    while (1) {
                        printf("Intruza, opcionalmente, a data inicial e final (YYYY-MM-DD YYYY-MM-DD): ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] == '\0') break;
                        
                        int y1, m1, d1, y2, m2, d2;
                        if (sscanf(args, "%d-%d-%d %d-%d-%d", &y1, &m1, &d1, &y2, &m2, &d2) == 6) break;
                        printf("Datas tem de ser validas\n");
                    }
                    break;
                case 5:
                    while (1) {
                        printf("Introduza um numero N (ex: '10'): ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] == '\0') {
                            printf("O input não pode ser vazio.\n");
                            continue;
                        }
                        int temp_n;
                        if (sscanf(args, "%d", &temp_n) == 1) break;
                        printf("N tem de ser um inteiro\n");
                    }
                    break;
                case 6:
                    while (1) {
                        printf("Introduza uma nacionalidade: ");
                        if (!fgets(args, sizeof(args), stdin)) break;
                        trim_newline(args);
                        if (args[0] != '\0') break;
                        printf("A nacionalidade não pode ser vazia.\n");
                    }
                    break;
                default:
                    break;
            }

            // Executar query e mostrar output
            run_query(program, qnum, args, command_number - 1, has_S);
            print_last_output_file(command_number, qnum);
            command_number++;

            // Perguntar se quer executar outra query
            int should_continue = 0;
            while (1) {
                printf("\nDeseja executar outra query? (s/N): ");
                char continuar[16];
                if (!fgets(continuar, sizeof(continuar), stdin)) {
                    should_continue = 0;
                    break; 
                }
                trim_newline(continuar);
                
                if (strcmp(continuar, "sim") == 0 || strcmp(continuar, "s") == 0 || strcmp(continuar, "S") == 0) {
                    should_continue = 1;
                    break;
                } else if (strcmp(continuar, "nao") == 0 || strcmp(continuar, "n") == 0 || strcmp(continuar, "N") == 0) {
                    should_continue = 0;
                    break;
                } else {
                    printf("Resposta inválida. Por favor, responda com 's' ou 'n'.\n");
                }
            }

            if (!should_continue) {
                printf("A sair...\n");
                break;
            }
        }

        free_program(program);
        printf("Dataset descarregado.\n");
        
        if (change_dataset == 0) {
            // Opção 0: Sair completamente
            break;
        }
        // Opção 7: Mudar dataset, volta ao loop externo para pedir novo dataset
    }

    return 0;
}