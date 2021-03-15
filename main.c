/**
 * 
 * Author:    Christale Crouard
 * Created:   15.03.2021
 * 
 **/

#include "bal.h"
#include "liste.h"
#include "SoPu.h"

char alpha[MAX_LENGTH] = "abcdefghijklmnopqrstuvwxyz";

void main(int argc, char **argv){

    int c;
    extern char *optarg;
    extern int optind;
    int nb_message = -1;                              /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
    char *nombre = malloc(MAX_LENGTH * sizeof(char)); //pour l'affichage du nombre de message
    int lg_message = 20;
    int source = -1; /* 0=puits, 1=source 2=bal*/
    int tcp = 1;     // 0=udp, 1=tcp
    int option = 0;  //message prioritaire ou pas | pas eu le temps de développer cette partie
    int n_emm = 0;
    int n_rec = 0; //numéros d'emmetteur et de recepteur
    int bal = 0;   //option bal, par défaut non

    printf("\n#####################################################\n");
    printf("###################    TSOCK    #####################\n");
    printf("#####################################################\n\n");

    while ((c = getopt(argc, argv, "upbsn:l:r:e:")) != -1)
    {
        switch (c)
        {
        case 'p':
            if (source == 1)
            {
                printf("usage: cmd [-p|-s][-n ##][nom][port##]\n");
                exit(1);
            }
            source = 0;
            break;

        case 's':
            if (source == 0)
            {
                printf("usage: cmd [-p|-s][-n ##][nom][port##]\n");
                exit(1);
            }
            source = 1;
            break;

        case 'u':
            if (tcp == 1)
                tcp = 0;
            break;

        case 'n':
            nb_message = atoi(optarg);
            break;

        case 'l':
            lg_message = atoi(optarg);
            break;

        case 'r':
            if (tcp == 0)
            {
                printf("usage: cmd [-e##][nom][port##]\n");
                exit(1);
            }
            n_rec = atoi(optarg);
            n_emm = 0;
            source = 0;
            break;

        case 'e':
            if (tcp == 0 || bal != 0)
            {
                printf("usage: cmd [-e##][-n##][-l##][nom][port##]\n");
                exit(1);
            }
            tcp = 1;
            n_emm = atoi(optarg);
            n_rec = 0;
            source = 1;
            break;

        case 'b':
            if (tcp == 0 || n_rec != 0 || n_rec != 0)
            {
                printf("usage: cmd [-b][port##]\n");
                exit(1);
            }
            bal = tcp = 1;
            source = 2;
            break;

        default:
            printf("usage: cmd [-p][-s][-n ##][-l ##]\n");
            exit(1);
            break;
        }
    }

    if (source == -1)
    {
        printf("usage: cmd [-p][-s][-n ##][-l ##]\n");
        exit(1);
    }

    if (tcp == 1)
        printf("Protocole                    : TCP\n");
    else
        printf("Protocole                    : UDP\n");

    if (source == 1)
    {
        printf("Mode                         : SOURCE\n");
    }
    else if (source == 0)
    {
        printf("Mode                         : PUITS\n");
    }
    else if (source == 2)
    {
        printf("Mode                         : BOITE AUX LETTRES\n");
    }

    if (nb_message != -1)
    {
        if (source == 1)
        {
            printf("Nombre de tampons à envoyer  : %d\n\n", nb_message);
        }
        else
            printf("Nombre de tampons à recevoir : %d\n\n", nb_message);
    }
    sprintf(nombre, "%d", nb_message);

    if (nb_message == -1)
    {
        if (source == 1)
        {
            nb_message = 10;
            printf("Nombre de tampons à envoyer  : 10 (par défaut)\n\n");
            sprintf(nombre, "%d", nb_message);
        }

        else
            printf("Nombre de tampons à recevoir : infini\n\n");
        strcpy(nombre, "infini");
    }

    if (source == 1 && tcp == 0 && n_emm == 0 && n_rec == 0 && bal == 0)
    {
        printf("SOURCE : lg_msg_emis=%d, port=%d, nb_envois=%s, TP=udp, dest=%s\n", lg_message, atoi(argv[argc - 1]), nombre, argv[argc - 2]);
        sourceUDP(atoi(argv[argc - 1]), argv[argc - 2], lg_message, nb_message, alpha, n_emm, n_rec);
    }
    else if (source == 0 && tcp == 0 && n_emm == 0 && n_rec == 0 && bal == 0)
    {
        printf("PUITS : lg_msg_recus=%d, port=%d, nb_receptions=%s, TP=udp\n", lg_message, atoi(argv[argc - 1]), nombre);
        puitsUDP(atoi(argv[argc - 1]), lg_message, nb_message, n_emm, n_rec);
    }

    else if (source == 1 && tcp == 1 && n_emm == 0 && n_rec == 0 && bal == 0)
    {
        printf("SOURCE : lg_msg_emis=%d, port=%d, nb_envois=%s, TP=tcp, dest=%s\n", lg_message, atoi(argv[argc - 1]), nombre, argv[argc - 2]);
        sourceTCP(atoi(argv[argc - 1]), argv[argc - 2], lg_message, nb_message, alpha, option, n_emm, n_rec);
    }
    else if (source == 0 && tcp == 1 && n_emm == 0 && n_rec == 0 && bal == 0)
    {
        printf("PUITS : lg_msg_recus=%d, port=%d, nb_receptions=%s, TP=tcp\n", lg_message, atoi(argv[argc - 1]), nombre);
        puitsTCP(atoi(argv[argc - 1]), lg_message, nb_message, option, n_emm, n_rec);
    }
    else if (bal = 1 && n_emm == 0 && n_rec == 0)
    {
        printf("PUITS : lg_msg_recus=%d, port=%d, TP=tcp\n", lg_message, atoi(argv[argc - 1]));
        printf("on effectue le programme bal\n");
        boite(atoi(argv[argc - 1]), lg_message, nb_message, option, n_emm, n_rec);
    }
    else if (bal == 0 && n_emm != 0 && n_rec == 0 && source == 1)
    {
        printf("SOURCE : lg_mesg_emis=%d, port=%d, nb_envois=%s, TP=tcp, dest=%s\n", lg_message, atoi(argv[argc - 1]), nombre, argv[argc - 2]);
        emmeteurTCP(atoi(argv[argc - 1]), argv[argc - 2], lg_message, nb_message, alpha, option, n_emm, n_rec);
    }
    else if (bal == 0 && n_emm == 0 && n_rec != 0)
    {
        printf("RECEPTION : Demande de récupération de ses lettres par le récepteur %d port=%d, TP=tcp\n", n_rec, atoi(argv[argc - 1]));
        recepteurTCP(atoi(argv[argc - 1]), argv[argc - 2], lg_message, nb_message, option, n_emm, n_rec);
    }
    /* char src[40];
    char dest[40];
    char *pdu = malloc(20 * sizeof(char));

    //strcpy(src, "r00002n00010l00020");
    //printf("Final copied string : %s\n",Sub_Char_Array(src, dest, 13));
    //printf("Final pdu : %s\n",

    //printf("%s",pduMaker(0,5,12,55,pdu)); */
}

//compile : gcc -W tsock_v1.c -o prog
// run : ./prog -s -u -n# -l# insa-202## ####
