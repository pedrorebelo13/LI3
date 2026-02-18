#include <stdlib.h>
#include <stdio.h>
#include "aeronaves.h"
#include "aeronave.h"
#include "validacao.h"
#include "main.h"
#include "mainUtils.h"


struct aeronave 
{
    char* identifier;
    char* manufacturer;
    char* model;
    int year;
    int capacity;
    int range;
};

int create_and_store_aeronave(char **aeronave_info, void *aeronaves_catalog, Program *program) {

    if (!valida_aeronave(aeronave_info, 6, program)) {
        return 1;
    }
    
    Aeronave* aeronave = (Aeronave*)malloc(sizeof(Aeronave)); 

    set_aeronave_identifier(aeronave, aeronave_info[0]);
    set_aeronave_manufacturer(aeronave, aeronave_info[1]);
    set_aeronave_model(aeronave, aeronave_info[2]);
    set_aeronave_year(aeronave, aeronave_info[3]);
    set_aeronave_capacity(aeronave, aeronave_info[4]);
    set_aeronave_range(aeronave, aeronave_info[5]);
    

    insert_aeronave((CatalogAeronave*)aeronaves_catalog, aeronave);
    return 0;
}


void free_aeronave(void *aeronave_void) {
    Aeronave* aeronave = (Aeronave*)aeronave_void;
    free(aeronave->identifier);
    free(aeronave->manufacturer);
    free(aeronave->model);
    free(aeronave);
}


//identifier
char* get_aeronave_identifier(Aeronave* aeronave) { 
    return strdup(aeronave->identifier);
}                   

void set_aeronave_identifier(Aeronave* aeronave, char* identifier) {
    aeronave->identifier = strdup(identifier);
}



//manufacturer
char* get_aeronave_manufacturer(Aeronave* aeronave) {
    return strdup(aeronave->manufacturer);
}

void set_aeronave_manufacturer(Aeronave* aeronave, char* manufacturer) {
    aeronave->manufacturer = strdup(manufacturer);
}



//model
char* get_aeronave_model(Aeronave* aeronave) {
    return strdup(aeronave->model);                 
}

void set_aeronave_model(Aeronave* aeronave, char* model) {
    aeronave->model = strdup(model);
}




//year
int get_aeronave_year(Aeronave* aeronave) {
    return aeronave->year;
}   

void set_aeronave_year(Aeronave* aeronave, char* year) {
    aeronave->year = atoi(year);
}



//capacity
int get_aeronave_capacity(Aeronave* aeronave) {
    return aeronave->capacity;
}

void set_aeronave_capacity(Aeronave* aeronave, char* capacity) {
    aeronave->capacity = atoi(capacity);
}




//range
int get_aeronave_range(Aeronave* aeronave) {
    return aeronave->range;
}

void set_aeronave_range(Aeronave* aeronave, char* range) {
    aeronave->range = atoi(range);
}
