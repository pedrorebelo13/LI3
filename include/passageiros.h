#ifndef PASSAGEIROS_H
#define PASSAGEIROS_H


/**
 * @brief Estrutura opaca que representa o catálogo de passageiros.
 *
 * Os detalhes da estrutura CatalogPassageiro são definidos no ficheiro de implementação.
 */
typedef struct catalogPassageiro CatalogPassageiro;

#include <glib.h>
#include "passageiro.h"

/**
 * @brief Inicializa o catálogo de passageiros.
 *
 * Cria e devolve um novo catálogo vazio para armazenar passageiros.
 *
 * @return Ponteiro para o catálogo inicializado.
 */
CatalogPassageiro *initialize_passageiros_catalog();

/**
 * @brief Liberta a memória associada ao catálogo de passageiros.
 *
 * @param catalog Ponteiro para o catálogo a ser libertado.
 */
void free_passageiros_catalog(CatalogPassageiro *catalog);

/**
 * @brief Insere um passageiro no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de passageiros.
 * @param passageiro Ponteiro para o passageiro a ser inserido.
 */
void insert_passageiro(CatalogPassageiro *catalog, Passageiro *passageiro);

/**
 * @brief Obtém um array de ponteiros para todos os passageiros do catálogo.
 *
 * @param catalog Ponteiro para o catálogo de passageiros.
 * @return Array de ponteiros para passageiros (GPtrArray).
 */
GPtrArray *get_passageiros_array(CatalogPassageiro *catalog);

/**
 * @brief Procura um passageiro pelo número de documento no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de passageiros.
 * @param docnumber String com o número de documento do passageiro.
 * @return Ponteiro para o passageiro encontrado ou NULL se não existir.
 */
Passageiro *catalog_get_passageiro_by_docnumber(CatalogPassageiro *catalog, char *docnumber);

#endif