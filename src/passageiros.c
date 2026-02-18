#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "passageiros.h"
#include "passageiro.h"
#include "mainUtils.h"


struct catalogPassageiro {
    GHashTable *passageiros;
    GPtrArray *passageiros_array;
};


CatalogPassageiro *initialize_passageiros_catalog() {
    CatalogPassageiro *catalog = (CatalogPassageiro *)malloc(sizeof(CatalogPassageiro));
    catalog->passageiros = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_passageiro);
    catalog->passageiros_array = g_ptr_array_new();
    return catalog;
}

void free_passageiros_catalog(CatalogPassageiro *catalog) {
    g_hash_table_destroy(catalog->passageiros);
    g_ptr_array_free(catalog->passageiros_array, TRUE);
    free(catalog);
}

void insert_passageiro(CatalogPassageiro *catalog, Passageiro *passageiro) {
    g_hash_table_insert(catalog->passageiros, get_passageiro_docNumber(passageiro), passageiro);
    g_ptr_array_add(catalog->passageiros_array, passageiro);
}

GPtrArray *get_passageiros_array(CatalogPassageiro *catalog) {
    return catalog->passageiros_array;
}

Passageiro *catalog_get_passageiro_by_docnumber(CatalogPassageiro *catalog, char *docnumber) {
    return (Passageiro *)g_hash_table_lookup(catalog->passageiros, docnumber);
}