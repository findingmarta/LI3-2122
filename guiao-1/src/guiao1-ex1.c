#include "guiao1-ex1.h"

//Esta função irá verificar se o PARÂMETRO é válido, i.e., se é um número inteiro positivo.
 
int validPar (char *par) {
    char caracInv[] = ".-aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";  
    int N = strlen(caracInv); 
    int r=0;
    if (strlen(par)==0) return 0;
    for (int i=0; i < N; i++) {
        if (strchr (par, caracInv[i]) != NULL) {
            i=N; 
            return 0;
        }
    }
    for (int i=0; i<strlen(par);i++) if (isdigit(par[i])) {
        r++;
        break;
    }
    return r;
}
 
//Função que irá verificar se um type é válido, isto é, se é do tipo User, Bot ou Organization.
    
int validType (char *type) {              
    int r1 = strcmp("User", type);
    int r2 = strcmp("Organization", type);
    int r3 = strcmp("Bot",type);
    
    if ((r1!=0) && (r2!=0) && (r3!=0)) return 0;
    
    return 1;
}

//Função que irá verificar se uma data é válida, isto é, se está no formato AAAA-MM-DD hh:mm:ss. 
    
int validDate (char *date){
    struct tm time = {0};
    strptime(date, "%Y-%m-%d %H:%M:%S", &time);
    if ((date[4] != '-') || (date[7] != '-')) return 0;
    int year = time.tm_year + 1900,
        mon = time.tm_mon +1,
        day = time.tm_mday,
        hour = time.tm_hour,
        min = time.tm_min,
        sec = time.tm_sec,
        r=0;
        
    if (year >= 2005 && year < 2021){
        if ((year == 2005) && (mon >= 4) && (mon <= 12)){ 
            if ((mon == 4) && (day >= 7) && (day <= 30)) r=1;
                else if (((mon == 5)||(mon == 7)||(mon == 8)||(mon == 10)||(mon == 12)) && ((day>=1) && (day<=31))) r=1; //meses com 31 dias de 2005
                    else if (((mon == 6)||(mon == 9)||(mon == 11)) && ((day>=1) && (day<=30))) r=1; //meses com 30 dias de 2005
        }
        else if (((mon == 1)||(mon == 3)||(mon == 5)||(mon == 7)||(mon == 8)||(mon == 10)||(mon == 12)) && ((day>=1) && (day<=31))) r=1; //meses com 31 dias após 2005
                else if (((mon == 4)||(mon == 5)||(mon == 6)||(mon == 9)||(mon == 11)) && ((day>=1) && (day<=30))) r=1; //meses com 30 dias após 2005
                    else if (mon == 2){
                        if ((((year%100 == 0) && (year%400 == 0)) || (year%4 ==0)) && (day>=1  && day<=29)) r=1; //ano bissexto
                            else if ((day>=1  && day<=28)) r=1; //ano não bissexto
    }
    if ((hour >= 0) && (hour <= 23)) {
        if ((min >= 0) && (min <= 59)){
            if ((sec >= 0) && (sec <= 60)) r=1;
        }
    }
    else r=0;
    }
    return r;
}

//Função que irá verificar se um login é válido, isto é, se é uma string.
   
int validString (char *l){
    return (!(strlen(l)==0)); 
}     


//Função que irá verificar se é um booleano.
    
int validBool (char *booleano) {              
    int r1 = strcmp("True", booleano);
    int r2 = strcmp("False", booleano);
    
    if ((r1!=0) && (r2!=0)) return 0;
    
    return 1;
}

//Função que verifica se uma lista é válida, ou seja, se os elementos são todos números inteiros positivos.

int validList (char *list, int r1) {                              
    int len = strlen(list);
    int i = 0; 
    
    if (len < 2) return 0;
   
    if ((!strcmp ("[]", list)) && r1 == 0) return 1;

    char* aux = strdup(list);
    memmove(aux, aux+1, len-2);
    aux[len-2] = 0;
    char *auxList = NULL;

    while (aux) {
        auxList = strdup(strsep (&aux, ","));
        if (validPar (auxList) == 0) return 0;
        i++;
    }

    if (i != r1) return 0;

    return 1;

}

// Esta função irá inicializar o usuário.

USER init_user(char *buffer, int *v) {
    USER u;
    char *aux = NULL;     
    int i = 1;
    int r1,r2; 
    *v=1;                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1: 
                if (validPar(aux)) u.id = aux; 
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
                if (validPar(aux)) {
                    u.followers = aux;
                    sscanf(u.followers, "%d", &r1);
                }
                else *v=0;
                break;
            case 6: 
                u.follower_list = aux;
                if (validList(aux,r1)) u.follower_list = aux; 
                else *v=0;
                break;
            case 7: 
                if (validPar(aux)) {
                    u.following = aux;
                    sscanf(u.following, "%d", &r2);
                }
                else *v=0;
                break;
            case 8:
                if (validList(aux,r2)) u.following_list = aux; 
                else *v=0;          
                break;
            case 9:
                if (validPar(aux)) u.public_gists = aux; 
                else *v=0;
                break; 
            case 10:
                if (validPar(aux)) u.public_repos = aux;
                else *v=0;
                break;
            default:
                printf ("Error acessing aux!\n");
        } 
    }   

    return u;
}

