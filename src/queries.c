#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "main.h"
#include "mainUtils.h"
#include "aeroportos.h"
#include "aeronaves.h"
#include "aeroporto.h"
#include "aeronave.h"
#include "reservas.h"
#include "reserva.h"
#include "passageiros.h"
#include "passageiro.h"
#include "voo.h"
#include "voos.h"
#include "arpAux.h"
#include "arnvAux.h"
#include "resvAux.h"
#include "voosAux.h"
#include "pssgAux.h"
#include "queries.h"
#include <glib.h>

void run_query(Program *program, int query_number, char *arguments, int command_number, int has_S) {
    switch (query_number)
    {
    case 1:
        query_aeroport_by_code(program, arguments, command_number, has_S);
        break;
    case 2:
        query_list_top_n_aeronaves_by_flightcount(program, arguments, command_number, has_S);
        break;
    case 3:
        query_aeroporto_mais_voos_by_data(program, arguments, command_number, has_S);
        break;
    case 4:
        query_4(program, arguments, command_number, has_S);
        break;
    case 5:
        query_5(program, arguments, command_number, has_S);
        break;
    case 6:
        query_6(program, arguments, command_number, has_S);
        break;
    default:
        printf("Query %d não implementada.\n", query_number);
        break;
    }
}

//query 1
void query_aeroport_by_code(Program *program, char *code, int command_number, int has_S) {
    CatalogAeroporto *aeroportos_catalog = get_aeroportos_catalog(program);
    Aeroporto *aeroporto = catalog_get_aeroporto_by_code(aeroportos_catalog, code);
    
    if (aeroporto == NULL)
    {
        save_output(command_number, "");
        return;
    }

    char* name = get_aeroporto_name(aeroporto);
    char* city = get_aeroporto_city(aeroporto);
    char* country = get_aeroporto_country(aeroporto);
    char* type = get_aeroporto_type(aeroporto);

    GHashTable *aeroporto_hash = get_aeroporto_hash(program);
    AeroportoQ1* aeroportoQ1 = g_hash_table_lookup(aeroporto_hash, code);
    int arrivalCount = 0;
    int departureCount = 0;
    if (aeroportoQ1 != NULL) {
        arrivalCount = get_aeroportoQ1_arrivalCount(aeroportoQ1);
        departureCount = get_aeroportoQ1_departureCount(aeroportoQ1);
    }

    if (has_S==0) {
        char *output = g_strdup_printf("%s;%s;%s;%s;%s;%d;%d\n", code, name, city, country, type, arrivalCount, departureCount);

        save_output(command_number, output);

        free(name);
        free(city);
        free(country);
        free(type);
        g_free(output);
        return;
    }
    
    char *output = g_strdup_printf("%s=%s=%s=%s=%s=%d=%d\n", code, name, city, country, type, arrivalCount, departureCount);

    save_output(command_number, output);

    free(name);
    free(city);
    free(country);
    free(type);
    g_free(output);
}



//query 2
void query_list_top_n_aeronaves_by_flightcount(Program *program, char *arguments, int command_number, int has_S) {
    CatalogAeronave *aeronaves_catalog = get_aeronaves_catalog(program);


    char manufacturer[100] = "";
    int n = 0;
    int matched;

    // Tentar ler N e manufacturer
    matched = sscanf(arguments, "%d %99s", &n, manufacturer);
    if (matched != 2) {
        manufacturer[0] = '\0';
        matched = sscanf(arguments, "%d", &n);
        if (matched != 1 || n <= 0) {
            save_output(command_number, "");
            return;
        }
    } else if (n <= 0) {
        save_output(command_number, "");
        return;
    }

    GPtrArray *sorted_aeronaves = get_aeronaves_sorted_array(program);
    GPtrArray *aeronaves_array = NULL;

    if (manufacturer[0] == '\0') {
        aeronaves_array = get_top_n_aeronaves(sorted_aeronaves, n, NULL, NULL);
    } else {
        aeronaves_array = get_top_n_aeronaves(sorted_aeronaves, n, manufacturer, aeronaves_catalog);
    }

    if (aeronaves_array == NULL) {
        save_output(command_number, "");
        return;
    }

    if(has_S==0) {
        char *output = g_strdup("");
        for (guint i = 0; i < aeronaves_array->len; i++) {
            AeronaveAux *aeronaveAux = g_ptr_array_index(aeronaves_array, i);
            Aeronave *aeronave = catalog_get_aeronave_by_identifier(aeronaves_catalog, get_aeronaveAux_identifier(aeronaveAux));
            char *identifier = get_aeronave_identifier(aeronave);
            char *manufacturer_str = get_aeronave_manufacturer(aeronave);
            char *model = get_aeronave_model(aeronave);
            char *new_line = g_strdup_printf("%s;%s;%s;%d\n", 
                identifier, 
                manufacturer_str, 
                model, 
                get_aeronaveAux_flightCount(aeronaveAux));
            free(identifier);
            free(manufacturer_str);
            free(model);
            char *old_output = output;
            output = g_strconcat(output, new_line, NULL);
            g_free(old_output);
            g_free(new_line);
        }

        save_output(command_number, output);
        g_free(output);
        
        
        g_ptr_array_free(aeronaves_array, TRUE);
        return;
    }

    char *output = g_strdup("");
    for (guint i = 0; i < aeronaves_array->len; i++) {
        AeronaveAux *aeronaveAux = g_ptr_array_index(aeronaves_array, i);
        Aeronave *aeronave = catalog_get_aeronave_by_identifier(aeronaves_catalog, get_aeronaveAux_identifier(aeronaveAux));
        char *identifier = get_aeronave_identifier(aeronave);
        char *manufacturer_str = get_aeronave_manufacturer(aeronave);
        char *model = get_aeronave_model(aeronave);
        char *new_line = g_strdup_printf("%s=%s=%s=%d\n", 
            identifier, 
            manufacturer_str, 
            model, 
            get_aeronaveAux_flightCount(aeronaveAux));
        free(identifier);
        free(manufacturer_str);
        free(model);
        char *old_output = output;
        output = g_strconcat(output, new_line, NULL);
        g_free(old_output);
        g_free(new_line);
    }

    save_output(command_number, output);
    g_free(output);
    
    
    g_ptr_array_free(aeronaves_array, TRUE);
}



