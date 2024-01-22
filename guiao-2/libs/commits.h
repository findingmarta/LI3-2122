#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct commit COMMIT;

void init_table2 ();
int hashFunction2 (int chave);
unsigned long hashFunctionString2(char* str);
void insert_commit(COMMIT commit);
void remove_commit(COMMIT commit);
COMMIT findCommit (char* str);
int getVisited (char *str);
void changeVisited (char *str);
int getDateCommit(char *id, int date);
void storeFriends (int N, int i, topN *u);
void print_table2();
COMMIT init_commit(char *buffer);
int *mainCommit (int *arr);
