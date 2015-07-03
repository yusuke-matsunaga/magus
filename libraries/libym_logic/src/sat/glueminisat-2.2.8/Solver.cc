/***************************************************************************************[Solver.cc]
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

#include <math.h>

#include "Sort.h"
#include "Select.h"      // added by nabesima
#include "Solver.h"
#include "System.h"    // added by nabesima
#include "Util.h"      // added by nabesima

#ifndef UINT64_MAX
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFUL
#endif

using namespace Glueminisat;

//=================================================================================================
// Options:


static const char* _cat = "CORE";

static DoubleOption  opt_var_decay         (_cat, "var-decay",   "The variable activity decay factor",            0.95,     DoubleRange(0, false, 1, false));
static DoubleOption  opt_clause_decay      (_cat, "cla-decay",   "The clause activity decay factor",              0.999,    DoubleRange(0, false, 1, false));
static DoubleOption  opt_random_var_freq   (_cat, "rnd-freq",    "The frequency with which the decision heuristic tries to choose a random variable", 0, DoubleRange(0, true, 1, true));
static DoubleOption  opt_random_seed       (_cat, "rnd-seed",    "Used by the random variable selection",         91648253, DoubleRange(0, false, HUGE_VAL, false));
static IntOption     opt_ccmin_mode        (_cat, "ccmin-mode",  "Controls conflict clause minimization (0=none, 1=basic, 2=deep)", 2, IntRange(0, 2));
static IntOption     opt_phase_saving      (_cat, "phase-saving", "Controls the level of phase saving (0=none, 1=limited, 2=full)", 2, IntRange(0, 2));
static BoolOption    opt_rnd_init_act      (_cat, "rnd-init",    "Randomize the initial activity", false);
static BoolOption    opt_luby_restart      (_cat, "luby",        "Use the Luby restart sequence", true);
static IntOption     opt_restart_first     (_cat, "rfirst",      "The base restart interval", 25, IntRange(1, INT32_MAX));  // modified by nabesima. the original interval is 100.
static DoubleOption  opt_restart_inc       (_cat, "rinc",        "Restart interval increase factor", 2, DoubleRange(1, false, HUGE_VAL, false));
static DoubleOption  opt_garbage_frac      (_cat, "gc-frac",     "The fraction of wasted memory allowed before a garbage collection is triggered",  0.20, DoubleRange(0, false, HUGE_VAL, false));

// added by nabesima
//static const char* _decision = "DECISION";
static const char*   _model = "MODEL";
static BoolOption    opt_partial_model     (_model, "partial-model",   "Generate a partial model if possible", false);
static const char*   _prop = "PROPAGATION";
static BoolOption    opt_bin_propagation   (_prop, "bin",              "Propagation by binary clauses are preferred to longer clauses", true);
static IntOption     opt_blocker_mode      (_prop, "blocker",          "The blocker strategy (0=minisat, 1=non-falsified lit)", 0, IntRange(0, 1));
static const char*   _simp = "SIMPLIFICATION";
static IntOption     opt_bin_lrn_min       (_simp, "bin-lrn-min",      "Apply learnt clause minimization by binary clauses", 2, IntRange(0, 2));
static IntOption     opt_lrn_min_size      (_simp, "lrn-min-size",     "The minimum size required to minimize a clause", INT32_MAX, IntRange(0, INT32_MAX));
static BoolOption    opt_lrn_min_var_bump  (_simp, "lrn-min-var-bump", "Bump the activity of variables used in clause minimization", false);
static BoolOption    opt_bin_uw_lit_elim   (_simp, "bin-uw-lit-elim",  "Apply unwatched literal elimination by binary clauses", true);
static IntOption     opt_lazy_interval     (_simp, "lazy-interval",    "Set the interval of lazy checking", 10, IntRange(1, INT32_MAX));
static BoolOption    opt_lazy_old_dom      (_simp, "lazy-old-dom",     "Use an old dominator", false);
static BoolOption    opt_lazy_fls_probing  (_simp, "lazy-fls",         "Apply lazy false-literal probing", true);
static BoolOption    opt_lazy_pol_probing  (_simp, "lazy-pol",         "Apply lazy polarity probing", true);
static BoolOption    opt_lazy_eqv_probing  (_simp, "lazy-eqv",         "Apply lazy equivalent variable probing", true);
static BoolOption    opt_lazy_bin_probing  (_simp, "lazy-bin",         "Apply lazy binary clause probing", true);
static BoolOption    opt_lazy_cla_probing  (_simp, "lazy-cla",         "Apply lazy clause probing", true);
static IntOption     opt_lazy_lrn_min      (_simp, "lazy-lrn-min",     "Apply learnt clause minimization by binary resolvents", 2, IntRange(0, 2));
static BoolOption    opt_lazy_uw_lit_elim  (_simp, "lazy-uw-lit-elim", "Apply lazy unwatched literal elimination", true);
static BoolOption    opt_lazy_bin_add      (_simp, "lazy-bin-add",     "On-demand addition of binary resolvents", true);
static BoolOption    opt_lazy_bin_decay    (_simp, "lazy-add-decay",   "Variable activity is decayed after on-demand addition", false);
static DoubleOption  opt_pr_min_lits       (_simp, "pr-min-lits",      "The minimum rate of newly probed literals for propagation", 0.002, DoubleRange(0, true, 1, true));
static IntOption     opt_pr_min_starts     (_simp, "pr-min-starts",    "The minimum number of restarts to propagate probed literals", 1, IntRange(1, INT32_MAX));
static DoubleOption  opt_rw_min_lits       (_simp, "rw-min-lits",      "The minimum rate of newly probed equivalent literals for rewriting", 0.05, DoubleRange(0, true, 1, true));
static IntOption     opt_rw_min_starts     (_simp, "rw-min-starts",    "The minimum number of restarts for rewriting", 300, IntRange(1, INT32_MAX));
static BoolOption    opt_rand_attach       (_simp, "rand-attach",      "Attach clauses by randomized order after rewriting", false);
static BoolOption    opt_sc_subsump_check  (_simp, "sc-subsump-check", "Apply subsumption checking for learnts generated from successive conflicts (experimental)", false);
static const char*   _eval = "EVALUATION";
static IntOption     opt_lbd_type          (_eval, "lbd",              "The type of LBD measure (0=LBD, 1=PSEUDO LBD, 2=Dynamic Size)", 0, IntRange(0, 2));
static IntOption     opt_lbd_update        (_eval, "lbd-update",       "LBD update strategy (0=none, 1=in propagation, 2=in analysis)", 2, IntRange(0, 2));
static IntOption     opt_dynamic_size      (_eval, "dynamic-size",     "A clause size threshold in dynamic size evaluation criteria", 12, IntRange(0, INT32_MAX));
static const char*   _dec = "DECISION";
static BoolOption    opt_decay_err         (_dec, "decay-err",         "Use deecaying factor with a error", false);
static DoubleOption  opt_decay_err_diff    (_dec, "decay-err-diff",    "A difference parameter to generate a deecaying factor with a error", 0.01, DoubleRange(0, false, 1, false));
static BoolOption    opt_init_fluc         (_dec, "init-fluc",         "Fluctuate variable activity widely in initial search", true);
static IntOption     opt_init_fluc_period  (_dec, "init-fluc-period",  "Fluctuation period in the number of conflicts", 75000, IntRange(1, INT32_MAX));
static DoubleOption  opt_init_fluc_decay   (_dec, "init-fluc-decay",   "Initial activity decay factor for fluctuation", 0.8, DoubleRange(0, false, 1, false));
static BoolOption    opt_decaying_restart  (_dec, "decaying-restart",  "Variable activity is decayed after restart", false);
static BoolOption    opt_uip_bumping       (_dec, "uip-bumping",       "Bump the uip for each conflict", true);
static DoubleOption  opt_white_act_weight  (_dec, "white-act-weight",  "The weight of activity bumping for white (current level) nodes", 1.0, DoubleRange(0, true, HUGE_VAL, false));
static DoubleOption  opt_black_act_weight  (_dec, "black-act-weight",  "The weight of activity bumping for black (previous levels) nodes", 1.0, DoubleRange(0, true, HUGE_VAL, false));
static IntOption     opt_lbd_act_bumping   (_dec, "lbd-act-bumping",   "Bump variable activity implied from lbd learnts in conflict analysis (0=none, 1=small src, 2=large src, 3=good src)", 1, IntRange(0, 3));
static DoubleOption  opt_lbd_act_rate      (_dec, "lbd-act-rate",      "The conditional ratio for LBD activity bumping (1.0 is loosest)", 1.0, DoubleRange(0, true, 5, true));
static DoubleOption  opt_lbd_act_weight    (_dec, "lbd-act-weight",    "The weight of activity bumping for LBD activity bumping", 1.0, DoubleRange(0, true, HUGE_VAL, false));
static const char*   _res = "RESTART";
static IntOption     opt_restart_strategy  (_res, "restart",           "The restart strategy (0=minisat, 1=lbd, 2=lbd + dec/conf)", 1, IntRange(0, 3));
static IntOption     opt_restart_blocking  (_res, "restart-blocking",  "Block the restart if local propagations per conflict exceeds global one (0=none, 1=trail, 2=props/conf)", 1, IntRange(0, 2));
static DoubleOption  opt_lbd_restart_rate  (_res, "lbd-restart-rate",  "Restart if local LBD average * this val > threashold", 0.8, DoubleRange(0, true, 1, true));
static DoubleOption  opt_dec_restart_rate  (_res, "dec-restart-rate",  "Restart if local average of decisions/conflict * this val > threashold", 0.95, DoubleRange(0, true, 1, true));
static DoubleOption  opt_blk_restart_rate  (_res, "blk-restart-rate",  "Block if local trail average * this val > global one", 0.7, DoubleRange(0, true, 5, true));
static IntOption     opt_lbd_queue_size    (_res, "lbd-queue-size",    "The window size of local average for LBDs", 50, IntRange(1, INT32_MAX));
static IntOption     opt_dec_queue_size    (_res, "dec-queue-size",    "The window size of local average for decisions/conflict", 50, IntRange(1, INT32_MAX));
static IntOption     opt_trl_queue_size    (_res, "trl-queue-size",    "The window size of local trails", 5000, IntRange(10, INT32_MAX));
static DoubleOption  opt_blk_restart_weight(_res, "blk-restart-weight","The scale of wait time when restart is blocked", 1, DoubleRange(0, true, HUGE_VAL, false));
static const char*   _red = "REDUCTION";
static IntOption     opt_reduce_db         (_red, "reduce",            "The reduction strategy of learnts (0=LRU, 1=LBD)", 1, IntRange(0, 1));
static DoubleOption  opt_reduction_rate    (_red, "reduction-rate",    "The reduction rate of learnts", 0.5, DoubleRange(0, true, 1, true));
static IntOption	 opt_max_lbd           (_red, "max-lbd",           "The max LBD of survived learnt clauses in reduce DB", 2, IntRange(2, INT32_MAX));
static IntOption	 opt_inc_lrn_limit     (_red, "inc-lrn-limit",     "Increment the limit of learnt clauses if many better clauses exist", 1000, IntRange(0, INT32_MAX));
static DoubleOption  opt_lbd_dec_rate      (_red, "lbd-dec-rate",      "Learnts with higer decreasing LBD rate than this rate are maintained", 1, DoubleRange(0, true, 1, true));
static DoubleOption  opt_dlbd_decay        (_red, "dlbd-decay",        "The delta-LBD decay factor", 0, DoubleRange(0, true, 1, true));
static IntOption     opt_frozen_lbd        (_red, "frozen-lbd",        "Protect clauses if their LBD decrease and is lower than this threashold", 30, IntRange(0, INT32_MAX));
static BoolOption    opt_ag_reduce_db      (_red, "ag-reduce",         "Use the aggressive reduce DB strategy", true);
static BoolOption    opt_lv0_reduce_db     (_red, "lv0-reduce",        "The reduce DB is applied at DLV 0", false);
static IntOption     opt_learnts_init      (_red, "linit",             "The initial size of learnt clauses", 20000, IntRange(1, INT32_MAX));
static IntOption     opt_learnts_inc       (_red, "linc",              "The factor with which the limit of learnts is added in each reduction", 1000, IntRange(1, INT32_MAX));
static const char*   _meta = "META";
static BoolOption    opt_diverse_strategy  (_meta, "diverse",          "Use various kinds of search parameters", false);
static IntOption     opt_diverse_base      (_meta, "diverse-base",     "The base number of restarts for changing search parameters", 500, IntRange(1, INT32_MAX));
static const char*   _stats = "STATS";
static IntOption     opt_stat_conf_size    (_stats, "stat-conf-size",  "The base number of conflicts for statistics infomation", 10000, IntRange(1, INT32_MAX));
static const char*   _solver = "SOLVER";
static BoolOption    opt_minisat		   (_solver, "minisat",        "Behaves as MiniSat 2.2.0", false);

// added by nabesima
// Blocker strategy
#define BLK_MINISAT          0
#define BLK_NON_FALSIFIED    1

// Learned clause measure
#define LM_LBD               0
#define LM_PSEUDO_LBD        1
#define LM_DYNAMIC_SIZE_K    2

// Learned clause minimization
#define MIN_NONE             0
#define MIN_SHALLOW          1
#define MIN_DEEP             2

// LBD update strategy
#define LUP_NONE             0
#define LUP_PROPAGATION      1
#define LUP_ANALYSIS         2

// Reduce DB measure
#define RDB_LRU              0
#define RDB_LBD              1

// LBD activity bumping strategy
#define LAB_NONE             0
#define LAB_SMALL_SRC        1
#define LAB_LARGE_SRC        2
#define LAB_GOOD_SRC         3

// Restart strategy
#define RST_MINISAT          0
#define RST_LBD              1
#define RST_LBD_DEC          2

// Restart blocking
#define RBK_NONE             0
#define RBK_TRAIL            1
#define RBK_PROPS_CONF       2

// The type of probed literals
#define PRB_PREMISE_CHAIN    0
#define PRB_FAILED_LIT       1
#define PRB_POLALITY         2
#define PRB_BIN_CLAUSE       3
#define PRB_CLAUSE           4
#define PRB_EQV_LIT          5
#define PRB_SELF_SUBSUMP     6
#define PRB_NUM_TYPES        7

// The special ID for an empty equivalent literal set.
#define NO_EQ_LITS           0

//=================================================================================================
// Constructor/Destructor:

Solver::Solver() :

    // Parameters (user settable):
    //
    verbosity        (0)
  , var_decay        (opt_var_decay)
  , clause_decay     (opt_clause_decay)
  , random_var_freq  (opt_random_var_freq)
  , random_seed      (opt_random_seed)
  , luby_restart     (opt_luby_restart)
  , ccmin_mode       (opt_ccmin_mode)
  , phase_saving     (opt_phase_saving)
  , rnd_pol          (false)
  , rnd_init_act     (opt_rnd_init_act)
  , garbage_frac     (opt_garbage_frac)
  , restart_first    (opt_restart_first)
  , restart_inc      (opt_restart_inc)

    // Parameters (the rest):
    //
  , learntsize_factor((double)1/(double)3), learntsize_inc(1.1)

    // Parameters (experimental):
    //
  , learntsize_adjust_start_confl (100)
  , learntsize_adjust_inc         (1.5)

    // Statistics: (formerly in 'SolverStats')
    //
  , solves(0), starts(0), decisions(0), rnd_decisions(0), propagations(0), conflicts(0)
  , dec_vars(0), clauses_literals(0), learnts_literals(0), max_literals(0), tot_literals(0)

  // added by nabesima
  , real_stime(realTime())
  , init_vars(0), simp_vars(0), unit_clauses(0), init_clauses(0), simp_clauses(0), simp_clauses_literals(0)
  , max_confs(0), min_confs(UINT64_MAX), blocked_restarts(0), last_blocked(0), tot_lbds(0), global_LBDs(0), premise_updates(0)
  , rm_lrn_lits(0), bin_rm_uw_cla_lits(0), bin_rm_uw_lrn_lits(0), lazy_rm_uw_cla_lits(0), lazy_rm_uw_lrn_lits(0), lazy_added_bins(0)
  , rewrite_dbs(0), rw_clause_lits(0), rw_learnt_lits(0), rw_taut_clauses(0), rw_taut_learnts(0)
  , rw_unit_clauses(0), rw_unit_learnts(0), sc_subsumed_learnts(0)
  , lbd_act_bumps(0), prop_lits_by_learnts(0), reduce_dbs(0)
  , learnts_len(MAX_STATS_SIZE), learnts_lbd(MAX_STATS_SIZE), succ_confs(MAX_STATS_SIZE), removed_learnts(0), shrinked_assignments(0)
  , num_param_changes(0), reduce_time(0.0), rewrite_time(0.0), bin_min_time(0.0)

  , ok                 (true)
  , cla_inc            (1)
  , var_inc            (1)
  , watches            (WatcherDeleted(ca))
  , bin_watches        (WatcherDeleted(ca))
  , qhead              (0)
  , simpDB_assigns     (-1)
  , simpDB_props       (0)
  , order_heap         (VarOrderLt(activity))
  , progress_estimate  (0)
  , remove_satisfied   (true)

  // Options added by nabesima
  , partial_model         (opt_partial_model)
  , bin_propagation       (opt_bin_propagation)
  , blocker_mode          (opt_blocker_mode)
  , bin_lrn_min           (opt_bin_lrn_min)
  , lrn_min_size          (opt_lrn_min_size)
  , lrn_min_var_bump      (opt_lrn_min_var_bump)
  , bin_uw_lit_elim       (opt_bin_uw_lit_elim)
  , lazy_interval         (opt_lazy_interval)
  , lazy_old_dom          (opt_lazy_old_dom)
  , lazy_fls_probing      (opt_lazy_fls_probing)
  , lazy_pol_probing      (opt_lazy_pol_probing)
  , lazy_eqv_probing      (opt_lazy_eqv_probing)
  , lazy_bin_probing      (opt_lazy_bin_probing)
  , lazy_cla_probing      (opt_lazy_cla_probing)
  , lazy_lrn_min          (opt_lazy_lrn_min)
  , lazy_uw_lit_elim      (opt_lazy_uw_lit_elim)
  , lazy_bin_add          (opt_lazy_bin_add)
  , lazy_bin_decay        (opt_lazy_bin_decay)
  , prop_min_lits         (opt_pr_min_lits)
  , prop_min_starts       (opt_pr_min_starts)
  , rewrite_min_lits      (opt_rw_min_lits)
  , rewrite_min_starts    (opt_rw_min_starts)
  , rand_attach           (opt_rand_attach)
  , sc_subsump_check      (opt_sc_subsump_check)
  , lbd_type              (opt_lbd_type)
  , lbd_update_mode       (opt_lbd_update)
  , dynamic_size          (opt_dynamic_size)
  , decay_err             (opt_decay_err)
  , decay_err_diff        (opt_decay_err_diff)
  , init_fluc             (opt_init_fluc)
  , init_fluc_period      (opt_init_fluc_period)
  , init_fluc_base        (0)
  , init_fluc_decay       (opt_init_fluc_decay)
  , init_fluc_finished    (true)
  , decaying_restart      (opt_decaying_restart)
  , uip_bumping           (opt_uip_bumping)
  , white_act_weight      (opt_white_act_weight)
  , black_act_weight      (opt_black_act_weight)
  , lbd_act_bumping       (opt_lbd_act_bumping)
  , lbd_act_rate          (opt_lbd_act_rate)
  , lbd_act_weight        (opt_lbd_act_weight)
  , restart_strategy      (opt_restart_strategy)
  , restart_blocking      (opt_restart_blocking)
  , lbd_restart_rate      (opt_lbd_restart_rate)
  , dec_restart_rate      (opt_dec_restart_rate)
  , blk_restart_rate      (opt_blk_restart_rate)
  , lbd_queue_size        (opt_lbd_queue_size)
  , dec_queue_size        (opt_dec_queue_size)
  , trl_queue_size        (opt_trl_queue_size)
  , blk_restart_weight    (opt_blk_restart_weight)
  , reduce_db             (opt_reduce_db)
  , reduction_rate        (opt_reduction_rate)
  , max_lbd               (opt_max_lbd)
  , inc_lrn_limit         (opt_inc_lrn_limit)
  , lbd_dec_rate          (opt_lbd_dec_rate)
  , dlbd_decay            (opt_dlbd_decay)
  , frozen_lbd            (opt_frozen_lbd)
  , ag_reduce_db          (opt_ag_reduce_db)
  , lv0_reduce_db         (opt_lv0_reduce_db)
  , reduce_db_base        (opt_learnts_init)
  , reduce_db_inc	      (opt_learnts_inc)
  , diverse_strategy      (opt_diverse_strategy)
  , diverse_base          (opt_diverse_base)
  , stat_conf_size        (opt_stat_conf_size)
  , use_minisat_param     (opt_minisat)

  // State data added by nabesima
  , lbd_comps             (0)
  , scf_comps             (0)
  , touch_comps           (0)
  , conflict_probing      (false)
  , propagated_starts     (0)
  , rewrited_lits         (0)
  , rewrited_starts       (0)
  , num_progress_report   (0)
  , progress_report_base  (100)
  , progress_report_inc   (1.2)
  , next_progress_report  (progress_report_base)


    // Resource constraints:
    //
  , conflict_budget    (-1)
  , propagation_budget (-1)
  , asynch_interrupt   (false)
{
    // added by nabesima
    stat_DLVs   .init(stat_conf_size);
    stat_trails .init(stat_conf_size);
    stat_reasons.init(stat_conf_size);
    local_LBDs  .init(lbd_queue_size);
    local_trails.init(trl_queue_size);

    if (use_minisat_param)
        setMiniSat220Params();

    if (init_fluc) {
        init_fluc_base = init_fluc_period / ((var_decay - init_fluc_decay) / 0.01);
        init_fluc_finished = false;
    }

    if (decay_err) {
        var_decay = add_error(var_decay);
        init_fluc_decay = add_error(init_fluc_decay);
    }

    lazy_simp = lazy_fls_probing || lazy_pol_probing || lazy_eqv_probing || lazy_bin_probing || lazy_cla_probing || lazy_lrn_min || lazy_uw_lit_elim || lazy_bin_add;
    num_probed_lits.growTo(PRB_NUM_TYPES);
    eq_lits.push();    // [0] is a dummy

    // Diverse strategy parameters.
    dsp_var_decay.push(0.90);
    dsp_var_decay.push(0.95);
//    dsp_phase_saving.push(0);
//    dsp_phase_saving.push(1);
//    dsp_phase_saving.push(2);
//    dsp_phase_saving.push(2);
    dsp_phase_saving.push(2);
    dsp_blocker.push(0);
    dsp_blocker.push(1);
    dsp_lazy_old_dom.push(true);
    dsp_lazy_old_dom.push(false);
    dsp_lbd_update.push(1);
    dsp_lbd_update.push(2);
    dsp_uip_bumping.push(true);
    dsp_uip_bumping.push(false);
    dsp_restart_strategy.push(1);
//    dsp_restart_strategy.push(1);
//    dsp_restart_strategy.push(2);
    dsp_lbd_restart_rate.push(0.75);
    dsp_lbd_restart_rate.push(0.80);
//    dsp_lbd_restart_rate.push(0.80);
    dsp_lbd_restart_rate.push(0.85);
    dsp_dec_restart_rate.push(0.55);
    dsp_dec_restart_rate.push(0.60);
    dsp_dec_restart_rate.push(0.65);
}

Solver::~Solver()
{
}

// added by nabesima
void Solver::setMiniSat220Params() {
    // Original parameters
    var_decay       = 0.95;
    clause_decay    = 0.999;
    random_var_freq = 0;
    random_seed     = 91648253;
    luby_restart    = true;
    ccmin_mode      = 2;
    phase_saving    = 2;
    rnd_pol         = false;
    rnd_init_act    = false;
    garbage_frac    = 0.20;
    restart_first   = 100;
    restart_inc     = 2;

    // Options added by nabesima
    partial_model    = false;
    bin_propagation  = false;
    blocker_mode     = BLK_MINISAT;
    bin_lrn_min      = false;
    lrn_min_var_bump = false;
    bin_uw_lit_elim  = false;
    lazy_interval    = 1;
    lazy_old_dom     = false;
    lazy_fls_probing = false;
    lazy_pol_probing = false;
    lazy_eqv_probing = false;
    lazy_bin_probing = false;
    lazy_cla_probing = false;
    lazy_lrn_min     = false;
    lazy_uw_lit_elim = false;
    lazy_bin_add     = false;
    lazy_bin_decay   = false;
    rand_attach      = false;
    sc_subsump_check = false;
    lbd_type         = LM_LBD;
    lbd_update_mode  = LUP_NONE;
    dynamic_size     = 0;
    decay_err        = false;
    init_fluc        = false;
    decaying_restart = false;
    uip_bumping      = false;
    white_act_weight = 1.0;
    black_act_weight = 1.0;
    lbd_act_bumping  = LAB_NONE;
    lbd_act_rate     = 1.0;
    lbd_act_weight   = 1.0;
    restart_strategy = RST_MINISAT;
    restart_blocking = false;
    reduce_db        = RDB_LRU;
    reduction_rate   = 0.5;
    max_lbd          = 2;
    inc_lrn_limit    = 0;
    lbd_dec_rate     = 1.0;
    dlbd_decay       = 0;
    frozen_lbd       = 0;
    ag_reduce_db     = false;
    lv0_reduce_db    = false;
}

// added by nabesima
void Solver::printOptions() const {
    // Original parameters
    printf("c Options:\n");
    printf("c CORE\n");
    printf("c  var_decay          = %.3f\n", var_decay);
    printf("c  clause_decay       = %.3f\n", clause_decay);
    printf("c  random_var_freq    = %.3f\n", random_var_freq);
    printf("c  random_seed        = %.3f\n", random_seed);
    printf("c  luby_restart       = %d\n", luby_restart);
    printf("c  ccmin_mode         = %d\n", ccmin_mode);
    printf("c  phase_saving       = %d\n", phase_saving);
    printf("c  rnd_pol            = %d\n", rnd_pol);
    printf("c  rnd_init_act       = %d\n", rnd_init_act);
    printf("c  garbage_frac       = %.3f\n", garbage_frac);
    printf("c  restart_first      = %d\n", restart_first);
    printf("c  restart_inc        = %.3f\n", restart_inc);

    // Options added by nabesima
    printf("c MODEL\n");
    printf("c  partial_model      = %d\n", partial_model);
    printf("c PROPAGATION\n");
    printf("c  bin_propagation    = %d\n", bin_propagation);
    printf("c  blocker_mode       = %d\n", blocker_mode);
    printf("c SIMPLIFICATION\n");
    printf("c  bin_lrn_min        = %d\n", bin_lrn_min);
    printf("c  lrn_min_size       = %d\n", lrn_min_size);
    printf("c  lrn_min_var_bump   = %d\n", lrn_min_var_bump);
    printf("c  bin_uw_lit_elim    = %d\n", bin_uw_lit_elim);
    printf("c  lazy_interval      = %d\n", lazy_interval);
    printf("c  lazy_old_dom       = %d\n", lazy_old_dom);
    printf("c  lazy_fls_probing   = %d\n", lazy_fls_probing);
    printf("c  lazy_pol_probing   = %d\n", lazy_pol_probing);
    printf("c  lazy_eqv_probing   = %d\n", lazy_eqv_probing);
    printf("c  lazy_bin_probing   = %d\n", lazy_bin_probing);
    printf("c  lazy_cla_probing   = %d\n", lazy_cla_probing);
    printf("c  lazy_lrn_min       = %d\n", lazy_lrn_min);
    printf("c  lazy_uw_lit_elim   = %d\n", lazy_uw_lit_elim);
    printf("c  lazy_bin_add       = %d\n", lazy_bin_add);
    printf("c  lazy_bin_decay     = %d\n", lazy_bin_decay);
    printf("c  prop_min_lits      = %.3f\n", prop_min_lits);
    printf("c  prop_min_starts    = %d\n", prop_min_starts);
    printf("c  rw_min_lits        = %.3f\n", rewrite_min_lits);
    printf("c  rw_min_starts      = %d\n", rewrite_min_starts);
    printf("c  rand_attach        = %d\n", rand_attach);
    printf("c  sc_subsump_chk     = %d\n", sc_subsump_check);
    printf("c EVALUATION\n");
    printf("c  lbd_type           = %d\n", lbd_type);
    printf("c  lbd_update_mode    = %d\n", lbd_update_mode);
    printf("c  dynamic_size       = %d\n", dynamic_size);
    printf("c DECISION\n");
    printf("c  decay_err          = %d\n", decay_err);
    printf("c  decay_err_diff     = %.10f\n", decay_err_diff);
    printf("c  init_fluc          = %d\n", init_fluc);
    printf("c  init_fluc_period   = %d\n", init_fluc_period);
    printf("c  init_fluc_decay    = %.3f\n", init_fluc_decay);
    printf("c  decaying_restart   = %d\n", decaying_restart);
    printf("c  uip_bumping        = %d\n", uip_bumping);
    printf("c  white_act_weight   = %.3f\n", white_act_weight);
    printf("c  black_act_weight   = %.3f\n", black_act_weight);
    printf("c  lbd_act_bumping    = %d\n", lbd_act_bumping);
    printf("c  lbd_act_rate       = %.3f\n", lbd_act_rate);
    printf("c  lbd_act_weight     = %.3f\n", lbd_act_weight);
    printf("c RESTART\n");
    printf("c  restart_strategy   = %d\n", restart_strategy);
    printf("c  restart_blocking   = %d\n", restart_blocking);
    printf("c  lbd_restart_rate   = %.3f\n", lbd_restart_rate);
    printf("c  dec_restart_rate   = %.3f\n", dec_restart_rate);
    printf("c  blk_restart_rate   = %.3f\n", blk_restart_rate);
    printf("c  lbd_queue_size     = %d\n", lbd_queue_size);
    printf("c  dec_queue_size     = %d\n", dec_queue_size);
    printf("c  trl_queue_size     = %d\n", trl_queue_size);
    printf("c  blk_restart_weight = %.3f\n", blk_restart_weight);
    printf("c REDUCTION\n");
    printf("c  reduce_db          = %d\n", reduce_db);
    printf("c  reduction_rate     = %.3f\n", reduction_rate);
    printf("c  max_lbd            = %d\n", max_lbd);
    printf("c  inc_lrn_limit      = %d\n", inc_lrn_limit);
    printf("c  lbd_dec_rate       = %.3f\n", lbd_dec_rate);
    printf("c  dlbd_decay         = %.3f\n", dlbd_decay);
    printf("c  frozen_lbd         = %d\n", frozen_lbd);
    printf("c  ag_reduce_db       = %d\n", ag_reduce_db);
    printf("c  lv0_reduce_db      = %d\n", lv0_reduce_db);
    printf("c  reduce_db_base     = %d\n", reduce_db_base);
    printf("c  reduce_db_inc      = %d\n", reduce_db_inc);
    printf("c META\n");
    printf("c  diverse_strategy   = %d\n", diverse_strategy);
    printf("c  diverse_base       = %d\n", diverse_base);
    printf("c STATS\n");
    printf("c  stat_conf_size     = %d\n", stat_conf_size);
    printf("c SOLVER\n");
    printf("c  minisat            = %d\n", use_minisat_param);
}
void Solver::printProblemStats() const {
    printf("c Problem: %d variables, %d clauses\n", nVars(), nClauses());
}
void Solver::printProblemStats(double parsed_time) const {
    printf("c Problem: %d variables, %d clauses, parsing %.2f s\n", nVars(), nClauses(), parsed_time);
}
void Solver::printStats() const {
    double cpu_time = cpuTime();
    double mem_used = memUsedPeak();

    uint32_t probed_vars = 0;
    for (int i=0; i < num_probed_lits.size(); i++)
        probed_vars += num_probed_lits[i];

    printf("c variables         : %-12d   (init %d, after simp %d)\n", nFreeVars(), init_vars, simp_vars);
    printf("c clauses           : %-12d   (init %d, after simp %d)\n", nClauses(), init_clauses, simp_clauses);
    printf("c restarts          : %-12"PRIu64"   (%.2f confs/res, %"PRIu64" ~ %"PRIu64" confs, %"PRIu64" blk, last %"PRIu64")\n", starts - 1, (double)conflicts / starts, min_confs, max_confs, blocked_restarts, last_blocked - 1);
    printf("c conflicts         : %-12"PRIu64"   (%.0f /sec, %"PRIu64" blks, avg %.2f)\n", conflicts, conflicts / cpu_time, succ_confs.total(), succ_confs.avg());
    printf("c decisions         : %-12"PRIu64"   (%4.2f%% random, %.0f /sec)\n", decisions, (float)rnd_decisions*100 / (float)decisions, decisions / cpu_time);
    printf("c propagations      : %-12"PRIu64"   (%.0f /sec, %4.2f lits/dec)\n", propagations, propagations/cpu_time, (float)propagations / decisions);
    printf("c conflict literals : %-12"PRIu64"   (%4.2f%% deleted, %4.2f%% by bin min, %.3f s)\n", tot_literals, (max_literals - tot_literals)*100 / (double)max_literals, (double)rm_lrn_lits*100 / (tot_literals + rm_lrn_lits), bin_min_time);
    printf("c lbd               : %-12.2f   (%4.2f for all, %4.2f%% bumps)\n", (double)tot_lbds / nLearnts(), (double)global_LBDs / conflicts, (double)lbd_act_bumps * 100 / prop_lits_by_learnts);
    printf("c reduce dbs        : %-12d   (%"PRIu64" learnts removed, %4.2f%%, %.3f s)\n", reduce_dbs, removed_learnts, removed_learnts * 100 / (double)conflicts, reduce_time);
    printf("c rewrite dbs       : %-12d   (%"PRIu64"+%"PRIu64" rw-lits, %"PRIu64"+%"PRIu64" taut, %"PRIu64"+%"PRIu64" unit, %.3f s)\n", rewrite_dbs, rw_clause_lits, rw_learnt_lits, rw_taut_clauses, rw_taut_learnts, rw_unit_clauses, rw_unit_learnts, rewrite_time);
    printf("c premise updates   : %-12"PRIu64"   (%4.2f /lit)\n", premise_updates, (double)premise_updates / simp_vars / 2);
    printf("c probed variables  : %-12d   (%d chain, %d fls, %d pol, %d eqv, %d bin, %d cla, %d ss)\n", probed_vars, num_probed_lits[PRB_PREMISE_CHAIN], num_probed_lits[PRB_FAILED_LIT], num_probed_lits[PRB_POLALITY], num_probed_lits[PRB_EQV_LIT], num_probed_lits[PRB_BIN_CLAUSE], num_probed_lits[PRB_CLAUSE], num_probed_lits[PRB_SELF_SUBSUMP]);
    printf("c unwatched lit elim: %-12"PRIu64"   (bin %"PRIu64"+%"PRIu64", %4.2f+%4.2f%%, lazy %"PRIu64"+%"PRIu64", %4.2f+%4.2f%% of %"PRIu64"+%"PRIu64")\n", bin_rm_uw_cla_lits + bin_rm_uw_lrn_lits + lazy_rm_uw_cla_lits + lazy_rm_uw_lrn_lits, bin_rm_uw_cla_lits, bin_rm_uw_lrn_lits, (double)bin_rm_uw_cla_lits * 100 / simp_clauses_literals, (double)bin_rm_uw_lrn_lits * 100 / tot_literals, lazy_rm_uw_cla_lits, lazy_rm_uw_lrn_lits, (double)lazy_rm_uw_cla_lits * 100 / simp_clauses_literals, (double)lazy_rm_uw_lrn_lits * 100 / tot_literals, simp_clauses_literals, tot_literals);
    printf("c lazy added bins   : %-12d\n", lazy_added_bins);
    printf("c subsumed learnts  : %-12d   (%4.2f %%)\n", sc_subsumed_learnts, (double)sc_subsumed_learnts * 100 / conflicts);
    printf("c partial model     : %-12d   (%4.2f %%, %d removed)\n", init_vars - shrinked_assignments, (double)(init_vars - shrinked_assignments) * 100 / init_vars, shrinked_assignments);

    printf("c\n");
    printf("c             learnts len        learnts lbd        succ confs\n");
    printf("c total %10"PRIu64"         %10"PRIu64"         %10"PRIu64"\n",
            learnts_len.total(),
            learnts_lbd.total(),
            succ_confs.total());
    printf("c avg   %10.3f         %10.3f         %10.3f\n",
            learnts_len.avg(),
            learnts_lbd.avg(),
            succ_confs.avg());
    for (int i=1; i < succ_confs.size(); i++)
        printf("c  %2d   %10"PRIu64" (%4.1f%%) %10"PRIu64" (%4.1f%%) %10"PRIu64" (%4.1f%%)\n", i,
                learnts_len.freq[i], learnts_len.freq[i] * 100 / (double)learnts_len.total(),
                learnts_lbd.freq[i], learnts_lbd.freq[i] * 100 / (double)learnts_lbd.total(),
                succ_confs.freq[i], succ_confs.freq[i] * 100 / (double)succ_confs.total());

    printf("c\n");

    if (mem_used != 0)
        printf("c Memory used       : %.2f MB\n", mem_used);
    printf("c CPU time          : %.3f s\n", cpu_time);
    printf("c Real time         : %.3f s\n", realTime() - real_stime);
}
//=================================================================================================
// Minor methods:


// Creates a new SAT variable in the solver. If 'decision' is cleared, variable will not be
// used as a decision variable (NOTE! This has effects on the meaning of a SATISFIABLE result).
//
Var Solver::newVar(bool sign, bool dvar)
{
    int v = nVars();
    watches  .init(mkLit(v, false));
    watches  .init(mkLit(v, true ));
    // added by nabesima
    if (bin_propagation) {
        bin_watches.init(mkLit(v, false));
        bin_watches.init(mkLit(v, true ));
    }
    assigns  .push(l_Undef);
    vardata  .push(mkVarData(CRef_Undef, 0));
    //activity .push(0);
    activity .push(rnd_init_act ? drand(random_seed) * 0.00001 : 0);
    seen     .push(0);
    polarity .push(sign);
    decision .push();
    trail    .capacity(v+1);
    setDecisionVar(v, dvar);

    // added by nabesima
    lbd_time.push(0);
    if (sc_subsump_check) {
        scf_time.push(0);
        scf_time.push(0);
    }
    touch_time.push(0);
    touch_time.push(0);
    if (bin_propagation) {
        bin_premise.push(mkLit(v, false));
        bin_premise.push(mkLit(v, true ));
    }
    if (bin_lrn_min || lazy_lrn_min) {
        bss_seen.push(0);
        bss_seen.push(0);
        bss_reached.push(0);
        bss_reached.push(0);
    }
    if (lazy_simp) {
        premise_lit.push(mkLit(v, false));
        premise_lit.push(mkLit(v, true ));
        lit2eq_lits.push(NO_EQ_LITS);
        lit2eq_lits.push(NO_EQ_LITS);
    }
    if (lazy_simp || bin_lrn_min)
        probed_vals.push(l_Undef);

    return v;
}


bool Solver::addClause_(vec<Lit>& ps)
{
    assert(decisionLevel() == 0);
    if (!ok) return false;

    // Check if clause is satisfied and remove false/duplicate literals:
    sort(ps);
    Lit p; int i, j;
    for (i = j = 0, p = lit_Undef; i < ps.size(); i++)
        if (value(ps[i]) == l_True || ps[i] == ~p)
            return true;
        else if (value(ps[i]) != l_False && ps[i] != p)
            ps[j++] = p = ps[i];
    ps.shrink(i - j);

    if (ps.size() == 0)
        return ok = false;
    else if (ps.size() == 1){
        unit_clauses++;    // added by nabesima
        uncheckedEnqueue(ps[0]);
        return ok = (propagate() == CRef_Undef);
    }else{
        CRef cr = ca.alloc(ps, false);
        clauses.push(cr);
        attachClause(cr);

        // DEBUG
        //org_clauses.push(ca.alloc(ps, false));
    }

    return true;
}


void Solver::attachClause(CRef cr) {
    const Clause& c = ca[cr];
    assert(c.size() > 1);
    // modified by nabesima
    //watches[~c[0]].push(Watcher(cr, c[1]));
    //watches[~c[1]].push(Watcher(cr, c[0]));
    if(bin_propagation && c.size() == 2) {
        bin_watches[~c[0]].push(Watcher(cr, c[1]));
        bin_watches[~c[1]].push(Watcher(cr, c[0]));
    }
    else {
        watches[~c[0]].push(Watcher(cr, c[1]));
        watches[~c[1]].push(Watcher(cr, c[0]));
    }
    // modified by nabesima
    //if (c.learnt()) learnts_literals += c.size();
    //else            clauses_literals += c.size();
    if (c.learnt()) { learnts_literals += c.size(); tot_lbds += c.lbd(); }
    else            { clauses_literals += c.size(); }
}

// added by nabesima
void Solver::attachAllClauses() {
    if (rand_attach) {
        shuffle(clauses, random_seed);
        shuffle(learnts, random_seed);
    }
    for (int i=0; i < learnts.size(); i++)
        attachClause(learnts[i]);
    for (int i=0; i < clauses.size(); i++)
        attachClause(clauses[i]);
}

void Solver::detachClause(CRef cr, bool strict) {
    const Clause& c = ca[cr];
    assert(c.size() > 1);

    // modified by nabesima
//    if (strict){
//        remove(watches[~c[0]], Watcher(cr, c[1]));
//        remove(watches[~c[1]], Watcher(cr, c[0]));
//    }else{
//        // Lazy detaching: (NOTE! Must clean all watcher lists before garbage collecting this clause)
//        watches.smudge(~c[0]);
//        watches.smudge(~c[1]);
//    }
    if (bin_propagation && c.size() == 2) {
        if (strict){
            remove(bin_watches[~c[0]], Watcher(cr, c[1]));
            remove(bin_watches[~c[1]], Watcher(cr, c[0]));
        }else{
            // Lazy detaching: (NOTE! Must clean all watcher lists before garbage collecting this clause)
            bin_watches.smudge(~c[0]);
            bin_watches.smudge(~c[1]);
        }
    }
    else {
        if (strict){
            remove(watches[~c[0]], Watcher(cr, c[1]));
            remove(watches[~c[1]], Watcher(cr, c[0]));
        }else{
            // Lazy detaching: (NOTE! Must clean all watcher lists before garbage collecting this clause)
            watches.smudge(~c[0]);
            watches.smudge(~c[1]);
        }
    }

    if (c.learnt()) learnts_literals -= c.size();
    else            clauses_literals -= c.size();

    // added by nabesima
    if (c.learnt()) tot_lbds -= c.lbd();
}

// added by nabesima
void Solver::detachAllClauses() {
    for (int i=0; i < nVars(); i++) {
        if (bin_propagation) {
            bin_watches[mkLit(i, false)].clear();
            bin_watches[mkLit(i, true )].clear();
        }
        watches[mkLit(i, false)].clear();
        watches[mkLit(i, true )].clear();
    }
    learnts_literals = 0;
    clauses_literals = 0;
    tot_lbds = 0;
}

void Solver::removeClause(CRef cr) {
    Clause& c = ca[cr];
    detachClause(cr);
    // Don't leave pointers to free'd memory!
    if (locked(c)) vardata[var(c[0])].reason = CRef_Undef;
    c.mark(1);
    ca.free(cr);
}

// added by nabesima
void Solver::removeClauseNoDetach(CRef cr) {
    Clause& c = ca[cr];
    // Don't leave pointers to free'd memory!
    if (locked(c)) vardata[var(c[0])].reason = CRef_Undef;
    c.mark(1);
    ca.free(cr);
}

bool Solver::satisfied(const Clause& c) const {
    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) == l_True)
            return true;
    return false; }


// Revert to the state at given level (keeping all assignment at 'level' but not beyond).
//
void Solver::cancelUntil(int level) {
    if (decisionLevel() > level){
        for (int c = trail.size()-1; c >= trail_lim[level]; c--){
            Var      x  = var(trail[c]);
            assigns [x] = l_Undef;
            if (phase_saving > 1 || (phase_saving == 1 && c > trail_lim.last()))
                polarity[x] = sign(trail[c]);
            insertVarOrder(x);
        }
        qhead = trail_lim[level];
        trail.shrink(trail.size() - trail_lim[level]);
        trail_lim.shrink(trail_lim.size() - level);
    } }


//=================================================================================================
// Major methods:


Lit Solver::pickBranchLit()
{
    Var next = var_Undef;

    // Random decision:
    if (drand(random_seed) < random_var_freq && !order_heap.empty()){
        next = order_heap[irand(random_seed,order_heap.size())];
        if (value(next) == l_Undef && decision[next])
            rnd_decisions++; }

    // Activity based decision:
    while (next == var_Undef || value(next) != l_Undef || !decision[next]) {
        if (order_heap.empty()){
            next = var_Undef;
            break;
        }else
            next = order_heap.removeMin();
    }

    return next == var_Undef ? lit_Undef : mkLit(next, rnd_pol ? drand(random_seed) < 0.5 : polarity[next]);
}


/*_________________________________________________________________________________________________
|
|  analyze : (confl : Clause*) (out_learnt : vec<Lit>&) (out_btlevel : int&)  ->  [void]
|
|  Description:
|    Analyze conflict and produce a reason clause.
|
|    Pre-conditions:
|      * 'out_learnt' is assumed to be cleared.
|      * Current decision level must be greater than root level.
|
|    Post-conditions:
|      * 'out_learnt[0]' is the asserting literal at level 'out_btlevel'.
|      * If out_learnt.size() > 1 then 'out_learnt[1]' has the greatest decision level of the
|        rest of literals. There may be others from the same level though.
|
|________________________________________________________________________________________________@*/
// modified by nabesima
//void Solver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel)
void Solver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel, int& lbd)
{
    int pathC = 0;
    Lit p     = lit_Undef;

    // added by nabesima
    int reason_nodes = 0;
    if (lbd_act_bumping) implied_by_learnts.clear();

    // Generate conflict clause:
    //
    out_learnt.push();      // (leave room for the asserting literal)
    int index   = trail.size() - 1;

    do{
        assert(confl != CRef_Undef); // (otherwise should be UIP)
        Clause& c = ca[confl];

        // The first literal must be true in binary clauses.
        if (bin_propagation && p != lit_Undef && c.size() == 2 && value(c[0]) == l_False) {
          assert(value(c[1]) == l_True);
          assert(p == c[1]);
          c[1] = c[0];
          c[0] = p;
        }

        if (c.learnt())
            claBumpActivity(c);

        // added by nabesima
        if (lbd_update_mode == LUP_ANALYSIS && c.learnt() && c.lbd() > 2) {
            int lbd = (lbd_type != LM_DYNAMIC_SIZE_K) ? getLBD(c) : dynamic_size + decisionLevel();
            if (lbd_type == LM_PSEUDO_LBD) lbd++;
            // Update?
            if (lbd <= c.lbd()) {
                tot_lbds = tot_lbds - c.lbd() + lbd;
                c.delta_lbd(c.delta_lbd() + c.lbd() - lbd);
                c.lbd(lbd);
                if(lbd <= frozen_lbd)
                    c.ttl(1);
            }
        }

        // added by nabesima
        Lit pre = lazy_simp ? premise_lit[toInt(~c[0])] : lit_Undef;
        // modified by nabesima
        //for (int j = (p == lit_Undef) ? 0 : 1; j < c.size(); j++){
        for (int j = (p == lit_Undef) ? 0 : 1; j < c.size(); ){
            Lit q = c[j];

            // added by nabesima
            if (lazy_uw_lit_elim && j > 1 &&
                    (premise_lit[toInt(~q)] == ~c[0] || premise_lit[toInt(c[0])] == q ||
                     premise_lit[toInt(~q)] == ~c[1] || premise_lit[toInt(c[1])] == q )) {
                c[j] = c.last();
                c.shrink(1);
                c.learnt() ? lazy_rm_uw_lrn_lits++ : lazy_rm_uw_cla_lits++;
                continue;
            }
            if (bin_propagation && bin_uw_lit_elim && j > 1 &&
                    (bin_premise[toInt(~q)] == ~c[0] || bin_premise[toInt(c[0])] == q ||
                     bin_premise[toInt(~q)] == ~c[1] || bin_premise[toInt(c[1])] == q )) {
                c[j] = c.last();
                c.shrink(1);
                c.learnt() ? bin_rm_uw_lrn_lits++ : bin_rm_uw_cla_lits++;
                continue;
            }

            // added by nabesima
            if (lazy_cla_probing && pre != lit_Undef)
                if (pre != premise_lit[toInt(~c[j])] && (!bin_propagation || pre != bin_premise[toInt(~c[j])]))
                    pre = lit_Undef;

            if (!seen[var(q)] && level(var(q)) > 0){
                // modified by nabesima
                //varBumpActivity(var(q));
                seen[var(q)] = 1;
                if (level(var(q)) >= decisionLevel()) {
                    pathC++;
                    // added by nabesima
                    reason_nodes++;
                    varBumpActivity(var(q), var_inc * white_act_weight);
                    if(lbd_act_bumping && reason(var(q)) != CRef_Undef && ca[reason(var(q))].learnt())
                        implied_by_learnts.push(q);
                }
                else {
                    out_learnt.push(q);
                    // added by nabesima
                    varBumpActivity(var(q), var_inc * black_act_weight);
                }
            }
            // added by nabesima
            j++;
        }

        // added by nabesima
        if (lazy_cla_probing && pre != lit_Undef)
            addProbedLit(~pre, PRB_CLAUSE);

        // Select next clause to look at:
        while (!seen[var(trail[index--])]);
        p     = trail[index+1];
        confl = reason(var(p));
        seen[var(p)] = 0;
        pathC--;

    }while (pathC > 0);
    out_learnt[0] = ~p;

    // added by nabesima
    if (uip_bumping) varBumpActivity(var(p));

    // Simplify conflict clause:
    //
    int i, j;
    out_learnt.copyTo(analyze_toclear);
    if (ccmin_mode == 2){
        uint32_t abstract_level = 0;
        for (i = 1; i < out_learnt.size(); i++)
            abstract_level |= abstractLevel(var(out_learnt[i])); // (maintain an abstraction of levels involved in conflict)

        for (i = j = 1; i < out_learnt.size(); i++)
            if (reason(var(out_learnt[i])) == CRef_Undef || !litRedundant(out_learnt[i], abstract_level))
                out_learnt[j++] = out_learnt[i];

    }else if (ccmin_mode == 1){
        for (i = j = 1; i < out_learnt.size(); i++){
            Var x = var(out_learnt[i]);

            if (reason(x) == CRef_Undef)
                out_learnt[j++] = out_learnt[i];
            else{
                Clause& c = ca[reason(var(out_learnt[i]))];
                for (int k = 1; k < c.size(); k++)
                    if (!seen[var(c[k])] && level(var(c[k])) > 0){
                        out_learnt[j++] = out_learnt[i];
                        break; }
            }
        }
    }else
        i = j = out_learnt.size();

    max_literals += out_learnt.size();
    out_learnt.shrink(i - j);
    tot_literals += out_learnt.size();

    // added by nabesima
    minimizeByBins(out_learnt);
    // added by nabesima
    stat_reasons.push(reason_nodes);

    // Find correct backtrack level:
    //
    if (out_learnt.size() == 1)
        out_btlevel = 0;
    else{
        int max_i = 1;
        // Find the first literal assigned at the next-highest level:
        for (int i = 2; i < out_learnt.size(); i++)
            if (level(var(out_learnt[i])) > level(var(out_learnt[max_i])))
                max_i = i;
        // Swap-in this literal at index 1:
        Lit p             = out_learnt[max_i];
        out_learnt[max_i] = out_learnt[1];
        out_learnt[1]     = p;
        out_btlevel       = level(var(p));
    }

    // added by nabesima
    lbd = (lbd_type != LM_DYNAMIC_SIZE_K) ? getLBD(out_learnt) : getDynamicSize(out_learnt);
    if (lbd_act_bumping && implied_by_learnts.size() > 0) {
        for(int i=0; i < implied_by_learnts.size(); i++) {
            Var v = var(implied_by_learnts[i]);
            int v_lbd = ca[reason(v)].lbd();
            bool bump = false;
            if (lbd_act_bumping == LAB_SMALL_SRC) {
                if (v_lbd < lbd * lbd_act_rate) bump = true;
            }
            else if (lbd_act_bumping == LAB_LARGE_SRC) {
                if (v_lbd * lbd_act_rate > lbd) bump = true;
            }
            else if (lbd_act_bumping == LAB_GOOD_SRC) {
                if (v_lbd < ((double)tot_lbds / nLearnts()) * lbd_act_rate) bump = true;
            }
            if (bump) {
                varBumpActivity(v, var_inc * lbd_act_weight);
                lbd_act_bumps++;
            }
        }
        prop_lits_by_learnts += implied_by_learnts.size();
        implied_by_learnts.clear();
    }

    for (int j = 0; j < analyze_toclear.size(); j++) seen[var(analyze_toclear[j])] = 0;    // ('seen[]' is now cleared)
}


