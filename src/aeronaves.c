#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "aeronaves.h"
#include "aeronave.h"

struct catalogAeronave {
    GHashTable *aeronaves;
    GPtrArray *aeronaves_array;
};

CatalogAeronave *initialize_aeronaves_catalog() {
    CatalogAeronave *catalog = (CatalogAeronave *)malloc(sizeof(CatalogAeronave));
    catalog->aeronaves = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeronave);
    catalog->aeronaves_array = g_ptr_array_new();
    return catalog;
}

void free_aeronaves_catalog(CatalogAeronave *catalog) {
    g_hash_table_destroy(catalog->aeronaves);
    g_ptr_array_free(catalog->aeronaves_array, TRUE);
    free(catalog);
}

void insert_aeronave(CatalogAeronave *catalog, Aeronave *aeronave) {
    g_hash_table_insert(catalog->aeronaves, get_aeronave_identifier(aeronave), aeronave);
    g_ptr_array_add(catalog->aeronaves_array, aeronave);
}

GPtrArray *get_aeronaves_array(CatalogAeronave *catalog) {
    return catalog->aeronaves_array;
}

Aeronave *catalog_get_aeronave_by_identifier(CatalogAeronave *catalog, char *identifier) {
    return (Aeronave *)g_hash_table_lookup(catalog->aeronaves, identifier);
}
