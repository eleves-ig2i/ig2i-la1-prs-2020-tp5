/**
 * Fichier : tube_srv_n2.c
 * Auteur : Nathan SALEZ
 * Description : réalise les opérations suivantes :
 * Lecture
 * Ecriture d’un message dans le tube nommé
 * Fin du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>

#include "tube_utils.h"
#include "utils.h"

int main(int argc,char * argv[])
{

    int descripteur_pipe_ecriture = 0;

    char message[CHAR_BUFFER_LENGTH] = "";
    do
    {
        CHECK_ERROR(
                descripteur_pipe_ecriture = open(PATH_PIPE_2, O_WRONLY),
                "Impossible d'ouvrir le tube nommé.\n"
        );
        fgets(message,CHAR_BUFFER_LENGTH,stdin);
        message[strlen(message)-1]='\0';
        CHECK_ERROR(
                write(descripteur_pipe_ecriture, message, CHAR_BUFFER_LENGTH),
                "Impossible d'écrire dans le tube nommé.\n"
        );
        close(descripteur_pipe_ecriture);
        fprintf(stdout,"Client - message envoyé au serveur : '%s'\n",message);
    }while( strcmp("PAUSE",message) != 0);

    fprintf(stdout,"Client - fermeture du programme..\n");
    return EXIT_SUCCESS;
}
