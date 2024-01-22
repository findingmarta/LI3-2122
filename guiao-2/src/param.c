#include "users.h"
#include "repos.h"
#include "param.h"
#include "commits.h"

struct Arvore_busca {
   int info;
   struct Arvore_busca *esq, *dir; 
};

struct mensagens {
    char *id;
    char *login;
    int r;
    struct mensagens* prox;
};

struct u {
    char *id;
    char *login;
    int r;
    struct u* prox;
};


struct l {
    char *language;
    int r;
    struct l* prox;
};

struct user {
    int valid;
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

struct commit {
    int visited;
    char *repo_id;
    char *author_id;
    char *committer_id;
    char *commit_at;
    char *message;
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
int string2int (char *date){
    removeChar (date, '-');
    int num = atoi(date);
    return num;
}

//Função que adiciona uma data à respetiva arvore do user
void dateWithUser(char *str, int date){
    USER u;
    u = findUser (str);
    u.arv = inserir_no(u.arv,date);
    remove_user(u);
    insert_user(u);
} 

//Função que vai inserir a data na arvore
Arvore *inserir_no(Arvore *arv, int date) {   

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

//Função responsável por contar o total de commits numa arvore
int contaCommits (Arvore *arv , int inicio, int fim){
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

//Conta o número de nodos da árvore
int contaNos(Arvore *arv){
   if(arv == NULL)
        return 0;
   else
        return 1 + contaNos(arv->esq) + contaNos(arv->dir);
}

//--------------------------------------5/6------------------------------------

//Insere numa lista ligada do user, um id, um login e o número de ocorrências 
void insereLL (topN **u, int ri, char *id, char *login, int N){
    int i;
    for (i = 0; i<N && (*u) && (((*u)->r) >= ri); i++){
        u = &((*u)->prox);
    }
    if (i<N){

        topN *u2 = *u;
        (*u) = malloc(sizeof (struct u));
        (*u)->r = ri;
        (*u)->id = id;
        (*u)->login = login;
        (*u)->prox = u2; 
    }

}


//Imprime o top N
void imprimeNusers (topN *u, int N, int i){
    char *exitfile;

    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/commands%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");

    for (int i = 0; i < N; i++) {
        if (u != NULL){
            fprintf (ef, "id:%s; login:%s; commits:%d\n", u->id, u->login, u->r);
            u = u->prox;
        }
    }
}


//----------------------------------8-------------------------------------------

//Função responsável por recolher as linguagens e as suas respetivas ocorrências
void insereLanguage (struct l** u, char *language){
    int i;
    int r;
    for (i=0; (*u); i++){
        if (strcmp(((*u)->language), language) == 0){
        (*u)->r++;
        return;
        }
        else u = &((*u)->prox);
    }
        (*u)=malloc(sizeof (struct l));
        (*u)->r = 1;
        (*u)->language = language;
        (*u)->prox = NULL; 
}


//Irá ordenar as linguagens em ordem decrescente de utilização
void ordenaLanguage (struct l** linguas2, struct l* linguas, int N){
 int i;
 for (i = 0; i<N && (*linguas2) && (((*linguas2)->r) >= (linguas->r)); i++){
        linguas2 = &((*linguas2)->prox);
    }
    if (i<N){
        struct l* u = *linguas2;
        (*linguas2) = malloc (sizeof(struct l));
        (*linguas2)->r = (linguas)->r;
        (*linguas2)->language = (linguas)->language;
        (*linguas2)->prox = u;
    }
}


//Função que irá imprimir uma struct de linguagens
void imprimeL (struct l *u, int N, int i){
    char *exitfile;

    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/commands%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");

    for (int i = 0; i<N && (u); i++){
        fprintf (ef, "%s:%d\n", u->language,u->r);
        u = u->prox;
    }
}

//--------------------------------------9-----------------------------------------

//Função que verifica se um Repos Owner é amigo do user
int isFriend(char *list, char *ownerRepos){
    
    removeChar(list, '[');
    removeChar(list, ']');

    char *aux = strdup(list);
    char *auxList = NULL;

    while (aux) {
        auxList = strdup(strsep (&aux, ","));
        if (strcmp (auxList, ownerRepos) == 0) return 0;
    }

    return 1;
}

//Imprime um top N de users
void imprimeUsers (topN *u, int N, int i){
    char *exitfile;

    exitfile = malloc(100*sizeof(char));
    sprintf(exitfile, "saida/commands%d_output.txt", i);
    FILE *ef = fopen (exitfile, "w");

    for (int i = 0; i < N; i++) {
        if (u != NULL){
            fprintf (ef, "id:%s; login:%s\n", u->id, u->login);
            u = u->prox;
        }
    }
}

//---------------------------------------10-------------------------------------------


/*
//Insere na lista ligada a mensagem com um novo valor de tamanho da mensagem, id e login
void addmessage (char* mensagem, char* reposid, char* commiterid){
    REPOS *r = findRepos (reposid);
    int size = strlen(mensagem);
    struct mensagens** m = &(r->message);
    char *login =getLoginUser(commiterid);
    
    while ((*m) && (*m)->r > size) m = &((*m)->prox);
    
    struct mensagens* new = malloc (sizeof (struct mensagens)),*prox;
    new->r = size;
    new->id = commiterid;
    new->login = login;
    prox = *m;
    *m = new;
    new->prox = prox;
}*/

//----------------------------------------------------------------------------------

//Função que vai ler todos os commits e começar a construir a arvore correspondente
int parseCommit (){
    char buffer[200000];
    COMMIT commit;

    FILE *data_file = fopen ("entrada/commits.csv", "r");   

    if (data_file == NULL) printf ("Error opening file.\n");

    fgets(buffer, 200000, data_file);                         
    while(fgets (buffer, 200000, data_file)) {
        commit = init_commit(buffer);

        dateWithUser(commit.committer_id, string2int(commit.commit_at));
        
        //addmessage(commit.message,commit.repo_id, commit.committer_id);
    } 

    fclose(data_file);

    return 0;
}
