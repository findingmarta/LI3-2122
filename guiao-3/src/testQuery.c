#include "users.h"
#include "repos.h"
#include "commits.h"
#include "queries.h"
#include "testQuery.h"

//Função responsável por comparar dois ficheiros 
static int compareFiles (FILE *e, FILE *c){
    char c1 = getc(e);
    char c2 = getc(c);
    int r=1;
    while (c1 != EOF && c2 != EOF && r){
        if (c1!=c2) r=0;
        c1 = getc(e);
        c2 = getc(c);
    }
    return r;
}


//Função que executa o teste da Query 1
void queryOne(double cpu_time_usedU, FILE *etf, int arrU[]){
    FILE *expected, *produced;

    expected = fopen ("saida/expected1_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    produced = fopen ("saida/commands1_output.txt", "w");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    fprintf(produced, "Bot: %d\n", arrU[1]);
    fprintf(produced, "Organization: %d\n", arrU[2]);
    fprintf(produced, "Users: %d\n", arrU[0]);

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (Query 1): %.6f\n", cpu_time_usedU); 
    else fprintf(etf,"Didn't work! CPU Time (Query 1): %.6f\n", cpu_time_usedU);
}



//Função que executa o teste da Query 2
void queryTwo(double cpu_time_usedR, double cpu_time_usedC, FILE *etf, int arrC[], int total_repos){
    FILE *expected, *produced;

    expected = fopen ("saida/expected2_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    produced = fopen ("saida/commands2_output.txt", "w");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    fprintf(produced, "Avg %.2f\n", (double)arrC[1]/ (double)total_repos);

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (Query 2): %.6f\n", cpu_time_usedR+cpu_time_usedC); 
    else fprintf(etf,"Didn't work! CPU Time (Query 2): %.6f\n", cpu_time_usedR+cpu_time_usedC);
}

//Função que executa o teste da Query 3
void queryThree(double cpu_time_usedC, FILE *etf, int arrC[]){
    FILE *expected, *produced;

    expected = fopen ("saida/expected3_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    produced = fopen ("saida/commands3_output.txt", "w");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    fprintf(produced, "Total %d\n", arrC[2]);

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (Query 3): %.6f\n", cpu_time_usedC); 
    else fprintf(etf,"Didn't work! CPU Time (Query 3): %.6f\n", cpu_time_usedC);
}


//Função que executa o teste da Query 4
void queryFour(double cpu_time_usedC, double cpu_time_usedU, FILE *etf, int arrC[], int total_users){
    FILE *expected, *produced;

    expected = fopen ("saida/expected4_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    produced = fopen ("saida/commands4_output.txt", "w");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    fprintf(produced, "Avg %.2f\n", (double)arrC[0] / (double)total_users);

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (Query 4): %.6f\n", cpu_time_usedU+cpu_time_usedC); 
    else fprintf(etf,"Didn't work! CPU Time (Query 4): %.6f\n", cpu_time_usedU+cpu_time_usedC);
}


//Função que executa o teste da Query 5
void queryFive(int i, FILE *etf){
    double cpu_time_used, cpu_time_used_4Ns, min, max, time;
    clock_t start, end,start1, end1;
    int date, date1, N;
    FILE *expected, *produced;

    expected = fopen ("saida/expected5_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    date = 20100101;
    date1 = 20150101;

    start = clock();
    for (N=100; N>=25; N-=25){
        if (N!=25) getTop(N, date, date1, 0, 0);
        else getTop(N, date, date1, 5, 0);
    }
    end = clock();
    cpu_time_used_4Ns = (((double)(end - start))/4) / CLOCKS_PER_SEC;

    produced = fopen ("saida/commands5_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=[25,50,75,100]) (Query 5): %.6f\n", cpu_time_used_4Ns); 
    else fprintf(etf,"Didn't work! CPU Time (N=[25,50,75,100]) (Query 5): %.6f\n", cpu_time_used_4Ns);

    expected = fopen ("saida/expected5_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    N=100;
    start = clock();
    getTop(N, date, date1, 5, 0);
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        getTop(N, date, date1, 0, 0);
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;
    
    produced = fopen ("saida/commands5_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 5): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 5): %.6f\n", cpu_time_used);
    
}


//Função que executa o teste da Query 6
void querySix(int i, FILE *etf){
    double cpu_time_used, min, max, time;
    clock_t start, end, start1, end1;
    FILE *expected, *produced;
    int N, len;
    char lang[]="Python";

    expected = fopen ("saida/expected6_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}
    
    N=100;
    len = strlen(lang);
    if (len > 0 && lang[len-1] == '\n') lang[len-1] = '\0';
    
    start = clock();
    insereCommits(lang);
    getTopCommits(N, lang, 6, 0);
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        insereCommits(lang);
        getTopCommits(N, lang, 0, 0);
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;

    produced = fopen ("saida/commands6_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}
    
    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 6): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 6): %.6f\n", cpu_time_used);
    
}


//Função que executa o teste da Query 7
void querySeven(int i, FILE *etf){
    double cpu_time_used, min, max, time;
    clock_t start, end, start1, end1;
    int date;
    FILE *expected, *produced;

    expected = fopen ("saida/expected7_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}

    date = 20140425;

    start = clock();
    inactiveRepos(date, 7, 0);
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        inactiveRepos(date, 0, 0);
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;
    
    produced = fopen ("saida/commands7_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 7): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 7): %.6f\n", cpu_time_used);

}

//Função que executa o teste da Query 8
void queryEight(int i, FILE *etf){
    double cpu_time_used, min, max, time;
    clock_t start, end, start1, end1;
    int date, N;
    FILE *expected, *produced;

    expected = fopen ("saida/expected8_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}
    
    N = 100;
    date = 20161005;
    start = clock();
    getTopLanguages (N, date, 8, 0); 
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        getTopLanguages (N, date, 0, 0); 
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;
    
    produced = fopen ("saida/commands8_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}

    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 8): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 8): %.6f\n", cpu_time_used);
    
}

//Função que executa o teste da Query 9
void queryNine(int i, FILE *etf){
    double cpu_time_used, min, max, time;
    clock_t start, end, start1, end1;
    int N;
    FILE *expected, *produced;

    expected = fopen ("saida/expected9_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}
    
    N = 100;
    start = clock();
    storeFriends(N, 9, 0); 
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        storeFriends(N, 0, 0);
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;

    produced = fopen ("saida/commands9_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}
    
    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 9): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 9): %.6f\n", cpu_time_used);
}

//Função que executa o teste da Query 10
void queryTen(int i, FILE *etf){
    double cpu_time_used, min, max, time;
    clock_t start, end, start1, end1;
    int N;
    FILE *expected, *produced;

    expected = fopen ("saida/expected10_output.txt", "r");
    if (expected == NULL) {
        printf ("Error opening expected file.\n");
        return;}
    
    N = 100;
    start = clock();
    getTopMensagens(N, 10, 0); 
    end = clock();
    min=end-start;
    max=end-start;
    time=end-start;

    start1 = clock();
    for (int j=0; j<9; j++){
        start = clock();
        getTopMensagens(N, 0, 0);
        end = clock();
        if ((end-start) < min ) min = end-start;
        else if ((end-start) > max) max=end-start;
    }
    end1 = clock();
    
    cpu_time_used = ((((double)(end1 - start1))+((double)time)-((double)min)-((double)max))/8) / CLOCKS_PER_SEC;

    produced = fopen ("saida/commands10_output.txt", "r");
    if (produced == NULL) {
        printf ("Error opening produced file.\n");
        return;}
    
    if (compareFiles (expected, produced)) fprintf(etf,"It works! CPU Time (N=100) (Query 10): %.6f\n", cpu_time_used); 
    else fprintf(etf,"Didn't work! CPU Time (N=100) (Query 10): %.6f\n", cpu_time_used);
}
