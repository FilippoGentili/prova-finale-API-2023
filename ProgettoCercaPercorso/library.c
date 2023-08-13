#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVEICOLI 512;

#define RED 0
#define BLACK 1

#define ADDS "aggiungi-stazione"
#define DELS "demolisci-stazione"
#define ADDA "aggiungi-auto"
#define DELA "rottama-auto"
#define PLAN "pianifica-percorso"

typedef struct st{
    int dist;
    struct car *root;
    struct st *next;
    struct st *prev;
}station;

typedef struct c{
    int color;
    int autonomy;
    struct c *dad;
    struct c *left;
    struct c *right;
}car;

station *head;
car *NIL;

//Gestione lista doppiamente concatenata
void insertL(station *l, int d);
void deleteL(station* l, int d);
int exists(station* l, int d);
station* searchL(station* l, int d);

//Gestione RB tree
car* createNode(car **root, int a);
void insertRB(car **root, car *el);
void insertFixupRB(car **root, car *el);
void deleteRB(car **root, car *el);
void deleteFixupRB(car **root, car *el);
void leftRotate(car **root, car *el);
void rightRotate(car **root, car *el);
car *treeSuccessor(car *el);



char* addStation(int d, int n, int* a);    //aggiunge una stazione alla distanza d, con n macchine di autonomia a[i]
char* demolishStation(int d);                  //demolisce la stazione alla distanza d
char* addCar(int d, int a);                //aggiumge l'auto a alla stazione di distanza d
char* scrapCar(int d, int a);                 //rimuove l'auto a dalla stazione di distanza d
void planRoute(int d1, int d2);        //trova il percorso più efficiente da d1 a d2

//Main------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    head = malloc(sizeof(station));
    head->prev = NULL;
    head->next = NULL;

    NIL = malloc(sizeof(car));
    NIL->left = NULL;
    NIL->right = NULL;
    NIL->dad = NULL;
    head->root = NIL;









}

//Gestione liste--------------------------------------------------------------------------------------------------------

void insertL(station* l, int d){
    station *tmp, *curr, *app;
    tmp = malloc(sizeof(station));
    if(tmp != NULL){
        tmp->dist = d;
        if(l == NULL){
            tmp->prev = NULL;
            tmp->next = NULL;
            l = tmp;
        }else{

            if(!exists(l, d)){  //potrei controllare nel for successivo per diminuire i tempi
                for(curr=l; curr->next != NULL; curr = curr->next){
                    if(tmp->dist > curr->dist){
                        app = curr->next;
                        curr->next = tmp;
                        tmp->prev = curr;
                        tmp->next = app;
                        app->prev = tmp;
                        break;
                    }
                }
            }else
                printf("non aggiunta\n");
        }
    }else
        printf("non aggiunta\n");

    printf("aggiunta\n");
}

void deleteL(station* l, int d){
    station *tmp;

    if(!exists(l, d) || l == NULL)
        printf("non demolita\n");
    else{
        for(tmp = l; tmp->next != NULL; tmp = tmp->next)
            if(tmp->dist == d){
                (tmp->prev)->next = tmp->next;
                (tmp->next)->prev = tmp->prev;
                free(tmp);
            }
    }

    printf("demolita\n");
}

