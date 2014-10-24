// nfm/nfm/output.c -- output functions
// Copyright(c) KAMINA Jiro 2006-2010.  All lefts reserved.
// $Header$

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdarg.h>
#include	<time.h>
#if defined(_MSC_VER)
#include	<windows.h>
#endif
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
#include	"message.h"

static void
putmsg(int mode, int mi, va_list ap)
{
	if (mi < 0) {
		char *fmt = va_arg(ap, char *);
		if (mode & B_PM_DISPLAY) {
			vfprintf(stdout, fmt, ap);
		}
		if ((mode & B_PM_FILE) && fileFp != NULL) {
			vfprintf(fileFp, fmt, ap);
		}
	} else {
		if (mode & B_PM_DISPLAY) {
			vfprintf(stdout, msg[mi], ap);
		}
		if ((mode & B_PM_FILE) && fileFp != NULL) {
			vfprintf(fileFp, msg[mi], ap);
		}
	}
	return;
}

static void
putmsg2(int mode, int mi, ...)
{
	va_list ap;

	ASSERT(mi < MSG_MSGEND);
	va_start(ap, mi);
	putmsg(mode, mi, ap);
	va_end(ap);
	return;
}

static void
put_option(int mode)
{
	char *s;

	outbuf[0] = '\0';
	strcpy(editbuf, "/T");
	if (sc_tact_freemseq == ON) {
		strcat(editbuf, "E,");
	}
	if (sc_tact_givenmatepath == ON) {
		strcat(editbuf, "M,");
	}
	if (sc_tact_freesamemseq == ON) {
		strcat(editbuf, "S,");
	}
	if (sc_tact_reusemseq == ON) {
		strcat(editbuf, "R,");
	}
	if (sc_tact_sameaslesspc == ON) {
		strcat(editbuf, "H-,");
	}
	if (sc_tact_sameasmorepc == ON) {
		strcat(editbuf, "H+,");
	}
	if (sc_tact_sameaslessfu == ON) {
		strcat(editbuf, "F-,");
	}
	if (sc_tact_sameasmorefu == ON) {
		strcat(editbuf, "F+,");
	}
	if (NOT(EQUAL(editbuf, "/T"))) {
		s = STR_END(editbuf);
		*s = '\0';
		strcat(outbuf, editbuf);
		strcat(outbuf, " ");
	}
	strcpy(editbuf, "/P");
	if (sc_print_loosemate == ON) {
		strcat(editbuf, "L,");
	}
	if (NOT(EQUAL(editbuf, "/P"))) {
		s = STR_END(editbuf);
		*s = '\0';
		strcat(outbuf, editbuf);
		strcat(outbuf, " ");
	}
	strcpy(editbuf, "/E");
	if (real_memsize != 0xffffffff) {
		sprintf(STR_ENDX1(editbuf), "R=%sMB,",
			fm_format_num(editbuf + 96, (ulong_t)DIV_MEGA((ulonglong_t)real_memsize)));
	}
	if (virtual_memsize != 0xffffffff) {
		sprintf(STR_ENDX1(editbuf), "V=%sMB,",
			fm_format_num(editbuf + 96, (ulong_t)DIV_MEGA((ulonglong_t)virtual_memsize)));
	}
	if (NOT(EQUAL(editbuf, "/E"))) {
		s = STR_END(editbuf);
		*s = '\0';
		strcat(outbuf, editbuf);
		strcat(outbuf, " ");
	}
	strcpy(editbuf, "/S");
	if (board_max_count != -1) {
		sprintf(STR_ENDX1(editbuf), "B=%s (%sMB),",
			fm_format_num(editbuf + 96, board_max_count),
			fm_format_num(editbuf + 112, (ulong_t)DIV_MEGA((ulonglong_t)board_max_memsize)));
	}
	if (mseq_max_count != -1) {
		sprintf(STR_ENDX1(editbuf), "M=%s (%sMB),",
			fm_format_num(editbuf + 96, mseq_max_count),
			fm_format_num(editbuf + 112, (ulong_t)DIV_MEGA((ulonglong_t)mseq_max_memsize)));
	}
	if (NOT(EQUAL(editbuf, "/S"))) {
		s = STR_END(editbuf);
		*s = '\0';
		strcat(outbuf, editbuf);
		strcat(outbuf, " ");
	}
	s = STR_END(outbuf);
	*s = '\0';
	if (outbuf[0] != '\0') {
		strcat(outbuf, "\n\n");
		putmsg2(mode, MSG_DIRECT, outbuf);
	}
	return;
}