// Esta função irá inicializar o repositório.

REPOS init_repos(char *buffer, int *v) {
    REPOS u;
    char *aux = NULL;     
    int i = 1; 
    *v=1;                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1: 
                if (validPar(aux)) u.id = aux; 
                else *v=0;
                break;
            case 2: 
                if (validPar(aux)) u.owner_id = aux; 
                else *v=0;
                break;
            case 3: 
                if (validString(aux)) u.full_name = aux;   
                else *v=0;
                break;
            case 4: 
                if (validString(aux)) u.license = aux; 
                else *v=0;
                break;
            case 5: 
                if (validBool(aux)) u.has_wiki = aux;
                else *v=0;
                break;
            case 6: 
                u.description = aux;
                break;
            case 7: 
                if (validString(aux)) u.language = aux; 
                else *v=0;
                break;
            case 8:
                if (validString(aux)) u.default_branch = aux; 
                else *v=0;
                break;
            case 9: 
                if (validDate(aux)) u.created_at = aux;
                else *v=0;  
                break;
            case 10: 
                if (validDate(aux)) u.updated_at = aux;
                else *v=0;  
                break;
            case 11: 
                if (validPar(aux)) u.forks_count = aux;
                else *v=0;  
                break;
            case 12: 
                if (validPar(aux)) u.open_issues = aux;
                else *v=0;  
                break;    
            case 13: 
                if (validPar(aux)) u.stargazers_count = aux;
                else *v=0;  
                break; 
            case 14: 
                if (validPar(aux)) u.size = aux;
                else *v=0;  
                break;         
            default:
                printf ("Error acessing aux!\n");
        } 
    }   
    return u;
}

// Esta função irá inicializar o commit.

COMMIT init_commits (char *buffer, int *v) {
    COMMIT u;
    char *aux = NULL;     
    int i = 1; 
    *v=1;                       

    while(buffer != NULL) {
        aux = strdup(strsep(&buffer,";"));

        switch (i++) {
            case 1:     
                if (validPar(aux)) u.repo_id = aux;
                else *v=0;  
                break;
            case 2: 
                if (validPar(aux)) u.author_id = aux;
                else *v=0;  
                break;
            case 3: 
                if (validPar(aux)) u.committer_id = aux;
                else *v=0;
                break;
            case 4: 
                if (validDate(aux)) u.commit_at = aux; 
                else *v=0;
                break;
            case 5: 
                if (validString(aux)) u.message = aux; 
                else *v=0;
                break;
            default:
                printf ("Error acessing aux!\n");
            }     
    }    
    return u;
}

// Função responsável por avaliar os users.

USER *doUser (void) {
    char *buffer = malloc (200000000);
    USER *user = NULL;
    USER val;
    int i = 0;
    int v;

    FILE *data_file = fopen ("entrada/users.csv", "r");   
        
    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");           
    }

    fgets(buffer, 20000000, data_file);                         
    while(fgets (buffer, 2000000, data_file)) {
        user = realloc(user, (i+1)*sizeof(USER)); 
        char *aux = strdup(buffer);
        val=init_user(buffer, &v);
        if (v) user[i++] = val;           
    }
    fclose(data_file);                                                   

    FILE *out_file = fopen ("saida/users-ok.csv", "w");  

    if (out_file == NULL) {                                                                             
        printf ("Error opening file.\n");                             
    }

    fprintf (out_file, "id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos\n");

    //imprime 
    for (int j = 0; j < i; j++){
        USER u = user [j];
        fprintf (out_file, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s", u.id, u.login, u.type,u.created_at, u.followers, u.follower_list, u.following, u.following_list, u.public_gists, u.public_repos);  
        
    }

    fclose(out_file);

    return user;
}

// Função responsável por avaliar os commits.

int doCommit (void) {
    char *buffer = malloc (200000000);
    COMMIT *commit = NULL;
    COMMIT val;
    int i = 0;
    int v;

    FILE *data_file = fopen ("entrada/commits.csv", "r");   
        
    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");           
        return 1;                              
    }

    fgets(buffer, 20000000, data_file);                         
    while(fgets (buffer, 20000000, data_file)) {
        commit = realloc(commit, (i+1)*sizeof(COMMIT)); 
        val=init_commits(buffer, &v);
        if (v) commit[i++] = val;           
    }

    fclose(data_file);                                                   

    FILE *out_file = fopen ("saida/commits-ok.csv", "w");  

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

// Função responsável por avaliar os repositórios.

int doRepos (void) {
    char *buffer = malloc (200000000);
    //char buffer[20000000];
    REPOS *repos = NULL;
    REPOS val;
    int i = 0;
    int v;

    FILE *data_file = fopen ("entrada/repos.csv", "r");   
        
    if (data_file == NULL) {                                                                             
        printf ("Error opening file.\n");  
    }

    fgets(buffer, 20000000, data_file);                         
    while(fgets (buffer, 20000000, data_file)) {
        repos = realloc(repos, (i+1)*sizeof(REPOS));
        val=init_repos(buffer, &v);
        if (v) repos[i++] = val;            
    }

    fclose(data_file);                                                   

    FILE *out_file = fopen ("saida/repos-ok.csv", "w");  

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

    return 0;
}