// Check if 'p' can be removed. 'abstract_levels' is used to abort early if the algorithm is
// visiting literals at levels that cannot be removed later.
// seen[v] == 1 if a var is contained in out_learnt and not uip. added by nabesima
bool Solver::litRedundant(Lit p, uint32_t abstract_levels)
{
    analyze_stack.clear(); analyze_stack.push(p);
    int top = analyze_toclear.size();
    while (analyze_stack.size() > 0){
        assert(reason(var(analyze_stack.last())) != CRef_Undef);
        Clause& c = ca[reason(var(analyze_stack.last()))]; analyze_stack.pop();

        // The first literal must be true in binary clauses.
        if (bin_propagation && c.size() == 2 && value(c[0]) == l_False) {
          assert(value(c[1]) == l_True);
          Lit tmp = c[0];
          c[0] =  c[1], c[1] = tmp;
        }

        for (int i = 1; i < c.size(); i++){
            Lit p  = c[i];
            if (!seen[var(p)] && level(var(p)) > 0){
                if (reason(var(p)) != CRef_Undef && (abstractLevel(var(p)) & abstract_levels) != 0){
                    seen[var(p)] = 1;
                    analyze_stack.push(p);
                    analyze_toclear.push(p);
                }else{
                    for (int j = top; j < analyze_toclear.size(); j++)
                        seen[var(analyze_toclear[j])] = 0;
                    analyze_toclear.shrink(analyze_toclear.size() - top);
                    return false;
                }
            }
        }
    }

    return true;
}

