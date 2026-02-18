#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "queries.h"
#include <glib.h>
#include "aeronaves.h"
#include "voos.h"
#include "aeronave.h"
#include "voo.h"
#include "aeroportos.h"
#include "aeroporto.h"
#include "reserva.h"
#include "reservas.h"
#include "utils.h"
#include "mainUtils.h"
#include "validacao.h"
#include "arpAux.h"
#include "arnvAux.h"
#include "resvAux.h"
#include "voosAux.h"
#include "pssgAux.h"

//____________________________________________geral__________________________________________________

void fids_Aux(char* fids, char* fid1, char* fid2) {
    // Extrair o(s) flight ID(s) da string fids
    if (strlen(fids) == 11) { // um voo com 7 digitos
        strncpy(fid1, fids + 2, 7);
        fid1[7] = '\0';
        fid2[0] = '\0';
    }

    if (strlen(fids) == 12) { //so um voo com 8 digitos
        strncpy(fid1, fids + 2, 8);
        fid1[8] = '\0';
        fid2[0] = '\0';
    }

    if (strlen(fids) == 22) { // dois voos com 7 digitos
        strncpy(fid1, fids + 2, 7);
        fid1[7] = '\0';
        
        strncpy(fid2, fids + 13, 7);
        fid2[7] = '\0';
    }

    if (strlen(fids) == 23) {  //so um voo com 8 digitos, o outro com 7
        if (fids[11] == ',') {
            strncpy(fid1, fids + 2, 8);
            fid1[8] = '\0';

            strncpy(fid2, fids + 14, 7);
            fid2[7] = '\0'; 
        }
        else if (fids[10] == ',') {
            strncpy(fid1, fids + 2, 7);
            fid1[7] = '\0';

            strncpy(fid2, fids + 13, 8);
            fid2[8] = '\0'; 
        }
    }

    if (strlen(fids) == 24) { // dois voos com 8 digitos
        strncpy(fid1, fids + 2, 8);
        fid1[8] = '\0';
        
        strncpy(fid2, fids + 14, 8);
        fid2[8] = '\0';
    }

}

Data string_to_data(char* data_str, int hora, int minuto) {
    Data data = {0};
    
    if (data_str == NULL) return data;
    
    //YYYY-MM-DD HH:MM
    sscanf(data_str, "%d-%d-%d", &data.ano, &data.mes, &data.dia);
    data.hora = hora;
    data.minuto = minuto;
    
    return data;
}

void argumento_to_data(char *arguments, Data *data1, Data *data2) {
    char date1_str[11]; // YYYY-MM-DD + \0
    char date2_str[11];
    
    // Limpa as strings
    memset(date1_str, 0, sizeof(date1_str));
    memset(date2_str, 0, sizeof(date2_str));
    
    sscanf(arguments, "%10s %10s", date1_str, date2_str);
 
    *data1 = string_to_data(date1_str,0,0);
    *data2 = string_to_data(date2_str,23,59);
}



struct semana {
    int number;
    GPtrArray *top10;
    GPtrArray *reservas;
    Data inicio;
    Data fim;
};

int get_semana_number(Semana* semana) {
    return semana->number;
}

void set_semana_number(Semana* semana, int number) {
    semana->number = number;
}

GPtrArray* get_semana_top10(Semana* semana) {
    return semana->top10;
}

void set_semana_top10(Semana* semana, GPtrArray* top10) {
    semana->top10 = top10;
}

GPtrArray* get_semana_reservas(Semana* semana) {
    return semana->reservas;
}

void set_semana_reservas(Semana* semana, GPtrArray* reservas) {
    semana->reservas = reservas;
}

Data get_semana_inicio(Semana* semana) {
    return semana->inicio;
}

void set_semana_inicio(Semana* semana, Data inicio) {
    semana->inicio = inicio;
}

Data get_semana_fim(Semana* semana) {
    return semana->fim;
}

void set_semana_fim(Semana* semana, Data fim) {
    semana->fim = fim;
}

void free_semana(gpointer data) {
    Semana *s = (Semana *)data;
    if (!s) return;
    if (s->top10) g_ptr_array_free(s->top10, TRUE);
    if (s->reservas) g_ptr_array_free(s->reservas, TRUE);
    free(s);
}

