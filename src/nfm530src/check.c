// nfm/nfm/check.c -- check processor
// Copyright(c) KAMINA Jiro 2006-2008.  All rights reserved.
// $Header: check.cv  1.3  07/10/20 22:44:41  Exp $

#include	<stdlib.h>
#include	<string.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"

// generate moves to place to the square with the step-vector
static void
gen_p_wsv(mseq_t *msp, int to, int vecidx)
{
	board_t *bdp_par;

	bdp_par = msp->parp->BDP;
	msp->from = 0;
	msp->to = to;
	msp->capt = SQ_ROOM;
	msp->mv_flag = 0x00;
	if (bdp_par->hi_hand[PC_FU] > 0 && hi_step_pc[vecidx][PC_FU] != 0) {
		msp->ps_flag = B_PS_UFCHECK;
		msp->pc = HI_FU;
		fm_attach_mseq(msp);
	}
	if (bdp_par->hi_hand[PC_KE] > 0 && hi_step_pc[vecidx][PC_KE] != 0 && RANK(to) != 1) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_KE;
		fm_attach_mseq(msp);
	}
	if (bdp_par->hi_hand[PC_GI] > 0 && hi_step_pc[vecidx][PC_GI] != 0) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_GI;
		fm_attach_mseq(msp);
	}
	if (bdp_par->hi_hand[PC_KI] > 0 && hi_step_pc[vecidx][PC_KI] != 0) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_KI;
		fm_attach_mseq(msp);
	}
	return;
}

// generate moves to place to the square with the run-vector
static void
gen_p_wrv(mseq_t *msp, int to, int vecidx)
{
	board_t *bdp_par;

	bdp_par = msp->parp->BDP;
	msp->from = 0;
	msp->to = to;
	msp->capt = SQ_ROOM;
	msp->mv_flag = 0x00;
	if (bdp_par->hi_hand[PC_KY] > 0 && hi_run_pc[vecidx][PC_KY] != 0) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_KY;
		fm_attach_mseq(msp);
	}
	if (bdp_par->hi_hand[PC_KA] > 0 && hi_run_pc[vecidx][PC_KA] != 0) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_KA;
		fm_attach_mseq(msp);
	}
	if (bdp_par->hi_hand[PC_HI] > 0 && hi_run_pc[vecidx][PC_HI] != 0) {
		msp->ps_flag = 0x0000;
		msp->pc = HI_HI;
		fm_attach_mseq(msp);
	}
	return;
}

// generate moves to move to the square with the step-vector from anywhere
static void
gen_mt_wsv_fa(mseq_t *msp, int to, int vecidx)
{
	board_t *bdp_par;
	int from;
	int i;
	uchar_t sq;

	bdp_par = msp->parp->BDP;
	msp->to = to;
	if (bdp_par->fire[to] & B_BD_HI_STEPFIRE) {
		for (i = 0; hi_step_vec[i] != 0; i++) {
			from = to - hi_step_vec[i];
			sq = bdp_par->board[from];
			if (IS_HI_PC(sq) && sq != HI_OU) {
				if (hi_step_pc[i][KIND(sq)] != 0) {
					msp->pc = sq;
					msp->from = from;
					msp->capt = bdp_par->board[to];
					if (hi_step_pc[vecidx][KIND(sq)] != 0) {
						msp->mv_flag = 0x00;
						msp->ps_flag = 0x0000;
						fm_attach_mseq(msp);
					}
					if (sq <= HI_GI && (RANK(from) <= 3 || RANK(to) <= 3)
						&& hi_step_pc[vecidx][KIND(sq) | B_PC_PROMOTE] != 0)
					{
						msp->mv_flag = B_MV_PROMOTE;
						msp->ps_flag = 0x0000;
						fm_attach_mseq(msp);
					}
				}
			}
		}
	}
	if (bdp_par->fire[to] & B_BD_HI_RUNFIRE) {
		for (i = 0; run_vec[i] != 0; i++) {
			for (from = to - run_vec[i]; IS_THRUABLE(bdp_par->board[from]); from -= run_vec[i])
				;
			sq = bdp_par->board[from];
			if (IS_HI_PC(sq)) {
				if (hi_run_pc[i][KIND(sq)] != 0) {
					msp->pc = sq;
					msp->from = from;
					msp->capt = bdp_par->board[to];
					if (hi_step_pc[vecidx][KIND(sq)] != 0) {
						msp->mv_flag = 0x00;
						msp->ps_flag = 0x0000;
						fm_attach_mseq(msp);
					}
					if (sq <= HI_HI && (RANK(from) <= 3 || RANK(to) <= 3)
						&& hi_step_pc[vecidx][KIND(sq) | B_PC_PROMOTE] != 0)
					{
						msp->mv_flag = B_MV_PROMOTE;
						msp->ps_flag = 0x0000;
						fm_attach_mseq(msp);
					}
				}
			}
		}
	}
	return;
}

