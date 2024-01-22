#include "users.h"
#include "repos.h"
#include "valid.h"
#include "queries.h"
#include "commits.h"

struct commit {
    char *repo_id;
    char *author_id;
    char *committer_id;
    char *commit_at;
    char *message;
};

static COMMIT hashTableC[TC];

//Inicializa a tabela
static void init_table () {
    int i;
    for(i=0; i<TC; i++)
        hashTableC[i].repo_id = "";
}

//Hash Function para gerar um índice 
static int hashFunction (int chave) {
    return chave % TC;
}

//Hash Function para gerar um índice a partir de uma string
static unsigned long hashFunctionString(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 
    return hash % TC; 
}

//Insere um commit na tabela
static void insert_commit(COMMIT commit) {
    unsigned long indice = hashFunctionString(commit.repo_id);
    while(strcmp(hashTableC[indice].repo_id,"")){
        indice = hashFunction(indice+1);
    }
    hashTableC[indice] = commit; 
}

//Pesquisa um commit na tabela
static COMMIT *findCommit (char* str) {
    int r=0;
    unsigned long indice = hashFunctionString(str);
    while((strcmp(hashTableC[indice].repo_id,"")!=0) && r==0) {
        if (strcmp(hashTableC[indice].repo_id, str) == 0) r=1;
        else indice = hashFunction(indice+1);
    }
    return &hashTableC[indice];
}

//Retorna o id do repositório do commit
char *getReposId (char *str) {   
    COMMIT *c;
    c = findCommit (str);
    if(!strcmp(c->repo_id,"")) return "";
    return strdup(c->repo_id);
}

//Função que verifica se um Repos Owner é amigo do user
static int isFriend(char *list, char *ownerRepos){
    int r=1;
    removeChar(list, '[');
    removeChar(list, ']');
    char *aux = (list);
    char *auxList = NULL;
    while (r && aux) {
        auxList = (strsep (&aux, ", "));
        if (strcmp (auxList, ownerRepos) == 0) r=0;
    }
    return r;
}


//Função que irá atribuir a um user o número de commits feitos no repos de um amigo
void storeFriends (int N, int i, int v) {
    char *followerList=NULL, *followingList=NULL, *ownerID=NULL;
    for (int indice = 0; indice < TC; indice++) {
        if (strcmp(hashTableC[indice].repo_id,"")){
            followingList = getFollowingList(hashTableC[indice].committer_id);
            followerList = getFollowerList(hashTableC[indice].committer_id);
            ownerID = getOwnerID(hashTableC[indice].repo_id);
            if (!isFriend(followerList, ownerID) && !isFriend(followingList, ownerID)) changeTotalC(hashTableC[indice].committer_id);
            free(followingList);
            free(followerList);
            free(ownerID);
        }
    }
   insereFriend(N,i,v);
}



//Devolve o top N de utilizadores com commits de uma determinada linguagem
void insereCommits (char *language) {
    int indice ;
    char *id=NULL, *lang=NULL;
    for (indice = 0; indice < TC; indice++) {
        if (strcmp(hashTableC[indice].repo_id, "")!=0) {
            lang= getLanguage(hashTableC[indice].repo_id);
            if (strcasecmp(lang,language)==0){
                id = getIdUser(hashTableC[indice].committer_id);
                changeCommitLang (id);
                free(id);
            }
            free(lang);
        }
    }
}

//Inicializar o commit
static COMMIT init_commit(char *buffer, int *v) {
    COMMIT c;
    char *aux = NULL;     
    int i=1;
    *v=1;
                       
    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));
        switch (i++) {
            case 1:     
                if (validPar(aux,0)) c.repo_id = aux;
                else *v=0;  
                break;
            case 2: 
                if (validPar(aux,0)) c.author_id = aux;
                else *v=0;  
                break;
            case 3: 
                if (validPar(aux,0)) c.committer_id = aux;
                else *v=0;
                break;
            case 4: 
                if (validDate(aux)) c.commit_at = aux; 
                else *v=0;
                break;
            case 5: 
                if (validString(aux)) c.message = aux;
                else *v=0;
                break;
            default:
                printf ("Error acessing aux!\n");
        }     
    }
    return c;
}

//Função principal do módulo commits
int *mainCommit (int arr[]) {
    int total_collab_bots = 0;
    int total_commits = 0;
    int total_collab = 0;
    char buffer[200000];
    char *author_id=NULL, *committer_id=NULL, *repo_id=NULL, *commit_at=NULL;
    clock_t start, end;
    COMMIT commit;
    int v;

    init_table();
    FILE *data_file = fopen ("entrada/commits-g3.csv", "r");   
    if (data_file == NULL) printf ("Error opening file.\n");
    if (fgets(buffer, 200000, data_file) == NULL) return NULL;                          
    while(fgets (buffer, 200000, data_file)) {
        commit=init_commit(buffer, &v);
        committer_id= getIdUser(commit.committer_id);
        author_id= getIdUser(commit.author_id);
        repo_id= getId(commit.repo_id);

        if (v && strcmp(committer_id,"") && strcmp(author_id,"") && strcmp(repo_id,"")) {
            total_commits++;
            if ((getValid(commit.committer_id) == 0)) {
                total_collab++;
                if (isBot(commit.committer_id) == 0) total_collab_bots++;  
                changeValid(commit.committer_id); 
            }
            if ((getValid(commit.author_id) == 0)) {
                total_collab++;
                if (isBot(commit.author_id) == 0) total_collab_bots++; 
                changeValid(commit.author_id); 
            }
            insert_commit(commit);
            changeRepos (string2int(commit.commit_at,1), commit.repo_id); 
            commit_at = strdup(commit.commit_at);
            dateWithUser(commit.committer_id, string2int(commit_at,1));
            insere_message (commit.author_id, strlen(commit.message), commit.repo_id); 
            free(committer_id);
            free(author_id);
            free(repo_id);
            free(commit_at);
        }  
    }
    start = clock();
    arr[0] = total_commits; 
    arr[1] = total_collab; 
    arr[2] = total_collab_bots;
    end = clock();
    arr[3] = (end-start);
    fclose(data_file);
    removeReposLines();
    return arr;
}