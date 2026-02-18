#ifndef VALIDACAO_H
#define VALIDACAO_H

#include "mainUtils.h"


/**
 * @brief Valida os dados de um aeroporto.
 *
 * Verifica se os campos fornecidos para um aeroporto são válidos,
 * incluindo verificação de tipos e formatos.
 *
 * @param aeroporto_info Array de strings com informações do aeroporto.
 * @param expected_fields Número esperado de campos.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_aeroporto(char **aeroporto_info, int expected_fields, Program*);

/**
 * @brief Valida os dados de um voo.
 *
 * Verifica se os campos fornecidos para um voo são válidos,
 * incluindo verificação de datas, horários e referências.
 *
 * @param voo_info Array de strings com informações do voo.
 * @param expected_fields Número esperado de campos.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_voo(char **voo_info, int expected_fields, Program *program);

/**
 * @brief Valida os dados de uma aeronave.
 *
 * Verifica se os campos fornecidos para uma aeronave são válidos,
 * incluindo verificação de tipos e valores numéricos.
 *
 * @param aeronave_info Array de strings com informações da aeronave.
 * @param expected_fields Número esperado de campos.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_aeronave(char **aeronave_info, int expected_fields, Program*);

/**
 * @brief Valida os dados de uma reserva.
 *
 * Verifica se os campos fornecidos para uma reserva são válidos,
 * incluindo verificação de preços, flags e referências.
 *
 * @param reserva_info Array de strings com informações da reserva.
 * @param expected_fields Número esperado de campos.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_reserva(char **reserva_info, int expected_fields, Program* program);

/**
 * @brief Valida os dados de um passageiro.
 *
 * Verifica se os campos fornecidos para um passageiro são válidos,
 * incluindo verificação de email, data de nascimento e género.
 *
 * @param passageiro_info Array de strings com informações do passageiro.
 * @param expected_fields Número esperado de campos.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_passageiro(char **passageiro_info, int expected_fields, Program*) ;

/**
 * @brief Valida o formato de uma data.
 *
 * Verifica se uma string representa uma data válida no formato esperado.
 *
 * @param date String com a data a validar.
 * @return 1 se válida, 0 caso contrário.
 */
int validate_date(const char *date);

/**
 * @brief Valida o formato de um endereço de email.
 *
 * Verifica se uma string representa um email válido.
 *
 * @param email String com o email a validar.
 * @return 1 se válido, 0 caso contrário.
 */
int validate_email(const char *email);

/**
 * @brief Compara duas datas.
 *
 * Compara duas estruturas Data e determina a ordem cronológica.
 *
 * @param a Primeira data.
 * @param b Segunda data.
 * @return Valor negativo se a < b, positivo se a > b, zero se iguais.
 */
int data_cmp(Data a, Data b);

/**
 * @brief Valida a data de nascimento.
 *
 * Verifica se uma string representa uma data de nascimento válida,
 * garantindo que não é uma data futura.
 *
 * @param date String com a data de nascimento a validar.
 * @return 1 se válida, 0 caso contrário.
 */
int validate_date_dob(const char *date);

#endif