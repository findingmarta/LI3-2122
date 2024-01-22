#include "users.h"
#include "repos.h"
#include "queries.h"
#include "commits.h"
#include "valid.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

//Coloca o texto em negrito
static void bold(int status) {
 static const char *seq[] = {"\x1b[0m", "\x1b[1m"};
 printf("%s", seq[!!status]);
}

//Coloca o texto com a cor ciano
static void cyan(int status) {
 static const char *seq[] = {"\x1b[0m", "\033[1;96m"};
 printf("%s", seq[!!status]);
}


//Coloca o texto em itálico
static void italic(int status) {
 static const char *seq[] = {"\x1b[0m", "\x1b[3m"};
 printf("%s", seq[!!status]);
}

//Devolve o tamanho de um ficheiro
int get_size(const char* file_name){
    FILE *file = fopen(file_name, "r");
    if(file == NULL) return 0;
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);
    return size;
}

//Inicializa o array que irá armazenar o número de caracteres por página
static int *initArr (int arrP[], int totalP, int query, int N){
	char buffer[20000];
	int r=0;
	char *exitfile;

	exitfile = malloc(100*sizeof(char));
	sprintf (exitfile,"saida/commands%d_output.txt",query);
	FILE *data_file = fopen(exitfile, "r");
	arrP[0]=r;
	for(int i=1; i<totalP; i++){
		for (int j=1; j<=N && fgets(buffer, 20000, data_file); j++)
			r+=strlen(buffer);
		arrP[i]=r;
	}
	return arrP;
	free (exitfile);
}

//Função responsável por imprimir os outputs
static void printBuffer (FILE *data_file, int totalP, int offset, int acc, int N) {
	char buffer[20000];
	int i;

	cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
	printf("\n");
	fseek(data_file,offset,SEEK_SET);
	for (i=1; i<=N && fgets(buffer, 20000, data_file); i++) {
		if (strcmp(buffer,"\n")) printf("  %s", buffer);
	}
	cyan(1); printf ("\n_________________________________"); cyan(0);
	printf (" Página");
	printf (" %d", acc); 
	printf (" de"); 
	printf (" %d ", totalP-1);
	cyan(1); printf ("___________________________________\n"); cyan(0);
	printf("\n");
	bold(1);
	printf ("  P -> próxima\n");
    printf ("  A -> anterior\n");
    printf ("  S <N> -> saltar para uma página\n");
    printf ("  U -> saltar para a última página\n");
    printf ("  M -> Menu\n");
    bold(0);

}

//Função responsável por decidir o que fazer de acordo com o input do utilizador 
static void print_pag (int totalP, int query, int N){
	char escolha[10];
	int arrP[totalP];
	int w=1, acc=1;
	char *exitfile;

	exitfile = malloc(100*sizeof(char));
	sprintf (exitfile,"saida/commands%d_output.txt",query);
	FILE *data_file = fopen(exitfile, "r");
	initArr(arrP, totalP, query, N);
	setbuf(stdin, 0);
	printBuffer(data_file,totalP,0, 1, N);
	
	while (w){	
  		printf ("\n  Sua escolha: "); assert (fgets(escolha,10,stdin) > 0);
  		if ((escolha[1]!= ' ') && (escolha[0]=='S')) {
  			assert (system("clear")!= -1);
  			printBuffer(data_file,totalP,arrP[acc-1],acc, N);
  		}
  		else {
  			char *str1, *str2;
  			char *r = malloc(10);
  			strcpy (r,escolha);
  			char *f =r;
  			str1 = strsep(&r, " ");
  			str2 = strsep(&r, "\n");
    		assert (system("clear")!= -1);
			if (!strcmp(escolha,"P\n")){
  				acc++;
  				if(acc==totalP) acc--;
  				printBuffer(data_file,totalP,arrP[acc-1],acc, N);
  			}	
  			else if (!strcmp(escolha,"A\n")){
  				acc--;
  				if(acc==0) acc++;
  				printBuffer(data_file,totalP,arrP[acc-1], acc, N);
  				}			
   			else if (*str1=='S'){
   				int pag = atoi(str2);
  				int c = acc;
  				acc=pag;
  				if (acc > 0 && acc < totalP) printBuffer(data_file,totalP,arrP[acc-1], acc, N);
  				else {
  					printBuffer(data_file,totalP,arrP[c-1],c,N);
  					acc = c;
  				}
  			}
  			else if (!strcmp(escolha,"M\n")) w=0;
  			else if (!strcmp(escolha,"U\n")) {
  				acc=totalP-1;
  				printBuffer(data_file,totalP,arrP[totalP-2],acc, N);
  			}
  			else printBuffer(data_file,totalP,arrP[acc-1],acc, N);
 			free(f);
 		}
 	}
 	free(exitfile);
}

