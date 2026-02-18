#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "arpAux.h"
#include "aeroportos.h"
#include "aeroporto.h"
#include "voos.h"
#include "voo.h"
#include "utils.h"
#include "reserva.h"
#include "reservas.h"
#include "mainUtils.h"

struct aeroportoQ1 {
    char* code;
    int arrivalCount;
    int departureCount;
};

AeroportoQ1* create_aeroportoQ1() {
    AeroportoQ1* aeroporto = (AeroportoQ1*)malloc(sizeof(struct aeroportoQ1));
    if (aeroporto != NULL) {
        aeroporto->code = NULL;
        aeroporto->arrivalCount = 0;
        aeroporto->departureCount = 0;
    }
    return aeroporto;
}

char* get_aeroportoQ1_code(AeroportoQ1* aeroporto) {
    return aeroporto->code;
}

void set_aeroportoQ1_code(AeroportoQ1* aeroporto, char* code) {
    aeroporto->code = strdup(code);
}

int get_aeroportoQ1_arrivalCount(AeroportoQ1* aeroporto) {
    return aeroporto->arrivalCount;
}

int get_aeroportoQ1_departureCount(AeroportoQ1* aeroporto) {
    return aeroporto->departureCount;
}

void set_aeroportoQ1_arrivalCount(AeroportoQ1* aeroporto, int arrivalCount) {
    aeroporto->arrivalCount = arrivalCount;
}

void set_aeroportoQ1_departureCount(AeroportoQ1* aeroporto, int departureCount) {
    aeroporto->departureCount = departureCount;
}

void free_aeroportoQ1(gpointer data) {
    AeroportoQ1* aeroporto = (AeroportoQ1*)data;
    free(aeroporto->code);
    free(aeroporto);
}   

