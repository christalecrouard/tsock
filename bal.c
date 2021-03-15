#include "bal.h"
#include "liste.h"
#include "SoPu.h"


void rangearray2(int num, char *dest, int start)
{
    char tab1[5] = {' ', ' ', ' ', ' ', ' '};
    char tab2[5] = {'0', '0', '0', '0', '0'};
    int i, factor, nb2;
    factor = 1;
    nb2 = num;
    int temp = nb2;
    while (temp)
    {
        temp = temp / 10;
        factor = factor * 10;
    }
    int j = 0;
    int k = 0;
    while (factor > 1)
    {
        factor = factor / 10;
        tab1[4 - j] = nb2 / factor + '0';
        nb2 = nb2 % factor;
        j++;
    }

    for (i = 0; i <= 4; i++)
    {
        if (tab1[i] == ' ')
        {
            tab2[i] = tab1[i];
            k++;
        }
    }
    for (i = 0; i + k <= 4; i++)
    {
        tab2[i + k] = tab1[4 - i];
    }
    for (i = start; i <= start + 4; i++)
    {
        dest[i] = tab2[i - start];
    }

}

char *pduMaker(int n_emm, int n_rec, int nb, int lg, char *pdu)
{

    strcpy(pdu, "xaaaaaaaaaaaaaaaaa");
    if (n_emm > 0)
    {
        memset(pdu, 'e', 1);
        rangearray2(n_emm, pdu, 1);
    }
    else if (n_rec > 0)
    {
        memset(pdu, 'r', 1);
        rangearray2(n_rec, pdu, 1);
    }
    else if (n_emm=0 && n_rec==0){
        memset(pdu, 'x', 1);
        
    }
    memset(pdu + 6, 'n', 1);
    memset(pdu + 12, 'l', 1);

    rangearray2(nb, pdu, 7);
    rangearray2(lg, pdu, 13);
    return pdu;
}

char *Sub_Char_Array(char *src, char *dest, int deb)
{

    // int deb possibles : 1, 7 et 13 (pour e, n et l)
    memset(dest, '\0', sizeof(dest));
    strncpy(dest, src + deb, 5);

    return dest;
}

void emmeteurTCP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int option, int n_emm, int n_rec)
{
    if (port_dist < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    shutdown(port_dist,2); //eviter les erreurs de bind 

    n_rec = 0;
    // INITIALISATION DU MESSAGE
    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));

    // CREATION DU SOCKET
    int port = socket(AF_INET, SOCK_STREAM, 0);
    if (port == -1)
    {
        printf("ERREUR : Echec de la création du socket\n");
        exit(1);
    }

    // ADRESSAGE
    struct sockaddr_in adr_dist;
    struct hostent *hp;

    memset((char *)&adr_dist, 0, sizeof(adr_dist));
    adr_dist.sin_family = AF_INET;
    adr_dist.sin_port = htons(port_dist);

    if ((hp = gethostbyname(nom_ad_dist)) == NULL)
    {
        printf("ERREUR : Echec de l'opération gethost\n");
        exit(1);
    }

    memcpy((char *)&(adr_dist.sin_addr.s_addr), hp->h_addr, hp->h_length);

    //CONNEXION
    printf("\nAttente d'établissement de la connexion\n");
    while (connect(port, (struct sockaddr *)&adr_dist, sizeof(adr_dist)) == -1)
    {
        NULL;
    }

    //ENVOI MESSAGE
    int i;

    //envoi du message ave l'info emmetteur
    char *pdu;
    pdu = malloc(MAX_PDU);
    pdu = pduMaker(n_emm, n_rec, nb, lg, pdu);
    int pdusent = send(port, pdu, MAX_PDU, 0);
    if (pdusent == -1)
    {
        printf("Erreur de l'envoi du pdu\n");
    }
    else
    {
        afficher_message(pdu, MAX_PDU, 0, 1, n_emm, n_rec);
    }

    for (i = 0; i < nb; i++)
    {
        construire_message(message, lg, i, n_emm - 1, alpha);
        int envoi = send(port, message, lg, 0);
        if (envoi == -1)
            printf("Erreur de l'envoi du message n°%d\n", i + 1);
        else
            afficher_message(message, lg, i + 1, 1, n_emm, n_rec);
    }

    //FIN DE CONNEXION
    if (!shutdown(port, 2))
    {
        printf("SOURCE : fin\n\n");
    }
    //free(pdu); free(message);
}

