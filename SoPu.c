#include "bal.h"
#include "liste.h"
#include "SoPu.h"

void construire_message(char *message, int lg, int dig, int num, char *alpha)
{

    int i, factor, num2;
    factor = 1;
    num2 = num + 1;
    int temp = num2;
    char tab[5] = {' ', ' ', ' ', ' ', ' '};

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
        tab[4 - j] = num2 / factor + '0';
        num2 = num2 % factor;
        j++;
    }

    if (lg <= 5)
    {
        for (i = 0; i < lg; i++)
        {
            if (tab[i] == ' ')
            {
                message[i] = tab[i];
                k++;
            }
        }
        for (i = 0; i < lg; i++)
        {
            message[i + k] = tab[4 - i];
        }
    }
    else
    {
        for (i = 0; i < lg; i++)
        {
            message[i] = alpha[dig % 26];
        }
        for (i = 0; i <= 4; i++)
        {
            if (tab[i] == ' ')
            {
                message[i] = tab[i];
                k++;
            }
        }
        for (i = 4; i >= 4 - (5 - k) + 1; i--)
        {
            message[k + (i - 4) * -1] = tab[i];
        }
    }
}

void rangearray(int num)
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
    for (i = 0; i <= 4; i++)
    {
        printf("%c", tab2[i]);
    }
}

void afficher_message(char *message, int lg, int nb, int source, int n_emm, int n_rec)
{

    int i;

    if (source)
    {

        if (n_emm == 0 && n_rec == 0)
        {
            printf("SOURCE : Envoi N°");
            rangearray(nb);
            printf(" (");
            rangearray(lg);
            printf(") ");
        }
        else if (n_emm != 0)
        {
            printf("SOURCE : Envoi lettre n°");
            rangearray(nb);
            printf(" à destination du récepteur");
            rangearray(n_emm);
            printf(" (");
            rangearray(lg);
            printf(") ");
        }

        printf("[");
        if (lg <= 31)
        {
            for (i = 0; i < lg; i++)
            {
                printf("%c", message[i]);
            }
            printf("]\n");
        }
        else
        {
            for (i = 0; i <= 30; i++)
            {
                printf("%c", message[i]);
            }
            printf("...]\n");
        }
    }

    else if (!source)
    {

        if (n_emm == 0 && n_rec == 0)
        {
            printf("PUITS : Reception N°");
            rangearray(nb);
            printf(" (");
            rangearray(lg);
            printf(") ");
        }
        else if (n_rec != 0)
        {
            printf("RECEPTION : Récupération lettre par le recepteur ");
            rangearray(n_rec);
            printf(" (");
            rangearray(lg);
            printf(") ");
        }

        printf("[");
        if (lg <= 31)
        {
            for (i = 0; i < lg; i++)
            {
                printf("%c", message[i]);
            }
            printf("]\n");
        }
        else
        {
            for (i = 0; i <= 30; i++)
            {
                printf("%c", message[i]);
            }
            printf("...]\n");
        }
    }
}

void sourceUDP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int n_emm, int n_rec)
{
    if (port_dist < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    close(port_dist,2); //eviter les erreurs de bind

    // INITIALISATION DU MESSAGE
    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));

    // CREATION DU SOCKET
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

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

    // ENVOI DU MESSAGE
    int lg_emis;
    int i;
    int lg_adr = sizeof(adr_dist);

    for (i = 0; i < nb; i++)
    {
        construire_message(message, lg, i, i, alpha);

        lg_emis = sendto(sock, message, lg, 0, (struct sockaddr *)&adr_dist, lg_adr);
        if (lg_emis == -1)
        {
            printf("ERREUR : Echec de l'envoi\n");
            exit(1);
        }
        afficher_message(message, lg, i + 1, 1, n_emm, n_rec);
    }

    //DESTROY
    if (close(sock) == -1)
    {
        printf("ERREUR: Le socket n'as pas été détruit\n\n");
    }
    else
    {
        printf("SOURCE : fin\n\n");
    };
}

