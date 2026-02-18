#ifndef VOOSAUX_H
#define VOOSAUX_H

#include <glib.h>
#include "mainUtils.h"

// Query 5 - Média de atrasos por companhia aérea
/**
 * @brief Estrutura auxiliar para companhia aérea (Query 5).
 */
typedef struct airlineAux AirlineAux;
/**
 * @brief Define o código da companhia aérea.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @param code String com o código.
 */
void set_airlineAux_code(AirlineAux* airline, char* code);
/**
 * @brief Obtém o código da companhia aérea.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @return String com o código.
 */
char* get_airlineAux_code(AirlineAux* airline);
/**
 * @brief Define a média de atraso.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @param average_delay Média de atraso.
 */
void set_airlineAux_average_delay(AirlineAux* airline, double average_delay);
/**
 * @brief Obtém a média de atraso.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @return Média de atraso.
 */
double get_airlineAux_average_delay(AirlineAux* airline);
/**
 * @brief Obtém a contagem de voos atrasados.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @return Número de voos atrasados.
 */
int get_airlineAux_flight_delayed_count(AirlineAux* airline);
/**
 * @brief Define a contagem de voos atrasados.
 * @param airline Ponteiro para a estrutura AirlineAux.
 * @param flight_delayed_count Número de voos atrasados.
 */
void set_airlineAux_flight_delayed_count(AirlineAux* airline, int flight_delayed_count);
/**
 * @brief Liberta a memória de uma estrutura AirlineAux.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_airlineAux(gpointer data);
/**
 * @brief Função auxiliar para processar a query 5.
 * @param program Ponteiro para a estrutura principal do programa.
 * @param auxHash Hash table auxiliar.
 */
void query5_aux(Program *program, GHashTable *auxHash);

/**
 * @brief Ordena companhias aéreas.
 * @param auxHash Hash table auxiliar.
 * @return Array de companhias aéreas ordenadas.
 */
GPtrArray *sort_airlines(GHashTable *auxHash);
/**
 * @brief Função de comparação para ordenar companhias por média de atraso.
 * @param a Primeiro elemento.
 * @param b Segundo elemento.
 * @return Valor negativo se a < b, positivo se a > b, zero se iguais.
 */
int compare_airlines_by_average_delay(gconstpointer a, gconstpointer b);

#endif