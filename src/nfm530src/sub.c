// nfm/nfm/sub.c -- sub-functions
// Copyright(c) KAMINA Jiro 2006-2013.  All lefts reserved.
// $Header$

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>
#include	<signal.h>
#include	<setjmp.h>
#if defined(_MSC_VER)
#include	<mbstring.h>
#include	<conio.h>
#include	<windows.h>
#endif
#include	"dosopt.h"
#include	"sjis.h"
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
#include	"message.h"

// analyze optional arguments
void
fm_optargs(int argc, char **argv)
{
	char *s, *t;
	ulong_t n;
	int c;

	if (argc == 0) {
		return;
	}
	while ((c = dosopt(argv, "P:O:E:I:S:T:")) != EOF) {
		switch (c) {
		case 'P':	// printing (output)
			for (s = optarg; *s != '\0'; s++) {
				switch (*s) {
				case ',':
				case ' ':
					break;
				case 'C':	// Chess move output style
					sc_print_chesstyle = ON;
					break;
				case 'D':	// Debug
					sc_print_debug = ON;
					break;
				case 'E':	// Ending figure
					sc_print_endfig = ON;
					break;
				case 'H':	// Hash information
					sc_print_hashinfo = ON;
					break;
				case 'L':	// Loose mate sequence
					sc_print_loosemate = ON;
					break;
				case 'M':	// Mate figure
					sc_print_matefig = ON;
					break;
				case 'N':	// Not display
					sc_print_notdisplay = ON;
					break;
				case 'T':	// Traditional move output style
					sc_print_tradstyle = ON;
					break;
				case 'S':	// Starting figure
					sc_print_startfig = ON;
					break;
				case 'V':	// Verbose
					sc_print_verbose = ON;
					break;
				default:
					fm_put_display(MSG_ERR_OPTARG);
					exit(1);
				}
			}
			break;
		case 'O':	// output file
#if defined(_MSC_VER)
			if (GetShortPathName(optarg, out_file, sizeof(out_file)) == 0) {
				strncpy(out_file, optarg, STR_SIZE_OF(out_file));
			}
#else
			strncpy(out_file, optarg, STR_SIZE_OF(out_file));
#endif
			fileFp = fm_fopen(out_file, "a");
			break;
		case 'E':	// environment
			for (s = optarg; *s != '\0'; s++) {
				switch (*s) {
				case ',':
				case ' ':
					break;
				case 'A':	// auto start
					sc_env_autostart = ON;
					break;
				case 'R':	// reset
					if (*(s + 1) != '=') {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					n = fm_strtoul(s + 2, &t, 10);
					if (NOT(EQUALZ(t, "MB")) && NOT(EQUALZ(t, "mb"))) {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					real_memsize = MUL_MEGA(n);
					s = t + STR_SIZE_OF("MB") - 1;
					break;
				case 'V':	// use memory
					if (*(s + 1) != '=') {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					n = fm_strtoul(s + 2, &t, 10);
					if (NOT(EQUALZ(t, "MB")) && NOT(EQUALZ(t, "mb"))) {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					virtual_memsize = MUL_MEGA(n);
					s = t + STR_SIZE_OF("MB") - 1;
					break;
				default:
					fm_put_display(MSG_ERR_OPTARG);
					exit(1);
				}
			}
			break;
		case 'I':	// input file
		case '*':
#if defined(_MSC_VER)
			if (GetShortPathName(optarg, in_file, sizeof(in_file)) == 0) {
				strncpy(in_file, optarg, STR_SIZE_OF(in_file));
			}
#else
			strncpy(in_file, optarg, STR_SIZE_OF(in_file));
#endif
			inFp = fm_fopen(in_file, "r");
			break;
		case 'S':	// size
			for (s = optarg; *s != '\0'; s++) {
				switch (*s) {
				case ',':
				case ' ':
					break;
				case 'B':	// maximum Board count
					if (*(s + 1) != '=') {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					n = fm_strtoul(s + 2, &t, 10);
					if (EQUALZ(t, "MB") || EQUALZ(t, "mb")) {
						board_max_memsize =
							((MUL_MEGA(n) + sizeof(board_t) - 1) / sizeof(board_t))
								* sizeof(board_t);
						board_max_count = (long)(board_max_memsize / sizeof(board_t));
						t += STR_SIZE_OF("MB");
					} else {
						if (n < DEFAULT_BOARD_INC_COUNT) {
							n = DEFAULT_BOARD_INC_COUNT;
						}
						board_max_count = n;
						board_max_memsize = sizeof(board_t) * board_max_count;
					}
					s = t - 1;
					break;
				case 'M':	// maximum Mseq count
					if (*(s + 1) != '=') {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					n = fm_strtoul(s + 2, &t, 10);
					if (EQUALZ(t, "MB") || EQUALZ(t, "mb")) {
						mseq_max_memsize =
							((MUL_MEGA(n) + sizeof(mseq_t) - 1) / sizeof(mseq_t))
								* sizeof(mseq_t);
						mseq_max_count = (long)(mseq_max_memsize / sizeof(mseq_t));
						t += STR_SIZE_OF("MB");
					} else {
						if (n < DEFAULT_MSEQ_INC_COUNT) {
							n = DEFAULT_MSEQ_INC_COUNT;
						}
						mseq_max_count = n;
						mseq_max_memsize = sizeof(mseq_t) * mseq_max_count;
					}
					s = t - 1;
					break;
				case 'R':	// Reuse mseq count
					if (*(s + 1) != '=') {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					n = fm_strtoul(s + 2, &t, 10);
					if (EQUALZ(t, "MB") || EQUALZ(t, "mb")) {
						mseq_reuse_count = MUL_MEGA(n) / sizeof(mseq_t);
						t += STR_SIZE_OF("MB");
					} else {
						mseq_reuse_count = n;
					}
					s = t - 1;
					break;
				default:
					fm_put_display(MSG_ERR_OPTARG);
					exit(1);
				}
			}
			break;
		case 'T':	// tactics
			for (s = optarg; *s != '\0'; s++) {
				switch (s[0]) {
				case ',':
				case ' ':
					break;
				case 'E':	// free Escape mseq
					sc_tact_freemseq = ON;
					break;
				case 'F':	// compare hand-Fu weekly
					if (s[1] == '-') {
						s++;
						if (sc_tact_sameasmorefu == ON
							|| sc_tact_sameaslesspc == ON || sc_tact_sameasmorepc == ON)
						{
							fm_put_display(MSG_ERR_OPTARG);
							exit(1);
						}
						sc_tact_sameaslessfu = ON;
					} else if (s[1] == '+') {
						s++;
						if (sc_tact_sameaslessfu == ON
							|| sc_tact_sameaslesspc == ON || sc_tact_sameasmorepc == ON)
						{
							fm_put_display(MSG_ERR_OPTARG);
							exit(1);
						}
						sc_tact_sameasmorefu = ON;
					} else {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					break;
				case 'H':	// compare Hand weekly
					if (s[1] == '-') {
						s++;
						if (sc_tact_sameaslessfu == ON || sc_tact_sameasmorefu == ON
							|| sc_tact_sameasmorepc == ON)
						{
							fm_put_display(MSG_ERR_OPTARG);
							exit(1);
						}
						sc_tact_sameaslesspc = ON;
					} else if (s[1] == '+') {
						s++;
						if (sc_tact_sameaslessfu == ON || sc_tact_sameasmorefu == ON
							|| sc_tact_sameaslesspc == ON)
						{
							fm_put_display(MSG_ERR_OPTARG);
							exit(1);
						}
						sc_tact_sameasmorepc = ON;
					} else {
						fm_put_display(MSG_ERR_OPTARG);
						exit(1);
					}
					break;
				case 'M':	// given Mate path
					sc_tact_givenmatepath = ON;
					break;
				case 'S':	// free Same mseq
					sc_tact_freesamemseq = ON;
					break;
				case 'R':	// Reuse mseq
					sc_tact_reusemseq = ON;
					break;
				case 'X':	// use eXtra-piece
					sc_tact_usextrapc = ON;
					break;
				default:
					fm_put_display(MSG_ERR_OPTARG);
					exit(1);
				}
			}
			break;
		default:
			fm_put_display(MSG_ERR_OPTARG);
			exit(1);
		}
	}
	if (inFp == NULL) {
		fm_put_display(MSG_ERR_OPTARG);
		exit(1);
	}
	return;
}

FILE *
fm_fopen(const char *file_name, const char *mode)
{
	FILE *Fp;

	Fp = fopen(file_name, mode);
	if (Fp == NULL) {
		fm_put_display(MSG_ERR_FILEOPEN, file_name, mode);
		exit(1);
	}
	return Fp;
}

int
fm_fgets(char *buf, long size, FILE *Fp)
{
	char *s;

	if (fgets(buf, size, Fp) == NULL) {
		buf[0] = '\0';
		return -1;
	}
	s = strchr(buf, '\n');
	if (s != NULL) {
		*s = '\0';
	}
	fm_ltrim(buf);
	fm_rtrim(buf);
	fm_str_toupper(buf);
	return 0;
}

void *
fm_calloc(size_t n, size_t size, int ret_null)
{
	void *p;

	p = calloc(n, size);
	if (p == NULL && ret_null == OFF) {
		fm_put_display(MSG_ERR_MEMALLOC);
		exit(1);
	}
	return p;
}

void
fm_reset_memory(void)
{
#if defined(_MSC_VER)
	char **y, **y_save;
	char **z;
	ulonglong_t x, x_sum, x_save;
    MEMORYSTATUS ms;

	memset(&ms, '\0', sizeof(ms));
	x = real_memsize;
	x_save = x;
	y = fm_calloc(MUL_MEGA(10), 1, OFF);
	x_sum = 10;
	CARRIAGE_RETURN();
	fm_put_display(MSG_MEGABYTES, fm_format_bignum(editbuf, x_sum));
	y_save = y;
	x -= MUL_MEGA(10);
	for (x -= MUL_MEGA(10); x > 0; x -= MUL_MEGA(10)) {
		z = fm_calloc(MUL_MEGA(10), 1, OFF);
		x_sum += 10;
		CARRIAGE_RETURN();
		fm_put_display(MSG_MEGABYTES, fm_format_bignum(editbuf, x_sum));
		*y = (char *)z;
		y = z;
		while (z < y + (MUL_MEGA(10) / sizeof(char **))) {
			*z = (char *)0xffffffff;
			z += 256;
		}
	}
	x = x_save;
	y = y_save;
	z = (char **)*y;
	y = z;
	x -= MUL_MEGA(10);
	for (x -= MUL_MEGA(10); x > 0; x -= MUL_MEGA(10)) {
		z = (char **)*y;
		free(y);
		y = z;
	}
	y = y_save;
	free(y);
	CLEAR_LINE();
#endif
	return;
}

// signal exit
void
fm_sigexit(int sig)
{
	long depth;

	signal(SIGINT, SIG_IGN);
	while (KBHIT()) {
		(void)GETCH();
	}
	CLEAR_LINE();
	total_elapse = time(NULL) - start_time;
	depth = 0;
	if (mseq_head[depth].brop == NULL) {
		for (depth = 1; mseq_head[depth].brop == NULL && depth <= depth_limit; depth++)
			;
		if (depth > depth_limit) {
			depth = 0;
		}
	}
	for (; depth <= depth_limit; depth++) {
		if (mseq_head[depth].brop == NULL) {
			if (depth > 0) {
				depth--;
			}
			break;
		}
	}
	fm_put_result(depth, str[STR_INTERRUPT]);
	fm_put_file(MSG_DIRECT, "\n");
	exit(sig);
}

board_t *
fm_get_board(long depth)
{
	board_t *bdp;
	int i;

	if (bdp_free == NULL) {
		if ((board_max_count > 0 || free_memsize < board_inc_memsize)
			&& (board_max_count == -1
				|| board_max_count < board_inc_count * (boardpool_count + 1)))
		{
		lb_boardover:;
			CLEAR_LINE();
			if (board_max_count == -1) {
				fm_put_both(MSG_ERR_BOARDNOMORE,
					fm_format_num(editbuf, board_inc_count * boardpool_count),
					fm_format_num(editbuf + 16,
						(ulong_t)DIV_MEGA(board_inc_memsize * boardpool_count)),
					fm_format_num(editbuf + 32, board_inc_count),
					fm_format_num(editbuf + 48, (ulong_t)DIV_MEGA(board_inc_memsize)),
					fm_format_num(editbuf + 64, mseq_inc_count * mseqpool_count),
					fm_format_num(editbuf + 90,
						(ulong_t)DIV_MEGA(mseq_inc_memsize * mseqpool_count)));
			} else {
				fm_put_both(MSG_ERR_BOARDOVER,
					fm_format_num(editbuf, board_inc_count * boardpool_count),
					fm_format_num(editbuf + 16,
						(ulong_t)DIV_MEGA(board_inc_memsize * boardpool_count)),
					fm_format_num(editbuf + 64, mseq_inc_count * mseqpool_count),
					fm_format_num(editbuf + 90,
						(ulong_t)DIV_MEGA(mseq_inc_memsize * mseqpool_count)));
			}
			fm_put_lastphasecount(depth);
			fm_put_both(MSG_DIRECT, "\n");
			fm_sigexit(SIG2BIG);
		}
		bdp = fm_calloc(board_inc_count, sizeof(*bdp), ON);
		if (bdp == NULL) {
			goto lb_boardover;
		}
		if (board_max_count <= 0) {
			free_memsize -= board_inc_memsize;
		}
		if (free_memsize < 0) {
			free_memsize = 0;
		}
		bdp_free = bdp;
		for (i = 1; i < board_inc_count; i++) {
			(bdp + i - 1)->next = bdp + i;
		}
		(bdp + board_inc_count - 1)->next = NULL;
		boardpool_count++;
		if (sc_print_verbose == ON) {
			CLEAR_LINE();
			fm_put_display(MSG_ADDBOARD,
				fm_format_num(editbuf, board_inc_count * boardpool_count),
				fm_format_num(editbuf + 16,
					(ulong_t)DIV_MEGA(board_inc_memsize * boardpool_count)),
				fm_format_num(editbuf + 32, (ulong_t)DIV_MEGA(free_memsize)));
			fm_put_record(depth);
		}
	}
	bdp = bdp_free;
	bdp_free = bdp_free->next;
	return bdp;
}

mseq_t *
fm_get_mseq(long depth)
{
	mseq_t *msp, *msp_brop;
	long i, n;
	static mseq_t *msp_brop_last = (mseq_t *)0xffffffff;
	static mseq_t *msp_prev_last;
	static long depth_last;
	static long depth_last2 = -1;

	if (msp_free == NULL) {
		if ((mseq_max_count > 0 || free_memsize < mseq_inc_memsize)
			&& (mseq_max_count == -1
				|| mseq_max_count < mseq_inc_count * (mseqpool_count + 1)))
		{
			if (sc_tact_reusemseq == ON) {
			lb_mseqreuse:;
				if (depth_last2 == -1) {
					depth_last2 = 0;
				}
				n = 0;
				while (depth_last2 <= depth - 2) {
					for (msp = mseq_head[depth_last2].brop; msp != NULL; msp = msp_brop) {
						msp_brop = msp->brop;
						if (IS_HI_PC(msp->pc)) {
							fm_free_hash(msp, hashhead_check);
						} else {
							fm_free_hash(msp, hashhead_escape);
						}
						FREE_MSEQ(msp);
						n++;
					}
					mseq_head[depth_last2].brop = NULL;
					mseq_head[depth_last2].flag |= B_MH_NOCELL;
					depth_last2++;
					depth_start++;
					for (msp = mseq_head[depth_last2].brop; msp != NULL; msp = msp->brop) {
						msp->parp = NULL;
					}
					if (mseq_reuse_count == 0 || n > mseq_reuse_count) {
						break;
					}
				}
				if (sc_print_verbose == ON) {
					CLEAR_LINE();
					fm_put_display(MSG_FREEMSEQ,
						fm_format_num(editbuf, n),
						fm_format_num(editbuf + 16, depth_last2 - 1),
						fm_format_num(editbuf + 32, depth));
				}
			} else {
				if (msp_brop_last == (mseq_t *)0xffffffff) {
					msp_brop_last = mseq_head[1].brop;
					msp_prev_last = NULL;
					depth_last = 1;
				}
				for (; depth_last <= depth - 2; depth_last++) {
					for (msp = msp_brop_last; msp != NULL; msp = msp_brop_last) {
						msp_brop_last = msp->brop;
						ASSERT(NOT(msp->ps_flag & B_PS_SAMEPHASE) || msp->SPHP != NULL);
						if (msp->ref_count == 0
							&& (NOT(msp->ps_flag & B_PS_SAMEPHASE)
								|| msp->SPHP->ref_count == 0))
						{
							msp->parp->ref_count--;
							if (msp_prev_last == NULL) {
								mseq_head[depth_last].brop = msp_brop_last;
							} else {
								msp_prev_last->brop = msp_brop_last;
							}
							if (IS_HI_PC(msp->pc)) {
								fm_free_hash(msp, hashhead_check);
							} else {
								fm_free_hash(msp, hashhead_escape);
							}
							return msp;
						} else {
							msp_prev_last = msp;
						}
					}
					msp_brop_last = mseq_head[depth_last + 1].brop;
					msp_prev_last = NULL;
				}
			}
		}
	}
	if (msp_free == NULL) {
		if ((mseq_max_count > 0 || free_memsize < mseq_inc_memsize)
			&& (mseq_max_count == -1
				|| mseq_max_count < mseq_inc_count * (mseqpool_count + 1)))
		{
		lb_mseqover:;
			CLEAR_LINE();
			if (mseq_max_count == -1) {
				fm_put_both(MSG_ERR_MSEQNOMORE,
					fm_format_num(editbuf, mseq_inc_count * mseqpool_count),
					fm_format_num(editbuf + 16,
						(ulong_t)DIV_MEGA(mseq_inc_memsize * mseqpool_count)),
					fm_format_num(editbuf + 32, mseq_inc_count),
					fm_format_num(editbuf + 48, (ulong_t)DIV_MEGA(mseq_inc_memsize)),
					fm_format_num(editbuf + 64, board_inc_count * boardpool_count),
					fm_format_num(editbuf + 90,
						(ulong_t)DIV_MEGA(board_inc_memsize * boardpool_count)));
			} else {
				fm_put_both(MSG_ERR_MSEQOVER,
					fm_format_num(editbuf, mseq_inc_count * mseqpool_count),
					fm_format_num(editbuf + 16,
						(ulong_t)DIV_MEGA(mseq_inc_memsize * mseqpool_count)),
					fm_format_num(editbuf + 64, board_inc_count * boardpool_count),
					fm_format_num(editbuf + 90,
						(ulong_t)DIV_MEGA(board_inc_memsize * boardpool_count)));
			}
			fm_put_lastphasecount(depth);
			fm_put_both(MSG_DIRECT, "\n");
			fm_sigexit(SIG2BIG);
		}
		msp = fm_calloc(mseq_inc_count, sizeof(*msp), ON);
		if (msp == NULL) {
			if (sc_tact_reusemseq == ON) {
				goto lb_mseqreuse;
			} else {
				goto lb_mseqover;
			}
		}
		if (mseq_max_count <= 0) {
			free_memsize -= mseq_inc_memsize;
		}
		if (free_memsize < 0) {
			free_memsize = 0;
		}
		msp_free = msp;
		for (i = 1; i < mseq_inc_count; i++) {
			(msp + i - 1)->brop = msp + i;
		}
		(msp + mseq_inc_count - 1)->brop = NULL;
		mseqpool_count++;
		if (sc_print_verbose == ON) {
			CLEAR_LINE();
			fm_put_display(MSG_ADDMSEQ,
				fm_format_num(editbuf, mseq_inc_count * mseqpool_count),
				fm_format_num(editbuf + 16,
					(ulong_t)DIV_MEGA(mseq_inc_memsize * mseqpool_count)),
				fm_format_num(editbuf + 32, (ulong_t)DIV_MEGA(free_memsize)));
			fm_put_record(depth);
		}
	}
	msp = msp_free;
	msp_free = msp_free->brop;
	return msp;
}

void
fm_attach_mseq_nfmh(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (IS_HI_PC(msp->pc)) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if (msp->ps_flag & B_PS_HI_CHECK) {
			goto fm_attach_mseq_exit2;
		}
		if ((msp->ps_flag & B_PS_YO_CHECK) && (msp->ps_flag & B_PS_UFCHECK)) {
			if (fm_test_hi_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (rc->hi_hand == msp->hi_hand) {
			if (sc_tact_givenmatepath == ON && (rc->mv_flag & B_MV_GIVEN)) {
				msp->ps_flag |= B_PS_GIVENMATEPATH;
				mseq_head[msp->depth].flag |= B_MH_MATE;
			} else {
				if (sc_tact_freesamemseq == ON) {
					goto fm_attach_mseq_exit;
				}
				msp->ps_flag |= B_PS_SAMEPHASE;
			}
		} else {
			msp->ps_flag |= B_PS_SAMEBOARD;
		}
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_attach_mseq_nfmha(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (IS_HI_PC(msp->pc)) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if (msp->ps_flag & B_PS_HI_CHECK) {
			goto fm_attach_mseq_exit2;
		}
		if ((msp->ps_flag & B_PS_YO_CHECK) && (msp->ps_flag & B_PS_UFCHECK)) {
			if (fm_test_hi_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (rc->hi_hand == msp->hi_hand) {
			if (sc_tact_givenmatepath == ON && (rc->mv_flag & B_MV_GIVEN)) {
				msp->ps_flag |= B_PS_GIVENMATEPATH;
				mseq_head[msp->depth].flag |= B_MH_MATE;
			} else {
				if (sc_tact_freesamemseq == ON) {
					goto fm_attach_mseq_exit;
				}
				msp->ps_flag |= B_PS_SAMEPHASE;
			}
		} else {
			msp->ps_flag |= B_PS_SAMEBOARD;
		}
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_attach_mseq_nfmhs(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (IS_HI_PC(msp->pc)) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if ((msp->ps_flag & B_PS_HI_CHECK)
			|| (msp->depth == depth_limit && NOT(msp->ps_flag & B_PS_YO_CHECK)))
		{
			goto fm_attach_mseq_exit2;
		}
		if ((msp->ps_flag & B_PS_YO_CHECK) && (msp->ps_flag & B_PS_UFCHECK)) {
			if (fm_test_hi_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (sc_tact_givenmatepath == ON && (rc->mv_flag & B_MV_GIVEN)) {
			msp->ps_flag |= B_PS_GIVENMATEPATH;
			mseq_head[msp->depth].flag |= B_MH_MATE;
		} else {
			if (sc_tact_freesamemseq == ON) {
				goto fm_attach_mseq_exit;
			}
			msp->ps_flag |= B_PS_SAMEPHASE;
		}
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_attach_mseq_nfmhsa(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (IS_HI_PC(msp->pc)) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if (msp->ps_flag & B_PS_HI_CHECK) {
			goto fm_attach_mseq_exit2;
		}
		if ((msp->ps_flag & B_PS_YO_CHECK) && (msp->ps_flag & B_PS_UFCHECK)) {
			if (fm_test_hi_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (sc_tact_givenmatepath == ON && (rc->mv_flag & B_MV_GIVEN)) {
			msp->ps_flag |= B_PS_GIVENMATEPATH;
			mseq_head[msp->depth].flag |= B_MH_MATE;
		} else {
			if (sc_tact_freesamemseq == ON) {
				goto fm_attach_mseq_exit;
			}
			msp->ps_flag |= B_PS_SAMEPHASE;
		}
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_attach_mseq_nfmsh(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (IS_HI_PC(msp->pc)) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if (msp->ps_flag & (B_PS_HI_CHECK | B_PS_YO_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if ((msp->ps_flag & B_PS_YO_CHECK) && (msp->ps_flag & B_PS_UFCHECK)) {
			if (fm_test_hi_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (sc_tact_freesamemseq == ON) {
			goto fm_attach_mseq_exit;
		}
		msp->ps_flag |= B_PS_SAMEPHASE;
	}
	if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && IS_HI_PC(msp->pc)) {
		if (fm_test_yo_matemseq(msp) == RC_ESCAPE) {
			goto fm_attach_mseq_exit;
		}
		msp->ps_flag |= B_PS_MATE;
		mseq_head[msp->depth].flag |= B_MH_MATE;
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_attach_mseq_nfmser(mseq_t *msp)
{
	mseq_t *msq;
	mseq_t *rc;

	if (fm_test_move(msp) == RC_ILLEGAL) {
		goto fm_attach_mseq_exit3;
	}
	mseq_head[msp->depth].all_move_count++;
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (flag_docheck == ON) {
		if ((msp->ps_flag & B_PS_YO_CHECK) || NOT(msp->ps_flag & B_PS_HI_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
		if (msp->ps_flag & B_PS_UFCHECK) {
			if (fm_test_yo_matemseq(msp) == RC_MATE) {
				msp->ps_flag |= B_PS_ESCAPE | B_PS_UFMATE;
			}
		}
	} else {
		if (msp->ps_flag & (B_PS_HI_CHECK | B_PS_YO_CHECK)) {
			goto fm_attach_mseq_exit2;
		}
	}
	mseq_head[msp->depth].legal_move_count++;
	fm_calc_hash(msp);
	rc = fm_test_hash(msp);
	if (rc != NULL) {
		if (rc->depth < msp->depth || rc->parp == msp->parp) {
			goto fm_attach_mseq_exit;
		}
		if (sc_tact_freesamemseq == ON) {
			goto fm_attach_mseq_exit;
		}
		msp->ps_flag |= B_PS_SAMEPHASE;
	}
	if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && flag_docheck == ON) {
		if (fm_test_yo_matemseq(msp) == RC_ESCAPE) {
			goto fm_attach_mseq_exit;
		}
		msp->ps_flag |= B_PS_MATE;
		mseq_head[msp->depth].flag |= B_MH_MATE;
	}
	if (msp->to == msp->parp->to) {
		msp->mv_flag |= B_MV_CAPTIT;
	}
	mseq_head[msp->depth].new_phase_count++;
	msq = fm_get_mseq(msp->depth);
	*msq = *msp;
	if (msq->ps_flag & B_PS_SAMEPHASE) {
		msq->SPHP = rc;
	} else {
		if (msp->ps_flag & B_PS_UFMATE) {
			msq->BDP = NULL;
		} else {
			msq->BDP = fm_get_board(msp->depth);
			*msq->BDP = *msp->BDP;
		}
		mseq_head[msp->depth].unique_phase_count++;
		fm_attach_hash(msq);
	}
	msq->brop = mseq_head[msq->depth].brop;
	mseq_head[msq->depth].brop = msq;
	msq->parp->ref_count++;
fm_attach_mseq_exit:;
	phase_count++;
	if (phase_count >= check_point) {
		fm_mile_stone(msp->depth);
	}
	fm_back_hash(msp);
fm_attach_mseq_exit2:;
	fm_back_phase(msp);
fm_attach_mseq_exit3:;
	return;
}

void
fm_free_mseq(long depth, mseq_t *msp)
{
	mseq_t *msq, *msq_prev;

	msq_prev = NULL;
	for (msq = mseq_head[depth].brop; msq != NULL; msq = msq->brop) {
		if (msq == msp) {
			if (NOT(msq->ps_flag & B_PS_SAMEPHASE) && msq->BDP != NULL) {
				FREE_BOARD(msq->BDP);
			}
			msq->parp->ref_count--;
			if (msq_prev == NULL) {
				mseq_head[depth].brop = msq->brop;
			} else {
				msq_prev->brop = msq->brop;
			}
			FREE_MSEQ(msq);
			break;
		}
		msq_prev = msq;
	}
	return;
}

void
fm_free_mseq_depthescape(long depth, mseq_t **hashhead)
{
	mseq_t *msp, *msp_brop, *msp_prev;

	msp_prev = NULL;
	for (msp = mseq_head[depth].brop; msp != NULL; msp = msp_brop) {
		msp_brop = msp->brop;
		if ((msp->ps_flag & B_PS_ESCAPE)
			|| ((msp->ps_flag & B_PS_SAMEPHASE) && (msp->SPHP->ps_flag & B_PS_ESCAPE)))
		{
			if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && msp->BDP != NULL) {
				FREE_BOARD(msp->BDP);
			}
			fm_free_hash(msp, hashhead);
			msp->parp->ref_count--;
			FREE_MSEQ(msp);
			if (msp_prev == NULL) {
				mseq_head[depth].brop = msp_brop;
			} else {
				msp_prev->brop = msp_brop;
			}
		} else {
			msp_prev = msp;
		}
	}
	return;
}

void
fm_free_mseq_depth(long depth, mseq_t **hashhead)
{
	mseq_t *msp, *msp_brop;

	for (msp = mseq_head[depth].brop; msp != NULL; msp = msp_brop) {
		msp_brop = msp->brop;
		if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && msp->BDP != NULL) {
			FREE_BOARD(msp->BDP);
		}
		fm_free_hash(msp, hashhead);
		msp->parp->ref_count--;
		FREE_MSEQ(msp);
	}
	mseq_head[depth].brop = NULL;
	mseq_head[depth].all_move_count = 0;
	mseq_head[depth].legal_move_count = 0;
	mseq_head[depth].new_phase_count = 0;
	mseq_head[depth].unique_phase_count = 0;
	return;
}

int
fm_get_rule_data(char *buf, long size, FILE *Fp)
{
	char *s, *t;
	int n;
	char buf2[32];

	FOREVER() {
		*buf = '\0';
		//!英小文字を保存するため、fm_fgets()を使用していない。
		if (fgets(buf, size, Fp) == NULL) {
			return 0;
		}
		fm_ltrim(buf);
		strncpy(buf2, buf, strlen(str[STR_SECT_PROBLEM_E]));
		fm_str_toupper(buf2);
		if (NOT(EQUALS(buf, str[STR_SECT_PROBLEM]))
			&& NOT(EQUALS(buf2, str[STR_SECT_PROBLEM_E])))
		{
			break;
		}
	}
	FOREVER() {
		s = strchr(buf, ',');
		if (s == NULL) {
			break;
		}
		if (isdigit((uchar_t)*(s + 1))
			&& isdigit((uchar_t)*(s + 2))
			&& isdigit((uchar_t)*(s + 3)))
		{
			memmove(s, s + 1, strlen(s));
		} else {
			*s = '\x07';
		}
	}
	s = strchr(buf, '\n');
	if (s != NULL) {
		*s = '\0';
		fm_rtrim(buf);
		s = STR_END(buf);
		if (*s == '\x07') {
			;
		} else if (*s == ',') {
			*s = '\x07';
		} else if (*s == '\\' && nthctype(buf, (int)(s - 1 - buf)) != CT_KJ1) {
			*s = '\0';
			fm_rtrim(buf);
		} else {
			*(s + 1) = '\x07';
			*(s + 2) = '\0';
		}
	}
	t = buf;
	n = 0;
	FOREVER() {
		s = strchr(t, '\x07');
		if (s == NULL) {
			break;
		}
		n++;
		t = s + 1;
	}
	return n;
}

int
fm_get_phase_data(char *buf, long size, FILE *Fp, int n_given)
{
	char *s, *t;
	int indata_dlmcnt;
	int n;

	n = n_given;
	if (rule == RULE_NFMHMA || rule == RULE_NFMHSMA) {
		indata_dlmcnt = 4;
	} else {
		indata_dlmcnt = 2;
	}
	FOREVER() {
		if (n >= indata_dlmcnt) {
			break;
		}
		t = STR_ENDX1(buf);
		if (fm_fgets(t, (int)(size - (t - buf)), Fp) == -1) {
			break;
		}
		FOREVER() {
			s = strchr(t, ',');
			if (s == NULL) {
				break;
			}
			*s = '\x07';
		}
		s = STR_END(buf);
		if (*s == '\x07') {
			;
		} else if (*s == '\\' && nthctype(buf, (int)(s - 1 - buf)) != CT_KJ1) {
			*s = '\0';
			fm_rtrim(buf);
			continue;
		} else {
			*(s + 1) = '\x07';
			*(s + 2) = '\0';
		}
		t = buf;
		n = 0;
		FOREVER() {
			s = strchr(t, '\x07');
			if (s == NULL) {
				break;
			}
			n++;
			t = s + 1;
		}
	}
	if (n >= indata_dlmcnt) {
		return n;
	} else {
		return 0;
	}
}

int
fm_get_extrapc_data(char *buf, long size, FILE *Fp)
{
	char *s, *t;

	FOREVER() {
		t = STR_ENDX1(buf);
		if (fm_fgets(t, (int)(size - (t - buf)), Fp) == -1) {
			break;
		}
		FOREVER() {
			s = strchr(t, ',');
			if (s == NULL) {
				break;
			}
			*s = '\x07';
		}
		s = STR_END(buf);
		if (*s == '\x07') {
			;
		} else if (*s == '\\' && nthctype(buf, (int)(s - 1 - buf)) != CT_KJ1) {
			*s = '\0';
			fm_rtrim(buf);
			continue;
		} else {
			*(s + 1) = '\x07';
			*(s + 2) = '\0';
		}
		break;
	}
	if (buf[0] == '\0') {
		return 0;
	} else {
		return 1;
	}
}

// make chess-style move string
static char *
make_move_chesstyle(mseq_t *msp)
{
	static char movebuf[EWASIZE];

	movebuf[0] = '\0';
	if (msp->pc == 0x00) {
		return movebuf;
	}
	ASSERT(msp->pc != SQ_ROOM
		&& ((msp->pc >= HI_FU && msp->pc <= HI_RY)
			|| (msp->pc >= YO_FU && msp->pc <= YO_RY)));
	strcpy(movebuf, graph_mv_pc[KIND(msp->pc)]);
	if (msp->from == 0) {
		strcat(movebuf, "-");
	} else {
		sprintf(STR_ENDX1(movebuf), "%s%s", file_num[FILE(msp->from)], rank_num[RANK(msp->from)]);
		if (IS_ANY_PC(msp->capt)) {
			ASSERT((msp->capt >= HI_FU && msp->capt <= HI_RY)
				|| (msp->capt >= YO_FU && msp->capt <= YO_RY));
			sprintf(STR_ENDX1(movebuf), "x%s", graph_mv_pc[KIND(msp->capt)]);
		} else {
			strcat(movebuf, "-");
		}
	}
	sprintf(STR_ENDX1(movebuf), "%s%s", file_num[FILE(msp->to)], rank_num[RANK(msp->to)]);
	if (msp->mv_flag & B_MV_PROMOTE) {
		sprintf(STR_ENDX1(movebuf), "=%s", graph_mv_pc[KIND(msp->pc) | B_PC_PROMOTE]);
	}
	if (msp->mv_flag & B_MV_REBORN) {
		sprintf(STR_ENDX1(movebuf), "(%s%s%s)",
			file_num[FILE(msp->from)], rank_num[RANK(msp->from)], graph_mv_pc[KIND(msp->capt)]);
	}
	return movebuf;
}

// make traditional move string
static char *
make_move_tradstyle(mseq_t *msp, int pos_flag)
{
	const int *vcp;
	int my_dir, other_dir;
	int my_vec, v_f, v_r;
	int turn;
	int i;
	uchar_t pc;
	static char movebuf[EWASIZE];
	static int direct[] = {
		(DIR_JUMP | DIR_LEFT),				// -12
		(DIR_UP | DIR_GO | DIR_LEFT),		// -11
		(DIR_SIDE | DIR_LEFT),				// -10
		(DIR_DOWN | DIR_LEFT),				//  -9
		0, 0, 0, 0, 0, 0, 0,				//  -2,...,-8
		(DIR_UP | DIR_GO | DIR_STRAIGHT),	//  -1
		0,									//   0
		(DIR_DOWN | DIR_STRAIGHT),			//   1
		0, 0, 0, 0, 0, 0,					//   2,...,7
		(DIR_JUMP | DIR_RIGHT),				//   8
		(DIR_UP | DIR_GO | DIR_RIGHT),		//   9
		(DIR_SIDE | DIR_RIGHT),				//  10
		(DIR_DOWN | DIR_RIGHT),				//  11
		0,									//  12
	};

	if (msp->pc == 0x00) {
		movebuf[0] = '\0';
		return movebuf;
	}
	ASSERT(msp->BDP != NULL);
	ASSERT(msp->pc != SQ_ROOM);
	if (msp->mv_flag & B_MV_CAPTIT) {
		if (pos_flag == ON) {
			sprintf(movebuf, "%s%s%s",
				file_num[FILE(msp->to)], rank_num[RANK(msp->to)], str[STR_CAPTIT]);
		} else {
			sprintf(movebuf, "%s", str[STR_CAPTIT]);
		}
	} else {
		sprintf(movebuf, "%s%s", file_num[FILE(msp->to)], rank_num[RANK(msp->to)]);
	}
	strcat(movebuf, graph_mv_pc[KIND(msp->pc)]);
	if (msp->from == 0) {
		pc = KIND(msp->pc);
		vcp = IS_HI_PC(msp->pc)? vec_hi_step[pc]: vec_yo_step[pc];
		for (; vcp[0] != 0; vcp++) {
			if (msp->BDP->board[msp->to - vcp[0]] == msp->pc) {
				strcat(movebuf, str[STR_PLACE]);
			}
		}
		vcp = IS_HI_PC(msp->pc)? vec_hi_run[pc]: vec_yo_run[pc];
		for (; vcp[0] != 0; vcp++) {
			for (i = 1; IS_ROOM(msp->BDP->board[msp->to - vcp[0] * i]); i++)
				;
			if (msp->BDP->board[msp->to - vcp[0] * i] == msp->pc) {
				strcat(movebuf, str[STR_PLACE]);
			}
		}
	} else {
		v_f = FILE(msp->to) - FILE(msp->from);
		v_r = RANK(msp->to) - RANK(msp->from);
		if (v_f == 0) {
			my_vec = (v_r > 0)? 1: -1;
		} else if (v_r == 0) {
			my_vec = (v_f > 0)? 10: -10;
		} else {
			my_vec = (msp->to - msp->from) / abs(v_r);
		}
		turn = IS_HI_PC(msp->pc)? 1: -1;
		my_dir = direct[my_vec * turn + 12];
		pc = KIND(msp->pc);
		other_dir = 0;
		vcp = IS_HI_PC(msp->pc)? vec_hi_step[pc]: vec_yo_step[pc];
		for (; vcp[0] != 0; vcp++) {
			if (vcp[0] != my_vec && msp->BDP->board[msp->to - vcp[0]] == msp->pc) {
				other_dir |= direct[vcp[0] * turn + 12];
			}
		}
		vcp = IS_HI_PC(msp->pc)? vec_hi_run[pc]: vec_yo_run[pc];
		for (; vcp[0] != 0; vcp++) {
			if (vcp[0] != my_vec) {
				for (i = 1; IS_ROOM(msp->BDP->board[msp->to - vcp[0] * i]); i++)
					;
				if (msp->BDP->board[msp->to - vcp[0] * i] == msp->pc) {
					other_dir |= direct[vcp[0] * turn + 12];
				}
			}
		}
		if (other_dir != 0) {
			if (abs(v_r) == 1) {
				my_dir &= ~DIR_GO;
			} else {
				my_dir &= ~DIR_UP;
			}
			if (my_dir & (DIR_UP | DIR_GO)) {
				if (my_dir & DIR_STRAIGHT) {
					if (!(other_dir & DIR_UP)) {
						my_dir &= ~DIR_STRAIGHT;
					}
				} else {
					if (!(other_dir & DIR_UP)) {
						my_dir &= ~(DIR_RIGHT | DIR_LEFT);
					}
				}
				if (!(other_dir & (DIR_DOWN | DIR_SIDE | DIR_JUMP))
					|| ((my_dir & DIR_STRAIGHT) && !(other_dir & DIR_STRAIGHT))
					|| ((my_dir & DIR_RIGHT) && !(other_dir & DIR_RIGHT))
					|| ((my_dir & DIR_LEFT) && !(other_dir & DIR_LEFT)))
				{
					my_dir &= ~(DIR_UP | DIR_GO);
				}
			} else if (my_dir & DIR_SIDE) {
				if (!(other_dir & DIR_SIDE)) {
					my_dir &= ~(DIR_RIGHT | DIR_LEFT);
				}
				if (!(other_dir & (DIR_UP | DIR_DOWN | DIR_JUMP))
					|| ((my_dir & DIR_RIGHT) && !(other_dir & DIR_RIGHT))
					|| ((my_dir & DIR_LEFT) && !(other_dir & DIR_LEFT)))
				{
					my_dir &= ~DIR_SIDE;
				}
			} else if (my_dir & DIR_DOWN) {
				if (my_dir & DIR_STRAIGHT) {
					if (!(other_dir & DIR_DOWN)) {
						my_dir &= ~DIR_STRAIGHT;
					}
				} else {
					if (!(other_dir & DIR_DOWN)) {
						my_dir &= ~(DIR_RIGHT | DIR_LEFT);
					}
				}
				if (!(other_dir & (DIR_UP | DIR_SIDE | DIR_JUMP))
					|| ((my_dir & DIR_STRAIGHT) && !(other_dir & DIR_UP))
					|| ((my_dir & DIR_RIGHT) && !(other_dir & DIR_RIGHT))
					|| ((my_dir & DIR_LEFT) && !(other_dir & DIR_LEFT)))
				{
					my_dir &= ~DIR_DOWN;
				}
			} else if (my_dir & DIR_JUMP) {
				if (!(other_dir & DIR_JUMP)) {
					my_dir &= ~(DIR_RIGHT | DIR_LEFT);
				}
				if (!(other_dir & (DIR_UP | DIR_SIDE | DIR_DOWN))) {
					my_dir &= ~DIR_JUMP;
				}
			}
			if (my_dir & DIR_RIGHT) {
				strcat(movebuf, str[STR_RIGHT]);
			} else if (my_dir & DIR_LEFT) {
				strcat(movebuf, str[STR_LEFT]);
			} else if (my_dir & DIR_STRAIGHT) {
				strcat(movebuf, str[STR_STRAIGHT]);
			}
			if (my_dir & DIR_GO) {
				strcat(movebuf, str[STR_GO]);
			} else if (my_dir & DIR_UP) {
				strcat(movebuf, str[STR_UP]);
			} else if (my_dir & DIR_SIDE) {
				strcat(movebuf, str[STR_SIDE]);
			} else if (my_dir & DIR_DOWN) {
				strcat(movebuf, str[STR_DOWN]);
			} else if (my_dir & DIR_JUMP) {
				strcat(movebuf, str[STR_JUMP]);
			}
		}
		if (msp->mv_flag & B_MV_PROMOTE) {
			strcat(movebuf, str[STR_PROMOTE]);
		} else if (KIND(msp->pc) <= PC_HI && KIND(msp->pc) != PC_KI) {
			if (IS_HI_PC(msp->pc)) {
				if (RANK(msp->from) <= 3 || RANK(msp->to) <= 3) {
					strcat(movebuf, str[STR_RAW]);
				}
			} else {
				if (RANK(msp->from) >= 7 || RANK(msp->to) >= 7) {
					strcat(movebuf, str[STR_RAW]);
				}
			}
		}
		if (msp->mv_flag & B_MV_REBORN) {
			sprintf(STR_ENDX1(movebuf), "/%s%s%s", file_num[FILE(msp->from)],
				rank_num[RANK(msp->from)], graph_mv_pc[KIND(msp->capt)]);
		}
	}
	return movebuf;
}

static char *
make_move_nfmstyle(mseq_t *msp, int pos_flag)
{
	static char movebuf[EWASIZE];

	movebuf[0] = '\0';
	if (msp->pc == 0x00) {
		return movebuf;
	}
	ASSERT(msp->pc != SQ_ROOM
		&& ((msp->pc >= HI_FU && msp->pc <= HI_RY)
			|| (msp->pc >= YO_FU && msp->pc <= YO_RY)));
	if (msp->mv_flag & B_MV_CAPTIT) {
		if (pos_flag == ON) {
			sprintf(movebuf, "%s%s", file_num[FILE(msp->to)], rank_num[RANK(msp->to)]);
		}
		strcat(movebuf, "同");
	} else {
		sprintf(movebuf, "%s%s", file_num[FILE(msp->to)], rank_num[RANK(msp->to)]);
	}
	strcat(movebuf, graph_mv_pc[KIND(msp->pc)]);
	if (msp->from != 0) {
		if (msp->mv_flag & B_MV_PROMOTE) {
			sprintf(STR_ENDX1(movebuf), "=%s", graph_mv_pc[KIND(msp->pc) | B_PC_PROMOTE]);
		}
		if (IS_ANY_PC(msp->capt)) {
			ASSERT((msp->capt >= HI_FU && msp->capt <= HI_RY && msp->capt != HI_OU)
				|| (msp->capt >= YO_FU && msp->capt <= YO_RY && msp->capt != YO_GY));
			sprintf(STR_ENDX1(movebuf), "x%s", graph_mv_pc[KIND(msp->capt)]);
		}
		if (msp->mv_flag & B_MV_REBORN) {
			strcat(movebuf, "/");
		} else {
			strcat(movebuf, "<");
		}
		sprintf(STR_ENDX1(movebuf), "%s%s",
			file_num[FILE(msp->from)], rank_num[RANK(msp->from)]);
		if (msp->mv_flag & B_MV_REBORN) {
			sprintf(STR_ENDX1(movebuf), "%s", graph_mv_pc[KIND(msp->capt)]);
		}
	}
	return movebuf;
}

// make move string
char *
fm_make_move(mseq_t *msp, int pos_flag)
{
	if (sc_print_chesstyle == ON) {
		return make_move_chesstyle(msp);
	} else if (sc_print_tradstyle == ON) {
		return make_move_tradstyle(msp, pos_flag);
	} else {
		return make_move_nfmstyle(msp, pos_flag);
	}
}

char *
fm_make_matedepth(mseq_t *msp)
{
	uchar_t pc;
	static char matebuf[EWASIZE];

	ASSERT(msp != NULL);
	switch (rule) {
	case RULE_NFMHM:
	case RULE_NFMHMA:
	case RULE_NFMHSM:
	case RULE_NFMHSMA:
	case RULE_NFMSERM:
	case RULE_NFMHA:
		sprintf(matebuf, str[STR_MATE], fm_format_num(editbuf, msp->depth));
		break;
	case RULE_NFMSHM:
		sprintf(matebuf, str[STR_MATE_SH], fm_format_num(editbuf, msp->depth - 1));
		break;
	}
	if (!(rule & RULE_TARGETFIG) || fm_test_targetmseq(msp) == RC_ESCAPE) {
		if (msp->hi_hand != 0) {
			strcat(matebuf, " ");
			for (pc = PC_HI; pc >= PC_FU; pc--) {
				if (msp->BDP->hi_hand[pc] >= 1) {
					strcat(matebuf, graph_pc[pc]);
					if (msp->BDP->hi_hand[pc] >= 2) {
						sprintf(STR_ENDX1(matebuf), "%d", msp->BDP->hi_hand[pc]);
					}
				}
			}
			strcat(matebuf, str[STR_PCLEFT]);
		}
	}
	return matebuf;
}

int
fm_test_normalpieceset(board_t *bdp)
{
	int p_f, pos;
	uchar_t pc;
	uchar_t pieceset[HAND_SIZE];

	memset(pieceset, 0, sizeof(pieceset));
	for (pc = PC_FU; pc <= PC_HI; pc++) {
		pieceset[pc] = bdp->hi_hand[pc] + bdp->yo_hand[pc];
	}
	for (p_f = 1; p_f <= 9; p_f++) {
		for (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			if (IS_ANY_PC(bdp->board[pos]) && KIND(bdp->board[pos]) != PC_OU) {
				pieceset[RAW_KIND(bdp->board[pos])]++;
			}
		}
	}
	return memcmp(pieceset, yo_hand_ini, sizeof(pieceset));
}

void
fm_set_constant(void)
{
	if (rule == RULE_NFMSHM) {
		depth_rsv = 3;
		str_mate_depth = STR_MATE_DEPTH_SH;
	} else {
		depth_rsv = 2;
		str_mate_depth = STR_MATE_DEPTH;
	}
	switch (rule) {
	case RULE_NFMHM:
	case RULE_NFMSERM:
		one_line_title_limit = 60;
		break;
	case RULE_NFMHSM:
	case RULE_NFMSHM:
	case RULE_NFMHA:
		one_line_title_limit = 56;
		break;
	case RULE_NFMHMA:
	case RULE_NFMHSMA:
		one_line_title_limit = 48;
		break;
	}
	switch (rule) {
	case RULE_NFMHM:
		fm_attach_mseq = fm_attach_mseq_nfmh;
		break;
	case RULE_NFMHMA:
		fm_attach_mseq = fm_attach_mseq_nfmha;
		break;
	case RULE_NFMHSM:
		fm_attach_mseq = fm_attach_mseq_nfmhs;
		break;
	case RULE_NFMHSMA:
	case RULE_NFMHA:
		fm_attach_mseq = fm_attach_mseq_nfmhsa;
		break;
	case RULE_NFMSHM:
		fm_attach_mseq = fm_attach_mseq_nfmsh;
		break;
	case RULE_NFMSERM:
		fm_attach_mseq = fm_attach_mseq_nfmser;
		break;
	}
	return;
}

void
fm_mile_stone(long depth)
{
	time_t old_total_elapse;
	int c;

	while (KBHIT()) {
		c = GETCH();
		switch (c) {
		case 0x03:	// ^C
			fm_sigexit(SIGINT);
			break;
		case 0x11:	// ^Q
			fm_sigexit(SIGQUIT);
			break;
		}
	}
	old_total_elapse = total_elapse;
	total_elapse = time(NULL) - start_time;
	if (total_elapse - old_total_elapse > 0) {
		fm_put_record(depth);
	}
	if (phase_count >= check_point) {
		check_point += check_interval;
	}
	return;
}

char *
fm_format_num(char *s, ulong_t n)
{
	int len;
	int i;

	ASSERT(s != NULL);
	sprintf(s, "%lu", n);
	len = (int)strlen(s);
	for (i = len - 3; i > 0; i -= 3) {
		memmove(s + i + 1, s + i, len - i + 1);
		*(s + i) = ',';
		len++;
	}
	return s;
}

char *
fm_format_bignum(char *s, ulonglong_t n)
{
	int len;
	int i;

	ASSERT(s != NULL);
	sprintf(s, "%llu", n);
	len = (int)strlen(s);
	for (i = len - 3; i > 0; i -= 3) {
		memmove(s + i + 1, s + i, len - i + 1);
		*(s + i) = ',';
		len++;
	}
	return s;
}

// left trimming
void
fm_ltrim(char *s)
{
	char *t;

	ASSERT(s != NULL);
	for (t = s; *t != '\0'; t++) {
		if (isspace((uchar_t)*t)) {
			;
		} else if (EQUALZ(t, "　")) {
			t++;
		} else {
			break;
		}
	}
	if (t != s) {
		memmove(s, t, strlen(t) + 1);
	}
	return;
}

// right trimming
void
fm_rtrim(char *s)
{
	char *t;

	ASSERT(s != NULL);
	for (t = STR_END(s); t >= s; t--) {
		if (isspace((uchar_t)*t)) {
			*t = '\0';
		} else if (EQUAL(t - 1, "　")) {
			t--;
			*t = '\0';
		} else {
			break;
		}
	}
	return;
}

// chage to uppercase for ASCII char.
void
fm_str_toupper(char *s)
{
	char *t;

	for (t = s; *t != '\0'; t++) {
		if (_ismbblead((uchar_t)*t)) {
			t++;
		} else {
			*t = toupper((uchar_t)*t);
		}
	}
	return;
}

long
fm_strtol(const char *buf, char **next, int base)
{
	char *s, *t;
	long n;
	char work[EWASIZE];

	strcpy(work, buf);
	for (s = work; *s != '\0' && (isdigit((uchar_t)*s) || *s == ','); s++) {
		if (*s == ',') {
			memmove(s, s + 1, strlen(s));
			s--;
		}
	}
	n = strtol(work, &t, base);
	if (next != NULL) {
		*next = (char *)buf;
		for (s = work; s != t; s++) {
			(*next)++;
			if (**next == ',') {
				(*next)++;
			}
		}
	}
	return n;
}

ulong_t
fm_strtoul(const char *buf, char **next, int base)
{
	char *s, *t;
	ulong_t n;
	char work[EWASIZE];

	strcpy(work, buf);
	for (s = work; *s != '\0' && (isdigit((uchar_t)*s) || *s == ','); s++) {
		if (*s == ',') {
			memmove(s, s + 1, strlen(s));
			s--;
		}
	}
	n = strtoul(work, &t, base);
	if (next != NULL) {
		*next = (char *)buf;
		for (s = work; s != t; s++) {
			(*next)++;
			if (**next == ',') {
				(*next)++;
			}
		}
	}
	return n;
}

