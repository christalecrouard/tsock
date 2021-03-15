#ifndef BAL_H
#define BAL_H

/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>

void rangearray2(int num, char *dest, int start);

char *pduMaker(int n_emm, int n_rec, int nb, int lg, char *pdu);

char *Sub_Char_Array(char *src, char *dest, int deb);

void emmeteurTCP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int option, int n_emm, int n_rec);

void recepteurTCP(int port_dist, char *nom_ad_dist, int lg, int nb, int option, int n_emm, int n_rec);

void boite(int port_local, int lg, int nb, int option, int n_emm, int n_rec);

void boite2(int port_local, int lg, int nb, int option, int n_emm, int n_rec);


#endif
