/*
 * Revision Control Information
 *
 * $Source: /users/pchong/CVS/sis/espresso/mincov_int.h,v $
 * $Author: pchong $
 * $Revision: 1.1.1.1 $
 * $Date: 2004/02/07 10:13:58 $
 *
 */
#include "port.h"
#include "utility.h"
#include "sparse.h"
#include "mincov.h"


typedef struct stats_struct stats_t;
struct stats_struct {
    int debug;			/* 1 if debugging is enabled */
    int max_print_depth;	/* dump stats for levels up to this level */
    int max_depth;		/* deepest the recursion has gone */
    int nodes;			/* total nodes visited */
    int component;		/* currently solving a component */
    int comp_count;		/* number of components detected */
    int gimpel_count;		/* number of times Gimpel reduction applied */
    int gimpel;			/* currently inside Gimpel reduction */
    long start_time;		/* cpu time when the covering started */
    int no_branching;
    int lower_bound;
};



typedef struct solution_struct solution_t;
struct solution_struct {
    sm_row *row;
    int cost;
};


extern solution_t *solution_alloc();
extern void solution_free(solution_t*);
extern solution_t *solution_dup(solution_t*);
extern void solution_accept(solution_t*, sm_matrix*, int*, int);
extern void solution_reject(solution_t*, sm_matrix*, int*, int);
extern void solution_add(solution_t*, int*, int);
extern solution_t *solution_choose_best(solution_t*, solution_t*);

extern solution_t *sm_maximal_independent_set(sm_matrix*, int*);
extern solution_t *sm_mincov(sm_matrix*, solution_t*, int*, int, int , int, stats_t*);
extern int gimpel_reduce(sm_matrix*,
			 solution_t*,
			 int*,
			 int,
			 int,
			 int,
			 stats_t*,
			 solution_t**);


#define WEIGHT(weight, col)	(weight == NIL(int) ? 1 : weight[col])
