/**
 * Fichier : tube_srv_n2.c
 * Auteur : Nathan SALEZ
 * Description : réalise les opérations suivantes :
 * Création d’un tube nommé
 * Ouverture du tube nommé en lecture
 * Lecture des messages des clients
 * Fin du programme lorsque le serveur reçoit le message "PAUSE"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "tube_utils.h"
#include "utils.h"

int main(int argc,char * argv[])
{
    CHECK_ERROR(
            mkfifo(PATH_PIPE_2,0600),
            "Impossible de créer un tube nommé.\n"
    );
    fprintf(stdout,"Serveur - tube '%s' nommé créé.\n",PATH_PIPE_2);

    int descripteur_pipe_lecture = 0;

    char message[CHAR_BUFFER_LENGTH] = "";
    do
    {
        // A chaque itération, on ouvre le tube pendant une courte durée
        // afin de laisser les clients écrire leur message
        CHECK_ERROR(
                descripteur_pipe_lecture = open(PATH_PIPE_2,O_RDONLY),
                "Impossible d'ouvrir le tube nommé.\n"
        );
        fprintf(stdout,"Serveur - ouverture du tube nommé '%s' en lecture.\n",PATH_PIPE_2);
        CHECK_ERROR(
                read(descripteur_pipe_lecture,message,CHAR_BUFFER_LENGTH),
                "Impossible de lire dans le tube nommé.\n"
                );
        close(descripteur_pipe_lecture);
        fprintf(stdout,"Serveur - message reçu '%s'\n",message);
        fprintf(stdout,"Serveur - fermeture du tube nommé '%s' en lecture.\n\n",PATH_PIPE_2);
    }while( strcmp("PAUSE",message) != 0);

    CHECK_ERROR(
            remove(PATH_PIPE_2),
            "Impossible de supprimer le tube nommé.\n"
    );
    fprintf(stdout,"Serveur - fichier '%s' supprimé.\n",PATH_PIPE_2);
    return EXIT_SUCCESS;
}
