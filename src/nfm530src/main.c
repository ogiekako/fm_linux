// Fairy Mate 4.xx
// nfm/nfm/main.c -- FM with Dr.NOSHITA's idea solver main routines
// Copyright(c) KAMINA Jiro 2006-2013.  All rights reserved.
// $Header$

static char copyright[] = "Copyright(c) KAMINA Jiro 2006-2013.  All rights reserved.";

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>
#include	<signal.h>
#include	<math.h>
#include	<sys/stat.h>
#include	<setjmp.h>
#if defined(_MSC_VER)
#include	<conio.h>
#include	<windows.h>
#endif
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
#include	"message.h"

static void
sysinit(void)
{
	signal(SIGINT, SIG_IGN);
	setvbuf(stdout, NULL, _IONBF, 0);
	inFp = NULL;
	fileFp = NULL;
#if defined(_MSC_VER)
	win32api_console_initialize();
	atexit(win32api_console_reset);
#endif
	return;
}

static void
systerm(void)
{
	if (inFp != NULL) {
		fclose(inFp);
		inFp = NULL;
	}
	if (fileFp != NULL) {
		fclose(fileFp);
		fileFp = NULL;
	}
	return;
}

static void
userinit(int argc, char **argv)
{
#if defined(_MSC_VER)
    MEMORYSTATUS ms;
#endif

	fm_put_display(MSG_COPYRIGHT, cmdname, release);
	fm_optargs(argc, argv);
	if (real_memsize != 0xffffffff) {
		fm_reset_memory();
	}
	hashhead_check = fm_calloc(hashhead_count, sizeof(hashhead_check[0]), OFF);
	hashhead_escape = fm_calloc(hashhead_count, sizeof(hashhead_escape[0]), OFF);
	fm_make_hashtable();
	if (sc_print_verbose == ON) {
		fm_put_meminfo();
	} else {
		fm_put_display(MSG_DIRECT, "\n");
	}
#if defined(_MSC_VER)
	memset(&ms, '\0', sizeof(ms));
	GlobalMemoryStatus(&ms);
	if (virtual_memsize == 0xffffffff) {
		free_memsize = ms.dwAvailPhys;
		if (board_max_count > 0) {
			free_memsize -= board_max_memsize;
		}
		if (mseq_max_count > 0) {
			free_memsize -= mseq_max_memsize;
		}
	} else {
		if (virtual_memsize > ms.dwTotalPhys) {
			free_memsize = ms.dwTotalPhys;
		} else {
			free_memsize = virtual_memsize;
		}
	}
#endif
	return;
}

static void
userterm(void)
{
	// now, do nothing here...
	return;
}

static int
ask_solve(void)
{
	int c;

	if (phase_count > 0) {
		fm_put_display(MSG_DIRECT, "\n");
	}
	fm_put_problem(B_PM_DISPLAY);
	if (sc_env_autostart == OFF) {
		FOREVER() {
			fm_put_display(MSG_ASKSOLVE);
			c = GETCH();
			if (isalnum(c)) {
				fm_put_display(MSG_DIRECT, "%c", c);
			}
			while (KBHIT()) {
				(void)GETCH();
			}
			c = toupper(c);
			if (c == 'N') {
				fm_put_display(MSG_DIRECT, "\n\n");
				return 'N';
			}
			if (c == 'X' || c == '\x03' || c == '\x11') {
				fm_put_display(MSG_DIRECT, "\n\n");
				return 'X';
			}
			if (c == 'Y' || c == '\n' || c == '\r') {
				break;
			}
			CARRIAGE_RETURN();
		}
		CLEAR_LINE();
	}
	fm_put_problem(B_PM_FILE);
	return 'Y';
}

static int
ask_continue(void)
{
	int c;

	fm_put_display(MSG_DIRECT, "\n");
	FOREVER() {
		fm_put_display(MSG_ASKCONTINUE);
		c = GETCH();
		if (isalnum(c)) {
			fm_put_display(MSG_DIRECT, "%c", c);
		}
		while (KBHIT()) {
			(void)GETCH();
		}
		c = toupper(c);
		if (c == 'N') {
			fm_put_display(MSG_DIRECT, "\n");
			return 'N';
		}
		if (c == 'Y' || c == '\n' || c == '\r') {
			break;
		}
		CARRIAGE_RETURN();
	}
	CLEAR_LINE();
	return 'Y';
}

