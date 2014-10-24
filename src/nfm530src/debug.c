// nfm/nfm/debug.c -- debug routines
// Copyright(c) Jiro Kamina 2006-2007.  All rights reserved.
// $Header: debug.cv  1.3  07/10/20 22:44:54  Exp $

#include	<stdlib.h>
#include	<string.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
#include	"message.h"

#if 0
static char *
dbg_ps_flag(mseq_t *msp)
{
	static char buf[128];

	buf[0] = '\0';
	if (msp->ps_flag & B_PS_HI_CHECK) {
		strcat(buf, "攻方王手 ");
	}
	if (msp->ps_flag & B_PS_HI_STEPCHECK) {
		strcat(buf, "攻方近接王手 ");
	}
	if (msp->ps_flag & B_PS_HI_RUNCHECK) {
		strcat(buf, "攻方遠隔王手 ");
	}
	if (msp->ps_flag & B_PS_HI_DBLCHECK) {
		strcat(buf, "攻方両王手 ");
	}
	if (msp->ps_flag & B_PS_YO_CHECK) {
		strcat(buf, "受方王手 ");
	}
	if (msp->ps_flag & B_PS_YO_STEPCHECK) {
		strcat(buf, "受方近接王手 ");
	}
	if (msp->ps_flag & B_PS_YO_RUNCHECK) {
		strcat(buf, "受方遠隔王手 ");
	}
	if (msp->ps_flag & B_PS_YO_DBLCHECK) {
		strcat(buf, "受方両王手 ");
	}
	if (msp->ps_flag & B_PS_MATE) {
		strcat(buf, "詰 ");
	}
	if (msp->ps_flag & B_PS_ESCAPE) {
		strcat(buf, "逃れ ");
	}
	if (msp->ps_flag & B_PS_UFMATE) {
		strcat(buf, "打歩詰 ");
	}
	if (msp->ps_flag & B_PS_UFCHECK) {
		strcat(buf, "打歩王手 ");
	}
	if (msp->ps_flag & B_PS_SAMEPHASE) {
		strcat(buf, "同一局面 ");
	}
	if (msp->ps_flag & B_PS_SAMEBOARD) {
		strcat(buf, "同一盤面 ");
	}
	return buf;
}

void
dbg_mseq(int mode, mseq_t *msp)
{
	int save_sc_print_tradstyle;

	save_sc_print_tradstyle = sc_print_tradstyle;
	sc_print_tradstyle = OFF;
	sprintf(outbuf, "self: %p\nparp: %p, brop: %p, amip: %p, BDP/SPHP: %p\n",
		msp, msp->parp, msp->brop, msp->amip, msp->BDP);
	if (mode == B_PM_DISPLAY) {
		fm_put_display(MSG_DIRECT, outbuf);
	} else if (mode == B_PM_FILE) {
		fm_put_file(MSG_DIRECT, outbuf);
	} else {
		fm_put_both(MSG_DIRECT, outbuf);
	}
	sprintf(outbuf, "hi_hash: %08x, yo_hash: %08x, hi_hand: %08x\n"
		"ps_flag: %04x, ref_count: %d, mv_flag: %02x, %s\n"
		"depth: %d, pc: %02x, from: %d%d, to: %d%d, capt: %02x  %s\n",
		msp->hi_hash, msp->yo_hash, msp->hi_hand,
		msp->ps_flag, msp->ref_count, msp->mv_flag, dbg_ps_flag(msp),
		msp->depth, msp->pc,
		(msp->from == 0)? 0: FILE(msp->from), (msp->from == 0)? 0: RANK(msp->from),
		(msp->to == 0)? 0: FILE(msp->to), (msp->to == 0)? 0: RANK(msp->to),
		msp->capt, fm_make_move(msp, ON));
	if (mode == B_PM_DISPLAY) {
		fm_put_display(MSG_DIRECT, outbuf);
		fm_put_display(MSG_DIRECT, "\n");
	} else if (mode == B_PM_FILE) {
		fm_put_file(MSG_DIRECT, outbuf);
		fm_put_file(MSG_DIRECT, "\n");
	} else {
		fm_put_both(MSG_DIRECT, outbuf);
		fm_put_both(MSG_DIRECT, "\n");
	}
	if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && msp->BDP != NULL) {
		//fm_put_phase(mode, msp->BDP);
		dbg_fire(mode, msp->BDP);
		if (mode == B_PM_DISPLAY) {
			fm_put_display(MSG_DIRECT, "\n");
		} else if (mode == B_PM_FILE) {
			fm_put_file(MSG_DIRECT, "\n");
		} else {
			fm_put_both(MSG_DIRECT, "\n");
		}
	}
	sc_print_tradstyle = save_sc_print_tradstyle;
}

