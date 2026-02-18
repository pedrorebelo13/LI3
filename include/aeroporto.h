#ifndef AEROPORTO_H
#define AEROPORTO_H

#include "mainUtils.h"


/**
 * @brief Estrutura opaca que representa um aeroporto.
 *
 * Os detalhes da estrutura Aeroporto são definidos no ficheiro de implementação.
 */
typedef struct aeroporto Aeroporto;


/**
 * @brief Cria e armazena um aeroporto no catálogo.
 *
 * Recebe as informações do aeroporto, o catálogo de aeroportos e o programa, cria a estrutura e armazena-a.
 *
 * @param aeroporto_info Array de strings com informações do aeroporto.
 * @param aeroportos_catalog Catálogo onde o aeroporto será armazenado.
 * @param program Estrutura do programa principal.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int create_and_store_aeroporto(char **aeroporto_info, void *aeroportos_catalog, Program *program);

/**
 * @brief Liberta a memória associada a um aeroporto.
 *
 * @param aeroporto_void Ponteiro genérico para a estrutura Aeroporto.
 */
void free_aeroporto(void *aeroporto_void);


/**
 * @brief Obtém o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o código do aeroporto.
 */
char* get_aeroporto_code(Aeroporto* aeroporto);
/**
 * @brief Define o código do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param code String com o novo código.
 */
void set_aeroporto_code(Aeroporto* aeroporto, char* code);

/**
 * @brief Obtém o nome do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o nome do aeroporto.
 */
char* get_aeroporto_name(Aeroporto* aeroporto);
/**
 * @brief Define o nome do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param name String com o novo nome.
 */
void set_aeroporto_name(Aeroporto* aeroporto, char* name);

/**
 * @brief Obtém a cidade do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o nome da cidade.
 */
char* get_aeroporto_city(Aeroporto* aeroporto);
/**
 * @brief Define a cidade do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param city String com o novo nome da cidade.
 */
void set_aeroporto_city(Aeroporto* aeroporto, char* city);

/**
 * @brief Obtém o país do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o nome do país.
 */
char* get_aeroporto_country(Aeroporto* aeroporto);
/**
 * @brief Define o país do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param country String com o novo nome do país.
 */
void set_aeroporto_country(Aeroporto* aeroporto, char* country);

/**
 * @brief Obtém o código ICAO do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o código ICAO.
 */
char* get_aeroporto_icao(Aeroporto* aeroporto);
/**
 * @brief Define o código ICAO do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param icao String com o novo código ICAO.
 */
void set_aeroporto_icao(Aeroporto* aeroporto, char* icao);

/**
 * @brief Obtém o tipo do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return String com o tipo do aeroporto.
 */
char* get_aeroporto_type(Aeroporto* aeroporto);
/**
 * @brief Define o tipo do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param type String com o novo tipo.
 */
void set_aeroporto_type(Aeroporto* aeroporto, char* type);

/**
 * @brief Obtém a latitude do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return Latitude do aeroporto.
 */
float get_aeroporto_latitude(Aeroporto* aeroporto);
/**
 * @brief Define a latitude do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param latitude String com a nova latitude.
 */
void set_aeroporto_latitude(Aeroporto* aeroporto, char* latitude);

/**
 * @brief Obtém a longitude do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @return Longitude do aeroporto.
 */
float get_aeroporto_longitude(Aeroporto* aeroporto);
/**
 * @brief Define a longitude do aeroporto.
 * @param aeroporto Ponteiro para a estrutura Aeroporto.
 * @param longitude String com a nova longitude.
 */
void set_aeroporto_longitude(Aeroporto* aeroporto, char* longitude);


#endif