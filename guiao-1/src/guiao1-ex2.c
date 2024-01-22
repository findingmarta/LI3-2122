#include "guiao1-ex2.h"

//Esta função irá inicializar o repositório.

REPOS init_repos2(char *buffer) {
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

//Esta função irá inicializar o commit.

COMMIT init_commits2 (char *buffer) {
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
    return u;
}

//Função que irá remover commits com users inexistentes. 

int comp_id (USER *u, int size_u, char *v){
    int r=1,j;

    for (j=0; j < size_u && strcmp (u[j].id, v); j++);
    if (j==size_u) r=0;
 
    return r;
}

//Função que irá remover commits com repos inexistentes. 

int comp_rep (REPOS *r, int size_r, char *v) {
    int result=1,j;

    for (j=0; j < size_r && strcmp (r[j].id, v); j++);
    if (j==size_r) result=0;
    
    return result;
}

//

int getSize (char buffer[], FILE *data_file) {
    int i=0;
    
    fgets(buffer, 2000000, data_file);                         
    while(fgets (buffer, 2000000, data_file)) i++;

    return i;
}

//Esta função irá escrever os usuários noutro ficheiro de saída e devolver o tamanho da lista de todos os users.

int doUser2 (USER *user) {
    char *buffer = malloc (200000000);
    //char buffer[200000];

    FILE *data_file = fopen ("saida/users-ok.csv", "r");

    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");                             
    }

    int i = getSize (buffer, data_file);

    FILE *out_file = fopen ("saida/users-final.csv", "w");  

    if (out_file == NULL) {                                                                             
        printf ("Error opening file.\n");                             
    }

    fprintf (out_file, "id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos\n");

    //Imprime 
    for (int j = 0; j < i; j++){
        USER u = user [j];

        fprintf (out_file, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", u.id, u.login, u.type,u.created_at, u.followers, u.follower_list, u.following, u.following_list, u.public_gists, u.public_repos);     
    }

    fclose(out_file);
     
    return i;
}

// Função responsável por avaliar os repositórios.

REPOS *doRepos2 (USER *u, int size_u) {
  //  char buffer[200000];
    char *buffer = malloc (200000000);
    REPOS *repos = NULL;
    REPOS val;
    int i = 0;

    doRepos();

    FILE *data_file = fopen ("saida/repos-ok.csv", "r");   
        
    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");    
    }

    fgets(buffer, 2000000, data_file);                         
    while(fgets (buffer, 2000000, data_file)) {
        repos = realloc(repos, (i+1)*sizeof(REPOS)); 
        val=init_repos2(buffer);
        if ((comp_id (u, size_u, val.owner_id)) && (strtol(val.size, NULL, 10) != 0)) repos[i++] = val;        
    }
    
    fclose(data_file);                                                   

    FILE *out_file = fopen ("saida/repos-final.csv", "w");  

    if (out_file == NULL) {                                                                             
        printf ("Error opening file.\n");                             
    }

    fprintf (out_file, "id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size\n");

    //imprime 
    for (int j = 0; j < i; j++){
        REPOS u = repos [j];

        fprintf (out_file, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", u.id, u.owner_id, u.full_name,u.license, u.has_wiki, u.description, u.language, u.default_branch, u.created_at, u.updated_at, u.forks_count, u.open_issues, u.stargazers_count, u.size);
          
    }

    fclose(out_file);

    return repos;
}                                                  

// Função responsável por avaliar os commits.

int doCommit2 (USER *u, REPOS *r, int size_u, int size_r) {
   // char buffer[2000];
    char *buffer = malloc (200000000);
    COMMIT *commit = NULL;
    COMMIT val;
    int i = 0;
    int v;

    FILE *data_file = fopen ("saida/commits-ok.csv", "r");   
        
    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");           
        return 1;                              
    }

    fgets(buffer, 2000000, data_file);                         
    while(fgets (buffer, 2000000, data_file)) {
        commit = realloc(commit, (i+1)*sizeof(COMMIT)); 
        val = init_commits2(buffer);
        if ((comp_id (u, size_u, val.author_id)) && (comp_id (u, size_u, val.committer_id)) && (comp_rep (r, size_r, val.repo_id))) commit[i++] = val;
    }

    fclose(data_file);                                                   

    FILE *out_file = fopen ("saida/commits-final.csv", "w");  

    if (out_file == NULL) {                                                                             
        printf ("Error opening file.\n");                             
    }

    fprintf (out_file, "repo_id;author_id;committer_id;commit_at;message\n");

    //imprime 
    for (int j = 0; j < i; j++){
        COMMIT u = commit [j];

        fprintf (out_file, "%s;%s;%s;%s;%s",u.repo_id, u.author_id, u.committer_id, u.commit_at, u.message);
          
    }

    fclose(out_file);

    return 0;
}

