#ifndef QUERIES_H
#define QUERIES_H

#include "mainUtils.h"

/**
 * @brief Executa uma query específica com base no número fornecido.
 *
 * Identifica e executa a query correspondente ao número fornecido,
 * passando os argumentos necessários.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param query_number Número da query a ser executada.
 * @param arguments String com os argumentos da query.
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void run_query(Program *program, int query_number, char *arguments, int command_number, int has_S);

/**
 * @brief Consulta informações de um aeroporto pelo código.
 *
 * Obtém e apresenta as informações detalhadas de um aeroporto específico
 * identificado pelo seu código.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param code String com o código do aeroporto.
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_aeroport_by_code(Program *program, char *code, int command_number, int has_S);

/**
 * @brief Lista as top N aeronaves por número de voos.
 *
 * Obtém e lista as N aeronaves com maior número de voos realizados,
 * ordenadas por contagem decrescente.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param arguments String com os argumentos (número N).
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_list_top_n_aeronaves_by_flightcount(Program *program, char *arguments, int command_number, int has_S);

/**
 * @brief Consulta o aeroporto com mais voos numa determinada data.
 *
 * Identifica e apresenta o aeroporto que registou o maior número de voos
 * numa data específica.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param arguments String com os argumentos (data).
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_aeroporto_mais_voos_by_data(Program *program, char *arguments, int command_number, int has_S);

/**
 * @brief Executa a query 4.
 *
 * Implementa a lógica da query 4 conforme especificado.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param arguments String com os argumentos da query.
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_4(Program *program, char *arguments, int command_number, int has_S);

/**
 * @brief Executa a query 5.
 *
 * Implementa a lógica da query 5 conforme especificado.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param arguments String com os argumentos da query.
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_5(Program *program, char *arguments, int command_number, int has_S);

/**
 * @brief Executa a query 6.
 *
 * Implementa a lógica da query 6 conforme especificado.
 *
 * @param program Ponteiro para a estrutura principal do programa.
 * @param arguments String com os argumentos da query.
 * @param command_number Número do comando para identificação do output.
 * @param has_S Flag indicando se o formato de saída é simplificado.
 */
void query_6(Program *program, char *arguments, int command_number, int has_S);

/**
 * @brief Guarda o output num ficheiro.
 *
 * Escreve o resultado de uma query num ficheiro de output específico,
 * identificado pelo número do comando.
 *
 * @param command_number Número do comando para identificação do ficheiro.
 * @param output String com o conteúdo a ser guardado.
 */
void save_output(int command_number, char *output);

#endif