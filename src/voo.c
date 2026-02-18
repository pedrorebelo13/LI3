#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "voos.h"
#include "voo.h"
#include "validacao.h"
#include "main.h"
#include "mainUtils.h"


struct voo 
{
    char* id;
    Data descolar;
    Data descolou;
    Data aterrar;
    Data aterrou;
    int gate;
    Status status;
    char* origem;
    char* destino;
    char* aircraft;
    char* airline;
    char* URL;
};

int create_and_store_voo(char **voo_info, void *voos_catalog, Program *program) {

    if (!valida_voo(voo_info, 12, program)) {
        return 1;
    }

    Voo* voo = (Voo*)malloc(sizeof(Voo));

    set_voo_id(voo, voo_info[0]);
    set_voo_descolar(voo, voo_info[1]);
    set_voo_descolou(voo, voo_info[2]);
    set_voo_aterrar(voo, voo_info[3]);
    set_voo_aterrou(voo, voo_info[4]);
    set_voo_gate(voo, voo_info[5]);
    set_voo_origem(voo, voo_info[7]);
    set_voo_destino(voo, voo_info[8]);
    set_voo_aircraft(voo, voo_info[9]);
    set_voo_airline(voo, voo_info[10]);
    //set_voo_URL(voo, voo_info[11]);
    set_voo_status(voo, voo_info[6]);

    insert_voo((CatalogVoo*)voos_catalog, voo);

    return 0;
}

void free_voo(void *voo_void) {
    Voo* voo = (Voo*)voo_void;
    
    free(voo->id);
    free(voo->origem);
    free(voo->destino);
    free(voo->aircraft);
    free(voo->airline);
    //free(voo->URL);
    free(voo);
}


//id
char* get_voo_id(Voo* voo) {
    return strdup(voo->id);
}

void set_voo_id(Voo* voo, char* id) {
    voo->id = strdup(id);
}


//descolar
Data get_voo_descolar(Voo* voo) {
    return voo->descolar;
}

void set_voo_descolar(Voo* voo, char* descolar) {
    int ano = 0, mes = 0, dia = 0, hora = 0, minuto = 0;
    int n = 0;
    /* Formato esperado: YYYY-MM-DD HH:MM  - aceitar também apenas date */
    if (descolar != NULL) {
        n = sscanf(descolar, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora, &minuto);
        if (n < 3) { /* failed to parse date at minimum */
            ano = mes = dia = hora = minuto = 0;
        } else {
            if (n < 5) { /* no time provided -> set time to 0:00 */
                hora = 0; minuto = 0;
            }
        }
    }
    voo->descolar.ano = ano;
    voo->descolar.mes = mes;
    voo->descolar.dia = dia;
    voo->descolar.hora = hora;
    voo->descolar.minuto = minuto;
}


//descolou
Data get_voo_descolou(Voo* voo) {
    return voo->descolou;
}

void set_voo_descolou(Voo* voo, char* descolou) {
    int ano = 0, mes = 0, dia = 0, hora = 0, minuto = 0;
    int n = 0;
    if (descolou != NULL) {
        n = sscanf(descolou, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora, &minuto);
        if (n < 3) {
            ano = mes = dia = hora = minuto = 0;
        } else {
            if (n < 5) { hora = 0; minuto = 0; }
        }
    }
    voo->descolou.ano = ano;
    voo->descolou.mes = mes;
    voo->descolou.dia = dia;
    voo->descolou.hora = hora;
    voo->descolou.minuto = minuto;
}


//aterrar
Data get_voo_aterrar(Voo* voo) {
    return voo->aterrar;
}

void set_voo_aterrar(Voo* voo, char* aterrar) {
    int ano = 0, mes = 0, dia = 0, hora = 0, minuto = 0;
    int n = 0;
    if (aterrar != NULL) {
        n = sscanf(aterrar, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora, &minuto);
        if (n < 3) {
            ano = mes = dia = hora = minuto = 0;
        } else {
            if (n < 5) { hora = 0; minuto = 0; }
        }
    }
    voo->aterrar.ano = ano;
    voo->aterrar.mes = mes;
    voo->aterrar.dia = dia;
    voo->aterrar.hora = hora;
    voo->aterrar.minuto = minuto;
}


//aterrou
Data get_voo_aterrou(Voo* voo) {
    return voo->aterrou;
}

void set_voo_aterrou(Voo* voo, char* aterrou) {
    int ano = 0, mes = 0, dia = 0, hora = 0, minuto = 0;
    int n = 0;
    if (aterrou != NULL) {
        n = sscanf(aterrou, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora, &minuto);
        if (n < 3) {
            ano = mes = dia = hora = minuto = 0;
        } else {
            if (n < 5) { hora = 0; minuto = 0; }
        }
    }
    voo->aterrou.ano = ano;
    voo->aterrou.mes = mes;
    voo->aterrou.dia = dia;
    voo->aterrou.hora = hora;
    voo->aterrou.minuto = minuto;
}


//gate
int get_voo_gate(Voo* voo) {
    return voo->gate;
}

void set_voo_gate(Voo* voo, char* gate) {
    voo->gate = atoi(gate);
}


//origem
char* get_voo_origem(Voo* voo) {
    if (!voo || !voo->origem) return NULL;
    return strdup(voo->origem);
}

void set_voo_origem(Voo* voo, char* origem) {
    voo->origem = strdup(origem);
}


//destino
char* get_voo_destino(Voo* voo) {
    return strdup(voo->destino);
}


void set_voo_destino(Voo* voo, char* destino) {
    voo->destino = strdup(destino);
}


//aircraft
char* get_voo_aircraft(Voo* voo) {
    return strdup(voo->aircraft);
}

void set_voo_aircraft(Voo* voo, char* aircraft) {
    voo->aircraft = strdup(aircraft);
}


//airline
char* get_voo_airline(Voo* voo) {
    return strdup(voo->airline);
}

void set_voo_airline(Voo* voo, char* airline) {
    voo->airline = strdup(airline);
}


//URL
char* get_voo_URL(Voo* voo) {
    return strdup(voo->URL);
}

void set_voo_URL(Voo* voo, char* URL) {
    voo->URL = strdup(URL);
}

//status
Status get_voo_status(Voo* voo) {
    return voo->status;
}

void set_voo_status(Voo* voo, char* status) {
    // Handle values with or without surrounding quotes
    if (strcmp(status, "On Time") == 0 || (status[0] == '"' && strcmp(status + 1, "On Time\"") == 0)) {
        voo->status = ON_TIME;
    } else if (strcmp(status, "Delayed") == 0 || (status[0] == '"' && strcmp(status + 1, "Delayed\"") == 0)) {
        voo->status = DELAYED;
    } else if (strcmp(status, "Cancelled") == 0 || (status[0] == '"' && strcmp(status + 1, "Cancelled\"") == 0)) {
        voo->status = CANCELLED;
    } else {
        // Fallback: treat unknown status as CANCELLED
        voo->status = CANCELLED;
    }
}