static void
before_solve(void)
{
	if (rule == RULE_NFMSHM || rule == RULE_NFMSERM || rule == RULE_NFMHA) {
		save_sc_tact_sameaslesspc = sc_tact_sameaslesspc;
		save_sc_tact_sameasmorepc = sc_tact_sameasmorepc;
		save_sc_tact_sameaslessfu = sc_tact_sameaslessfu;
		save_sc_tact_sameasmorefu = sc_tact_sameasmorefu;
		sc_tact_sameaslesspc = OFF;
		sc_tact_sameasmorepc = OFF;
		sc_tact_sameaslessfu = OFF;
		sc_tact_sameasmorefu = OFF;
	}
	if (rule == RULE_NFMSHM || rule == RULE_NFMSERM) {
		save_sc_tact_freemseq = sc_tact_freemseq;
		sc_tact_freemseq = OFF;
	}
	mseq_head = fm_calloc(depth_limit + depth_rsv, sizeof(mseq_head[0]), OFF);
	free_memsize -= sizeof(mseq_head[0]) * (depth_limit + depth_rsv);
	return;
}

static void
before_solve2(void)
{
	mseq_t *msp;
	int i;

	phase_count = 0;
	check_point = 0;
	total_elapse = 0;
	start_time = time((time_t *)NULL);	// dummy
	for (i = 0; i < depth_limit + depth_rsv; i++) {
		mseq_head[i] = mseqhead_void;
	}
	for (i = 0; i < hashhead_count; i++) {
		hashhead_check[i] = NULL;
		hashhead_escape[i] = NULL;
	}
	msp = fm_get_mseq(0);
	*msp = mseq_prob;
	msp->BDP = fm_get_board(0);
	*msp->BDP = board_prob;
	if (rule != RULE_NFMHA) {
		fm_attach_hash(msp);
	}
	mseq_head[0].brop = msp;
	mseq_head[0].all_move_count = 1;
	mseq_head[0].legal_move_count = 1;
	mseq_head[0].new_phase_count = 1;
	mseq_head[0].unique_phase_count = 1;
	phase_count++;
	if (rule & RULE_TARGETFIG) {
		mseq_target = mseq_void;
		mseq_target.BDP = &board_target;
		fm_set_fire(&mseq_target);
		fm_init_hash(&mseq_target);
	}
	if (sc_tact_givenmatepath == ON
		&& (rule == RULE_NFMHM || rule == RULE_NFMHMA
			|| rule == RULE_NFMHSM || rule == RULE_NFMHSMA || rule == RULE_NFMHA))
	{
		for (msp = msp_givenmate->brop; msp != NULL; msp = msp->brop) {
			fm_attach_hash(msp);
		}
	}
	retry = OFF;
	return;
}

static void
solve_nfmh(void)
{
	mseq_t *msp;
	time_t old_total_elapse;
	long depth;

	if (sc_tact_givenmatepath == ON) {
		msp = msp_givenmate->brop;
	}
	for (depth = 0; depth < depth_limit; depth++) {
		if ((depth + depth_limit) % 2 == 1) {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_check);
				msp = msp->brop;
			}
			fm_check(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_escape);
			}
		} else {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_escape);
				msp = msp->brop;
			}
			fm_escape(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE)) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_check);
			}
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		fm_test_yo_mate(depth);
		total_elapse = time((time_t *)NULL) - start_time;
	}
	return;
}

static void
solve_nfmha(void)
{
	mseq_t *msp;
	time_t old_total_elapse;
	long depth;

	if (sc_tact_givenmatepath == ON) {
		msp = msp_givenmate->brop;
	}
	if (mseq_target.ps_flag & B_PS_HI_CHECK) {
		mask_targetdepth = 1;
	} else {
		mask_targetdepth = 0;
	}
	for (depth = 0; depth < depth_limit; depth++) {
		if ((depth + depth_limit) % 2 == mask_targetdepth) {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_check);
				msp = msp->brop;
			}
			fm_check(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_escape);
			}
			if (mask_targetdepth == 1) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		} else {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_escape);
				msp = msp->brop;
			}
			fm_escape(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE)) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_check);
			}
			if (mask_targetdepth == 0) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		total_elapse = time((time_t *)NULL) - start_time;
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

