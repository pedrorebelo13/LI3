#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "resvAux.h"
#include "utils.h"
#include "reserva.h"
#include "reservas.h"
#include "voos.h"
#include "voo.h"
#include "mainUtils.h"

struct reservaAux {
    char* id;
    char* docNumber;
    char* flightIds;
    Data departure;
    double price;
};

char* get_reservaAux_id(ReservaAux* reserva) { 
    return reserva->id;
}

void set_reservaAux_id(ReservaAux* reserva, char* id) {
    reserva->id = strdup(id);
}

char* get_reservaAux_docNumber(ReservaAux* reserva) {
    return reserva->docNumber;
}

void set_reservaAux_docNumber(ReservaAux* reserva, char* docNumber) {
    reserva->docNumber = strdup(docNumber);
}

char* get_reservaAux_flightIds(ReservaAux* reserva) {
    return reserva->flightIds;
}

void set_reservaAux_flightIds(ReservaAux* reserva, char* flightIds) {
    reserva->flightIds = strdup(flightIds);
}

Data get_reservaAux_departure(ReservaAux* reserva) {
    return reserva->departure;
}

void set_reservaAux_departure(ReservaAux* reserva, Data departure) {
    reserva->departure = departure;
}

double get_reservaAux_price(ReservaAux* reserva) {
    return reserva->price;
}

void set_reservaAux_price(ReservaAux* reserva, double price) {
    reserva->price = price;
}

void free_reservaAux(gpointer data) {
    ReservaAux *r = (ReservaAux *)data;
    if (!r) return;
    if (r->id) free(r->id);
    if (r->docNumber) free(r->docNumber);
    if (r->flightIds) free(r->flightIds);
    free(r);
}

GPtrArray *sort_reservas_by_departure(Program *program) {
    CatalogReserva *reservas_catalog = get_reservas_catalog(program);
    GPtrArray *reservas_array = get_reservas_array(reservas_catalog);
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    //novo array de ReservaAux
    GPtrArray *reservaAux_array = g_ptr_array_new_with_free_func(free_reservaAux);
    for (guint i = 0; i < reservas_array->len; i++) {
        Reserva *reserva = g_ptr_array_index(reservas_array, i);
        ReservaAux *reservaAux = (ReservaAux *)malloc(sizeof(ReservaAux));

        char *rid = get_reserva_rid(reserva);
        char *pdocNumber = get_reserva_pdocNumber(reserva);
        char *flightIds = get_reserva_fids(reserva);
        
        set_reservaAux_id(reservaAux, rid);
        set_reservaAux_docNumber(reservaAux, pdocNumber);
        set_reservaAux_flightIds(reservaAux, flightIds);
        
        free(rid);
        free(pdocNumber);
        
        // Inicializar com data inválida por padrão
        Data departure = {0};
        
        if (strlen(flightIds) == 11 || strlen(flightIds) == 22 || (strlen(flightIds) == 23 && flightIds[10] == ',')) {
            char flight_id[8];
            strncpy(flight_id, flightIds + 2, 7);
            flight_id[7] = '\0';
            Voo *voo = catalog_get_voo_by_identifier(voos_catalog, flight_id);
            if (voo) {
                departure = get_voo_descolar(voo);
            }
        } else if (strlen(flightIds) == 12 || strlen(flightIds) == 24 || (strlen(flightIds) == 23 && flightIds[11] == ',')) {
            char flight_id[9];
            strncpy(flight_id, flightIds + 2, 8);
            flight_id[8] = '\0';
            Voo *voo = catalog_get_voo_by_identifier(voos_catalog, flight_id);
            if (voo) {
                departure = get_voo_descolar(voo);
            }
        }
        
        set_reservaAux_departure(reservaAux, departure);
        set_reservaAux_price(reservaAux, get_reserva_price(reserva));

        free(flightIds);
        g_ptr_array_add(reservaAux_array, reservaAux);
    }
    // Ordenar array por departure
    g_ptr_array_sort(reservaAux_array, compare_reservas_by_departure);
    
    return reservaAux_array;
}

int compare_reservas_by_departure(gconstpointer a, gconstpointer b) {
    const ReservaAux *ra = *(const ReservaAux **)a;
    const ReservaAux *rb = *(const ReservaAux **)b;  
    if (!ra || !rb) {
        if (!ra && !rb) return 0;
        return (!ra) ? 1 : -1;
    }
    Data da = get_reservaAux_departure((ReservaAux *)ra);
    Data db = get_reservaAux_departure((ReservaAux *)rb);
    // Comparar por data de partida
    if (da.ano != db.ano) {
        return da.ano - db.ano;
    }
    if (da.mes != db.mes) {
        return da.mes - db.mes;
    }
    if (da.dia != db.dia) {
        return da.dia - db.dia;
    }
    if (da.hora != db.hora) {
        return da.hora - db.hora;
    }
    if (da.minuto != db.minuto) {
        return da.minuto - db.minuto;
    }
    return 0;
}