int exists(station* l, int d){

    for(station *tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return 1;

    return 0;
}

station* searchL(station* l, int d){
    for(station *tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return tmp;
}

//Gestione RB tree------------------------------------------------------------------------------------------------------

car *createNode(car **root, int a){

}

void insertRB(car **root, car *el){
    car *x = *root;
    car *y = NIL;

    while(x != NIL){
        y = x;
        if(el->autonomy < x->autonomy)
            x = x->left;
        else
            x = x->right;
    }
    el->dad = y;

    if(y == NIL)
        *root = el;
    else if(el->autonomy < y->autonomy)
        y->left = el;
    else
        y->right = el;

    el->left = NIL;
    el->right = NIL;
    el->color = RED;

    insertFixupRB(root, el);

    return;
}

void insertFixupRB(car **root, car *el){
    car *y = NIL;

    if(el == *root)
        (*root)->color = BLACK;
    else{
        car *x = el->dad;
        if(x->color == RED){
            if(x == x->dad->left){
                y = x->dad->right;
                if(y->color == RED){
                    x->color = BLACK;
                    y->color = RED;
                    x->dad->color = RED;
                    insertFixupRB(*root, x->dad);
                }else{
                    if(el == x->right){
                        el = x;
                        leftRotate(*root, el);
                        x = el->dad;
                    }
                    x->color = BLACK;
                    x->dad->color = RED;
                    rightRotate(*root, el);
                }
            }else{
                y = x->dad->left;
                if(y->color == RED){
                    x->color = BLACK;
                    y->color = RED;
                    x->dad->color = RED;
                    insertFixupRB(*root, x->dad);
                }else{
                    if(el == x->left){
                        el = x;
                        rightRotate(*root, el);
                        x = el->dad;
                    }
                    x->color = BLACK;
                    x->dad->color = RED;
                    leftRotate(*root, el);
                }
            }
        }
    }

}

void deleteRB(car **root, car *el){
    car *x, *y;

    if(el->left == NIL || el->right == NIL)
        y = el;
    else
        y = treeSuccessor(el);

    if(y->left != NIL)
        x = y->left;
    else
        x = y->right;
    x->dad = y->dad;

    if(y->dad == NIL)
        *root = x;
    else if(y == y->dad->left)
        y->dad->left = x;
    else
        y->dad->right = x;

    if(y != el)
        el->autonomy = y->autonomy;
    if(y->color == BLACK)
        deleteFixupRB(*root, x);
}

void deleteFixupRB(car **root, car *el){

    car *w;

    if(el->color == RED || el->dad == NIL)
        el->color = BLACK;
    else if(el == el->dad->right){
        w = el->dad->right;
        if(w->color == RED){
            w->color = BLACK;
            el->dad->color = RED;
            leftRotate(*root, el->dad);
            w = el->dad->right;
        }
        if(w->left->color == BLACK && w->right->color){
            w->color = RED;
            deleteFixupRB(*root, el->dad);
        }else if(w->right->color == BLACK){
            w->left->color = BLACK;
            w->color = RED;
            rightRotate(*root, w);
            w = el->dad->right;
        }
        w->color = el->dad->color;
        el->dad->color = BLACK;
        leftRotate(*root, el->dad);
    }else{
        w = el->dad->left;
        if(w->color == RED){
            w->color = BLACK;
            el->dad->color = RED;
            rightRotate(*root, el->dad);
            w = el->dad->left;
        }
        if(w->right->color == BLACK && w->left->color){
            w->color = RED;
            deleteFixupRB(*root, el->dad);
        }else if(w->left->color == BLACK){
            w->right->color = BLACK;
            w->color = RED;
            leftRotate(*root, w);
            w = el->dad->left;
        }
        w->color = el->dad->color;
        el->dad->color = BLACK;
        rightRotate(*root, el->dad);
    }
}

void leftRotate(car **root, car *el){
    car *y = el->right;

    el->right = y->left;
    if(y->left != NIL)
        y->left->dad = el;
    y->dad = el->dad;
    if(el->dad == NIL)
        *root = y;
    else if(el == el->dad->left)
        el->dad->right = y;
    else
        el->dad->left = y;

    y->left = el;
    el->dad = y;
}

void rightRotate(car **root, car *el){
    car *y = el->left;

    el->left = y->right;
    if(y->right != NIL)
        y->right->dad = el;
    y->dad = el->dad;
    if(el->dad == NIL)
        *root = y;
    else if(el == el->dad->right)
        el->dad->left = y;
    else
        el->dad->right = y;

    y->right = el;
    el->dad = y;
}

car *treeSuccessor(car *el){
    if(el->right != NIL){
        while(el->left != NIL)
            el = el->left;
        return el;
    }
    car *y = el->dad;
    while(y != NIL && el == y->right){
        el = y;
        y = y->dad;
    }
    return y;
}

//Other functions-------------------------------------------------------------------------------------------------------

char* aggiungiStazione(int d, int n, int* a){
    if(!esiste(d)){
        //do
        return "aggiunta";
    }
    else
        return "non aggiunta";

}

char* demolisciStazione(int d){
    if(esiste(d)){
        //do
        return "demolita";
    }else
        return "non demolita";
}

char* aggiungiAuto(int d, int a){
    if(esiste(d)){
        //do
        return "aggiunta";
    }else
        return "non aggiunta";
}

char* rottamaAuto(int d, int a){
    if(esiste_s(d) && ){
        //do
        return "rottamata";
    }else
        return "non demolita";
}
