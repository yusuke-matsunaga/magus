/****************************************************************************************[Solver.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Glueminisat_Solver_h
#define Glueminisat_Solver_h

#include "Vec.h"
#include "Heap.h"
#include "Alg.h"
#include "Options.h"
#include "SolverTypes.h"

// added by nabesima
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS      // for INT32_MAX, ...
#endif
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS     // for PRIu64, ...
#endif
#include <stdint.h>              // for INT32_MAX, ...
#include <inttypes.h>            // for PRIu64, ...
#if defined(HAS_SYS_TIME_H)
#include <sys/time.h>
#endif
#include "LocalVals.h"
#define GLUEMINISAT_VERSION "glueminisat-2.2.8-build122"
#define MAX_STATS_SIZE 15

namespace Glueminisat {

//=================================================================================================
// Solver -- the main class:

class Solver {
public:

    // Constructor/Destructor:
    //
    Solver();
    virtual ~Solver();

    // Problem specification:
    //
    Var     newVar    (bool polarity = true, bool dvar = true); // Add a new variable with parameters specifying variable mode.

    bool    addClause (const vec<Lit>& ps);                     // Add a clause to the solver.
    bool    addEmptyClause();                                   // Add the empty clause, making the solver contradictory.
    bool    addClause (Lit p);                                  // Add a unit clause to the solver.
    bool    addClause (Lit p, Lit q);                           // Add a binary clause to the solver.
    bool    addClause (Lit p, Lit q, Lit r);                    // Add a ternary clause to the solver.
    bool    addClause_(      vec<Lit>& ps);                     // Add a clause to the solver without making superflous internal copy. Will
                                                                // change the passed vector 'ps'.

    // Solving:
    //
    bool    simplify     ();                        // Removes already satisfied clauses.
    bool    solve        (const vec<Lit>& assumps); // Search for a model that respects a given set of assumptions.
    lbool   solveLimited (const vec<Lit>& assumps); // Search for a model that respects a given set of assumptions (With resource constraints).
    bool    solve        ();                        // Search without assumptions.
    bool    solve        (Lit p);                   // Search for a model that respects a single assumption.
    bool    solve        (Lit p, Lit q);            // Search for a model that respects two assumptions.
    bool    solve        (Lit p, Lit q, Lit r);     // Search for a model that respects three assumptions.
    bool    okay         () const;                  // FALSE means solver is in a conflicting state

    void    toDimacs     (FILE* f, const vec<Lit>& assumps);            // Write CNF to file in DIMACS-format.
    void    toDimacs     (const char *file, const vec<Lit>& assumps);
    void    toDimacs     (FILE* f, Clause& c, vec<Var>& map, Var& max);

    // Convenience versions of 'toDimacs()':
    void    toDimacs     (const char* file);
    void    toDimacs     (const char* file, Lit p);
    void    toDimacs     (const char* file, Lit p, Lit q);
    void    toDimacs     (const char* file, Lit p, Lit q, Lit r);

    // Variable mode:
    //
    void    setPolarity    (Var v, bool b); // Declare which polarity the decision heuristic should use for a variable. Requires mode 'polarity_user'.
    void    setDecisionVar (Var v, bool b); // Declare if a variable should be eligible for selection in the decision heuristic.

    // Read state:
    //
    lbool   value      (Var x) const;       // The current value of a variable.
    lbool   value      (Lit p) const;       // The current value of a literal.
    lbool   modelValue (Var x) const;       // The value of a variable in the last model. The last call to solve must have been satisfiable.
    lbool   modelValue (Lit p) const;       // The value of a literal in the last model. The last call to solve must have been satisfiable.
    int     nAssigns   ()      const;       // The current number of assigned literals.
    int     nClauses   ()      const;       // The current number of original clauses.
    int     nLearnts   ()      const;       // The current number of learnt clauses.
    int     nVars      ()      const;       // The current number of variables.
    int     nFreeVars  ()      const;

    // Resource contraints:
    //
    void    setConfBudget(int64_t x);
    void    setPropBudget(int64_t x);
    void    budgetOff();
    void    interrupt();          // Trigger a (potentially asynchronous) interruption of the solver.
    void    clearInterrupt();     // Clear interrupt indicator flag.

    // Memory managment:
    //
    virtual void garbageCollect();
    void    checkGarbage(double gf);
    void    checkGarbage();

    // added by nabesima
    void    setMiniSat220Params();
    void    setPartialModel    (bool b)                    { partial_model = b; }
    void    printOptions       ()                   const;
    void    printProblemStats  ()                   const;
    void    printProblemStats  (double parsed_time) const;
    void    printStats         ()                   const;

    // Extra results: (read-only member variable)
    //
    vec<lbool> model;             // If problem is satisfiable, this vector contains the model (if any).
    vec<Lit>   conflict;          // If problem is unsatisfiable (possibly under assumptions),
                                  // this vector represent the final conflict clause expressed in the assumptions.

    // Mode of operation:
    //
    int       verbosity;
    double    var_decay;
    double    clause_decay;
    double    random_var_freq;
    double    random_seed;
    bool      luby_restart;
    int       ccmin_mode;         // Controls conflict clause minimization (0=none, 1=basic, 2=deep).
    int       phase_saving;       // Controls the level of phase saving (0=none, 1=limited, 2=full).
    bool      rnd_pol;            // Use random polarities for branching heuristics.
    bool      rnd_init_act;       // Initialize variable activities with a small random value.
    double    garbage_frac;       // The fraction of wasted memory allowed before a garbage collection is triggered.

    int       restart_first;      // The initial restart limit.                                                                (default 100)
    double    restart_inc;        // The factor with which the restart limit is multiplied in each restart.                    (default 1.5)
    double    learntsize_factor;  // The intitial limit for learnt clauses is a factor of the original clauses.                (default 1 / 3)
    double    learntsize_inc;     // The limit for learnt clauses is multiplied with this factor each restart.                 (default 1.1)

    int       learntsize_adjust_start_confl;
    double    learntsize_adjust_inc;

    // Statistics: (read-only member variable)
    //
    uint64_t solves, starts, decisions, rnd_decisions, propagations, conflicts;
    uint64_t dec_vars, clauses_literals, learnts_literals, max_literals, tot_literals;

    // added by nabesima
    struct FreqDist {
        vec<uint64_t> freq;
        uint64_t      sum;
        uint64_t      wsum;
        FreqDist(int sz) : sum(0), wsum(0) { freq.growTo(sz + 1); }
        void     inc  (int sz)  { sum++; wsum += sz; sz = sz < freq.size() ? sz : freq.size() - 1; freq[sz]++; }
        int      size () const  { return freq.size(); }
        uint64_t total() const  { return sum; }
        double   avg  () const  { return (double)wsum / sum; }
    };

    // statistics information added by nabesima
    double    real_stime;
    uint32_t  init_vars, simp_vars, unit_clauses, init_clauses, simp_clauses;
    uint64_t  simp_clauses_literals;
    uint64_t  max_confs, min_confs;
    uint64_t  blocked_restarts, last_blocked;
    uint64_t  tot_lbds;                // The total of LBDs of survived learnts.
    uint64_t  global_LBDs;             // The total of LBDs of every learnts (contains removed ones).
    uint64_t  premise_updates;         // The number of changes of values in the premise array.
    uint64_t  rm_lrn_lits;             // The number of removed literals from learnts by minimization.
    vec<uint32_t> num_probed_lits;     // The number of literals probed by each technique.
    uint64_t  bin_rm_uw_cla_lits;      // The number of removed literals in clauses by unwatched literal elimination.
    uint64_t  bin_rm_uw_lrn_lits;      // The number of removed literals in learnts by unwatched literal elimination.
    uint64_t  lazy_rm_uw_cla_lits;     // The number of removed literals in clauses by unwatched literal elimination.
    uint64_t  lazy_rm_uw_lrn_lits;     // The number of removed literals in learnts by unwatched literal elimination.
    uint32_t  lazy_added_bins;         // The number of added binary resolvents.
    uint32_t  rewrite_dbs;             // The number of rewriting clause database.
    uint64_t  rw_clause_lits;          // The number of rewrited literals in clauess.
    uint64_t  rw_learnt_lits;          // The number of rewrited literals in learnts.
    uint64_t  rw_taut_clauses;         // The number of rewrited tautological clauses.
    uint64_t  rw_taut_learnts;         // The number of rewrited tautological learnts.
    uint64_t  rw_unit_clauses;         // The number of unit clauses generated by rewriting.
    uint64_t  rw_unit_learnts;         // The number of unit learnts generated by rewriting.
    uint32_t  sc_subsumed_learnts;     // The number of subsumed learnts generated from successive conflicts.
    uint64_t  lbd_act_bumps;           // The number of variable activity bumpings by learnts.
    uint64_t  prop_lits_by_learnts;    // The number of propagated literals by learnts.
    uint32_t  reduce_dbs;              // The number of calls for reducing database.
    FreqDist  learnts_len;             // The frequency distribution of length of learnt clauses.
    FreqDist  learnts_lbd;             // The frequency distribution of lbd of learnt clauses.
    FreqDist  succ_confs;              // The frequency distribution of successive conflicts.
    uint64_t  removed_learnts;         // The number of removed learnts.
    int       shrinked_assignments;    // The number of shrinked literals from a model.
    LocalVals<uint32_t> stat_DLVs;     // The bounded queue for storing local conflicting DLVs.
    LocalVals<uint32_t> stat_trails;   // The bounded queue for storing local trail sizes.
    LocalVals<uint32_t> stat_reasons;  // The bounded queue for storing local reason area sizes.
    int       num_param_changes;       // The number of parameter changes by diversity search.
    double    reduce_time;             // The cpu time of reducing clause database.
    double    rewrite_time;            // The cpu time of rewriting clause database.
    double    bin_min_time;            // The cpu time of binary minimization of learned clauses.

protected:

    // Helper structures:
    //
    // modified by nabesima
    //struct VarData { CRef reason; int level; };
    //static inline VarData mkVarData(CRef cr, int l){ VarData d = {cr, l}; return d; }
    struct VarData { CRef reason; int level; Lit premise; };
    static inline VarData mkVarData(CRef cr, int l, Lit p = lit_Undef){ VarData d = {cr, l, p}; return d; }

    struct Watcher {
        CRef cref;
        Lit  blocker;
        Watcher(CRef cr, Lit p) : cref(cr), blocker(p) {}
        bool operator==(const Watcher& w) const { return cref == w.cref; }
        bool operator!=(const Watcher& w) const { return cref != w.cref; }
    };

    struct WatcherDeleted
    {
        const ClauseAllocator& ca;
        WatcherDeleted(const ClauseAllocator& _ca) : ca(_ca) {}
        bool operator()(const Watcher& w) const { return ca[w.cref].mark() == 1; }
    };

    struct VarOrderLt {
        const vec<double>&  activity;
        bool operator () (Var x, Var y) const { return activity[x] > activity[y]; }
        VarOrderLt(const vec<double>&  act) : activity(act) { }
    };

    // DEBUG
    //vec<CRef>           org_clauses;          // List of problem clauses.

    // Solver state:
    //
    bool                ok;               // If FALSE, the constraints are already unsatisfiable. No part of the solver state may be used!
    vec<CRef>           clauses;          // List of problem clauses.
    vec<CRef>           learnts;          // List of learnt clauses.
    double              cla_inc;          // Amount to bump next clause with.
    vec<double>         activity;         // A heuristic measurement of the activity of a variable.
    double              var_inc;          // Amount to bump next variable with.
    OccLists<Lit, vec<Watcher>, WatcherDeleted>
                        watches,          // 'watches[lit]' is a list of constraints watching 'lit' (will go there if literal becomes true).
                        bin_watches;      // watch list for binary clauses.
    vec<lbool>          assigns;          // The current assignments.
    vec<char>           polarity;         // The preferred polarity of each variable.
    vec<char>           decision;         // Declares if a variable is eligible for selection in the decision heuristic.
    vec<Lit>            trail;            // Assignment stack; stores all assigments made in the order they were made.
    vec<int>            trail_lim;        // Separator indices for different decision levels in 'trail'.
    vec<VarData>        vardata;          // Stores reason and level for each variable.
    int                 qhead;            // Head of queue (as index into the trail -- no more explicit propagation queue in MiniSat).
    int                 simpDB_assigns;   // Number of top-level assignments since last execution of 'simplify()'.
    int64_t             simpDB_props;     // Remaining number of propagations that must be made before next execution of 'simplify()'.
    vec<Lit>            assumptions;      // Current set of assumptions provided to solve by the user.
    Heap<VarOrderLt>    order_heap;       // A priority queue of variables ordered with respect to the variable activity.
    double              progress_estimate;// Set by 'search()'.
    bool                remove_satisfied; // Indicates whether possibly inefficient linear scan for satisfied clauses should be performed in 'simplify'.

    ClauseAllocator     ca;

    // Options added by nabesima
    bool                partial_model;          // Generate a partial model if possible.
    bool                bin_propagation;        // Propagations by binary clauses are preferred to longer clauses.
    int                 blocker_mode;           // The blocker strategy.
    int                 bin_lrn_min;            // Apply learnt clause minimization by binary clauses.
    int                 lrn_min_size;           // The minimum size required to minimize a learnt clause.
    bool                lrn_min_var_bump;       // Bump the activity of variables used in clause minimization.
    bool                bin_uw_lit_elim;        // Apply unwatched literal elimination by binary clauses.
    int                 lazy_interval;          // The interval of lazy checking.
    bool                lazy_old_dom;           // Use an old dominator.
    bool                lazy_fls_probing;       // Apply lazy false-literal probing.
    bool                lazy_pol_probing;       // Apply lazy polarity probing.
    bool                lazy_eqv_probing;       // Apply lazy equivalent literal probing.
    bool                lazy_bin_probing;       // Apply lazy binary clause probing.
    bool                lazy_cla_probing;       // Apply lazy clause probing.
    int                 lazy_lrn_min;           // Apply learnt clause minimization by binary clauses or resolvents
    bool                lazy_uw_lit_elim;       // Apply lazy unwatched literal elimination.
    bool                lazy_bin_add;           // On-demand addition of binary resolvents.
    bool                lazy_bin_decay;         // Variable activity is decayed after on-demand addition.
    bool                lazy_simp;              // Use lazy simplification methods.
    double              prop_min_lits;          // The minimum rate of newly probed literals for propagation.
    int                 prop_min_starts;        // The minimum number of restarts for propagation of probed literals.
    double              rewrite_min_lits;       // The minimum rate of newly probed equivalent literals for rewriting.
    int                 rewrite_min_starts;     // The minimum number of restarts for rewriting.
    bool                rand_attach;            // Attach clauses by randomized order.
    bool                sc_subsump_check;       // Apply subsumption checking for learnts generated from successive conflicts.
    int                 lbd_type;               // Type of learned clause measure.
    int                 lbd_update_mode;        // The LBD update strategy.
    int                 dynamic_size;           // A clause size threshold in dynamic size evaluation criteria.
    bool                decay_err;              // Use deecaying factor with a error.
    double              decay_err_diff;         // A difference parameter to generate a deecaying factor with a error.
    bool                init_fluc;              // Fluctuate variable activity widely in initial search.
    uint32_t            init_fluc_period;       // Fluctuation period in the number of conflicts.
    int                 init_fluc_base;         // Base interval in fluctuation.
    double              init_fluc_decay;        // Initial activity start decay factor for fluctuation.
    bool                init_fluc_finished;     // Whether the fluctation period is finished.
    bool                decaying_restart;       // Variable activity is decayed after every restart.
    bool                uip_bumping;            // Bump the uip for each conflict.
    double              white_act_weight;       // The weight of activity bumping for white (current level) nodes.
    double              black_act_weight;       // The weight of activity bumping for black (previous levels) nodes.
    int                 lbd_act_bumping;        // Bump variable activity implied from low-lbd learnts in conflict analysis.
    double              lbd_act_rate;           // The conditional ratio for LBD activity bumping.
    double              lbd_act_weight;         // The weight of activity bumping for LBD activity bumping.
    int                 restart_strategy;       // The restart strategy.
    int                 restart_blocking;       // Block the restart if local propagations per conflict exceeds global one.
    double              lbd_restart_rate;       // Restart if local LBD average * this val < threashold.
    double              dec_restart_rate;       // Restart if local decisions/conflict average * this val < threashold.
    double              blk_restart_rate;       // Block if local trail average * this val > global one.
    int                 lbd_queue_size;         // The window size of local average for LBDs.
    int                 dec_queue_size;         // The window size of local average for decisions/conflict.
    int                 trl_queue_size;         // The window size of local trails.
    double              blk_restart_weight;     // The scale of wait time when restart is blocked.
    int                 reduce_db;              // The reduction strategy of learnts.
    double              reduction_rate;         // The reduction rate of learnts.
    int                 max_lbd;                // The max LBD of survived learnt clauses in reduce DB
    int                 inc_lrn_limit;          // Increment the limit of learnt clauses if many better clauses exist.
    double              lbd_dec_rate;           // Learnts with higer decreasing LBD rate than this rate are maintained.
    double              dlbd_decay;             // The delta-LBD decay factor.
    int                 frozen_lbd;             // Protect clauses if their LBD decrease and is lower than this threashold.
    bool	            ag_reduce_db;           // Use the aggressive reduce DB strategy.
    bool	            lv0_reduce_db;          // The reduce db is applied at DLV 0.
    int                 reduce_db_base;         // The initial reduce-DB limit.
    int                 reduce_db_inc;          // The factor with which the reduce-DB limit is added to the limit.
    bool                diverse_strategy;       // Use various kinds of search parameters.
    int                 diverse_base;           // The base number of restarts for changing search parameters.
    int                 stat_conf_size;         // The base number of conflicts for statistics infomation.
    bool                use_minisat_param;      // Behaves as MiniSat 2.2.0.

    // Search parameters for diverse strategy.
    vec<double>         dsp_var_decay;
    vec<int>            dsp_phase_saving;
    vec<bool>           dsp_blocker;
    vec<bool>           dsp_lazy_old_dom;
    vec<int>            dsp_lbd_update;
    vec<bool>           dsp_uip_bumping;
    vec<int>            dsp_restart_strategy;
    vec<double>         dsp_lbd_restart_rate;
    vec<double>         dsp_dec_restart_rate;

    // added by nabesima
    struct PrbLit {
        Lit  p;
        int  type;
        PrbLit(Lit q, int t) : p(q), type(t) {}
    };
    struct EqLits {
        vec<Lit> lits;
        Lit      delegate_;
        EqLits() : delegate_(lit_Undef) {}
        Lit        delegate    ()               const { assert(delegate_ != lit_Undef); return delegate_; }
        int        size        (void)           const { return lits.size(); }
        void       push        (const Lit& p)         { lits.push(p); if (lits.size() == 1) delegate_ = p; }
        void       eliminate   ()                     { lits.clear(true); delegate_ = lit_Undef; }
        const Lit& operator [] (int index)      const { return lits[index]; }
        Lit&       operator [] (int index)            { return lits[index]; }
    };

    // State data added by nabesima
    uint32_t            lbd_comps;              // The number of LBD computation.
    vec<uint32_t>       lbd_time;               // The counter to compute LBD score.
    vec<Lit>            implied_by_learnts;     // A set of literals implied by learnts.
    uint32_t            scf_comps;              // The number of subsumption checkings for learnts generated from successive conflicts.
    vec<uint32_t>       scf_time;               // The counter to identify literal occurence.
    uint32_t            touch_comps;            // The number of touches.
    vec<uint32_t>       touch_time;             // The counter to identify touched variable.
    bool                conflict_probing;       // True if a conflict occurs in probing.
    vec<lbool>          probed_vals;            // A set of probed truth values.
    vec<PrbLit>         probed_lits;            // A set of newly probed literals.
    uint64_t            propagated_starts;      // The number of restarts at when the last propagation of probed literals occurs.
    vec<Lit>            bin_premise;            // Premise literals for each literal in binary clauses.
    vec<Lit>            premise_lit;            // Premise literals for each literal in binary resolvents.
    vec<int>            lit2eq_lits;            // The mapping from literals to the equivalent literal sets.
    vec<EqLits>         eq_lits;                // The mapping from literals to the equivalent literal sets.
    uint32_t            rewrited_lits;          // The number of rewrited literals.
    uint64_t            rewrited_starts;        // The number of restarts at when the last rewriting occurs.
    LocalVals<uint32_t> local_LBDs;             // The bounded queue for storing local learnt's LBDs.
    LocalVals<uint32_t> local_trails;           // The bounded queue for storing local trail size.
    int                 num_progress_report;    // The number of progress printing.
    double              progress_report_base;   // The initial number of conflicts to report progress.
    double              progress_report_inc;    // The limit for reporting progress is multiplied with this factor.
    uint64_t            next_progress_report;   // The number of conflicts to report progress.

    // Temporaries (to reduce allocation overhead). Each variable is prefixed by the method in which it is
    // used, exept 'seen' wich is used in several places.
    //
    vec<char>           seen;
    vec<Lit>            analyze_stack;
    vec<Lit>            analyze_toclear;
    vec<Lit>            add_tmp;

    double              max_learnts;
    double              learntsize_adjust_confl;
    int                 learntsize_adjust_cnt;

    // added by nabesima
    vec<char>           bss_seen;
    vec<Lit>            bss_seen_toclear;
    vec<char>           bss_reached;
    vec<Lit>            bss_reached_toclear;
    vec<Lit>            bin_check_lits;

    // Resource contraints:
    //
    int64_t             conflict_budget;    // -1 means no budget.
    int64_t             propagation_budget; // -1 means no budget.
    bool                asynch_interrupt;

    // Main internal methods:
    //
    void     insertVarOrder   (Var x);                                                 // Insert a variable in the decision order priority queue.
    Lit      pickBranchLit    ();                                                      // Return the next decision variable.
    void     newDecisionLevel ();                                                      // Begins a new decision level.
    Lit      lastDecisionLit  () const;                                                // Return the last decision literal. added by nabesima
    //void     uncheckedEnqueue (Lit p, CRef from = CRef_Undef);                         // Enqueue a literal. Assumes value of literal is undefined.
    void     uncheckedEnqueue (Lit p, CRef from = CRef_Undef, Lit pre = lit_Undef);    // Enqueue a literal. Assumes value of literal is undefined.  modified by nabesima
    bool     enqueue          (Lit p, CRef from = CRef_Undef);                         // Test if fact 'p' contradicts current state, enqueue otherwise.
    CRef     propagate        ();                                                      // Perform unit propagation. Returns possibly conflicting clause.
    bool     propProbedLits   ();                                                      // Propagates probed literals.
    void     lazyProbing      (Lit p, Lit new_dom);                                    // Apply lazy probing techniques. added by nabesima
    void     addProbedLit     (Lit p, int type);                                       // Adds a probed literal to the probed literal cache.
    void     putEquivLit      (Lit p, Lit q);                                          // Put two equivalent literals in the table. added by nabesima
    void     cancelUntil      (int level);                                             // Backtrack until a certain level.
    // modified by nabesima
    //void     analyze          (CRef confl, vec<Lit>& out_learnt, int& out_btlevel);    // (bt = backtrack)
    void     analyze          (CRef confl, vec<Lit>& out_learnt, int& out_btlevel, int& lbd);    // (bt = backtrack)
    void     analyzeFinal     (Lit p, vec<Lit>& out_conflict);                         // COULD THIS BE IMPLEMENTED BY THE ORDINARIY "analyze" BY SOME REASONABLE GENERALIZATION?
    bool     litRedundant     (Lit p, uint32_t abstract_levels);                       // (helper method for 'analyze()')
    void     minimizeByBins   (vec<Lit>& out_learnt);                                  // Minimize a learnt by binary self-subsumption checking. added by nabesima
    void     minimizeByBins   (vec<Lit>& out_learnt, vec<Lit>& premise, bool deep);    // Minimize a learnt by binary self-subsumption checking. added by nabesima
    bool     probePremises    (vec<Lit>& premise);                                     // Probes on the specified premise array.
    lbool    search           (int nof_conflicts);                                     // Search for a given number of conflicts.
    lbool    solve_           ();                                                      // Main solve method (assumptions given in 'assumptions').
    void     reduceDB         ();                                                      // Reduce the set of learnt clauses.
    void     removeSatisfied  (vec<CRef>& cs);                                         // Shrink 'cs' to contain only non-satisfied clauses.
    bool     rewriteClauses   ();                                                      // Rewrite equivalent literals in clauses into a representative one. added by nabesima
    bool     rewriteClauses   (vec<CRef>& cs, uint64_t& rw_lits, uint64_t& taut_clas, uint64_t& fixed_vars);
    void     rebuildOrderHeap ();

    // Maintaining Variable/Clause activity:
    //
    void     varDecayActivity ();                      // Decay all variables with the specified factor. Implemented by increasing the 'bump' value instead.
    void     varDecayActivity (double decay);          // Decay all variables with the specified factor. Implemented by increasing the 'bump' value instead. added by nabesima
    void     varBumpActivity  (Var v, double inc);     // Increase a variable with the current 'bump' value.
    void     varBumpActivity  (Var v);                 // Increase a variable with the current 'bump' value.
    void     claDecayActivity ();                      // Decay all clauses with the specified factor. Implemented by increasing the 'bump' value instead.
    void     claBumpActivity  (Clause& c);             // Increase a clause with the current 'bump' value.

    // Operations on clauses:
    //
    void     attachClause     (CRef cr);               // Attach a clause to watcher lists.
    void     attachAllClauses ();                      // Attach all clauses and learnts. added by nabesima.
    void     detachClause     (CRef cr, bool strict = false); // Detach a clause to watcher lists.
    void     detachAllClauses ();                      // Detach all clauses and learnts. added by nabesima.
    void     removeClause     (CRef cr);               // Detach and free a clause.
    void     removeClauseNoDetach(CRef cr);            // Free a clause without detach. added by nabesima
    bool     locked           (const Clause& c) const; // Returns TRUE if a clause is a reason for some implication in the current state.
    bool     satisfied        (const Clause& c) const; // Returns TRUE if a clause is satisfied in the current state.

    void     relocAll         (ClauseAllocator& to);

    // Misc:
    //
    int      decisionLevel    ()      const; // Gives the current decisionlevel.
    uint32_t abstractLevel    (Var x) const; // Used to represent an abstraction of sets of decision levels.
    CRef     reason           (Var x) const;
    int      level            (Var x) const;
    Lit      premise          (Var x) const; // added by nabesima
    double   progressEstimate ()      const; // DELETE THIS ?? IT'S NOT VERY USEFUL ...
    bool     withinBudget     ()      const;

    // added by nabesima
    int      getLBD             (const Lit* ps, int size);
    int      getLBD             (vec<Lit>& ps)    { return getLBD((Lit*)ps, ps.size());     }
    int      getLBD             (const Clause& c) { return getLBD((const Lit*)c, c.size()); }
    int      getDynamicSize     (const Lit* ps, int size);
    int      getDynamicSize     (vec<Lit>& ps)    { return getDynamicSize((Lit*)ps, ps.size());     }
    int      getDynamicSize     (const Clause& c) { return getDynamicSize((const Lit*)c, c.size()); }
    void     incScfComps        ();
    void     incTouchComps      ();
    void     makePartialModel   ();
    void     printLit           (Lit l, bool detail=true) const;
    void     printLits          (vec<Lit>& lits)          const;
    void     printClause        (const Clause& c)         const;
    void     printSortedClause  (const Clause& c)         const;
    void     printDecisionStack (int from=0)              const;
    void     printProgressHeader()                        const;
    void     printProgress      (const char sign = ' ');

    // Static helpers:
    //

    // added by nabesima
    template<class T>
    inline static void shuffle(vec<T>& array, double seed) {
        // Fisher–Yates shuffle
        T tmp;
        for (int i = array.size() - 1; i > 0; i--) {
            int idx = irand(seed, i + 1);
            tmp = array[idx]; array[idx] = array[i]; array[i] = tmp;
        }
    }
    // Returns a value with a error.
    inline static double add_error(double val, double diff = 0.01) {
        double e = 0.0;
        while (e < val) e += diff;
        return e;
    }

    // Returns a random float 0 <= x < 1. Seed must never be 0.
    static inline double drand(double& seed) {
        seed *= 1389796;
        int q = (int)(seed / 2147483647);
        seed -= (double)q * 2147483647;
        return seed / 2147483647; }

    // Returns a random integer 0 <= x < size. Seed must never be 0.
    static inline int irand(double& seed, int size) {
        return (int)(drand(seed) * size); }
};


//=================================================================================================
// Implementation of inline methods:

inline CRef Solver::reason (Var x) const { return vardata[x].reason; }
inline int  Solver::level  (Var x) const { return vardata[x].level; }
// added by nabesima
inline Lit  Solver::premise(Var x) const { return vardata[x].premise; }

inline void Solver::insertVarOrder(Var x) {
    if (!order_heap.inHeap(x) && decision[x]) order_heap.insert(x); }

// added by nabesima
inline void Solver::addProbedLit(Lit p, int type) {
    lbool b = probed_vals[var(p)] ^ sign(p);
    if (b == l_Undef) {
        probed_vals[var(p)] = lbool(!sign(p));
        if (value(p) == l_True && level(var(p)) == 0)    // otherwise, conflict will occurs in propagation of probed literals.
            return;
        num_probed_lits[type]++;
        probed_lits.push(PrbLit(p, type));
    }
    else if (b == l_False)
        conflict_probing = false;
}


// modified by nabesima
//inline void Solver::varDecayActivity() { var_inc *= (1 / var_decay); }
inline void Solver::varDecayActivity() { varDecayActivity(var_decay); }
inline void Solver::varDecayActivity(double decay) { var_inc *= (1 / decay); }
inline void Solver::varBumpActivity(Var v) { varBumpActivity(v, var_inc); }
inline void Solver::varBumpActivity(Var v, double inc) {
    if ( (activity[v] += inc) > 1e100 ) {
        // Rescale:
        for (int i = 0; i < nVars(); i++)
            activity[i] *= 1e-100;
        var_inc *= 1e-100;
    }

    // Update order_heap with respect to new activity:
    if (order_heap.inHeap(v))
        order_heap.decrease(v); }

inline void Solver::claDecayActivity() { cla_inc *= (1 / clause_decay); }
inline void Solver::claBumpActivity (Clause& c) {
        if ( (c.activity() += cla_inc) > 1e20 ) {
            // Rescale:
            for (int i = 0; i < learnts.size(); i++)
                ca[learnts[i]].activity() *= 1e-20;
            cla_inc *= 1e-20; } }

inline void Solver::checkGarbage(void){ return checkGarbage(garbage_frac); }
inline void Solver::checkGarbage(double gf){
    if (ca.wasted() > ca.size() * gf)
        garbageCollect(); }

// NOTE: enqueue does not set the ok flag! (only public methods do)
inline bool     Solver::enqueue         (Lit p, CRef from)      { return value(p) != l_Undef ? value(p) != l_False : (uncheckedEnqueue(p, from), true); }
inline bool     Solver::addClause       (const vec<Lit>& ps)    { ps.copyTo(add_tmp); return addClause_(add_tmp); }
inline bool     Solver::addEmptyClause  ()                      { add_tmp.clear(); return addClause_(add_tmp); }
inline bool     Solver::addClause       (Lit p)                 { add_tmp.clear(); add_tmp.push(p); return addClause_(add_tmp); }
inline bool     Solver::addClause       (Lit p, Lit q)          { add_tmp.clear(); add_tmp.push(p); add_tmp.push(q); return addClause_(add_tmp); }
inline bool     Solver::addClause       (Lit p, Lit q, Lit r)   { add_tmp.clear(); add_tmp.push(p); add_tmp.push(q); add_tmp.push(r); return addClause_(add_tmp); }
// modified by nabesima
//inline bool     Solver::locked          (const Clause& c) const { return value(c[0]) == l_True && reason(var(c[0])) != CRef_Undef && ca.lea(reason(var(c[0]))) == &c; }
inline bool     Solver::locked          (const Clause& c) const {
    if (bin_propagation && c.size() == 2) {
        return (value(c[0]) == l_True && reason(var(c[0])) != CRef_Undef && ca.lea(reason(var(c[0]))) == &c) ||
               (value(c[1]) == l_True && reason(var(c[1])) != CRef_Undef && ca.lea(reason(var(c[1]))) == &c);
    }
    return value(c[0]) == l_True && reason(var(c[0])) != CRef_Undef && ca.lea(reason(var(c[0]))) == &c;
}
inline void     Solver::newDecisionLevel()                      { trail_lim.push(trail.size()); }
inline Lit      Solver::lastDecisionLit ()                const { return trail_lim.size() > 0 ? trail[trail_lim.last()] : lit_Undef; }

inline int      Solver::decisionLevel ()      const   { return trail_lim.size(); }
inline uint32_t Solver::abstractLevel (Var x) const   { return 1 << (level(x) & 31); }
inline lbool    Solver::value         (Var x) const   { return assigns[x]; }
inline lbool    Solver::value         (Lit p) const   { return assigns[var(p)] ^ sign(p); }
inline lbool    Solver::modelValue    (Var x) const   { return model[x]; }
inline lbool    Solver::modelValue    (Lit p) const   { return model[var(p)] ^ sign(p); }
inline int      Solver::nAssigns      ()      const   { return trail.size(); }
// modified by nabesima
//inline int      Solver::nClauses      ()      const   { return clauses.size(); }
inline int      Solver::nClauses      ()      const   { return clauses.size() + unit_clauses; }
inline int      Solver::nLearnts      ()      const   { return learnts.size(); }
inline int      Solver::nVars         ()      const   { return vardata.size(); }
inline int      Solver::nFreeVars     ()      const   { return (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]); }
inline void     Solver::setPolarity   (Var v, bool b) { polarity[v] = b; }
inline void     Solver::setDecisionVar(Var v, bool b)
{
    if      ( b && !decision[v]) dec_vars++;
    else if (!b &&  decision[v]) dec_vars--;

    decision[v] = b;
    insertVarOrder(v);
}
inline void     Solver::setConfBudget(int64_t x){ conflict_budget    = conflicts    + x; }
inline void     Solver::setPropBudget(int64_t x){ propagation_budget = propagations + x; }
inline void     Solver::interrupt(){ asynch_interrupt = true; }
inline void     Solver::clearInterrupt(){ asynch_interrupt = false; }
inline void     Solver::budgetOff(){ conflict_budget = propagation_budget = -1; }
inline bool     Solver::withinBudget() const {
    return !asynch_interrupt &&
           (conflict_budget    < 0 || conflicts < (uint64_t)conflict_budget) &&
           (propagation_budget < 0 || propagations < (uint64_t)propagation_budget); }

// FIXME: after the introduction of asynchronous interrruptions the solve-versions that return a
// pure bool do not give a safe interface. Either interrupts must be possible to turn off here, or
// all calls to solve must return an 'lbool'. I'm not yet sure which I prefer.
inline bool     Solver::solve         ()                    { budgetOff(); assumptions.clear(); return solve_() == l_True; }
inline bool     Solver::solve         (Lit p)               { budgetOff(); assumptions.clear(); assumptions.push(p); return solve_() == l_True; }
inline bool     Solver::solve         (Lit p, Lit q)        { budgetOff(); assumptions.clear(); assumptions.push(p); assumptions.push(q); return solve_() == l_True; }
inline bool     Solver::solve         (Lit p, Lit q, Lit r) { budgetOff(); assumptions.clear(); assumptions.push(p); assumptions.push(q); assumptions.push(r); return solve_() == l_True; }
inline bool     Solver::solve         (const vec<Lit>& assumps){ budgetOff(); assumps.copyTo(assumptions); return solve_() == l_True; }
inline lbool    Solver::solveLimited  (const vec<Lit>& assumps){ assumps.copyTo(assumptions); return solve_(); }
inline bool     Solver::okay          ()      const   { return ok; }

inline void     Solver::toDimacs     (const char* file){ vec<Lit> as; toDimacs(file, as); }
inline void     Solver::toDimacs     (const char* file, Lit p){ vec<Lit> as; as.push(p); toDimacs(file, as); }
inline void     Solver::toDimacs     (const char* file, Lit p, Lit q){ vec<Lit> as; as.push(p); as.push(q); toDimacs(file, as); }
inline void     Solver::toDimacs     (const char* file, Lit p, Lit q, Lit r){ vec<Lit> as; as.push(p); as.push(q); as.push(r); toDimacs(file, as); }

// added by nabesima
inline int Solver::getLBD(const Lit* ps, int size) {
    lbd_comps++;
    if (lbd_comps == 0)
        for (int i=0; i < lbd_time.size(); i++)
            lbd_time[i] = UINT32_MAX;
    int lbd = 0;
    for (int i=0; i < size; i++) {
        int lv = level(var(ps[i]));
        if (lbd_time[lv] != lbd_comps) {
            lbd_time[lv] = lbd_comps;
            lbd++;
        }
    }
    return lbd;
}
inline int Solver::getDynamicSize(const Lit* ps, int size) {
    if (size < dynamic_size) return size;
    return dynamic_size + size;
}
inline void Solver::incScfComps() {
    scf_comps++;
    if (scf_comps == 0)
        for (int i=0; i < scf_time.size(); i++)
            scf_time[i] = UINT32_MAX;
}

inline void Solver::incTouchComps() {
    touch_comps++;
    if (touch_comps == 0)
        for (int i=0; i < touch_time.size(); i++)
            touch_time[i] = UINT32_MAX;
}

//=================================================================================================
// Debug etc:


//=================================================================================================
}

#endif