static void
put_rule_and_depth(int mode)
{
	outbuf[0] = '\0';
	if (title[0] != '\0' && strlen(title) > (size_t)one_line_title_limit) {
		strcat(outbuf, str[STR_LEFTQUOTE]);
		strcat(outbuf, title);
		strcat(outbuf, str[STR_RIGHTQUOTE]);
		strcat(outbuf, "\n\n");
	}
	if (cond & COND_PWC) {
		strcat(outbuf, str[STR_COND_PWC]);
	}
	if (cond & COND_NO_PROM) {
		strcat(outbuf, str[STR_COND_NOPROM]);
	}
	if (rule == RULE_NFMHM) {
		strcat(outbuf, str[STR_HELP_MATE]);
	} else if (rule == RULE_NFMHMA) {
		if (mseq_prob.ps_flag & B_PS_HI_CHECK) {
			strcat(outbuf, str[STR_HELP_MATE_TFIG_YO1ST]);
		} else {
			strcat(outbuf, str[STR_HELP_MATE_TFIG]);
		}
	} else if (rule == RULE_NFMHSM) {
		strcat(outbuf, str[STR_HELP_SELFMATE]);
	} else if (rule == RULE_NFMHSMA) {
		if (mseq_prob.ps_flag & B_PS_HI_CHECK) {
			strcat(outbuf, str[STR_HELP_SELFMATE_TFIG_YO1ST]);
		} else {
			strcat(outbuf, str[STR_HELP_SELFMATE_TFIG]);
		}
	} else if (rule == RULE_NFMSHM) {
		strcat(outbuf, str[STR_SERIES_HELP_MATE]);
	} else if (rule == RULE_NFMSERM) {
		strcat(outbuf, str[STR_SERIES_MATE]);
	} else if (rule == RULE_NFMHA) {
		strcat(outbuf, str[STR_HELP_TARGETFIG]);
	}
	strcat(outbuf, " ");
	sprintf(STR_ENDX1(outbuf), str[str_mate_depth], fm_format_num(editbuf, depth_limit));
	if (title[0] != '\0' && strlen(title) <= (size_t)one_line_title_limit) {
		strcat(outbuf, " ");
		strcat(outbuf, str[STR_LEFTQUOTE]);
		strcat(outbuf, title);
		strcat(outbuf, str[STR_RIGHTQUOTE]);
	}
	strcat(outbuf, "\n");
	putmsg2(mode, MSG_DIRECT, outbuf);
	return;
}

