#ifndef RESVAUX_H
#define RESVAUX_H
#include "voo.h"
#include "main.h"

// Query 4 - Passageiro com mais gastos numa semana
/**
 * @brief Estrutura auxiliar para reserva (Query 4).
 */
typedef struct reservaAux ReservaAux;
/**
 * @brief Obtém o identificador da reserva.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @return String com o identificador.
 */
char* get_reservaAux_id(ReservaAux* reserva);
/**
 * @brief Define o identificador da reserva.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @param id String com o identificador.
 */
void set_reservaAux_id(ReservaAux* reserva, char* id);
/**
 * @brief Obtém o número de documento do passageiro.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @return String com o número de documento.
 */
char* get_reservaAux_docNumber(ReservaAux* reserva);
/**
 * @brief Define o número de documento do passageiro.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @param docNumber String com o número de documento.
 */
void set_reservaAux_docNumber(ReservaAux* reserva, char* docNumber);
/**
 * @brief Obtém os identificadores dos voos.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @return String com os identificadores.
 */
char* get_reservaAux_flightIds(ReservaAux* reserva);
/**
 * @brief Define os identificadores dos voos.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @param flightIds String com os identificadores.
 */
void set_reservaAux_flightIds(ReservaAux* reserva, char* flightIds);
/**
 * @brief Obtém a data de partida.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @return Estrutura Data com a data de partida.
 */
Data get_reservaAux_departure(ReservaAux* reserva);
/**
 * @brief Define a data de partida.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @param departure Data de partida.
 */
void set_reservaAux_departure(ReservaAux* reserva, Data departure);
/**
 * @brief Obtém o preço da reserva.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @return Preço da reserva.
 */
double get_reservaAux_price(ReservaAux* reserva);
/**
 * @brief Define o preço da reserva.
 * @param reserva Ponteiro para a estrutura ReservaAux.
 * @param price Preço.
 */
void set_reservaAux_price(ReservaAux* reserva, double price);
/**
 * @brief Liberta a memória de uma estrutura ReservaAux.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_reservaAux(gpointer data);
/**
 * @brief Ordena reservas por data de partida.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return Array de reservas ordenadas.
 */
GPtrArray *sort_reservas_by_departure(Program *program);
/**
 * @brief Função de comparação para ordenar reservas por data de partida.
 * @param a Primeiro elemento.
 * @param b Segundo elemento.
 * @return Valor negativo se a < b, positivo se a > b, zero se iguais.
 */
int compare_reservas_by_departure(gconstpointer a, gconstpointer b);

#endif