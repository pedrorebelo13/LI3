#ifndef AERONAVE_H
#define AERONAVE_H


/**
 * @brief Estrutura opaca que representa uma aeronave.
 *
 * Os detalhes da estrutura Aeronave são definidos no ficheiro de implementação.
 */
typedef struct aeronave Aeronave;

#include "mainUtils.h"


/**
 * @brief Cria e armazena uma aeronave no catálogo.
 *
 * Recebe as informações da aeronave, o catálogo de aeronaves e o programa, cria a estrutura e armazena-a.
 *
 * @param aeronave_info Array de strings com informações da aeronave.
 * @param aeronaves_catalog Catálogo onde a aeronave será armazenada.
 * @param program Estrutura do programa principal.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int create_and_store_aeronave(char **aeronave_info, void *aeronaves_catalog, Program *program);

/**
 * @brief Liberta a memória associada a uma aeronave.
 *
 * @param aeronave_void Ponteiro genérico para a estrutura Aeronave.
 */
void free_aeronave(void *aeronave_void);


/**
 * @brief Obtém o identificador da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return String com o identificador da aeronave.
 */
char* get_aeronave_identifier(Aeronave* aeronave);
/**
 * @brief Define o identificador da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param identifier String com o novo identificador.
 */
void set_aeronave_identifier(Aeronave* aeronave, char* identifier);

/**
 * @brief Obtém o fabricante da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return String com o nome do fabricante.
 */
char* get_aeronave_manufacturer(Aeronave* aeronave);
/**
 * @brief Define o fabricante da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param manufacturer String com o novo nome do fabricante.
 */
void set_aeronave_manufacturer(Aeronave* aeronave, char* manufacturer);

/**
 * @brief Obtém o modelo da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return String com o modelo da aeronave.
 */
char* get_aeronave_model(Aeronave* aeronave);
/**
 * @brief Define o modelo da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param model String com o novo modelo.
 */
void set_aeronave_model(Aeronave* aeronave, char* model);

/**
 * @brief Obtém o ano de fabrico da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return Ano de fabrico.
 */
int get_aeronave_year(Aeronave* aeronave);
/**
 * @brief Define o ano de fabrico da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param year String com o novo ano de fabrico.
 */
void set_aeronave_year(Aeronave* aeronave, char* year);

/**
 * @brief Obtém a capacidade da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return Capacidade da aeronave.
 */
int get_aeronave_capacity(Aeronave* aeronave);
/**
 * @brief Define a capacidade da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param capacity String com a nova capacidade.
 */
void set_aeronave_capacity(Aeronave* aeronave, char* capacity);

/**
 * @brief Obtém o alcance da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @return Alcance da aeronave.
 */
int get_aeronave_range(Aeronave* aeronave);
/**
 * @brief Define o alcance da aeronave.
 * @param aeronave Ponteiro para a estrutura Aeronave.
 * @param range String com o novo alcance.
 */
void set_aeronave_range(Aeronave* aeronave, char* range);



#endif