//query 3
void query_aeroporto_mais_voos_by_data(Program *program, char *arguments, int command_number, int has_S) {
    CatalogAeroporto *aeroportos_catalog = get_aeroportos_catalog(program);
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    
    Data data1 = {0};
    
    Data data2 = {0};

    argumento_to_data(arguments, &data1, &data2);

    AeroportoAux *aeroportoAux_mais_voos = NULL;

    get_aeroporto_mais_voos(aeroportos_catalog, voos_catalog, data1, data2, &aeroportoAux_mais_voos);

    if (aeroportoAux_mais_voos == NULL) {
        save_output(command_number, "");
        return;
    }

    char* aeroportoAux_code = get_aeroportoAux_code(aeroportoAux_mais_voos);
    int departure_count = get_aeroportoAux_departureCount(aeroportoAux_mais_voos);

    Aeroporto *aeroporto_mais_voos = catalog_get_aeroporto_by_code(aeroportos_catalog, aeroportoAux_code);
    if (aeroporto_mais_voos == NULL) {
        free_aeroportoAux(aeroportoAux_mais_voos);
        save_output(command_number, "");
        return;
    }

    char* aeroporto_code = get_aeroporto_code(aeroporto_mais_voos);
    char* aeroporto_name = get_aeroporto_name(aeroporto_mais_voos);
    char* aeroporto_city = get_aeroporto_city(aeroporto_mais_voos);
    char* aeroporto_country = get_aeroporto_country(aeroporto_mais_voos);

    if(has_S==0) {
        char *output = g_strdup_printf("%s;%s;%s;%s;%d\n", aeroporto_code, aeroporto_name, aeroporto_city, aeroporto_country, departure_count);

        free(aeroporto_code);
        free(aeroporto_name);
        free(aeroporto_city);
        free(aeroporto_country);

        save_output(command_number, output);
        g_free(output);
        
        free_aeroportoAux(aeroportoAux_mais_voos);
        return;
    }
    char *output = g_strdup_printf("%s=%s=%s=%s=%d\n", aeroporto_code, aeroporto_name, aeroporto_city, aeroporto_country, departure_count);

    free(aeroporto_code);
    free(aeroporto_name);
    free(aeroporto_city);
    free(aeroporto_country);

    save_output(command_number, output);
    g_free(output);
    
    free_aeroportoAux(aeroportoAux_mais_voos);
}