void puitsUDP(int port, int lg, int nb, int n_emm, int n_rec)
{
    if (port < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    close(port,2); //eviter les erreurs de bind

    // CREATION DU SOCKET
    int sock;

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("ERREUR : Le socket n'a pas été créé\n");
        exit(1);
    }

    // ADRESSAGE
    struct sockaddr_in ad_local;

    int lg_ad_local = sizeof(ad_local);

    memset((char *)&ad_local, 0, sizeof(ad_local));

    ad_local.sin_family = AF_INET;
    ad_local.sin_port = htons(port);
    ad_local.sin_addr.s_addr = INADDR_ANY;

    //BINDING
    if (bind(sock, (struct sockaddr *)&ad_local, lg_ad_local) == -1)
    {
        printf("ERREUR : L'opération bind a échoué\n");
        exit(1);
    }

    //RECEPTION
    struct sockaddr client;
    int lg_client = sizeof(client);
    int i = 0;

    char *message;
    message = malloc(MAX_LENGTH * sizeof(char));

    if (nb == -1)
    {
        i = 1;
        while (1)
        {
            int reception = recvfrom(sock, message, lg, 0, &client, &lg_client);
            if (reception == -1)
            {
                printf("ERREUR : Echec de la réception\n");
                exit(1);
            }
            afficher_message(message, lg, i, 0, n_emm, n_rec);
            i++;
        }
    }
    else
    {
        for (i = 0; i < nb; i++)
        {
            int reception = recvfrom(sock, message, lg, 0, &client, &lg_client);

            if (reception == -1)
            {
                printf("ERREUR : Echec de la réception\n");
                exit(1);
            }
            afficher_message(message, lg, i + 1, 0, n_emm, n_rec);
        }
    }

    //DESTROY
    if (close(sock) == -1)
    {
        printf("ERREUR: Le socket n'as pas été détruit\n\n");
    }
    else
    {
        printf("PUITS : fin\n\n");
    };
}

void sourceTCP(int port_dist, char *nom_ad_dist, int lg, int nb, char *alpha, int option, int n_emm, int n_rec)
{
    if (port_dist < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    shutdown(port_local,2); //eviter les erreurs de bind 

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

    for (i = 0; i < nb; i++)
    {
        construire_message(message, lg, i, i, alpha);
        if (option != 0)
        {
            int envoi = send(port, message, lg, MSG_OOB);
            if (envoi == -1)
            {
                printf("Erreur de l'envoi du message n°%d\n", i + 1);
            }
            else
                afficher_message(message, lg, i + 1, 1, n_emm, n_rec);
        }
        else
        {
            int envoi = send(port, message, lg, 0);
            if (envoi == -1)
            {
                printf("Erreur de l'envoi du message n°%d\n", i + 1);
            }
            else
                afficher_message(message, lg, i + 1, 1, n_emm, n_rec);
        }
    }

    //FIN DE CONNEXION
    if (!shutdown(port, 2))
    {
        printf("SOURCE : fin\n\n");
    }
}

void puitsTCP(int port_local, int lg, int nb, int option, int n_emm, int n_rec)
{
    if (port_local < 5000) { printf("[-b][-n ##][-l ##] [port]\nEntrer un port superieur a 5000 \n"); exit(2);}
    shutdown(port_local,2); //eviter les erreurs de bind 

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

    if (nb == -1)
    { //Si nombre de réceptions est infini : on passe par le fork, puis attente d'une nouvelle connexion
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
                        if ((lg_rcv = recv(sock_bis, message, lg, 0)) < 0)
                        {
                            perror("ERREUR : L'opération recv a échoué\n");
                        }
                        else
                        {
                            if (lg_rcv == 0)
                            {
                                if (!shutdown(sock, 2))
                                {
                                    printf("PUITS : fin\n\n");
                                }
                                exit(1);
                            }
                            afficher_message(message, lg, i + 1, 0, n_emm, n_rec);
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
    }
    else
    { //Sinon nombre de receptions fini => Programme père puis fin du programme
        printf("\n");

        if ((sock_bis = accept(sock, (struct sockaddr *)&ad_local, &lg_ad_local)) == -1)
        {
            printf("ERREUR: L'opération accept a échoué\n");
            exit(1);
        }

        for (i = 0; i < nb; i++)
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
                if ((lg_rcv = recv(sock_bis, message, lg, 0)) < 0)
                {
                    perror("ERREUR : L'opération recv a échoué\n");
                }
                else
                    afficher_message(message, lg, i + 1, 0, n_emm, n_rec);
            }
        }

        if (close(sock) == -1)
        {
            printf("ERREUR: Le socket n'as pas été détruit\n\n");
        }
        else
        {
            printf("PUITS : fin\n\n");
        };
    }
}

