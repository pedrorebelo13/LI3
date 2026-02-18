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

#include "arpAux.h"
#include "arnvAux.h"
#include "resvAux.h"
#include "voosAux.h"
#include "pssgAux.h"

#include "main.h"
#include "mainUtils.h"

struct program {
	CatalogAeroporto *aeroportos_catalog;
	CatalogAeronave *aeronaves_catalog;
	CatalogVoo *voos_catalog;
	CatalogPassageiro *passageiros_catalog;
	CatalogReserva *reservas_catalog;
    GHashTable *aeroporto_hash;
	GPtrArray *aeronaves_sorted_array;
    GHashTable *aeronaves_hash;
    GPtrArray *airline_aux_array;
    GHashTable *airline_hash;
    GHashTable *nationalitys_hash;
    GPtrArray *semanas_array;
    GPtrArray *reservasAux_array;
};

Program* initialize_program(char **arguments) {
    Program *program = (Program *)malloc(sizeof(Program));

    // Leitura e inserção dos dados dos aeroportos no sistema
    if (inicializa_aeroportos(program, arguments) != 0) {
        free(program);
        return NULL;
    }

    // Leitura e inserção dos dados das aeronaves no sistema
    if (inicializa_aeronaves(program, arguments, program->aeroportos_catalog) != 0) {
        free(program);
        return NULL;
    }

    // Leitura e inserção dos dados dos voos no sistema
    if (inicializa_voos(program, arguments, program->aeroportos_catalog, program->aeronaves_catalog) != 0) {
        free(program);
        return NULL;
    }

    // Leitura e inserção dos dados das passageiros no sistema
    if (inicializa_passageiros(program, arguments, program->aeroportos_catalog, program->aeronaves_catalog, program->voos_catalog) != 0) {
        free(program);
        return NULL;
    }

    // Leitura e inserção dos dados das reservas no sistema
    if (inicializa_reservas(program, arguments, program->aeroportos_catalog, program->aeronaves_catalog, program->voos_catalog, program->passageiros_catalog) != 0) {
        free(program);
        return NULL;
    }
    
    return program;
}



void run_program(Program *program, char **arguments) {

    // Leitura do ficheiro de queries
    FILE *input_file = fopen(arguments[2], "r");

    if (input_file == NULL)
    {
        perror("Erro ao abrir ficheiro.");
        return;
    }

    char *line_buffer;
    size_t buffer_size = BUFFER_SIZE;
    int read = 0;

    line_buffer = (char *)malloc(buffer_size * sizeof(char));


    //query 1 auxiliar
    GHashTable *auxHashQ1 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroportoQ1);
    query1_aux(program, auxHashQ1);
    program->aeroporto_hash = auxHashQ1;
    
    //querie2 auxiliar
    GHashTable *auxHashQ2 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeronaveAux);
    query2_aux(program, auxHashQ2);
    program->aeronaves_sorted_array = sort_aeronaves(auxHashQ2);
    program->aeronaves_hash = auxHashQ2;

    //querie4 auxiliar
    GPtrArray *reservasAux_array = sort_reservas_by_departure(program);
    GPtrArray *semanas_array = semanasInicia(reservasAux_array);
    reservas_por_semana(semanas_array, reservasAux_array);
    top10_passageiros_por_semana(semanas_array);
    program->semanas_array = semanas_array;
    program->reservasAux_array = reservasAux_array; 
    
    //querie5 auxiliar
    GHashTable *auxHashQ5 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_airlineAux);
    query5_aux(program, auxHashQ5);
    program->airline_aux_array = sort_airlines(auxHashQ5);
    program->airline_hash = auxHashQ5;

    //querie6 auxiliar
    GHashTable *auxHashQ6 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_nationalitys);
    query6_aux(program, auxHashQ6);
    program->nationalitys_hash = auxHashQ6;

    // Leitura do ficheiro de comandos e execução das queries
    int command_number = 0;

    // Para cada comando
    while ((read = getline(&line_buffer, &buffer_size, input_file)) != -1) 
    {
        // Ignora newline
        line_buffer[read - 1] = '\0';

        // Obter número da query e flag 'S' opcional
        char *p = line_buffer;
        char *endptr = NULL;
        int query_number = (int)strtol(p, &endptr, 10);
        int has_S = 0;
        if (endptr && *endptr == 'S') {
            has_S = 1;
            endptr++;
        }
        // Saltar espaços para obter o argumento real
        while (*endptr == ' ') endptr++;

        run_query(program, query_number, endptr, command_number, has_S);

        command_number++;
    }

    free(line_buffer);
    fclose(input_file);
}



