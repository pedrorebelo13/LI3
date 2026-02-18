#ifndef RESERVA_H
#define RESERVA_H


/**
 * @brief Estrutura opaca que representa uma reserva.
 *
 * Os detalhes da estrutura Reserva são definidos no ficheiro de implementação.
 */
typedef struct reserva Reserva;


#include "mainUtils.h"


/**
 * @brief Cria e armazena uma reserva no catálogo.
 *
 * Recebe as informações da reserva, o catálogo de reservas e o programa, cria a estrutura e armazena-a.
 *
 * @param reserva_info Array de strings com informações da reserva.
 * @param reservas_catalog Catálogo onde a reserva será armazenada.
 * @param program Estrutura do programa principal.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int create_and_store_reserva(char **reserva_info, void *reservas_catalog, Program *program);

/**
 * @brief Liberta a memória associada a uma reserva.
 *
 * @param reserva_void Ponteiro genérico para a estrutura Reserva.
 */
void free_reserva(void *reserva_void);

/**
 * @brief Obtém o identificador da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return String com o identificador da reserva.
 */
char* get_reserva_rid(Reserva* reserva);
/**
 * @brief Define o identificador da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param rid String com o novo identificador.
 */
void set_reserva_rid(Reserva* reserva, char* rid);

/**
 * @brief Obtém os identificadores dos voos associados à reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return String com os identificadores dos voos.
 */
char* get_reserva_fids(Reserva* reserva);
/**
 * @brief Define os identificadores dos voos associados à reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param fids String com os novos identificadores dos voos.
 */
void set_reserva_fids(Reserva* reserva, char* fids);

/**
 * @brief Obtém o número de documento do passageiro.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return String com o número de documento.
 */
char* get_reserva_pdocNumber(Reserva* reserva);
/**
 * @brief Define o número de documento do passageiro.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param pdocNumber String com o novo número de documento.
 */
void set_reserva_pdocNumber(Reserva* reserva, char* pdocNumber);

/**
 * @brief Obtém o preço da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return Preço da reserva.
 */
double get_reserva_price(Reserva* reserva);
/**
 * @brief Define o preço da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param price String com o novo preço.
 */
void set_reserva_price(Reserva* reserva, char* price);

/**
 * @brief Obtém a flag de bagagem extra.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return 1 se tem bagagem extra, 0 caso contrário.
 */
int get_reserva_extraLuggage(Reserva* reserva);
/**
 * @brief Define a flag de bagagem extra.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param extraLuggage String com a nova flag.
 */
void set_reserva_extraLuggage(Reserva* reserva, char* extraLuggage);

/**
 * @brief Obtém a flag de embarque prioritário.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return 1 se tem embarque prioritário, 0 caso contrário.
 */
int get_reserva_priorityBoarding(Reserva* reserva);
/**
 * @brief Define a flag de embarque prioritário.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param priorityBoarding String com a nova flag.
 */
void set_reserva_priorityBoarding(Reserva* reserva, char* priorityBoarding);

/**
 * @brief Obtém o lugar atribuído na reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return String com o lugar.
 */
char* get_reserva_seat(Reserva* reserva);
/**
 * @brief Define o lugar atribuído na reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param seat String com o novo lugar.
 */
void set_reserva_seat(Reserva* reserva, char* seat);

/**
 * @brief Obtém o código QR da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @return String com o código QR.
 */
char* get_reserva_qrCode(Reserva* reserva);
/**
 * @brief Define o código QR da reserva.
 * @param reserva Ponteiro para a estrutura Reserva.
 * @param qrCode String com o novo código QR.
 */
void set_reserva_qrCode(Reserva* reserva, char* qrCode);


#endif