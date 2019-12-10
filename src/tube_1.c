/**
 * Fichier : tube_1.c
 * Auteur : Nathan SALEZ
 * Description :
 * crée un tube, puis crée un fils,
 * attend la mort de son fils pour aller lire dans le tube.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include "utils.h"

#define IN_PIPE 1

#define OUT_PIPE 0

#define CHAR_BUFFER_LENGTH 100

int main(int argc,char * argv[])
{
    // descripteur[0] désigne la sortie du tube (dans laquelle on peut lire des données) ;
    // descripteur[1] désigne l’entrée du tube (dans laquelle on peut écrire des données) ;
    int pipe_descriptor[2];
    CHECK_ERROR(
            pipe(pipe_descriptor),
            "Impossible de créer un pipe.\n"
            );

    pid_t pid_courant;
    CHECK_ERROR(
            pid_courant = fork(),
            "Impossible de créer un processus fils.\n"
            );

    if( pid_courant == 0)
    {
        char * message = "Hello there";
        close( pipe_descriptor[OUT_PIPE]);
        write(pipe_descriptor[IN_PIPE],message,CHAR_BUFFER_LENGTH);
        exit(EXIT_SUCCESS);
    } else
    {
        CHECK_ERROR(
                waitpid(pid_courant,NULL,0),
                "CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n"
        );

        char message_from_son[CHAR_BUFFER_LENGTH];
        close( pipe_descriptor[IN_PIPE]);
        read(pipe_descriptor[OUT_PIPE],message_from_son,CHAR_BUFFER_LENGTH);

        fprintf(stdout,"Message provenant du fils : '%s'\n",message_from_son);
    }
    return EXIT_SUCCESS;
}