static void
put_phase(int mode, board_t *bdp)
{
	int p_f, p_r;		// position = (file, rank)
	uchar_t pc, sq;

	if (fm_test_normalpieceset(bdp) != 0) {
		strcpy(outbuf, str[STR_HAND]);
		if (memcmp(bdp->yo_hand, hand_void, sizeof(bdp->yo_hand)) == 0) {
			strcat(outbuf, str[STR_NOTHING]);
		} else {
			for (pc = PC_HI; pc >= PC_FU; pc--) {
				if (bdp->yo_hand[pc] >= 1) {
					strcat(outbuf, graph_pc[pc]);
					if (bdp->yo_hand[pc] >= 2) {
						sprintf(STR_ENDX1(outbuf), "%d", bdp->yo_hand[pc]);
					}
					strcat(outbuf, " ");
				}
			}
		}
		strcat(outbuf, "\n");
		putmsg2(mode, MSG_DIRECT, outbuf);
	}
	putmsg2(mode, MSG_DIRECT, str[STR_FILENUM]);
	putmsg2(mode, MSG_DIRECT, "\n");
	putmsg2(mode, MSG_DIRECT, "+---------------------------+\n");
	for (p_r = 1; p_r <= 9; p_r++) {
		strcpy(outbuf, "|");
		for (p_f = 9; p_f >= 1; p_f--) {
			sq = bdp->board[POS(p_f, p_r)];
			if (IS_HI_PC(sq)) {
				strcat(outbuf, graph_hi_pc[KIND(sq)]);
			} else if (IS_YO_PC(sq)) {
				strcat(outbuf, graph_yo_pc[KIND(sq)]);
			} else {
				strcat(outbuf, graph_ex_pc[KIND(sq)]);
			}
		}
		strcat(outbuf, "|");
		strcat(outbuf, board_rank_num[p_r]);
		strcat(outbuf, "\n");
		putmsg2(mode, MSG_DIRECT, outbuf);
	}
	putmsg2(mode, MSG_DIRECT, "+---------------------------+\n");
	strcpy(outbuf, str[STR_HAND]);
	if (memcmp(bdp->hi_hand, hand_void, sizeof(bdp->hi_hand)) == 0) {
		strcat(outbuf, str[STR_NOTHING]);
	} else {
		for (pc = PC_HI; pc >= PC_FU; pc--) {
			if (bdp->hi_hand[pc] >= 1) {
				strcat(outbuf, graph_pc[pc]);
				if (bdp->hi_hand[pc] >= 2) {
					sprintf(STR_ENDX1(outbuf), "%d", bdp->hi_hand[pc]);
				}
				strcat(outbuf, " ");
			}
		}
	}
	strcat(outbuf, "\n");
	putmsg2(mode, MSG_DIRECT, outbuf);
	return;
}

void
fm_put_phase(int mode, board_t *bdp)
{
	put_phase(mode, bdp);
	return;
}