void query1_aux(Program *program, GHashTable *auxHash){
    CatalogReserva *reservas_catalog = get_reservas_catalog(program);
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    GPtrArray *reservas_array =  get_reservas_array(reservas_catalog);

    // Percorrer todas as reservas
    for (guint i = 0; i < reservas_array->len; i++) {
        Reserva *reserva = g_ptr_array_index(reservas_array, i);
        char * voo_ids = get_reserva_fids(reserva);
        size_t voo_ids_len = strlen(voo_ids);

        if (voo_ids_len == 11 || voo_ids_len == 12) {
            char flight_id[9];
            if (voo_ids_len == 11) {
                strncpy(flight_id, voo_ids + 2, 7);
                flight_id[7] = '\0';
            } else {
                strncpy(flight_id, voo_ids + 2, 8);
                flight_id[8] = '\0';
            }
            Voo *voo = catalog_get_voo_by_identifier(voos_catalog, flight_id);

            Status status = get_voo_status(voo);
            if (status == CANCELLED) {
                free(voo_ids);
                continue;
            }

            char * origem = get_voo_origem(voo);
            char * destino = get_voo_destino(voo);

            // Atualizar contadores no auxHash para o aeroporto de origem
            AeroportoQ1 *aeroportoOrigemAux = g_hash_table_lookup(auxHash, origem);
            if (!aeroportoOrigemAux) {
                aeroportoOrigemAux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                set_aeroportoQ1_code(aeroportoOrigemAux, origem);
                set_aeroportoQ1_arrivalCount(aeroportoOrigemAux, 0);
                set_aeroportoQ1_departureCount(aeroportoOrigemAux, 1);
                char *key = strdup(origem);
                g_hash_table_insert(auxHash, key, aeroportoOrigemAux);
            } else {
                aeroportoOrigemAux->departureCount++;
            }    
            // Atualizar contadores no auxHash para o aeroporto de destino
            AeroportoQ1 *aeroportoDestinoAux = g_hash_table_lookup(auxHash, destino);
            if (!aeroportoDestinoAux) {
                aeroportoDestinoAux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                set_aeroportoQ1_code(aeroportoDestinoAux, destino);
                set_aeroportoQ1_arrivalCount(aeroportoDestinoAux, 1);
                set_aeroportoQ1_departureCount(aeroportoDestinoAux, 0);
                char *key = strdup(destino);
                g_hash_table_insert(auxHash, key, aeroportoDestinoAux);
            } else {
                aeroportoDestinoAux->arrivalCount++;
            }
            free(voo_ids);
            free(origem);
            free(destino);
        }
        else { // dois voos
            char flight_id1[9];
            char flight_id2[9];
            fids_Aux(voo_ids, flight_id1, flight_id2);

            Voo *voo1 = catalog_get_voo_by_identifier(voos_catalog, flight_id1);
            Voo *voo2 = catalog_get_voo_by_identifier(voos_catalog, flight_id2);
            Status status1 = get_voo_status(voo1);
            Status status2 = get_voo_status(voo2);
            if (status1 == CANCELLED && status2 == CANCELLED) {
                free(voo_ids);
                continue;
            }
            if (status1 != CANCELLED) {
                char * origem1 = get_voo_origem(voo1);
                char * destino1 = get_voo_destino(voo1);
                char * destino2 = get_voo_destino(voo2);

                // Atualizar contadores no auxHash para o aeroporto de origem do primeiro voo
                AeroportoQ1 *aeroportoOrigem1Aux = g_hash_table_lookup(auxHash, origem1);
                if (!aeroportoOrigem1Aux) {
                    aeroportoOrigem1Aux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                    set_aeroportoQ1_code(aeroportoOrigem1Aux, origem1);
                    set_aeroportoQ1_arrivalCount(aeroportoOrigem1Aux, 0);
                    set_aeroportoQ1_departureCount(aeroportoOrigem1Aux, 1);
                    char *key = strdup(origem1);
                    g_hash_table_insert(auxHash, key, aeroportoOrigem1Aux);
                } else {
                    aeroportoOrigem1Aux->departureCount++;
                }
                // Atualizar contadores no auxHash para o aeroporto de destino do primeiro voo
                AeroportoQ1 *aeroportoDestino1Aux = g_hash_table_lookup(auxHash, destino1);
                if (!aeroportoDestino1Aux) {
                    aeroportoDestino1Aux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                    set_aeroportoQ1_code(aeroportoDestino1Aux, destino1);
                    set_aeroportoQ1_arrivalCount(aeroportoDestino1Aux, 1);
                    set_aeroportoQ1_departureCount(aeroportoDestino1Aux, 1);
                    char *key = strdup(destino1);
                    g_hash_table_insert(auxHash, key, aeroportoDestino1Aux);
                } else {
                    aeroportoDestino1Aux->arrivalCount++;
                    if (status2 != CANCELLED){
                        aeroportoDestino1Aux->departureCount++;
                    }
                }
                if (status2 != CANCELLED) {
                    // Atualizar contadores no auxHash para o aeroporto de destino do segundo voo
                    AeroportoQ1 *aeroportoDestino2Aux = g_hash_table_lookup(auxHash, destino2);
                    if (!aeroportoDestino2Aux) {
                        aeroportoDestino2Aux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                        set_aeroportoQ1_code(aeroportoDestino2Aux, destino2);
                        set_aeroportoQ1_arrivalCount(aeroportoDestino2Aux, 1);
                        set_aeroportoQ1_departureCount(aeroportoDestino2Aux, 0);
                        char *key = strdup(destino2);
                        g_hash_table_insert(auxHash, key, aeroportoDestino2Aux);
                    } else {
                        aeroportoDestino2Aux->arrivalCount++;
                    }
                }
                if (origem1) free(origem1);
                if (destino1) free(destino1);
                if (destino2) free(destino2);
                free(voo_ids);
            } else if (status2 != CANCELLED) {
                // Caso: primeiro voo cancelado, segundo voo ativo -> contar apenas o segundo voo
                char * origem2 = get_voo_origem(voo2);
                char * destino2 = get_voo_destino(voo2);

                AeroportoQ1 *aeroportoOrigem2Aux = g_hash_table_lookup(auxHash, origem2);
                if (!aeroportoOrigem2Aux) {
                    aeroportoOrigem2Aux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                    set_aeroportoQ1_code(aeroportoOrigem2Aux, origem2);
                    set_aeroportoQ1_arrivalCount(aeroportoOrigem2Aux, 0);
                    set_aeroportoQ1_departureCount(aeroportoOrigem2Aux, 1);
                    char *key = strdup(origem2);
                    g_hash_table_insert(auxHash, key, aeroportoOrigem2Aux);
                } else {
                    aeroportoOrigem2Aux->departureCount++;
                }
                
                AeroportoQ1 *aeroportoDestino2Aux = g_hash_table_lookup(auxHash, destino2);
                if (!aeroportoDestino2Aux) {
                    aeroportoDestino2Aux = (AeroportoQ1 *)malloc(sizeof(AeroportoQ1));
                    set_aeroportoQ1_code(aeroportoDestino2Aux, destino2);
                    set_aeroportoQ1_arrivalCount(aeroportoDestino2Aux, 1);
                    set_aeroportoQ1_departureCount(aeroportoDestino2Aux, 0);
                    char *key = strdup(destino2);
                    g_hash_table_insert(auxHash, key, aeroportoDestino2Aux);
                } else {
                    aeroportoDestino2Aux->arrivalCount++;
                }
                
                if (origem2) free(origem2);
                if (destino2) free(destino2);
                free(voo_ids);
            }
        }
    }
}  

