// nfm/nfm/funcdef.h -- function definitions
// Copyright(c) KAMINA Jiro 2006-2010.  All rights reserved.
// $Header$

#if !defined(FUNCDEF_H_)
#define FUNCDEF_H_

#include	"nfm.h"

// getprob.c
extern int fm_get_problem(void);

// check.c
extern void fm_check(long depth);
extern void fm_hi_move(long depth);

// escape.c
extern void fm_escape(long depth);
extern void fm_yo_move(long depth);

// sub.c
// - system
extern void fm_optargs(int argc, char **argv);
extern FILE *fm_fopen(const char *file_name, const char *mode);
extern int fm_fgets(char *buf, long size, FILE *Fp);
extern void *fm_calloc(size_t n, size_t size, int ret_null);
extern void fm_reset_memory(void);
extern void fm_sigexit(int sig);
// - link
extern board_t *fm_get_board(long depth);
extern void fm_free_board_depth(long depth);
extern mseq_t *fm_get_mseq(long depth);
extern void fm_attach_mseq_nfmh(mseq_t *msp);
extern void fm_attach_mseq_nfmha(mseq_t *msp);
extern void fm_attach_mseq_nfmhs(mseq_t *msp);
extern void fm_attach_mseq_nfmhsa(mseq_t *msp);
extern void fm_attach_mseq_nfmsh(mseq_t *msp);
extern void fm_attach_mseq_nfmser(mseq_t *msp);
extern void fm_free_mseq(long depth, mseq_t *msp);
extern void fm_free_mseq_depthescape(long depth, mseq_t **hashhead);
extern void fm_free_mseq_depth(long depth, mseq_t **hashhead);
// - others
extern int fm_get_rule_data(char *buf, long size, FILE *Fp);
extern int fm_get_phase_data(char *buf, long size, FILE *Fp, int n);
extern int fm_get_extrapc_data(char *buf, long size, FILE *Fp);
extern char *fm_make_move(mseq_t *msp, int pos_flag);
extern char *fm_make_matedepth(mseq_t *msp);
extern int fm_test_normalpieceset(board_t *bdp);
extern void fm_mile_stone(long depth);
extern void fm_set_constant(void);
extern char *fm_format_num(char *s, ulong_t n);
extern char *fm_format_bignum(char *s, ulonglong_t n);
extern void fm_ltrim(char *s);
extern void fm_rtrim(char *s);
extern void fm_str_toupper(char *s);
extern long fm_strtol(const char *buf, char **next, int base);
extern ulong_t fm_strtoul(const char *buf, char **next, int base);

// mate.c
extern void fm_make_matemseq(long depth_mate);
extern void fm_make_escapemseq(long depth_escape);
extern int fm_test_yo_matemseq(mseq_t *ms_parp);
extern int fm_test_yo_ufmatemseq(mseq_t *ms_parp);
extern void fm_test_yo_mate(long depth);
extern int fm_test_hi_matemseq(mseq_t *ms_parp);
extern int fm_test_hi_ufmatemseq(mseq_t *ms_parp);
extern void fm_test_hi_mate(long depth);
extern int fm_test_targetmseq(mseq_t *msp_par);
extern void fm_test_target(long depth);
extern int fm_change_givenmatepath(void);

// phase.c
extern void fm_set_phase(mseq_t *msp);
extern void fm_back_phase(mseq_t *msp);
extern void fm_prev_phase(mseq_t *msp);
extern void fm_set_fire(mseq_t *msp);
extern int fm_test_move(mseq_t *msp);
extern int fm_set_givenphase(mseq_t *msp);

// hash.c
extern mseq_t *fm_test_hash(mseq_t *msp);
extern void fm_attach_hash(mseq_t *msp);
extern void fm_free_hash(mseq_t *msp, mseq_t **hashhead);
extern void fm_calc_hash(mseq_t *msp);
extern void fm_back_hash(mseq_t *msp);
extern void fm_init_hash(mseq_t *msp);
extern void fm_make_hashtable(void);

// output.c
extern void fm_put_both(int msgcode, ...);
extern void fm_put_display(int msgcode, ...);
extern void fm_put_file(int msgcode, ...);
extern void fm_put_oneway(int msgcode, ...);
extern void fm_fflush(void);
extern void fm_put_phase(int mode, board_t *bdp);
extern void fm_put_problem(int mode);
extern void fm_put_result(long depth, const char *state);
extern void fm_put_record(long depth);
extern void fm_put_matemseq(mseq_t *msp_start, mseq_t *msp_mate);
extern void fm_put_meminfo(void);
extern void fm_put_lastphasecount(long depth_last);
//extern void fm_put_interrupted(void);
//extern void fm_put_indata(long depth, board_t *bdp, long n);
extern void fm_put_hashinfo(void);
extern void fm_put_indata(char *buf);

// console.c
extern void win32api_console_initialize(void);
extern void win32api_console_carriage_return(void);
extern void win32api_console_cursor_up(int y);
extern void win32api_console_cursor_down(int y);
extern void win32api_console_clear_line(void);
extern void win32api_console_reset(void);

// debug.c
extern char *dbg_ps_flag(mseq_t *msp);
extern void dbg_mseq(int mode, mseq_t *msp);
extern void dbg_mseq_list(int mode, mseq_t *msh);
extern void dbg_hash_list(int mode);
//extern void dbg_mseqhead(int mode);
extern void dbg_mseqhead(int mode, long depth_start, long depth_end);
extern void dbg_fire(int mode, board_t *bdp);
extern void dbg_check_board(void);
extern void dbg_check_mseq(void);
extern void dbg_ref_count(void);
extern char *dbg_mseqhead_flag(long depth);
extern void dbg_phasecount(void);

#endif // !FUNCDEF_H_

