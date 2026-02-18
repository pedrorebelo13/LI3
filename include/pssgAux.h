#ifndef PSSGAUX_H
#define PSSGAUX_H

/**
 * @brief Estrutura auxiliar para passageiro (Query 4).
 */
typedef struct passageiroAux PassageiroAux;
/**
 * @brief Obtém o número de documento do passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @return String com o número de documento.
 */
char* get_passageiroAux_docNumber(PassageiroAux* passageiro);
/**
 * @brief Define o número de documento do passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @param docNumber String com o número de documento.
 */
void set_passageiroAux_docNumber(PassageiroAux* passageiro, char* docNumber);
/**
 * @brief Obtém o total gasto pelo passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @return Total gasto.
 */
double get_passageiroAux_priceTotal(PassageiroAux* passageiro);
/**
 * @brief Define o total gasto pelo passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @param priceTotal Total gasto.
 */
void set_passageiroAux_priceTotal(PassageiroAux* passageiro, double priceTotal);
/**
 * @brief Obtém o contador de reservas do passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @return Contador de reservas.
 */
int get_passageiroAux_counter(PassageiroAux* passageiro);
/**
 * @brief Define o contador de reservas do passageiro.
 * @param passageiro Ponteiro para a estrutura PassageiroAux.
 * @param counter Contador de reservas.
 */
void set_passageiroAux_counter(PassageiroAux* passageiro, int counter);
/**
 * @brief Liberta a memória de uma estrutura PassageiroAux.
 * @param data Ponteiro genérico para a estrutura.
 */
void free_passageiroAux(gpointer data);

/**
 * @brief Calcula o top 10 de reservas por semana.
 * @param semanas_array Array de semanas.
 */
void top10_passageiros_por_semana(GPtrArray *semanas_array);
/**
 * @brief Função de comparação para ordenar passageiros por total gasto.
 * @param a Primeiro elemento.
 * @param b Segundo elemento.
 * @return Valor negativo se a < b, positivo se a > b, zero se iguais.
 */
int compare_passageiros_by_priceTotal(gconstpointer a, gconstpointer b);
/**
 * @brief Define as semanas dentro de um intervalo de datas.
 * @param semanas_array Array de semanas.
 * @param data1 Data inicial.
 * @param data2 Data final.
 * @param semana_inicio Ponteiro para o índice da semana inicial.
 * @param semana_fim Ponteiro para o índice da semana final.
 */

 /**
 * @brief Obtém o passageiro com mais gastos num intervalo de semanas.
 * @param semanas_array Array de semanas.
 * @param semana_inicio Índice da semana inicial.
 * @param semana_fim Índice da semana final.
 * @param result_counter Ponteiro para o contador resultado.
 * @param result_docNumber Ponteiro para o número de documento resultado.
 */
void get_passageiro_q4(GPtrArray *semanas_array, int semana_inicio, int semana_fim, int *result_counter, char **result_docNumber);

#endif