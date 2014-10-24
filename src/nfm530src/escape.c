// nfm/nfm/escape.c -- escape processor
// Copyright(c) Jiro Kamina 2006-2007.  All rights reserved.
// $Header: escape.cv  1.3  07/10/20 22:45:01  Exp $

#include	<stdlib.h>
#include	<string.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
//#include	"message.h"

// generate moves to move to the square from anywhere
static void
gen_moveto_fromanywhere(mseq_t *msp, int to)
{
	board_t *bdp_par;
	int from;
	int i;
	uchar_t sq;

	bdp_par = msp->parp->BDP;
	msp->to = to;
	msp->capt = bdp_par->board[to];
	if (bdp_par->fire[to] & B_BD_YO_STEPFIRE) {
		for (i = 0; yo_step_vec[i] != 0; i++) {
			from = to - yo_step_vec[i];
			sq = bdp_par->board[from];
			if (IS_YO_PC(sq) && sq != YO_GY) {
				if (yo_step_pc[i][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					msp->mv_flag = 0x00;
					fm_attach_mseq(msp);
					if (sq <= YO_HI && sq != YO_KI && (RANK(from) >= 7 || RANK(to) >= 7)) {
						msp->ps_flag = 0x0000;
						msp->mv_flag = B_MV_PROMOTE;
						fm_attach_mseq(msp);
					}
				}
			}
		}
	}
	if (bdp_par->fire[to] & B_BD_YO_RUNFIRE) {
		for (i = 0; run_vec[i] != 0; i++) {
			for (from = to - run_vec[i]; IS_THRUABLE(bdp_par->board[from]); from -= run_vec[i])
				;
			sq = bdp_par->board[from];
			if (IS_YO_PC(sq) && sq != YO_GY) {
				if (yo_run_pc[i][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					msp->mv_flag = 0x00;
					fm_attach_mseq(msp);
					if (sq <= YO_HI && sq != YO_KI && (RANK(from) >= 7 || RANK(to) >= 7)) {
						msp->ps_flag = 0;
						msp->mv_flag = B_MV_PROMOTE;
						fm_attach_mseq(msp);
					}
				}
			}
		}
	}
	return;
}

// generate mvoes to YO_GY escape
static void
gen_gy_escape(mseq_t *msp)
{
	const int *vcp;
	board_t *bdp_par;
	int to;

	bdp_par = msp->parp->BDP;
	msp->pc = YO_GY;
	msp->from = bdp_par->yo_gy_pos;
	for (vcp = vec_yo_step[PC_OU]; vcp[0] != 0; vcp++) {
		to = bdp_par->yo_gy_pos + vcp[0];
		if (IS_YO_MOVABLE(bdp_par->board[to]) && !(bdp_par->fire[to] & B_BD_HI_FIRE)) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			fm_attach_mseq(msp);
		}
	}
	return;
}

static void
gath_shut_to(mseq_t *msp, int to)
{
	board_t *bdp_par;
	uchar_t pc;

	bdp_par = msp->parp->BDP;
	msp->from = 0;
	msp->to = to;
	msp->capt = SQ_ROOM;
	msp->mv_flag = 0x00;
	for (pc = PC_FU; pc <= PC_HI; pc++) {
		if (bdp_par->yo_hand[pc] > 0) {
			msp->ps_flag = 0x0000;
			msp->pc = (uchar_t)pc | B_PC_YONDER;
			fm_attach_mseq(msp);
		}
	}
	if (bdp_par->fire[to] != 0) {
		gen_moveto_fromanywhere(msp, to);
	}
	return;
}

static void
gath_escapemove(mseq_t *msp_par)
{
	board_t *bdp_par;
	int to;
	int v_f, v_r, vec;
	mseq_t mseq;
	board_t board_tmp;

	mseq.parp = msp_par;
	mseq.brop = NULL;
	mseq.amip = NULL;
	mseq.BDP = &board_tmp;
	mseq.depth = msp_par->depth + 1;
	mseq.hi_hash = msp_par->hi_hash;
	mseq.yo_hash = msp_par->yo_hash;
	mseq.hi_hand = msp_par->hi_hand;
	mseq.ref_count = 0;
	bdp_par = msp_par->BDP;
	memcpy(&board_tmp.board, &bdp_par->board, OFFSET_OF(board_t, fire, board));
	ASSERT(bdp_par->yo_gy_pos != 0);
	if (!(msp_par->ps_flag & B_PS_HI_DBLCHECK)) {
		if (bdp_par->fire[bdp_par->hi_ck_pos] & (B_BD_YO_STEPFIRE | B_BD_YO_RUNFIRE)) {
			gen_moveto_fromanywhere(&mseq, bdp_par->hi_ck_pos);
		}
	}
	gen_gy_escape(&mseq);
	if (msp_par->ps_flag & B_PS_HI_DBLCHECK) {
		return;
	}
	if (msp_par->ps_flag & B_PS_HI_RUNCHECK) {
		v_f = FILE(bdp_par->yo_gy_pos) - FILE(bdp_par->hi_ck_pos);
		if (v_f > 0) {
			v_f = 1;
		} else if (v_f < 0) {
			v_f = -1;
		}
		v_r = RANK(bdp_par->yo_gy_pos) - RANK(bdp_par->hi_ck_pos);
		if (v_r > 0) {
			v_r = 1;
		} else if (v_r < 0) {
			v_r = -1;
		}
		vec = VEC(v_f, v_r);
		for (to = bdp_par->yo_gy_pos - vec; to != bdp_par->hi_ck_pos; to -= vec) {
			gath_shut_to(&mseq, to);
		}
	}
	return;
}

void
fm_escape(long depth)
{
	mseq_t *msp_par;
	long save_count;

	switch (rule) {
	case RULE_NFMHM:
	case RULE_NFMHMA:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_escapemove(msp_par);
			if (mseq_head[depth + 1].legal_move_count == save_count) {
				msp_par->ps_flag |= B_PS_MATE;
				mseq_head[depth].flag |= B_MH_MATE;
			} else {
				// 2008-03-15 0.22 BF
//				FREE_BOARD(msp_par->BDP);
				if (!(msp_par->ps_flag & B_PS_GIVENMATEPATH)) {
					FREE_BOARD(msp_par->BDP);
				}
			}
		}
		if (mseq_head[depth + 1].legal_move_count == 0 && !(mseq_head[depth].flag & B_MH_MATE)) {
			mseq_head[depth].flag |= B_MH_ESCAPE;
		}
		break;
	case RULE_NFMHSM:
	case RULE_NFMHSMA:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_escapemove(msp_par);
			if (mseq_head[depth + 1].legal_move_count == save_count) {
				msp_par->ps_flag |= B_PS_ESCAPE;
			}
			// 2008-03-15 0.22 BF
//			FREE_BOARD(msp_par->BDP);
			if (!(msp_par->ps_flag & B_PS_GIVENMATEPATH)) {
				FREE_BOARD(msp_par->BDP);
			}
		}
		if (mseq_head[depth + 1].legal_move_count == 0) {
			mseq_head[depth].flag |= B_MH_ESCAPE;
		}
		break;
	case RULE_NFMHA:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_escapemove(msp_par);
			if (mseq_head[depth + 1].legal_move_count == save_count) {
				msp_par->ps_flag |= B_PS_ESCAPE;
			} else {
				// 2008-03-15 0.22 BF
//				FREE_BOARD(msp_par->BDP);
				if (!(msp_par->ps_flag & B_PS_GIVENMATEPATH)) {
					FREE_BOARD(msp_par->BDP);
				}
			}
		}
		if (mseq_head[depth + 1].legal_move_count == 0) {
			mseq_head[depth].flag |= B_MH_ESCAPE;
		}
		break;
	}
	return;
}

