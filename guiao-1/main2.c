#include "guiao1-ex2.h"

//Função principal do exercício 2.

int main () {
    char buffer [200000]; 

    USER *user = doUser();  

    int size_u = doUser2(user);
    REPOS *repos = doRepos2(user, size_u);
    FILE *data_file = fopen ("saida/repos-final.csv", "r");
    int size_r = getSize(buffer,data_file);
    printf("%d\n", size_r);
    printf("%d\n", size_u);
    doCommit2(user, repos, size_u, size_r);

    return 0;
}
