#include <stdlib.h>
#include <stdio.h>
#include "reservas.h"
#include "reserva.h"
#include "validacao.h"
#include "main.h"
#include "mainUtils.h"


struct reserva 
{
    char* rid;
    char* fids;
    char* pdocNumber;
    char* seat;
    double price;
    int extraLuggage;  ;
    int priorityBoarding;
    char* qrCode;
};

int create_and_store_reserva(char **reserva_info, void *reservas_catalog, Program *program) {

    if (!valida_reserva(reserva_info, 8, program)) {
        return 1;
    }

    Reserva* reserva = (Reserva*)malloc(sizeof(Reserva));  

    set_reserva_rid(reserva, reserva_info[0]);
    set_reserva_fids(reserva, reserva_info[1]);
    set_reserva_pdocNumber(reserva, reserva_info[2]);
    //set_reserva_seat(reserva, reserva_info[3]);
    set_reserva_price(reserva, reserva_info[4]);
    set_reserva_extraLuggage(reserva, reserva_info[5]);
    set_reserva_priorityBoarding(reserva, reserva_info[6]);
    //set_reserva_qrCode(reserva, reserva_info[7]);

    insert_reserva((CatalogReserva*)reservas_catalog, reserva);
    return 0;
}

void free_reserva(void *reserva_void) {
    Reserva* reserva = (Reserva*)reserva_void;
    free(reserva->rid);
    free(reserva->fids);
    free(reserva->pdocNumber);
    //free(reserva->seat);
    //free(reserva->qrCode);
    free(reserva);
}


//rid
char* get_reserva_rid(Reserva* reserva) {
    return strdup(reserva->rid);
}

void set_reserva_rid(Reserva* reserva, char* rid) {
    reserva->rid = strdup(rid);
}



//fids
char* get_reserva_fids(Reserva* reserva) {
    return strdup(reserva->fids);
}

void set_reserva_fids(Reserva* reserva, char* fids) {
    reserva->fids = strdup(fids);
}



//pdocNumber
char* get_reserva_pdocNumber(Reserva* reserva) {
    return strdup(reserva->pdocNumber);
}

void set_reserva_pdocNumber(Reserva* reserva, char* pdocNumber) {
    reserva->pdocNumber = strdup(pdocNumber);
}


//seat
char* get_reserva_seat(Reserva* reserva) {
    return strdup(reserva->seat);
}

void set_reserva_seat(Reserva* reserva, char* seat) {
    reserva->seat = strdup(seat);
}



//price
double get_reserva_price(Reserva* reserva) {
    return reserva->price;
}

void set_reserva_price(Reserva* reserva, char* price) {
    reserva->price = atof(price);
}



//extraLuggage
int get_reserva_extraLuggage(Reserva* reserva) {
    return reserva->extraLuggage;
}

void set_reserva_extraLuggage(Reserva* reserva, char* extraLuggage) {
     if (extraLuggage[0] == 'T')
        reserva->extraLuggage = 1;
    else
        reserva->extraLuggage = 0;
}



//priorityBoarding
int get_reserva_priorityBoarding(Reserva* reserva) {
    return reserva->priorityBoarding;
}

void set_reserva_priorityBoarding(Reserva* reserva, char* priorityBoarding) {
     if (priorityBoarding[0] == 'T')
        reserva->priorityBoarding = 1;
    else
        reserva->priorityBoarding = 0;
}



//qrCode
char* get_reserva_qrCode(Reserva* reserva) {
    return strdup(reserva->qrCode);
}

void set_reserva_qrCode(Reserva* reserva, char* qrCode) {
    if (qrCode == NULL) reserva->qrCode = strdup("QRCODE");
    else {
    reserva->qrCode = strdup(qrCode);
    }
}