static void
solve_nfmhs(void)
{
	mseq_t *msp;
	time_t old_total_elapse;
	long depth;

	if (sc_tact_givenmatepath == ON) {
		msp = msp_givenmate->brop;
	}
	for (depth = 0; depth < depth_limit; depth++) {
		if ((depth + depth_limit) % 2 == 0) {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_check);
				msp = msp->brop;
			}
			fm_check(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if ((mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))
				|| (mseq_head[depth + 1].flag & B_MH_MATE))
			{
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_escape);
			}
		} else {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_escape);
				msp = msp->brop;
			}
			fm_escape(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_check);
			}
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		fm_test_hi_mate(depth);
		total_elapse = time((time_t *)NULL) - start_time;
	}
	return;
}

static void
solve_nfmhsa(void)
{
	mseq_t *msp;
	time_t old_total_elapse;
	long depth;

	if (sc_tact_givenmatepath == ON) {
		msp = msp_givenmate->brop;
	}
	if (mseq_target.ps_flag & B_PS_HI_CHECK) {
		mask_targetdepth = 1;
	} else {
		mask_targetdepth = 0;
	}
	for (depth = 0; depth < depth_limit; depth++) {
		if ((depth + depth_limit) % 2 == mask_targetdepth) {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_check);
				msp = msp->brop;
			}
			fm_check(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE)) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_escape);
			}
			if (mask_targetdepth == 1) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		} else {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_escape);
				msp = msp->brop;
			}
			fm_escape(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_check);
			}
			if (mask_targetdepth == 0) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		total_elapse = time((time_t *)NULL) - start_time;
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