static void
put_phase2(int mode, board_t *bdp, board_t *bdq)
{
	int p_f, p_r;		// position = (file, rank)
	uchar_t pc, sq;

	if (fm_test_normalpieceset(bdp) != 0) {
		strcpy(editbuf, str[STR_HAND]);
		if (memcmp(bdp->yo_hand, hand_void, sizeof(bdp->yo_hand)) == 0) {
			strcat(editbuf, str[STR_NOTHING]);
		} else {
			for (pc = PC_HI; pc >= PC_FU; pc--) {
				if (bdp->yo_hand[pc] >= 1) {
					strcat(editbuf, graph_pc[pc]);
					if (bdp->yo_hand[pc] >= 2) {
						sprintf(STR_ENDX1(editbuf), "%d", bdp->yo_hand[pc]);
					}
					strcat(editbuf, " ");
				}
			}
		}
		sprintf(outbuf, "%s%*c%s", editbuf, 39 - strlen(editbuf), ' ', str[STR_HAND]);
		if (memcmp(bdq->yo_hand, hand_void, sizeof(bdq->yo_hand)) == 0) {
			strcat(outbuf, str[STR_NOTHING]);
		} else {
			for (pc = PC_HI; pc >= PC_FU; pc--) {
				if (bdq->yo_hand[pc] >= 1) {
					strcat(outbuf, graph_pc[pc]);
					if (bdq->yo_hand[pc] >= 2) {
						sprintf(STR_ENDX1(outbuf), "%d", bdq->yo_hand[pc]);
					}
					strcat(outbuf, " ");
				}
			}
		}
		strcat(outbuf, "\n");
		putmsg2(mode, MSG_DIRECT, outbuf);
	}
	putmsg2(mode, MSG_DIRECT, str[STR_FILENUM]);
	putmsg2(mode, MSG_DIRECT, "           ");
	putmsg2(mode, MSG_DIRECT, str[STR_FILENUM]);
	putmsg2(mode, MSG_DIRECT, "\n");
	putmsg2(mode, MSG_DIRECT, "+---------------------------+");
	putmsg2(mode, MSG_DIRECT, "          +---------------------------+\n");
	for (p_r = 1; p_r <= 9; p_r++) {
		strcpy(outbuf, "|");
		for (p_f = 9; p_f >= 1; p_f--) {
			sq = bdp->board[POS(p_f, p_r)];
			if (IS_HI_PC(sq)) {
				strcat(outbuf, graph_hi_pc[KIND(sq)]);
			} else if (IS_YO_PC(sq)) {
				strcat(outbuf, graph_yo_pc[KIND(sq)]);
			} else {
				strcat(outbuf, graph_ex_pc[KIND(sq)]);
			}
		}
		strcat(outbuf, "|");
		strcat(outbuf, board_rank_num[p_r]);
		strcat(outbuf, "        |");
		for (p_f = 9; p_f >= 1; p_f--) {
			sq = bdq->board[POS(p_f, p_r)];
			if (IS_HI_PC(sq)) {
				strcat(outbuf, graph_hi_pc[KIND(sq)]);
			} else if (IS_YO_PC(sq)) {
				strcat(outbuf, graph_yo_pc[KIND(sq)]);
			} else {
				strcat(outbuf, graph_ex_pc[KIND(sq)]);
			}
		}
		strcat(outbuf, "|");
		strcat(outbuf, board_rank_num[p_r]);
		strcat(outbuf, "\n");
		putmsg2(mode, MSG_DIRECT, outbuf);
	}
	putmsg2(mode, MSG_DIRECT, "+---------------------------+");
	putmsg2(mode, MSG_DIRECT, "          +---------------------------+\n");
	strcpy(editbuf, str[STR_HAND]);
	if (memcmp(bdp->hi_hand, hand_void, sizeof(bdp->hi_hand)) == 0) {
		strcat(editbuf, str[STR_NOTHING]);
	} else {
		for (pc = PC_HI; pc >= PC_FU; pc--) {
			if (bdp->hi_hand[pc] >= 1) {
				strcat(editbuf, graph_pc[pc]);
				if (bdp->hi_hand[pc] >= 2) {
					sprintf(STR_ENDX1(editbuf), "%d", bdp->hi_hand[pc]);
				}
				strcat(editbuf, " ");
			}
		}
	}
	sprintf(outbuf, "%s%*c%s", editbuf, 39 - strlen(editbuf), ' ', str[STR_HAND]);
	if (memcmp(bdq->hi_hand, hand_void, sizeof(bdq->hi_hand)) == 0) {
		strcat(outbuf, str[STR_NOTHING]);
	} else {
		for (pc = PC_HI; pc >= PC_FU; pc--) {
			if (bdq->hi_hand[pc] >= 1) {
				strcat(outbuf, graph_pc[pc]);
				if (bdq->hi_hand[pc] >= 2) {
					sprintf(STR_ENDX1(outbuf), "%d", bdq->hi_hand[pc]);
				}
				strcat(outbuf, " ");
			}
		}
	}
	strcat(outbuf, "\n");
	putmsg2(mode, MSG_DIRECT, outbuf);
	return;
}

void
fm_put_both(int mi, ...)
{
	va_list ap;

	ASSERT(mi < MSG_MSGEND);
	va_start(ap, mi);
	putmsg(B_PM_BOTH, mi, ap);
	va_end(ap);
	return;
}

void
fm_put_display(int mi, ...)
{
	va_list ap;

	ASSERT(mi < MSG_MSGEND);
	va_start(ap, mi);
	putmsg(B_PM_DISPLAY, mi, ap);
	va_end(ap);
	return;
}

void
fm_put_file(int mi, ...)
{
	va_list ap;

	ASSERT(mi < MSG_MSGEND);
	va_start(ap, mi);
	putmsg(B_PM_FILE, mi, ap);
	va_end(ap);
	return;
}

void
fm_put_oneway(int mi, ...)
{
	va_list ap;

	ASSERT(mi < MSG_MSGEND);
	va_start(ap, mi);
	if (fileFp != NULL) {
		putmsg(B_PM_FILE, mi, ap);
	} else {
		putmsg(B_PM_DISPLAY, mi, ap);
	}
	va_end(ap);
	return;
}

void
fm_fflush(void)
{
	if (fileFp != NULL) {
		fflush(fileFp);
	}
	return;
}

