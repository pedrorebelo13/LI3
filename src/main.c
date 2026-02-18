#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include "parser.h"
#include "queries.h"
#include "validacao.h"
#include "utils.h"

#include "aeroportos.h"
#include "aeronaves.h"
#include "voos.h"
#include "passageiros.h"
#include "reservas.h"
#include "mainUtils.h"
#include "main.h"

int main(int argc,char** argv)
{
    // Verifica se o número de argumentos é válido
    if (argc != 3)
    {
        printf("Numero de argumentos inválido. Recebe <caminho-dataset> <ficheiro-input>\n");
        return 1;
    }

    // Cria pasta de output caso não exista
    g_mkdir_with_parents(OUTPUT_NAME, 0700);

    Program *program = initialize_program(argv);
    if (program == NULL) {
        fprintf(stderr, "Falha na inicialização do programa.\n");
        return 1;
    }

    run_program(program, argv);

    free_program(program);

    return 0;
}