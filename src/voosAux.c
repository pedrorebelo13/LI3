#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "voosAux.h"
#include "utils.h"
#include "mainUtils.h"
#include "voos.h"
#include "voo.h"

//_______________________________________query5_________________________________________
struct airlineAux {
    char* code;
    double average_delay;
    int flight_delayed_count;
};

char* get_airlineAux_code(AirlineAux* airline) { 
    return airline->code;
}
void set_airlineAux_code(AirlineAux* airline, char* code) {
    airline->code = strdup(code);
}

double get_airlineAux_average_delay(AirlineAux* airline) {
    return airline->average_delay;
}
void set_airlineAux_average_delay(AirlineAux* airline, double average_delay) {
    airline->average_delay = average_delay;
}

int get_airlineAux_flight_delayed_count(AirlineAux* airline) {
    return airline->flight_delayed_count;
}
void set_airlineAux_flight_delayed_count(AirlineAux* airline, int count) {
    airline->flight_delayed_count = count;
}

void free_airlineAux(gpointer data) {
    AirlineAux *a = (AirlineAux *)data;
    if (!a) return;
    if (a->code) free(a->code);
    free(a);
}

void query5_aux(Program *program, GHashTable *auxHash) {
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    GPtrArray *voos_array = get_voos_array(voos_catalog);
    // Percorrer todos os voos
    for (guint i = 0; i < voos_array->len; i++) {
        Voo *voo = g_ptr_array_index(voos_array, i);
        char *airline_code = get_voo_airline(voo);
        if (get_voo_status(voo) == DELAYED) {
            Data descolar = get_voo_descolar(voo);
            Data descolou = get_voo_descolou(voo);
                double delay = descolou.dia * 24 * 60 + descolou.hora * 60 + descolou.minuto - (descolar.dia * 24 * 60 + descolar.hora * 60 + descolar.minuto); 
            // Atualizar contadores no auxHash para a airline
            AirlineAux *airlineAux = g_hash_table_lookup(auxHash, airline_code);
            if (!airlineAux) {
                airlineAux = (AirlineAux *)malloc(sizeof(AirlineAux));
                set_airlineAux_code(airlineAux, airline_code);
                set_airlineAux_average_delay(airlineAux, delay);
                set_airlineAux_flight_delayed_count(airlineAux, 1);
                char *key = strdup(airline_code);
                g_hash_table_insert(auxHash, key, airlineAux);
            } else {
                double current_avg = get_airlineAux_average_delay(airlineAux);
                int current_count = get_airlineAux_flight_delayed_count(airlineAux);
                // Atualiza a média de atraso
                double new_avg = (current_avg * (double)current_count + delay) / (double)(current_count + 1);
                set_airlineAux_average_delay(airlineAux, new_avg);
                airlineAux->flight_delayed_count = current_count + 1;
            }
        }
        free(airline_code);
    }
    casasDecimais(auxHash, 3);
}

GPtrArray* sort_airlines(GHashTable *auxHash) {
    if (!auxHash) return NULL;
    // Criar arrays para o resultado e temporário
    GPtrArray *result = g_ptr_array_new_with_free_func(NULL); 
    // Copiar referências da hash para o array temporário
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, auxHash);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        if (value) g_ptr_array_add(result, value);
    }
    // Ordenar array por average_delay
    g_ptr_array_sort(result, compare_airlines_by_average_delay);
    return result;
}

int compare_airlines_by_average_delay(gconstpointer a, gconstpointer b) {
    const AirlineAux *aa = *(const AirlineAux **)a;  // Note o duplo ponteiro
    const AirlineAux *ab = *(const AirlineAux **)b;  // Note o duplo ponteiro
    // Verificar ponteiros
    if (!aa || !ab) {
        if (!aa && !ab) return 0;
        return (!aa) ? 1 : -1;
    }
    // Comparar por average_delay (ordem decrescente: maior primeiro)
    if (aa->average_delay > ab->average_delay) {
        return -1;
    } else if (aa->average_delay < ab->average_delay) {
        return 1;
    }
    // Em caso de empate, comparar por code
    if (!aa->code && !ab->code) return 0;
    if (!aa->code) return 1;
    if (!ab->code) return -1;
    return strcmp(aa->code, ab->code);
}