int day_of_week(Data d) {
    struct tm t = {0};
    t.tm_year = d.ano - 1900;   // ano desde 1900
    t.tm_mon  = d.mes - 1;      // 0-11
    t.tm_mday = d.dia;          // 1-31
    t.tm_hour = 12;
    t.tm_isdst = -1;

    if (mktime(&t) == -1) return -1;
    return t.tm_wday; // 0 = domingo, 1 = segunda, ... 6 = sábado
}

GPtrArray* semanasInicia(GPtrArray *reservas_array) {
    //calcula o primeiro domingo antes da primeira data e o ultimo sabado depois da ultima data
    Data primeiro_dia = get_reservaAux_departure(g_ptr_array_index(reservas_array, 0));
    int dia_da_semana = day_of_week(primeiro_dia);
    Data inicio_semana = primeiro_dia;
    inicio_semana.dia -= dia_da_semana; // ajustar para o domingo anterior
    if (inicio_semana.dia <= 0) {
        inicio_semana.mes--;
        if (inicio_semana.mes <= 0) {
            inicio_semana.mes = 12;
            inicio_semana.ano--;
        }
        // Ajustar dia para o dia do mês anterior
        int dias_no_mes = 31;
        if (inicio_semana.mes == 2) {
            dias_no_mes = (inicio_semana.ano % 4 == 0 && (inicio_semana.ano % 100 != 0 || inicio_semana.ano % 400 == 0)) ? 29 : 28;
        } else if (inicio_semana.mes == 4 || inicio_semana.mes == 6 || inicio_semana.mes == 9 || inicio_semana.mes == 11) {
            dias_no_mes = 30;
        }
        inicio_semana.dia += dias_no_mes;
    }

    Data ultimo_dia = get_reservaAux_departure(g_ptr_array_index(reservas_array, reservas_array->len - 1));
    dia_da_semana = day_of_week(ultimo_dia);
    Data fim_semana = ultimo_dia;
    fim_semana.dia += (6 - dia_da_semana); // ajustar para o sábado seguinte
    int dias_no_mes = 31;
    if (fim_semana.mes == 2) {
        dias_no_mes = (fim_semana.ano % 4 == 0 && (fim_semana.ano % 100 != 0 || fim_semana.ano % 400 == 0)) ? 29 : 28;
    } else if (fim_semana.mes == 4 || fim_semana.mes == 6 || fim_semana.mes == 9 || fim_semana.mes == 11) {
        dias_no_mes = 30;
    }
    if (fim_semana.dia > dias_no_mes) {
        fim_semana.dia -= dias_no_mes;
        fim_semana.mes++;
        if (fim_semana.mes > 12) {
            fim_semana.mes = 1;
            fim_semana.ano++;
        }
    }

    // Criar hash table de semanas
    GPtrArray *semanas_array = g_ptr_array_new_with_free_func(free_semana);
    int semana_num = 1;
    Data semana_inicio = inicio_semana;
    while (semana_inicio.ano < fim_semana.ano || (semana_inicio.ano == fim_semana.ano && (semana_inicio.mes < fim_semana.mes || (semana_inicio.mes == fim_semana.mes && semana_inicio.dia <= fim_semana.dia)))) {
        Semana *semana = (Semana *)malloc(sizeof(Semana));
        set_semana_number(semana, semana_num);
        set_semana_inicio(semana, semana_inicio);
        Data semana_fim = semana_inicio;
        semana_fim.dia += 6;
        int dias_no_mes = 31;
        if (semana_fim.mes == 2) {
            dias_no_mes = (semana_fim.ano % 4 == 0 && (semana_fim.ano % 100 != 0 || semana_fim.ano % 400 == 0)) ? 29 : 28;
        } else if (semana_fim.mes == 4 || semana_fim.mes == 6 || semana_fim.mes == 9 || semana_fim.mes == 11) {
            dias_no_mes = 30;
        }
        if (semana_fim.dia > dias_no_mes) {
            semana_fim.dia -= dias_no_mes;
            semana_fim.mes++;
            if (semana_fim.mes > 12) {
                semana_fim.mes = 1;
                semana_fim.ano++;
            }
        }
        set_semana_fim(semana, semana_fim);
        semana->top10 = g_ptr_array_new_with_free_func(free_passageiroAux);
        semana->reservas = g_ptr_array_new();
        g_ptr_array_add(semanas_array, semana);
        semana_num++;
        // Avançar para o próximo domingo
        semana_inicio.dia += 7;
        // Recalcular dias do mês
        int dias_semana_inicio = 31;
        if (semana_inicio.mes == 2) {
            dias_semana_inicio = (semana_inicio.ano % 4 == 0 && (semana_inicio.ano % 100 != 0 || semana_inicio.ano % 400 == 0)) ? 29 : 28;
        } else if (semana_inicio.mes == 4 || semana_inicio.mes == 6 || semana_inicio.mes == 9 || semana_inicio.mes == 11) {
            dias_semana_inicio = 30;
        }
        
        if (semana_inicio.dia > dias_semana_inicio) {
            semana_inicio.dia -= dias_semana_inicio;
            semana_inicio.mes++;
            if (semana_inicio.mes > 12) {
                semana_inicio.mes = 1;
                semana_inicio.ano++;
            }
        }
    }
    return semanas_array;
}