void free_program(Program *program) {
    free_aeroportos_catalog(program->aeroportos_catalog);
    free_aeronaves_catalog(program->aeronaves_catalog);
    free_voos_catalog(program->voos_catalog);
    free_passageiros_catalog(program->passageiros_catalog);
    free_reservas_catalog(program->reservas_catalog);
    if (program->aeronaves_sorted_array) g_ptr_array_free(program->aeronaves_sorted_array, TRUE);
    if (program->aeronaves_hash) g_hash_table_destroy(program->aeronaves_hash);
    if (program->aeroporto_hash) g_hash_table_destroy(program->aeroporto_hash);
    if (program->airline_aux_array) g_ptr_array_free(program->airline_aux_array, TRUE);
    if (program->airline_hash) g_hash_table_destroy(program->airline_hash);
    if (program->nationalitys_hash) g_hash_table_destroy(program->nationalitys_hash);
    if (program->semanas_array) g_ptr_array_free(program->semanas_array, TRUE);
    if (program->reservasAux_array) g_ptr_array_free(program->reservasAux_array, TRUE);

    free(program);
}


CatalogAeroporto *get_aeroportos_catalog(Program *program) {
    return program->aeroportos_catalog;
}

CatalogAeronave *get_aeronaves_catalog(Program *program) {
    return program->aeronaves_catalog;
}

CatalogVoo *get_voos_catalog(Program *program) {
    return program->voos_catalog;
}

CatalogPassageiro *get_passageiros_catalog(Program *program) {
    return program->passageiros_catalog;
}

CatalogReserva *get_reservas_catalog(Program *program) {
    return program->reservas_catalog;
}

GHashTable *get_aeroporto_hash(Program *program) {
    return program->aeroporto_hash;
}

GPtrArray *get_aeronaves_sorted_array(Program *program) {
    return program->aeronaves_sorted_array;
}

GPtrArray *get_airline_aux_array(Program *program) {
    return program->airline_aux_array;
}

GHashTable *get_nationalitys_hash(Program *program) {
    return program->nationalitys_hash;
}

GPtrArray *get_semanas_array(Program *program) {
    return program->semanas_array;
}


void set_aeroporto_hash(Program *program, GHashTable *hash) {
    program->aeroporto_hash = hash;
}

void set_aeronaves_sorted_array(Program *program, GPtrArray *array) {
    program->aeronaves_sorted_array = array;
}

void set_airline_aux_array(Program *program, GPtrArray *array) {
    program->airline_aux_array = array;
}

void set_nationalitys_hash(Program *program, GHashTable *hash) {
    program->nationalitys_hash = hash;
}

void set_semanas_array(Program *program, GPtrArray *array) {
    program->semanas_array = array;
}

int inicializa_aeroportos(Program *program, char **arguments){
    char aeroportos_file_path[512];
    snprintf(aeroportos_file_path, sizeof(aeroportos_file_path), "%s/airports.csv", arguments[1]);
    FILE* aeroportos_file = fopen(aeroportos_file_path, "r");

    if (aeroportos_file ==  NULL)
    {
        perror("Erro ao abrir ficheiro.");
        return 1;
    }

    CatalogAeroporto *aeroportos_catalog = initialize_aeroportos_catalog();
    program->aeroportos_catalog = aeroportos_catalog;

    const char *aeroportos_file_name = strrchr(aeroportos_file_path, '/');
    aeroportos_file_name = aeroportos_file_name ? aeroportos_file_name + 1 : aeroportos_file_path;

    parse_csv_and_store_data(aeroportos_file, create_and_store_aeroporto, aeroportos_catalog, aeroportos_file_name, program);
    
    fclose(aeroportos_file);
    return 0;
}


