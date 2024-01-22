#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TU 5945161

typedef struct user USER; 
typedef struct u topN;
typedef struct Arvore_busca Arvore;

char *getIdUser (char *str);
char *getLoginUser (char *str);
char *getFollowerList (char *str);
char *getFollowingList (char *str);
void changeTotalC (char *str);
int getValid (char *str); 
void changeValid (char *str);
void changeCommitLang (char *str);
int isBot (char *str);
void insereLL (struct u** u, int r, char *id, char *login, int N);
void insereFriend (int N, int i, int v);
void dateWithUser(char *str, int date);
void getTop (int N, int inicio, int fim, int i, int v);
void getTopCommits (int N, char *language, int i, int v);
int *mainUser (int arr[]);