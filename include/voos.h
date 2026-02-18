#ifndef VOOS_H
#define VOOS_H

/**
 * @brief Estrutura opaca que representa o catálogo de voos.
 * Os detalhes da estrutura CatalogVoo são definidos no ficheiro de implementação.
 */
typedef struct catalogVoo CatalogVoo;

#include "voo.h"
#include <glib.h>


/**
 * @brief Inicializa o catálogo de voos.
 *
 * Cria e devolve um novo catálogo vazio para armazenar voos.
 *
 * @return Ponteiro para o catálogo inicializado.
 */
CatalogVoo *initialize_voos_catalog();

/**
 * @brief Liberta a memória associada ao catálogo de voos.
 *
 * @param catalog Ponteiro para o catálogo a ser libertado.
 */
void free_voos_catalog(CatalogVoo *catalog);

/**
 * @brief Insere um voo no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de voos.
 * @param voo Ponteiro para o voo a ser inserido.
 */
void insert_voo(CatalogVoo *catalog, Voo *voo);

/**
 * @brief Obtém um array de ponteiros para todos os voos do catálogo.
 *
 * @param catalog Ponteiro para o catálogo de voos.
 * @return Array de ponteiros para voos (GPtrArray).
 */
GPtrArray *get_voos_array(CatalogVoo *catalog);

/**
 * @brief Procura um voo pelo identificador no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de voos.
 * @param identifier String com o identificador do voo.
 * @return Ponteiro para o voo encontrado ou NULL se não existir.
 */
Voo *catalog_get_voo_by_identifier(CatalogVoo *catalog, char *identifier);


#endif