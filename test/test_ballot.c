///
/// Tests for functions in ../src/ballot.c.
///

#include "ballot.h"
#include "libvc.h"
#include "helpers.h"

#include <lib211.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Asserts that `clean_name(name)` produces `expected`. (Borrows both
// arguments.)
static void assert_clean_name(const char* name, const char* expected);

// Test case functions (you need more!):
static void test_clean_name(void);
static void test_ballot(void);
static void test_ballot_with_vc(void);
static void maximum(void);


///
/// MAIN FUNCTION
///

int main(int argc, char* argv[])
{
    test_clean_name();
    test_ballot();
    test_ballot_with_vc();
    maximum();
    
}


///
/// TEST CASE FUNCTIONS
///

static void test_clean_name(void)
{
    assert_clean_name("Alan Turing", "ALANTURING");
    assert_clean_name("Stroustrup, Bjarne", "STROUSTRUPBJARNE");
    assert_clean_name("C4P0", "CP");
    assert_clean_name("    ", "");
    assert_clean_name("Brian-Linus","BRIANLINUS");
    assert_clean_name("brian linus","BRIANLINUS");
    assert_clean_name("","");
    assert_clean_name("-","");
    assert_clean_name("BRIAN LINUS","BRIANLINUS");
    assert_clean_name("LINUSLOVESMEMES","LINUSLOVESMEMES");
}

static void test_ballot(void)
{
    ballot_t ballot = ballot_create();
    assert(ballot);

    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_insert(ballot, strdupb("A", "test_ballot"));
    ballot_insert(ballot, strdupb("B", "test_ballot"));
    ballot_insert(ballot, strdupb("C", "test_ballot"));

    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "D");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "B");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "A");
    CHECK_STRING(ballot_leader(ballot), "C");
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_destroy(ballot);
}

static void test_ballot_with_vc(void)
{
    ballot_t ball = ballot_create();
    assert(ball);
    ballot_destroy(ball);
    ballot_t ballot = ballot_create();
    ballot_insert(ballot, strdupb("A","test_ballot"));
    ballot_insert(ballot, strdupb("B","test_ballot"));
    ballot_insert(ballot, strdupb("C","test_ballot"));
    vote_count_t vc = vc_create();
    assert(vc);
    CHECK_POINTER(vc_min(vc),NULL);
    count_ballot(vc,ballot);
    CHECK_SIZE(vc_lookup(vc,"A"),1);
    CHECK_SIZE(vc_lookup(vc,"B"),0);
    CHECK_SIZE(vc_lookup(vc,"C"),0);
    count_ballot(vc,ballot);
    CHECK_SIZE(vc_lookup(vc,"A"),2);
    CHECK_SIZE(vc_lookup(vc,"B"),0);
    CHECK_SIZE(vc_lookup(vc,"C"),0);
    CHECK_STRING(vc_max(vc),"A");
    CHECK_STRING(vc_min(vc),"A");
    ballot_eliminate(ballot,"B");
    count_ballot(vc,ballot);
    CHECK_SIZE(vc_lookup(vc,"A"),3);
    CHECK_SIZE(vc_lookup(vc,"B"),0);
    CHECK_SIZE(vc_lookup(vc,"C"),0);
    ballot_eliminate(ballot,"A");
    count_ballot(vc,ballot);
    CHECK_SIZE(vc_lookup(vc,"A"),3);
    CHECK_SIZE(vc_lookup(vc,"B"),0);
    CHECK_SIZE(vc_lookup(vc,"C"),1);
    CHECK_STRING(vc_max(vc),"A");
    CHECK_STRING(vc_min(vc),"C");
    CHECK_STRING(ballot_leader(ballot),"C");
    assert(strcmp(vc_max(vc), "A") == 0);
    ballot_eliminate(ballot,"C");
    count_ballot(vc,ballot);
    CHECK_SIZE(vc_lookup(vc,"A"),3);
    CHECK_SIZE(vc_lookup(vc,"B"),0);
    CHECK_SIZE(vc_lookup(vc,"C"),1);
    vc_destroy(vc);
    ballot_destroy(ballot);
}

static void maximum(void)
{
    ballot_t ballot = ballot_create();
    assert(ballot);
    ballot_insert(ballot, strdupb("A","test_ballot"));
    ballot_insert(ballot, strdupb("B","test_ballot"));
    ballot_insert(ballot, strdupb("C","test_ballot"));
    ballot_insert(ballot, strdupb("D","test_ballot"));
    ballot_insert(ballot, strdupb("E","test_ballot"));
    ballot_insert(ballot, strdupb("F","test_ballot"));
    ballot_insert(ballot, strdupb("G","test_ballot"));
    ballot_insert(ballot, strdupb("H","test_ballot"));
    ballot_insert(ballot, strdupb("I","test_ballot"));
    ballot_insert(ballot, strdupb("J","test_ballot"));
    ballot_insert(ballot, strdupb("K","test_ballot"));
    ballot_insert(ballot, strdupb("L","test_ballot"));
    ballot_insert(ballot, strdupb("M","test_ballot"));
    ballot_insert(ballot, strdupb("N","test_ballot"));
    ballot_insert(ballot, strdupb("O","test_ballot"));
    ballot_insert(ballot, strdupb("P","test_ballot"));
    ballot_insert(ballot, strdupb("Q","test_ballot"));
    vote_count_t vc = vc_create();
    assert(vc);
    vc_destroy(vc);
    ballot_destroy(ballot);
}


///
/// HELPER FUNCTIONS
///

// Asserts that `clean_name(name)` returns `expected`.
static void assert_clean_name(const char* name, const char* expected)
{
    char* actual = strdupb(name, "assert_clean_name");
    clean_name(actual);
    CHECK_STRING(actual, expected);
    free(actual);
}
