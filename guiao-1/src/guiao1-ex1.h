#define _DEFAULT_SOURCE 
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

typedef struct user {
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
}USER;

typedef struct commit {
    char *repo_id;
    char *author_id;
    char *committer_id;
    char *commit_at;
    char *message;

}COMMIT;

typedef struct repos {
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
}REPOS; 

int validPar (char *par);
int validType (char *type);
int validDate (char *date);
int validString (char *l);
int validBool (char *booleano);
int validList (char *list, int r);
USER init_user(char *buffer, int *v);
REPOS init_repos(char *buffer, int *v);
COMMIT init_commits (char *buffer, int *v);
USER *doUser (void);
int doCommit (void);
int doRepos (void);