// added by nabesima
void Solver::minimizeByBins(vec<Lit>& ps) {
    if (ps.size() > lrn_min_size) return;
    double cpu_time = cpuTime();
    incTouchComps();
    for (int i=0; i < ps.size(); i++)
        touch_time[toInt(ps[i])] = touch_comps;
    assert(bss_reached_toclear.size() == 0);

    if (bin_propagation && bin_lrn_min)
        minimizeByBins(ps, bin_premise, bin_lrn_min == MIN_DEEP);
    if (lazy_lrn_min)
        minimizeByBins(ps, premise_lit, lazy_lrn_min == MIN_DEEP);

    for (int k=0; k < bss_reached_toclear.size(); k++) {
        Lit p = bss_reached_toclear[k];
        bss_reached[toInt(p)] = 0;
        if (lrn_min_var_bump && touch_time[toInt(~p)] == touch_comps)
            varBumpActivity(var(p));
    }
    bss_reached_toclear.clear();
    bin_min_time += cpuTime() - cpu_time;
}
void Solver::minimizeByBins(vec<Lit>& ps, vec<Lit>& premise, bool deep) {
    int i, j;
    for (i = j = 1; i < ps.size(); i++) {
        Lit q = ~ps[i];
        if (bss_reached[toInt(q)]) {
            ps[j++] = ps[i];
            continue;
        }
        assert(bss_seen_toclear.size() == 0);
        bool redundant = false;
        while (true) {
            Lit p = premise[toInt(q)];    // p -> q
            if (value(p) != l_True) {     // Fail. there is no path to a literal in ps.
                if (value(p) == l_False && bss_seen[toInt(~p)]) {
                    // ~p should be true to prevent a conflict.
                    for (int k=0; k < bss_seen_toclear.size(); k++) {
                        Lit r = bss_seen_toclear[k];
                        bss_reached[toInt(r)] = 1;
                        bss_reached_toclear.push(r);
                        if (r == ~p) break;
                    }
                    redundant = true;
                    addProbedLit(~p, PRB_FAILED_LIT);
                }
                break;
            }
            if (p == ~ps[i]) {                // Self-loop to ps[i]
                if (lazy_eqv_probing && p != q) {
                    for (int k = 0; k < bss_seen_toclear.size(); k++)
                        putEquivLit(p, bss_seen_toclear[k]);
                }
                break;
            }
            if (bss_seen[toInt(p)]) {         // Loop to an intermediate node
                if (lazy_eqv_probing && p != q) {
                    int k = bss_seen_toclear.size() - 1;
                    while (bss_seen_toclear[k] != p) {
                        putEquivLit(p, bss_seen_toclear[k]);
                        k--;
                        assert(k >= 0);
                    }
                }
                break;
            }
            if (touch_time[toInt(~p)] == touch_comps || bss_reached[toInt(p)]) {
                for (int k=0; k < bss_seen_toclear.size(); k++) {
                    Lit r = bss_seen_toclear[k];
                    bss_reached[toInt(r)] = 1;
                    bss_reached_toclear.push(r);
                }
                redundant = true;
                if (!bss_reached[toInt(p)]) {
                    bss_reached[toInt(p)] = 1;      // to forbid removing the literal ~q in ps.
                    bss_reached_toclear.push(p);
                }
                break;
            }
            bss_seen[toInt(p)] = 1;
            bss_seen_toclear.push(p);
            q = p;

            if (!deep) break;
        }
        if (!redundant)
            ps[j++] = ps[i];
        for (int k=0; k < bss_seen_toclear.size(); k++)
            bss_seen[toInt(bss_seen_toclear[k])] = 0;
        bss_seen_toclear.clear();
    }

    ps.shrink(i - j);
    tot_literals -= i - j;
    rm_lrn_lits  += i - j;
}

