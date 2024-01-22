#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct user USER; 
typedef struct u topN;
typedef struct Arvore_busca Arvore;

void init_table ();
int hashFunction (int chave);
unsigned long hashFunctionString(char* str);
void insert_user(USER user);
void remove_user(USER user);
USER findUser (char *str);
char *getIdUser (char *str);
int getIndex (char *str);
char *getLoginUser (char *str);
char *getFollowerList (char *str);
char *getFollowingList (char *str);
Arvore *getArvUser (char *str);
int getValid (char *str); 
void changeValid (char *str);
int isBot (char *str)
void getTop (int N, struct u* u1, int inicio, int fim, int i);
void print_table();
USER init_user(char *buffer);
int *mainUser (int *arr);