//_______________________________________query3_________________________________________
struct aeroportoAux {
    char* code;
    int departureCount;
};

char* get_aeroportoAux_code(AeroportoAux* aeroporto) { 
    return aeroporto->code;
}                   

void set_aeroportoAux_code(AeroportoAux* aeroporto, char* code) {
    aeroporto->code = strdup(code);
}

int get_aeroportoAux_departureCount(AeroportoAux* aeroporto) {
    return aeroporto->departureCount;
}

void set_aeroportoAux_departureCount(AeroportoAux* aeroporto, int departureCount) {
    aeroporto->departureCount = departureCount;
}

void get_aeroporto_mais_voos(CatalogAeroporto *aeroportos_catalog, CatalogVoo *voos_catalog, Data data1, Data data2, AeroportoAux **top_aeroporto) {
    GHashTable *auxHashAeroportos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroportoAux);
    GPtrArray *voos_array = get_voos_array(voos_catalog);

    for (guint i = 0; i < voos_array->len; i++) {
        Voo *voo = g_ptr_array_index(voos_array, i);
        if (get_voo_status(voo) == CANCELLED) continue;
        Data voo_data = get_voo_descolou(voo);

        if (voo_data.ano < data1.ano || voo_data.ano > data2.ano) {
            continue;
        } else {
            if (voo_data.ano == data1.ano) {
                if (voo_data.mes < data1.mes) continue;
                if (voo_data.mes == data1.mes && voo_data.dia < data1.dia) continue;
            }
            if (voo_data.ano == data2.ano) {
                if (voo_data.mes > data2.mes) continue;
                if (voo_data.mes == data2.mes && voo_data.dia > data2.dia) continue;
            }
        }

        char *code = get_voo_origem(voo);


        AeroportoAux *aeroportoAux = g_hash_table_lookup(auxHashAeroportos, code);
        if (!aeroportoAux) {
            aeroportoAux = (AeroportoAux *)malloc(sizeof(AeroportoAux));
            set_aeroportoAux_code(aeroportoAux, code);
            set_aeroportoAux_departureCount(aeroportoAux, 1);

            char *chave = strdup(code);
            g_hash_table_insert(auxHashAeroportos, chave, aeroportoAux);
        } else {
            aeroportoAux->departureCount++;
        }

        free(code);
    }

    // Encontrar top aeroporto
    GHashTableIter iter;
    gpointer key, value;
    char *top_code = NULL;
    int max_count = 0;

    g_hash_table_iter_init(&iter, auxHashAeroportos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *code = (char *)key;
        AeroportoAux *aeroporto = (AeroportoAux *)value;
        int contador = get_aeroportoAux_departureCount(aeroporto);

        if (contador > max_count || (contador == max_count && top_code != NULL && strcmp(code, top_code) < 0)) {
            max_count = contador;
            top_code = code;
        }
    }

    if (top_code != NULL) {
        AeroportoAux *found = g_hash_table_lookup(auxHashAeroportos, top_code);
        if (found) {
            AeroportoAux *result = (AeroportoAux *)malloc(sizeof(AeroportoAux));
            result->code = strdup(get_aeroportoAux_code(found));
            result->departureCount = get_aeroportoAux_departureCount(found);
            *top_aeroporto = result;
        } else {
            *top_aeroporto = NULL;
        }
    }
    g_hash_table_destroy(auxHashAeroportos);
}

void free_aeroportoAux(gpointer data) {
    AeroportoAux *a = (AeroportoAux *)data;
    if (!a) return;
    if (a->code) free(a->code);
    free(a);
}

//Query6

AeroportoQ1* get_top_aeroporto_from_nationality(GHashTable* nationalitysHash, char* nationality) {
    Nationalitys* nationalitys = g_hash_table_lookup(nationalitysHash, nationality);
    if (!nationalitys) return NULL;
    GHashTable* aeroportos_hash = get_nationalitys_aeroportos(nationalitys);
    GHashTableIter iter;
    gpointer key, value;
    char* top_code = NULL;
    int max_count = 0;
    g_hash_table_iter_init(&iter, aeroportos_hash);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char* code = (char*)key;
        AeroportoQ1* aeroporto = (AeroportoQ1*)value;
        int contador = get_aeroportoQ1_arrivalCount(aeroporto);
        if (contador > max_count || (contador == max_count && top_code != NULL && strcmp(code, top_code) < 0)) {
            max_count = contador;
            top_code = code;
        }
    }
    if (top_code == NULL) return NULL;
    return g_hash_table_lookup(aeroportos_hash, top_code);
}