void reservas_por_semana(GPtrArray *semanas_array, GPtrArray *reservas_array) {
    for (guint i = 0; i < reservas_array->len; i++) {
        ReservaAux *reservaAux = g_ptr_array_index(reservas_array, i);
        Data departure = get_reservaAux_departure(reservaAux);

        // Encontrar a semana correspondente
        for (guint j = 0; j < semanas_array->len; j++) {
            Semana *semana = (Semana *)g_ptr_array_index(semanas_array, j);
            Data inicio = get_semana_inicio(semana);
            Data fim = get_semana_fim(semana);

            // Verificar se departure está entre inicio (inclusive) e fim (inclusive)
            int departure_after_inicio = (departure.ano > inicio.ano) || 
                                        (departure.ano == inicio.ano && departure.mes > inicio.mes) ||
                                        (departure.ano == inicio.ano && departure.mes == inicio.mes && departure.dia >= inicio.dia);
            
            int departure_before_fim = (departure.ano < fim.ano) || 
                                      (departure.ano == fim.ano && departure.mes < fim.mes) ||
                                      (departure.ano == fim.ano && departure.mes == fim.mes && departure.dia <= fim.dia);

            if (departure_after_inicio && departure_before_fim) {
                // Adicionar a reserva à semana encontrada
                GPtrArray *reservas_semana = get_semana_reservas(semana);
                g_ptr_array_add(reservas_semana, reservaAux);
                set_semana_reservas(semana, reservas_semana);
                break; // Encontrada a semana correta, sair do loop
            }
        }
    }
}



void define_semanas(GPtrArray *semanas_array, Data data1, Data data2, int *semana_inicio, int *semana_fim) {
    *semana_inicio = -1;
    *semana_fim = -1;
    // Obter intervalo total do dataset
    Data dataset_inicio = get_semana_inicio(g_ptr_array_index(semanas_array, 0));
    Data dataset_fim = get_semana_fim(g_ptr_array_index(semanas_array, semanas_array->len - 1));
    dataset_inicio.hora = 0; dataset_inicio.minuto = 0;
    dataset_fim.hora = 23; dataset_fim.minuto = 59;

    // Se o intervalo pedido não intersecta o dataset, devolve vazio
    if (data_cmp(data2, dataset_inicio) < 0 || data_cmp(data1, dataset_fim) > 0) {
        *semana_inicio = 1;
        *semana_fim = 0;
        return;
    }

    for (guint i = 0; i < semanas_array->len; i++) {
        Semana *semana = (Semana *)g_ptr_array_index(semanas_array, i);
        Data inicio = get_semana_inicio(semana);
        Data fim = get_semana_fim(semana);
        inicio.hora = 0; inicio.minuto = 0;
        fim.hora = 23; fim.minuto = 59;
        int semana_num = get_semana_number(semana);

        // data1 dentro desta semana?
        if (*semana_inicio == -1) {
            int data1_after_inicio = (data_cmp(data1, inicio) >= 0);
            int data1_before_fim = (data_cmp(data1, fim) <= 0);
            if (data1_after_inicio && data1_before_fim) {
                *semana_inicio = semana_num;
            }
        }

        // data2 dentro desta semana?
        int data2_after_inicio = (data_cmp(data2, inicio) >= 0);
        int data2_before_fim = (data_cmp(data2, fim) <= 0);
        if (data2_after_inicio && data2_before_fim) {
            *semana_fim = semana_num;
        }
    }
    if (*semana_inicio == -1) *semana_inicio = 1; // data1 antes da primeira semana
    if (*semana_fim == -1) *semana_fim = (int)semanas_array->len; // data2 depois da última semana
}