// generate moves to move to the square with the run-vector from anywhere
static void
gen_mt_wrv_fa(mseq_t *msp, int to, int vecidx)
{
	board_t *bdp_par;
	int from;
	int i;
	uchar_t sq;

	bdp_par = msp->parp->BDP;
	msp->to = to;
	if (bdp_par->fire[to] & B_BD_HI_STEPFIRE) {
		for (i = 0; hi_step_vec[i] != 0; i++) {
			from = to - hi_step_vec[i];
			sq = bdp_par->board[from];
			if (IS_HI_PC(sq)) {
				if (hi_step_pc[i][KIND(sq)] != 0 && hi_run_pc[vecidx][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					msp->capt = bdp_par->board[to];
					msp->mv_flag = 0x00;
					fm_attach_mseq(msp);
				}
			}
		}
	}
	if (bdp_par->fire[to] & B_BD_HI_RUNFIRE) {
		for (i = 0; run_vec[i] != 0; i++) {
			for (from = to - run_vec[i]; IS_THRUABLE(bdp_par->board[from]); from -= run_vec[i])
				;
			sq = bdp_par->board[from];
			if (IS_HI_PC(sq)) {
				if (hi_run_pc[i][KIND(sq)] != 0 && hi_run_pc[vecidx][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					msp->capt = bdp_par->board[to];
					msp->mv_flag = 0x00;
					fm_attach_mseq(msp);
					if ((sq == HI_KA || sq == HI_HI) && (RANK(from) <= 3 || RANK(to) <= 3)
						&& hi_run_pc[vecidx][KIND(sq) | B_PC_PROMOTE] != 0)
					{
						msp->ps_flag = 0x0000;
						msp->mv_flag = B_MV_PROMOTE;
						fm_attach_mseq(msp);
					}
				}
			}
		}
	}
	return;
}

// generate moves to move from the square to anywhere
static void
gen_mf_ta(mseq_t *msp, int from)
{
	const int *vcp;
	board_t *bdp_par;
	int to;
	uchar_t pc;

	bdp_par = msp->parp->BDP;
	msp->pc = bdp_par->board[from];
	msp->from = from;
	pc = bdp_par->board[from];
	for (vcp = vec_hi_step[KIND(pc)]; vcp[0] != 0; vcp++) {
		to = from + vcp[0];
		if (IS_HI_MOVABLE(bdp_par->board[to])) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			fm_attach_mseq(msp);
			if (pc <= HI_GI && (RANK(from) <= 3 || RANK(to) <= 3)) {
				msp->ps_flag = 0x0000;
				msp->mv_flag = B_MV_PROMOTE;
				fm_attach_mseq(msp);
			}
		}
	}
	for (vcp = vec_hi_run[KIND(pc)]; vcp[0] != 0; vcp++) {
		for (to = from + vcp[0]; IS_ROOM(bdp_par->board[to]); to += vcp[0]) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			fm_attach_mseq(msp);
			if (pc <= HI_HI && (RANK(from) <= 3 || RANK(to) <= 3)) {
				msp->ps_flag = 0x0000;
				msp->mv_flag = B_MV_PROMOTE;
				fm_attach_mseq(msp);
			}
		}
		if (IS_YO_PC(bdp_par->board[to])) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			fm_attach_mseq(msp);
			if (pc <= HI_HI && (RANK(from) <= 3 || RANK(to) <= 3)) {
				msp->ps_flag = 0x0000;
				msp->mv_flag = B_MV_PROMOTE;
				fm_attach_mseq(msp);
			}
		}
	}
	return;
}

