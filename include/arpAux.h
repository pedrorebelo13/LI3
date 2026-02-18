#ifndef ARP_AUX_H
#define ARP_AUX_H
#include <glib.h>
#include "voos.h"
#include "aeroportos.h"
#include "aeroporto.h"
#include "voo.h"

/**
 * @brief Estrutura auxiliar para estatísticas de aeroporto (Query 1).
 */
typedef struct aeroportoQ1 AeroportoQ1;
/**
 * @brief Cria uma nova estrutura AeroportoQ1.
 * @return Ponteiro para a nova estrutura AeroportoQ1.
 */
AeroportoQ1* create_aeroportoQ1();
/**
 * @brief Define o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @param code String com o código do aeroporto.
 */
void set_aeroportoQ1_code(AeroportoQ1* aeroporto, char* code);
/**
 * @brief Obtém o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @return String com o código do aeroporto.
 */
char* get_aeroportoQ1_code(AeroportoQ1* aeroporto);
/**
 * @brief Define a contagem de chegadas.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @param arrivalCount Número de chegadas.
 */
void set_aeroportoQ1_arrivalCount(AeroportoQ1* aeroporto, int arrivalCount);
/**
 * @brief Obtém a contagem de chegadas.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @return Número de chegadas.
 */
int get_aeroportoQ1_arrivalCount(AeroportoQ1* aeroporto);
/**
 * @brief Define a contagem de partidas.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @param departureCount Número de partidas.
 */
void set_aeroportoQ1_departureCount(AeroportoQ1* aeroporto, int departureCount);
/**
 * @brief Obtém a contagem de partidas.
 * @param aeroporto Ponteiro para a estrutura AeroportoQ1.
 * @return Número de partidas.
 */
int get_aeroportoQ1_departureCount(AeroportoQ1* aeroporto);
/**
 * @brief Liberta a memória de uma estrutura AeroportoQ1.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_aeroportoQ1(gpointer data);
/**
 * @brief Função auxiliar para processar a query 1.
 * @param program Ponteiro para a estrutura principal do programa.
 * @param auxHash Hash table auxiliar.
 */
void query1_aux(Program *program, GHashTable *auxHash);

//QUERY3
/**
 * @brief Estrutura auxiliar para estatísticas de aeroporto (Query 3).
 */
typedef struct aeroportoAux AeroportoAux;
/**
 * @brief Define o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura AeroportoAux.
 * @param code String com o código.
 */ 
void set_aeroportoAux_code(AeroportoAux* aeroporto, char* code);
/**
 * @brief Obtém o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura AeroportoAux.
 * @return String com o código.
 */
char* get_aeroportoAux_code(AeroportoAux* aeroporto);
/**
 * @brief Define a contagem de partidas.
 * @param aeroporto Ponteiro para a estrutura AeroportoAux.
 * @param departureCount Número de partidas.
 */
void set_aeroportoAux_departureCount(AeroportoAux* aeroporto, int departureCount);
/**
 * @brief Obtém a contagem de partidas.
 * @param aeroporto Ponteiro para a estrutura AeroportoAux.
 * @return Número de partidas.
 */
int get_aeroportoAux_departureCount(AeroportoAux* aeroporto);
/**
 * @brief Obtém o aeroporto com mais voos num período.
 * @param aeroportos_catalog Catálogo de aeroportos.
 * @param voos_catalog Catálogo de voos.
 * @param data1 Data inicial.
 * @param data2 Data final.
 * @param aeroporto_out Ponteiro para o aeroporto resultado.
 */
void get_aeroporto_mais_voos(CatalogAeroporto *aeroportos_catalog, CatalogVoo *voos_catalog, Data data1, Data data2, AeroportoAux **aeroporto_out);
/**
 * @brief Liberta a memória de uma estrutura AeroportoAux.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_aeroportoAux(gpointer data);

//QUERY6
/**
 * @brief Obtém o aeroporto mais frequente para uma nacionalidade.
 * @param nationalitysHash Hash table de nacionalidades.
 * @param nationality String com a nacionalidade.
 * @return Ponteiro para o aeroporto mais frequente.
 */
AeroportoQ1* get_top_aeroporto_from_nationality(GHashTable* nationalitysHash, char* nationality);

#endif 