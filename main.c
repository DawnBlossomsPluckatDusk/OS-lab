#include "sys.h"
int main(){
    char inst[MAX_Length];
    init();
    while (1){
        printf("myfile:%s>",now->name);
        int i=0;
        memset(inst,0,sizeof(char));
        gets(inst);
        if(strstr(inst,"touch")) touch(inst);
        else if(strstr(inst,"mkdir")) mkdir(inst);
        else if(strstr(inst,"cat")) cat(inst);
        else if(strstr(inst,"ls")) ls();
        else if(strstr(inst,"rm")) rm(inst);
        else if(strstr(inst,"pwd")) pwd();
        else if(strstr(inst,"cd")) cd(inst);
        else if(strstr(inst,"exit")) exit_sys(inst);
        else printf("%s:command not found!!!\n",inst);
    }

    return 0;
}

void init(){
    int i;
    head=(struct DIR*)malloc(sizeof(struct DIR));
    strcpy(head->name,"/");
    for(i=0;i<DIR_NUM;i++) head->next[i]=NULL;
    head->pre=NULL;
    now = head;
    printf("init finished\n");
}
void touch(char *inst){
    int i=0;
    char split[]=" ";
    char *filename=strtok(inst,split);
    filename = strtok(NULL,split);
    if(filename==NULL){
        printf("touch:command error!!!\n");
        return ;
    }
    while((now->next[i]!=NULL)&&(strcmp(filename,now->next[i]->name))) i++;
    if(i>=DIR_NUM){
        printf("this dir has 10 sub-file,please cd to other dir\n");
        return ;
    }
    struct DIR* p;
    if((now->next[i]!=NULL)&&(!strcmp(filename,now->next[i]->name))) p=now->next[i];
    else p=(struct DIR*)malloc(sizeof(struct DIR));
    strcpy(p->name,filename);
    p->pre = now;
    now->next[i] = p;
    p->file_fcb.access=6;
    memset(p->file_fcb.txt,0,sizeof(char));
    char text[FILE_SIZE];
    memset(text,0,sizeof(text));
    
    while(gets(text)){
        if(!strcmp(text,"quit")) break;
        strcat(p->file_fcb.txt,text);
        strcat(p->file_fcb.txt,"\n");
        memset(text,0,sizeof(text));
    }

    printf("touch a file,name is %s\n",filename);
}
void mkdir(char *inst){
    int i=0;
    char split[]=" ";
    char *filename=strtok(inst,split);
    filename = strtok(NULL,split);
    if(filename==NULL){
        printf("mkdir:command error!!!\n");
        return ;
    }
    while((now->next[i]!=NULL)&&(strcmp(filename,now->next[i]->name))) i++;
    if(i>=DIR_NUM){
        printf("this dir has 10 sub-file,please cd to other dir\n");
        return ;
    }

    struct DIR* p;
    if((now->next[i]!=NULL)&&(!strcmp(filename,now->next[i]->name))){
        printf("%s:this dir has existed!!!\n",filename);
        return ;
    }
    else p=(struct DIR*)malloc(sizeof(struct DIR));
    strcpy(p->name,filename);
    p->pre = now;
    now->next[i] = p;
    p->file_fcb.access=1;
    memset(p->file_fcb.txt,0,sizeof(char));

    printf("touch a dir,name is %s\n",filename);
    
    
}
void cat(char *inst){
    int i;
    char split[]=" ";
    char *filename=strtok(inst,split);
    filename = strtok(NULL,split);
    if(filename==NULL){
        printf("cat:command error!!!\n");
        return ;
    }
    for(i = 0;i < 10;i++){
            if((now->next[i]!=NULL) &&(!strcmp(now->next[i]->name,filename))){
                now = now->next[i];
                if(now->file_fcb.access>=4) printf("%s\n",now->file_fcb.txt);
                else printf("no permission to read this file or it is a dir\n");
                return ;
            }
        }
    printf("%s:not found the file\n",filename);
}
void ls(){
    int i;
    for(i=0;i<DIR_NUM;i++){
        if(now->next[i]!=NULL){
            if(now->next[i]->file_fcb.access < 4) printf("%-5d%-20sdir\n",now->next[i]->file_fcb.access,now->next[i]->name);
            else printf("%-5d%-20sfile\n",now->next[i]->file_fcb.access,now->next[i]->name);
        }
    }
    
}
void rm(char *inst){
    int i=0;
    char split[]=" ";
    char *filename=strtok(inst,split);
    filename = strtok(NULL,split);
    if(filename==NULL){
        printf("rm:command error!!!\n");
        return ;
    }
    for(i=0;i<DIR_NUM;i++){
        if((now->next[i]!=NULL) &&(!strcmp(now->next[i]->name,filename))){
            if(now->next[i]->file_fcb.access>=4){
                struct DIR* p=now->next[i];
                now->next[i]=NULL;
                free(p);
            }
            else{
                int j;
                struct DIR* p=now->next[i];
                for(j=0;j<DIR_NUM;j++){
                    if(p->next[i]!=NULL){
                        printf("can't delete not empty dir\n");
                        return ;
                    }
                }
                now->next[i]=NULL;
                free(p);
            }
            return ;
        }
    }
    printf("%s:not found\n",filename);
}
void pwd(){\
int i;
    char path[PATH_Length];
    struct DIR* p=now;
    memset(path,0,sizeof(char));
    printf("now:%-20s\n",p->name);
    while(p->pre!=NULL){
        p=p->pre;
        printf("father:%-20s\n",p->name);
        memset(path,0,sizeof(char));
    }
    return ;
}
void cd(char *inst){
    int i=0;
    char split[]=" ";
    char *filename=strtok(inst,split);
    filename = strtok(NULL,split);
    if(filename==NULL){
        printf("cd:command error!!!\n");
        return ;
    }
    if(!strcmp(filename,"..")){
        now = now->pre;
    }
    else if(strcmp(filename,".")){
        for(i=0;i<DIR_NUM;i++){
            if((now->next[i]!=NULL)&&(!strcmp(now->next[i]->name,filename))){
                if(now->next[i]->file_fcb.access<4) now = now->next[i];
                else{
                    printf("%s:it is a file!!!\n",filename);
                }
                return ;
            }
        }
        printf("%s:not found\n",filename);
    }
}
void exit_sys(){
    free_point(head);
    printf("exit filesystem and free memory\n");
}

void free_point(struct DIR* point){
    int i;
    for(i=0;i<DIR_NUM;i++){
        if(point->next[i]!=NULL) free_point(point->next[i]);
        free(point);
    }
}