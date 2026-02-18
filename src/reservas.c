#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include "reservas.h"
#include "reserva.h"

struct catalogReserva {
    GHashTable *reservas;
    GPtrArray *reservas_array;
};

catalogReserva *initialize_reservas_catalog() {
    catalogReserva *catalog = (catalogReserva *)malloc(sizeof(catalogReserva));
    catalog->reservas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_reserva);
    catalog->reservas_array = g_ptr_array_new();
    return catalog;
}

void free_reservas_catalog(catalogReserva *catalog) {
    g_hash_table_destroy(catalog->reservas);
    g_ptr_array_free(catalog->reservas_array, TRUE);
    free(catalog);
}

void insert_reserva(catalogReserva *catalog, Reserva *reserva) {
    g_hash_table_insert(catalog->reservas, get_reserva_rid(reserva), reserva);
    g_ptr_array_add(catalog->reservas_array, reserva);
}

GPtrArray *get_reservas_array(catalogReserva *catalog) {
    return catalog->reservas_array;
}

Reserva *catalog_get_reserva_by_rid(catalogReserva *catalog, const char *rid) {
    if (!catalog || !rid) return NULL;
    return g_hash_table_lookup(catalog->reservas, rid);
}