bool Solver::probePremises(vec<Lit>& premise) {

    // Assigns path IDs.
    int pids = 0;
    vec<int> pid;
    pid.growTo(nVars() * 2);

    vec<Lit> stack;
    for (int i=0; i < nVars() * 2; i++) {
        Lit l = toLit(i);
        assert(pid[toInt(l)] >= 0);
        if (pid[toInt(l)] > 0)
            continue;

        stack.clear();
        stack.push(l);
        while (true) {
            Lit q = stack.last();
            Lit p = premise[toInt(q)];    // p -> q
            if (p == q) {
                // Assign a new ID to every element in stack.
                pids++;
                for (int j=0; j < stack.size(); j++)
                    pid[toInt(stack[j])] = pids;
                break;
            }
            if (pid[toInt(p)] > 0) {
                // Assigns the ID of p to every element in stack.
                for (int j=0; j < stack.size(); j++)
                    pid[toInt(stack[j])] = pid[toInt(p)];
                break;
            }
            else if (pid[toInt(p)] < 0) {
                // ループ発見 p -> ... -> p
                bool registered = true;
                {
                    Lit t = premise_lit[toInt(p)];
                    while (t != p) {
                        if (lit2eq_lits[toInt(p)] != lit2eq_lits[toInt(t)])
                            registered = false;
                        putEquivLit(p, t);
                        t = premise_lit[toInt(t)];
                    }
                }

                if (!registered) {
                    Lit t = premise_lit[toInt(p)];
                    printf("LOOP: "); printLit(p); printf(" ");
                    while (t != p) {
                        printLit(t); printf(" ");
                        t = premise_lit[toInt(t)];
                    }
                    printLit(p); printf("\n");
                }
                // Assign a new ID to every element in stack.
                pids++;
                for (int j=0; j < stack.size(); j++)
                    pid[toInt(stack[j])] = pids;
                break;
            }
            else
                pid[toInt(p)] = -1;    // -1 means the current path.
            stack.push(p);
        }
    }

    printf("Num of paths = %d / %d\n", pids, nVars() * 2);

    for (int i=0; i < nVars(); i++) {
        Lit pos = mkLit(i, false);
        Lit neg = ~pos;
        assert(pid[toInt(pos)] > 0 && pid[toInt(neg)] > 0);
        if (pid[toInt(pos)] == pid[toInt(neg)]) {
            Lit p = premise_lit[toInt(pos)];
            Lit q = premise_lit[toInt(neg)];
            while (p != q) {
                Lit pp = premise_lit[toInt(p)];
                Lit qq = premise_lit[toInt(q)];
                if (pp == q) { p = pp; break; }
                if (p == qq) { q = qq; break; }
                p = pp;
                q = qq;
            }
            if (value(p) == l_True)
                return false;
            if (value(p) == l_Undef) {
                printLit(pos); printf(" and "); printLit(neg);
                printf(" is one the same path from ");
                printLit(p); printf("\n");
                addProbedLit(~p, PRB_FAILED_LIT);
            }
        }
    }

    return true;
}

