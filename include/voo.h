#ifndef VOO_H
#define VOO_H


/**
 * @brief Estrutura opaca que representa um voo.
 *
 * Os detalhes da estrutura Voo são definidos no ficheiro de implementação.
 */
typedef struct voo Voo;

/**
 * @brief Estrutura que representa uma data e hora.
 *
 * Utilizada para armazenar ano, mês, dia, hora e minuto de eventos relacionados ao voo.
 */
typedef struct {
    int ano;      
    int mes;     
    int dia;     
    int hora;    
    int minuto;  
} Data;

/**
 * @brief Enumeração do estado do voo.
 *
 * Indica se o voo está a horas, atrasado ou cancelado.
 */
typedef enum {
    ON_TIME = 0,    
    DELAYED = 1,    
    CANCELLED = 2   
} Status;

#include "mainUtils.h"



/**
 * @brief Cria e armazena um voo no catálogo.
 *
 * Recebe as informações do voo, o catálogo de voos e o programa, cria a estrutura e armazena-a.
 *
 * @param voo_info Array de strings com informações do voo.
 * @param voos_catalog Catálogo onde o voo será armazenado.
 * @param program Estrutura do programa principal.
 * @return 0 em caso de sucesso, outro valor em caso de erro.
 */
int create_and_store_voo(char **voo_info, void *voos_catalog, Program *program);

/**
 * @brief Liberta a memória associada a um voo.
 *
 * @param voo_void Ponteiro genérico para a estrutura Voo.
 */
void free_voo(void *voo_void);


/**
 * @brief Obtém o identificador do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com o identificador do voo.
 */
char* get_voo_id(Voo* voo);
/**
 * @brief Define o identificador do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param id String com o novo identificador.
 */
void set_voo_id(Voo* voo, char* id);

/**
 * @brief Obtém a data prevista de descolagem.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Estrutura Data com a data prevista de descolagem.
 */
Data get_voo_descolar(Voo* voo);
/**
 * @brief Define a data prevista de descolagem.
 * @param voo Ponteiro para a estrutura Voo.
 * @param descolar String com a nova data prevista.
 */
void set_voo_descolar(Voo* voo, char* descolar);

/**
 * @brief Obtém a data real de descolagem.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Estrutura Data com a data real de descolagem.
 */
Data get_voo_descolou(Voo* voo);
/**
 * @brief Define a data real de descolagem.
 * @param voo Ponteiro para a estrutura Voo.
 * @param descolou String com a nova data real.
 */
void set_voo_descolou(Voo* voo, char* descolou);

/**
 * @brief Obtém a data prevista de aterragem.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Estrutura Data com a data prevista de aterragem.
 */
Data get_voo_aterrar(Voo* voo);
/**
 * @brief Define a data prevista de aterragem.
 * @param voo Ponteiro para a estrutura Voo.
 * @param aterrar String com a nova data prevista.
 */
void set_voo_aterrar(Voo* voo, char* aterrar);

/**
 * @brief Obtém a data real de aterragem.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Estrutura Data com a data real de aterragem.
 */
Data get_voo_aterrou(Voo* voo);
/**
 * @brief Define a data real de aterragem.
 * @param voo Ponteiro para a estrutura Voo.
 * @param aterrou String com a nova data real.
 */
void set_voo_aterrou(Voo* voo, char* aterrou);

/**
 * @brief Obtém o número do gate do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Número do gate.
 */
int get_voo_gate(Voo* voo);
/**
 * @brief Define o número do gate do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param gate String com o novo número do gate.
 */
void set_voo_gate(Voo* voo, char* gate);

/**
 * @brief Obtém o aeroporto de origem do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com o código do aeroporto de origem.
 */
char* get_voo_origem(Voo* voo);
/**
 * @brief Define o aeroporto de origem do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param origem String com o novo código de origem.
 */
void set_voo_origem(Voo* voo, char* origem);

/**
 * @brief Obtém o aeroporto de destino do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com o código do aeroporto de destino.
 */
char* get_voo_destino(Voo* voo);
/**
 * @brief Define o aeroporto de destino do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param destino String com o novo código de destino.
 */
void set_voo_destino(Voo* voo, char* destino);

/**
 * @brief Obtém o modelo da aeronave do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com o modelo da aeronave.
 */
char* get_voo_aircraft(Voo* voo);
/**
 * @brief Define o modelo da aeronave do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param aircraft String com o novo modelo da aeronave.
 */
void set_voo_aircraft(Voo* voo, char* aircraft);

/**
 * @brief Obtém a companhia aérea do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com o nome da companhia aérea.
 */
char* get_voo_airline(Voo* voo);
/**
 * @brief Define a companhia aérea do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param airline String com o novo nome da companhia aérea.
 */
void set_voo_airline(Voo* voo, char* airline);

/**
 * @brief Obtém a URL de informação do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return String com a URL.
 */
char* get_voo_URL(Voo* voo);
/**
 * @brief Define a URL de informação do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param URL String com a nova URL.
 */
void set_voo_URL(Voo* voo, char* URL);

/**
 * @brief Obtém o estado do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @return Enum Status do voo.
 */
Status get_voo_status(Voo* voo);
/**
 * @brief Define o estado do voo.
 * @param voo Ponteiro para a estrutura Voo.
 * @param status String com o novo estado.
 */
void set_voo_status(Voo* voo, char* status);


#endif