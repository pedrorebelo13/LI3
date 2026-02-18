#ifndef MAIN_H
#define MAIN_H

#/**
 * @brief Nome do diretório de saída dos resultados.
 */
#define OUTPUT_NAME "resultados"

#/**
 * @brief Formato do nome dos ficheiros de saída dos comandos.
 *
 * Utiliza o número do comando para gerar o nome do ficheiro.
 */
#define OUTPUT_FILE_NAME "command%d_output.txt"

#/**
 * @brief Tamanho do buffer utilizado para leitura de dados.
 */
#define BUFFER_SIZE 200

#/**
 * @brief Inclusão da biblioteca GLib para estruturas de dados e utilitários.
 */
#include <glib.h>


#endif