void
fm_put_problem(int mode)
{
	if (mode & B_PM_FILE) {
		time_t now = time((time_t *)NULL);
		fm_put_file(MSG_START, cmdname, release, ctime(&now));
	}
	put_option(mode);
	put_rule_and_depth(mode);
	putmsg2(mode, MSG_DIRECT, "\n");
	if (rule == RULE_NFMHMA || rule == RULE_NFMHSMA) {
		put_phase2(mode, &board_prob, &board_target);
	} else {
		fm_put_phase(mode, &board_prob);
	}
	putmsg2(mode, MSG_DIRECT, "\n");
	return;
}

static void
put_problem_phase_sub(uchar_t board[], int kind, uchar_t hand[])
{
	size_t line_lim;
	int p_f, p_r, pos, pc;

	strcpy(editbuf, "+");
	for (pc = PC_HI; pc >= PC_FU; pc--) {
		if (hand[pc] >= 1) {
			sprintf(STR_ENDX1(editbuf), "%s", graph_pc[pc]);
			if (hand[pc] >= 2) {
				sprintf(STR_ENDX1(editbuf), "%d", hand[pc]);
			}
		}
	}
	if (EQUAL(editbuf, "+")) {
		editbuf[0] = '\0';
	}
	outbuf[0] = '\0';
	line_lim = 76;
	for (p_r = 1; p_r <= 9; p_r++) {
		for (p_f = 9; p_f >= 1; p_f--) {
			pos = POS(p_f, p_r);
			if (board[pos] & kind) {
				sprintf(STR_ENDX1(outbuf), "%d%s", pos, graph_pc[KIND(board[pos])]);
				if (strlen(outbuf) >= line_lim) {
					strcat(outbuf, "\\\n");
					line_lim += 2 + 76;
				}
			}
		}
	}
	if (outbuf[0] == '\0') {
	// 盤上駒なし
		strcat(strcpy(outbuf, editbuf), "\n");
	} else if (EQUAL(editbuf, "+")) {
	// 持駒なし
		if (EQUAL(STR_END(outbuf) - 1, "\\\n")) {
			strcpy(STR_END(outbuf) - 1, "\n");
		} else {
			strcat(outbuf, "\n");
		}
	} else {
	// 盤上駒あり、持駒あり
		if (NOT(EQUAL(STR_END(outbuf) - 1, "\\\n"))) {
			if (strlen(outbuf) + 1 + strlen(editbuf) >= line_lim) {
				strcat(outbuf, "\\\n");
			} else {
				strcat(outbuf, " ");
			}
		}
		strcat(strcat(outbuf, editbuf), "\n");
	}
	fm_put_file(MSG_DIRECT, outbuf);
	return;
}

static void
put_problem_phase(board_t *bdp)
{
	put_problem_phase_sub(bdp->board, B_PC_HITHER, bdp->hi_hand);
	put_problem_phase_sub(bdp->board, B_PC_YONDER, bdp->yo_hand);
	return;
}

static void
put_hma_problem(board_t *bdp_target)
{
	switch (rule) {
	case RULE_NFMHM:
	case RULE_NFMHMA:
		strcpy(outbuf, "H#@");
		break;
	case RULE_NFMHSM:
	case RULE_NFMHSMA:
		strcpy(outbuf, "HS#@");
		break;
	case RULE_NFMSHM:
	case RULE_NFMSERM:
	case RULE_NFMHA:
		return;
	}
	fm_put_file(MSG_DIRECT, "\n");
	fm_put_file(MSG_DIRECT, str[STR_SECT_PROBLEM]);
	fm_put_file(MSG_DIRECT, "\n");
	strcat(outbuf, fm_format_num(editbuf, depth_start));
	if (cond & COND_PWC) {
		strcat(strcat(outbuf, "/"), str[STR_COND_PWC]);
	}
	if (cond & COND_NO_PROM) {
		strcat(strcat(outbuf, "/"), str[STR_COND_NOPROM]);
	}
	strcat(outbuf, "\n");
	fm_put_file(MSG_DIRECT, outbuf);
	put_problem_phase(&board_prob);
	put_problem_phase(bdp_target);
	fm_put_file(MSG_DIRECT, str[STR_SECT_END]);
	fm_put_file(MSG_DIRECT, "\n");
	return;
}

