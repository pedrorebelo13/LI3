#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "pssgAux.h"
#include "utils.h"
#include "passageiro.h"
#include "passageiros.h"
#include "resvAux.h"
#include "mainUtils.h"

struct passageiroAux {
    char* docNumber;
    double priceTotal;
    int counter;
};

char* get_passageiroAux_docNumber(PassageiroAux* passageiro) { 
    return passageiro->docNumber;
}
void set_passageiroAux_docNumber(PassageiroAux* passageiro, char* docNumber) {
    passageiro->docNumber = strdup(docNumber);
}

double get_passageiroAux_priceTotal(PassageiroAux* passageiro) {
    return passageiro->priceTotal;
}
void set_passageiroAux_priceTotal(PassageiroAux* passageiro, double priceTotal) {
    passageiro->priceTotal = priceTotal;
}

int get_passageiroAux_counter(PassageiroAux* passageiro) {
    return passageiro->counter;
}

void set_passageiroAux_counter(PassageiroAux* passageiro, int counter) {
    passageiro->counter = counter;
}

void free_passageiroAux(gpointer data) {
    PassageiroAux *p = (PassageiroAux *)data;
    if (!p) return;
    if (p->docNumber) free(p->docNumber);
    free(p);
}


void top10_passageiros_por_semana(GPtrArray *semanas_array) {
    for (guint j = 0; j < semanas_array->len; j++) {
        Semana *semana = (Semana *)g_ptr_array_index(semanas_array, j);
        GPtrArray *reservas_semana = get_semana_reservas(semana);
        GPtrArray *top10 = get_semana_top10(semana);

        GHashTable *passageirosHash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_passageiroAux);
        for (guint i = 0; i < reservas_semana->len; i++) {
            ReservaAux *reservaAux = g_ptr_array_index(reservas_semana, i);
            char *docNumber = get_reservaAux_docNumber(reservaAux);

            PassageiroAux *passageiroAux = g_hash_table_lookup(passageirosHash, docNumber);
            if (!passageiroAux) {
                passageiroAux = (PassageiroAux *)malloc(sizeof(PassageiroAux));
                set_passageiroAux_docNumber(passageiroAux, docNumber);
                set_passageiroAux_priceTotal(passageiroAux, get_reservaAux_price(reservaAux));
                set_passageiroAux_counter(passageiroAux, 0);

                char *key = strdup(docNumber);
                g_hash_table_insert(passageirosHash, key, passageiroAux);
            } else {
                passageiroAux->priceTotal += get_reservaAux_price(reservaAux);
            }
        }
        // Criar array temporário para ordenar
        GPtrArray *passageiros_array = g_ptr_array_new_with_free_func(NULL);
        GHashTableIter iter;
        gpointer key, value;
        g_hash_table_iter_init(&iter, passageirosHash);
        while (g_hash_table_iter_next(&iter, &key, &value)) {
            if (value) g_ptr_array_add(passageiros_array, value);
        }
        // Ordenar por priceTotal
        g_ptr_array_sort(passageiros_array, compare_passageiros_by_priceTotal);
        // Selecionar top 10 - copiar dados ao
         // inves de usar referências
        for (guint k = 0; k < passageiros_array->len && k < 10; k++) {
            PassageiroAux *original = g_ptr_array_index(passageiros_array, k);
            PassageiroAux *copia = (PassageiroAux *)malloc(sizeof(PassageiroAux));
            set_passageiroAux_docNumber(copia, get_passageiroAux_docNumber(original));
            set_passageiroAux_priceTotal(copia, get_passageiroAux_priceTotal(original));
            set_passageiroAux_counter(copia, 1); 
            g_ptr_array_add(top10, copia);
        }
        g_ptr_array_free(passageiros_array, TRUE);
        g_hash_table_destroy(passageirosHash);
        set_semana_top10(semana, top10);
    }
}

int compare_passageiros_by_priceTotal(gconstpointer a, gconstpointer b) {
    const PassageiroAux *pa = *(const PassageiroAux **)a;  // Note o duplo ponteiro
    const PassageiroAux *pb = *(const PassageiroAux **)b;  // Note o duplo ponteiro
    // Verificar ponteiros
    if (!pa || !pb) {
        if (!pa && !pb) return 0;
        return (!pa) ? 1 : -1;
    }
    // Comparar por priceTotal (ordem decrescente)
    if (pb->priceTotal > pa->priceTotal) return 1;
    if (pb->priceTotal < pa->priceTotal) return -1;
    // Em caso de empate, comparar por docNumber
    if (!pa->docNumber && !pb->docNumber) return 0;
    if (!pa->docNumber) return 1;
    if (!pb->docNumber) return -1;
    return strcmp(pa->docNumber, pb->docNumber);
}

void get_passageiro_q4(GPtrArray *semanas_array, int semana_inicio, int semana_fim, int *result_counter, char **result_docNumber) {
    GHashTable *passageirosHash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_passageiroAux);
    
    // Ajustar índices (semana_number começa em 1, mas array começa em 0)
    int start_idx = semana_inicio - 1;
    int end_idx = semana_fim - 1;
    
    if (start_idx < 0) start_idx = 0;
    if (end_idx >= (int)semanas_array->len) end_idx = (int)semanas_array->len - 1;

    for (int i = start_idx; i <= end_idx; i++) {
        Semana *semana = (Semana *)g_ptr_array_index(semanas_array, i);
        GPtrArray *top10 = get_semana_top10(semana);

        for (guint j = 0; j < top10->len; j++) {
            PassageiroAux *passageiroAux = g_ptr_array_index(top10, j);
            char *docNumber = get_passageiroAux_docNumber(passageiroAux);

            PassageiroAux *existing = g_hash_table_lookup(passageirosHash, docNumber);
            if (!existing) {
                PassageiroAux *new_passageiro = (PassageiroAux *)malloc(sizeof(PassageiroAux));
                set_passageiroAux_docNumber(new_passageiro, docNumber);
                set_passageiroAux_priceTotal(new_passageiro, 0); 
                set_passageiroAux_counter(new_passageiro, 1);
                char *key = strdup(docNumber);
                g_hash_table_insert(passageirosHash, key, new_passageiro);
            } else {
                // Já apareceu numa semana anterior, incrementar contador de semanas
                existing->counter += 1;
            }
        }
    }

    // Encontrar passageiro com maior counter
    GHashTableIter iter;
    gpointer key, value;
    char *top_docNumber = NULL;
    int max_counter = -1;

    g_hash_table_iter_init(&iter, passageirosHash);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *docNumber = (char *)key;
        PassageiroAux *passageiro = (PassageiroAux *)value;
        int counter_val = get_passageiroAux_counter(passageiro);

        // Comparar por counter (decrescente), depois por docNumber (crescente)
        if (counter_val > max_counter || (counter_val == max_counter && top_docNumber != NULL && strcmp(docNumber, top_docNumber) < 0)) {
            max_counter = counter_val;
            top_docNumber = docNumber;
        }
    }
    *result_counter = max_counter;
    *result_docNumber = (top_docNumber != NULL) ? strdup(top_docNumber) : NULL;  
    g_hash_table_destroy(passageirosHash);
}