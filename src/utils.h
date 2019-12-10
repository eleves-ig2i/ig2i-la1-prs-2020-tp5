//
// Created by naysson on 26/11/2019.
//

#ifndef TP5_UTILS_H
#define TP5_UTILS_H

#include <time.h>

/**
 * \def CHECK_ERROR
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK_ERROR(sts,msg) if ((sts) == -1) {perror(msg);exit(1);}

/**
 * \def CHECK_DIFF
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK_DIFF(sts,msg,good_code) if ((sts) != (good_code)) {perror(msg);exit(1);}

/**
 * \def CHECK_DIFF_ERROR
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK_DIFF_ERROR(sts,msg,error_code) if ((sts) == (error_code)) {perror(msg);exit(1);}

/**
 * Fonction utilisée pour transformer un timestamp (nombre de secondes depuis le 01 01 1970), en un format de date lisible.
 * @param tm
 * @param buffer
 * @param buffer_length
 */
void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length);


#endif //TP5_UTILS_H
