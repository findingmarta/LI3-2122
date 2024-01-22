#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#define __USE_XOPEN
#include <time.h>

int validPar (char *par, int i);
int validType (char *type);
int validDate (char *date);
int validString (char *l);
int validBool (char *booleano);
int validList (char *list, int r);