/*_________________________________________________________________________________________________
|
|  analyzeFinal : (p : Lit)  ->  [void]
|
|  Description:
|    Specialized analysis procedure to express the final conflict in terms of assumptions.
|    Calculates the (possibly empty) set of assumptions that led to the assignment of 'p', and
|    stores the result in 'out_conflict'.
|________________________________________________________________________________________________@*/
void Solver::analyzeFinal(Lit p, vec<Lit>& out_conflict)
{
    out_conflict.clear();
    out_conflict.push(p);

    if (decisionLevel() == 0)
        return;

    seen[var(p)] = 1;

    for (int i = trail.size()-1; i >= trail_lim[0]; i--){
        Var x = var(trail[i]);
        if (seen[x]){
            if (reason(x) == CRef_Undef){
                assert(level(x) > 0);
                out_conflict.push(~trail[i]);
            }else{
                Clause& c = ca[reason(x)];
                if (bin_propagation && c.size() == 2 && value(c[0]) == l_False) {
                  assert(value(c[1]) == l_True);
                  Lit tmp = c[0];
                  c[0] =  c[1], c[1] = tmp;
                }
                for (int j = 1; j < c.size(); j++)
                    if (level(var(c[j])) > 0)
                        seen[var(c[j])] = 1;
            }
            seen[x] = 0;
        }
    }

    seen[var(p)] = 0;
}


// modified by nabesima
//void Solver::uncheckedEnqueue(Lit p, CRef from)
void Solver::uncheckedEnqueue(Lit p, CRef from, Lit premise)
{
    assert(value(p) == l_Undef);
    assigns[var(p)] = lbool(!sign(p));
    vardata[var(p)] = mkVarData(from, decisionLevel(), premise);
    trail.push_(p);
}


/*_________________________________________________________________________________________________
|
|  propagate : [void]  ->  [Clause*]
|
|  Description:
|    Propagates all enqueued facts. If a conflict arises, the conflicting clause is returned,
|    otherwise CRef_Undef.
|
|    Post-conditions:
|      * the propagation queue is empty, even if there was a conflict.
|________________________________________________________________________________________________@*/
CRef Solver::propagate()
{
    CRef  confl        = CRef_Undef;
    int   num_props    = 0;
    Lit   dec_lit      = decisionLevel() > 0 ? trail[trail_lim.last()] : lit_Undef;    // added by nabesima
    bool  do_lazy_simp = lazy_simp && (starts % lazy_interval == 0);                   // added by nabesima

    watches.cleanAll();
    // added by nabesima
    if (bin_propagation ) bin_watches.cleanAll();
    if (lazy_bin_add    ) bin_check_lits.clear();

REPROPAGATE:    // added by nabsima

    while (qhead < trail.size()){
        Lit p = trail[qhead++];     // 'p' is enqueued fact to propagate.

        // added by nabesima
        Lit far_dom = lit_Undef;
        if (do_lazy_simp) {
            if (p == dec_lit)
                far_dom = p;
            else if (lazy_old_dom || level(var(premise_lit[toInt(p)])) == decisionLevel())
                far_dom = premise_lit[toInt(p)];
        }
//        Lit far_dom = p;
//        if (lazy_simp) {
//            if (p == dec_lit || premise_lit[toInt(p)] == dec_lit || (dec_lit != lit_Undef && premise_lit[toInt(~dec_lit)] == ~p))
//                far_dom = dec_lit;
//        }
        // First, Propagate binary clauses
        if (bin_propagation) {
            vec<Watcher>& ws = bin_watches[p];
            for (int i=0; i < ws.size(); i++) {
              Lit q = ws[i].blocker;
              if (value(q) == l_False)
                return ws[i].cref;
              if (value(q) == l_Undef) {
                uncheckedEnqueue(q, ws[i].cref, far_dom);
                if (do_lazy_simp && far_dom != lit_Undef)
                    lazyProbing(q, far_dom);
                if (do_lazy_simp && lazy_bin_add && value(premise_lit[toInt(~q)]) != l_True)
                    bin_check_lits.push(q);
              }
              if (bin_lrn_min) // Now, q is true.
                  bin_premise[toInt(q)] = p;    // p -> q = -p v q
//              if (lazy_bin_probing && premise_lit[toInt(p)] == premise_lit[toInt(~q)] && !(value(~premise_lit[toInt(p)]) == l_True && level(var(~premise_lit[toInt(p)])) == 0)) {
//                  printClause(ca[ws[i].cref]);
//                  printf("p = "); printLit(p); printf("\n");
//                  printf("q = "); printLit(q); printf("\n");
//                  printf("pre of  p = "); printLit(premise_lit[toInt( p)]); printf("\n");
//                  printf("pre of -q = "); printLit(premise_lit[toInt(~q)]); printf("\n");
//                  probed_lits.push(PrbLit(~premise_lit[toInt(p)], PRB_BIN_CLAUSE));
//              }
            }
        }

        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        for (i = j = (Watcher*)ws, end = i + ws.size();  i != end;){
            // Try to avoid inspecting the clause:
            Lit blocker = i->blocker;
            if (value(blocker) == l_True){
                *j++ = *i++; continue; }

            // Make sure the false literal is data[1]:
            CRef     cr        = i->cref;
            Clause&  c         = ca[cr];
            Lit      false_lit = ~p;
            if (c[0] == false_lit)
                c[0] = c[1], c[1] = false_lit;
            assert(c[1] == false_lit);
            i++;

            // If 0th watch is true, then clause is already satisfied.
            Lit     first = c[0];
            Watcher w     = Watcher(cr, first);
            if (first != blocker && value(first) == l_True) {
                *j++ = w; continue; }

            // Look for new watch:
            // modified by nabesima
            //for (int k = 2; k < c.size(); k++)
            //    if (value(c[k]) != l_False){
            //        c[1] = c[k]; c[k] = false_lit;
            //        watches[~c[1]].push(w);
            //        goto NextClause; }
            for (int k = 2; k < c.size(); ) {
                Lit ck = c[k];
                if (value(ck) != l_False) {
                    c[1] = ck; c[k] = false_lit;
                    // modified by nabesima
                    //watches[~c[1]].push(w);
                    watches[~c[1]].push(blocker_mode == BLK_MINISAT ? w : Watcher(cr, c[1]));
                    goto NextClause;
                }
                // added by nabesima
                // Checks whther each falsified literal has the same dominator or not.
                if (do_lazy_simp && far_dom != lit_Undef && level(var(ck)) != 0 && ~ck != far_dom && premise(var(ck)) != far_dom)
                    far_dom = lit_Undef;
                k++;
            }

            // Did not find watch -- clause is unit under assignment:
            *j++ = w;
            if (value(first) == l_False){
                confl = cr;
                qhead = trail.size();
                // Copy the remaining watches:
                while (i < end)
                    *j++ = *i++;
            } else {
                // modified by nabesima
                //uncheckedEnqueue(first, cr);
                uncheckedEnqueue(first, cr, far_dom);

                // added by nabesima
                if (do_lazy_simp && far_dom != lit_Undef)
                    lazyProbing(first, far_dom);
                if (do_lazy_simp && lazy_bin_add && value(premise_lit[toInt(~first)]) != l_True)
                    bin_check_lits.push(first);
            }

            // added by nabesima
            if (lbd_update_mode == LUP_PROPAGATION && c.learnt() && c.lbd() > 2) {
                int lbd = (lbd_type != LM_DYNAMIC_SIZE_K) ? getLBD(c) : dynamic_size + decisionLevel();
                if (lbd_type == LM_PSEUDO_LBD) lbd++;
                // Update?
                if (lbd <= c.lbd()) {
                    tot_lbds = tot_lbds - c.lbd() + lbd;
                    c.delta_lbd(c.delta_lbd() + c.lbd() - lbd);
                    c.lbd(lbd);
                    if(lbd <= frozen_lbd)
                        c.ttl(1);
                }
            }

        NextClause:;
        }
        ws.shrink(i - j);
    }
    propagations += num_props;
    simpDB_props -= num_props;

    // added by nabesima
    if (confl == CRef_Undef && lazy_bin_add) {
        for (int i=0; i < bin_check_lits.size(); i++) {
            Lit   p = bin_check_lits[i];
            Lit   q = ~premise_lit[toInt(~p)];    // ~q -> -p  =  p -> q
            lbool t = value(q);
            if (t != l_True) {
                // Make a learnt clause: p -> q
                add_tmp.clear(); add_tmp.push(q); add_tmp.push(~p);
                CRef cr = ca.alloc(add_tmp, true);
                learnts.push(cr);
                ca[cr].lbd(2);
                attachClause(cr);
                if (lazy_bin_decay) varDecayActivity();
                lazy_added_bins++;
                if (t == l_Undef)
                    uncheckedEnqueue(q, cr);
                else {
                    confl = cr;
                    break;
                }
            }
        }
        bin_check_lits.clear();
        if (confl == CRef_Undef && qhead < trail.size())
            goto REPROPAGATE;
    }

    return confl;
}

// added by nabesima
inline /*__attribute__((always_inline))*/ void Solver::lazyProbing(Lit p, Lit new_dom) {
    assert(new_dom != lit_Undef);

    // Use the representative one if it exists.
    int id = lit2eq_lits[toInt(new_dom)];
    if (id != NO_EQ_LITS)
        new_dom = id > 0 ? eq_lits[id].delegate() : ~eq_lits[-id].delegate();

    Lit old_dom = premise_lit[toInt(p)];
    if (old_dom == new_dom)
        if (!bin_propagation || new_dom == (old_dom = bin_premise[toInt(p)]))
            return;
    Lit opp_dom = premise_lit[toInt(~p)];

    //printf("p = "); printLit(p); printf(", old_dom = "); printLit(old_dom); printf(", new dom = "); printLit(new_dom); printf("\n");

    if (lazy_fls_probing && (opp_dom == new_dom || opp_dom == old_dom))
        addProbedLit(~opp_dom, PRB_FAILED_LIT);
    if (lazy_pol_probing && (old_dom == ~new_dom || premise_lit[toInt(old_dom)] == ~premise_lit[toInt(new_dom)]))
        addProbedLit(p, PRB_POLALITY);
    else if (lazy_bin_probing && (premise_lit[toInt(new_dom)] == ~old_dom || premise_lit[toInt(old_dom)] == ~new_dom))
        addProbedLit(p, PRB_BIN_CLAUSE);
    if (lazy_eqv_probing && opp_dom != ~p) {
        if (~new_dom == opp_dom)
            putEquivLit(p, new_dom);
        if (~old_dom == opp_dom)
            putEquivLit(p, old_dom);
    }

    premise_lit[toInt(p)] = new_dom;
    premise_updates++;
}
// added by nabesima
inline void Solver::putEquivLit(Lit p, Lit q) {
    assert(p != q);

    int p_id = lit2eq_lits[toInt(p)];
    int q_id = lit2eq_lits[toInt(q)];

    if (p_id == NO_EQ_LITS && q_id == NO_EQ_LITS) {
        // Make a new equivalent literal set.
        uint32_t id = eq_lits.size();
        assert(id > 0);
        eq_lits.push();
        eq_lits.last().push(p);
        eq_lits.last().push(q);
        lit2eq_lits[toInt( p)] = +id;
        lit2eq_lits[toInt( q)] = +id;
        lit2eq_lits[toInt(~p)] = -id;
        lit2eq_lits[toInt(~q)] = -id;
        num_probed_lits[PRB_EQV_LIT]++;
    }
    else if (p_id == NO_EQ_LITS) {  // q_id != NO_EQ_LITS
        if (q_id < 0) { q_id = -q_id; p = ~p; q = ~q; }
        EqLits& q_set = eq_lits[q_id];
        assert(q_set.size() > 0);
        q_set.push(p);
        lit2eq_lits[toInt( p)] = +q_id;
        lit2eq_lits[toInt(~p)] = -q_id;
        num_probed_lits[PRB_EQV_LIT]++;
    }
    else if (q_id == NO_EQ_LITS) {  // p_id != NO_EQ_LITS
        if (p_id < 0) { p_id = -p_id; p = ~p; q = ~q; }
        EqLits& p_set = eq_lits[p_id];
        assert(p_set.size() > 0);
        p_set.push(q);
        lit2eq_lits[toInt( q)] = +p_id;
        lit2eq_lits[toInt(~q)] = -p_id;
        num_probed_lits[PRB_EQV_LIT]++;
    }
    else if (p_id == -q_id) {  // Has conflicts?
        conflict_probing = true;
    }
    else if (p_id != q_id) {  // merges two equivalent literal sets.
        if (p_id < 0) { p_id = -p_id; q_id = -q_id; p = ~p; q = ~q; }
        if (q_id > 0) {
            EqLits& p_set = eq_lits[p_id];
            EqLits& q_set = eq_lits[q_id];
            // p_set += q_set
            for (int i=0; i < q_set.size(); i++) {
                lit2eq_lits[toInt( q_set[i])] = +p_id;
                lit2eq_lits[toInt(~q_set[i])] = -p_id;
                p_set.push(q_set[i]);
            }
            q_set.eliminate();
            //printf("merged p_set size = %d\n", p_set.size());
        }
        else {
            EqLits& p_set = eq_lits[p_id];
            EqLits& q_set = eq_lits[-q_id];
            // p_set += q_set
            for (int i=0; i < q_set.size(); i++) {
                lit2eq_lits[toInt( q_set[i])] = -p_id;
                lit2eq_lits[toInt(~q_set[i])] = +p_id;
                p_set.push(~q_set[i]);
            }
            q_set.eliminate();
            //printf("merged p_set size = %d\n", p_set.size());
        }
        num_probed_lits[PRB_EQV_LIT]++;
    }
    else
        return;  // already registered.
}

