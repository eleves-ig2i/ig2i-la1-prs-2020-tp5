/**
 * Fichier : tube_2.c
 * Auteur : Nathan SALEZ
 * Description :
 * Le fils récupère les messages à envoyer à son père via une saisie de l’utilisateur,
 * et setermine quand le message envoyé est AU REVOIR.
 * Le père ne se terminera que lorsque le fils lui aura envoyé le message AU REVOIR.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
        char message[CHAR_BUFFER_LENGTH] = "";
        while( strcmp(message,"AU REVOIR") != 0)
        {
            fgets(message,CHAR_BUFFER_LENGTH,stdin);
            if( strlen(message) >= 2) {
                message[strlen(message) - 1] = '\0';
                close(pipe_descriptor[OUT_PIPE]);
                write(pipe_descriptor[IN_PIPE], message, CHAR_BUFFER_LENGTH);
            }
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        char message_from_son[CHAR_BUFFER_LENGTH]="";
        while( strcmp(message_from_son,"AU REVOIR") != 0 ) {
            close(pipe_descriptor[IN_PIPE]);
            read(pipe_descriptor[OUT_PIPE], message_from_son, CHAR_BUFFER_LENGTH);
            fprintf(stdout, "Message provenant du fils : '%s'\n", message_from_son);
        }
    }
    return EXIT_SUCCESS;
}
