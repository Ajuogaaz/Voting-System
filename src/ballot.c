#include "ballot.h"
#include "helpers.h"

#include <lib211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};

// Helper Function
static struct entry* find_name(ballot_t, const char*);

ballot_t ballot_create(void)
{
    ballot_t ball = malloc(sizeof(struct ballot));
    if (ball == NULL) exit(2);
    ball->length = 0;
    return ball;
}

void ballot_destroy(ballot_t ballot)
{
    size_t i = 0;
    if (ballot != NULL) {
        while (i < ballot->length) {
            free(ballot->entries[i].name);
            i++;
        }
        free(ballot);
    }
}

void ballot_insert(ballot_t ballot, char* name)
{
    if (ballot->length >= MAX_CANDIDATES) {
        exit(3);
    } else {
        clean_name(name);
        size_t number = ballot->length;
        ballot->entries[number].name = name;
        ballot->entries[number].active = true;
        ++ballot->length;
    }
}

const char* ballot_leader(ballot_t ballot)
{
    size_t i = 0;
    while (ballot != NULL && i < ballot->length){
        if (ballot->entries[i].active == true) return ballot->entries[i].name;
        ++i;
    }
    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    struct entry* ptr;
    
    while((ptr = find_name(ballot, name)) != NULL){
            ptr->active = false;
    }
}

void count_ballot(vote_count_t vc, ballot_t ballot)
 {
    const char* winner = ballot_leader(ballot);
    if(winner != NULL){
        size_t* count = vc_update(vc, winner);
        if(count == NULL)
            {
                exit(4);
            } else {
            ++*count;
        }
    }
}


ballot_t read_ballot(FILE* inf)
{
    char* name;
    size_t i = 0;
    
    ballot_t ballot = ballot_create();
        
    for(;;){
        name = fread_line(inf);

        if(i == 0 && name == NULL){
            ballot_destroy(ballot);
            free(name);
            return NULL;
        }
        
        
        if((name == NULL || strcmp(name, "%") == 0)){
            free(name);
            return ballot;
                        
        }
        
        if(i >= MAX_CANDIDATES){
            free(name);
            exit(3);
        }
        ++i;
               
        ballot_insert(ballot, name);
        
    }

    return ballot;
    
}

void clean_name(char* name)
{
    char* source = name;
    char* destination = name;
    while (*source != '\0') {
        if (isalpha(*source) == 0) {
            ++source;
        } else if (isalpha(*source) == 1) {
            ++source;
            ++destination;
        } else {
            *destination = toupper(*source);
            ++source;
            ++destination;
        }
    }
    *destination = '\0';
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i) {
        bool active = ballot->entries[i].active;
        fprintf(outf, "%c%s%s\n",
                active? ' ' : '[',
                ballot->entries[i].name,
                active? "" : "]");
    }
}

static struct entry* find_name(ballot_t ballot, const char* name) {
    int i = 0;
    while (ballot != NULL && i < ballot->length) {
        if (strcmp(ballot->entries[i].name,name) == 0 && ballot->entries[i].active) {
            return &(ballot->entries[i]);
        }
        ++i;
    }
    return NULL;
}
