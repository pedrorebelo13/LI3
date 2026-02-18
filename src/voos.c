#include <stdlib.h>
#include <stdio.h>
#include "voos.h"
#include "voo.h"
#include <glib-2.0/glib.h>


struct catalogVoo {
    GHashTable *voos;
    GPtrArray *voos_array;
};

CatalogVoo *initialize_voos_catalog() {
    CatalogVoo *catalog = (CatalogVoo *)malloc(sizeof(CatalogVoo));
    catalog->voos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_voo);
    catalog->voos_array = g_ptr_array_new();
    return catalog;
}

void free_voos_catalog(CatalogVoo *catalog) {
    // Libertar hash table primeiro (isso libera os Voos)
    g_hash_table_destroy(catalog->voos);
    // Depois libertar o array (só a estrutura e buffer, sem liberar elementos)
    g_ptr_array_free(catalog->voos_array, TRUE);
    free(catalog);
}

void insert_voo(CatalogVoo *catalog, Voo *voo) {
    g_hash_table_insert(catalog->voos, get_voo_id(voo), voo);
    g_ptr_array_add(catalog->voos_array, voo);
}


GPtrArray *get_voos_array(CatalogVoo *catalog) {
    return catalog->voos_array;
}

Voo *catalog_get_voo_by_identifier(CatalogVoo *catalog, char *identifier) {
    return (Voo *)g_hash_table_lookup(catalog->voos, identifier);
}