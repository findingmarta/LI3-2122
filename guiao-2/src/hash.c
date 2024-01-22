#include <stdio.h>
#include "users.h"
#include "repos.h"
#include "param.h"
#include "commits.h"

int executeQuery (char *buffer, int *arrU, int *arrR, int *arrC, int i) {
	char *exitfile;
	char str1[11], str2[11], lang[10];
    int total_users, N, op, date, date1, date2, date3;
    total_users = arrU[0] + arrU[1] + arrU[2];

    exitfile = malloc(100*sizeof(char));
	sprintf(exitfile, "saida/commands%d_output.txt", i);
	FILE *ef = fopen (exitfile, "w");

    op = string2int(strdup(strsep(&buffer," ")));

    parseCommit();

	switch (op) {
			case 1:				
				fprintf(ef, "BOT: %d\n", arrU[1]);
				fprintf(ef, "Organization: %d\n", arrU[2]);
				fprintf(ef, "Users: %d\n", arrU[0]);
				break;
			case 2:
				fprintf(ef, "Avg: %.2f\n", (double)arrR[0] / (double)arrC[1]);
	 			break;
			case 3:
				fprintf(ef, "Total bots colaboradores: %d\n", arrC[2]);
				break;
			case 4:    
				fprintf(ef, "Avg: %.2f\n", (double)arrC[0] / (double)total_users);
				break;
			case 5:
				N = string2int(strdup(strsep(&buffer," ")));
				if (N < 0){
					printf("\nN inválido!\n");
					return 1;
				}
				date = string2int(strdup(strsep(&buffer," ")));
				date1 = string2int(strdup(strsep(&buffer," ")));
	   			if (date1 < date) {
   					printf("Intervalo de datas inválido!\n");
   					return 1;
   				}

   				topN *u = NULL;
   				getTop (N, u, date1, date2, i);

				break;
			case 6:
				N = string2int(strdup(strsep(&buffer," ")));
				if (N < 0){
					printf("\nN inválido!\n");
					return 1;
				}
				printf("\n Linguagem: ");
				scanf("%s", lang);

				//lang = (strdup(strsep(&buffer," ")));

				topN *u1 = NULL;
				getTopCommits (N, u1, lang, i);
				break;
			case 7:
				date = string2int(strdup(strsep(&buffer," ")));

				inactiveRepos(date, i);
				break;
			case 8:
				N = string2int(strdup(strsep(&buffer," ")));
				if (N < 0){
					printf("\nN inválido!\n");
					return 1;
				}
				date = string2int(strdup(strsep(&buffer," ")));
				getTopLanguages (N, date3, i);
				break;
			case 9:
				N = string2int(strdup(strsep(&buffer," ")));
				if (N < 0){
					printf("\nN inválido!\n");
					return 1;
				}
				topN *u2 = NULL;
				storeFriends(N, i, u2);
				break;
			case 10:
			/*  N = string2int(strdup(strsep(&buffer," ")));
				if (N < 0){
					printf("\nN inválido!\n");
					return 1;
				}

				getTopMensagens(N, i); */
				break;
			default:
				printf("Query inválida, tente novamente!\n");
				
		}

	fclose(ef);
}

//Função principal do programa
int main () {
	char buffer[20000];
    int *arrU, *arrR, *arrC;
    int u[2], r[2], c[3];

    arrU = mainUser(u);
    arrR = mainRepos(r);
    arrC = mainCommit(c);

    FILE *data_file;

	data_file = fopen ("entrada/commands.txt", "r");   
    if (data_file == NULL) printf ("Error opening file.\n");

	for(int i = 1; (fgets (buffer, 20000, data_file)); i++) {

		executeQuery(buffer, arrU, arrR, arrC, i);

	}	
	
	fclose (data_file);

	return 0;
}
