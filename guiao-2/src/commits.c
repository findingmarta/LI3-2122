#include "users.h"
#include "repos.h"
#include "param.h"
#include "commits.h"

#define T 1709443
#define TU 854771

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


COMMIT hashTableC[T];

//Inicializa a tabela
void init_table2 () {
    int i;
    for(i=0; i<T; i++)
        hashTableC[i].repo_id = "";
}

//Hash Function para gerar um índice 
int hashFunction2 (int chave) {
    return chave % T;
}

//Hash Function para gerar um índice a partir de uma string
unsigned long hashFunctionString2(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 

    return hash % T; 
}

//Insere um commit na tabela
void insert_commit(COMMIT commit) {
    unsigned long indice = hashFunctionString2(commit.repo_id);
    while(hashTableC[indice].repo_id != ""){
        indice = hashFunction2(indice+1);  //será do tipo unsigned long
    }
    hashTableC[indice] = commit; 
}

//Remove um commit da tabela
void remove_commit(COMMIT commit) {
    unsigned long indice = hashFunctionString2(commit.repo_id);
    while(hashTableC[indice].repo_id != ""){
        if (strcmp(hashTableC[indice].repo_id, commit.repo_id) == 0) hashTableC[indice].repo_id = "";
        else indice = hashFunction2(indice+1);
    }
}

//Pesquisa um commit na tabela
COMMIT findCommit (char* str) {
    unsigned long indice = hashFunctionString2(str);
    while(hashTableC[indice].repo_id != ""){
        if (strcmp(hashTableC[indice].repo_id, str) == 0) return hashTableC[indice];
        else indice = hashFunction2(indice+1);
    }      
}

//Retorna o valor do visited
int getVisited (char *str) {   
    COMMIT c;
    c = findCommit (str);
    return c.visited;
}

//Modifica o valor do visited
void changeVisited (char *str) {
    COMMIT c;
    c = findCommit (str);
    c.visited = 1;
    remove_commit(c);
    insert_commit (c);
}


//Verifica se um dado repositório realizou algum commit depois de uma dada data
int getDateCommit(char *id, int date) {
    int r = 0;
    COMMIT c;

    c = findCommit(id);
    if (string2int(c.commit_at) >= date) {
        r=1;
        changeVisited (id);
    }

    return r;
}

//Função que calcula a quantidade de commits feitos por um user no repositório de um amigo
void storeFriends (int N, int i, topN *u) {
    char *id, *login, *followerList, *followingList, *ownerID;
    int index;
    int arrUsers[TU];

    for (int k = 0; k < TU; k++) arrUsers[k] = 0;

    for (int indice = 0; indice < T; indice++) {
        if (hashTableC[indice].repo_id != ""){
            id = getIdUser(hashTableC[indice].committer_id);
            login = getLoginUser(id);
            followingList = getFollowingList(id);
            followerList = getFollowerList(id);
            index = getIndex(id);
            ownerID = getOwnerID(hashTableC[indice].repo_id);
            
            if ((isFriend(followerList, ownerID) == 0) && (isFriend(followingList, ownerID) == 0)) {
                arrUsers[index]++; 
            }
        }
    }

    for (int j = 0; j < TU; j++) {
        insereLL (&u, arrUsers[j], id, login, N);
    }

    imprimeUsers(u, N, i);
}

//Imprime a tabela num ficheiro de saída
void print_table2(){
    int i;
    printf("\n--------------------- A CRIAR TABELA HASH COMMITS ---------------------\n");
    FILE *out_file = fopen ("saida/commits-ok.csv", "w");  

    if (out_file == NULL) printf ("Error opening file.\n");                             

    fprintf (out_file, "repo_id;author_id;committer_id;commit_at;message\n");
    for(i=0; i<T; i++) {
        if(hashTableC[i].repo_id != "")
            fprintf (out_file, "%s;%s;%s;%s;%s", hashTableC[i].repo_id, hashTableC[i].author_id, hashTableC[i].committer_id,hashTableC[i].commit_at, hashTableC[i].message); 
        else
            fprintf(out_file, "\n");

    }
    fclose(out_file);

    printf("\n-------------------------- CONCLUIDO --------------------------------\n");
}

//Inicializar o commit
COMMIT init_commit(char *buffer) {
    COMMIT u;
    char *aux = NULL;     
    int i = 1;
                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1:     
                u.repo_id = aux;
                break;
            case 2: 
                u.author_id = aux;
                break;
            case 3: 
                u.committer_id = aux;
                break;
            case 4: 
                u.commit_at = aux; 
                break;
            case 5: 
                u.message = aux; 
                break;
            default:
                printf ("Error acessing aux!\n");
            }     
    }
    u.visited = 0;
    return u;
}

//Função principal do módulo commits
int *mainCommit (int *arr) {
    char buffer[200000];
    COMMIT commit;
    int total_commits = 0;
    int total_collab = 0;
    int total_collab_bots = 0;

    init_table2();

    FILE *data_file = fopen ("entrada/commits.csv", "r");   

    if (data_file == NULL) printf ("Error opening file.\n");

    fgets(buffer, 200000, data_file);                         
    while(fgets (buffer, 200000, data_file)) {

        total_commits++;
        commit=init_commit(buffer);

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
    } 

    arr[0] = total_commits; 
    arr[1] = total_collab; 
    arr[2] = total_collab_bots;

    fclose(data_file);
    print_table2();

    return arr;
}