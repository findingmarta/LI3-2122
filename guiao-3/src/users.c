#include "users.h"
#include "repos.h"
#include "valid.h"
#include "queries.h"
#include "commits.h"

struct Arvore_busca {
   int info;
   struct Arvore_busca *esq, *dir; 
};

struct user {
    int commit_lang;
    int valid;
    int totalC;
    char *public_repos;
    char *id;
    char *followers;
    char *login;
    char *type;
    char *created_at;
    char *following;
    char *follower_list;
    char *following_list;
    char *public_gists;
    Arvore *arv;
}; 

struct u {
    char *id;
    char *login;
    int r;
    struct u* prox;
};

static USER hashTableU[TU];

//Inicializa a tabela
static void init_table () {                                      
    int i;
    for(i=0; i<TU; i++)
        hashTableU[i].id = "";
}

//Hash Function para gerar um índice 
static int hashFunction (int chave) {
    return chave % TU;
}

//Hash Function para gerar um índice a partir de uma string
static unsigned long hashFunctionString(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 
    return hash % TU; 
}

//Inserir um user na tabela
static void insert_user(USER user) {
    unsigned long indice = hashFunctionString(user.id);
    while(strcmp(hashTableU[indice].id, "")){
        indice = hashFunction(indice+1);
    }
    hashTableU[indice] = user;
}

//Pesquisa um user na tabela
static USER *findUser (char* str) {
    int r=0;
    unsigned long indice = hashFunctionString(str);
    while((strcmp(hashTableU[indice].id, "")!=0) && r==0){
        if (strcmp(hashTableU[indice].id, str) == 0) r=1;
        else indice = hashFunction(indice+1);
    }
    return &hashTableU[indice];
}

//Retorna o id do usuário
char *getIdUser (char *str) {   
    USER *u;
    u = findUser (str);
    if(strcmp(u->id,"")==0) return "";
    return strdup(u->id);
}

//Retorna o login do usuário
char *getLoginUser (char *str) {   
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível aceder ao login.\n");
        return "";}
    return strdup(u->login);
}

//Retorna a lista de seguidores do usuário
char *getFollowerList (char *str) {   
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível aceder à follower_list.\n");
        return "";}
    return strdup(u->follower_list);
}

//Retorna a lista de users que determinado usuário segue
char *getFollowingList (char *str) {   
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível aceder à following_list.\n");
        return "";}
    return strdup(u->following_list);
}

//Retorna o total de commits feito por um usuário
void changeTotalC (char *str) {   
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível aceder ao total de commits do user.\n");
        return;}
    (u->totalC)++;
}

//Retorna o valor do valid
int getValid (char *str) {   
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível aceder ao valid.\n");
        return -1;}
    return u->valid;
}

//Modifica o valor do valid
void changeValid (char *str) {
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível modificar o valid.\n");
        return;}
    u->valid = 1;
}

//Modifica o valor do campo commit_lang
void changeCommitLang (char *str) {
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("RepoID não encontrado, impossível modificar o campo commit_lang.\n");
        return;}
    (u->commit_lang)++;
}

//Verifica se um user é do tipo Bot
int isBot (char *str) {
    USER *u;
    u = findUser (str);
    if(!strcmp(u->id,"")){
        printf("ID não encontrado, impossível verificar o tipo Bot.\n");
        return 1;}
    if (!strcmp ("Bot", u->type)) return 0;
    return 1;
}

//Insere numa lista ligada do user, um id, um login e o número de ocorrências 
void insereLL (topN **u, int ri, char *id, char *login, int N){
    int i;
    for (i = 0; i<N && (*u) && (((*u)->r) >= ri); i++) u = &((*u)->prox);
    if (i<N ){
        topN *u2 = *u;
        (*u) = malloc(sizeof(struct u));
        (*u)->r = ri;
        (*u)->id = id;
        (*u)->login = login;
        (*u)->prox = u2; 
    }
}

/*
//Função responsável por libertar o espaço alocado por uma lista ligada
void freeList (topN *u){
    topN *f;
    while(u!=NULL){
        f = u;
        u = u->prox;
        free(f);
    }
}
*/

//Imprime um top N
static void imprimeNusers(topN *u, int N, int i, int c, int v){
    char *exitfile;
    exitfile = malloc(100*sizeof(char));
    if (v==0){
    sprintf(exitfile, "saida/commands%d_output.txt", i);
    }
    else sprintf(exitfile, "saida/expected%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");
    for (int i = 0; i < N; i++) {
        if (u != NULL && c == 1){
            fprintf (ef, "%s;%s;%d\n", u->id, u->login, u->r);
            u = u->prox;
        }
        else if (u != NULL && c == 0){
            fprintf (ef, "%s;%s\n", u->id, u->login);
            u = u->prox;
        }
    }
    fclose(ef);
    free(exitfile);
}


//Insere numa lista ligada o user com o respetivo id, login e total de commits feitos no repos de um amigo
void insereFriend (int N, int i, int v){
    topN *u = NULL;
    for (int i=0; i<TU; i++){
        if(strcmp(hashTableU[i].id, "")){
            insereLL(&u, hashTableU[i].totalC, hashTableU[i].id, hashTableU[i].login, N);
        }
    }
    //topN *init = u;
    if (i!=0) imprimeNusers(u,N,i,0, v);
    // freeList(init);
}



