
#define TR 1539961

typedef struct repos REPOS;
typedef struct l LANG;
typedef struct mensagens MESS;

void removeReposLines();
void changeRepos(int date, char *str);
char *getId (char *str);
char *getOwnerID(char *str);
char *getLanguage(char *str);
void getTopLanguages (int N, int date, int i, int v);
int inactiveRepos (int date, int i, int v);
void insere_message (char * author_id, int ri, char * repos_id);
int getTopMensagens (int N, int i, int v);
int mainRepos ();