/**
 * Fichier : tube_3.c
 * Auteur : Nathan SALEZ
 * Description :
 * Le fils récupère les messages à envoyer à son père via une saisie de l’utilisateur,
 * et setermine quand le message envoyé est AU REVOIR.
 * Le père ne se terminera que lorsque le fils lui aura envoyé le message AU REVOIR.
 * Le père répond à son fils via un autre tube, en réaction au message reçu. Il envoie toujours la
 * même réponse, après une attente de 2 secondes;
 * Le fils attend d’avoir reçu la réponse de son père pour envoyer le message suivant.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

#define IN_PIPE 1

#define OUT_PIPE 0

#define CHAR_BUFFER_LENGTH 100

#define MESSAGE_PERE "Est-ce que ça m'intéresse ?"

int main(int argc,char * argv[])
{
    // descripteur[0] désigne la sortie du tube (dans laquelle on peut lire des données) ;
    // descripteur[1] désigne l’entrée du tube (dans laquelle on peut écrire des données) ;
    int descripteur_flux_fils_a_pere[2];
    int descripteur_flux_pere_a_fils[2];
    CHECK_ERROR(
            pipe(descripteur_flux_fils_a_pere),
            "Impossible de créer un pipe.\n"
            );
    CHECK_ERROR(
            pipe(descripteur_flux_pere_a_fils),
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
            fprintf(stdout,"Ecrivez un message à votre père : ");
            fgets(message,CHAR_BUFFER_LENGTH,stdin);
            if( strlen(message) >= 2) {
                message[strlen(message) - 1] = '\0';
                close(descripteur_flux_fils_a_pere[OUT_PIPE]);
                write(descripteur_flux_fils_a_pere[IN_PIPE], message, CHAR_BUFFER_LENGTH);
            }

            // le fils attend d'avoir la réponse du père.
            char message_from_father[CHAR_BUFFER_LENGTH];
            while( read(descripteur_flux_pere_a_fils[OUT_PIPE],message_from_father,CHAR_BUFFER_LENGTH) == 0);

            fprintf(stdout,"Message du père : '%s'\n",message_from_father);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        char message_from_son[CHAR_BUFFER_LENGTH]="";
        while( strcmp(message_from_son,"AU REVOIR") != 0 ) {
            close(descripteur_flux_fils_a_pere[IN_PIPE]);
            read(descripteur_flux_fils_a_pere[OUT_PIPE], message_from_son, CHAR_BUFFER_LENGTH);
            sleep(2);

            close( descripteur_flux_pere_a_fils[OUT_PIPE]);
            write( descripteur_flux_pere_a_fils[IN_PIPE],MESSAGE_PERE,CHAR_BUFFER_LENGTH);
        }
    }
    return EXIT_SUCCESS;
}
