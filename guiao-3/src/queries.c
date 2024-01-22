#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "users.h"
#include "repos.h"
#include "valid.h"
#include "commits.h"

//Função que remove determinado char de uma string
void removeChar(char * str, char charToRemmove){
    int i, j;
    int len = strlen(str);
    for(i=0; i<len; i++) {
        if(str[i] == charToRemmove) {
            for(j=i; j<len; j++) str[j] = str[j+1];
            len--;
            i--;
        }
    }
}

//Função que transforma uma string data num inteiro
int string2int (char *str, int isDate){
    if (isDate==1) removeChar (str, '-');
    int num = atoi(str);
    return num;
}	

//Função que executa uma query
int executeQuery (char *buffer, int arrU[], int arrC[], int comando, int total_repos, int total_users, int v) {
	char *exitfile;
	char lang[200], str[200], str1[200];
    int N, op, date, date1;

    exitfile = malloc(100*sizeof(char));
    if (v==0) sprintf(exitfile, "saida/commands%d_output.txt", comando);
	else sprintf(exitfile, "saida/expected%d_output.txt", comando);
	FILE *ef = fopen (exitfile, "w");
    if (ef == NULL) printf ("Error opening exit file.\n");
    op = string2int(strsep(&buffer," "),0);

	switch (op) {
			case 1:				
				fprintf(ef, "Bot: %d\n", arrU[1]);
				fprintf(ef, "Organization: %d\n", arrU[2]);
				fprintf(ef, "Users: %d\n", arrU[0]);
				break;
			case 2:
				fprintf(ef, "Avg %.2f\n", (double)arrC[1]/ (double)total_repos);
	 			break;
			case 3:
				fprintf(ef, "Total %d\n", arrC[2]);
				break;
			case 4:
				fprintf(ef, "Avg %.2f\n", (double)arrC[0] / (double)total_users);
				break;
			case 5:                                                
				N = string2int(strsep(&buffer," "),0);
				if (N < 0){
					printf("N inválido!\n");
					return 1;}

				strcpy(str,strsep(&buffer," "));
				strcpy(str1,strsep(&buffer," "));
				if(!validDate(str) || !validDate(str1)) {
   					printf("Data(s) inválida(s)!\n");
					return 1;}
				date = string2int(str,1);
				date1 = string2int(str1,1);
	   			if (date1 < date) {
   					printf("Intervalo de datas inválido!\n");
   					return 1;}

   				getTop (N, date, date1, comando, v);
				break;
			case 6:                                              
				N = string2int(strsep(&buffer," "),0);
				if (N < 0){
					printf("N inválido!\n");
					return 1;}
				strcpy(lang, strsep(&buffer," "));
				int len = strlen(lang);
				if (len > 0 && lang[len-1] == '\n') lang[len-1] = '\0'; 

				insereCommits (lang);
				getTopCommits(N, lang, comando, v);
				break;
			case 7:
				strcpy(str,strsep(&buffer," "));
				if(!validDate(str)) {
   					printf("Data inválida!\n");
					return 1;}
				date = string2int(str,1);

				inactiveRepos(date, comando, v);
				break;
			case 8:
				N = string2int(strsep(&buffer," "),0);
				if (N < 0){
					printf("N inválido!\n");
					return 1;}
				strcpy(str,strsep(&buffer," "));
				if(!validDate(str)) {
   					printf("Data inválida!\n");
					return 1;}
				date = string2int(str,1);

				getTopLanguages (N, date, comando, v);
				break;
			case 9:                                           
				N = string2int(strsep(&buffer," "),0);
				if (N < 0){
					printf("N inválido!\n");
					return 1;}

				storeFriends(N, comando, v);
				break;
			case 10:                                         
				N = string2int(strsep(&buffer," "),0);
				if (N < 0){
					printf("N inválido!\n");
					return 1;}
					
				getTopMensagens(N, comando, v);
				break;
			default:
				printf("Query inválida, tente novamente!\n");
		}
	fclose(ef);
	return 0;
}