void recepteurTCP(int port_dist, char *nom_ad_dist, int lg, int nb, int option, int n_emm, int n_rec)
{
    if (port_dist < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    shutdown(port_dist,2); //eviter les erreurs de bind

    // CREATION DU SOCKET
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("ERREUR : Echec de la création du socket\n");
        exit(1);
    }

    // ADRESSAGE
    struct sockaddr_in adr_dist;
    struct hostent *hp;

    memset((char *)&adr_dist, 0, sizeof(adr_dist));
    adr_dist.sin_family = AF_INET;
    adr_dist.sin_port = htons(port_dist);

    if ((hp = gethostbyname(nom_ad_dist)) == NULL)
    {
        printf("ERREUR : Echec de l'opération gethost\n");
        exit(1);
    }

    memcpy((char *)&(adr_dist.sin_addr.s_addr), hp->h_addr, hp->h_length);

    //Adress locale
    struct sockaddr_in ad_local;

    memset((char *)&ad_local, 0, sizeof(ad_local));

    ad_local.sin_family = AF_INET;
    ad_local.sin_port = htons(port_dist);
    ad_local.sin_addr.s_addr = INADDR_ANY;

    //CONNEXION
    printf("\nAttente d'établissement de la connexion\n");
    while (connect(sock, (struct sockaddr *)&adr_dist, sizeof(adr_dist)) == -1)
    {
        NULL;
    }

    //ENVOI MESSAGE

    //envoi du message avec l'info recepteur
    char *pdu;
    pdu = malloc(MAX_PDU);
    pdu = pduMaker(n_emm, n_rec, nb, lg, pdu);
    int pdusent = send(sock, pdu, MAX_PDU, 0);
    if (pdusent == -1)
    {
        printf("Erreur de l'envoi du pdu\n");
    }

    //RECEPTION DES MESSAGES

    int lg_rcv;

    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));

    if ((lg_rcv = recv(sock, message, MAX_PDU, 0)) < 0)
    {
        perror("ERREUR : L'opération recv a échoué\n");
    }
    else{   
        
        if (message[0]=='x'){
            printf("La BAL n'a pas de messages pour le recepteur %d\n", n_rec);
            if (shutdown(sock,2) == -1){
                printf("ERREUR: Le socket n'as pas été détruit\n\n");
                exit(0);
            }
            else{
                printf("PUITS : fin\n\n");
                exit(1);
            };
            exit(1);
        }       
        afficher_message(message, MAX_PDU, 1, 1, n_emm, n_rec);
        char dest[5];
        if (lg_rcv == 0){
            exit(1);
        }
        nb = atoi(Sub_Char_Array(message, dest, 7));
        lg = atoi(Sub_Char_Array(message, dest, 13));
        printf("nombre de messages emis %d\n", nb);
        printf("message emis de taille %d\n", lg);
    }

    int i = 1;
    while (1)
    {
        //////////////////:
        if (i!=0)
        {
            for (i = 1; i <= nb; i++)
            {
                if (option)
                {
                    if ((lg_rcv = recv(sock, message, lg, MSG_OOB)) < 0)
                    {
                        perror("ERREUR : L'opération recv a échoué\n");
                    }
                    else
                        afficher_message(message, lg, i, 0, n_emm, n_rec);
                }
                else
                {
                    if ((lg_rcv = recv(sock, message, lg, 0)) < 0)
                    {
                        perror("ERREUR : L'opération recv a échoué\n");
                    }
                    else
                        afficher_message(message, lg, i, 0, n_emm, n_rec);
                }
            }

            if (close(sock) == -1)
            {
                printf("ERREUR: Le socket n'as pas été détruit\n\n");
                exit(0);
            }
            else
            {
                printf("PUITS : fin\n\n");
                exit(1);
            };
        }
        i++;
    }
    //free(pdu); free(message);
}

