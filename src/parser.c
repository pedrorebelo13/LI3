#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h" 
#include "main.h"
#include "mainUtils.h"


#define BUFFER_SIZE 200
#define DELIMITER ','

char* extract_next_field(char** line_ptr) {
    char *start = *line_ptr;
    if (*start == '\0') {
        return NULL; // Fim da linha
    }

    char *end;
    int in_quotes = 0;
    
    // Se o campo começa com aspas, a vírgula seguinte é ignorada.
    if (*start == '"') {
        in_quotes = 1;
        start++; // Ignora as aspas iniciais para a pesquisa
    }

    end = start;
    
    while (*end != '\0') {
        if (*end == '"') {
            // Se encontrar aspas dentro do campo
            if (in_quotes) {
                // Se o próximo caractere for uma vírgula ou o fim da string, as aspas terminam o campo citado.
                if (*(end + 1) == DELIMITER || *(end + 1) == '\0') {
                    in_quotes = 0;
                    // Marca o fim do campo (depois das aspas de fecho)
                    break;
                }
            }
        }
        
        // Se encontrar o delimitador (e NÃO estamos dentro de aspas), o campo terminou.
        if (*end == DELIMITER && !in_quotes) {
            break; 
        }

        end++;
    }

    // Se encontrar o delimitador ou o fim da string
    size_t field_len = end - *line_ptr;
    
    if (*end == '\0' && in_quotes) {
        return NULL; 
    }

    char *field = (char *)malloc(field_len + 1);

    strncpy(field, *line_ptr, field_len);
    field[field_len] = '\0';

    // Move o ponteiro para o início do próximo campo
    if (*end == DELIMITER) {
        *line_ptr = end + 1; // Salta o delimitador (,)
    } else if (*end == '\0') { //Se chegou ao fim da linha
        *line_ptr = end;
    } else { //Se encontrou aspas
        char* next_char = end;
        if (*next_char == '"') next_char++; // Salta a aspa de fecho
        if (*next_char == DELIMITER) {
             *line_ptr = next_char + 1; // Salta a vírgula e vai para o próximo campo
        } else {
             *line_ptr = next_char; // Aponta para o '\0' ou caractere inesperado
        }
    }

    return field;
}

static int count_fields(const char *line) {
    if (!line) return 0;
    char *copy = strdup(line);
    char *p = copy;
    int count = 0;
    char *field;
    while ((field = extract_next_field(&p)) != NULL) {
        count++;
        free(field);
        if (*p == '\0') break;
    }
    free(copy);
    return count;
}

