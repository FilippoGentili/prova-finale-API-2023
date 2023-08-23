
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXVEICOLI 512

#define RED 0
#define BLACK 1

#define ADDS "aggiungi-stazione"
#define DELS "demolisci-stazione"
#define ADDA "aggiungi-auto"
#define DELA "rottama-auto"
#define PLAN "pianifica-percorso"
#define MAXCHAR 18

typedef struct c{
    int color;
    int autonomy;
    struct c *dad;
    struct c *left;
    struct c *right;
}car;

typedef struct st{
    int dist;
    car *root;
    struct st *next;
    struct st *prev;
}station;



station *head;
car *NIL;

//Gestione lista doppiamente concatenata
int insertL(station *l, int d);
int deleteL(station* l, int d);
int existsL(station* l, int d);
station* searchL(station* l, int d);
int length(station *l);
void printL(station *l);
void destroyL(station *l);

//Gestione RB tree
car* createNode(int a);
void insertRB(car **root, car *el);
void insertFixupRB(car **root, car *el);
void deleteRB(car **root, car *el);
void deleteFixupRB(car **root, car *el);
void leftRotate(car **root, car *el);
void rightRotate(car **root, car *el);
car *treeSuccessor(car *el);
int existsRB(car *root, int a);
car* searchRB(car *root, int a);
void destroyRB(car *root);


void addStation(station *l, int d);    //aggiunge una stazione alla distanza d, con n macchine di autonomia a[i]
void demolishStation(station *l, int d);                  //demolisce la stazione alla distanza d
void addCar(car **root, int a);                //aggiumge l'auto a alla stazione di distanza d
void scrapCar(car **root, int a);                 //rimuove l'auto a dalla stazione di distanza d
station *planRoute(station *l);        //trova il percorso più efficiente da d1 a d2
void rec_plan(station *l, station *route, station *def, int dest);
void rec_plan_back(station *l, station *route, station *def, int dest);
station *findReachable(station *l);
station *findReachable_back(station *l);
station *chooseBest(station *x, station *y);

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

    int s, n;
    char* in;
    station* tmp;
    in = malloc(sizeof(char)*(MAXCHAR+1));

    s = scanf(" %s", in);

    while(s>0){
        if(in[0] == 'a'){
            if(in[9] == 's'){
                s = scanf("%d", &n); //distanza
                addStation(head, n);
            }else if(in[9] == 'a'){
                s = scanf("%d", &n); //distanza
                tmp = searchL(head, n);
                s = scanf("%d", &n); //autonomia
                if(tmp != NULL)
                    addCar(&(tmp->root), n);
                else
                    printf("non aggiunta");
            }
        }else if(in[0] == 'd'){
            s = scanf("%d", &n); //distanza
            demolishStation(head, n);
        }else if(in[0] == 'r'){
            s = scanf("%d", &n); //distanza
            tmp = searchL(head, n);
            s = scanf("%d", &n); //autonomia
            if(tmp != NULL)
                scrapCar(&(tmp->root), n);
            else
                printf("non rottamata");
        }else if(in[0] == 'p'){
            s = scanf("%d", &n);
            tmp = searchL(head, n);
            tmp = planRoute(tmp);
            printL(tmp); //prendo in ingresso il nodo della stazione di partenza
        }else
            printf("error while reading\n");

        s = scanf(" %s", in);
    }

    destroyL(head);
    destroyRB(NIL);

    return 0;
}

//Gestione liste--------------------------------------------------------------------------------------------------------

int insertL(station* l, int d){
    station *tmp, *curr, *app;
    tmp = malloc(sizeof(station));
    if(tmp != NULL){
        tmp->dist = d;
        if(l == NULL){
            tmp->prev = NULL;
            tmp->next = NULL;
            l = tmp;
        }else{

            if(existsL(l, d) == 0){  //potrei controllare nel for successivo per diminuire i tempi
                for(curr=l; curr->next != NULL; curr = curr->next){
                    if(tmp->dist > curr->dist){ //ordine crescente
                        app = curr->next;
                        curr->next = tmp;
                        tmp->prev = curr;
                        tmp->next = app;
                        app->prev = tmp;
                        break;
                    }
                }
            }else
                return 0;
        }
    }else
        return 0;

    return 1;
}

