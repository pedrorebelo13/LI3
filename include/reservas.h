#ifndef RESERVAS_H
#define RESERVAS_H


/**
 * @brief Estrutura opaca que representa o catálogo de reservas.
 *
 * Os detalhes da estrutura catalogReserva são definidos no ficheiro de implementação.
 */
typedef struct catalogReserva catalogReserva;

#include <glib.h>
#include "reserva.h"

/**
 * @brief Inicializa o catálogo de reservas.
 *
 * Cria e devolve um novo catálogo vazio para armazenar reservas.
 *
 * @return Ponteiro para o catálogo inicializado.
 */
catalogReserva *initialize_reservas_catalog();

/**
 * @brief Liberta a memória associada ao catálogo de reservas.
 *
 * @param catalog Ponteiro para o catálogo a ser libertado.
 */
void free_reservas_catalog(catalogReserva *catalog);

/**
 * @brief Insere uma reserva no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de reservas.
 * @param reserva Ponteiro para a reserva a ser inserida.
 */
void insert_reserva(catalogReserva *catalog, Reserva *reserva);

/**
 * @brief Obtém um array de ponteiros para todas as reservas do catálogo.
 *
 * @param catalog Ponteiro para o catálogo de reservas.
 * @return Array de ponteiros para reservas (GPtrArray).
 */
GPtrArray *get_reservas_array(catalogReserva *catalog);

/**
 * @brief Procura uma reserva pelo identificador no catálogo.
 *
 * @param catalog Ponteiro para o catálogo de reservas.
 * @param rid String com o identificador da reserva.
 * @return Ponteiro para a reserva encontrada ou NULL se não existir.
 */
Reserva *catalog_get_reserva_by_rid(catalogReserva *catalog, const char *rid);

#endif