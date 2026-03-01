#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ETATS 50
#define MAX_TRANSITIONS 100

// Structure pour stocker UNE transition (ex: 0 -> 1 avec 'a')
typedef struct {
    int etat_source;
    char symbole;
    int etat_destination;
} Transition;

// Structure pour stocker tout l'automate
typedef struct {
    int etats[MAX_ETATS];
    int nb_etats;

    int etats_initiaux[MAX_ETATS];
    int nb_initiaux;

    int etats_finaux[MAX_ETATS];
    int nb_finaux;

    char alphabet[26];
    int nb_symboles;

    Transition transitions[MAX_TRANSITIONS];
    int nb_transitions;
} Automate;// Fonction pour initialiser un automate vide
void initialiser_automate(Automate *A) {
    A->nb_etats = 0; A->nb_initiaux = 0; A->nb_finaux = 0;
    A->nb_symboles = 0; A->nb_transitions = 0;
}

// Fonction simplifiée pour lire le fichier .dot
int charger_automate(char *fichier, Automate *A) {
   FILE *f = fopen(fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", nom_fichier);
        return 0; // Échec
    }

    initialiser_automate(A);
    char ligne[256];

    // On lit le fichier ligne par ligne
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int src, dest;
        char sym;

        // 1. Chercher les transitions : ex: "0 -> 1 [label="a"];"
        //strstr permet de chercher un caractere precis dans une chaine de caracteres
        if (strstr(ligne, "->") != NULL && strstr(ligne, "label") != NULL) {
                //faire extractio
            sscanf(ligne, "%d -> %d [label=\"%c\"]", &src, &dest, &sym);

            // Ajouter la transition
            A->transitions[A->nb_transitions].etat_source = src;
            A->transitions[A->nb_transitions].etat_destination = dest;
            A->transitions[A->nb_transitions].symbole = sym;
            A->nb_transitions++;
        }
        // 2. Chercher l'état initial : ex: "start -> 0;"
        else if (strstr(ligne, "start ->") != NULL) {
            sscanf(ligne, " start -> %d;", &dest);
            A->etats_initiaux[A->nb_initiaux++] = dest;
        }
        //double circle cad les etats qui suivent sont des etats finaux et le nombre apres est celcui l'etat final
        // 3. Chercher les états finaux : ex: "node [shape=doublecircle]; 1; 2;"
        else if (strstr(ligne, "doublecircle") != NULL) {
            // C'est une simplification, en réalité il faudrait parser les numéros après le ;
            // Pour l'exemple, on va extraire un entier de base s'il est présent à la fin
            int final_state;
            if(sscanf(ligne, " node [shape=doublecircle]; %d;", &final_state) == 1) {
               A->etats_finaux[A->nb_finaux++] = final_state;
            }
        }
    }
    fclose(fichier);
    // (Dans un vrai projet, il faudrait aussi remplir A->etats et A->alphabet dynamiquement)
    return 1; // Succès
}void afficher_automate(Automate A) {
    printf("\n--- Informations de l'Automate ---\n");

    printf("Etats initiaux : ");
    for (int i = 0; i < A.nb_initiaux; i++) {
        printf("q%d ", A.etats_initiaux[i]);
    }

    printf("\nEtats finaux : ");
    for (int i = 0; i < A.nb_finaux; i++) {
        printf("q%d ", A.etats_finaux[i]);
    }

    printf("\n\nListe des Transitions :\n");
    for (int i = 0; i < A.nb_transitions; i++) {
        printf("  q%d --(%c)--> q%d\n",
               A.transitions[i].etat_source,
               A.transitions[i].symbole,
               A.transitions[i].etat_destination);
    }
    printf("----------------------------------\n\n");
}int main() {
    Automate mon_automate;
    int choix;
    int automate_charge = 0; // Pour vérifier si un fichier a été lu

    do {
        printf("=== MENU PRINCIPAL ===\n");
        printf("1. Charger un automate depuis un fichier .dot\n");
        printf("2. Afficher l'automate\n");
        printf("0. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                // Pour tester, crée un fichier "test.dot" dans le même dossier
                if (charger_automate(fichier.dot, &mon_automate)) {
                    printf(">> Automate charge avec succes !\n\n");
                    automate_charge = 1;
                }
                break;
            case 2:
                if (automate_charge) {
                    afficher_automate(mon_automate);
                } else {
                    printf(">> Erreur : Veuillez charger un automate d'abord (Choix 1).\n\n");
                }
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf(">> Choix invalide.\n\n");
        }
    } while (choix != 0);

    return 0;
}