// put elapse time, search phase count, solution count
void
fm_put_result(long depth, const char *state)
{
	long hour;
	int min, sec;

	hour = (long)(total_elapse / 3600);
	min = (int)((total_elapse % 3600) / 60);
	sec = (int)(total_elapse % 60);
	if (hour > 0) {
		sprintf(outbuf, str[STR_ELAPSE_HMS], hour, min, sec);
	} else if (min > 0) {
		sprintf(outbuf, str[STR_ELAPSE_MS], min, sec);
	} else {
		sprintf(outbuf, str[STR_ELAPSE_S], sec);
	}
	strcat(outbuf, "  ");
	sprintf(STR_ENDX1(outbuf), str[STR_PHASES], fm_format_num(editbuf, phase_count));
	strcat(outbuf, "  ");
	sprintf(STR_ENDX1(outbuf), str[STR_LASTDEPTH], fm_format_num(editbuf, depth));
	if (state != NULL) {
		strcat(outbuf, "  ");
		strcat(outbuf, state);
	}
	strcat(outbuf, "\n");
	CLEAR_LINE();
	fm_put_both(MSG_DIRECT, outbuf);
	return;
}

// put elapse time, search phase count
void
fm_put_record(long depth)
{
	long hour;
	int min, sec;

	hour = (long)(total_elapse / 3600);
	min = (int)((total_elapse % 3600) / 60);
	sec = (int)(total_elapse % 60);
	if (hour > 0) {
		sprintf(outbuf, str[STR_ELAPSE_HMS], hour, min, sec);
	} else if (min > 0) {
		sprintf(outbuf, str[STR_ELAPSE_MS], min, sec);
	} else {
		sprintf(outbuf, str[STR_ELAPSE_S], sec);
	}
	strcat(outbuf, "  ");
	sprintf(STR_ENDX1(outbuf), str[STR_PHASES], fm_format_num(editbuf, phase_count));
	strcat(outbuf, "  ");
	sprintf(STR_ENDX1(outbuf), str[STR_DEPTH], fm_format_num(editbuf, depth));
	strcat(outbuf, "   \b\b\b");
	CARRIAGE_RETURN();
	fm_put_display(MSG_DIRECT, outbuf);
	return;
}