int deleteL(station* l, int d){
    station *tmp;

    if(existsL(l, d) == 0 || l == NULL)
        return 0;
    else{
        for(tmp = l; tmp->next != NULL; tmp = tmp->next)
            if(tmp->dist == d){
                (tmp->prev)->next = tmp->next;
                (tmp->next)->prev = tmp->prev;
                free(tmp);
            }
    }

    return 1;
}

int existsL(station* l, int d){

    for(station *tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return 1;

    return 0;
}

station* searchL(station* l, int d){
    for(station *tmp = l; tmp->next != NULL; tmp = tmp->next)
        if(tmp->dist == d)
            return tmp;
    return NULL;
}

int length(station *l){
    int count=0;
    for(station *tmp = l; tmp!=NULL; tmp=tmp->next)
        count++;
    return count;
}

void printL(station *l){
    station *tmp = l;

    while(tmp->next != NULL)
        printf(" %d", tmp->dist);

    printf("\n");
}

void destroyL(station *l){
    station *succ;

    while(l != NULL){
        succ = l->next;
        destroyRB(l->root);
        free(l);
        l = succ;
    }
}

//Gestione RB tree------------------------------------------------------------------------------------------------------

car *createNode(int a){
    car* new = NULL;
    new = malloc(sizeof(car));
    new->left = NULL;
    new->right = NULL;
    new->dad = NULL;
    new->color = RED;
    new->autonomy = a;

    return new;
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
                    insertFixupRB(root, x->dad);
                }else{
                    if(el == x->right){
                        el = x;
                        leftRotate(root, el);
                        x = el->dad;
                    }
                    x->color = BLACK;
                    x->dad->color = RED;
                    rightRotate(root, el);
                }
            }else{
                y = x->dad->left;
                if(y->color == RED){
                    x->color = BLACK;
                    y->color = RED;
                    x->dad->color = RED;
                    insertFixupRB(root, x->dad);
                }else{
                    if(el == x->left){
                        el = x;
                        rightRotate(root, el);
                        x = el->dad;
                    }
                    x->color = BLACK;
                    x->dad->color = RED;
                    leftRotate(root, el);
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
        deleteFixupRB(root, x);

    free(el);
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
            leftRotate(root, el->dad);
            w = el->dad->right;
        }
        if(w->left->color == BLACK && w->right->color){
            w->color = RED;
            deleteFixupRB(root, el->dad);
        }else if(w->right->color == BLACK){
            w->left->color = BLACK;
            w->color = RED;
            rightRotate(root, w);
            w = el->dad->right;
        }
        w->color = el->dad->color;
        el->dad->color = BLACK;
        leftRotate(root, el->dad);
    }else{
        w = el->dad->left;
        if(w->color == RED){
            w->color = BLACK;
            el->dad->color = RED;
            rightRotate(root, el->dad);
            w = el->dad->left;
        }
        if(w->right->color == BLACK && w->left->color){
            w->color = RED;
            deleteFixupRB(root, el->dad);
        }else if(w->left->color == BLACK){
            w->right->color = BLACK;
            w->color = RED;
            leftRotate(root, w);
            w = el->dad->left;
        }
        w->color = el->dad->color;
        el->dad->color = BLACK;
        rightRotate(root, el->dad);
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

int existsRB(car *root, int a){
    int flag = 0;
    car *curr;
    curr = root;
    while(curr != NIL){
        if(curr->autonomy == a)
            flag = 1;
        else if(curr->autonomy < a)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return flag;
}

car* searchRB(car *root, int a){
    car *curr;
    curr = root;
    while(curr != NIL){
        if(curr->autonomy == a)
            return curr;
        else if(curr->autonomy < a)
            curr = curr->right;
        else
            curr = curr->left;
    }
    return NIL;
}

void destroyRB(car *root){

    if(root->left == NULL && root->right == NULL)
        free(root);
    else{
        if(root->left != NULL)
            destroyRB(root->left);

        if(root->right != NULL)
            destroyRB(root->right);
    }
}

//Other functions-------------------------------------------------------------------------------------------------------

void addStation(station *l, int d){

    int n, a, s;
    station *app;

    if(insertL(l, d) == 0){
        printf("non aggiunta\n");
    }else{

        s = scanf("%d", &n);
        if(s!=0){
            app = searchL(l, d);

            for(int i=0; i<n; i++){
                s = scanf("%d", &a);
                insertRB(&(app->root), createNode(a));
            }

            printf("aggiunta\n");
        }

    }
}

void demolishStation(station *l, int d){

    int flag;
    flag = deleteL(l, d);
    if(flag)
        printf("demolita\n");
    else
        printf("non demolita\n");
}

void addCar(car **root, int a){
    if(existsRB(*root, a) == 0){
        car *x = createNode(a);
        insertRB(root, x);
    }
    printf("aggiunta\n");
}

void scrapCar(car **root, int a){

    car *tmp = searchRB(*root, a);
    if(tmp != NIL){
        deleteRB(root, tmp);
        printf("rottamata");
    }else
        printf("non rottamata");
}

station* planRoute(station *l){
    int dest, s;
    station *curr;
    station *fin;

    curr = malloc(sizeof(station));
    curr->next = NULL;
    curr->prev = NULL;

    fin = malloc(sizeof(station));
    fin->next = NULL;
    fin->prev = NULL;

    insertL(curr, l->dist);
    s = scanf("%d", &dest);

    if(s!=0){
        if(l->dist < dest)
            rec_plan(l, curr, fin, dest);
        else
            rec_plan_back(l, curr, fin, dest);
    }

    return fin;
}

void rec_plan(station *l, station *route, station *def, int dest){

    station *curr = findReachable(l);

    if(existsL(curr, dest) == 1){
        insertL(route, dest);
        if(length(route) < length(def) || length(def) == 0){
            def = route;    //da fare la free del vecchio def?
        }else if(length(route) == length(def)){
            def = chooseBest(route, def);
        }
        return;
    }else{
        for(station *tmp = l; tmp != NULL; tmp = tmp->next){
            insertL(route, tmp->dist);
            rec_plan(tmp, route, def, dest);
            deleteL(route, tmp->dist);
        }
    }
}

void rec_plan_back(station *l, station *route, station *def, int dest){
    station *curr = findReachable_back(l);

    if(existsL(curr, dest) == 1){
        insertL(route, dest);
        if(length(route) < length(def) || length(def) == 0){
            def = route;    //da fare la free del vecchio def?
        }else if(length(route) == length(def)){
            def = chooseBest(route, def);
        }
        return;
    }else{
        for(station *tmp = l; tmp != NULL; tmp = tmp->prev){
            insertL(route, tmp->dist);
            rec_plan(tmp, route, def, dest);
            deleteL(route, tmp->dist);
        }
    }
}

station *findReachable(station *l){

    int in, a;
    car *tmp;
    station *reach = malloc(sizeof(station));
    reach->next = NULL;
    reach->prev = NULL;

    in = l->dist;
    tmp = l->root;
    while(tmp->right != NIL)
        tmp = tmp->right;

    a = tmp->autonomy;

    for(station *curr = l; (curr->dist - in) < a; curr = curr->next){
        insertL(reach, curr->dist);
    }

    return reach;
}

station *findReachable_back(station *l){

    int in, a;
    car *tmp;
    station *reach = malloc(sizeof(station));
    reach->next = NULL;
    reach->prev = NULL;

    in = l->dist;
    tmp = l->root;
    while(tmp->right != NIL)
        tmp = tmp->right;

    a = tmp->autonomy;

    for(station *curr = l; abs(curr->dist - in) < a; curr = curr->prev){
        insertL(reach, curr->dist);
    }

    return reach;
}

station *chooseBest(station *x, station *y){

    station *a = x;
    station *b = y;

    while(a->next != NULL){ //hanno stessa lunghezza
        a = a->next;
        b = b->next;
    }

    while(a->prev != NULL){
        if(a->dist != b->dist){
            if(a->dist < b->dist)
                return x;
            else
                return y;
        }
    }

    return NULL;
}