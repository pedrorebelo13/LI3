#include <stdlib.h>
#include <stdio.h>
#include "passageiros.h"
#include "passageiro.h"
#include <voo.h>
#include "validacao.h"
#include "main.h"
#include "mainUtils.h"



struct passageiro 
{
    char* docNumber;
    char* firstName;
    char* lastName;
    Data dob;
    char* nationality;
    Gender gender;
    char* email;
    char* phone;
    char* address;
    char* photoURL;

};

int create_and_store_passageiro(char **passageiro_info, void *passageiros_catalog, Program *program) {

    if (!valida_passageiro(passageiro_info, 10, program)) {
        return 1;
    }
    
    Passageiro* passageiro = (Passageiro*)malloc(sizeof(Passageiro));

    set_passageiro_docNumber(passageiro, passageiro_info[0]);
    set_passageiro_firstName(passageiro, passageiro_info[1]);
    set_passageiro_lastName(passageiro, passageiro_info[2]);
    set_passageiro_dob(passageiro, passageiro_info[3]);
    set_passageiro_nationality(passageiro, passageiro_info[4]);
    set_passageiro_gender(passageiro, passageiro_info[5]);
    //set_passageiro_email(passageiro, passageiro_info[6]);
    //set_passageiro_phone(passageiro, passageiro_info[7]);
    //set_passageiro_address(passageiro, passageiro_info[8]);
    //set_passageiro_photoURL(passageiro, passageiro_info[9]);
    

    insert_passageiro((CatalogPassageiro*)passageiros_catalog, passageiro);
    return 0;
}

void free_passageiro(void *passageiro_void) {
    Passageiro* passageiro = (Passageiro*)passageiro_void;
    free(passageiro->docNumber);
    free(passageiro->firstName);
    free(passageiro->lastName);
    free(passageiro->nationality);
    //free(passageiro->email);
    //free(passageiro->phone);
    //free(passageiro->address);
    //free(passageiro->photoURL);
    free(passageiro);
}


//docNumber
char* get_passageiro_docNumber(Passageiro* passageiro) {
    return strdup(passageiro->docNumber);
}

void set_passageiro_docNumber(Passageiro* passageiro, char* docNumber) {
    passageiro->docNumber = strdup(docNumber);
}



//firstName
char* get_passageiro_firstName(Passageiro* passageiro) {
    return strdup(passageiro->firstName);
}   

void set_passageiro_firstName(Passageiro* passageiro, char* firstName) {
    passageiro->firstName = strdup(firstName);
}



//lastName
char* get_passageiro_lastName(Passageiro* passageiro) {
    return strdup(passageiro->lastName);
}

void set_passageiro_lastName(Passageiro* passageiro, char* lastName) {
    passageiro->lastName = strdup(lastName);
}



//dob
Data get_passageiro_dob(Passageiro* passageiro) {
    return passageiro->dob;
}

void set_passageiro_dob(Passageiro* passageiro, char* dob_str) {
    sscanf(dob_str, "%d-%d-%d", &passageiro->dob.ano, &passageiro->dob.mes, &passageiro->dob.dia);
}



//nationality
char* get_passageiro_nationality(Passageiro* passageiro) {
    return strdup(passageiro->nationality);
}  

void set_passageiro_nationality(Passageiro* passageiro, char* nationality) {
    passageiro->nationality = strdup(nationality);
}



//gender
Gender get_passageiro_gender(Passageiro* passageiro) {
    return passageiro->gender;
}   

void set_passageiro_gender(Passageiro* passageiro, char* gender_str) {
    if (gender_str[0] == 'M')
        passageiro->gender = M;
    else if (gender_str[0] == 'F')
        passageiro->gender = F;
    else
        passageiro->gender = O;
}  



//email
char* get_passageiro_email(Passageiro* passageiro) {
    return strdup(passageiro->email);
}

void set_passageiro_email(Passageiro* passageiro, char* email) {
    passageiro->email = strdup(email);
}



//phone
char* get_passageiro_phone(Passageiro* passageiro) {
    return strdup(passageiro->phone);
}   

void set_passageiro_phone(Passageiro* passageiro, char* phone) {
    passageiro->phone = strdup(phone);
}



//address
char* get_passageiro_address(Passageiro* passageiro) {
    return strdup(passageiro->address);
}

void set_passageiro_address(Passageiro* passageiro, char* address) {
    passageiro->address = strdup(address);
}



//photoURL
char* get_passageiro_photoURL(Passageiro* passageiro) {
    return strdup(passageiro->photoURL);
}

void set_passageiro_photoURL(Passageiro* passageiro, char* photoURL) {
    if (photoURL == NULL) passageiro->photoURL = strdup("PHOTO"); 
    else {
    passageiro->photoURL = strdup(photoURL);}
}
