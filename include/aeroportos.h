#ifndef AEROPORTOS_H
#define AEROPORTOS_H

#include "aeroporto.h"
#include <glib.h>

/**
 * @brief Estrutura opaca que representa o catálogo de aeroportos.
 *
 * Os detalhes da estrutura CatalogAeroporto são definidos no ficheiro de implementação.
 */
typedef struct catalogAeroporto CatalogAeroporto;


/**
 * @brief Inicializa o catálogo de aeroportos.
 *
 * Cria e devolve um novo catálogo vazio para armazenar aeroportos.
 *
 * @return Ponteiro para o catálogo inicializado.
 */
CatalogAeroporto *initialize_aeroportos_catalog();

/**
 * @brief Liberta a memória associada ao catálogo de aeroportos.
 *
 * @param catalog Ponteiro para o catálogo a ser libertado.
 */
void free_aeroportos_catalog(CatalogAeroporto *catalog);

/**
 * @brief Insere um aeroporto no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeroportos.
 * @param aeroporto Ponteiro para o aeroporto a ser inserido.
 */
void insert_aeroporto(CatalogAeroporto *catalog, Aeroporto *aeroporto);

/**
 * @brief Obtém um array de ponteiros para todos os aeroportos do catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeroportos.
 * @return Array de ponteiros para aeroportos (GPtrArray).
 */
GPtrArray *get_aeroportos_array(CatalogAeroporto *catalog);

/**
 * @brief Procura um aeroporto pelo código no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeroportos.
 * @param code String com o código do aeroporto.
 * @return Ponteiro para o aeroporto encontrado ou NULL se não existir.
 */
Aeroporto *catalog_get_aeroporto_by_code(CatalogAeroporto *catalog, char *code);

#endif