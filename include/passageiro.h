#ifndef PASSAGEIRO_H
#define PASSAGEIRO_H


/**
 * @brief Estrutura opaca que representa um passageiro.
 *
 * Os detalhes da estrutura Passageiro são definidos no ficheiro de implementação.
 */
typedef struct passageiro Passageiro;

/**
 * @brief Enumeração do género do passageiro.
 *
 * Define os géneros possíveis: masculino, feminino ou outro.
 */
typedef enum {
    M = 0,  /**< Masculino */
    F = 1,  /**< Feminino */
    O = 2   /**< Outro */
} Gender;

#include <voo.h>
#include "mainUtils.h"


/**
 * @brief Cria e armazena um passageiro no catálogo.
 *
 * Recebe as informações do passageiro, o catálogo de passageiros e o programa, cria a estrutura e armazena-a.
 *
 * @param passageiro_info Array de strings com informações do passageiro.
 * @param passageiros_catalog Catálogo onde o passageiro será armazenado.
 * @param program Estrutura do programa principal.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int create_and_store_passageiro(char **passageiro_info, void *passageiros_catalog, Program *program);

/**
 * @brief Liberta a memória associada a um passageiro.
 *
 * @param passageiro_void Ponteiro genérico para a estrutura Passageiro.
 */
void free_passageiro(void *passageiro_void);

/**
 * @brief Obtém o número do documento do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com o número do documento.
 */
char* get_passageiro_docNumber(Passageiro* passageiro);
/**
 * @brief Define o número do documento do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param docNumber String com o novo número do documento.
 */
void set_passageiro_docNumber(Passageiro* passageiro, char* docNumber);

/**
 * @brief Obtém o primeiro nome do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com o primeiro nome.
 */
char* get_passageiro_firstName(Passageiro* passageiro);
/**
 * @brief Define o primeiro nome do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param firstName String com o novo primeiro nome.
 */
void set_passageiro_firstName(Passageiro* passageiro, char* firstName);

/**
 * @brief Obtém o último nome do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com o último nome.
 */
char* get_passageiro_lastName(Passageiro* passageiro);
/**
 * @brief Define o último nome do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param lastName String com o novo último nome.
 */
void set_passageiro_lastName(Passageiro* passageiro, char* lastName);

/**
 * @brief Obtém a data de nascimento do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return Estrutura Data com a data de nascimento.
 */
Data get_passageiro_dob(Passageiro* passageiro);
/**
 * @brief Define a data de nascimento do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param dob_str String com a nova data de nascimento.
 */
void set_passageiro_dob(Passageiro* passageiro, char* dob_str);

/**
 * @brief Obtém a nacionalidade do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com a nacionalidade.
 */
char* get_passageiro_nationality(Passageiro* passageiro);
/**
 * @brief Define a nacionalidade do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param nationality String com a nova nacionalidade.
 */
void set_passageiro_nationality(Passageiro* passageiro, char* nationality);

/**
 * @brief Obtém o género do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return Enum Gender com o género.
 */
Gender get_passageiro_gender(Passageiro* passageiro);
/**
 * @brief Define o género do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param gender_str String com o novo género.
 */
void set_passageiro_gender(Passageiro* passageiro, char* gender_str);

/**
 * @brief Obtém o email do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com o email.
 */
char* get_passageiro_email(Passageiro* passageiro);
/**
 * @brief Define o email do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param email String com o novo email.
 */
void set_passageiro_email(Passageiro* passageiro, char* email);

/**
 * @brief Obtém o número de telefone do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com o número de telefone.
 */
char* get_passageiro_phone(Passageiro* passageiro);
/**
 * @brief Define o número de telefone do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param phone String com o novo número de telefone.
 */
void set_passageiro_phone(Passageiro* passageiro, char* phone);

/**
 * @brief Obtém a morada do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com a morada.
 */
char* get_passageiro_address(Passageiro* passageiro);
/**
 * @brief Define a morada do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param address String com a nova morada.
 */
void set_passageiro_address(Passageiro* passageiro, char* address);

/**
 * @brief Obtém a URL da foto do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @return String com a URL da foto.
 */
char* get_passageiro_photoURL(Passageiro* passageiro);
/**
 * @brief Define a URL da foto do passageiro.
 * @param passageiro Ponteiro para a estrutura Passageiro.
 * @param photoURL String com a nova URL da foto.
 */
void set_passageiro_photoURL(Passageiro* passageiro, char* photoURL);


#endif


