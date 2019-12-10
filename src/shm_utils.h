//
// Created by naysson on 26/11/2019.
//

#ifndef TP5_SHM_UTILS_H
#define TP5_SHM_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "utils.h"

#define PATH_KEY_IPC "/var/www"
#define SALT 1
#define SHARED_BUFFER_SIZE 100
#define BUFFER_TIME_LENGTH 20

#define SEMAPHORE_SAISIE_NOM "/shm.saisie"


/**
 * Affiche les infos d'un segment partagé sur la sortie standard.
 * @param infos
 */
void afficher_infos_shm(int shmid);

/**
 * Génère la clé de l'ipc à l'aide des arguments en paramètre.
 * @param path
 * @param salt
 * @return
 */
key_t generate_key_ipc(char * path,int salt);


#endif //TP5_SHM_UTILS_H
