#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_E 50
#define MAX_T 100

// Structure d'une transition
typedef struct {
    int depart;
    char symbole;
    int arrivee;
} Transition;

// Structure principale de l'automate
typedef struct {
    int etats[MAX_E];
    int total_etats;

    int etats_depart[MAX_E];
    int total_depart;

    int etats_fin[MAX_E];
    int total_fin;

    char alphabet[26];
    int total_symboles;

    Transition transitions[MAX_T];
    int total_transitions;
} Systeme;


// Initialisation de l'automate
void initialiser(Systeme *S) {
    S->total_etats = 0;
    S->total_depart = 0;
    S->total_fin = 0;
    S->total_symboles = 0;
    S->total_transitions = 0;
}


// declaration d'une fonction pour charger l'automate
int charger_systeme(char *nom_fichier, Systeme *S) {

    FILE *f = fopen("C:\\Users\\douae\\OneDrive\\Documents\\theorieLangages\\dottest.txt", "r");
    //ttester si on a un echec dans l'ouverture du fichier
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier.\n");
        return 0;
    }
//appel de la fonction initialisation,pour initialiser l'automate
    initialiser(S);

    char ligne[256];
//lecture ligne par ligne de l'automate d'apres le fichier
    while (fgets(ligne, sizeof(ligne), f)) {
//initialisation des variable spour stocker la source ,la destionation et l'alphabet
        int src, dest;
        char sym;

        // Lecture des transitions
        //strstr permet de chercher une sous chaine a l'interieur d'une chaine
        if (strstr(ligne, "->") && strstr(ligne, "label")) {
//on fait une extraction avec sscanf,on extrait la source,la destionation et l'alphabet d'apres la transition trouvee
            if (sscanf(ligne, "%d -> %d [label=\"%c\"]", &src, &dest, &sym) == 3) {

                // Stocker transition
               // Donc on enregistre exactement la transition lue dans le fichier .dot.
                S->transitions[S->total_transitions].depart = src;
                S->transitions[S->total_transitions].arrivee = dest;
                S->transitions[S->total_transitions].symbole = sym;
                //incrementation de nombre de transitions
                S->total_transitions++;


             //   Après avoir ajoute la transition, le code verifie si les etats existent deja.
             // Ajouter les etats s'ils n'existent pas
                int existe = 0;

                for (int i = 0; i < S->total_etats; i++)
                //si on trouve la source,on met existe a 1
                    if (S->etats[i] == src) existe = 1;
//sinon on incremente le nombre d'etats,et on enregeistre la source trouve
                if (!existe)
                    S->etats[S->total_etats++] = src;
//meme chose pour la destination
                existe = 0;
                for (int i = 0; i < S->total_etats; i++)
                    if (S->etats[i] == dest) existe = 1;

                if (!existe)
                    S->etats[S->total_etats++] = dest;

                // Ajouter les symboles a l'alphabet
                //initialiser existe a 0
                existe = 0;
                //si le symbole est deja trouve ,on met existe a 1
                for (int i = 0; i < S->total_symboles; i++)
                    if (S->alphabet[i] == sym) existe = 1;
//sinon on incremente le nombre de symboles et on enregeistre l'alphabet trouve
                if (!existe)
                    S->alphabet[S->total_symboles++] = sym;
            }
        }

        // Etat initial
        else if (strstr(ligne, "start ->")) {
            if (sscanf(ligne, " start -> %d;", &dest) == 1)






            //si on a trouve start on fait l'extraction de l'etat de depart
                S->etats_depart[S->total_depart++] = dest;
        }

        // Etat final
        else if (strstr(ligne, "doublecircle")) {
            int fin;
            //si on trouve double circle,on fait l;extraction de l'etat finale
            if (sscanf(ligne, " node [shape=doublecircle]; %d;", &fin) == 1)
                S->etats_fin[S->total_fin++] = fin;
        }
    }
//fermer le fichier
    fclose(f);
    return 1;
}


// declaration d'une fonction pour affichage de l'automate
void afficher(Systeme S) {

    printf("\n--- Informations ---\n");
//affichage de setats
    printf("Tous les etats : ");
    for (int i = 0; i < S.total_etats; i++)
        printf("e%d ", S.etats[i]);
//affichage d'alphabets
    printf("\nAlphabet : ");
    for (int i = 0; i < S.total_symboles; i++)
        printf("%c ", S.alphabet[i]);
//affichage des etats initiaux
    printf("\n\nEtats initiaux : ");
    for (int i = 0; i < S.total_depart; i++)
        printf("e%d ", S.etats_depart[i]);
//affichage des etats finaux
    printf("\nEtats finaux : ");
    for (int i = 0; i < S.total_fin; i++)
        printf("e%d ", S.etats_fin[i]);
//affichage des transitions
    printf("\n\nTransitions :\n");
    for (int i = 0; i < S.total_transitions; i++) {
        printf("  e%d --(%c)--> e%d\n",
               S.transitions[i].depart,
               S.transitions[i].symbole,
               S.transitions[i].arrivee);
    }

    printf("--------------------\n\n");
}


// Menu principal
int main() {
//declarrer un automate
    Systeme mon_systeme;
    int choix;
    int charge = 0;

    do {
        printf("=== MENU ===\n");
        printf("1. Charger fichier\n");
        printf("2. Afficher\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {

            case 1:
                if (charger_systeme("C:\\Users\\douae\\OneDrive\\Documents\\theorieLangages\\dottest.txt", &mon_systeme)) {
                    printf(">> Chargement reussi !\n\n");
                    charge = 1;
                }
                break;

            case 2:
                if (charge)
                    afficher(mon_systeme);
                else
                    printf(">> Veuillez charger le fichier d'abord.\n\n");
                break;

            case 0:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide.\n\n");
        }

    } while (choix != 0);

    return 0;

    }
