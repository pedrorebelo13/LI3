#ifndef PARSER_H
#define PARSER_H

#include "mainUtils.h"

/**
 * @brief Tipo de função para armazenar dados processados.
 *
 * Define o protótipo de funções que recebem dados parseados e os armazenam
 * numa estrutura de dados específica.
 *
 * @param char** Array de strings com os campos processados.
 * @param void* Ponteiro genérico para o catálogo onde os dados serão armazenados.
 * @param Program* Ponteiro para a estrutura principal do programa.
 * @return int 0 em caso de sucesso, outro valor em caso de erro.
 */
typedef int(StoreData)(char**, void*, Program*);

/**
 * @brief Extrai o próximo campo de uma linha CSV.
 *
 * Processa a linha apontada por line_ptr, extraindo o próximo campo delimitado
 * por vírgula ou fim de linha, respeitando aspas.
 *
 * @param line_ptr Ponteiro para o ponteiro da linha a ser processada.
 * @return String com o campo extraído, ou NULL se não houver mais campos.
 */
char* extract_next_field(char** line_ptr);

/**
 * @brief Faz o parse de um ficheiro CSV e armazena os dados.
 *
 * Lê o ficheiro CSV linha a linha, extrai os campos, valida os dados e
 * armazena-os usando a função store_data fornecida.
 *
 * @param csv Ponteiro para o ficheiro CSV aberto.
 * @param store_data Função callback para armazenar os dados processados.
 * @param data_catalog Ponteiro genérico para o catálogo de dados.
 * @param csv_name Nome do ficheiro CSV para fins de log.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int parse_csv_and_store_data(FILE* csv, StoreData* store_data, void* data_catalog, const char *csv_name, Program *program);

/**
 * @brief Remove aspas de um campo.
 *
 * Remove aspas duplas no início e fim de uma string, se existirem.
 *
 * @param field String do campo a ser processado.
 * @return String sem aspas.
 */
char* remove_quotes(char* field);

/**
 * @brief Regista uma entrada inválida no ficheiro de erros.
 *
 * Escreve os campos inválidos num ficheiro de log específico para o CSV,
 * incluindo o cabeçalho se ainda não tiver sido escrito.
 *
 * @param fields Array de strings com os campos da entrada inválida.
 * @param field_count Número de campos no array.
 * @param csv_name Nome do ficheiro CSV de origem.
 * @param header String com o cabeçalho do CSV.
 * @param header_written Flag indicando se o cabeçalho já foi escrito.
 */
void log_invalid_entry(char **fields, int field_count, const char *csv_name, const char *header,int header_written);

#endif