//query 4
void query_4(Program *program, char *arguments, int command_number, int has_S) {
    CatalogPassageiro *passageiros_catalog = get_passageiros_catalog(program);
    
    Data data1 = {0};
    Data data2 = {0};
    int semana_inicio = 0;
    int semana_fim = 0;
    GPtrArray *semanas_array = get_semanas_array(program);

    if (arguments != NULL && strlen(arguments) > 0) {
        argumento_to_data(arguments, &data1, &data2);
        define_semanas(semanas_array, data1, data2, &semana_inicio, &semana_fim);
    } else{
        semana_inicio = 1;
        semana_fim = (semanas_array->len > 0) ? semanas_array->len : 1;
    }
    
    int counter = 0;
    char *passageiro_resultado = NULL;
    get_passageiro_q4(semanas_array, semana_inicio, semana_fim, &counter, &passageiro_resultado);
    
    if (passageiro_resultado == NULL || counter < 0) {
        save_output(command_number, "");
        return;
    }
    
    Passageiro *passageiro = catalog_get_passageiro_by_docnumber(passageiros_catalog, passageiro_resultado);
    
    char* first_name = get_passageiro_firstName(passageiro);
    char* last_name = get_passageiro_lastName(passageiro);
    Data dob = get_passageiro_dob(passageiro);
    char* dob_str = g_strdup_printf("%04d-%02d-%02d", dob.ano, dob.mes, dob.dia);
    char* nationality = get_passageiro_nationality(passageiro);
    
    if (has_S == 0) {
        char *output = g_strdup_printf("%s;%s;%s;%s;%s;%d\n", passageiro_resultado, first_name, last_name, dob_str, nationality, counter);
        free(passageiro_resultado);
        free(first_name);
        free(last_name);
        free(dob_str);
        free(nationality);
        save_output(command_number, output);
        g_free(output);
        return;
    }
    
    char *output = g_strdup_printf("%s=%s=%s=%s=%s=%d\n", passageiro_resultado, first_name, last_name, dob_str, nationality, counter);
    free(passageiro_resultado);
    free(first_name);
    free(last_name);
    free(dob_str);
    free(nationality);
    save_output(command_number, output);
    g_free(output);
}



//query 5
void query_5(Program *program, char *arguments, int command_number, int has_S) {
    int n = 0;
    int matched = sscanf(arguments, "%d", &n);
    if (matched != 1 || n <= 0) {
        save_output(command_number, "");
        return;
    }

    GPtrArray *airline_aux_array = get_airline_aux_array(program);
    if (airline_aux_array == NULL || airline_aux_array->len == 0) {
        save_output(command_number, "");
        return;
    }

    if (has_S == 0) {
        char *output = g_strdup("");
        for (guint i = 0; i < (guint)n && i < airline_aux_array->len; i++) {
            AirlineAux *airlineAux = g_ptr_array_index(airline_aux_array, i);
            char *code = get_airlineAux_code(airlineAux);
            int flight_delayed_count = get_airlineAux_flight_delayed_count(airlineAux);
            double average_delay = get_airlineAux_average_delay(airlineAux);
            char *new_line = g_strdup_printf("%s;%d;%.3f\n", code, flight_delayed_count, average_delay);
            char *old_output = output;
            output = g_strconcat(output, new_line, NULL);
            g_free(old_output);
            g_free(new_line);
        }
        save_output(command_number, output);
        g_free(output);
        return;
    }
    char *output = g_strdup("");
    for (guint i = 0; i < (guint)n && i < airline_aux_array->len; i++) {
        AirlineAux *airlineAux = g_ptr_array_index(airline_aux_array, i);
        char *code = get_airlineAux_code(airlineAux);
        int flight_delayed_count = get_airlineAux_flight_delayed_count(airlineAux);
        double average_delay = get_airlineAux_average_delay(airlineAux);
    char *new_line = g_strdup_printf("%s=%d=%.3f\n", code, flight_delayed_count, average_delay);
        char *old_output = output;
        output = g_strconcat(output, new_line, NULL);
        g_free(old_output);
        g_free(new_line);
    }
    save_output(command_number, output);
    g_free(output);
    return;
}



//query 6
void query_6(Program *program, char *nationality, int command_number, int has_S) {
    GHashTable *nationalitys_hash = get_nationalitys_hash(program);
    Nationalitys *nat = g_hash_table_lookup(nationalitys_hash, nationality);
    if (nat == NULL) {
        save_output(command_number, "");
        return;
    }

    AeroportoQ1 *top_aeroporto = get_top_aeroporto_from_nationality(nationalitys_hash, nationality);
    if (!top_aeroporto) {
        save_output(command_number, "");
        return;
    }

    char *code = get_aeroportoQ1_code(top_aeroporto);
    int arrivalCount = get_aeroportoQ1_arrivalCount(top_aeroporto);
    if (has_S == 0) {
        char *output = g_strdup_printf("%s;%d\n", code, arrivalCount);
        save_output(command_number, output);
        g_free(output);
    } else {
        char *output = g_strdup_printf("%s=%d\n", code, arrivalCount);
        save_output(command_number, output);
        g_free(output);
    }
    return;
}



void save_output(int command_number, char *output){
    char *output_file = g_strdup_printf(OUTPUT_NAME "/" OUTPUT_FILE_NAME, command_number + 1);
    FILE *file = fopen(output_file, "w");

    if (file == NULL)
    {
        perror("Erro ao abrir ficheiro.");
        g_free(output_file);
        return;
    }

    if (output[0] == '\0')
    {
        fprintf(file, "\n");
    } else {
        fprintf(file, "%s", output);
    }

    fclose(file);

    g_free(output_file);
}