//Função que irá executar o pedido do usuário
int executMenu (int arrU[], int arrC[], int total_repos, int total_users) {
	int op, N, totalP, date, date1;
	char lang[10];
	char str1[11], str2[11];

	do {
		bold(1); italic(1);printf("\nInsira opcão (11 para mostrar menu): "); bold(0); italic(0);
		assert (scanf("%d", &op) > 0);

		switch (op) {
			case 0:
				assert (system("clear")!= -1);
				printf("\n");
				printf("Quitting...\n");
				printf("\n");
				break;
			case 1:
				assert (system("clear")!= -1);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				printf("\n  |1| Quantidade de bots, organizações e utilizadores\n"); bold(0);
				printf("\n  Bot: %d\n", arrU[1]);
				printf("  Organization: %d\n", arrU[2]);
				printf("  Users: %d\n", arrU[0]);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				break;
			case 2:
				assert (system("clear")!= -1);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |2| Número médio de colaboradores por repositório \n"); bold(0);
				printf("\n  Avg %.2f\n", (double)arrC[1] / (double)total_repos);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				break;
			case 3:
				assert (system("clear")!= -1);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |3| Quantidade de repositórios com bots  \n"); bold(0);
				printf("\n  Total %d\n", arrC[2]);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				break;
			case 4:    
				assert (system("clear")!= -1);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |4| Quantidade média de commits por utilizador \n"); bold(0);
				printf("\n  Avg %.2f\n", (double)arrC[0] / (double)total_users);
				cyan(1); printf("\n_________________________________________________________\n"); cyan(0);
				break;
			case 5:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |5| Top N de utilizadores mais ativos num determinado intervalo de datas \n"); bold(0);

				printf("\n  Número de utilizadores: ");
				assert (scanf("%d", &N) > 0);
				while (N < 0){
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |5| Top N de utilizadores mais ativos num determinado intervalo de datas \n"); bold(0);
					printf("\n  Número de utilizadores: ");
					assert (scanf("%d", &N) > 0);
				}
				
				printf("\n  Data inicial (AAAA-MM-DD): ");
				assert (scanf("%s", str1) > 0);
				while (!(validDate(str1))) {
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |5| Top N de utilizadores mais ativos num determinado intervalo de datas \n"); bold(0);
					printf("\n  Número de utilizadores: %d\n",N);
					printf("\n  Data inicial (AAAA-MM-DD): ");
					assert (scanf("%s", str1) > 0);
				}	
				
				printf("\n  Data Final (AAAA-MM-DD): ");
				assert (scanf("%s", str2) > 0);
				while (!(validDate(str2))) {
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |5| Top N de utilizadores mais ativos num determinado intervalo de datas \n"); bold(0);
					printf("\n  Número de utilizadores: %d\n",N);
					printf("\n  Data inicial (AAAA-MM-DD): %s\n",str1);
					printf("\n  Data Final (AAAA-MM-DD): ");
					assert (scanf("%s", str2) > 0);
				}	
				
				date = string2int (str1,1);
   				date1 = string2int (str2,1);
   				if (date1 < date) {
   					printf("\n  Datas inválidas!\n");
   					return 1;
   				}
   				
   				if(N%25 == 0) totalP = N/25;
   				else totalP = N/25+1;
   				
   				getTop (N, date, date1, 5,0);
   				print_pag (totalP+1,op,25);
				break;
			case 6:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |6| Top N de utilizadores com mais commits em repositórios de uma determinada linguagem \n"); bold(0);
				
				printf("\n  Número de utilizadores: ");
				assert (scanf("%d", &N) > 0);
				while (N < 0){
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |6| Top N de utilizadores com mais commits em repositórios de uma determinada linguagem \n"); bold(0);
					printf("\n  Número de utilizadores: ");
					assert (scanf("%d", &N) > 0);
				}
				
				printf("\n  Linguagem: ");
				assert (scanf("%s", lang) > 0);

				if(N%25 == 0) totalP = N/25;
   				else totalP = N/25+1;

   				insereCommits (lang);
				getTopCommits (N, lang, op, 0);
				print_pag (totalP+1,op,25);
				break;
			case 7:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |7| Repositórios inativos a partir de uma determinada data \n"); bold(0);
				
				printf("\n  Data (AAAA-MM-DD): ");
				assert (scanf("%s", str1) > 0);
				while (!(validDate(str1))) {
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |7| Repositórios inativos a partir de uma determinada data \n"); bold(0);
					printf("\n  Data (AAAA-MM-DD): ");
					assert (scanf("%s", str1) > 0);
				}	
				
				date = string2int (str1,1);
				
				N=inactiveRepos(date, op, 0);
				if(N%25 == 0) totalP = N/25;
   				else totalP = N/25+1;
				
				print_pag (totalP+1,op,25);
				break;
			case 8:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |8| Top N de linguagens mais utilizadas a partir de uma determinada data  \n"); bold(0);
				
				printf("\n  Número de linguagens: ");
				assert (scanf("%d", &N) > 0);
				while (N < 0){
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |8| Top N de linguagens mais utilizadas a partir de uma determinada data \n"); bold(0);
					printf("\n  Número de utilizadores: ");
					assert (scanf("%d", &N) > 0);
				}

				printf("\n  Data (AAAA-MM-DD): ");
				assert (scanf("%s", str1) > 0);
				while (!(validDate(str1))) {
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |8| Top N de linguagens mais utilizadas a partir de uma determinada data \n"); bold(0);
					printf("\n  Número de utilizadores: %d\n", N);
					printf("\n  Data (AAAA-MM-DD): ");
					assert (scanf("%s", str1) > 0);
				}	

				date = string2int (str1,1);

				if(N%25 == 0) totalP = N/25;
   				else totalP = N/25+1;

				getTopLanguages (N, date, op, 0);
				print_pag (totalP+1,op,25);
				break;
			case 9:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |9| Top N de utilizadores com mais commits em repositórios cujo owner é um amigo seu \n"); bold(0);
				printf("\n  Número de utilizadores: ");
				
				assert (scanf("%d", &N) > 0);
				while (N < 0){
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |9| Top N de utilizadores com mais commits em repositórios cujo owner é um amigo seu \n"); bold(0);
					printf("\n  Número de utilizadores: ");
					assert (scanf("%d", &N) > 0);
				}

				if(N%25 == 0) totalP = N/25;
   				else totalP = N/25+1;

               	storeFriends(N, op, 0);
               	print_pag (totalP+1,op,25);
               	break;
			case 10:
				assert (system("clear")!= -1);
				cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
				bold(1); printf("\n  |10| Top N de utilizadores com as maiores mensagens por cada repositório \n"); bold(0);
				printf("\n  Número de utilizadores: ");

				assert (scanf("%d", &N) > 0);
				while (N < 0){
					assert (system("clear")!= -1);
					cyan(1); printf("\n___________________________________________________________________________________\n"); cyan(0);
					bold(1); printf("\n  |10| Top N de utilizadores com as maiores mensagens por cada repositório  \n"); bold(0);
					printf("\n  Número de utilizadores: ");
					assert (scanf("%d", &N) > 0);
				}

				int n= getTopMensagens(N, op, 0);
   				if(n%N == 0) totalP = n/N;
   				else totalP = n/N+1;

   				print_pag (totalP+1,op,N);
				break;
			case 11:
				assert (system("clear")!= -1);
				cyan(1);printf(" __  __                         \n");
				cyan(0);printf("|  |/  |  _____   _____   _   _ \n");
				cyan(1);printf("|  __  | |  ___| |  _  | | | | |\n");
				cyan(0);printf("| |  | | |  _|_  | | | | | |_| |\n");
				cyan(1);printf("|_|  |_| |_____| |_| |_| |_____|\n");

   
				bold(0);cyan(0);
    			printf("\n ");	
    			printf("\n ___________________________________________________________________________________________ ");	
    			printf("\n|                                                                                           |");
    			printf("\n|  |1| Quantidade de bots, organizações e utilizadores                                      |");	
    			printf("\n|___________________________________________________________________________________________|");
    			printf("\n|                                                                                           |");	
    			printf("\n|  |2| Número médio de colaboradores por repositório                                        |");
    			printf("\n|___________________________________________________________________________________________|");	
    			printf("\n|                                                                                           |");	
    			printf("\n|  |3| Quantidade de repositórios com bots                                                  |");	
    			printf("\n|___________________________________________________________________________________________|");	
    			printf("\n|                                                                                           |");
    			printf("\n|  |4| Quantidade média de commits por utilizador                                           | ");	
    			printf("\n|___________________________________________________________________________________________|");
    			printf("\n|                                                                                           |");	
    			printf("\n|  |5| Top N de utilizadores mais ativos num determinado intervalo de datas                 | ");	
    			printf("\n|___________________________________________________________________________________________|");
    			printf("\n|                                                                                           |");	
    			printf("\n|  |6| Top N de utilizadores com mais commits em repositórios de uma determinada linguagem  | ");	
  				printf("\n|___________________________________________________________________________________________|");
  				printf("\n|                                                                                           |");	
    			printf("\n|  |7| Repositórios inativos a partir de uma determinada data                               | ");	
    			printf("\n|___________________________________________________________________________________________|");
    			printf("\n|                                                                                           |"); 	
    			printf("\n|  |8| Top N de linguagens mais utilizadas a partir de uma determinada data                 | ");	
    			printf("\n|___________________________________________________________________________________________|");
    			printf("\n|                                                                                           |");	
    			printf("\n|  |9| Top N de utilizadores com mais commits em repositórios cujo owner é um amigo seu     | ");
    			printf("\n|___________________________________________________________________________________________|");
				printf("\n|                                                                                           |");	
    			printf("\n|  |10| Top N de utilizadores com as maiores mensagens por cada repositório                 |");	
    			printf("\n|___________________________________________________________________________________________|");
				printf("\n|                                                                                           |");
    			printf("\n|  |0| Sair                                                                                 |");	
    			printf("\n|___________________________________________________________________________________________|\n");
    			printf("\n");
    			break;
			default:
				printf("Query inválida, tente novamente!\n");
				
		}
	}while(op != 0);
	return 0;
}