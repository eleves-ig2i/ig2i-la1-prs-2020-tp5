/**
 * Fichier : shm_e2.c
 * Auteur : Nathan SALEZ
 * Description : crée un segment de mémoire partagée,
 * dans lequel il stocke une chaîne de caractères de 100 caractères maximum,
 * et affiche toutes les informations connues sur ce segment de mémoire partagée,
 * Le programme crée un fils,le fils écrit dans la mémoire partagée et meurt.
 * Le père attend la mort de son fils, lit dans la mémoire partagée et affiche le message du fils.
 * puis détruit le segment de mémoire partagée.
 */

#include "shm_utils.h"

#include <unistd.h>
#include <wait.h>
#include <string.h>


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
    fprintf(stdout,"shm_e2 - création du segment partagé réussie.\n");

    // Récupération des infos du segment partagé.
    afficher_infos_shm(shmid);
    fprintf(stdout,"shm_e2 - récupération des infos du segment partagé réussie.\n");

    // Accrochage d'un segment de mémoire partagée à une variable.
    char * segment_partage = shmat(shmid,
                                   NULL,  // on veut que le noyau alloue dynamique l'adresse du segment.
            0); // autorisation de lecture et d'écriture
    assert(segment_partage != NULL);

    pid_t pid_courant = 0;
    CHECK_ERROR(
            pid_courant = fork(),
            "Impossible de créer un processus fils.\n"
    );

    // Traitement du processus fils
    if( pid_courant == 0)
    {
        //Ecriture dans la mémoire partagée
        sprintf(segment_partage,"Je suis le fils %d",pid_courant);
        exit(EXIT_SUCCESS);
    }

    CHECK_ERROR(waitpid(pid_courant,NULL,0),"CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n");

    // Lecture dans la mémoire partagée
    fprintf(stdout,"shm_e2 - père - lecture du message : '%s'\n",segment_partage);

    // Destruction du segment partagé
    strcpy(segment_partage,"\0");
    CHECK_ERROR(
            shmctl(shmid,IPC_RMID,NULL),
            "Impossible de détruire le segment de mémoire partagée."
            );
    fprintf(stdout,"shm_e2 - destruction du segment partagé %d réussie.\n",shmid);

    return 0;
}