void
dbg_mseq_list(int mode, mseq_t *msh)
{
	mseq_t *msp;

	CLEAR_LINE();
	for (msp = msh; msp != NULL; msp = msp->brop) {
		dbg_mseq(mode, msp);
	}
}

void
dbg_hash_list(int mode)
{
	mseq_t *msp;
	long index;

	for (index = 0; index < DEFAULT_HASHHEAD_COUNT; index++) {
		if (hashhead_check[index] != NULL) {
			fm_put_both(MSG_DIRECT, "index: %d\n", index);
			for (msp = hashhead_check[index]; msp != NULL; msp = msp->amip) {
				dbg_mseq(mode, msp);
			}
		}
	}
	for (index = 0; index < DEFAULT_HASHHEAD_COUNT; index++) {
		if (hashhead_escape[index] != NULL) {
			fm_put_both(MSG_DIRECT, "index: %d\n", index);
			for (msp = hashhead_escape[index]; msp != NULL; msp = msp->amip) {
				dbg_mseq(mode, msp);
			}
		}
	}
}

void
dbg_mseqhead(int mode, long depth_start, long depth_end)
{
	long depth;

	for (depth = depth_start; depth <= depth_end + 1; depth++) {
//		sprintf(outbuf, "brop: %p, count: %d, flag: %08x\n",
//			mseq_head[depth].brop, mseq_head[depth].count, mseq_head[depth].flag);
		if (mseq_head[depth].all_move_count > 0
			|| mseq_head[depth].legal_move_count > 0
			|| mseq_head[depth].new_phase_count > 0
			|| mseq_head[depth].unique_phase_count > 0)
		{
			sprintf(outbuf, "%5d: %9d %9d %9d %9d\n",
				depth,
				mseq_head[depth].all_move_count, mseq_head[depth].legal_move_count,
				mseq_head[depth].new_phase_count, mseq_head[depth].unique_phase_count);
			if (mode == B_PM_DISPLAY) {
				fm_put_display(MSG_DIRECT, outbuf);
			} else if (mode == B_PM_FILE) {
				fm_put_file(MSG_DIRECT, outbuf);
			} else {
				fm_put_both(MSG_DIRECT, outbuf);
			}
		}
		if (depth > 1 && mseq_head[depth - 1].flag & B_MH_MATE) {
			break;
		}
	}
	if (mode == B_PM_DISPLAY) {
		fm_put_display(MSG_DIRECT, "\n");
	} else if (mode == B_PM_FILE) {
		fm_put_file(MSG_DIRECT, "\n");
	} else {
		fm_put_both(MSG_DIRECT, "\n");
	}
}

void
dbg_fire(int mode, board_t *bdp)
{
	int p_f, p_r;		// position = (file, rank)
	uchar_t pc, sq;

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
	fm_put_oneway(MSG_DIRECT, outbuf);
	fm_put_oneway(MSG_DIRECT, str[STR_FILENUM]);
	fm_put_oneway(MSG_DIRECT, "\n");
	fm_put_oneway(MSG_DIRECT, "+---------------------------+\n");
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
		strcat(outbuf, "  ");
		for (p_f = 9; p_f >= 1; p_f--) {
			sprintf(STR_ENDX1(outbuf), "%x ", (bdp->fire[POS(p_f, p_r)] & 0xf0) >> 4);
		}
		strcat(outbuf, "  ");
		for (p_f = 9; p_f >= 1; p_f--) {
			sprintf(STR_ENDX1(outbuf), "%x ", (bdp->fire[POS(p_f, p_r)] & 0x0f));
		}
		strcat(outbuf, "\n");
		fm_put_oneway(MSG_DIRECT, outbuf);
	}
	fm_put_oneway(MSG_DIRECT, "+---------------------------+\n");
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
	fm_put_oneway(MSG_DIRECT, outbuf);
}

