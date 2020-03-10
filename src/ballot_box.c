#include "ballot_box.h"
#include "helpers.h"

#include <stdlib.h>
#include <string.h>

// A `ballot_box_t` (defined in `ballot_box.h`) is a linked list made
// of `struct bb_node`s. Each node owns the next node, so the whole list
// is owned by the pointer to the first node.
struct bb_node
{
    ballot_t ballot;
    struct bb_node* next;
};


// The empty ballot box is the null pointer.
const ballot_box_t empty_ballot_box = NULL;

// Destroy Helper Function
static ballot_box_t bb_destroy_one(ballot_box_t); 

void bb_destroy(ballot_box_t bb)
{
    if (bb != empty_ballot_box) {
        while (bb->next != empty_ballot_box) {
            bb = bb_destroy_one(bb);
        }
        ballot_destroy(bb->ballot);
        
        free(bb);
        
    }
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    ballot_box_t head = mallocb(sizeof *head, "bb_insert");
    head->ballot = ballot;
    head->next   = *bbp;
    *bbp = head;
}

ballot_box_t read_ballot_box(FILE* inf)
{
 
    ballot_box_t bbp = empty_ballot_box;
    ballot_t ballot;
    
    for(;;){
        ballot = read_ballot(inf);
        
        if(ballot == NULL) break;
       
        bb_insert(&bbp, ballot);
    }
    return bbp;
    
}

vote_count_t bb_count(ballot_box_t bb)
{
    ballot_box_t dst = bb;
    vote_count_t result = vc_create();
    while(bb != NULL) { 
        count_ballot(result, dst->ballot);
        
        if (dst->next == NULL) {
            break;
        } else {
            dst = dst->next;
        }
    }
    return result;
}

void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    ballot_box_t dst = bb;
    while(bb != NULL) {
        ballot_eliminate(dst->ballot, candidate);
        if (dst->next == NULL) {
            break;
        } else {
            dst = dst->next;
        }
    }
}

char* get_irv_winner(ballot_box_t bb)
{
    for(;;){
        vote_count_t tally  = bb_count(bb);
        size_t total_votes = vc_total(tally);
        if (total_votes ==  0){
            vc_destroy(tally);
            return NULL;
        }
        
        const char* winner = vc_max(tally);
        size_t winnervotes = vc_lookup(tally, winner);
        const char* loser = vc_min(tally);

        if(winnervotes*2 > total_votes){
            char* absolute_winner = strdupb(winner, "get_irv_winner");
            
            vc_destroy(tally);
            
            return absolute_winner;
        } else {
            bb_eliminate(bb, loser);
            vc_destroy(tally);
            
        }        
    }
    
}

    
static ballot_box_t bb_destroy_one(ballot_box_t bb)
{
    ballot_box_t next = bb->next;
    ballot_destroy(bb->ballot);
    free(bb);
    return next;
}
