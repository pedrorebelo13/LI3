#ifndef AERONAVES_H
#define AERONAVES_H
  

/**
 * @brief Estrutura opaca que representa o catálogo de aeronaves.
 *
 * Os detalhes da estrutura CatalogAeronave são definidos no ficheiro de implementação.
 */
typedef struct catalogAeronave CatalogAeronave;

#include "aeronave.h"
#include <glib.h> 


/**
 * @brief Inicializa o catálogo de aeronaves.
 *
 * Cria e devolve um novo catálogo vazio para armazenar aeronaves.
 *
 * @return Ponteiro para o catálogo inicializado.
 */
CatalogAeronave *initialize_aeronaves_catalog();

/**
 * @brief Liberta a memória associada ao catálogo de aeronaves.
 *
 * @param catalog Ponteiro para o catálogo a ser libertado.
 */
void free_aeronaves_catalog(CatalogAeronave *catalog);

/**
 * @brief Insere uma aeronave no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeronaves.
 * @param aeronave Ponteiro para a aeronave a ser inserida.
 */
void insert_aeronave(CatalogAeronave *catalog, Aeronave *aeronave);

/**
 * @brief Obtém um array de ponteiros para todas as aeronaves do catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeronaves.
 * @return Array de ponteiros para aeronaves (GPtrArray).
 */
GPtrArray *get_aeronaves_array(CatalogAeronave *catalog);

/**
 * @brief Procura uma aeronave pelo identificador no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de aeronaves.
 * @param identifier String com o identificador da aeronave.
 * @return Ponteiro para a aeronave encontrada ou NULL se não existir.
 */
Aeronave *catalog_get_aeronave_by_identifier(CatalogAeronave *catalog, char *identifier);

#endif