
#include "guiao1-ex1.h" 

USER init_user2(char *buffer);
REPOS init_repos2(char *buffer);
COMMIT init_commits2 (char *buffer);
int comp_id (USER *u, int size_u, char *v);
int comp_rep (REPOS *r, int size_r, char *v);
int getSize (char buffer[], FILE *data_file);
int doUser2 (USER *user);
REPOS *doRepos2 (USER *u, int size_u);
int doCommit2 (USER *u, REPOS *r, int size_u, int size_r);
