/**
 * Fichier : shm_cli.c
 * Auteur : Nathan SALEZ
 * Description : a pour mission d’aller lire le message laissé par shm_srv et de
 * l’afficher.
 */

#include "shm_utils.h"

int main() {

    int shmid = 0;

    CHECK_ERROR(
            shmid = shmget(
                    generate_key_ipc(PATH_KEY_IPC,SALT),
                    SHARED_BUFFER_SIZE*sizeof(char),
                    SHM_R
            ),
            "Impossible de récupérer le segment de mémoire partagée."
    );

    char * segment_partage = (char *)shmat(shmid,NULL,0);

    fprintf(stdout,"shm_cli - message provenant du serveur : '%s'\n",segment_partage);

    return 0;
}