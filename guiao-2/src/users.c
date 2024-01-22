#include "users.h"
#include "repos.h"
#include "param.h"
#include "commits.h"

#define T 854771

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


USER hashTableU[T];

//Inicializa a tabela
void init_table () {
    int i;
    for(i=0; i<T; i++)
        hashTableU[i].id = "";
}

//Hash Function para gerar um índice 
int hashFunction (int chave) {
    return chave % T;
}

//Hash Function para gerar um índice a partir de uma string
unsigned long hashFunctionString(char* str) { 
    unsigned long hash = 5381; 
    int c; 
    while((c = *(str++))) 
        hash = ((hash << 5) + hash) + c; 

    return hash % T; 
}

//Inserir um user na tabela
void insert_user(USER user) {
    unsigned long indice = hashFunctionString(user.id);
    while(hashTableU[indice].id != ""){
        indice = hashFunction(indice+1);
    }
    hashTableU[indice] = user;
}

//Remover um user da tabela
void remove_user(USER user) {
    unsigned long indice = hashFunctionString(user.id);
    while(hashTableU[indice].id != ""){
        if (strcmp(hashTableU[indice].id, user.id) == 0) hashTableU[indice].id = "";
        else indice = hashFunction(indice+1);
    }
}

//Pesquisa um user na tabela
USER findUser (char* str) {
    unsigned long indice = hashFunctionString(str);
    while(hashTableU[indice].id != ""){
        if (strcmp(hashTableU[indice].id, str) == 0) return hashTableU[indice];
        else indice = hashFunction(indice+1);
    }      
}

//Retorna o id do usuário
char *getIdUser (char *str) {   
    USER u;
    u = findUser (str);
    return u.id;
}

//Retorna o index do user
int getIndex (char *str) {   
    unsigned long indice = hashFunctionString(str);
    while(hashTableU[indice].id != ""){
        if (strcmp(hashTableU[indice].id, str) == 0) return indice;
        else indice = hashFunction(indice+1);
    }      
    
    return indice;
}

//Retorna o login do usuário
char *getLoginUser (char *str) {   
    USER u;
    u = findUser (str);
    return u.login;
}

//Retorna a lista de seguidores do usuário
char *getFollowerList (char *str) {   
    USER u;
    u = findUser (str);
    return u.follower_list;
}

//Retorna a lista de users que determinado usuário segue
char *getFollowingList (char *str) {   
    USER u;
    u = findUser (str);
    return u.following_list;
}

//Retorna a árvore correspondente ao usuário
Arvore *getArvUser (char *str) {   
    USER u;
    u = findUser (str);
    return u.arv;
}

//Retorna o valor do valid
int getValid (char *str) {   
    USER u;
    u = findUser (str);
    return u.valid;
}

//Modifica o valor do valid
void changeValid (char *str) {
    USER u;
    u = findUser (str);
    u.valid = 1;
    remove_user(u);
    insert_user (u);
}

//Verifica se um user é do tipo Bot
int isBot (char *str) {
    USER u;
    u = findUser (str);
    if (strcmp ("Bot", u.type) == 0) return 0;
    return 1;
}


//Imprime os N utilizadores mais ativos
void getTop (int N, topN *u1, int inicio, int fim, int i) {
    int indice;
    int r;
    for (indice = 0; indice < T; indice++) {
        if (hashTableU[indice].id != ""){
            r = contaCommits (hashTableU[indice].arv, inicio, fim);
            insereLL (&u1, r, hashTableU[indice].id, hashTableU[indice].login, N);
        }
    }
    
    imprimeNusers(u1,N,i);
}

//Imprime a tabela num ficheiro de saída
void print_table(){
    int i;
    printf("\n---------------------- A CRIAR TABELA HASH USERS ----------------------\n");
    FILE *out_file = fopen ("saida/users-ok.csv", "w");  

    if (out_file == NULL) printf ("Error opening file->\n");                             

    fprintf (out_file, "id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos\n");

    for(i=0; i<T; i++) {
        if(hashTableU[i].id != "")
            fprintf (out_file, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", hashTableU[i].id, hashTableU[i].login, hashTableU[i].type,hashTableU[i].created_at, hashTableU[i].followers, hashTableU[i].follower_list, hashTableU[i].following, hashTableU[i].following_list, hashTableU[i].public_gists, hashTableU[i].public_repos); 
        else
            fprintf(out_file, "\n");

    }
    fclose(out_file);

    printf("\n-------------------------- CONCLUIDO ----------------------------------\n");
}


//Inicializar o usuário
USER init_user(char *buffer) {
    USER u;
    char *aux = NULL;     
    int i = 1;                      

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));
        switch (i++) {
            case 1: 
                u.id = aux;
                break;
            case 2: 
                u.login = aux;
                break;
            case 3: 
                u.type = aux; 
                break;
            case 4: 
                u.created_at = aux; 
                break;
            case 5: 
                u.followers = aux;
                break;
            case 6: 
                u.follower_list = aux; 
                break;
            case 7: 
                u.following = aux;
                break;
            case 8:
                u.following_list = aux;        
                break;
            case 9:
                u.public_gists = aux; 
                break; 
            case 10:
                u.public_repos = aux;
                break;
            default:
                printf ("Error acessing aux!\n");
        } 
        u.valid = 0;
        u.arv = NULL; 
    } 
    
    return u;
}

//Função principal do módulo users
int *mainUser (int *arr) {
    char buffer[200000];
    USER user;
    int total_u = 0;
    int total_b = 0;
    int total_o = 0;

    init_table();

    FILE *data_file = fopen ("entrada/users.csv", "r");   

    if (data_file == NULL) printf ("Error opening file.\n");           

    fgets(buffer, 200000, data_file);  

    while(fgets (buffer, 200000, data_file)) {
        user = init_user(buffer);

        if ((strcmp("User", user.type)) == 0) total_u++;
        else if ((strcmp("Bot", user.type)) == 0) total_b++;
        else if ((strcmp("Organization", user.type)) == 0) total_o++;

        insert_user(user);  

    }

    arr[0] = total_u; 
    arr[1] = total_b; 
    arr[2] = total_o;

    fclose(data_file);

    print_table();

    return arr;

}