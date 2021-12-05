#ifndef EROWS_H_
#define EROWS_H_

// responsible for handling data for individual lines of texts i.e rows
typedef struct erow
{
    int size;
    int rsize; // render size
    char* chars; // chars pre render i.e \t
    char* render; // rendered chars i.e tabs being rendered
} erow;

#endif