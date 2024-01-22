#include "users.h"
#include "repos.h"
#include "param.h"
#include "commits.h"

#define T 451411 

struct Arvore_busca {
   int info;
   struct Arvore_busca *esq, *dir; 
};

struct repos {
    char *id;
    char *owner_id;
    char *full_name;
    char *license;
    char *has_wiki;
    char *description;
    char *language;
    char *default_branch;
    char *created_at;
    char *updated_at;
    char *forks_count;
    char *open_issues;
    char *stargazers_count;
    char *size;
    struct mensagens *message;

}; 

struct u {
    char *id;
    char *login;
    int r;
    struct u* prox;
};

struct mensagens {
    char *id;
    char *login;
    int r;
    struct mensagens* prox;
};

struct l {
    char *language;
    int r;
    struct l* prox;
};

REPOS hashTableR[T];

//Inicializa a tabela
void init_table3 () {
    int i;
    for(i=0; i<T; i++)
        hashTableR[i].id = "";
}

//Hash Function para gerar um índice 
int hashFunction3 (int chave) {
    return chave % T;
}

//Hash Function para gerar um índice a partir de uma string
unsigned long hashFunctionString3(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 

    return hash % T; 
}

//Inserir um repos na tabela
void insert_repos(REPOS repos) {
    unsigned long indice = hashFunctionString3(repos.id);
    while(hashTableR[indice].id != ""){
        indice = hashFunction3(indice+1);  //será do tipo unsigned long
    }
    hashTableR[indice] = repos; 
}

//Remove um repositorio da tabela
void remove_repos(REPOS repos) {
    unsigned long indice = hashFunctionString3(repos.id);
    while(hashTableR[indice].id != ""){
        if (strcmp(hashTableR[indice].id, repos.id) == 0) hashTableR[indice].id = "";
        else indice = hashFunction3(indice+1);
    }
}

//Pesquisa um repositório na tabela
REPOS *findRepos (char* str) {
    unsigned long indice = hashFunctionString3(str);
    while(hashTableR[indice].id != ""){
        if (strcmp(hashTableR[indice].id, str) == 0) return &hashTableR[indice];
        else indice = hashFunction3(indice+1);
    }      
}

//Devolve o id de um owner
char *getOwnerID(char *str){
    REPOS *r;
    r = findRepos(str);
    return r->owner_id;
}

//Devolve o top N de linguagens mais utilizadas a partir de uma data
void getTopLanguages (int N, int date, int i) {
    struct l* linguas = NULL, *linguas2 = NULL;
    for (int indice = 0; indice < T; indice++) {
        if ((hashTableR[indice].id != "") && (string2int(hashTableR[indice].created_at) > date)){
        insereLanguage (&linguas,hashTableR[indice].language);
        }
    }
    while (linguas) {
        ordenaLanguage (&linguas2, linguas, N);
        linguas = linguas->prox;
    }
    imprimeL (linguas2, N, i);

}

//Devolve a lista de repositórios inativos a partir de uma data
int inactiveRepos (int date, int i) {
    char *exitfile;

    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/commands%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");

    for (int indice = 0; indice < T; indice++) {
        if (hashTableR[indice].id != "") {
            if ((getVisited(hashTableR[indice].id) == 0) && (getDateCommit(hashTableR[indice].id, date) == 0)) {
                fprintf(ef, "%s;%s\n", hashTableR[indice].id,hashTableR[indice].description);
            }
        }
    }

    return 0; 
}

//Devolve o top N de utilizadores com commits de uma determinada linguagem
void getTopCommits (int N, topN *u1, char *language, int i) {
    int indice;
    int r;
    char *id, *login;
    Arvore *arv;
    for (indice = 0; indice < T; indice++) {
        if ((hashTableR[indice].id != "")&&(strcasecmp(hashTableR[indice].language,language)==0)){
            id = getIdUser(hashTableR[indice].owner_id);
            login =getLoginUser(hashTableR[indice].owner_id);
            arv = getArvUser(hashTableR[indice].owner_id);
            r = contaNos (arv);
            insereLL (&u1, r, id, login, N);
        }
    }
    
    imprimeNusers(u1,N,i);

}

/*
//Imprime o top N de mensagens
void printmensagens (struct mensagens* m, char*rid, int N, int k){
    int i;
    char *exitfile;

    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/commands%d_output.txt", k);
    FILE *ef = fopen (exitfile, "w");
    for (i=0; (i < N) && m;i++) {
        fprintf(ef, "%s;%s;%d;%s\n", m->id, m->login, m->r, rid);
         m=m->prox;
    }
}

//Devolve os utilizadores com maiores mensagens de commit por cada repositório
void getTopMensagens (int N, int i) {
    int indice;
    for (indice = 0; indice < T; indice++) {
        if (hashTableR[indice].id != ""){
            printmensagens (hashTableR[indice].message,hashTableR[indice].id, N, i);
        }
    }
}*/

//Imprime a tabela num ficheiro de saída
void print_table3(){
    int i;
    printf("\n---------------------- A CRIAR TABELA HASH REPOS ----------------------\n");
    FILE *out_file = fopen ("saida/repos-ok.csv", "w");  

    if (out_file == NULL) printf ("Error opening file.\n");                             

    fprintf (out_file, "id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size\n");

    for(i=0; i<T; i++) {
        if(hashTableR[i].id != "")
            fprintf (out_file, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", hashTableR[i].id, hashTableR[i].owner_id, hashTableR[i].full_name,hashTableR[i].license, hashTableR[i].has_wiki, hashTableR[i].description, hashTableR[i].language, hashTableR[i].default_branch, hashTableR[i].created_at, hashTableR[i].updated_at, hashTableR[i].forks_count, hashTableR[i].open_issues, hashTableR[i].stargazers_count, hashTableR[i].size); 
        else
            fprintf(out_file, "%2d =\n", i);
    }

    fclose(out_file);

    printf("\n-------------------------- CONCLUIDO ---------------------------------\n");

    return;
}

//Inicializar o repositório
REPOS init_repos(char *buffer) {
    REPOS u;
    char *aux = NULL;     
    int i = 1;                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1: 
                u.id = aux;
                break;
            case 2: 
                u.owner_id = aux; 
                break;
            case 3: 
                u.full_name = aux;   
                break;
            case 4: 
                u.license = aux; 
                break;
            case 5: 
                u.has_wiki = aux;
                break;
            case 6: 
                u.description = aux;
                break;
            case 7: 
                u.language = aux; 
                break;
            case 8:
                u.default_branch = aux; 
                break;
            case 9: 
                u.created_at = aux;
                break;
            case 10: 
                u.updated_at = aux;
                break;
            case 11: 
                u.forks_count = aux;
                break;
            case 12: 
                u.open_issues = aux;
                break;    
            case 13: 
                u.stargazers_count = aux;
                break; 
            case 14: 
                u.size = aux;
                break;         
            default:
                printf ("Error acessing aux!\n");
        } 
    }  

    return u;
}

//Função principal do módulo repos
int *mainRepos (int *arr) {
    char buffer[200000];
    REPOS repos;
    int total_repos = 0;
    
    init_table3();

    FILE *data_file = fopen ("entrada/repos.csv", "r");   

    if (data_file == NULL) printf ("Error opening file.\n"); 
    fgets(buffer, 200000, data_file);                         
    while(fgets (buffer, 200000, data_file)) {
        repos=init_repos(buffer);
        total_repos++;
        insert_repos(repos);        
    }


    fclose(data_file);

    print_table3();

    arr[0] = total_repos; 

    return arr;
}