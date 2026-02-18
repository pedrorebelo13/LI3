#ifndef MAINUTILS_H
#define MAINUTILS_H

#include "main.h"
#include "mainUtils.h"


/**
 * @brief Estrutura opaca que representa o catálogo de aeroportos.
 */
typedef struct catalogAeroporto CatalogAeroporto;
/**
 * @brief Estrutura opaca que representa o catálogo de aeronaves.
 */
typedef struct catalogAeronave CatalogAeronave;
/**
 * @brief Estrutura opaca que representa o catálogo de voos.
 */
typedef struct catalogVoo CatalogVoo;
/**
 * @brief Estrutura opaca que representa o catálogo de passageiros.
 */
typedef struct catalogPassageiro CatalogPassageiro;
/**
 * @brief Estrutura opaca que representa o catálogo de reservas.
 */
typedef struct catalogReserva CatalogReserva;

/**
 * @brief Estrutura principal do programa.
 */
typedef struct program Program;


/**
 * @brief Inicializa a estrutura principal do programa.
 * @param arguments Argumentos recebidos pelo programa.
 * @return Ponteiro para a estrutura Program inicializada.
 */
Program* initialize_program(char **arguments);
/**
 * @brief Executa o programa principal.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 */
void run_program(Program *program, char **arguments);
/**
 * @brief Liberta a memória associada à estrutura principal do programa.
 * @param program Ponteiro para a estrutura Program.
 */
void free_program(Program *program);
/**
 * @brief Inicializa o catálogo de aeroportos.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int inicializa_aeroportos(Program *program, char **arguments);
/**
 * @brief Inicializa o catálogo de aeronaves.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 * @param aeroportos_catalog Ponteiro para o catálogo de aeroportos.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int inicializa_aeronaves(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog);
/**
 * @brief Inicializa o catálogo de voos.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 * @param aeroportos_catalog Ponteiro para o catálogo de aeroportos.
 * @param aeronaves_catalog Ponteiro para o catálogo de aeronaves.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int inicializa_voos(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog);
/**
 * @brief Inicializa o catálogo de passageiros.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 * @param aeroportos_catalog Ponteiro para o catálogo de aeroportos.
 * @param aeronaves_catalog Ponteiro para o catálogo de aeronaves.
 * @param voos_catalog Ponteiro para o catálogo de voos.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int inicializa_passageiros(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog, CatalogVoo *voos_catalog);
/**
 * @brief Inicializa o catálogo de reservas.
 * @param program Ponteiro para a estrutura Program.
 * @param arguments Argumentos recebidos pelo programa.
 * @param aeroportos_catalog Ponteiro para o catálogo de aeroportos.
 * @param aeronaves_catalog Ponteiro para o catálogo de aeronaves.
 * @param voos_catalog Ponteiro para o catálogo de voos.
 * @param passageiros_catalog Ponteiro para o catálogo de passageiros.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int inicializa_reservas(Program *program, char **arguments, CatalogAeroporto *aeroportos_catalog, CatalogAeronave *aeronaves_catalog, CatalogVoo *voos_catalog, CatalogPassageiro *passageiros_catalog);

#include "aeronaves.h"
#include "voos.h"
#include "aeroportos.h"
#include "passageiros.h"
#include "reservas.h"


/**
 * @brief Obtém o catálogo de aeronaves do programa.
 * @param program Ponteiro para a estrutura Program.
 * @return Ponteiro para o catálogo de aeronaves.
 */
CatalogAeronave *get_aeronaves_catalog(Program *program);
/**
 * @brief Obtém o catálogo de aeroportos do programa.
 * @param program Ponteiro para a estrutura Program.
 * @return Ponteiro para o catálogo de aeroportos.
 */
CatalogAeroporto *get_aeroportos_catalog(Program *program);
/**
 * @brief Obtém o catálogo de voos do programa.
 * @param program Ponteiro para a estrutura Program.
 * @return Ponteiro para o catálogo de voos.
 */
CatalogVoo *get_voos_catalog(Program *program);
/**
 * @brief Obtém o catálogo de passageiros do programa.
 * @param program Ponteiro para a estrutura Program.
 * @return Ponteiro para o catálogo de passageiros.
 */
CatalogPassageiro *get_passageiros_catalog(Program *program);
/**
 * @brief Obtém o catálogo de reservas do programa.
 * @param program Ponteiro para a estrutura Program.
 * @return Ponteiro para o catálogo de reservas.
 */
CatalogReserva *get_reservas_catalog(Program *program);
/**
 * @brief Obtém o array de aeronaves ordenadas.
 * @param program Ponteiro para a estrutura Program.
 * @return Array de ponteiros para aeronaves (GPtrArray).
 */
GPtrArray *get_aeronaves_sorted_array(Program *program);
/**
 * @brief Obtém a hash table de aeroportos.
 * @param program Ponteiro para a estrutura Program.
 * @return Hash table de aeroportos (GHashTable).
 */
GHashTable *get_aeroporto_hash(Program *program);
/**
 * @brief Obtém o array auxiliar de companhias aéreas.
 * @param program Ponteiro para a estrutura Program.
 * @return Array auxiliar de companhias aéreas (GPtrArray).
 */
GPtrArray *get_airline_aux_array(Program *program);
/**
 * @brief Obtém a hash table de nacionalidades.
 * @param program Ponteiro para a estrutura Program.
 * @return Hash table de nacionalidades (GHashTable).
 */
GHashTable *get_nationalitys_hash(Program *program);

/* Setters for auxiliary structures (for interactive program setup) */

/**
 * @brief Define a hash table de aeroportos no programa.
 * @param program Ponteiro para a estrutura Program.
 * @param hash Hash table de aeroportos.
 */
void set_aeroporto_hash(Program *program, GHashTable *hash);
/**
 * @brief Define o array de aeronaves ordenadas no programa.
 * @param program Ponteiro para a estrutura Program.
 * @param array Array de aeronaves ordenadas.
 */
void set_aeronaves_sorted_array(Program *program, GPtrArray *array);
/**
 * @brief Define o array auxiliar de companhias aéreas no programa.
 * @param program Ponteiro para a estrutura Program.
 * @param array Array auxiliar de companhias aéreas.
 */
void set_airline_aux_array(Program *program, GPtrArray *array);
/**
 * @brief Define a hash table de nacionalidades no programa.
 * @param program Ponteiro para a estrutura Program.
 * @param hash Hash table de nacionalidades.
 */
void set_nationalitys_hash(Program *program, GHashTable *hash);
/**
 * @brief Define o array de semanas no programa.
 * @param program Ponteiro para a estrutura Program.
 * @param array Array de semanas.
 */
void set_semanas_array(Program *program, GPtrArray *array);

#endif