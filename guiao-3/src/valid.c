#include "valid.h"
#include "queries.h"

//Função que irá verificar se um parâmetro é válido.
int validPar (char *par, int j) {
    int N;
    N = strlen(par);
    if (N==0) return 0;                     
    if (j==0) 
        for (int i=0; i < N; i++) 
            if (isdigit (par[i]) == 0) return 0;
    if (j==1) 
        for (int i=0; i < N-1; i++) 
            if (isdigit (par[i]) == 0) return 0;    
    if (j==2) 
        for (int i=1; i < N; i++)
            if (isdigit (par[i]) == 0) return 0;
    return 1;
}

//Função que verifica se uma lista é válida, ou seja, se os elementos são todos números inteiros positivos.
int validList (char *list, int r) {                             
    int i = 0; 
    int len = strlen(list);
    if (len < 2) return 0;
    if ((strcmp (list,"[]")==0) && r==0) return 1;

    char* aux = strdup(list);
    removeChar(aux, '[');
    removeChar(aux, ']');
    char *auxList = NULL;
    while (aux) {
        auxList = (strsep (&aux, ","));                             
        if (validPar (auxList, 2) == 0) {
            free(aux);
            return 0;
        }    
        i++;
    }
    free(aux);
    if (i != r) return 0;
    return 1;
}

//Função que irá verificar se um type é válido, isto é, se é do tipo User, Bot ou Organization.
int validType (char *type) {              
    int r1 = strcmp("User", type);
    int r2 = strcmp("Organization", type);
    int r3 = strcmp("Bot",type);
    if ((r1!=0) && (r2!=0) && (r3!=0)) return 0;
    return 1;
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
    if (year >= 2000 && year < 2023 && mon>=1 && mon<=12 && day>=1 && day<=31){
        if (((mon == 1)||(mon == 3)||(mon == 5)||(mon == 7)||(mon == 8)||(mon == 10)||(mon == 12)) && ((day>=1) && (day<=31))) r=1; //meses com 31 dias após 2005
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
