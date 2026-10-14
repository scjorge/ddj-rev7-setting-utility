#include "ini_parser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void trim(char *s){
    char *p=s; while(*p && isspace((unsigned char)*p)) p++;
    if(p!=s) memmove(s,p,strlen(p)+1);
    size_t n=strlen(s);
    while(n && isspace((unsigned char)s[n-1])) s[--n]=0;
}

bool ini_get_setting(const char *path,const char *section,bool *enabled,int *option){
    FILE *f=fopen(path,"r");
    if(!f) return false;
    char line[160]; bool in=false, found=false;
    *enabled=false; *option=0;
    while(fgets(line,sizeof line,f)){
        // corta comentário
        char *c=strpbrk(line,";#"); if(c) *c=0;
        trim(line);
        if(line[0]==0) continue;
        if(line[0]=='['){
            char *e=strchr(line,']'); if(!e) continue; *e=0;
            in = (strcmp(line+1,section)==0);
            if(in) found=true;
            continue;
        }
        if(!in) continue;
        char *eq=strchr(line,'='); if(!eq) continue;
        *eq=0; char *key=line, *val=eq+1; trim(key); trim(val);
        if(strcasecmp(key,"enabled")==0)
            *enabled = (strcasecmp(val,"yes")==0||strcasecmp(val,"sim")==0||strcmp(val,"1")==0||strcasecmp(val,"true")==0);
        else if(strcasecmp(key,"option")==0)
            *option = atoi(val);
    }
    fclose(f);
    return found;
}
