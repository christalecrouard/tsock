#ifndef SOPU_H
#define SOPU_H

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

#define MAX_LENGTH 99999
#define MAX_CO 5
#define MAX_PDU 18

void construire_message(char *message, int lg, int dig, int num, char *alpha);

void rangearray(int num);

void afficher_message(char *message, int lg, int nb, int source, int n_emm, int n_rec);

void sourceUDP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int n_emm, int n_rec);

void puitsUDP(int port, int lg, int nb, int n_emm, int n_rec);

void sourceTCP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int option, int n_emm, int n_rec);

void puitsTCP(int port_local, int lg, int nb, int option, int n_emm, int n_rec);

#endif
