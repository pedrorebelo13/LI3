#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "validacao.h"
#include "voos.h"
#include "aeronaves.h"
#include "passageiros.h"
#include "reservas.h"
#include "aeroportos.h"
#include "main.h"
#include "mainUtils.h"
#include "utils.h"



int valida_aeroporto(char **aeroporto_info, int expected_fields, Program* program) {
    for (int i = 0; i < expected_fields; i++) {
        if (aeroporto_info[i] == NULL || strlen(aeroporto_info[i]) == 0) {

            return 0;
        }
    } 

    //code
    if (strlen(aeroporto_info[0]) != 3) return 0;
    
    for(int i=0; i<3; i++){
        if (aeroporto_info[0][i] < 'A' || aeroporto_info[0][i] > 'Z') return 0;
    }

    // Verificar se code já existe (duplicado)
    CatalogAeroporto *aeroportos_catalog = get_aeroportos_catalog(program);
    if (catalog_get_aeroporto_by_code(aeroportos_catalog, aeroporto_info[0]) != NULL) {
        return 0;
    }

    //type
    if ( (strcmp(aeroporto_info[7], "small_airport") != 0) &&
         (strcmp(aeroporto_info[7], "medium_airport") != 0) &&
         (strcmp(aeroporto_info[7], "large_airport") != 0) &&
         (strcmp(aeroporto_info[7], "heliport") != 0) &&
         (strcmp(aeroporto_info[7], "seaplane_base") != 0) ) return 0;


    //latitude e longitude
    double lat = atof(aeroporto_info[4]);
    double lon = atof(aeroporto_info[5]);
    if (lat < -90.0 || lat > 90.0) return 0;
    if (lon < -180.0 || lon > 180.0) return 0;
    
    const char *dot = strchr(aeroporto_info[4], '.');
    if (!dot) return 0;
    if (strlen(dot + 1) > 8) return 0;

    const char *dot2 = strchr(aeroporto_info[5], '.');
    if (!dot2) return 0; 
    if (strlen(dot2 + 1) > 8) return 0;
    
    // validar caracteres lat/lon
    for (int i = 0; aeroporto_info[4][i]; i++){
        char c = aeroporto_info[4][i];
        if (!((c >= '0' && c <= '9') || c == '.' || (c == '-' && i == 0))) return 0;
    }

    for (int i = 0; aeroporto_info[5][i]; i++){
        char c = aeroporto_info[5][i];
        if (!((c >= '0' && c <= '9') || c == '.' || (c == '-' && i == 0))) return 0;
    }

    // ICAO (4 letras)
    if (strlen(aeroporto_info[6]) != 4) return 0;
    for (int i = 0; i < 4; i++) if (!(aeroporto_info[6][i] >= 'A' && aeroporto_info[6][i] <= 'Z')) return 0;

    return 1;

}


int valida_voo(char **voo_info, int expected_fields, Program *program) {
    for (int i = 0; i < expected_fields; i++) {
        if (voo_info[i] == NULL || voo_info[i][0] == '\0') {
            return 0;
        }
    }

    //status
    if (strcmp(voo_info[6], "On Time") != 0 && strcmp(voo_info[6], "Delayed") != 0 && strcmp(voo_info[6], "Cancelled") != 0) {
        return 0;
    }

    //id
    if(strlen(voo_info[0]) != 7 && strlen(voo_info[0]) != 8){
        return 0;
    }

    for(int i=0; i<2; i++){
        if (voo_info[0][i] < 'A' || voo_info[0][i] > 'Z'){
            return 0;
        }
    }

    int size_digits = strlen(voo_info[0]);
    for(int i=2; i<size_digits; i++){
        if (voo_info[0][i] < '0' || voo_info[0][i] > '9'){
            return 0;
        }
    }

    // Verificar se voo já existe (duplicado)
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    if (catalog_get_voo_by_identifier(voos_catalog, voo_info[0]) != NULL) {
        return 0;
    }
    
    //datas
    
    for (int i = 1; i <= 4; i++) {
        if (!validate_date(voo_info[i])) {
            return 0;
        }
    }

    // prevista < actual descolar
    if (strcmp(voo_info[1], "N/A") != 0 && strcmp(voo_info[2], "N/A") != 0) {
        if (strcmp(voo_info[1], voo_info[2]) > 0) return 0;
    }
    // prevista < actual aterrar
    if (strcmp(voo_info[3], "N/A") != 0 && strcmp(voo_info[4], "N/A") != 0) {
        if (strcmp(voo_info[3], voo_info[4]) > 0) return 0;
    }
    // prevista aterrar >= prevista descolar
    if (strcmp(voo_info[1], "N/A") != 0 && strcmp(voo_info[3], "N/A") != 0) {
        if (strcmp(voo_info[3], voo_info[1]) < 0) return 0;
    }
    // actual aterrar >= actual descolar
    if (strcmp(voo_info[2], "N/A") != 0 && strcmp(voo_info[4], "N/A") != 0) {
        if (strcmp(voo_info[4], voo_info[2]) < 0) return 0;
    }

    //origem e destino
    if (strcmp(voo_info[7], voo_info[8])==0) {
        return 0;
    }
        
    // gate deve ser inteiro positivo
    for (const char *p = voo_info[5]; *p; ++p) {
        if (*p < '0' || *p > '9') return 0;
    }

    // Verificar existência da aeronave
    CatalogAeronave *aeronaves_catalog = get_aeronaves_catalog(program);
    if (catalog_get_aeronave_by_identifier(aeronaves_catalog, voo_info[9]) == NULL) return 0;

    // Verificar existência dos aeroportos de origem e destino
    CatalogAeroporto *aeroportos_catalog = get_aeroportos_catalog(program);
    if (catalog_get_aeroporto_by_code(aeroportos_catalog, voo_info[7]) == NULL) return 0;
    if (catalog_get_aeroporto_by_code(aeroportos_catalog, voo_info[8]) == NULL) return 0;


    // Regras para Cancelled / não Cancelled relativamente a N/A
    if (strcmp(voo_info[6], "Cancelled") == 0) {
        if (!(strcmp(voo_info[2], "N/A") == 0 && strcmp(voo_info[4], "N/A") == 0)) {
            return 0;
        }
    }

    return 1;
}


