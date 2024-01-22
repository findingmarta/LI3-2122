#include "guiao1-ex1.h"

//Função principal do exercício 1.

int main () {
    USER *user = doUser();  
    printf("A\n");
    doRepos();
    printf("B\n");
    doCommit();

    return 0;
}