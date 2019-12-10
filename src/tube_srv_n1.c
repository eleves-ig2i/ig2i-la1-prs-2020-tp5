/**
 * Fichier : tube_srv_n1.c
 * Auteur : Nathan SALEZ
 * Description : réalise les opérations suivantes :
 * ● Création d’un tube nommé
 * ● Ecriture d’un message dans le tube nommé
 * ● attente de quelques secondes (pour avoir le temps de lancer l’exécution du client)
 * ● Fin du programme
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "tube_utils.h"
#include "utils.h"

int main(int argc,char * argv[])
{
    CHECK_ERROR(
            mkfifo(PATH_PIPE,0600),
            "Impossible de créer un tube nommé.\n"
    );
    fprintf(stdout,"Serveur - tube nommé créé.\n");

    int descripteur_pipe_ecriture = 0;
    CHECK_ERROR(
            descripteur_pipe_ecriture = open(PATH_PIPE,O_RDWR | O_NONBLOCK),
            // Remarque : l'appel de open() avec l'attribut WR_ONLY est bloquant sans l'attribut O_NONBLOCK
            // Il faut que le tube nommé soit ouvert en lecture avant d'être ouvert en écriture.
            // Lorsque ce n'est pas le cas, et qu'on ajoute l'attribut O_NONBLOCK, on obtient l'erreur "No such device or address"
            "Impossible d'ouvrir le tube nommé.\n"
            );

    char message[CHAR_BUFFER_LENGTH] = "En attente d'un message ..";

    write(descripteur_pipe_ecriture,message,CHAR_BUFFER_LENGTH);
    fprintf(stdout,"Serveur - message envoyé ..\n");

    sleep(15);

    close(descripteur_pipe_ecriture);
    fprintf(stdout,"Serveur - fermeture du tube nommé '%s'.\n",PATH_PIPE);

    CHECK_ERROR(
            remove(PATH_PIPE),
            "Impossible de supprimer le tube nommé.\n"
    );
    fprintf(stdout,"Serveur - fichier '%s' supprimé.\n",PATH_PIPE);
    return EXIT_SUCCESS;
}
