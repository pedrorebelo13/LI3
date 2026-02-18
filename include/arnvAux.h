#ifndef ARNVAUX_H
#define ARNVAUX_H

#include <glib.h>
#include "aeronaves.h"
#include "voos.h"
#include "aeronave.h"
#include "voo.h"
#include "mainUtils.h"


// Query 2 - Top N aeronaves por número de voos
/**
 * @brief Estrutura auxiliar para aeronave (Query 2).
 */
typedef struct aeronaveAux AeronaveAux;
/**
 * @brief Define o identificador da aeronave.
 * @param aeronave Ponteiro para a estrutura AeronaveAux.
 * @param identifier String com o identificador.
 */
void set_aeronaveAux_identifier(AeronaveAux* aeronave, char* identifier);
/**
 * @brief Obtém o identificador da aeronave.
 * @param aeronave Ponteiro para a estrutura AeronaveAux.
 * @return String com o identificador.
 */
char* get_aeronaveAux_identifier(AeronaveAux* aeronave);
/**
 * @brief Define a contagem de voos.
 * @param aeronave Ponteiro para a estrutura AeronaveAux.
 * @param flightCount Número de voos.
 */
void set_aeronaveAux_flightCount(AeronaveAux* aeronave, int flightCount);
/**
 * @brief Obtém a contagem de voos.
 * @param aeronave Ponteiro para a estrutura AeronaveAux.
 * @return Número de voos.
 */
int get_aeronaveAux_flightCount(AeronaveAux* aeronave);
/**
 * @brief Preenche a hash table auxiliar com aeronaves e suas contagens de voos.
 * @param auxHash Hash table auxiliar.
 * @param aeronaves_catalog Catálogo de aeronaves.
 * @param filtered_voos Array de voos filtrados.
 */
void fill_aeronaveAux(GHashTable *auxHash, CatalogAeronave *aeronaves_catalog, GPtrArray *filtered_voos);
/**
 * @brief Função de comparação para ordenar aeronaves por contagem de voos.
 * @param a Primeiro elemento.
 * @param b Segundo elemento.
 * @return Valor negativo se a < b, positivo se a > b, zero se iguais.
 */
gint compare_aeronaves_by_flightcount(gconstpointer a, gconstpointer b);
/**
 * @brief Ordena aeronaves por contagem de voos.
 * @param auxHash Hash table auxiliar.
 * @return Array ordenado de aeronaves.
 */
GPtrArray* sort_aeronaves(GHashTable *auxHash);
/**
 * @brief Obtém as top N aeronaves.
 * @param aeronaves_array Array de aeronaves.
 * @param N Número de aeronaves a retornar.
 * @param manufacturer Filtro por fabricante (pode ser NULL).
 * @param aeronaves_catalog Catálogo de aeronaves.
 * @return Array com as top N aeronaves.
 */
GPtrArray* get_top_n_aeronaves(GPtrArray *aeronaves_array, int N, char *manufacturer, CatalogAeronave *aeronaves_catalog);
/**
 * @brief Liberta a memória de uma estrutura AeronaveAux.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_aeronaveAux(gpointer data);
/**
 * @brief Função auxiliar para processar a query 2.
 * @param program Ponteiro para a estrutura principal do programa.
 * @param auxHash Hash table auxiliar.
 */
void query2_aux(Program *program, GHashTable *auxHash);

#endif