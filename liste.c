#include "bal.h"
#include "liste.h"
#include "SoPu.h"

struct listeBal *Creer_Liste_Bal()
{

    struct listeBal *ma_liste;
    ma_liste = malloc(sizeof(struct listeBal));

    ma_liste->firstBal = NULL;
    ma_liste->lastBal = NULL;
    ma_liste->currentBal = ma_liste->firstBal;

    return ma_liste;
}

int videBal(struct listeBal *ma_liste)
{
    return (ma_liste->firstBal == NULL);
};

int videLettre(struct balCase *mon_bal)
{
    return (mon_bal->firstLettre == NULL);
};

void Avancer_Bal(struct listeBal *ma_liste)
{
    ma_liste->currentBal = ma_liste->currentBal->nextBal;
}

void Aller_Debut_Bal(struct listeBal *ma_liste)
{
    ma_liste->currentBal = ma_liste->firstBal;
}

int Existe_Bal(int num, struct listeBal *ma_liste)
{
    if (!videBal(ma_liste))
    {
        Aller_Debut_Bal(ma_liste);
        while (ma_liste->currentBal != NULL)
        {
            if (ma_liste->currentBal->numBal == num)
                return 1;
            else
                Avancer_Bal(ma_liste);
        }
        return 0;
    }
    else{
        return 0;
    } 
}

struct balCase *Creer_Bal(){
    struct balCase *bal = malloc(sizeof(struct balCase));
    bal->firstLettre= NULL;
    bal->lastLettre= NULL;
    bal->nextBal= NULL;
    bal->currentLettre= bal->firstLettre;
    return bal;
}

struct balCase* Ajouter_Bal(struct listeBal *ma_liste, int num, int n, int l)
{
    struct balCase *new;
    struct balCase *bal = Creer_Bal();

   if (videBal(ma_liste)){
        ma_liste->lastBal = bal;
        ma_liste->firstBal = ma_liste->lastBal;
        ma_liste->currentBal = ma_liste->firstBal;

        ma_liste->lastBal->numBal = num;
        ma_liste->lastBal->nb = n;
        ma_liste->lastBal->lg = l;
        
        return bal;
    }
    else if ((Existe_Bal(num, ma_liste)) == 0){
        new = ma_liste->lastBal;
        ma_liste->lastBal = bal;
        new->nextBal = ma_liste->lastBal;
        
        ma_liste->lastBal->numBal = num;
        ma_liste->lastBal->nb = n;
        ma_liste->lastBal->lg = l;

        return bal;
    } 
    else{
        return Cherche_Bal(num,ma_liste);
    }
}

struct lettre* Creer_Lettre(int num, char *buff){

    struct lettre* ma_lettre = malloc(sizeof(struct lettre));
    ma_lettre->numLettre=num;
    ma_lettre->message=malloc(MAX_LENGTH*sizeof(char));
    ma_lettre->nextLettre=NULL;
    strcpy(ma_lettre->message, buff);
    return ma_lettre;
    
}

struct balCase *Cherche_Bal(int num, struct listeBal *ma_liste)
{

    if (!videBal(ma_liste))
    {
        Aller_Debut_Bal(ma_liste);
        while (ma_liste->currentBal != NULL)
        {
            if (ma_liste->currentBal->numBal == num)
            {
                return ma_liste->currentBal;
            }
            else
            {
                Avancer_Bal(ma_liste);
            }
        }
    }
    return NULL;
}

int Compte_Bal(struct listeBal *ma_liste)
{
    int cmpt = 0;
    if (videBal(ma_liste))
    {
        return 0;
    }
    else
    {
        Aller_Debut_Bal(ma_liste);
        while (ma_liste->currentBal != NULL)
        {
            cmpt++;
            Avancer_Bal(ma_liste);
        }
        return cmpt;
    }
}

int Ajouter_Lettre(struct listeBal *ma_liste, int numBal, char *mess, int numL, int n, int l)
{
    struct balCase * aux = Ajouter_Bal(ma_liste,numBal,n,l);
    struct lettre *ma_lettre = Creer_Lettre(numL,mess);
    struct lettre *new;

    if (videLettre(aux)){
        aux->nb = n; aux->lg = l;
        aux->lastLettre = ma_lettre;
        aux->firstLettre = aux->lastLettre;
        ma_lettre->numLettre = 1;
        return 1;
    }
    else{
        new = aux->lastLettre;
        aux->lastLettre = ma_lettre;
        new->nextLettre = aux->lastLettre;
        //ma_lettre->numLettre = aux->lastLettre->numLettre + 1;
        return 1;  
    }
    //free(aux);
    return 0;
}

int Delete_Lettre(struct listeBal *ma_liste, int numBal, int numL)
{
    if (Existe_Bal(numBal, ma_liste) == 0){
        printf("ERREUR : La BAL n'existe pas\n");
        return 0;
    }
    else{
        if (videLettre(Cherche_Bal(numBal,ma_liste))){
            printf("ERREUR : La lettre n'existe pas\n");
            return 0;
        }
        else{
            struct balCase *aux = Cherche_Bal(numBal, ma_liste);
            struct lettre *auxL = aux->firstLettre;
            aux->firstLettre = aux->firstLettre->nextLettre;
            if (aux->firstLettre!=NULL)
                aux->firstLettre->numLettre = 1;
            else
                return 1;
            free(auxL);
            return 1;
        }
    }
}

void Afficher_Liste_Bal(struct listeBal *ma_liste)
{
    Aller_Debut_Bal(ma_liste);
    while (ma_liste->currentBal != NULL)
    {
        printf("BAL n°%d\n", ma_liste->currentBal->numBal);
        Avancer_Bal(ma_liste);
    }
    Aller_Debut_Bal(ma_liste);
}

void Afficher_Liste_Lettre(struct balCase *monBal)
{
    struct lettre *auxL = monBal->firstLettre;
    int i = 1;
    if (videLettre(monBal)){
        printf("La liste est vide\n");
    }
    else{
        while (auxL != NULL)
        {
            printf("%d - message %d :%s\n", i, auxL->numLettre, auxL->message);
            auxL = auxL->nextLettre;
            i++;
        }
    }  
}
