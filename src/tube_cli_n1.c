/**
 * Fichier : tube_srv_n1.c
 * Auteur : Nathan SALEZ
 * Description : réalise les opérations suivantes :
 * Ouverture d'un tube nommé en lecture
 * Lecture d’un message dans le tube nommé
 * Affiche le message sur la sortie standard
 * Fin du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#include "tube_utils.h"
#include "utils.h"

int main(int argc,char * argv[])
{

    int descripteur_pipe_lecture = 0;
    CHECK_ERROR(
            descripteur_pipe_lecture = open(PATH_PIPE,O_RDONLY),
            "Impossible d'ouvrir le tube nommé.\n"
    );

    char message[CHAR_BUFFER_LENGTH] = "";

    read(descripteur_pipe_lecture,message,CHAR_BUFFER_LENGTH);
    fprintf(stdout,"Client - message reçu : '%s'\n",message);

    close(descripteur_pipe_lecture);
    fprintf(stdout,"Client - fermeture du programme..\n");
    return EXIT_SUCCESS;
}
