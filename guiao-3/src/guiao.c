#include <assert.h>
#include <stdio.h>
#include "menu.h"
#include "users.h"
#include "repos.h"
#include "commits.h"
#include "queries.h"

//Função principal do programa
int main() {
    char buffer[20000];
    int total_users, total_repos, u[4], c[4];

    assert (system("clear")!= -1);
    printf("\n------------- A CRIAR HASHTABLES --------------\n");
    mainUser(u);
    total_repos = mainRepos();
    mainCommit(c);
    printf("\n-----------------  CONCLUIDO ------------------\n");
    total_users = u[0] + u[1] + u[2];
    FILE *data_file;
    data_file = fopen ("entrada/commands.txt", "r");   
    if (data_file == NULL) printf ("Error opening file.\n");
    if (!get_size("entrada/commands.txt")) executMenu(u, c, total_repos, total_users);
    for (int i=1; fgets(buffer, 20000, data_file); i++) executeQuery(buffer, u, c, i, total_repos, total_users, 0);
    fclose (data_file);
    return 0;
}