static void
gath_checkmove(mseq_t *msp_par)
{
	board_t *bdp_par;
	int ck_pos, from;
	int ck_vec;
	int i;
	mseq_t mseq;
	board_t board_tmp;
	uchar_t ck_sq;

	mseq.parp = msp_par;
	mseq.brop = NULL;
	mseq.amip = NULL;
	mseq.BDP = &board_tmp;
	mseq.depth = msp_par->depth + 1;
	mseq.hi_hash = msp_par->hi_hash;
	mseq.yo_hash = msp_par->yo_hash;
	mseq.hi_hand = msp_par->hi_hand;
	mseq.ref_count = 0;
	ASSERT(msp_par->BDP != NULL);
	bdp_par = msp_par->BDP;
	memcpy(&board_tmp.board, &bdp_par->board, OFFSET_OF(board_t, fire, board));
	for (i = 0; hi_step_vec[i] != 0; i++) {
		ck_pos = bdp_par->yo_gy_pos - hi_step_vec[i];
		ck_sq = bdp_par->board[ck_pos];
		if (IS_ROOM(ck_sq)) {
			// 王手場所は空である：
			// => 王手場所に王手利きをもつ駒を打つ
			gen_p_wsv(&mseq, ck_pos, i);
			if (bdp_par->fire[ck_pos] & (B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE)) {
				// 王手場所には攻方駒の利きがある：
				// => 王手場所に利きの元になっている駒で王手利きをもつ駒を移動する
				gen_mt_wsv_fa(&mseq, ck_pos, i);
			}
		} else if (IS_YO_PC(ck_sq)) {
			if (bdp_par->fire[ck_pos] & (B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE)) {
				// 王手場所には攻方駒の利きがある：
				// => 王手場所に利きの元になっている駒で王手利きをもつ駒を移動する
				gen_mt_wsv_fa(&mseq, ck_pos, i);
			}
		}
	}
	for (i = 0; run_vec[i] != 0; i++) {
		ck_vec = run_vec[i];
		for (ck_pos = bdp_par->yo_gy_pos - ck_vec; IS_THRUABLE(bdp_par->board[ck_pos]);
			ck_pos -= ck_vec)
		{
			if (IS_ROOM(bdp_par->board[ck_pos])) {
				// 王手場所に攻方駒が移動できる：
				// => 王手場所に王手利きをもつ駒を打つ
				gen_p_wrv(&mseq, ck_pos, i);
				if (bdp_par->fire[ck_pos] & (B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE)) {
					// 王手場所には攻方駒の利きがある：
					// => 王手場所に利きの元になっている駒を移動する
					gen_mt_wrv_fa(&mseq, ck_pos, i);
				}
			}
		}
		ck_sq = bdp_par->board[ck_pos];
		if (IS_YO_PC(ck_sq)) {
			if (bdp_par->fire[ck_pos] & (B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE)) {
				gen_mt_wrv_fa(&mseq, ck_pos, i);
			}
		} else if (IS_HI_PC(ck_sq)) {
			from = ck_pos;
			for (ck_pos = from - ck_vec; IS_THRUABLE(bdp_par->board[ck_pos]); ck_pos -= ck_vec)
				;
			if (IS_HI_PC(bdp_par->board[ck_pos])) {
				if (hi_run_pc[i][KIND(bdp_par->board[ck_pos])] != 0) {
					gen_mf_ta(&mseq, from);
				}
			}
		}
	}
	return;
}

void
fm_check(long depth)
{
	mseq_t *msp_par;
	long save_count;

	switch (rule) {
	case RULE_NFMHM:
	case RULE_NFMHMA:
	case RULE_NFMHA:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_checkmove(msp_par);
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
	case RULE_NFMHSM:
	case RULE_NFMHSMA:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_checkmove(msp_par);
			if (mseq_head[depth + 1].legal_move_count == save_count) {
				if (msp_par->ps_flag & B_PS_YO_CHECK) {
					if (fm_test_hi_matemseq(msp_par) == RC_MATE) {
						msp_par->ps_flag |= B_PS_MATE;
						mseq_head[depth].flag |= B_MH_MATE;
					} else {
						msp_par->ps_flag |= B_PS_ESCAPE;
					}
				} else {
					msp_par->ps_flag |= B_PS_ESCAPE;
				}
			}
			// 2008-03-15 0.22 BF
//			if (!(msp_par->ps_flag & B_PS_MATE)) {
			if (!(msp_par->ps_flag & (B_PS_MATE | B_PS_GIVENMATEPATH))) {
				FREE_BOARD(msp_par->BDP);
			}
		}
		if (mseq_head[depth + 1].legal_move_count == 0 && !(mseq_head[depth].flag & B_MH_MATE)) {
			mseq_head[depth].flag |= B_MH_ESCAPE;
		}
		break;
	case RULE_NFMSHM:
	case RULE_NFMSERM:
		for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
			if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
				continue;
			}
			save_count = mseq_head[depth + 1].legal_move_count;
			gath_checkmove(msp_par);
			if (mseq_head[depth + 1].legal_move_count == save_count && depth == depth_limit) {
				msp_par->ps_flag |= B_PS_ESCAPE;
			}
		}
		if (mseq_head[depth + 1].legal_move_count == 0 && depth == depth_limit) {
			mseq_head[depth].flag |= B_MH_ESCAPE;
		}
		break;
	}
	return;
}