/*_________________________________________________________________________________________________
|
|  reduceDB : ()  ->  [void]
|
|  Description:
|    Remove half of the learnt clauses, minus the clauses locked by the current assignment. Locked
|    clauses are clauses that are reason to some assignment. Binary clauses are never removed.
|________________________________________________________________________________________________@*/
struct reduceDB_lt {
    ClauseAllocator& ca;
    reduceDB_lt(ClauseAllocator& ca_) : ca(ca_) {}
    bool operator () (CRef x, CRef y) {
        return ca[x].size() > 2 && (ca[y].size() == 2 || ca[x].activity() < ca[y].activity()); }
};
// added by nabesima
struct reduceDB_lt_by_lbd {
    ClauseAllocator& ca;
    reduceDB_lt_by_lbd(ClauseAllocator& ca_) : ca(ca_) {}
    bool operator () (CRef x, CRef y) {
        // Subsumption criteria
        if (ca[x].subsumed() && !ca[y].subsumed()) return true;
        if (ca[y].subsumed() && !ca[x].subsumed()) return false;
        // First criteria
        if (ca[x].size() >  2 && ca[y].size() == 2) return true;
        if (ca[y].size() >  2 && ca[x].size() == 2) return false;
        if (ca[x].size() == 2 && ca[y].size() == 2) return false;
        // Second one
        if (ca[x].lbd() > ca[y].lbd()) return true;
        if (ca[x].lbd() < ca[y].lbd()) return false;
        // Last one
        return ca[x].activity() < ca[y].activity();
    }
};
void Solver::reduceDB()
{
    double cpu_time = cpuTime();
    int     i = 0, j = 0;

    // added by nabesima
    reduce_dbs++;

    // modified by nabesima
    if (reduce_db == RDB_LRU) {    // minisat version
        double  extra_lim = cla_inc / learnts.size();    // Remove any clause below this activity

        sort(learnts, reduceDB_lt(ca));

        int limit = learnts.size() * (1 - reduction_rate);

        // Don't delete binary or locked clauses. From the rest, delete clauses from the first half
        // and clauses with activity smaller than 'extra_lim':
        for (i = j = 0; i < learnts.size(); i++){
            Clause& c = ca[learnts[i]];

            if (c.size() > 2 && !locked(c) && (i < limit || c.activity() < extra_lim || c.subsumed())) {
                removeClause(learnts[i]);
                removed_learnts++;
            }
            else
                learnts[j++] = learnts[i];
        }
    }
    else if (reduce_db == RDB_LBD) {    // lbd version

        //printf("median1 = "); printClause(ca[select(learnts, learnts.size() / 2, reduceDB_lt_by_lbd(ca))]);
        sort(learnts, reduceDB_lt_by_lbd(ca));

        // When a lot of "good" clauses exist, hold some of them (glucose 3.0)
        if (ca[learnts[learnts.size() * reduction_rate]].lbd() <= 3)
            max_learnts += inc_lrn_limit;

        int limit = learnts.size() * (1 - reduction_rate);

        for (i = j = 0; i < learnts.size(); i++) {
            Clause& c = ca[learnts[i]];

            if (c.lbd() > max_lbd && c.size() > 2 && !locked(c) && ((i < limit && c.ttl() == 0 && (double)c.delta_lbd() / (c.delta_lbd() + c.lbd()) <= lbd_dec_rate) || c.subsumed())) {
                removeClause(learnts[i]);
                removed_learnts++;
            }
            else {
                if (c.ttl() > 0) {
                    c.ttl(c.ttl() - 1);
                    limit++;    //we keep c, so we can delete an other clause (glucose 3.0)
                }
                c.delta_lbd( c.delta_lbd() * dlbd_decay );
                learnts[j++] = learnts[i];
            }
        }
    }
    else
        assert(false);

    learnts.shrink(i - j);
    checkGarbage();
    reduce_time += cpuTime() - cpu_time;
}


void Solver::removeSatisfied(vec<CRef>& cs)
{
    // modified by nabesima
//    int i, j;
//    for (i = j = 0; i < cs.size(); i++){
//        Clause& c = ca[cs[i]];
//        // modified by nabesima
//        if (satisfied(c))
//            removeClause(cs[i]);
//        else
//            cs[j++] = cs[i];
//    }
//    cs.shrink(i - j);

    // added by nabesima
    assert(decisionLevel() == 0);
    int i, j;
    for (i = j = 0; i < cs.size(); i++){
        Clause& c = ca[cs[i]];
        int  k, l;
        for (k = l = 0; k < c.size(); k++) {
            Lit ck = c[k];
            if (value(ck) == l_True)
                break;
            // Lazy unwatched literal elimination
            if (lazy_uw_lit_elim && l >= 2 &&
                (premise_lit[toInt(~ck)] == ~c[0] || premise_lit[toInt(c[0])] == ck ||
                 premise_lit[toInt(~ck)] == ~c[1] || premise_lit[toInt(c[1])] == ck )) {
                c.learnt() ? lazy_rm_uw_lrn_lits++ : lazy_rm_uw_cla_lits++;
                continue;
            }
            if (bin_propagation && bin_uw_lit_elim && l >= 2 &&
                (bin_premise[toInt(~ck)] == ~c[0] || bin_premise[toInt(c[0])] == ck ||
                 bin_premise[toInt(~ck)] == ~c[1] || bin_premise[toInt(c[1])] == ck )) {
                c.learnt() ? bin_rm_uw_lrn_lits++ : bin_rm_uw_cla_lits++;
                continue;
            }
            c[l++] = ck;
        }
        if (k < c.size())
            removeClause(cs[i]);
        else {
            c.shrink(k - l);
            cs[j++] = cs[i];
        }
    }
    cs.shrink(i - j);
}

// added by nabesima
bool Solver::rewriteClauses() {
    double cpu_time = cpuTime();
    assert(lazy_eqv_probing);
    assert(decisionLevel() == 0);

    if (num_probed_lits[PRB_EQV_LIT] == rewrited_lits || (num_probed_lits[PRB_EQV_LIT] - rewrited_lits < nFreeVars() * rewrite_min_lits || (rewrited_starts != 0 && starts <= rewrited_starts + rewrite_min_starts)))
        return true;

    if (!ok || propagate() != CRef_Undef)
        return ok = false;

    bool no_conf = true;
    detachAllClauses();
    no_conf &= rewriteClauses(clauses, rw_clause_lits, rw_taut_clauses, rw_unit_clauses);
    no_conf &= rewriteClauses(learnts, rw_learnt_lits, rw_taut_learnts, rw_unit_learnts);
    attachAllClauses();

    // Inactivate equivalent variables except for representative ones.
    for (int i=0; i < eq_lits.size(); i++) {
        EqLits& lits = eq_lits[i];
        if (lits.size() == 0) continue;
        Lit p = lits.delegate();
        vec<Lit> pos, neg, undef;
        for (int j=0; j < lits.size(); j++) {
            if (lits[j] != p)
                setDecisionVar(var(lits[j]), false);
            if (value(lits[j]) == l_True) pos.push(lits[j]);
            else if (value(lits[j]) == l_False) neg.push(lits[j]);
            else undef.push(lits[j]);
        }
        assert(decision[var(p)]);
        if (pos.size() > 0 && neg.size() > 0)
            ok = no_conf = false;
        else if (pos.size() > 0 || neg.size() > 0)
            for (int j=0; j < undef.size(); j++)
                uncheckedEnqueue(pos.size() > 0 ? undef[j] : ~undef[j]);
    }

    if (!ok || propagate() != CRef_Undef)
        ok = no_conf = false;

//    // DEBUG
//    incTouchComps();
//    for (int i=0; i < eq_lits.size(); i++) {
//        EqLits& lits = eq_lits[i];
//        int def = 0;
//        int undef = 0;
//        for (int j=0; j < lits.size(); j++) {
//            Lit p = lits[j];
//            if (touch_time[var(p)] != touch_comps)
//                touch_time[var(p)] = touch_comps;
//            else {
//                printLit(p); printf(" is duplicated\n");
//            }
//            if (value(p) == l_Undef) undef++; else def++;
//        }
//        if (def > 0 && undef > 0)
//            printf("EqLits[%d] has def and undef lits\n", i);
//    }
//    for (int i=0; i < nVars(); i++) {
//        Lit pos = mkLit(i, false);
//        Lit neg = ~pos;
//        if (lit2eq_lits[toInt(pos)] != NO_EQ_LITS && touch_time[var(pos)] != touch_comps) {
//            printLit(pos); printf(" does not exist in the equivalent set\n");
//        }
//        if (lit2eq_lits[toInt(neg)] != NO_EQ_LITS && touch_time[var(neg)] != touch_comps) {
//            printLit(pos); printf(" does not exist in the equivalent set\n");
//        }
//    }

    checkGarbage();
    rebuildOrderHeap();

    rewrite_dbs++;
    rewrited_lits   = num_probed_lits[PRB_EQV_LIT];
    rewrited_starts = starts;
    if (verbosity) printProgress('e');
    rewrite_time += cpuTime() - cpu_time;
    return no_conf;
}
// added by nabesima
bool Solver::rewriteClauses(vec<CRef>& cs, uint64_t& rw_lits, uint64_t& tauts, uint64_t& units)
{
    assert(lazy_eqv_probing);

    int i, j;
    for (i = j = 0; i < cs.size(); i++) {
        Clause& c = ca[cs[i]];

        // Locked clauses are used as reasons for propagated literals at DLV 0.
        if (locked(c)) { cs[j++] = cs[i]; continue; }

        incTouchComps();
        int  k, l;
        for (k = l = 0; k < c.size(); k++) {
            Lit from = c[k];
            Lit to   = from;
            int id   = lit2eq_lits[toInt(from)];
            if (id != NO_EQ_LITS)
                to = id > 0 ? eq_lits[id].delegate() : ~eq_lits[-id].delegate();
            assert(to != lit_Undef);

            // Lazy unwatched literal elimination
            if (lazy_uw_lit_elim && l >= 2 &&
                (premise_lit[toInt(~from)] == ~c[0] || premise_lit[toInt(c[0])] == from ||
                 premise_lit[toInt(~from)] == ~c[1] || premise_lit[toInt(c[1])] == from )) {
                c.learnt() ? lazy_rm_uw_lrn_lits++ : lazy_rm_uw_cla_lits++;
                continue;
            }
            if (bin_propagation && bin_uw_lit_elim && l >= 2 &&
                (bin_premise[toInt(~from)] == ~c[0] || bin_premise[toInt(c[0])] == from ||
                 bin_premise[toInt(~from)] == ~c[1] || bin_premise[toInt(c[1])] == from )) {
                c.learnt() ? bin_rm_uw_lrn_lits++ : bin_rm_uw_cla_lits++;
                continue;
            }
            // duplicated literals.
            if (touch_time[toInt(to)] == touch_comps)
                continue;
            // tautological clause.
            if (touch_time[toInt(~to)] == touch_comps) {
                tauts++;
                break;
            }

            if (from != to) rw_lits++;

            c[l++] = to;
            touch_time[toInt(to)] = touch_comps;
        }

        if (k < c.size())
            removeClauseNoDetach(cs[i]);
        else {
            c.shrink(k - l);
            if (c.size() > 1)
                cs[j++] = cs[i];
            else if (c.size() == 0)
                ok = false;
            else {
                assert(c.size() == 1);
                if (value(c[0]) == l_False)
                    ok = false;
                else if (value(c[0]) == l_Undef) {
                    uncheckedEnqueue(c[0]);
                    units++;
                }
                removeClauseNoDetach(cs[i]);
            }
        }
    }
    cs.shrink(i - j);

    return ok;
}

void Solver::rebuildOrderHeap()
{
    vec<Var> vs;
    for (Var v = 0; v < nVars(); v++)
        if (decision[v] && value(v) == l_Undef)
            vs.push(v);
    order_heap.build(vs);
}


/*_________________________________________________________________________________________________
|
|  simplify : [void]  ->  [bool]
|
|  Description:
|    Simplify the clause database according to the current top-level assigment. Currently, the only
|    thing done here is the removal of satisfied clauses, but more things can be put here.
|________________________________________________________________________________________________@*/
bool Solver::simplify()
{
    assert(decisionLevel() == 0);

    if (!ok || propagate() != CRef_Undef)
        return ok = false;

    if (nAssigns() == simpDB_assigns || (simpDB_props > 0))
        return true;

    // Remove satisfied clauses:
    removeSatisfied(learnts);
    if (remove_satisfied)        // Can be turned off.
        removeSatisfied(clauses);
    checkGarbage();
    rebuildOrderHeap();

    simpDB_assigns = nAssigns();
    simpDB_props   = clauses_literals + learnts_literals;   // (shouldn't depend on stats really, but it will do for now)

    return true;
}