void boite(int port_local, int lg, int nb, int option, int n_emm, int n_rec)
{

    if (port_local < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    shutdown(port_local,2); //eviter les erreurs de bind

    //GESTION BAL
    struct listeBal *ma_liste = malloc(sizeof(struct listeBal));
    ma_liste=Creer_Liste_Bal();
    char type;

    // CREATION DU SOCKET
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("ERREUR : Le socket n'a pas été créé\n");
        exit(1);
    }

    //ADRESSAGE
    struct sockaddr_in serveur, client;
    int size_seveur = sizeof(serveur);
    int size_client = sizeof(client);

    memset((char *)& serveur, 0, size_seveur);

    serveur.sin_family = AF_INET;
    serveur.sin_port = htons(port_local);
    serveur.sin_addr.s_addr = INADDR_ANY;

    //BINDING
    if (bind(sock, (struct sockaddr *)&serveur, size_seveur) == -1)
    {
        printf("ERREUR : L'opération bind a échoué\n");
        exit(1);
    }

    //LISTEN
    if (listen(sock, MAX_CO) == -1)
    {
        NULL;
    }

    //ACCEPT
    int lg_rcv, sock_bis;
    
    //int lg_ad_local = sizeof(ad_local);
    printf("\nAttente d'établissement de la connexion");

    int i = 0;
    int emm = 0;
    int rec = 0;

    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));
    char *pdu;
    pdu = malloc(MAX_PDU);

    while (1)
    {
        printf("\n");
        if ((sock_bis = accept(sock, (struct sockaddr *)&client, &size_client)) == -1)
        {
            printf("ERREUR: L'opération accept a échoué\n");
            exit(3);
        }

        i = 0;

        while (1)
        {
            if (option)
            {
                if ((lg_rcv = recv(sock_bis, message, lg, MSG_OOB)) < 0)
                {
                    perror("ERREUR : L'opération recv a échoué\n");
                }
                else
                    afficher_message(message, lg, i + 1, 0, n_emm, n_rec);
            }
            else
            {
                if (i == 0)
                { //reception et traitement du pdu
                    if ((lg_rcv = recv(sock_bis, pdu, MAX_PDU, 0)) < 0)
                    {
                        perror("ERREUR : L'opération recv a échoué\n");
                    }
                    else
                    {

                        char dest[5];
                        type = pdu[0];

                        if (type == 'e')
                        {
                            emm = atoi(Sub_Char_Array(pdu, dest, 1));
                            nb = atoi(Sub_Char_Array(pdu, dest, 7));
                            int lg1 = lg;
                            lg = atoi(Sub_Char_Array(pdu, dest, 13));
                            printf("message de l'emmeteur n %d\n", emm);
                            printf("nombre de messages emis %d\n", nb);
                            printf("message emis de taille %d\n\n", lg);
                            if (Existe_Bal(emm,ma_liste) && !videLettre(Cherche_Bal(emm,ma_liste)) && lg!=lg1){
                                printf("La prise en charge de messages de longueurs différentes dans une même BAL n'est pas encore prise en compte\n");
                                i =0;
                                lg =lg1;
                                break;
                            }
                        }

                        if (type == 'r')
                        {
                            rec = atoi(Sub_Char_Array(pdu, dest, 1));
                            printf("message du recepteur n %d\n", rec);

                            if (Existe_Bal(rec, ma_liste))
                            { //si le bal existe envoi du pdu vers recepteur
                                struct balCase *aux = Cherche_Bal(rec, ma_liste);
                                if (!videLettre(aux))
                                {
                                    char *pdus = malloc(MAX_PDU);
                                    pdus = pduMaker(0, rec, aux->nb, aux->lg, pdus);
                                    int envoi = send(sock_bis, pdus, MAX_PDU, 0);
                                    if (envoi == -1)
                                        printf("Erreur de l'envoi du message n°%d\n", i);
                                    //free(pdus);
                                }
                                printf("nombre de messages à emettre %d\n", aux->nb);
                                printf("message à emettre de taille %d\n\n", aux->lg);
                                //free(aux);
                            }
                            else
                            {
                                printf("ERREUR : La BAL n'existe pas\n");
                                char *pdux = malloc(MAX_PDU);
                                pdux = pduMaker(0, 0, 1, 1, pdux);
                                int envoi = send(sock_bis, pdux, MAX_PDU, 0); //pb ici
                                if (envoi == -1)
                                    printf("Erreur de l'envoi du message n°%d\n", i);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (type == 'e') //reception des messages
                    {
                        if ((lg_rcv = recv(sock_bis, message, lg, 0)) < 0)
                        {
                            perror("ERREUR : L'opération recv a échoué\n");
                        }
                        else
                        {
                            if (lg_rcv == 0)
                            {
                                shutdown(sock_bis, 2);
                                i = 0;
                                break;
                            }
                            afficher_message(message, lg, i, 0, n_emm, n_rec);
                            Ajouter_Lettre(ma_liste, emm, message, i, nb, lg);
                        }
                    }
                    else if (type == 'r')
                    { //envoi de messages
                        if (Existe_Bal(rec, ma_liste))
                        {
                            if (!videLettre(Cherche_Bal(rec, ma_liste)))
                            {
                                struct balCase *aux = Cherche_Bal(rec, ma_liste);
                                while (aux->firstLettre != NULL)
                                {
                                    int envoi = send(sock_bis, aux->firstLettre->message, aux->lg, 0); //pb ici
                                    if (envoi == -1)
                                        printf("Erreur de l'envoi du message n°%d\n", i);
                                    afficher_message(aux->firstLettre->message, aux->lg, aux->firstLettre->numLettre, 1, 0, 0);
                                    if(!Delete_Lettre(ma_liste,rec,1)){
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                printf("ERREUR : Il n'y a plus de lettre\n");
                                shutdown(sock_bis, 2);
                                break;
                            }
                        }
                        else
                        {
                            printf("ERREUR : La BAL n'existe pas\n");
                            shutdown(sock_bis, 2);
                        }
                    }
                }
            }
            i++;
        }
        printf("\nAttente d'établissement de la connexion\n");
    }
}

void boite2(int port_local, int lg, int nb, int option, int n_emm, int n_rec)
{
    //GESTION BAL
    struct listeBal *ma_liste;
    char type;

    // CREATION DU SOCKET
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("ERREUR : Le socket n'a pas été créé\n");
        exit(1);
    }

    //ADRESSAGE
    struct sockaddr_in ad_local;

    int lg_adr_local = sizeof(ad_local);

    memset((char *)&ad_local, 0, sizeof(ad_local));

    ad_local.sin_family = AF_INET;
    ad_local.sin_port = htons(port_local);
    ad_local.sin_addr.s_addr = INADDR_ANY;

    //BINDING
    if (bind(sock, (struct sockaddr *)&ad_local, lg_adr_local) == -1)
    {
        printf("ERREUR : L'opération bind a échoué\n");
        exit(1);
    }

    //LISTEN
    while (listen(sock, MAX_CO) == -1)
    {
        NULL;
    }

    //ACCEPT
    int lg_rcv, sock_bis;
    int lg_ad_local = sizeof(ad_local);
    printf("\nAttente d'établissement de la connexion");

    int i = 0;

    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));

    while (1)
    {
        printf("\n");
        if ((sock_bis = accept(sock, (struct sockaddr *)&ad_local, &lg_ad_local)) == -1)
        {
            printf("ERREUR: L'opération accept a échoué\n");
            exit(1);
        }

        switch (fork())
        {
            case -1:
                printf("ERREUR: L'opération fork a échoué\n");
                exit(1);

            case 0:
                close(sock);
                i = 0;

                while (1)
                {
                    if (option)
                    {
                        if ((lg_rcv = recv(sock_bis, message, lg, MSG_OOB)) < 0)
                        {
                            perror("ERREUR : L'opération recv a échoué\n");
                        }
                        else
                            afficher_message(message, lg, i + 1, 0, n_emm, n_rec);
                    }
                    else
                    {

                        if (i == 0)
                        {
                            if ((lg_rcv = recv(sock_bis, message, MAX_PDU, 0)) < 0)
                            {
                                perror("ERREUR : L'opération recv a échoué\n");
                            }
                            else
                            {
                                if (lg_rcv == 0)
                                {
                                    exit(1);
                                }
                                type = message[0];
                                //n_emm = atoi(scanf(message[]))
                                afficher_message(message, MAX_PDU, i, 0, n_emm, n_rec);
                            }
                        }
                        else
                        {
                            if ((lg_rcv = recv(sock_bis, message, lg, 0)) < 0)
                            {
                                perror("ERREUR : L'opération recv a échoué\n");
                            }
                            else
                            {
                                if (lg_rcv == 0)
                                {
                                    exit(1);
                                }
                                afficher_message(message, lg, i, 0, n_emm, n_rec);
                            }
                        }
                    }
                    i++;
                }

                printf("\nAttente d'établissement de la connexion\n");
                exit(0);

            default:
                close(sock_bis);
        }
    }
    //NULL;
}

