#include "library.h"

#include <stdio.h>
#include <stdlib.h>

#define MAXVEICOLI 512;

typedef struct st{
    int dist;
    car *root;
    st *next;
    st *prev;
}stations;

typedef struct car{
    int autonomia;
    car *dad;
    car *left;
    car *right;
}cars;

//Gestione lista doppiamente concatenata
stations* insertL(stations* l, int d);
stations* deleteL(stations* l, int d);
bool exists(stations* l, int d);
stations* searchL(stations* l, int d);

char* addStation(int d, int n, int* a);    //aggiunge una stazione alla distanza d, con n macchine di autonomia a[i]
char* demolishStation(int d);                  //demolisce la stazione alla distanza d
char* addCar(int d, int a);                //aggiumge l'auto a alla stazione di distanza d
char* scrapCar(int d, int a);                 //rimuove l'auto a dalla stazione di distanza d
void planRoute(int d1, int d2);        //trova il percorso più efficiente da d1 a d2

bool esiste_s(int d);                              //controlla se esiste una stazione alla distanza d
bool esiste_a(int d, int a);                       //controlla se esiste un'auto di autonomia a nella stazione d


int main(int argc, char* argv[]){

}

stations* insertL(stations* l, int d){
    stations *tmp, *curr, *app;
    tmp = malloc(sizeof(stations));
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
                return NULL;
        }
    }else
        printf("out of memory!\n");

    return l;
}

stations* deleteL(stations* l, int d){
    stations *tmp;

    if(!exists(l, d) || l == NULL)
        return NULL;
    else{
        for(tmp = l; tmp->next != NULL; tmp = tmp->next)
            if(tmp->dist == d){
                (tmp->prev)->next = tmp->next;
                (tmp->next)->prev = tmp->prev;
                free(tmp);
            }
    }

    return l;
}


bool exists(stations* l, int d){
    for(tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return true;

    return false;
}

stations* searchL(stations* l, int d){
    for(tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return tmp;
}


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

bool esiste_s(int d){

}

bool esiste_a(int d, int a){

}
