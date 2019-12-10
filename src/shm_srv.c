/**
 * Fichier : shm_srv.c
 * Auteur : Nathan SALEZ
 * Description : crée un segment de mémoire partagée,
 * dans lequel il stocke une chaîne de caractères de 100 caractères maximum,
 * et affiche toutes les informations connues sur ce segment de mémoire partagée,
 * écrit un message dans le segment de mémoire partagée
 * et attend 30 secondes avant de seterminer.
 */

#include "shm_utils.h"

#include <string.h>
#include <unistd.h>


int main() {
    // Création du segment partagé
    int shmid = 0;
    CHECK_ERROR(
            shmid = shmget(
                    generate_key_ipc(PATH_KEY_IPC,SALT),
                    SHARED_BUFFER_SIZE*sizeof(char),
                    SHM_R | SHM_W | IPC_CREAT | IPC_EXCL
                    ),
            "Impossible de créer un segment de mémoire partagée."
            );
    fprintf(stdout,"shm_srv - création du segment partagé réussie.\n");

    // Récupération des infos du segment partagé.
    afficher_infos_shm(shmid);
    fprintf(stdout,"shm_srv - récupération des infos du segment partagé réussie.\n");

    char * segment_partage = shmat(shmid,NULL,0);
    strcpy(segment_partage,"Bonjour, ceci est un message venant du serveur.");
    fprintf(stdout,"shm_srv - écriture du message effectué.\n");
    sleep(30);

    // Destruction du segment partagé
    strcpy(segment_partage,"\0");
    CHECK_ERROR(
            shmctl(shmid,IPC_RMID,NULL),
            "Impossible de détruire le segment de mémoire partagée."
            );
    fprintf(stdout,"shm_srv - destruction du segment partagé %d réussie.\n",shmid);

    return 0;
}