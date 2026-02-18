#include <stdlib.h>
#include <stdio.h>
#include "aeroportos.h"
#include "aeroporto.h"
#include "validacao.h"
#include "main.h"
#include "mainUtils.h"


struct aeroporto 
{
    char* code;
    char* name;
    char* city;
    char* country;
    float latitude;
    float longitude;;
    char* icao;
    char* type;
};

int create_and_store_aeroporto(char **aeroporto_info, void *aeroportos_catalog, Program *program) {

    if (!valida_aeroporto(aeroporto_info, 8, program)) {
        return 1;
    }

    Aeroporto* aeroporto = (Aeroporto*)malloc(sizeof(Aeroporto));

    set_aeroporto_code(aeroporto, aeroporto_info[0]);
    set_aeroporto_name(aeroporto, aeroporto_info[1]);
    set_aeroporto_city(aeroporto, aeroporto_info[2]);
    set_aeroporto_country(aeroporto, aeroporto_info[3]);
    set_aeroporto_latitude(aeroporto, aeroporto_info[4]);
    set_aeroporto_longitude(aeroporto, aeroporto_info[5]);
    //set_aeroporto_icao(aeroporto, aeroporto_info[6]);
    set_aeroporto_type(aeroporto, aeroporto_info[7]);

    insert_aeroporto((CatalogAeroporto*)aeroportos_catalog, aeroporto);
    return 0;
}

void free_aeroporto(void *aeroporto_void) {
    Aeroporto* aeroporto = (Aeroporto*)aeroporto_void;
    free(aeroporto->code);
    free(aeroporto->name);
    free(aeroporto->city);
    free(aeroporto->country);
    //free(aeroporto->icao);
    free(aeroporto->type);
    free(aeroporto);
}



//code
char* get_aeroporto_code(Aeroporto* aeroporto) {
    return strdup(aeroporto->code);
}

void set_aeroporto_code(Aeroporto* aeroporto, char* code) {
    aeroporto->code = strdup(code);
}



//name
char* get_aeroporto_name(Aeroporto* aeroporto) {
    return strdup(aeroporto->name);
}

void set_aeroporto_name(Aeroporto* aeroporto, char* name) {
    aeroporto->name = strdup(name);
}



//city
char* get_aeroporto_city(Aeroporto* aeroporto) {
    return strdup(aeroporto->city);
}   

void set_aeroporto_city(Aeroporto* aeroporto, char* city) {
    aeroporto->city = strdup(city);
}




//country
char* get_aeroporto_country(Aeroporto* aeroporto) {
    return strdup(aeroporto->country);
}

void set_aeroporto_country(Aeroporto* aeroporto, char* country) {
    aeroporto->country = strdup(country);
}



//latitude
float get_aeroporto_latitude(Aeroporto* aeroporto) {
    return aeroporto->latitude;
}

void set_aeroporto_latitude(Aeroporto* aeroporto, char* latitude) {
    aeroporto->latitude = atof(latitude);
}




//longitude
float get_aeroporto_longitude(Aeroporto* aeroporto) {
    return aeroporto->longitude;
}

void set_aeroporto_longitude(Aeroporto* aeroporto, char* longitude) {
    aeroporto->longitude = atof(longitude);
}



//icao
char* get_aeroporto_icao(Aeroporto* aeroporto) {
    return strdup(aeroporto->icao);
}

void set_aeroporto_icao(Aeroporto* aeroporto, char* icao) {
    aeroporto->icao = strdup(icao);
}



//type
char* get_aeroporto_type(Aeroporto* aeroporto) {
    return strdup(aeroporto->type);
}

void set_aeroporto_type(Aeroporto* aeroporto, char* type) {
    aeroporto->type = strdup(type);
}