//Função que vai inserir a data na arvore
static Arvore *inserir_no(Arvore *arv, int date) {   
    if(arv == NULL){ 
        arv = (Arvore *) malloc(sizeof(Arvore)); 
        arv->info = date;
        arv->esq = NULL; 
        arv->dir = NULL; 
    }
    else if(date <= arv->info) arv->esq = inserir_no(arv->esq,date); 
    else arv->dir = inserir_no(arv->dir,date);
    return arv;                                                                 
}

//Função que adiciona uma data à respetiva arvore do user
void dateWithUser(char *str, int date){
    USER *u;
    u = findUser (str);
    if(strcmp(u->id,"")==0){
        printf("ID não encontrado, impossível inserir datas na árvore.\n");
        return;}
    u->arv = inserir_no(u->arv,date);
}

//Função responsável por contar o total de commits numa arvore
static int contaCommits (Arvore *arv , int inicio, int fim){
    int r = 0;
    if(arv == NULL) return 0;
    else if(arv -> info == inicio){ 
        r++;
        r += contaCommits (arv-> dir, inicio, fim);
    }
    else if((inicio < arv->info) && (arv->info <= fim)){ 
        r++;
        r += contaCommits (arv->esq, inicio, fim);
        r += contaCommits (arv->dir, inicio, fim);
    }
    else if ((inicio < arv->info) && (arv->info > fim)) r += contaCommits (arv->esq, inicio, fim); 
    else if (inicio > (arv->info)) r += contaCommits (arv->dir, inicio, fim);
    return r;
}

//Imprime os N utilizadores mais ativos num intervalo
void getTop (int N, int inicio, int fim, int i, int v) {
    topN *u = NULL;
    int indice;
    int r;
    for (indice = 0; indice < TU; indice++) {
        if (strcmp(hashTableU[indice].id, "")){
            r = contaCommits (hashTableU[indice].arv, inicio, fim);
            insereLL (&u, r, hashTableU[indice].id, hashTableU[indice].login, N);
        }
    }
    //topN *init = u;
    if(i!=0)imprimeNusers(u,N,i,1,v);
   // freeList(u);
}

//Verifica o top N de users com mais commits feitos numa determinada linguagem
void getTopCommits (int N, char *language, int i, int v) {
    topN *u = NULL;
    int indice,r;
    char *id=NULL,*login=NULL;
    for (indice = 0; indice < TU; indice++) {
        if (strcmp(hashTableU[indice].id, "")){
            r = hashTableU[indice].commit_lang;
            id = hashTableU[indice].id;
            login = hashTableU[indice].login;
            insereLL (&u,r,id,login,N);
            hashTableU[indice].commit_lang = 0;
        }    
    }
    //topN *init = u;
    if(i!=0) imprimeNusers(u,N,i,1, v);
    //freeList(u);
}        

//Inicializar o usuário
static USER init_user(char *buffer, int *v) {
    USER u;
    char *aux = NULL;     
    int i = 1;
    int r1,r2; 
    *v=1;                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));
        switch (i++) {
            case 1: 
                if (validPar(aux,0)) u.id = aux; 
                else *v=0;
                break;
            case 2: 
                if (validString(aux)) u.login = aux; 
                else *v=0;
                break;
            case 3: 
                if (validType(aux)) u.type = aux; 
                else *v=0;
                break;
            case 4: 
                if (validDate(aux)) u.created_at = aux; 
                else *v=0;
                break;
            case 5: 
                if (validPar(aux,0)) {
                    u.followers = aux;
                    sscanf(u.followers, "%d", &r1);}
                else *v=0;
                break;
            case 6: 
                if (validList(aux,r1)) u.follower_list = aux; 
                else *v=0;
                break;
            case 7: 
                if (validPar(aux,0)) {
                    u.following = aux;
                    sscanf(u.following, "%d", &r2);}
                else *v=0;
                break;
            case 8:
                if (validList(aux,r2)) u.following_list = aux; 
                else *v=0;          
                break;
            case 9:
                if (validPar(aux,0)) u.public_gists = aux; 
                else *v=0;
                break; 
            case 10:
                if (validPar(aux,1)) u.public_repos = aux;
                else *v=0;
                break;
            default:
                printf ("Error acessing aux!\n");
        } 
    }   
    u.valid = 0;
    u.totalC = 0;
    u.arv = NULL; 
    u.commit_lang = 0;
    free(aux);
    return u;
}

//Função principal do módulo users
int *mainUser (int arr[]) {
    char buffer[200000];
    clock_t start, end;
    int total_u = 0;
    int total_b = 0;
    int total_o = 0;
    USER user;
    int v;

    init_table();
    FILE *data_file = fopen ("entrada/users-g3.csv", "r");   
    if (data_file == NULL) printf ("Error opening file.\n");           
    if (fgets(buffer, 200000, data_file) == NULL) return NULL;   
    while(fgets (buffer, 200000, data_file)) {
        user = init_user(buffer, &v);
        if (v) {
            if ((strcmp("User", user.type)) == 0) total_u++;
            else if ((strcmp("Bot", user.type)) == 0) total_b++;
            else if ((strcmp("Organization", user.type)) == 0) total_o++;
            insert_user(user);  
        }
    }
    start = clock();
    arr[0] = total_u; 
    arr[1] = total_b; 
    arr[2] = total_o;
    end = clock();
    arr[3] = (double)(end-start);
    fclose(data_file);
    return arr;
}