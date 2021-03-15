#ifndef LISTE_H
#define LISTE_H

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

struct lettre
{
    int numLettre;
    char *message;
    struct lettre *nextLettre;
};

struct balCase
{
    struct balCase *nextBal;
    struct lettre *firstLettre;
    struct lettre *lastLettre;
    struct lettre *currentLettre;
    int numBal;
    int nb;
    int lg;
};

struct listeBal
{
    struct balCase *firstBal;
    struct balCase *lastBal;
    struct balCase *currentBal;
};

struct listeBal *Creer_Liste_Bal();

int videBal(struct listeBal *ma_liste);

int videLettre(struct balCase *mon_bal);

void Avancer_Bal(struct listeBal *ma_liste);

void Aller_Debut_Bal(struct listeBal *ma_liste);

int Existe_Bal(int num, struct listeBal *ma_liste);

struct balCase *Creer_Bal();

struct balCase *Ajouter_Bal(struct listeBal *ma_liste, int num, int n, int l);

struct lettre *Creer_Lettre(int num, char *buff);

struct balCase *Cherche_Bal(int num, struct listeBal *ma_liste);

int Compte_Bal(struct listeBal *ma_liste);

int Ajouter_Lettre(struct listeBal *ma_liste, int numBal, char *mess, int numL, int n, int l);

int Delete_Lettre(struct listeBal *ma_liste, int numBal, int numL);

void Afficher_Liste_Bal(struct listeBal *ma_liste);

void Afficher_Liste_Lettre(struct balCase *monBal);

#endif