static void
solve_nfmsh(void)
{
	time_t old_total_elapse;
	long depth;

	for (depth = 0; depth < depth_limit; depth++) {
		fm_yo_move(depth);
		old_total_elapse = total_elapse;
		total_elapse = time((time_t *)NULL) - start_time;
		if (mseq_head[depth].flag & B_MH_ESCAPE) {
			break;
		}
		if (total_elapse - old_total_elapse > 0) {
			fm_put_record(depth + 1);
		}
		fm_check(depth + 1);
		old_total_elapse = total_elapse;
		total_elapse = time((time_t *)NULL) - start_time;
		if (mseq_head[depth + 2].flag & B_MH_MATE) {
			break;
		}
		if (total_elapse - old_total_elapse > 0) {
			fm_put_record(depth + 2);
		}
		fm_free_mseq_depth(depth + 2, hashhead_check);
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

static void
solve_nfmser(void)
{
	time_t old_total_elapse;
	long depth;

	for (depth = 0; depth < depth_limit; depth++) {
		flag_docheck = ON;
		fm_check(depth);
		old_total_elapse = total_elapse;
		total_elapse = time((time_t *)NULL) - start_time;
		if (mseq_head[depth + 1].flag & B_MH_MATE) {
			break;
		}
		if (total_elapse - old_total_elapse > 0) {
			fm_put_record(depth + 1);
		}
		fm_free_mseq_depth(depth + 1, hashhead_check);
		flag_docheck = OFF;
		fm_hi_move(depth);
		old_total_elapse = total_elapse;
		total_elapse = time((time_t *)NULL) - start_time;
		if (mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE)) {
			break;
		}
		if (total_elapse - old_total_elapse > 0) {
			fm_put_record(depth + 1);
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

static void
solve_nfma(void)
{
	mseq_t *msp;
	time_t old_total_elapse;
	long depth;

	if (sc_tact_givenmatepath == ON) {
		msp = msp_givenmate->brop;
	}
	if (mseq_target.ps_flag & B_PS_HI_CHECK) {
		mask_targetdepth = 1;
	} else {
		mask_targetdepth = 0;
	}
	for (depth = 0; depth < depth_limit; depth++) {
		if ((depth + depth_limit) % 2 == mask_targetdepth) {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_check);
				msp = msp->brop;
			}
			fm_check(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_escape);
			}
			if (mask_targetdepth == 1) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		} else {
			if (sc_tact_givenmatepath == ON) {
				fm_free_hash(msp, hashhead_escape);
				msp = msp->brop;
			}
			fm_escape(depth);
			old_total_elapse = total_elapse;
			total_elapse = time((time_t *)NULL) - start_time;
			if (mseq_head[depth].flag & B_MH_ESCAPE) {
				break;
			}
			if (total_elapse - old_total_elapse > 0) {
				fm_put_record(depth + 1);
			}
			if (sc_tact_freemseq == ON) {
				fm_free_mseq_depthescape(depth, hashhead_check);
			}
			if (mask_targetdepth == 0) {
				fm_test_target(depth + 1);
				old_total_elapse = total_elapse;
				total_elapse = time((time_t *)NULL) - start_time;
				if (mseq_head[depth + 1].flag & B_MH_MATE) {
					break;
				}
				if (total_elapse - old_total_elapse > 0) {
					fm_put_record(depth + 1);
				}
			}
		}
	}
	if (depth >= depth_limit && NOT(mseq_head[depth].flag & (B_MH_MATE | B_MH_ESCAPE))) {
		total_elapse = time((time_t *)NULL) - start_time;
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

static void
solve(void)
{
	signal(SIGINT, SIG_IGN);
	start_time = time((time_t *)NULL);
	fm_put_record(0);
	switch (rule) {
	case RULE_NFMHM:
		solve_nfmh();
		break;
	case RULE_NFMHMA:
		solve_nfmha();
		break;
	case RULE_NFMHSM:
		solve_nfmhs();
		break;
	case RULE_NFMHSMA:
		solve_nfmhsa();
		break;
	case RULE_NFMSHM:
		solve_nfmsh();
		break;
	case RULE_NFMSERM:
		solve_nfmser();
		break;
	case RULE_NFMHA:
		solve_nfma();
		break;
	}
	signal(SIGINT, SIG_DFL);
	return;
}

static void
after_solve(void)
{
	mseq_t *msp, *msp_brop;
	long depth;

	for (depth = 0; depth <= depth_limit + depth_rsv - 1; depth++) {
		if (mseq_head[depth].flag & B_MH_MATE) {
			fm_put_result(depth, str[STR_TERMINATE]);
			fm_make_matemseq(depth);
			break;
		}
		if (mseq_head[depth].flag & B_MH_ESCAPE) {
			fm_put_result(depth, str[STR_TERMINATE]);
			fm_put_both(MSG_DIRECT, "\n");
			fm_put_both(MSG_NOTMATE);
			break;
		}
	}
	if (sc_print_verbose == ON) {
		fm_put_lastphasecount(depth - 1);
	}
	if (sc_print_hashinfo == ON) {
		fm_put_hashinfo();
	}
	fm_put_file(MSG_DIRECT, "\n");
	fm_fflush();
	if (retry == ON) {
		if (fm_change_givenmatepath() == -1) {
			retry = OFF;
		}
	}
	for (depth = 0; depth <= depth_limit + depth_rsv - 1; depth++) {
		for (msp = mseq_head[depth].brop; msp != NULL; msp = msp_brop) {
			if (msp->BDP != NULL && NOT(msp->ps_flag & B_PS_SAMEPHASE)) {
				FREE_BOARD(msp->BDP);
			}
			msp_brop = msp->brop;
			FREE_MSEQ(msp);
		}
		mseq_head[depth].brop = NULL;
	}
	// 使用セル数の検証
//	if (sc_print_verbose == ON) {
//		dbg_check_mseq();
//		dbg_check_board();
//		fm_put_both(MSG_DIRECT, "\n");
//	}
	return;
}

static void
after_solve2(void)
{
	if (sc_env_autostart == OFF) {
		return;
	}
	free(mseq_head);
	free_memsize += sizeof(mseq_head[0]) * (depth_limit + depth_rsv);
	if (rule == RULE_NFMSHM || rule == RULE_NFMSERM || rule == RULE_NFMHA) {
		sc_tact_sameaslesspc = save_sc_tact_sameaslesspc;
		sc_tact_sameasmorepc = save_sc_tact_sameasmorepc;
		sc_tact_sameaslessfu = save_sc_tact_sameaslessfu;
		sc_tact_sameasmorefu = save_sc_tact_sameasmorefu;
	}
	if (rule == RULE_NFMSHM || rule == RULE_NFMSERM) {
		sc_tact_freemseq = save_sc_tact_freemseq;
	}
	return;
}

int
main(int argc, char **argv)
{
	int c;

	sysinit();
	userinit(argc, argv);
	FOREVER() {
		if (fm_get_problem() != 0) {
			break;
		}
		fm_set_constant();
		c = ask_solve();
		if (c == 'N') {
			continue;
		}
		if (c == 'X') {
			break;
		}
		before_solve();
		FOREVER() {
			before_solve2();
			solve();
			after_solve();
			if (sc_tact_givenmatepath == OFF || retry == OFF) {
				break;
			}
			c = ask_continue();
			if (c == 'N') {
				break;
			}
		}
		if (sc_env_autostart == OFF) {
			break;
		}
		after_solve2();
	}
	userterm();
	systerm();

	return 0;
}