void
dbg_check_board(void)
{
	board_t *bdp;
	mseq_t *msp;
	long depth;
	long n;

	sprintf(outbuf, "  合計盤面セル数: %11s\n",
		fm_format_num(editbuf, board_inc_count * boardpool_count));
	fm_put_both(MSG_DIRECT, outbuf);
	n = 0;
	for (bdp = bdp_free; bdp != NULL; bdp = bdp->next) {
		n++;
	}
	sprintf(outbuf, "未使用盤面セル数: %11s\n", fm_format_num(editbuf, n));
	fm_put_both(MSG_DIRECT, outbuf);
	n = 0;
	for (depth = 0; depth <= depth_limit + 1; depth++) {
		for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
			if (NOT(msp->ps_flag & B_PS_SAMEPHASE) && msp->BDP != NULL) {
				n++;
			}
		}
	}
	sprintf(outbuf, "  使用盤面セル数: %11s\n", fm_format_num(editbuf, n));
	fm_put_both(MSG_DIRECT, outbuf);
}

void
dbg_check_mseq(void)
{
	mseq_t *msp;
	long depth;
	long n;

	sprintf(outbuf, "  合計手順セル数: %11s\n",
		fm_format_num(editbuf, mseq_inc_count * mseqpool_count));
	fm_put_both(MSG_DIRECT, outbuf);
	n = 0;
	for (msp = msp_free; msp != NULL; msp = msp->brop) {
		n++;
	}
	sprintf(outbuf, "未使用手順セル数: %11s\n", fm_format_num(editbuf, n));
	fm_put_both(MSG_DIRECT, outbuf);
	n = 0;
	for (depth = 0; depth <= depth_limit + 1; depth++) {
		for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
			n++;
		}
	}
	sprintf(outbuf, "  使用手順セル数: %11s\n", fm_format_num(editbuf, n));
	fm_put_both(MSG_DIRECT, outbuf);
}

void
dbg_ref_count(void)
{
	mseq_t *msp, *msq;
	long depth;
	long n;

	fm_put_both(MSG_DIRECT, "\n");
	for (depth = 0; depth <= depth_limit - 1; depth++) {
		for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
			n = 0;
			for (msq = mseq_head[depth + 1].brop; msq != NULL; msq = msq->brop) {
				if (msq->parp == msp) {
					n++;
				}
			}
			sprintf(outbuf, "% 6d: % 3d / % 3d / %s\n", depth, msp->ref_count, n,
				(msp->ref_count != n)? "x": "");
			fm_put_both(MSG_DIRECT, outbuf);
		}
	}
}

void
dbg_phasecount(void)
{
	long depth;

	for (depth = 0; depth <= depth_limit + depth_rsv - 1; depth++) {
		if (mseq_head[depth].all_move_count > 0) {
			break;
		}
	}
	if (depth > depth_limit + 1) {
		return;
	}
	fm_put_both(MSG_DIRECT, "\n");
	fm_put_both(MSG_PHASECOUNTHEAD);
	for (; depth <= depth_limit + depth_rsv - 1; depth++) {
		if (mseq_head[depth].all_move_count == 0) {
			break;
		}
		fm_put_both(MSG_PHASECOUNTLINE, depth,
			fm_format_num(editbuf, mseq_head[depth].all_move_count),
			fm_format_num(editbuf + 16, mseq_head[depth].legal_move_count),
			fm_format_num(editbuf + 32, mseq_head[depth].new_phase_count),
			fm_format_num(editbuf + 48, mseq_head[depth].unique_phase_count));
	}
}

long trace_limit_depth = 10;

void
dbg_trace_mseq(long depth, mseq_t *msp_parp)
{
	mseq_t *msp;
	int x;

	if (depth >= trace_limit_depth) {
		return;
	}
	if (depth == 0) {
		fm_put_file(MSG_DIRECT, "\n");
	}
	depth++;
	for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
		if (msp->parp == msp_parp) {
			x = sc_print_tradstyle;
			sc_print_tradstyle = OFF;
			fm_put_file(MSG_DIRECT, "%*d: %s\n", depth % 50, depth, fm_make_move(msp, ON));
			sc_print_tradstyle = x;
			dbg_trace_mseq(depth, msp);
		}
	}
}
//		fm_put_phase((B_PM_DISPLAY | B_PM_FILE), msp->BDP);
#endif
