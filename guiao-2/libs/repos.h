#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct repos REPOS;
typedef struct l LANG;
typedef struct mensagens MESS;

void init_table3 ();
int hashFunction3 (int chave);
unsigned long hashFunctionString3(char* str);
void insert_repos(REPOS repos);
void remove_repos(REPOS repos);
void insert_repos(REPOS repos);
REPOS *findRepos (char *str);
char *getOwnerID(char *str);
void getTopLanguages (int N, int date, int i);
int inactiveRepos (int date, int i);
void getTopCommits (int N, topN *u1, char *language, int i);
void printmensagens (struct mensagens* m, char*rid, int N, int k);
void getTopMensagens (int N, int i);
void print_table3();
REPOS init_repos(char *buffer);
int *mainRepos (int *arr);