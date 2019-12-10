//
// Created by naysson on 26/11/2019.
//

#include "shm_utils.h"

key_t generate_key_ipc(char * path,int salt)
{
    assert(path != NULL);
    return ftok(path,salt);
}


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