int inicializa_aeronaves(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog){
    char aeronaves_file_path[512];
    snprintf(aeronaves_file_path, sizeof(aeronaves_file_path), "%s/aircrafts.csv", arguments[1]);
    FILE* aeronaves_file = fopen(aeronaves_file_path, "r");

    if (aeronaves_file == NULL) 
    {
        perror("Erro ao abrir ficheiro de aeronaves.");
        free_aeroportos_catalog(aeroportos_catalog);
        return 1;
    }

    CatalogAeronave *aeronaves_catalog = initialize_aeronaves_catalog(); 
    program->aeronaves_catalog = aeronaves_catalog;

    const char *aeronaves_file_name = strrchr(aeronaves_file_path, '/');
    aeronaves_file_name = aeronaves_file_name ? aeronaves_file_name + 1 : aeronaves_file_path;

    parse_csv_and_store_data(aeronaves_file, create_and_store_aeronave, aeronaves_catalog, aeronaves_file_name, program);

    fclose(aeronaves_file);
    return 0;
}



int inicializa_voos(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog){
    char voos_file_path[512];
    snprintf(voos_file_path, sizeof(voos_file_path), "%s/flights.csv", arguments[1]);
    FILE* voos_file = fopen(voos_file_path, "r");

    if (voos_file == NULL) 
    {
        perror("Erro ao abrir ficheiro de voos.");
        free_aeroportos_catalog(aeroportos_catalog);
        free_aeronaves_catalog(aeronaves_catalog);
        return 1;
    }

    CatalogVoo *voos_catalog = initialize_voos_catalog();
    program->voos_catalog = voos_catalog;

    const char *voos_file_name = strrchr(voos_file_path, '/');
    voos_file_name = voos_file_name ? voos_file_name + 1 : voos_file_path;

    parse_csv_and_store_data(voos_file, create_and_store_voo, voos_catalog, voos_file_name, program);

    fclose(voos_file);
    return 0;
}



int inicializa_passageiros(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog, CatalogVoo *voos_catalog){
    char passageiros_file_path[512];
    snprintf(passageiros_file_path, sizeof(passageiros_file_path), "%s/passengers.csv", arguments[1]);
    FILE* passageiros_file = fopen(passageiros_file_path, "r");

    if (passageiros_file == NULL) 
    {
        perror("Erro ao abrir ficheiro de passageiros.");
        free_aeroportos_catalog(aeroportos_catalog);
        free_aeronaves_catalog(aeronaves_catalog);
        free_voos_catalog(voos_catalog);
        return 1;
    }

    CatalogPassageiro *passageiros_catalog = initialize_passageiros_catalog(); 
    program->passageiros_catalog = passageiros_catalog;

    const char *passageiros_file_name = strrchr(passageiros_file_path, '/');
    passageiros_file_name = passageiros_file_name ? passageiros_file_name + 1 : passageiros_file_path;

    parse_csv_and_store_data(passageiros_file, create_and_store_passageiro, passageiros_catalog, passageiros_file_name, program);

    fclose(passageiros_file);
    return 0;
}


int inicializa_reservas(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog, CatalogVoo *voos_catalog, CatalogPassageiro *passageiros_catalog){
    char reservas_file_path[512];
    snprintf(reservas_file_path, sizeof(reservas_file_path), "%s/reservations.csv", arguments[1]);
    FILE* reservas_file = fopen(reservas_file_path, "r");

    if (reservas_file == NULL) 
    {
        perror("Erro ao abrir ficheiro de reservas.");
        free_aeroportos_catalog(aeroportos_catalog);
        free_aeronaves_catalog(aeronaves_catalog);
        free_voos_catalog(voos_catalog);
        free_passageiros_catalog(passageiros_catalog);
        return 1;
    }

    CatalogReserva *reservas_catalog = initialize_reservas_catalog(); 
    program->reservas_catalog = reservas_catalog;

    const char *reservas_file_name = strrchr(reservas_file_path, '/');
    reservas_file_name = reservas_file_name ? reservas_file_name + 1 : reservas_file_path;

    parse_csv_and_store_data(reservas_file, create_and_store_reserva, reservas_catalog, reservas_file_name, program);

    fclose(reservas_file);
    return 0;
}