/*_________________________________________________________________________________________________
|
|  search : (nof_conflicts : int) (params : const SearchParams&)  ->  [lbool]
|
|  Description:
|    Search for a model the specified number of conflicts.
|    NOTE! Use negative value for 'nof_conflicts' indicate infinity.
|
|  Output:
|    'l_True' if a partial assigment that is consistent with respect to the clauseset is found. If
|    all variables are decision variables, this means that the clause set is satisfiable. 'l_False'
|    if the clause set is unsatisfiable. 'l_Undef' if the bound on number of conflicts is reached.
|________________________________________________________________________________________________@*/
lbool Solver::search(int nof_conflicts)
{
    assert(ok);
    int       backtrack_level;
    int	      lbd;                                     // added by nabesima
    int       conflictC = 0;
    int       decisionC = 0;                           // added by nabesima
    int64_t	  old_propC = 0;                           // added by nabesima
    int64_t	  cur_propC = 0;                           // added by nabesima
    int       dec_restart_limit = dec_queue_size;    // added by nabesima
    int       num_succ_confs = 0;                      // added by nabesima
    vec<CRef> sc_learnts;                              // added by nabesima
    vec<Lit>  learnt_clause;
    starts++;

    // added by nabesima
    local_LBDs.clear();

    // added by nabesima
    if (lazy_simp)
        if (!propProbedLits() || conflict_probing)
            return l_False;

    // added by nabesima
    if (lazy_eqv_probing)
        if (!rewriteClauses())
            return l_False;

    // TEST
    if (false && starts % 100 == 0)
        if (!probePremises(premise_lit))
            return l_False;

    for (;;){

        old_propC  = propagations;                // added by nabesima
        CRef confl = propagate();
        cur_propC += propagations - old_propC;    // added by nabesima

        if (confl != CRef_Undef){
            // CONFLICT
            conflicts++; conflictC++;

            // glucose3's heuristics. Effective for SAT!
            if (!init_fluc_finished) {
                if (conflicts % init_fluc_base == 0)
                    var_decay += 0.01;
                if (init_fluc_period <= conflicts) {
                    init_fluc_finished = true;
                    printProgress('f');
                }
            }

            // added by nabesima
            stat_DLVs.push(decisionLevel());
            stat_trails.push(trail.size());
            local_trails.push(trail.size());
            // added by nabesima
            bool blocked = false;
            if (restart_blocking == RBK_TRAIL && local_trails.ready() && local_LBDs.ready() && trail.size() * blk_restart_rate > local_trails.average())
                blocked = true;
            else if (restart_blocking == RBK_PROPS_CONF && (double)cur_propC / conflictC * blk_restart_rate > (double)propagations / conflicts)
                blocked = true;

            if (blocked) {
                // DEBUG
                //printf("%-12"PRIu64" local trail avg %.3f, local trail %.3f\n", conflicts, local_trails.average(), local_trails.last());
                local_LBDs.wait((int)(lbd_queue_size * blk_restart_weight));
                dec_restart_limit = conflictC + dec_queue_size;
                blocked_restarts++;
                last_blocked = starts;
            }

            if (decisionLevel() == 0) {
                // added by nabesima
                succ_confs.inc(num_succ_confs + 1);  // '1' means a refutation.
                return l_False;
            }

            learnt_clause.clear();
            analyze(confl, learnt_clause, backtrack_level, lbd);

            // added by nabesima
            global_LBDs += lbd;
            local_LBDs.push(lbd);
            learnts_len.inc(learnt_clause.size());
            learnts_lbd.inc(lbd);

            cancelUntil(backtrack_level);

            if (learnt_clause.size() == 1) {
                assert(value(learnt_clause[0]) == l_Undef && backtrack_level == 0);
                uncheckedEnqueue(learnt_clause[0]);
            } else {
                CRef cr = ca.alloc(learnt_clause, true);
                learnts.push(cr);

                // added by nabesima
                assert(lbd > 0);
                ca[cr].lbd(lbd);

                attachClause(cr);
                claBumpActivity(ca[cr]);

                uncheckedEnqueue(learnt_clause[0], cr);

                // added by nabesima
                if (sc_subsump_check && sc_learnts.size() > 0) {
                    Clause& c_new = ca[cr];
                    // Mark every literal in c_new.
                    incScfComps();
                    for (int i=0; i < c_new.size(); i++)
                        scf_time[toInt(c_new[i])] = scf_comps;
                    // Check whether c_new subsumes a clause in sc_learnts.
                    for (int i=0; i < sc_learnts.size(); i++) {
                        if (sc_learnts[i] == CRef_Undef) continue;   // already subsumed
                        Clause& c_old = ca[sc_learnts[i]];
                        // Counts the number of c_new literals in c_old.
                        int num = 0;
                        for (int j=0; j < c_old.size(); j++)
                            if (scf_time[toInt(c_old[j])] == scf_comps)
                                num++;
                        if (num == c_new.size()) {    // if c_new subsumes c_old
                            c_old.subsumed(true);
                            if (c_old.lbd() < c_new.lbd())
                                c_new.lbd(c_old.lbd());
                            sc_learnts[i] = CRef_Undef;
                            sc_subsumed_learnts++;
                        }
                    }
                }
                sc_learnts.push(cr);
            }
            // added by nabesima
            num_succ_confs++;

            varDecayActivity();
            claDecayActivity();

            // modified by nabesima
            //if (--learntsize_adjust_cnt == 0){
            if (!ag_reduce_db && --learntsize_adjust_cnt == 0){
                learntsize_adjust_confl *= learntsize_adjust_inc;
                learntsize_adjust_cnt    = (int)learntsize_adjust_confl;
                max_learnts             *= learntsize_inc;
                // modified by nabesima
//                if (verbosity >= 1) {
//                    printf("| %9d | %7d %8d %8d | %8d %8d %6.0f | %6.3f %% |\n",
//                           (int)conflicts,
//                           (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]), nClauses(), (int)clauses_literals,
//                           (int)max_learnts, nLearnts(), (double)learnts_literals/nLearnts(), progressEstimate()*100);
//                }
            }

            if (conflicts >= next_progress_report) {
                if (verbosity) printProgress();
                progress_report_base *= progress_report_inc;
                next_progress_report += progress_report_base;
            }

        }else{
            // NO CONFLICT

            // added by nabesima
            if (num_succ_confs > 0) {
                succ_confs.inc(num_succ_confs);
                num_succ_confs = 0;
            }
            sc_learnts.clear();

            // modified by nabesima
            bool restart = false;
            switch (restart_strategy) {
            case RST_MINISAT: {  // Minisat version
                if (nof_conflicts >= 0 && conflictC >= nof_conflicts)
                    restart = true;
                break;
            }
            // Glucose version
            case RST_LBD: {
                if (local_LBDs.ready() && local_LBDs.average() * lbd_restart_rate > (double)global_LBDs / conflicts)
                    restart = true;
                break;
            }
            case RST_LBD_DEC: {
                if (local_LBDs.ready() && local_LBDs.average() * lbd_restart_rate > (double)global_LBDs / conflicts)
                    restart = true;
                else if (conflictC >= dec_restart_limit && (double)decisionC / conflictC * dec_restart_rate > (double)decisions / conflicts)
                    restart = true;
                break;
            }
            default:
                assert(false);
                /* no break */
            }

            if (restart || !withinBudget()) {
                // Reached bound on number of conflicts:
                progress_estimate = progressEstimate();
                cancelUntil(0);
                // added by nabesima
                if (decaying_restart) varDecayActivity();
                return l_Undef; }

            // Simplify the set of problem clauses:
            if (decisionLevel() == 0 && !simplify())
                return l_False;

            // modified by nabesima
            if (!ag_reduce_db && learnts.size() - nAssigns() >= max_learnts && (!lv0_reduce_db || decisionLevel() == 0)) { // Minisat version
                if (verbosity) printProgress('r');    // added by nabesima
                reduceDB();    // Reduce the set of learnt clauses:
                if (verbosity) printProgress('R');    // added by nabesima
            }
            else if (ag_reduce_db && nLearnts() >= max_learnts && (!lv0_reduce_db || decisionLevel() == 0)) {  // glucose 1.0 version
                if (verbosity) printProgress('r');    // added by nabesima
                max_learnts = nLearnts() * reduction_rate + reduce_db_base + reduce_db_inc * reduce_dbs;
                reduceDB();    // Reduce the set of learnt clauses:
                if (verbosity) printProgress('R');    // added by nabesima
            }

            Lit next = lit_Undef;
            while (decisionLevel() < assumptions.size()){
                // Perform user provided assumption:
                Lit p = assumptions[decisionLevel()];
                if (value(p) == l_True){
                    // Dummy decision level:
                    newDecisionLevel();
                }else if (value(p) == l_False){
                    analyzeFinal(~p, conflict);
                    return l_False;
                }else{
                    next = p;
                    break;
                }
            }

            if (next == lit_Undef){
                // New variable decision:
                decisions++;
                decisionC++;    // added by nabesima
                next = pickBranchLit();

                if (next == lit_Undef)
                    // Model found:
                    return l_True;
            }

            // Increase decision level and enqueue 'next'
            newDecisionLevel();
            uncheckedEnqueue(next);
        }
    }
    // added by nabesima to supress warning message.
    assert(false);
    return l_Undef;
}

// added by nabesima
inline bool Solver::propProbedLits() {
    assert(decisionLevel() == 0);
    if (conflict_probing)
        return false;
    if (probed_lits.size() == 0 || (probed_lits.size() < nFreeVars() * prop_min_lits || (propagated_starts != 0 && starts <= propagated_starts + prop_min_starts)))
        return true;

    for (int i=0; i < probed_lits.size(); i++) {
        PrbLit prb = probed_lits[i];    // PrbLit& is not allowed since probed_lits may enlarge in propagate.
        if (value(prb.p) == l_Undef) {
            uncheckedEnqueue(prb.p);
            if (propagate() != CRef_Undef)
                return false;
        }
        else if (value(prb.p) == l_False)
            return false;

        if (!lazy_simp) continue;

        // Premise chain
        Lit q = premise_lit[toInt(~prb.p)];
        if (value(q) == l_Undef)
            probed_lits.push(PrbLit(~q, PRB_PREMISE_CHAIN));
        else if (value(q) == l_True)
            return false;

        // DEBUG
//        for (int i=0; i < nVars(); i++) {
//            Var v = i;
//            Lit p = mkLit(v, false);
//            if (value(p) == l_Undef && value(premise_lit[toInt(p)]) == l_True) {
//                printf("@ p = "); printLit(p); printf("\n");
//                printf("@ pre of p = "); printLit(premise_lit[toInt(p)]); printf("\n");
//            }
//            p = ~p;
//            if (value(p) == l_Undef && value(premise_lit[toInt(p)]) == l_True) {
//                printf("@ p = "); printLit(p); printf("\n");
//                printf("@ pre of p = "); printLit(premise_lit[toInt(p)]); printf("\n");
//            }
//        }

        // Equivalent literals
        int id = lit2eq_lits[toInt(prb.p)];
        if (id != NO_EQ_LITS) {
            EqLits& lits = eq_lits[abs(id)];
            vec<Lit> undef;
            for (int j=0; j < lits.size(); j++) {
                if (value(lits[j]) == l_True  && id < 0) return false;
                if (value(lits[j]) == l_False && id > 0) return false;
                if (value(lits[j]) == l_Undef) undef.push(id > 0 ? lits[j] : ~lits[j]);
            }
            for (int j=0; j < undef.size(); j++)
                if (value(undef[j]) == l_Undef)
                    probed_lits.push(PrbLit(undef[j], PRB_EQV_LIT));
                else if (value(undef[j]) == l_False)
                    return false;
        }
    }
    probed_lits.clear();

//    varDecayActivity();
//    claDecayActivity();

    propagated_starts = starts;
    if (verbosity) printProgress('p');

    return true;
}

double Solver::progressEstimate() const
{
    double  progress = 0;
    double  F = 1.0 / nVars();

    for (int i = 0; i <= decisionLevel(); i++){
        int beg = i == 0 ? 0 : trail_lim[i - 1];
        int end = i == decisionLevel() ? trail.size() : trail_lim[i];
        progress += pow(F, i) * (end - beg);
    }

    return progress / nVars();
}

/*
  Finite subsequences of the Luby-sequence:

  0: 1
  1: 1 1 2
  2: 1 1 2 1 1 2 4
  3: 1 1 2 1 1 2 4 1 1 2 1 1 2 4 8
  ...


 */

static double luby(double y, int x){

    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for (size = 1, seq = 0; size < x+1; seq++, size = 2*size+1);

    while (size-1 != x){
        size = (size-1)>>1;
        seq--;
        x = x % size;
    }

    return pow(y, seq);
}

// NOTE: assumptions passed in member-variable 'assumptions'.
lbool Solver::solve_()
{
    // added by nabesima
    if (init_vars == 0 && init_clauses == 0) {
        init_vars    = simp_vars    = nVars();
        init_clauses = simp_clauses = nClauses();
        simp_clauses_literals = clauses_literals;
    }

    model.clear();
    conflict.clear();
    if (!ok) return l_False;

    solves++;

    max_learnts               = nClauses() * learntsize_factor;
    learntsize_adjust_confl   = learntsize_adjust_start_confl;
    learntsize_adjust_cnt     = (int)learntsize_adjust_confl;

    // added by nabesima
    if (max_learnts < reduce_db_base) {
        reduce_db_base = (uint64_t)((max_learnts/2 < 5000) ? 5000 : max_learnts/2);
        reduce_db_inc  /= 2;
    }
    if (ag_reduce_db)
        max_learnts = reduce_db_base;
    if (init_fluc)
        var_decay = init_fluc_decay;

    if (verbosity >= 1){
        // modified by nabesima
//        printf("c     ================================[ Search Statistics ]================================\n");
//        printf("c     | Conflicts |     ORIGINAL     |          LEARNT          | Conf    Trail  Progress |\n");
//        printf("c     |           |    Vars  Clauses |    Limit  Clauses Lit/Cl |   DLVs    Size          |\n");
//        printf("c     =====================================================================================\n");
        printProgress();  // added by nabesima
    }

    // Search:
    lbool status        = l_Undef;
    int   curr_restarts = 0;
    while (status == l_Undef){
        double rest_base = luby_restart ? luby(restart_inc, curr_restarts) : pow(restart_inc, curr_restarts);
        uint64_t prev_conflicts = conflicts;    // added by nabesima
        status = search(rest_base * restart_first);
        if (!withinBudget()) break;
        curr_restarts++;

        // added by nabesima
        if (diverse_strategy && init_fluc_finished && starts % diverse_base == 0) {
            var_decay         = dsp_var_decay       [irand(random_seed, dsp_var_decay       .size())];
            phase_saving      = dsp_phase_saving    [irand(random_seed, dsp_phase_saving    .size())];
            blocker_mode      = dsp_blocker         [irand(random_seed, dsp_blocker         .size())];
            lazy_old_dom      = dsp_lazy_old_dom    [irand(random_seed, dsp_lazy_old_dom    .size())];
            lbd_update_mode   = dsp_lbd_update      [irand(random_seed, dsp_lbd_update      .size())];
            uip_bumping       = dsp_uip_bumping     [irand(random_seed, dsp_uip_bumping     .size())];
            restart_strategy  = dsp_restart_strategy[irand(random_seed, dsp_restart_strategy.size())];
            lbd_restart_rate  = dsp_lbd_restart_rate[irand(random_seed, dsp_lbd_restart_rate.size())];
            dec_restart_rate  = dsp_dec_restart_rate[irand(random_seed, dsp_dec_restart_rate.size())];
            num_param_changes++;
            if (verbosity) {
                printProgress('c');
                printf("c\n");
                printf("c  #%-4d  v-decay = %.2f  phase    = %-4d  blocker  = %-4d  old-dom  = %-4d\n", num_param_changes, var_decay, phase_saving, blocker_mode, lazy_old_dom);
                printf("c         restart = %-4d  lbd-rate = %.2f  dec-rate = %.2f  uip-bump = %-4d\n", restart_strategy, lbd_restart_rate, dec_restart_rate, uip_bumping);
                printf("c\n");
            }

        }

        // added by nabesima
        uint64_t confs = conflicts - prev_conflicts;
        if (confs < min_confs) min_confs = confs;
        if (confs > max_confs) max_confs = confs;
    }

    if (verbosity >= 1) {
        printProgress();  // added by nabesima
        // modified by nabesima
        //printf("c     =====================================================================================\n");
        printf("c\n");
    }


    if (status == l_True){
        // added by nabesima
        if (partial_model) makePartialModel();
        // Extend & copy model:
        model.growTo(nVars());

        // modified by nabesima
        //for (int i = 0; i < nVars(); i++) model[i] = value(i);
        if (!lazy_eqv_probing)
            for (int i = 0; i < nVars(); i++) model[i] = value(i);
        else {
            for (int i = 0; i < nVars(); i++) {
                Lit p  = mkLit(i, false);
                int id = lit2eq_lits[toInt(p)];
                if (id == NO_EQ_LITS) {
                    model[i] = value(i);
//                    if (model[i] == l_Undef) {
//                        printf("var %d has no eq lits, but the value is undef.\n", i);
//                    }
                }
                else {
                    Lit q = id > 0 ? eq_lits[id].delegate() : ~eq_lits[-id].delegate();
                    model[i]   = value(q);
//                    assigns[i] = value(q);
//                    if (model[i] == l_Undef) {
//                        printf("val of var %d is undef.\n", i );
//                    }
                }
            }
        }
/*
        // DEBUG
        // added by nabesima
        // Verify a model
        printf("c verifying...\n");
        for (int i=0; i < org_clauses.size(); i++) {
            Clause& c = ca[org_clauses[i]];
            if (!satisfied(c)) {
                printf("UNSATISFIED (1): ");
                printClause(c);
            }
            bool satisfied = false;
            for (int j=0; j < c.size(); j++) {
                Lit p = c[j];
                if ((model[var(p)] ^ sign(p)) == l_True) {
                    satisfied = true;
                    break;
                }
            }
            if (!satisfied) {
                printf("UNSATISFIED (2): ");
                printClause(c);
            }
        }
        printf("c done.\n");
*/
    }else if (status == l_False && conflict.size() == 0)
        ok = false;

    cancelUntil(0);
    return status;
}

