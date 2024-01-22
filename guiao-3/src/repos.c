#include "users.h"
#include "repos.h"
#include "valid.h"
#include "queries.h"
#include "commits.h"

struct repos {
    int date;
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

struct mensagens {
    char *id;
    char *login;
    char *rid;     
    int r;
    struct mensagens* prox;
};

struct l {
    char *language;
    int r;
    struct l* prox;
};


static REPOS hashTableR[TR];

//Inicializa a tabela
static void init_table () {
    int i;
    for(i=0; i<TR; i++)
        hashTableR[i].id = "";
}

//Hash Function para gerar um índice 
static int hashFunction (int chave) {
    return chave % TR;
}

//Hash Function para gerar um índice a partir de uma string
static unsigned long hashFunctionString(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 
    return hash % TR; 
}

//Inserir um repos na tabela
static void insert_repos(REPOS repos) {
    unsigned long indice = hashFunctionString(repos.id);
    while(strcmp(hashTableR[indice].id, "")){
        indice = hashFunction(indice+1);
    }
    hashTableR[indice] = repos; 
}

//Pesquisa um repositório na tabela
static REPOS *findRepos (char* str) {
    int r=0;
    unsigned long indice = hashFunctionString(str);
    while((strcmp(hashTableR[indice].id, "")!=0) && r==0) {
        if (strcmp(hashTableR[indice].id, str) == 0) r=1;
        else indice = hashFunction(indice+1);
    }
    return &hashTableR[indice];
}

//Remove linhas de um repositório
void removeReposLines() {
    char *repos_id=NULL;
    for (int i=0; i<TR; i++){
        repos_id = getReposId(hashTableR[i].id);
        if (strcmp(hashTableR[i].id, "") && !strcmp(repos_id, "")) hashTableR[i].id = "";
        if (strcmp(repos_id, "") ) free(repos_id);
    }
}

//Atualiza o campo data do repositório com uma mais recente
void changeRepos(int date, char *str){
     REPOS *r;
     r = findRepos(str);
     if(strcmp(r->id,"")==0) return;
     if ((r->date)<date) r->date = date;
}

//Devolve o id de um repos
char *getId(char *str){
    REPOS *r;
    r = findRepos(str);
    if(strcmp(r->id,"")==0) return "";
    return strdup(r->id);
}

//Devolve o id de um owner
char *getOwnerID(char *str){
    REPOS *r;
    r = findRepos(str);
    if(strcmp(r->id,"")==0){
        printf("ID do owner não encontrado.\n");
        return "";}
    return strdup(r->owner_id);
}

//Devolve a language
char *getLanguage(char *str){
    REPOS *r;
    r = findRepos(str);
    if(strcmp(r->id,"")==0){
        printf("Language não encontrada.\n");
        return "";}
    return strdup(r->language);
}

//Função que irá imprimir uma struct de linguagens
static void imprimeL (struct l *ling, int N, int i, int v){
    char *exitfile;
    exitfile = malloc(100*sizeof(char));
    if (v==0) sprintf(exitfile, "saida/commands%d_output.txt", i);
    else sprintf(exitfile, "saida/expected%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");
    for (int i = 0; i<N && (ling); i++){
        fprintf (ef, "%s\n", ling->language);
        ling = ling->prox;
    }
  fclose(ef);  
  free(exitfile);     
}

//Função responsável por recolher as linguagens e as suas respetivas ocorrências
static void insereLanguage (struct l** ling, char *language){
    int i;
    for (i=0; (*ling); i++){
        if (strcmp(((*ling)->language), language) == 0){
            (*ling)->r++;
            return;
        }
        else ling = &((*ling)->prox);
    }
        (*ling)=malloc(sizeof (struct l));
        (*ling)->r = 1;
        (*ling)->language = language;
        (*ling)->prox = NULL;
}


//Irá ordenar as linguagens em ordem decrescente de utilização
static void ordenaLanguage (struct l** linguas2, struct l* linguas, int N){
    int i;
    for (i = 0; i<N && (*linguas2) && (((*linguas2)->r) >= (linguas->r)); i++) linguas2 = &((*linguas2)->prox);
    if (i<N){
        struct l* u = *linguas2;
        (*linguas2) = malloc (sizeof(struct l));
        (*linguas2)->r = (linguas)->r;
        (*linguas2)->language = (linguas)->language;
        (*linguas2)->prox = u;
    }
}

//Devolve o top N de linguagens mais utilizadas a partir de uma data
void getTopLanguages (int N, int date, int i, int v) {
    struct l* linguas = NULL, *linguas2 = NULL;
    for (int indice = 0; indice < TR; indice++) {
        if ((strcmp(hashTableR[indice].id, "")) && (string2int(hashTableR[indice].created_at,1) > date)){
            insereLanguage (&linguas,hashTableR[indice].language);
        }
    }
    while (linguas) {
        ordenaLanguage (&linguas2, linguas, N);
        linguas = linguas->prox;
    }
    if(i!=0) imprimeL (linguas2, N, i, v);
    free(linguas);
    free(linguas2);
}

//Devolve a lista de repositórios inativos a partir de uma data
int inactiveRepos (int date, int i, int v) {
    int totalLinhas=0;
    char *exitfile;
    exitfile = malloc(100*sizeof(char));
    if (v==0) sprintf(exitfile, "saida/commands%d_output.txt", i);
    else sprintf(exitfile, "saida/expected%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");
    for (int indice = 0; indice < TR; indice++) {
        if (strcmp(hashTableR[indice].id, "")) {
            if (date > (hashTableR[indice].date)) {
                fprintf(ef, "%s;%s\n", hashTableR[indice].id,hashTableR[indice].description); 
                totalLinhas++; 
            }
        }
    }
    fclose(ef);
    if(i==0) remove("saida/commands0_output.txt");
    free (exitfile);
    return totalLinhas;
}

//Devolve a mensagem de um repos
static MESS **getmessage ( char *str ){
    REPOS *r;
    r = findRepos (str);
    return &(r->message);
}

//Insere na struct o tamanho da mensagem, o repos onde ela se encontra e quem a escreveu
void insere_message (char * author_id, int ri, char * repos_id) {
    int i;
    MESS **m = (getmessage (repos_id));
    for (i = 0; (*m) && (((*m)->r) >= ri); i++)m = &((*m)->prox);
    
    MESS *m2 = *m;
    (*m) = malloc(sizeof (struct mensagens));
    (*m)->r = ri;
    (*m)->id = author_id;
    (*m)->login = getLoginUser(author_id);
    (*m)->rid = repos_id;
    (*m)->prox = m2; 
      
}

//Imprime o top N
static void imprime(MESS *m, int N, int i, FILE *ef){
    for (int i = 0; i<N ; i++) {
        if (m) {
            fprintf (ef, "%s;%s;%d;%s\n", m->id, m->login, m->r, m->rid);
            m = m->prox;
        }
        else fprintf (ef, "\n");
    }
    //free(m);
}

//Devolve os utilizadores com maiores mensagens de commit por cada repositório
int getTopMensagens (int N, int i, int v) {
    int indice, r=0;
    char *exitfile;
    exitfile = malloc(100*sizeof(char));
    if (v==0) sprintf(exitfile, "saida/commands%d_output.txt", i);
    else sprintf(exitfile, "saida/expected%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");                          
    for (indice = 0; indice < TR; indice++) {
        if (strcmp(hashTableR[indice].id, "")){
            MESS *m = hashTableR[indice].message;
            imprime(m,N,i,ef);
            r++;
        }
    }
   fclose (ef);
    if(i==0) remove("saida/commands0_output.txt");
   free(exitfile);   
   return r*N; 
}

//Inicializar o repositório
static REPOS init_repos(char *buffer, int *v) {
    REPOS r;
    char *aux = NULL;     
    int i = 1;                       
    *v=1;

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1: 
                if (validPar(aux,0))r.id = aux;
                else *v=0;
                break;
            case 2: 
                if (validPar(aux,0)) r.owner_id = aux; 
                else *v=0;
                break;
            case 3: 
                if (validString(aux)) r.full_name = aux;   
                else *v=0;
                break;
            case 4: 
                if (validString(aux)) r.license = aux; 
                else *v=0;
                break;
            case 5: 
                if (validBool(aux)) r.has_wiki = aux;
                else *v=0;
                break;
            case 6: 
                if (validString(aux)) r.description = aux;
                else *v=0;
                break;
            case 7: 
                if (validString(aux)) r.language = aux; 
                else *v=0;
                break;
            case 8:
                if (validString(aux)) r.default_branch = aux; 
                else *v=0;
                break;
            case 9: 
                if (validDate(aux)) r.created_at = aux;
                else *v=0;  
                break;
            case 10: 
                if (validDate(aux)) r.updated_at = aux;
                else *v=0;  
                break;
            case 11: 
                if (validPar(aux,0)) r.forks_count = aux;
                else *v=0;  
                break;
            case 12: 
                if (validPar(aux,0)) r.open_issues = aux;
                else *v=0;  
                break;    
            case 13: 
                if (validPar(aux,0)) r.stargazers_count = aux;
                else *v=0;  
                break; 
            case 14: 
                if (validPar(aux,1)) r.size = aux;
                else *v=0;  
                break; 
            default:
                printf ("Error acessing aux!\n");
        } 
    }
    r.message = NULL;
    r.date=0;
    free(aux);
    return r;
}

//Função principal do módulo repos
int mainRepos () {
    char buffer[200000];
    REPOS repos;
    int total_repos = 0;
    int v;
    
    init_table();
    FILE *data_file = fopen ("entrada/repos-g3.csv", "r");   
    if (data_file == NULL) printf ("Error opening file.\n"); 
    if (fgets(buffer, 200000, data_file) == NULL) return -1; 
    while(fgets (buffer, 200000, data_file)) {
        repos=init_repos(buffer, &v);
        char *owner_id = getIdUser(repos.owner_id);
        if (v && strcmp((owner_id),"")) {
            total_repos++;
            insert_repos(repos);   
            free(owner_id);
       }        
    }
    fclose(data_file);
    return total_repos;
}