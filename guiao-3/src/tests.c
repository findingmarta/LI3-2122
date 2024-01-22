#include <time.h>
#include <stdio.h>
#include "users.h"
#include "repos.h"
#include "commits.h"
#include "queries.h"
#include "testQuery.h"

//Função principal da parte relacionada com os testes das queries
int main () {
    double cpu_time_usedU, cpu_time_usedR, cpu_time_usedC;
    int u[4], c[4], total_repos, total_users;
    clock_t startR, endR;
    char *exitfile;
    
    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/tests_output.txt");
    FILE *etf = fopen (exitfile, "w");
    if (etf == NULL) printf ("Error opening exit file.\n");
    
    printf("\n------------ A EXECUTAR TESTES ------------\n");
    mainUser(u);
    cpu_time_usedU = (double)u[3] / CLOCKS_PER_SEC;
    total_users = u[0] + u[1] + u[2];

    startR = clock();
    total_repos=mainRepos();
    endR = clock();
    cpu_time_usedR = ((double)(endR - startR)) / CLOCKS_PER_SEC;
    
    mainCommit(c);
    cpu_time_usedC = (double)c[3] / CLOCKS_PER_SEC;

    printf("\n----------------- Query1 -----------------\n");
    queryOne(cpu_time_usedU,etf,u);
    printf("----------------- Query2 -----------------\n");
    queryTwo(cpu_time_usedR, cpu_time_usedC,etf,c, total_repos);
    printf("----------------- Query3 -----------------\n");
    queryThree(cpu_time_usedC,etf,c);
    printf("----------------- Query4 -----------------\n");
    queryFour(cpu_time_usedC, cpu_time_usedU,etf,c, total_users);
    printf("----------------- Query5 -----------------\n");
    queryFive(5,etf);
    printf("----------------- Query6 -----------------\n");
    querySix(6,etf);
    printf("----------------- Query7 -----------------\n");
    querySeven(7,etf);
    printf("----------------- Query8 -----------------\n");
    queryEight(8,etf);
    printf("----------------- Query9 -----------------\n");
    queryNine(9,etf);
    printf("----------------- Query10 ----------------\n");
    queryTen(10,etf);
    printf("\n---------------- CONCLUIDO ----------------\n");
    fclose(etf);
    free(exitfile);
    return 0;
}