void
fm_put_matemseq(mseq_t *msp_start, mseq_t *msp_mate)
{
	mseq_t *msp;
	int mode;
	int pos_flag;
	int nf;

	if (sc_print_notdisplay == ON) {
		if (fileFp == NULL) {
			mode = B_PM_DISPLAY;
		} else {
			mode = B_PM_FILE;
		}
	} else {
		mode = B_PM_BOTH;
	}
	if (msp_start->depth > 0) {
		pos_flag = ON;
		if ((sc_print_startfig == ON || sc_tact_reusemseq == ON)
			&& msp_start != msp_start_last)
		{
			msp_start_last = msp_start;
			putmsg2(mode, MSG_DIRECT, "\n");
			putmsg2(mode, MSG_FIGTITLE,
				str[STR_STARTFIG], fm_format_num(editbuf, msp_start->depth),
				fm_make_move(msp_start, ON));
			putmsg2(mode, MSG_DIRECT, "\n");
			fm_put_phase(mode, msp_start->BDP);
			if (sc_tact_reusemseq == ON && msp_start->depth == depth_start) {
				put_hma_problem(msp_start->BDP);
			}
		}
	}
	putmsg2(mode, MSG_DIRECT, "\n");
	if (flag_loose_mate == ON) {
		if (msp_start->depth == 0) {
			putmsg2(mode, MSG_LOOSEMATE1ST);
		} else {
			putmsg2(mode, MSG_LOOSEMATE, fm_format_num(editbuf, msp_start->depth + 1));
		}
	} else if (mate_count >= 2) {
		if (msp_start->depth == 0) {
			putmsg2(mode, MSG_ANOTHERMATE1ST);
		} else {
			putmsg2(mode, MSG_ANOTHERMATE, fm_format_num(editbuf, msp_start->depth + 1));
		}
	}
	if (rule == RULE_NFMHMA || rule == RULE_NFMHSMA) {
		if ((msp_mate->ps_flag & B_PS_MATE)
			&& ((rule == RULE_NFMHMA
					&& (msp_mate->depth + depth_limit) % 2 != mask_targetdepth
					&& (msp_mate->ps_flag & B_PS_HI_CHECK)
					&& fm_test_yo_matemseq(msp_mate) == RC_MATE)
				|| (rule == RULE_NFMHSMA
					&& (msp_mate->depth + depth_limit) % 2 == mask_targetdepth
					&& (msp_mate->ps_flag & B_PS_YO_CHECK)
					&& fm_test_hi_matemseq(msp_mate) == RC_MATE)))
		{
			putmsg2(mode, MSG_SHORTMATE);
		}
	}
	if (msp_mate->ps_flag & B_PS_GIVENMATEPATH) {
		putmsg2(mode, MSG_SHORTMATE);
	}
	if (sc_print_tradstyle == ON) {
		if (cond & COND_PWC) {
			nf = 8;
		} else {
			nf = 10;
		}
	} else if (sc_print_chesstyle == ON) {
		if (cond & COND_PWC) {
			nf = 4;
		} else {
			nf = 6;
		}
	} else {
		if (cond & COND_PWC) {
			nf = 6;
		} else {
			nf = 8;
		}
	}
	for (msp = msp_start->amip; msp != NULL; msp = msp->amip) {
		strcpy(outbuf, fm_make_move(msp, pos_flag));
		pos_flag = OFF;
		strcat(outbuf, " ");
		putmsg2(mode, MSG_DIRECT, outbuf);
		if ((msp->depth - msp_start->depth) % nf == 0) {
			putmsg2(mode, MSG_DIRECT, "\n");
		}
		if (msp->ps_flag & B_PS_MATE) {
			strcpy(outbuf, fm_make_matedepth(msp));
			strcat(outbuf, "\n");
			putmsg2(mode, MSG_DIRECT, outbuf);
			if (sc_print_matefig == ON) {
				putmsg2(mode, MSG_DIRECT, "\n");
				if ((rule & RULE_TARGETFIG) && fm_test_targetmseq(msp) == RC_MATE) {
					putmsg2(mode, MSG_FIGTITLE,
						str[STR_TARGETFIG], fm_format_num(editbuf, msp->depth),
						fm_make_move(msp, ON));
				} else {
					putmsg2(mode, MSG_FIGTITLE,
						str[STR_MATEFIG], fm_format_num(editbuf, msp->depth),
						fm_make_move(msp, ON));
				}
				putmsg2(mode, MSG_DIRECT, "\n");
				fm_put_phase(mode, msp->BDP);
			}
			break;
		}
		if (msp->ps_flag & (B_PS_SAMEPHASE | B_PS_GIVENMATEPATH)) {
			strcpy(outbuf, str[STR_JOIN]);
			strcat(outbuf, "\n");
			putmsg2(mode, MSG_DIRECT, outbuf);
			if (sc_print_endfig == ON && msp->BDP != NULL) {
				putmsg2(mode, MSG_DIRECT, "\n");
				putmsg2(mode, MSG_FIGTITLE, str[STR_ENDFIG],
					fm_format_num(editbuf, msp->depth), fm_make_move(msp, ON));
				putmsg2(mode, MSG_DIRECT, "\n");
				fm_put_phase(mode, msp->BDP);
			}
			break;
		}
	}
	if (msp == NULL
		&& (NOT(sc_print_tradstyle == ON && (msp_mate->depth - msp_start->depth) % 10 == 0)
			|| NOT(sc_print_tradstyle == OFF
				&& (msp_mate->depth - msp_start->depth) % 8 == 0)))
	{
		putmsg2(mode, MSG_DIRECT, "\n");
	}
	return;
}