//=================================================================================================
// Writing CNF to DIMACS:
//
// FIXME: this needs to be rewritten completely.

static Var mapVar(Var x, vec<Var>& map, Var& max)
{
    if (map.size() <= x || map[x] == -1){
        map.growTo(x+1, -1);
        map[x] = max++;
    }
    return map[x];
}


void Solver::toDimacs(FILE* f, Clause& c, vec<Var>& map, Var& max)
{
    if (satisfied(c)) return;

    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) != l_False)
            fprintf(f, "%s%d ", sign(c[i]) ? "-" : "", mapVar(var(c[i]), map, max)+1);
    fprintf(f, "0\n");
}


void Solver::toDimacs(const char *file, const vec<Lit>& assumps)
{
    FILE* f = fopen(file, "wr");
    if (f == NULL)
        fprintf(stderr, "could not open file %s\n", file), exit(1);
    toDimacs(f, assumps);
    fclose(f);
}


void Solver::toDimacs(FILE* f, const vec<Lit>& assumps)
{
    // Handle case when solver is in contradictory state:
    if (!ok){
        fprintf(f, "p cnf 1 2\n1 0\n-1 0\n");
        return; }

    vec<Var> map; Var max = 0;

    // Cannot use removeClauses here because it is not safe
    // to deallocate them at this point. Could be improved.
    int cnt = 0;
    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]]))
            cnt++;

    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]])){
            Clause& c = ca[clauses[i]];
            for (int j = 0; j < c.size(); j++)
                if (value(c[j]) != l_False)
                    mapVar(var(c[j]), map, max);
        }

    // Assumptions are added as unit clauses:
    cnt += assumptions.size();

    fprintf(f, "p cnf %d %d\n", max, cnt);

    for (int i = 0; i < assumptions.size(); i++){
        assert(value(assumptions[i]) != l_False);
        fprintf(f, "%s%d 0\n", sign(assumptions[i]) ? "-" : "", mapVar(var(assumptions[i]), map, max)+1);
    }

    for (int i = 0; i < clauses.size(); i++)
        toDimacs(f, ca[clauses[i]], map, max);

    if (verbosity > 0)
        printf("Wrote %d clauses with %d variables.\n", cnt, max);
}


//=================================================================================================
// Garbage Collection methods:

void Solver::relocAll(ClauseAllocator& to)
{
    // All watchers:
    //
    // for (int i = 0; i < watches.size(); i++)
    watches.cleanAll();
    // added by nabesima
    if (bin_propagation) bin_watches.cleanAll();
    for (int v = 0; v < nVars(); v++)
        for (int s = 0; s < 2; s++){
            Lit p = mkLit(v, s);
            // printf(" >>> RELOCING: %s%d\n", sign(p)?"-":"", var(p)+1);
            vec<Watcher>& ws = watches[p];
            for (int j = 0; j < ws.size(); j++)
                ca.reloc(ws[j].cref, to);
            // added by nabesima
            if (bin_propagation) {
                vec<Watcher>& ws = bin_watches[p];
                for (int j = 0; j < ws.size(); j++)
                    ca.reloc(ws[j].cref, to);
            }
        }

    // All reasons:
    //
    for (int i = 0; i < trail.size(); i++){
        Var v = var(trail[i]);
        if (reason(v) != CRef_Undef && (ca[reason(v)].reloced() || locked(ca[reason(v)])))
            ca.reloc(vardata[v].reason, to);
    }

    // All learnt:
    //
    for (int i = 0; i < learnts.size(); i++)
        ca.reloc(learnts[i], to);

    // All original:
    //
    for (int i = 0; i < clauses.size(); i++)
        ca.reloc(clauses[i], to);

//    // DEBUG
//    for (int i = 0; i < org_clauses.size(); i++)
//        ca.reloc(org_clauses[i], to);
}

void Solver::garbageCollect()
{
    // Initialize the next region to a size corresponding to the estimated utilization degree. This
    // is not precise but should avoid some unnecessary reallocations for the new region:
    ClauseAllocator to(ca.size() - ca.wasted());

    relocAll(to);
    if (verbosity >= 2)
        printf("c     |  Garbage collection:   %12d bytes => %12d bytes             |\n",
               ca.size()*ClauseAllocator::Unit_Size, to.size()*ClauseAllocator::Unit_Size);
    to.moveTo(ca);
}

// added by nabesima
void Solver::makePartialModel() {
    //if (decisionLevel() == 0) return;
    for (int k=trail.size() - 1; k >= 0; k--) { // trail_lim[0]; k--) {
        Lit p = trail[k];

        // Since unit clauses are not added to database.
        if (level(var(p)) == 0 && reason(var(p)) == CRef_Undef)
            continue;

        lbool          old = assigns[var(p)];
        assigns[var(p)] = l_Undef;

        vec<Watcher>&  ws = watches[~p];
        Watcher        *i, *j, *end;
        bool            undef = false;
        for (i = j = (Watcher*)ws, end = i + ws.size();  i != end;) {
            // Try to avoid inspecting the clause:
            Lit blocker = i->blocker;
            if (value(blocker) == l_True) { *j++ = *i++; continue; }

            CRef     cr = i->cref;
            Clause&  c  = ca[cr];

            if (c.learnt()) { *j++ = *i++; continue; }

            // Make sure the undef literal is data[1]:
            Lit undef_lit = p;
            if (c[0] == undef_lit)
                c[0] = c[1], c[1] = undef_lit;
            assert(c[1] == undef_lit);
            i++;

            //printf("p = "); printLit(p); printf(" : "); printClause(c);

            // If 0th watch is true, then clause is already satisfied.
            Lit     first = c[0];
            if (first != blocker && value(first) == l_True) {
                *j++ = Watcher(cr, first); continue; }

            // Look for new watch:
            for (int k = 2; k < c.size(); k++)
                if (value(c[k]) == l_True) {
                    c[1] = c[k]; c[k] = undef_lit;
                    watches[~c[1]].push(Watcher(cr, c[k]));
                    goto NextClause; }

            // Did not find watch -- clause is unit under assignment:
            *j++ = Watcher(cr, first);
            // Copy the remaining watches:
            while (i < end)
                *j++ = *i++;
            undef = true;

        NextClause:;
        }
        ws.shrink(i - j);

        if (undef)
            assigns[var(p)] = old;
        else {
            shrinked_assignments++;
            //printf("**** shrinked "); printLit(p); printf(" *****\n");
        }
    }
}
//=================================================================================================
// Printing methods (added by nabesima):
void Solver::printLit(Lit l, bool detail) const {
    if      (l.x == lit_Undef.x) printf("undef");
    else if (l.x == lit_Error.x) printf("error");
    else if (detail) {
        printf("%s%d:%c", sign(l) ? "-" : "", var(l)+1, value(l) == l_True ? '1' : (value(l) == l_False ? '0' : 'X'));
        if (value(l) != l_Undef) printf("@%d", level(var(l)));
    }
    else {
        printf("%s%d", sign(l) ? "-" : "", var(l)+1);
    }
}
void Solver::printLits(vec<Lit>& lits) const {
    for (int i=0; i < lits.size(); i++) {
        printLit(lits[i]);
        printf(" ");
    }
}
void Solver::printClause(const Clause& c) const {
    printf("{ ");
    for (int i=0; i < c.size(); i++) {
        printLit(c[i]);
        printf(" ");
    }
    printf("} ");
    if (c.learnt()) printf("lbd=%d/%d ", c.lbd(), c.delta_lbd());
    if (c.has_extra()) printf("act=%f", c.activity());
    printf("\n");
}
void Solver::printSortedClause(const Clause& c) const {
    vec<Lit> lits;
    for (int i=0; i < c.size(); i++)
        lits.push(c[i]);
    sort(lits);
    printf("{ ");
    for (int i=0; i < lits.size(); i++) {
        printLit(lits[i]);
        printf(" ");
    }
    printf("} ");
    if (c.has_extra()) printf("act=%f", c.activity());
    printf("\n");
}
void Solver::printDecisionStack(int from) const {
    int idx = (from == 0) ? 0 : trail_lim[from-1];
    for (int i=from; i < decisionLevel(); i++) {
        printf("DLV%3d: ", i);
        for (; idx < trail_lim[i]; idx++) {
            printLit(trail[idx], false);
            printf(" ");
        }
        printf("\n");
    }
    printf("DLV%3d: ", decisionLevel());
    for (; idx < trail.size(); idx++) {
        printLit(trail[idx], false);
        printf(" ");
    }
    printf("\n");
}
void Solver::printProgressHeader() const {
    printf("c\n");
    printf("c        #Restarts  #Confs   #Vars   #Clauses    Limit #Learnts  Lit/Cl LBD/Cl  DLVs Trail Progress\n");
    printf("c\n");
}
void Solver::printProgress(const char sign) {
    if (num_progress_report == 0) {
        if (starts > 0) {
            double cpu_time = cpuTime();
            uint32_t probed_vars = 0;
            for (int i=0; i < num_probed_lits.size(); i++)
                probed_vars += num_probed_lits[i];
            printf("c\n");
            printf("c    restarts          : %-12"PRIu64"   (%.2f confs/res, %"PRIu64" ~ %"PRIu64" confs, %"PRIu64" blk, last %"PRIu64")\n", starts - 1, (double)conflicts / starts, min_confs, max_confs, blocked_restarts, last_blocked - 1);
            printf("c    conflicts         : %-12"PRIu64"   (%.0f /sec, %"PRIu64" blks, avg %.2f)\n", conflicts, conflicts / cpu_time, succ_confs.total(), succ_confs.avg());
            printf("c    decisions         : %-12"PRIu64"   (%4.2f%% random, %.0f /sec)\n", decisions, (float)rnd_decisions*100 / (float)decisions, decisions / cpu_time);
            printf("c    propagations      : %-12"PRIu64"   (%.0f /sec, %4.2f lits/dec)\n", propagations, propagations/cpu_time, (float)propagations / decisions);
            printf("c    conflict literals : %-12"PRIu64"   (%4.2f%% deleted, %4.2f%% by bin min, %.3f s)\n", tot_literals, (max_literals - tot_literals)*100 / (double)max_literals, (double)rm_lrn_lits*100 / (tot_literals + rm_lrn_lits), bin_min_time);
            printf("c    lbd               : %-12.2f   (%4.2f for all, %4.2f%% bumps)\n", (double)tot_lbds / nLearnts(), (double)global_LBDs / conflicts, (double)lbd_act_bumps * 100 / prop_lits_by_learnts);
            printf("c    reduce dbs        : %-12d   (%"PRIu64" learnts removed, %4.2f%%, %.3f s)\n", reduce_dbs, removed_learnts, removed_learnts * 100 / (double)conflicts, reduce_time);
            printf("c    rewrite dbs       : %-12d   (%"PRIu64"+%"PRIu64" rw-lits, %"PRIu64"+%"PRIu64" taut, %"PRIu64"+%"PRIu64" unit, %.3f s)\n", rewrite_dbs, rw_clause_lits, rw_learnt_lits, rw_taut_clauses, rw_taut_learnts, rw_unit_clauses, rw_unit_learnts, rewrite_time);
            printf("c    premise updates   : %-12"PRIu64"   (%4.2f /lit)\n", premise_updates, (double)premise_updates / simp_vars / 2);
            printf("c    probed variables  : %-12d   (%d chain, %d fls, %d pol, %d eqv, %d bin, %d cla, %d ss)\n", probed_vars, num_probed_lits[PRB_PREMISE_CHAIN], num_probed_lits[PRB_FAILED_LIT], num_probed_lits[PRB_POLALITY], num_probed_lits[PRB_EQV_LIT], num_probed_lits[PRB_BIN_CLAUSE], num_probed_lits[PRB_CLAUSE], num_probed_lits[PRB_SELF_SUBSUMP]);
            printf("c    unwatched lit elim: %-12"PRIu64"   (bin %"PRIu64"+%"PRIu64", %4.2f+%4.2f%%, lazy %"PRIu64"+%"PRIu64", %4.2f+%4.2f%% of %"PRIu64"+%"PRIu64")\n", bin_rm_uw_cla_lits + bin_rm_uw_lrn_lits + lazy_rm_uw_cla_lits + lazy_rm_uw_lrn_lits, bin_rm_uw_cla_lits, bin_rm_uw_lrn_lits, (double)bin_rm_uw_cla_lits * 100 / simp_clauses_literals, (double)bin_rm_uw_lrn_lits * 100 / tot_literals, lazy_rm_uw_cla_lits, lazy_rm_uw_lrn_lits, (double)lazy_rm_uw_cla_lits * 100 / simp_clauses_literals, (double)lazy_rm_uw_lrn_lits * 100 / tot_literals, simp_clauses_literals, tot_literals);
            printf("c    lazy added bins   : %-12d\n", lazy_added_bins);
            printf("c    subsumed learnts  : %-12d   (%4.2f %%)\n", sc_subsumed_learnts, (double)sc_subsumed_learnts * 100 / conflicts);
        }
        printProgressHeader();
    }
    if (++num_progress_report > 15) num_progress_report = 0;
    printf("c%5d  %c %7d %9d %8d %8d %8d %8d %6.0f %6.0f %6.1f %5.1f%% %6.3f%%\n",
            (int)(realTime() - real_stime), sign,
            (int)starts, (int)conflicts, nFreeVars(), nClauses(),
            (int)max_learnts, nLearnts(),
            (double)learnts_literals/nLearnts(), (double)tot_lbds/nLearnts(),
            stat_DLVs.average(), stat_trails.average() * 100 / nVars(), progressEstimate()*100);
    fflush(stdout);
}
