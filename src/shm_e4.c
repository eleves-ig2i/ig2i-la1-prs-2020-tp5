/**
 * Fichier : shm_e4.c
 * Auteur : Nathan SALEZ
 * Description : crée un segment de mémoire partagée,dans lequel il stocke une chaîne de caractères de 100 caractères maximum,
 * Le programme crée un fils,le fils écrit dans la mémoire partagée et meurt.
 * Le programme crée à présent 2 fils.
 * Chaque fils écrit un message différent dans la zone de mémoire partagée.Ce message est écrit
 * caractère par caractère,
 * (par exemple le premier fils écrit des caractères A, le second des caractères B),
 * et fait une pause entre chaque caractère (par exemple avec sleep).
 * Le père attend la mort de ses deux fils,
 * lit dans la mémoire partagée et affiche le message
 * puis détruit le segment de mémoire partagée.
 *
 * Le dernier processus doit avoir le dernier mot.
 */

#include "shm_utils.h"

#include <unistd.h>
#include <wait.h>
#include <string.h>

#include <semaphore.h>
#include <fcntl.h>

sem_t * semaphore_saisie;

/**
 * Ecrit le caractère c sur le segment partagé, nb fois avec une pause de p_sec secondes entre chaque saisie.
 * @param c le caractère à saisir
 * @param nb le nombre de caractères à saisir
 * @param p_sec la pause en secondes, entre chaque saisie de caractère.
 * @param segment_partage
 */
void ecrire_caractere(char c[2], size_t nb,int p_sec,int shmid)
{
    char * segment_partage = shmat(shmid,NULL,0);
    sem_wait(semaphore_saisie);
    fprintf(stdout,"shm_e4 - processus %d - j'entre dans la section critique.\n",getpid());
    for(size_t i = 0; i < nb; i++)
    {
        strcat(segment_partage,c);
        sleep(p_sec);
    }
    fprintf(stdout,"shm_e4 - processus %d - je sors de la section critique.\n",getpid());
    sem_post(semaphore_saisie);
}

int main() {
    int shmid = 0;

    CHECK_DIFF_ERROR(
            semaphore_saisie = sem_open(SEMAPHORE_SAISIE_NOM,
                    O_CREAT | O_EXCL,
                    0600,
                    1),
            "Impossible de créer le sémaphore de saisie.\n",
            SEM_FAILED
    );

    CHECK_ERROR(
            shmid = shmget(generate_key_ipc(PATH_KEY_IPC,SALT),SHARED_BUFFER_SIZE*sizeof(char),SHM_R | SHM_W | IPC_CREAT | IPC_EXCL),
            "Impossible de créer un segment de mémoire partagée."
            );
    fprintf(stdout,"shm_e4 - création du segment partagé réussie.\n");

    pid_t pid_apres_fils1, pid_apres_fils2;
    CHECK_ERROR(
            pid_apres_fils1 = fork(),
            "Impossible de créer le processus fils 1.\n"
    );

    // Traitement du processus fils 1
    if(pid_apres_fils1 == 0)
    {
        //Ecriture dans la mémoire partagée
        ecrire_caractere("A",8,1,shmid);
        exit(EXIT_SUCCESS);
    }

    CHECK_ERROR(
            pid_apres_fils2 = fork(),
            "Impossible de créer le processus fils 2.\n"
    );

    // Traitement du processus fils 2
    if(pid_apres_fils2 == 0)
    {
        //Ecriture dans la mémoire partagée
        ecrire_caractere("B",8,5,shmid);
        exit(EXIT_SUCCESS);
    }

    CHECK_ERROR(waitpid(pid_apres_fils1, NULL, 0), "CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n");
    CHECK_ERROR(waitpid(pid_apres_fils2, NULL, 0), "CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n");

    // Accrochage d'un segment de mémoire partagée à une variable.
    char * segment_partage = shmat(shmid,NULL,0);
    assert(segment_partage != NULL);
    // Lecture dans la mémoire partagée
    fprintf(stdout,"shm_e4 - père - lecture du message : '%s'\n",segment_partage);


    // Destruction du segment partagé
    strcpy(segment_partage,"\0");
    CHECK_ERROR(shmctl(shmid,IPC_RMID,NULL),"Impossible de détruire le segment de mémoire partagée.");
    fprintf(stdout,"shm_e4 - destruction du segment partagé %d réussie.\n",shmid);

    // Destruction du sémaphore de saisie
    CHECK_ERROR(
            sem_unlink(SEMAPHORE_SAISIE_NOM),
            "Impossible de détruire le sémaphore de saisie.\n"
            );
    CHECK_ERROR(
            sem_close(semaphore_saisie),
            "Impossible de libérer la mémoire allouée à semaphore_saisie.\n"
    );

    return 0;
}