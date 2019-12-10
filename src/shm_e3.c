/**
 * Fichier : shm_e3.c
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
 */

#include "shm_utils.h"

#include <unistd.h>
#include <wait.h>
#include <string.h>

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
    for(size_t i = 0; i < nb; i++)
    {
        // @todo verification de la taille du buffer
        strcat(segment_partage,c);
        sleep(p_sec);
    }
}

int main() {
    int shmid = 0;
    CHECK_ERROR(
            shmid = shmget(generate_key_ipc(PATH_KEY_IPC,SALT),SHARED_BUFFER_SIZE*sizeof(char),SHM_R | SHM_W | IPC_CREAT | IPC_EXCL),
            "Impossible de créer un segment de mémoire partagée."
            );
    fprintf(stdout,"shm_e3 - création du segment partagé réussie.\n");

    pid_t pid_apres_fils1, pid_apres_fils2;
    CHECK_ERROR(
            pid_apres_fils1 = fork(),
            "Impossible de créer le processus fils 1.\n"
    );

    // Traitement du processus fils 1
    if(pid_apres_fils1 == 0)
    {
        //Ecriture dans la mémoire partagée
        fprintf(stdout,"\tshm_e3 - fils 1 - je suis là !\n");
        ecrire_caractere("A",5,5,shmid);
        fprintf(stdout,"\tshm_e3 - fils 1 - j'ai terminé d'écrire !\n");
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
        fprintf(stdout,"\tshm_e3 - fils 2 - je suis là !\n");
        ecrire_caractere("B",5,1,shmid);
        fprintf(stdout,"\tshm_e3 - fils 2 - j'ai terminé d'écrire !\n");
        exit(EXIT_SUCCESS);
    }

    CHECK_ERROR(waitpid(pid_apres_fils1, NULL, 0), "CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n");
    CHECK_ERROR(waitpid(pid_apres_fils2, NULL, 0), "CHECK - Le processus fils n'existe pas ou n'est pas un fils du processus parent.\n");

    // Accrochage d'un segment de mémoire partagée à une variable.
    char * segment_partage = shmat(shmid,NULL,0);
    assert(segment_partage != NULL);
    // Lecture dans la mémoire partagée
    fprintf(stdout,"shm_e3 - père - lecture du message : '%s'\n",segment_partage);


    // Destruction du segment partagé
    strcpy(segment_partage,"\0");
    CHECK_ERROR(shmctl(shmid,IPC_RMID,NULL),"Impossible de détruire le segment de mémoire partagée.");
    fprintf(stdout,"shm_e3 - destruction du segment partagé %d réussie.\n",shmid);

    return 0;
}