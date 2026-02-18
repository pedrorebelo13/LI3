#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include "arnvAux.h"
#include "aeronave.h"
#include "voo.h"
#include "aeronaves.h"
#include "voos.h"
#include "mainUtils.h"

struct aeronaveAux {
    char* identifier;
    int fligthCount;
};

char* get_aeronaveAux_identifier(AeronaveAux* aeronave) { 
    return aeronave->identifier;
}                   

void set_aeronaveAux_identifier(AeronaveAux* aeronave, char* identifier) {
    aeronave->identifier = strdup(identifier);
}

int get_aeronaveAux_flightCount(AeronaveAux* aeronave) {
    return aeronave->fligthCount;
}

void set_aeronaveAux_flightCount(AeronaveAux* aeronave, int flightCount) {
    aeronave->fligthCount = flightCount;
}

void free_aeronaveAux(gpointer data) {
    AeronaveAux *a = (AeronaveAux *)data;
    if (!a) return;
    if (a->identifier) free(a->identifier);
    free(a);
}

void fill_aeronaveAux(GHashTable *auxHash, CatalogAeronave *aeronaves_catalog, GPtrArray *filtered_voos) {
    // Percorrer todos os voos
    for (guint i = 0; i < filtered_voos->len; i++) {
        Voo *voo = g_ptr_array_index(filtered_voos, i);
        
        char *aeronave_id = get_voo_aircraft(voo);

        if (get_voo_status(voo) == CANCELLED) continue;

        AeronaveAux *aeronaveaux = g_hash_table_lookup(auxHash, aeronave_id);
        
        if (!aeronaveaux) {
            // Primeira ocorrência desta aeronave
            aeronaveaux = (AeronaveAux *)malloc(sizeof(AeronaveAux));

            set_aeronaveAux_identifier(aeronaveaux, aeronave_id);
            set_aeronaveAux_flightCount(aeronaveaux, 1);

            char *key = strdup(aeronave_id);
            g_hash_table_insert(auxHash, key, aeronaveaux);
        } else {
            // Aeronave já existe
            aeronaveaux->fligthCount++;
        }
        free(aeronave_id);    }
}


int compare_aeronaves_by_flightcount(gconstpointer a, gconstpointer b) {
    const AeronaveAux *aa = *(const AeronaveAux **)a;  // Note o duplo ponteiro
    const AeronaveAux *ab = *(const AeronaveAux **)b;  // Note o duplo ponteiro
    // Verificar ponteiros
    if (!aa || !ab) {
        if (!aa && !ab) return 0;
        return (!aa) ? 1 : -1;
    }
    // Comparar por número de voos (ordem decrescente)
    if (ab->fligthCount != aa->fligthCount) {
        return ab->fligthCount - aa->fligthCount;
    }
    // Em caso de empate, comparar por identifier
    if (!aa->identifier && !ab->identifier) return 0;
    if (!aa->identifier) return 1;
    if (!ab->identifier) return -1;
    return strcmp(aa->identifier, ab->identifier);
}

GPtrArray* sort_aeronaves(GHashTable *auxHash) {
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
    // Ordenar array por flightCount
    g_ptr_array_sort(result, compare_aeronaves_by_flightcount);
    
    return result;
}
    

GPtrArray *get_top_n_aeronaves(GPtrArray *aeronaves_array, int N, char *manufacturer, CatalogAeronave *aeronaves_catalog) {
    if (!aeronaves_array || N <= 0) return NULL;

    GPtrArray *result = g_ptr_array_new_with_free_func(NULL);
    if (manufacturer == NULL || manufacturer[0] == '\0') {
        // Sem filtro de fabricante
        for (guint i = 0; i < (guint)N && i < aeronaves_array->len; i++) {
            g_ptr_array_add(result, g_ptr_array_index(aeronaves_array, i));
        }
        return result;
    }
    else {
        // Com filtro de fabricante
        for (guint i = 0; i < aeronaves_array->len; i++) {
            AeronaveAux *aeronaveAux = g_ptr_array_index(aeronaves_array, i);
            Aeronave *aeronave = catalog_get_aeronave_by_identifier(aeronaves_catalog, get_aeronaveAux_identifier(aeronaveAux));
            char *this_manufacturer = get_aeronave_manufacturer(aeronave);

            if (this_manufacturer && strcmp(this_manufacturer, manufacturer) == 0) {
                g_ptr_array_add(result, aeronaveAux);
                if (result->len >= (guint)N) {
                    free(this_manufacturer);
                    break;
                }
            }
            free(this_manufacturer);
        }
    }

    return result;
   
}

void query2_aux(Program *program, GHashTable *auxHash) {
    CatalogAeronave *aeronaves_catalog = get_aeronaves_catalog(program);
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    GPtrArray *voos_array = get_voos_array(voos_catalog);
    GPtrArray *filtered_voos = g_ptr_array_new();


    for(guint i = 0; i<voos_array->len; i++){
        Voo *voo = g_ptr_array_index(voos_array, i);
        char *aircraft_id = get_voo_aircraft(voo);
        free(aircraft_id);
    
        if (get_voo_status(voo) == CANCELLED) {
            continue;
        }

        g_ptr_array_add(filtered_voos, voo);
    }

    fill_aeronaveAux(auxHash, aeronaves_catalog, filtered_voos);
    g_ptr_array_free(filtered_voos, TRUE);
}