static void
gath_himove(mseq_t *msp_par)
{
	board_t *bdp_par;
	const int *vcp;
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
			if (IS_HI_PC(bdp_par->board[pos])) {
				pc = bdp_par->board[pos];
				for (vcp = vec_hi_step[KIND(pc)]; vcp[0] != 0; vcp++) {
					posxv = pos + vcp[0];
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if ((bdp_par->fire[posxv] & B_BD_HI_STEPFIRE)
						&& IS_HI_MOVABLE(bdp_par->board[posxv])
						&& (pc != HI_OU || !(bdp_par->fire[posxv] & B_BD_YO_FIRE)))
					{
						mseq.ps_flag = 0x0000;
						mseq.pc = pc;
						mseq.from = pos;
						mseq.to = posxv;
						mseq.capt = bdp_par->board[posxv];
						mseq.mv_flag = 0x00;
						fm_attach_mseq(&mseq);
						if (pc <= HI_HI && pc != HI_KI && (RANK(pos) <= 3 || RANK(posxv) <= 3)) {
							mseq.ps_flag = 0x0000;
							mseq.mv_flag = B_MV_PROMOTE;
							fm_attach_mseq(&mseq);
						}
					}
				}
				for (vcp = vec_hi_run[KIND(bdp_par->board[pos])]; vcp[0] != 0; vcp++) {
					for (posxv = pos + vcp[0]; IS_ROOM(bdp_par->board[posxv]); posxv += vcp[0]) {
						ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
						if (bdp_par->fire[posxv] & B_BD_HI_RUNFIRE) {
							mseq.ps_flag = 0x0000;
							mseq.pc = pc;
							mseq.from = pos;
							mseq.to = posxv;
							mseq.capt = SQ_ROOM;
							mseq.mv_flag = 0x00;
							fm_attach_mseq(&mseq);
							if (pc <= HI_HI && pc != HI_KI
								&& (RANK(pos) <= 3 || RANK(posxv) <= 3))
							{
								mseq.ps_flag = 0x0000;
								mseq.mv_flag = B_MV_PROMOTE;
								fm_attach_mseq(&mseq);
							}
						}
					}
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if ((bdp_par->fire[posxv] & B_BD_HI_RUNFIRE)
						&& IS_HI_MOVABLE(bdp_par->board[posxv]))
					{
						mseq.ps_flag = 0x0000;
						mseq.pc = pc;
						mseq.from = pos;
						mseq.to = posxv;
						mseq.capt = bdp_par->board[posxv];
						mseq.mv_flag = 0x00;
						fm_attach_mseq(&mseq);
						if (pc <= HI_HI && pc != HI_KI && (RANK(pos) <= 3 || RANK(posxv) <= 3)) {
							mseq.mv_flag = B_MV_PROMOTE;
							mseq.ps_flag = 0;
							fm_attach_mseq(&mseq);
						}
					}
				}
			} else if (IS_ROOM(bdp_par->board[pos])) {
				for (pc = PC_FU; pc <= PC_HI; pc++) {
					if (bdp_par->hi_hand[pc] > 0) {
						mseq.ps_flag = 0x0000;
						mseq.pc = (uchar_t)pc | B_PC_HITHER;
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
fm_hi_move(long depth)
{
	mseq_t *msp_par;
	long save_count;

	for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
		if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
			continue;
		}
		save_count = mseq_head[depth + 1].legal_move_count;
		gath_himove(msp_par);
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

