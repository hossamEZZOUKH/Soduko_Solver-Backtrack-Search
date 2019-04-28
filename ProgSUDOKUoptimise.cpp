#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

////////////////////////////////////////////
//    implémentation de la liste
////////////////////////////////////////////

typedef struct _list
{
    int i, j;
    int nbValeursPossibles;
    struct _list *next;
} LIST;

// retourne un nouvel élément initialisé
static LIST* new_elem (int i, int j, int n)
{
    LIST* ret = (LIST*) malloc(sizeof* ret);
    if (ret != NULL)
        ret->i = i, ret->j = j, ret->nbValeursPossibles = n, ret->next = NULL;
    return ret;
}

// supprime intégralement une liste chainée
void liste_delete (LIST** list)
{
    LIST* tmp;
    while ( (tmp = *list) != NULL)
        *list = (*list)->next, free(tmp);
}

// ajoute en tête
void liste_cons (LIST** list, int i, int j, int n)
{
    LIST* elem = new_elem (i, j, n);
    if (elem != NULL)
        elem->next = *list, *list = elem;
}

// insertion dans une liste triée
void insertion (LIST** list, LIST* elem)
{
    if (*list == NULL)
        *list = elem, elem->next = NULL;
    else if ((*list)->nbValeursPossibles < elem->nbValeursPossibles)
        insertion (&(*list)->next, elem);
    else
        elem->next = *list, *list = elem;
}

// tri insertion sur liste chainée
LIST* list_sort (LIST* list)
{
    LIST *curr, *list2 = NULL, *tmp;
    for (curr = list; curr != NULL; curr = tmp)
    {
        tmp = curr->next;
        insertion (&list2, curr);
    }
    return list2;
}


// Variables globales (tableaux) pour la mémorisation
bool existeSurLigne[9][9];
bool existeSurColonne[9][9];
bool existeSurBloc[9][9];


bool estValide (int grille[9][9], LIST* position)
{
    if (position == NULL)
        return true;

    int i = position->i, j = position->j;

    for (int k=0; k < 9; k++)
    {
        // Vérifie dans les tableaux si la valeur est déjà présente
        if ( !existeSurLigne[i][k] && !existeSurColonne[j][k] && !existeSurBloc[3*(i/3)+(j/3)][k] )
        {
            // Ajoute k aux valeurs enregistrées
            existeSurLigne[i][k] = existeSurColonne[j][k] = existeSurBloc[3*(i/3)+(j/3)][k] = true;

            if ( estValide(grille, position->next) )
            {
                // Ecrit le choix valide dans la grille
                grille[i][j] = k+1;
                return true;
            }
            // Supprime k des valeurs enregistrées
            existeSurLigne[i][k] = existeSurColonne[j][k] = existeSurBloc[3*(i/3)+(j/3)][k] = false;
        }
    }

    return false;
}

// Calcule le nombre de valeurs possibles pour une case vide
int nb_possibles (int grille[9][9], int i, int j)
{
    int ret = 0;
    for (int k=0; k < 9; k++)
        if ( !existeSurLigne[i][k] && !existeSurColonne[j][k] && !existeSurBloc[3*(i/3)+(j/3)][k] )
            ret++;
    return ret;
}

bool resolution (int grille[9][9])
{
    // Initialise les tableaux
    for (int i=0; i < 9; i++)
        for (int j=0; j < 9; j++)
            existeSurLigne[i][j] = existeSurColonne[i][j] = existeSurBloc[i][j] = false;

    // Enregistre dans les tableaux toutes les valeurs déjà présentes 
    int k;
    for (int i=0; i < 9; i++)
        for (int j=0; j < 9; j++)
            if ( (k = grille[i][j]) != 0)
                existeSurLigne[i][k-1] = existeSurColonne[j][k-1] = existeSurBloc[3*(i/3)+(j/3)][k-1] = true;

    // crée et remplit une liste pour les cases vides à visiter
    LIST* positions = NULL;
    for (int i=0; i < 9; i++)
        for (int j=0; j < 9; j++)
            if ( grille[i][j] == 0 )
                liste_cons ( &positions, i, j, nb_possibles(grille, i, j) );

    // Trie la liste (ordre croissant)
    positions = list_sort (positions);

    // Appelle la fonction de backtracking récursive estValide()
    bool ret = estValide (grille, positions);
    // Détruit la liste
    liste_delete (&positions);
    // retourne le resultat
    return ret;
}
// Fonction d'affichage
void affichage (int grille[9][9])
{
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
        {
            printf( ((j+1)%3) ? "%d " : "%d|", grille[i][j]);
        }
        putchar('\n');
        if (!((i+1)%3))
            puts("------------------");
    }
    puts("\n\n");
}
int main (void)
{
    int grille[9][9] =
    {
        {9,0,0,1,0,0,0,0,5},
        {0,0,5,0,9,0,2,0,1},
        {8,0,0,0,4,0,0,0,0},
        {0,0,0,0,8,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,2,6,0,0,9},
        {2,0,0,3,0,0,0,0,6},
        {0,0,0,2,0,0,9,0,0},
        {0,0,1,9,0,4,5,7,0}
    };

    printf("Grille avant\n");
    affichage(grille);

    estValide(grille,0);

    printf("Grille apres\n");
    affichage(grille);
}