static void
gath_yomove(mseq_t *msp_par)
{
	const int *vcp;
	board_t *bdp_par;
	int p_f, pos, posxv;
	mseq_t mseq;
	board_t board_tmp;
	uchar_t pc;

	mseq.parp = msp_par;
	mseq.brop = NULL;
	mseq.amip = NULL;
	mseq.BDP = &board_tmp;
	mseq.depth = msp_par->depth + 1;
	mseq.hi_hash = msp_par->hi_hash;
	mseq.yo_hash = msp_par->yo_hash;
	mseq.hi_hand = msp_par->hi_hand;
	mseq.ref_count = 0;
	bdp_par = msp_par->BDP;
	memcpy(&board_tmp.board, &bdp_par->board, OFFSET_OF(board_t, fire, board));
	for (p_f = 1; p_f <= 9; p_f++) {
		for (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			if (IS_YO_PC(bdp_par->board[pos])) {
				pc = bdp_par->board[pos];
				for (vcp = vec_yo_step[KIND(pc)]; vcp[0] != 0; vcp++) {
					posxv = pos + vcp[0];
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if ((bdp_par->fire[posxv] & B_BD_YO_STEPFIRE)
						&& IS_YO_MOVABLE(bdp_par->board[posxv])
						&& (pc != YO_GY || !(bdp_par->fire[posxv] & B_BD_HI_FIRE)))
					{
						mseq.ps_flag = 0x0000;
						mseq.pc = pc;
						mseq.from = pos;
						mseq.to = posxv;
						mseq.capt = bdp_par->board[posxv];
						mseq.mv_flag = 0x00;
						fm_attach_mseq(&mseq);
						if (pc <= YO_HI && pc != YO_KI && (RANK(pos) >= 7 || RANK(posxv) >= 7)) {
							mseq.ps_flag = 0x0000;
							mseq.mv_flag = B_MV_PROMOTE;
							fm_attach_mseq(&mseq);
						}
					}
				}
				for (vcp = vec_yo_run[KIND(bdp_par->board[pos])]; vcp[0] != 0; vcp++) {
					for (posxv = pos + vcp[0]; IS_ROOM(bdp_par->board[posxv]); posxv += vcp[0]) {
						ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
						if (bdp_par->fire[posxv] & B_BD_YO_RUNFIRE) {
							mseq.ps_flag = 0x0000;
							mseq.pc = pc;
							mseq.from = pos;
							mseq.to = posxv;
							mseq.capt = SQ_ROOM;
							mseq.mv_flag = 0x00;
							fm_attach_mseq(&mseq);
							if (pc <= YO_HI && pc != YO_KI
								&& (RANK(pos) >= 7 || RANK(posxv) >= 7))
							{
								mseq.ps_flag = 0x0000;
								mseq.mv_flag = B_MV_PROMOTE;
								fm_attach_mseq(&mseq);
							}
						}
					}
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if ((bdp_par->fire[posxv] & B_BD_YO_RUNFIRE)
						&& IS_YO_MOVABLE(bdp_par->board[posxv]))
					{
						mseq.ps_flag = 0x0000;
						mseq.pc = pc;
						mseq.from = pos;
						mseq.to = posxv;
						mseq.capt = bdp_par->board[posxv];
						mseq.mv_flag = 0x00;
						fm_attach_mseq(&mseq);
						if (pc <= YO_HI && pc != YO_KI && (RANK(pos) >= 7 || RANK(posxv) >= 7)) {
							mseq.mv_flag = B_MV_PROMOTE;
							mseq.ps_flag = 0;
							fm_attach_mseq(&mseq);
						}
					}
				}
			} else if (IS_ROOM(bdp_par->board[pos])) {
				for (pc = PC_FU; pc <= PC_HI; pc++) {
					if (bdp_par->yo_hand[pc] > 0) {
						mseq.ps_flag = 0x0000;
						mseq.pc = (uchar_t)pc | B_PC_YONDER;
						mseq.from = 0;
						mseq.to = pos;
						mseq.capt = SQ_ROOM;
						mseq.mv_flag = 0x00;
						fm_attach_mseq(&mseq);
					}
				}
			}
		}
	}
	return;
}

void
fm_yo_move(long depth)
{
	mseq_t *msp_par;
	long save_count;

	for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
		if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
			continue;
		}
		save_count = mseq_head[depth + 1].legal_move_count;
		gath_yomove(msp_par);
		if (mseq_head[depth + 1].legal_move_count == save_count) {
			msp_par->ps_flag |= B_PS_ESCAPE;
		}
		FREE_BOARD(msp_par->BDP);
	}
	if (mseq_head[depth + 1].legal_move_count == 0) {
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

