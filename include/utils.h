#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include "aeronaves.h"
#include "voos.h"
#include "aeroportos.h"
#include "aeroporto.h"
#include "aeronave.h"
#include "voo.h"

/**
 * @brief Extrai dois identificadores de voo de uma string.
 *
 * Processa a string fids e extrai os dois identificadores de voo,
 * armazenando-os em fid1 e fid2.
 *
 * @param fids String com os identificadores dos voos.
 * @param fid1 Buffer para armazenar o primeiro identificador.
 * @param fid2 Buffer para armazenar o segundo identificador.
 */
void fids_Aux(char* fids, char* fid1, char* fid2);

/**
 * @brief Converte uma string para estrutura Data.
 * @param data_str String com a data.
 * @param hora Hora do dia.
 * @param minuto Minuto da hora.
 * @return Estrutura Data preenchida.
 */
Data string_to_data(char* data_str, int hora, int minuto);
/**
 * @brief Extrai duas datas de uma string de argumentos.
 * @param arguments String com os argumentos.
 * @param data1 Ponteiro para a primeira data.
 * @param data2 Ponteiro para a segunda data.
 */
void argumento_to_data(char *arguments, Data *data1, Data *data2);

/**
 * @brief Estrutura auxiliar para semana (Query 4).
 */
typedef struct semana Semana;
/**
 * @brief Obtém o número da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @return Número da semana.
 */

int get_semana_number(Semana* semana);
/**
 * @brief Define o número da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @param number Número da semana.
 */
void set_semana_number(Semana* semana, int number);
/**
 * @brief Obtém o top 10 de passageiros da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @return Array com o top 10.
 */
GPtrArray* get_semana_top10(Semana* semana);
/**
 * @brief Define o top 10 de passageiros da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @param top10 Array com o top 10.
 */
void set_semana_top10(Semana* semana, GPtrArray* top10);
/**
 * @brief Obtém as reservas da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @return Array de reservas.
 */
GPtrArray* get_semana_reservas(Semana* semana);
/**
 * @brief Define as reservas da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @param reservas Array de reservas.
 */
void set_semana_reservas(Semana* semana, GPtrArray* reservas);
/**
 * @brief Obtém a data de início da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @return Data de início.
 */
Data get_semana_inicio(Semana* semana);
/**
 * @brief Define a data de início da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @param inicio Data de início.
 */
void set_semana_inicio(Semana* semana, Data inicio);
/**
 * @brief Obtém a data de fim da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @return Data de fim.
 */
Data get_semana_fim(Semana* semana);
/**
 * @brief Define a data de fim da semana.
 * @param semana Ponteiro para a estrutura Semana.
 * @param fim Data de fim.
 */
void set_semana_fim(Semana* semana, Data fim);
/**
 * @brief Liberta a memória de uma estrutura Semana.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_semana(gpointer data);

/**
 * @brief Calcula o dia da semana.
 * @param d Data.
 * @return Dia da semana (0-6).
 */
int day_of_week(Data d);
/**
 * @brief Inicializa o array de semanas.
 * @param reservas_array Array de reservas.
 * @return Array de semanas.
 */
GPtrArray* semanasInicia(GPtrArray *reservas_array);
/**
 * @brief Distribui reservas por semanas.
 * @param semanas_array Array de semanas.
 * @param reservas_array Array de reservas.
 */
void reservas_por_semana(GPtrArray *semanas_array, GPtrArray *reservas_array);


void define_semanas(GPtrArray *semanas_array, Data data1, Data data2, int *semana_inicio, int *semana_fim);

/**
 * @brief Obtém o array de semanas do programa.
 * @param program Ponteiro para a estrutura principal do programa.
 * @return Array de semanas.
 */
GPtrArray *get_semanas_array(Program *program);


/**
 * @brief Arredonda valores para um número específico de casas decimais.
 * @param auxHash Hash table auxiliar.
 * @param casas Número de casas decimais.
 */
void casasDecimais(GHashTable *auxHash, int casas);

// Query 6 - Aeroportos mais frequentes por nacionalidade
/**
 * @brief Estrutura auxiliar para nacionalidades (Query 6).
 */
typedef struct nationalitys Nationalitys;
/**
 * @brief Define a nacionalidade.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @param nationality String com a nacionalidade.
 */
void set_nationalitys_nationality(Nationalitys* nationalitys, char* nationality);
/**
 * @brief Obtém a nacionalidade.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @return String com a nacionalidade.
 */
char* get_nationalitys_nationality(Nationalitys* nationalitys);
/**
 * @brief Define a tabela hash de aeroportos.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @param aeroportos Tabela hash de aeroportos.
 */
void set_nationalitys_aeroportos(Nationalitys* nationalitys, GHashTable* aeroportos);
/**
 * @brief Obtém a tabela hash de aeroportos.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @return Tabela hash de aeroportos.
 */
GHashTable* get_nationalitys_aeroportos(Nationalitys* nationalitys);
/**
 * @brief Define a contagem de passageiros.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @param passengerCount Número de passageiros.
 */
void set_nationalitys_passengerCount(Nationalitys* nationalitys, int passengerCount);
/**
 * @brief Obtém a contagem de passageiros.
 * @param nationalitys Ponteiro para a estrutura Nationalitys.
 * @return Número de passageiros.
 */
int get_nationalitys_passengerCount(Nationalitys* nationalitys);
/**
 * @brief Liberta a memória de uma estrutura Nationalitys.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_nationalitys(gpointer data);
/**
 * @brief Função auxiliar para processar a query 6.
 * @param program Ponteiro para a estrutura principal do programa.
 * @param auxHash Hash table auxiliar.
 */
void query6_aux(Program *program, GHashTable *auxHash);

#endif