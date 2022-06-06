#ifndef SYS_H
#define SYS_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_Length 30
#define FILE_NAME 10
#define FILE_SIZE 4096
#define DIR_NUM 10
#define PATH_Length 30

typedef struct FCBs{
    char txt[FILE_SIZE];
    int access;
} FCB;

struct DIR{
    char name[FILE_NAME];
    struct DIR* pre;
    struct DIR* next[DIR_NUM];
    FCB file_fcb;
} ;

struct DIR* head;
struct DIR* now;

void init();
void touch(char *);
void mkdir(char *);
void cat(char *);
void ls();
void rm(char *);
void pwd(void);
void cd(char *);
void free_point(struct DIR*);
void exit_sys();

#endif