int valida_aeronave(char **aeronave_info, int expected_fields, Program* program) {
    // Campos obrigatórios não vazios
    
    for (int i = 0; i < expected_fields; i++) {
        if (aeronave_info[i] == NULL || aeronave_info[i][0] == '\0') return 0;
    }

    // identifier: letras maiúsculas / dígitos / hífen, sem espaços
    for (const char *p = aeronave_info[0]; *p; ++p) {
        if (*p == ' ') return 0;
        if (!((*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '-')) return 0;
    }

    // Verificar se aeronave já existe (duplicado)
    CatalogAeronave *aeronaves_catalog = get_aeronaves_catalog(program);
    if (catalog_get_aeronave_by_identifier(aeronaves_catalog, aeronave_info[0]) != NULL) {
        return 0;
    }

    // year: 4 dígitos, até 2025
    if (strlen(aeronave_info[3]) != 4) return 0;
    for (int i = 0; i < 4; i++) if (aeronave_info[3][i] < '0' || aeronave_info[3][i] > '9') return 0;
    int year = atoi(aeronave_info[3]);
    if (year > 2025) return 0;

    return 1; 
}

int valida_reserva(char **reserva_info, int expected_fields, Program* program) {
    for (int i = 0; i < expected_fields; i++) {
        if (reserva_info[i] == NULL || reserva_info[i][0] == '\0') {
            return 0; 
        }
    }


    //rid
    if (strlen(reserva_info[0]) != 10) {
        return 0;
    }

    if(reserva_info[0][0] != 'R'){
        return 0;
    }

    for(int i=1; i<10; i++){
        if (reserva_info[0][i] < '0' || reserva_info[0][i] > '9'){
            return 0;
        }
    }

    // Verificar se rid já existe (duplicado)
    CatalogReserva *reservas_catalog = get_reservas_catalog(program);
    if (catalog_get_reserva_by_rid(reservas_catalog, reserva_info[0]) != NULL) {
        return 0;
    }

    //docnumber

    if (strlen(reserva_info[2]) != 9) return 0;
    
    for(int i=0; i<9; i++){
        if (reserva_info[2][i] < '0' || reserva_info[2][i] > '9'){
            return 0;
        }
    }
    
    //passageiro
    CatalogPassageiro *passageiros_catalog = get_passageiros_catalog(program);
    if (catalog_get_passageiro_by_docnumber(passageiros_catalog, reserva_info[2]) == NULL) {
        return 0;
    }
    

    //fids

    if (reserva_info[1][0] != '[' || reserva_info[1][strlen(reserva_info[1])-1] != ']') return 0;

    CatalogVoo *voos_catalog = get_voos_catalog(program);

    if (strlen(reserva_info[1]) < 11 || strlen(reserva_info[1]) > 24) return 0;
    
    if (strlen(reserva_info[1]) != 11 && strlen(reserva_info[1]) != 12 &&
        strlen(reserva_info[1]) != 22 && strlen(reserva_info[1]) != 23 &&
        strlen(reserva_info[1]) != 24) {
        return 0;
    }

    if (strlen(reserva_info[1]) == 11) { // um voo com 7 digitos
        char flight_id[8];
        strncpy(flight_id, reserva_info[1] + 2, 7);
        flight_id[7] = '\0';
    
        if (catalog_get_voo_by_identifier(voos_catalog, flight_id) == NULL) {
            return 0;
        }
    }

    if (strlen(reserva_info[1]) == 12) { //so um voo com 8 digitos
        char flight_id[9];
        strncpy(flight_id, reserva_info[1] + 2, 8);
        flight_id[8] = '\0';
    
        if (catalog_get_voo_by_identifier(voos_catalog, flight_id) == NULL) {
            return 0;
        }
    }

    if (strlen(reserva_info[1]) == 22 || strlen(reserva_info[1]) == 23 || strlen(reserva_info[1]) == 24) {
        char flight_id[9];
        char flight_id2[9];
        
        fids_Aux(reserva_info[1], flight_id, flight_id2);

        if ((catalog_get_voo_by_identifier(voos_catalog, flight_id) == NULL) || 
            (catalog_get_voo_by_identifier(voos_catalog, flight_id2) == NULL)){
            return 0;
        }

        Voo *v1 = catalog_get_voo_by_identifier(voos_catalog, flight_id);
        Voo *v2 = catalog_get_voo_by_identifier(voos_catalog, flight_id2);


        Data v1_descolar = get_voo_descolar(v1);
        Data v2_descolar = get_voo_descolar(v2);
        if (data_cmp(v1_descolar, v2_descolar) < 0) {
            char* destino = get_voo_destino(v1);
            char* origem = get_voo_origem(v2);

            int aux = (strcmp(destino, origem) == 0);
            free(destino);
            free(origem);
            if (!aux) return 0;
        }
        else {
            char* destino = get_voo_destino(v2);
            char* origem = get_voo_origem(v1);

            int aux = (strcmp(destino, origem) == 0);
            free(destino);
            free(origem);
            if (!aux) return 0;
        }
    }
    
    return 1;
}


int valida_passageiro(char **passageiro_info, int expected_fields, Program* program) {
    for (int i = 0; i < expected_fields; i++) {

        if (passageiro_info[i] == NULL || passageiro_info[i][0] == '\0') {
            return 0; 
        }
    }
    
   
    //docnumber
    if (strlen(passageiro_info[0]) != 9) return 0;
    
    for(int i=0; i<9; i++){
        if (passageiro_info[0][i] < '0' || passageiro_info[0][i] > '9'){
            return 0;
        }
    }

    // Verificar se passageiro já existe (duplicado)
    CatalogPassageiro *passageiros_catalog = get_passageiros_catalog(program);
    if (catalog_get_passageiro_by_docnumber(passageiros_catalog, passageiro_info[0]) != NULL) {
        return 0;
    }

    if (strcmp(passageiro_info[5], "M") != 0 && 
        strcmp(passageiro_info[5], "F") != 0 && 
        strcmp(passageiro_info[5], "O") != 0) return 0;

    //dob: aceitar formato apenas com data (YYYY-MM-DD) ou N/A
    if (!validate_date_dob(passageiro_info[3])) {
        return 0;
    }

    //email
    if (!validate_email(passageiro_info[6])) {
        return 0;
    }

    return 1;
}



int validate_date(const char *date){

    if (strcmp(date,"N/A") == 0) return 1;

    if (strlen(date) != 16 || date[4] != '-' || date[7] != '-' || date[10] != ' ' || date[13] != ':') {
        return 0;  
    }
    
    int year = atoi(date);
    int month = atoi(date + 5);
    int day = atoi(date + 8);
    int hour = atoi(date + 11);
    int minute = atoi(date + 14);
    
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    if (year>2025) return 0;
    if (year==2025 && month>9) return 0;
    if (year==2025 && month==9 && day>30) return 0;

    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;

    return 1;
}

// Valida datas apenas com ano-mês-dia: "YYYY-MM-DD
int validate_date_dob(const char *date) {
    if (!date) return 0;
    if (strcmp(date, "N/A") == 0) return 0;
    // formato esperado: 10 chars: YYYY-MM-DD
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') return 0;
    int year = atoi(date);
    int month = atoi(date + 5);
    int day = atoi(date + 8);
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (year > 2025) return 0;
    if (year == 2025 && month > 9) return 0;
    if (year == 2025 && month == 9 && day > 30) return 0;
    return 1;
}

int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0;  

    const char *dot = strchr(at + 1, '.');
    if (!dot || dot == at + 1) return 0; 

    
    if (strlen(dot + 1) < 2 || strlen(dot + 1) > 3) return 0;

    for (const char *p = email; p < at; p++) {
        if (!((*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') || *p == '.')) {
            return 0;
        }
    }

    for (const char *p = at + 1; p < dot; p++) {
        if (!((*p >= 'a' && *p <= 'z'))){
            return 0;
        }
    }

    for (const char *p = dot + 1; *p != '\0'; p++) {
        if (!((*p >= 'a' && *p <= 'z'))){
            return 0;
        }
    }
    
    return 1;
}

// Compara duas Datas : return <0 se a<b, 0 se =, >0 se a>b
int data_cmp(Data a, Data b) {
    if (a.ano != b.ano) return (a.ano - b.ano);
    if (a.mes != b.mes) return (a.mes - b.mes);
    if (a.dia != b.dia) return (a.dia - b.dia);
    if (a.hora != b.hora) return (a.hora - b.hora);
    return 0;
}