void casasDecimais(GHashTable *auxHash, int casas) {
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, auxHash);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        AirlineAux *airlineAux = (AirlineAux *)value;
        double avg_delay = get_airlineAux_average_delay(airlineAux);
        // arredondar para x casas decimais
        double new_avg = round(avg_delay * pow(10, casas)) / pow(10, casas);
        set_airlineAux_average_delay(airlineAux, new_avg);
    }
}



//_______________________________________query6_________________________________________

struct nationalitys {
    char* nationality;
    GHashTable* aeroportos;
};

char* get_nationalitys_nationality(Nationalitys* nat) {
    return nat->nationality;
}
void set_nationalitys_nationality(Nationalitys* nat, char* nationality) {
    nat->nationality = strdup(nationality);
}
GHashTable* get_nationalitys_aeroportos(Nationalitys* nat) {
    return nat->aeroportos;
}
void set_nationalitys_aeroportos(Nationalitys* nat, GHashTable* aeroportos) {
    nat->aeroportos = aeroportos;
}

void free_nationalitys(gpointer data) {
    Nationalitys *n = (Nationalitys *)data;
    if (!n) return;
    if (n->nationality) free(n ->nationality);
    if (n->aeroportos) g_hash_table_destroy(n->aeroportos);
    free(n);
}