void
fm_put_meminfo(void)
{
#if defined(_MSC_VER)
    MEMORYSTATUS ms;

	fm_put_display(MSG_DIRECT, "\n");
	memset(&ms, '\0', sizeof(ms));
	GlobalMemoryStatus(&ms);
	fm_put_display(MSG_TOTALPHISMEM,
		fm_format_bignum(editbuf, ms.dwTotalPhys),
		fm_format_num(editbuf + 24, (ulong_t)DIV_MEGA((ulonglong_t)ms.dwTotalPhys)));
	fm_put_display(MSG_AVAILPHISMEM,
		fm_format_bignum(editbuf, ms.dwAvailPhys),
		fm_format_num(editbuf + 24, (ulong_t)DIV_MEGA((ulonglong_t)ms.dwAvailPhys)));
	fm_put_display(MSG_TOTALVIRTMEM,
		fm_format_bignum(editbuf, ms.dwTotalVirtual),
		fm_format_num(editbuf + 24, (ulong_t)DIV_MEGA((ulonglong_t)ms.dwTotalVirtual)));
	fm_put_display(MSG_AVAILVIRTMEM,
		fm_format_bignum(editbuf, ms.dwAvailVirtual),
		fm_format_num(editbuf + 24, (ulong_t)DIV_MEGA((ulonglong_t)ms.dwAvailVirtual)));
	fm_put_display(MSG_DIRECT, "\n");
#endif
	return;
}

void
fm_put_lastphasecount(long depth_last)
{
	long depth;

	fm_put_both(MSG_DIRECT, "\n");
	fm_put_both(MSG_PHASECOUNTHEAD);
	for (depth = ((depth_last - 11 < 0)? 0: (depth_last - 11));
		depth <= depth_last + depth_rsv - 1; depth++)
	{
		if (mseq_head[depth].all_move_count == 0) {
			break;
		}
		fm_put_both(MSG_PHASECOUNTLINE, depth,
			fm_format_num(editbuf, mseq_head[depth].all_move_count),
			fm_format_num(editbuf + 16, mseq_head[depth].legal_move_count),
			fm_format_num(editbuf + 32, mseq_head[depth].new_phase_count),
			fm_format_num(editbuf + 48, mseq_head[depth].unique_phase_count));
	}
	return;
}

void
fm_put_hashinfo(void)
{
	fm_put_both(MSG_DIRECT, "\n");
	fm_put_both(MSG_SINFO);
	fm_put_both(MSG_HIHASH);
	fm_put_both(MSG_HIHASHTEST,
		fm_format_num(editbuf, test_hash_check_count),
		fm_format_num(editbuf + 16, found_hash_check_count),
		fm_format_num(editbuf + 32, notfound_hash_check_count));
	fm_put_both(MSG_HIHASHCOMP,
		(comp_hash_check_found + comp_hash_check_notfound) / test_hash_check_count,
		(found_hash_check_count == 0)? 0: (comp_hash_check_found / found_hash_check_count),
		comp_hash_check_notfound / notfound_hash_check_count);
	fm_put_both(MSG_YOHASH);
	fm_put_both(MSG_YOHASHTEST,
		fm_format_num(editbuf, test_hash_escape_count),
		fm_format_num(editbuf + 16, found_hash_escape_count),
		fm_format_num(editbuf + 32, notfound_hash_escape_count));
	fm_put_both(MSG_YOHASHCOMP,
		(comp_hash_escape_found + comp_hash_escape_notfound) / test_hash_escape_count,
		(found_hash_escape_count == 0)? 0: (comp_hash_escape_found / found_hash_escape_count),
		comp_hash_escape_notfound / notfound_hash_escape_count);
	return;
}

void
fm_put_indata(char *buf)
{
	char *s;

	ASSERT(buf != outbuf);
	strcpy(outbuf, buf);
	while ((s = strchr(outbuf, '\07')) != NULL) {
		*s = ',';
	}
	s = STR_END(outbuf);
	if (*s != '\n') {
		strcat(outbuf, "\n");
	}
	fm_put_display(MSG_DIRECT, outbuf);
	return;
}

