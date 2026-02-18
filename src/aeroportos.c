#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "aeroportos.h"
#include "aeroporto.h"

struct catalogAeroporto {
    GHashTable *aeroportos;
    GPtrArray *aeroportos_array;
};

CatalogAeroporto *initialize_aeroportos_catalog() {
    CatalogAeroporto *catalog = (CatalogAeroporto *)malloc(sizeof(CatalogAeroporto));
    catalog->aeroportos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroporto);
    catalog->aeroportos_array = g_ptr_array_new();
    return catalog;
}

void free_aeroportos_catalog(CatalogAeroporto *catalog) {
    g_hash_table_destroy(catalog->aeroportos);
    g_ptr_array_free(catalog->aeroportos_array, TRUE);
    free(catalog);
}

void insert_aeroporto(CatalogAeroporto *catalog, Aeroporto *aeroporto) {
    g_hash_table_insert(catalog->aeroportos, get_aeroporto_code(aeroporto), aeroporto);
    g_ptr_array_add(catalog->aeroportos_array, aeroporto);
}

GPtrArray *get_aeroportos_array(CatalogAeroporto *catalog) {
    return catalog->aeroportos_array;
}

Aeroporto *catalog_get_aeroporto_by_code(CatalogAeroporto *catalog, char *code) {
    return (Aeroporto *)g_hash_table_lookup(catalog->aeroportos, code);
}