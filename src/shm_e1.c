/**
 * Fichier : shm_e1.c
 * Auteur : Nathan SALEZ
 * Description : crée un segment de mémoire partagée,
 * dans lequel il stocke une chaîne de caractères de 100 caractères maximum,
 * et affiche toutes les informations connues sur ce segment de mémoire partagée,
 * puis détruit le segment de mémoire partagée.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define PATH_KEY_IPC "/var/www"
#define SALT 1
#define SHARED_BUFFER_SIZE 100
#define BUFFER_TIME_LENGTH 20

/**
 * \def CHECK_ERROR
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK_ERROR(sts,msg) if ((sts) == -1) {perror(msg);exit(1);}

/**
 * Génère la clé de l'ipc à l'aide des arguments en paramètre.
 * @param path
 * @param salt
 * @return
 */
key_t generate_key_ipc(char * path,int salt)
{
    assert(path != NULL);
    return ftok(path,salt);
}


/**
 * Fonction utilisée pour transformer un timestamp (nombre de secondes depuis le 01 01 1970), en un format de date lisible.
 * @param tm
 * @param buffer
 * @param buffer_length
 */
void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length)
{
    struct tm *tm = localtime(&timestamp);
    strftime(buffer,buffer_length,"%F %T",tm);
}


/**
 * Affiche les infos d'un segment partagé sur la sortie standard.
 * @param infos
 */
void afficher_infos_shm(int shmid)
{
    struct shmid_ds infos;
    char buffer_time[BUFFER_TIME_LENGTH+1];
    CHECK_ERROR(
            shmctl(shmid,IPC_STAT,&infos),
            "Impossible de récupérer les infos du segment de mémoire partagée.\n"
    );
    fprintf(stdout,"Segment partagé %d - proprietaire : %d \n",shmid,infos.shm_perm.uid);
    fprintf(stdout,"Segment partagé %d - groupe du proprietaire  :%d \n",shmid,infos.shm_perm.gid);
    fprintf(stdout,"Segment partagé %d - createur : %d \n",shmid,infos.shm_perm.cuid);
    fprintf(stdout,"Segment partagé %d - groupe du createur : %d \n",shmid,infos.shm_perm.cgid);
    fprintf(stdout,"Segment partagé %d - mode d'accès  : %d \n",shmid,infos.shm_perm.mode);
    fprintf(stdout,"Segment partagé %d - taille du segment : %d \n",shmid,(int )infos.shm_segsz);
    transform_timestamp_to_string(infos.shm_ctime,buffer_time,BUFFER_TIME_LENGTH);
    fprintf(stdout, "Segment partagé %d - dernière modification : %s \n",shmid, buffer_time);
}

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
    fprintf(stdout,"shm_e1 - création du segment partagé réussie.\n");

    // Récupération des infos du segment partagé.
    afficher_infos_shm(shmid);
    fprintf(stdout,"shm_e1 - récupération des infos du segment partagé réussie.\n");


    // Destruction du segment partagé
    CHECK_ERROR(
            shmctl(shmid,IPC_RMID,NULL),
            "Impossible de détruire le segment de mémoire partagée."
            );
    fprintf(stdout,"shm_e1 - destruction du segment partagé %d réussie.\n",shmid);
    return 0;
}