void query6_aux(Program *program, GHashTable *auxHash) {
    CatalogReserva *reservas_catalog = get_reservas_catalog(program);
    CatalogPassageiro *passageiros_catalog = get_passageiros_catalog(program);
    CatalogVoo *voos_catalog = get_voos_catalog(program);
    GPtrArray *reservas_array =  get_reservas_array(reservas_catalog);
    // Percorrer todas as reservas
    for (guint i = 0; i < reservas_array->len; i++) {
        Reserva *reserva = g_ptr_array_index(reservas_array, i);
        char * passageiro_id = get_reserva_pdocNumber(reserva);
        char * voo_ids = get_reserva_fids(reserva);
        int tipo = 0;
        char *destino = NULL;
        char *destino2 = NULL;

        if (voo_ids && (strlen(voo_ids) == 11 || strlen(voo_ids) == 12)) {
            char flight_id[9];
            if (strlen(voo_ids) == 11) {
                strncpy(flight_id, voo_ids + 2, 7);
                flight_id[7] = '\0';
            } else {
                strncpy(flight_id, voo_ids + 2, 8);
                flight_id[8] = '\0';
            }
            Voo *voo = catalog_get_voo_by_identifier(voos_catalog, flight_id);
            if (voo) {
                if (get_voo_status(voo) == CANCELLED) {
                    free(passageiro_id);
                    free(voo_ids);
                    continue;
                }
                destino = get_voo_destino(voo);
            }
            tipo = 1;
        } else if (voo_ids && (strlen(voo_ids) == 22 || strlen(voo_ids) == 23 || strlen(voo_ids) == 24)) {
            char flight_id1[9];
            char flight_id2[9];
            fids_Aux(voo_ids, flight_id1, flight_id2);
            Voo *voo1 = catalog_get_voo_by_identifier(voos_catalog, flight_id1);
            Voo *voo2 = catalog_get_voo_by_identifier(voos_catalog, flight_id2);
            if (voo1) {
                destino = get_voo_destino(voo1);
                if (get_voo_status(voo1) == CANCELLED) {
                    free(destino);
                    destino = NULL;
                }
            }
            if (voo2) {
                destino2 = get_voo_destino(voo2);
                if (get_voo_status(voo2) == CANCELLED) {
                    free(destino2);
                    destino2 = NULL;
                }
            }
            if (destino== NULL && destino2== NULL) {
                if (passageiro_id) free(passageiro_id);
                if (voo_ids) free(voo_ids);
                continue;
            }
            tipo = 2;
        }

        Passageiro *passageiro = catalog_get_passageiro_by_docnumber(passageiros_catalog, passageiro_id);
        char *nation = get_passageiro_nationality(passageiro);
        Nationalitys *natobj = g_hash_table_lookup(auxHash, nation);
        if (!natobj) {
            natobj = (Nationalitys *)malloc(sizeof(Nationalitys));
            set_nationalitys_nationality(natobj, nation);
            GHashTable* aeroportos_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroportoQ1);
            set_nationalitys_aeroportos(natobj, aeroportos_hash);
            if (tipo == 1 && destino) {
                AeroportoQ1* aeroporto = create_aeroportoQ1();
                set_aeroportoQ1_code(aeroporto, destino);
                set_aeroportoQ1_arrivalCount(aeroporto, 1);
                g_hash_table_insert(aeroportos_hash, strdup(destino), aeroporto);
            } else if (tipo == 2) {
                if (destino != NULL) {
                    AeroportoQ1* aeroporto1 = create_aeroportoQ1();
                    set_aeroportoQ1_code(aeroporto1, destino);
                    set_aeroportoQ1_arrivalCount(aeroporto1, 1);
                    g_hash_table_insert(aeroportos_hash, strdup(destino), aeroporto1);
                }
                if (destino2 != NULL) {
                    AeroportoQ1* aeroporto2 = create_aeroportoQ1();
                    set_aeroportoQ1_code(aeroporto2, destino2);
                    set_aeroportoQ1_arrivalCount(aeroporto2, 1);
                    g_hash_table_insert(aeroportos_hash, strdup(destino2), aeroporto2);
                }
            }
            char *key = strdup(nation);
            g_hash_table_insert(auxHash, key, natobj);
        } else {
            GHashTable* aeroportos_hash = get_nationalitys_aeroportos(natobj);
            if (tipo == 1 && destino) {
                AeroportoQ1* aeroporto = g_hash_table_lookup(aeroportos_hash, destino);
                if (!aeroporto) {
                    aeroporto = create_aeroportoQ1();
                    set_aeroportoQ1_code(aeroporto, destino);
                    set_aeroportoQ1_arrivalCount(aeroporto, 1);
                    g_hash_table_insert(aeroportos_hash, strdup(destino), aeroporto);
                } else {
                    int current_count = get_aeroportoQ1_arrivalCount(aeroporto);
                    set_aeroportoQ1_arrivalCount(aeroporto, current_count + 1);
                }
            } else if (tipo == 2) {
                if (destino != NULL) {
                    AeroportoQ1* aeroporto1 = g_hash_table_lookup(aeroportos_hash, destino);
                    if (!aeroporto1) {
                        aeroporto1 = create_aeroportoQ1();
                        set_aeroportoQ1_code(aeroporto1, destino);
                        set_aeroportoQ1_arrivalCount(aeroporto1, 1);
                        g_hash_table_insert(aeroportos_hash, strdup(destino), aeroporto1);
                    } else {
                        int current_count = get_aeroportoQ1_arrivalCount(aeroporto1);
                        set_aeroportoQ1_arrivalCount(aeroporto1, current_count + 1);
                    }
                }
                if (destino2 != NULL) {
                    AeroportoQ1* aeroporto2 = g_hash_table_lookup(aeroportos_hash, destino2);
                    if (!aeroporto2) {
                        aeroporto2 = create_aeroportoQ1();
                        set_aeroportoQ1_code(aeroporto2, destino2);
                        set_aeroportoQ1_arrivalCount(aeroporto2, 1);
                        g_hash_table_insert(aeroportos_hash, strdup(destino2), aeroporto2);
                    } else {
                        int current_count = get_aeroportoQ1_arrivalCount(aeroporto2);
                        set_aeroportoQ1_arrivalCount(aeroporto2, current_count + 1);
                    }
                }
            }
        }
        free(passageiro_id);
        free(voo_ids);
        free(nation);
        if (destino) free(destino);
        if (destino2) free(destino2);
    }
}