int parse_csv_and_store_data(FILE* csv, StoreData* store_data, void* data_catalog, const char *csv_name, Program *program)
{
    char *line_buffer;
    size_t buffer_size = BUFFER_SIZE;
    int read = 0;
    int header_written = 0;
    int expected_fields = 0;

    line_buffer = (char *)malloc(buffer_size * sizeof(char));

    // Lê a primeira linha (cabeçalho)
    read = getline(&line_buffer, &buffer_size, csv);
    if (read != -1) {
        // Remove o caractere newline no final da linha
        while (read > 0 && (line_buffer[read-1] == '\n' || line_buffer[read-1] == '\r')) {
            line_buffer[--read] = '\0';
        }
        expected_fields = count_fields(line_buffer);
        log_invalid_entry(NULL, 0, csv_name, line_buffer, header_written);  // Escreve o cabeçalho no arquivo de erros
    }

    // Função auxiliae para contar aspas numa linha
    int count_quotes_local(const char *s) {
        int q = 0; if (!s) return 0; while (*s) { if (*s == '"') q++; s++; } return q; }

    // Lê as linhas subsequentes, juntando linhas quando há campos multiline (aspas não fechadas)
    while ((read = getline(&line_buffer, &buffer_size, csv)) != -1) {
        // Remover newline e carriage return da linha lida
        while (read > 0 && (line_buffer[read-1] == '\n' || line_buffer[read-1] == '\r')) {
            line_buffer[--read] = '\0';
        }

        // Acumular se aspas ímpares (campo multiline) - CSV padrão permite newlines dentro de campos entre aspas
        if (count_quotes_local(line_buffer) % 2 != 0) {
            // Usa GString para facilitar
            GString *acc = g_string_new(line_buffer);
            char *continuation = NULL; size_t cont_size = 0; int r2;
            while ((r2 = getline(&continuation, &cont_size, csv)) != -1) {
                // Remover newline e carriage return da continuação
                while (r2 > 0 && (continuation[r2-1] == '\n' || continuation[r2-1] == '\r')) {
                    continuation[--r2] = '\0';
                }
                g_string_append_c(acc, '\n'); // preserva quebra interna
                g_string_append(acc, continuation);
                if (count_quotes_local(acc->str) % 2 == 0) break; // aspas balanceadas
            }
            // Substitui line_buffer pelo conteúdo acumulado
            free(line_buffer);
            line_buffer = strdup(acc->str);
            buffer_size = strlen(line_buffer) + 1;
            g_string_free(acc, TRUE);
            if (continuation) free(continuation);
        }

        char **fields = NULL;
        int field_count = 0;
        char *line_start = line_buffer;
        char *field_data;
        while ((field_data = extract_next_field(&line_start)) != NULL) {
            char *cleaned_field_ptr = remove_quotes(field_data);
            fields = (char **)realloc(fields, (field_count + 1) * sizeof(char *));
            fields[field_count] = strdup(cleaned_field_ptr);
            field_count++;
            free(field_data);
            if (*line_start == '\0') break;
        }
        // Ajustar número de campos conforme o cabeçalho
        if (expected_fields > 0) {
            if (field_count < expected_fields) {
                // Null-terminate para validação e logging
                fields = (char **)realloc(fields, (field_count + 1) * sizeof(char *));
                fields[field_count] = NULL;
                log_invalid_entry(fields, field_count, csv_name, NULL, header_written);
                for (int i = 0; i < field_count; i++) free(fields[i]);
                free(fields);
                continue; // próxima linha
            } else if (field_count > expected_fields) {
                // Truncate campos a mais (liberta extras)
                for (int i = expected_fields; i < field_count; i++) {
                    free(fields[i]);
                }
                fields = (char **)realloc(fields, (expected_fields + 1) * sizeof(char *));
                field_count = expected_fields;
            }
        }

    // Null-terminate array 
    fields = (char **)realloc(fields, (field_count + 1) * sizeof(char *));
    fields[field_count] = NULL;
        
        int create_result = store_data(fields, data_catalog, program);
        if (create_result != 0) {
            log_invalid_entry(fields, field_count, csv_name, NULL, header_written);
        }

        for (int i = 0; i < field_count; i++) {
            free(fields[i]);
        }
        free(fields);
    }

    free(line_buffer);
    return 0;
}

char* remove_quotes(char* field){
    size_t len = strlen(field);
    
    // Tenta remover a aspa de fecho (se existir)
    if (len > 0 && field[len - 1] == '"') {
        field[len - 1] = '\0';
        len--; // Novo comprimento
    }

    // Tenta remover a aspa de abertura (se existir)
    if (len > 0 && field[0] == '"') {
        return field + 1; // Devolve o ponteiro para a 2ª posição
    }
    
    return field; // Devolve o ponteiro original se não houver aspas
}

void write_field_with_quotes(FILE *file, const char *field){
    fprintf(file, "\"%s\"", field);
}


void log_invalid_entry(char **fields, int field_count, const char *csv_name, const char *header,int header_written){
    const char *output_dir = "resultados/";

    size_t error_file_size = strlen(output_dir) + strlen(csv_name) + 12;  // 8 para "_errors" + \0
    
    char *error_file = (char *)malloc(error_file_size);
    
    strcpy(error_file, output_dir);
    strcat(error_file, csv_name);

    char *dot_position = strrchr(error_file, '.');
    if (dot_position != NULL && strcmp(dot_position, ".csv") == 0){
        strcpy(dot_position, "_errors.csv");
    } else {
        strcat(error_file, "_errors.csv");
    }

    FILE *file = fopen(error_file, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo de erros");
        free(error_file);
        return;
    }


    if (!header_written && header != NULL){
        fprintf(file, "%s\n", header);
        header_written = 1;
    }

    if (fields != NULL){
        for (int i = 0; i < field_count; i++){
            write_field_with_quotes(file, fields[i]);
            if (i < field_count - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    free(error_file);
}