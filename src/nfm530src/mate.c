// nfm/nfm/mate.c -- struct mate sequence
// Copyright(c) KAMINA Jiro 2006-2007.  All rights reserved.
// $Header: mate.cv  1.3  07/10/20 22:47:02  Exp $

#include	<stdlib.h>
#include	<string.h>
#include	"nfm.h"
#include	"macro.h"
#include	"funcdef.h"

void
fm_make_matemseq(long depth_mate)
{
	mseq_t *msp, *msp_brop, *msp_mate, *msq, *msq_prev;
	board_t *sphp_bdp;
	long depth;

	// 詰以外の盤面セルの回収
	for (msp = mseq_head[depth_mate].brop; msp != NULL; msp = msp->brop) {
		if (!(msp->ps_flag & (B_PS_MATE | B_PS_GIVENMATEPATH | B_PS_SAMEPHASE))
			&& msp->BDP != NULL)
		{
			FREE_BOARD(msp->BDP);
		}
	}
	// 奇数深度の手順セルの順序逆転
	for (depth = 1; depth <= depth_mate; depth += 2) {
		for (msp = mseq_head[depth].brop, mseq_head[depth].brop = NULL; msp != NULL;
			msp = msp_brop)
		{
			msp_brop = msp->brop;
			msp->brop = mseq_head[depth].brop;
			mseq_head[depth].brop = msp;
		}
	}
	// 詰手順の印付けと盤面セルの付加と出力
	mate_count = 0;
	msp_start_last = NULL;
	flag_loose_mate = OFF;
	for (msp = mseq_head[depth_mate].brop; msp != NULL; msp = msp->brop) {
		if (((msp->ps_flag & B_PS_MATE) && ((rule & RULE_TARGETFIG) || msp->hi_hand == 0))
			|| (msp->ps_flag & B_PS_GIVENMATEPATH))
		{
			msp_mate = msp;
			msp->mv_flag |= B_MV_MATEPATH;
			msq_prev = NULL;
			for (msq = msp; msq != NULL; msq = msq->parp) {
				msq->amip = msq_prev;
				msq_prev = msq;
				if (msq->parp != NULL) {
					msq->parp->mv_flag |= B_MV_MATEPATH;
					if (msq->parp->BDP == NULL) {
						// 2008-03-15 0.22 BF 仕掛
//						if (msq->BDP == NULL) {
//						}
						msq->parp->BDP = fm_get_board(depth_mate);
						ASSERT(msq->BDP != NULL);
						*msq->parp->BDP = *msq->BDP;
						fm_prev_phase(msq);
					}
				}
			}
			if (msp_mate->ps_flag & B_PS_GIVENMATEPATH) {
				retry = ON;
			}
			mate_count++;
			fm_put_matemseq(msq_prev, msp_mate);
			break;
		}
	}
	// 駒余り詰の場合および余詰の場合の詰手順の印付けと盤面セルの付加と出力
	for (msp = mseq_head[depth_mate].brop; msp != NULL; msp = msp->brop) {
		if (!(msp->ps_flag & (B_PS_MATE | B_PS_GIVENMATEPATH)) || (msp->mv_flag & B_MV_MATEPATH)) {
			continue;
		}
		msp_mate = msp;
		if (mate_count == 0) {
			msp->mv_flag |= B_MV_MATEPATH;
		}
		msq_prev = NULL;
		for (msq = msp; msq != NULL; msq = msq->parp) {
			msq->amip = msq_prev;
			msq_prev = msq;
			if (msq->parp != NULL) {
				if (msq->parp->mv_flag & B_MV_MATEPATH) {
					msq->mv_flag |= B_MV_MATEBRANCH;
					msq = msq->parp;
					msq->amip = msq_prev;
					msq_prev = msq;
					break;
				}
				if (mate_count == 0) {
					msq->parp->mv_flag |= B_MV_MATEPATH;
				}
				if (msq->parp->BDP == NULL) {
					msq->parp->BDP = fm_get_board(depth_mate);
					ASSERT(msq->BDP != NULL);
					*msq->parp->BDP = *msq->BDP;
					fm_prev_phase(msq);
				}
			}
		}
		mate_count++;
		fm_put_matemseq(msq_prev, msp_mate);
	}
	if (sc_print_loosemate == ON) {
		flag_loose_mate = ON;
		for (depth = depth_mate; depth >= 1; depth--) {
			for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
				ASSERT(!(msp->ps_flag & B_PS_SAMEPHASE) || msp->SPHP != NULL);
				if (!(msp->ps_flag & B_PS_SAMEPHASE)
					|| (msp->mv_flag & B_MV_MATEPATH)
					|| !(msp->SPHP->mv_flag & B_MV_MATEPATH)
					|| msp->SPHP->hi_hand != msp->hi_hand)
				{
					continue;
				}
				msp_mate = msp;
				msp->mv_flag |= B_MV_MATEJOIN;
				ASSERT(msp->SPHP->BDP != NULL);
				sphp_bdp = msp->SPHP->BDP;
				msp->BDP = fm_get_board(depth_mate);
				*msp->BDP = *sphp_bdp;
				msq_prev = NULL;
				for (msq = msp; msq != NULL; msq = msq->parp) {
					msq->amip = msq_prev;
					msq_prev = msq;
					if (msq->parp != NULL) {
						if (msq->parp->mv_flag & B_MV_MATEPATH) {
							msq->mv_flag |= B_MV_MATEBRANCH;
							msq = msq->parp;
							msq->amip = msq_prev;
							msq_prev = msq;
							fm_put_matemseq(msq_prev, msp_mate);
							break;
						}
						if (msq->parp->BDP == NULL) {
							msq->parp->BDP = fm_get_board(depth_mate);
							ASSERT(msq->BDP != NULL);
							*msq->parp->BDP = *msq->BDP;
							fm_prev_phase(msq);
						}
					}
				}
			}
		}
	}
	return;
}

int
test_yo_legalmove(mseq_t *msp)
{
	int rc;

	rc = RC_LEGAL;
	if (fm_test_move(msp) == RC_ILLEGAL) {
		rc = RC_ILLEGAL;
		goto test_legal_exit2;
	}
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (msp->ps_flag & B_PS_HI_CHECK) {
		rc = RC_ILLEGAL;
		goto test_legal_exit;
	}
test_legal_exit:;
	fm_back_phase(msp);
test_legal_exit2:;
	return rc;
}

// test YO_GY escape or not
static int
test_yo_gyescape(mseq_t *msp)
{
	const int *vcp;
	board_t *bdp_par;
	int to;

	bdp_par = msp->parp->BDP;
	msp->pc = YO_GY;
	msp->from = bdp_par->yo_gy_pos;
	for (vcp = vec_yo_step[PC_OU]; vcp[0]; vcp++) {
		to = bdp_par->yo_gy_pos + vcp[0];
		if (IS_YO_MOVABLE(bdp_par->board[to]) && !(bdp_par->fire[to] & B_BD_HI_FIRE)) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			if (test_yo_legalmove(msp) == RC_LEGAL) {
				return RC_ESCAPE;
			}
		}
	}
	return RC_NOTESCAPE;
}

// test to remove check piece or not
static int
test_yo_moveto_fromanywhere(mseq_t *msp, int to)
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
					if (sq <= YO_HI && sq != YO_KI && (RANK(from) >= 7 || RANK(to) >= 7)) {
						msp->mv_flag = B_MV_PROMOTE;
					} else {
						msp->mv_flag = 0x00;
					}
					if (test_yo_legalmove(msp) == RC_LEGAL) {
						return RC_ESCAPE;
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
					if (sq <= YO_HI && sq != YO_KI && (RANK(from) >= 7 || RANK(to) >= 7)) {
						msp->mv_flag = B_MV_PROMOTE;
					} else {
						msp->mv_flag = 0x00;
					}
					if (test_yo_legalmove(msp) == RC_LEGAL) {
						return RC_ESCAPE;
					}
				}
			}
		}
	}
	return RC_NOTESCAPE;
}

// test to shut check-fire
static int
test_yo_shutto(mseq_t *msp, int to)
{
	board_t *bdp_par;
	uchar_t pc;

	bdp_par = msp->parp->BDP;
	msp->from = 0;
	msp->to = to;
	msp->capt = SQ_ROOM;
	for (pc = PC_HI; pc >= PC_FU; pc--) {
		if (bdp_par->yo_hand[pc] > 0) {
			msp->ps_flag = 0x0000;
			msp->pc = (uchar_t)pc | B_PC_YONDER;
			msp->mv_flag = 0x00;
			if (test_yo_legalmove(msp) == RC_LEGAL) {
				return RC_ESCAPE;
			}
		}
	}
	if (bdp_par->fire[to] != 0) {
		if (test_yo_moveto_fromanywhere(msp, to) == RC_ESCAPE) {
			return RC_ESCAPE;
		}
	}
	return RC_NOTESCAPE;
}

int
fm_test_yo_matemseq(mseq_t *msp_par)
{
	board_t *bdp_par;
	int to;
	int v_f, v_r, vec;
	mseq_t mseq;
	board_t board_tmp;

	mseq.parp = msp_par;
	mseq.BDP = &board_tmp;
	mseq.depth = msp_par->depth + 1;
	bdp_par = msp_par->BDP;
	memcpy(&board_tmp.board, &bdp_par->board, OFFSET_OF(board_t, fire, board));
	ASSERT(bdp_par->yo_gy_pos != 0);
	if (test_yo_gyescape(&mseq) == RC_ESCAPE) {
		// 玉が逃げられれば、逃れとする。
		return RC_ESCAPE;
	}
	if (msp_par->ps_flag & B_PS_HI_DBLCHECK) {
		// 両王手で玉が逃げられなければ、詰とする。
		return RC_MATE;
	}
	if (bdp_par->fire[bdp_par->hi_ck_pos] & (B_BD_YO_STEPFIRE | B_BD_YO_RUNFIRE)) {
		if (test_yo_moveto_fromanywhere(&mseq, bdp_par->hi_ck_pos) == RC_ESCAPE) {
			// 王手駒を取れれば、逃れとする。
			return RC_ESCAPE;
		}
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
			if (test_yo_shutto(&mseq, to) == RC_ESCAPE) {
				// 合駒が利けば、逃れとする。
				return RC_ESCAPE;
			}
		}
	}
	// 玉が逃げられず、王手駒を取れず、合駒が利かなければ、詰とする。
	return RC_MATE;
}

void
fm_test_yo_mate(long depth)
{
	mseq_t *msp_par;
	int rc;

	for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
		if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
			continue;
		}
		rc = fm_test_yo_matemseq(msp_par);
		ASSERT(rc == RC_MATE || rc == RC_ESCAPE);
		if (rc == RC_MATE) {
			msp_par->ps_flag |= B_PS_MATE;
			mseq_head[depth].flag |= B_MH_MATE;
		}
	}
	if (!(mseq_head[depth].flag & B_MH_MATE)) {
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

int
test_hi_legalmove(mseq_t *msp)
{
	int rc;

	rc = RC_LEGAL;
	if (fm_test_move(msp) == RC_ILLEGAL) {
		rc = RC_ILLEGAL;
		goto test_legal_exit2;
	}
	fm_set_phase(msp);
	fm_set_fire(msp);
	if (msp->ps_flag & B_PS_YO_CHECK) {
		rc = RC_ILLEGAL;
		goto test_legal_exit;
	}
test_legal_exit:;
	fm_back_phase(msp);
test_legal_exit2:;
	return rc;
}

// test HI_OU escape or not
static int
test_hi_ouescape(mseq_t *msp)
{
	const int *vcp;
	board_t *bdp_par;
	int to;

	bdp_par = msp->parp->BDP;
	msp->pc = HI_OU;
	msp->from = bdp_par->hi_ou_pos;
	for (vcp = vec_hi_step[PC_OU]; vcp[0]; vcp++) {
		to = bdp_par->hi_ou_pos + vcp[0];
		if (IS_HI_MOVABLE(bdp_par->board[to]) && !(bdp_par->fire[to] & B_BD_YO_FIRE)) {
			msp->ps_flag = 0x0000;
			msp->to = to;
			msp->capt = bdp_par->board[to];
			msp->mv_flag = 0x00;
			if (test_hi_legalmove(msp) == RC_LEGAL) {
				return RC_ESCAPE;
			}
		}
	}
	return RC_NOTESCAPE;
}

// test to remove check piece or not
static int
test_hi_moveto_fromanywhere(mseq_t *msp, int to)
{
	board_t *bdp_par;
	int from;
	int i;
	uchar_t sq;

	bdp_par = msp->parp->BDP;
	msp->to = to;
	msp->capt = bdp_par->board[to];
	if (bdp_par->fire[to] & B_BD_HI_STEPFIRE) {
		for (i = 0; hi_step_vec[i] != 0; i++) {
			from = to - hi_step_vec[i];
			sq = bdp_par->board[from];
			if (IS_HI_PC(sq) && sq != HI_OU) {
				if (hi_step_pc[i][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					if (sq <= HI_HI && sq != HI_KI && (RANK(from) <= 3 || RANK(to) <= 3)) {
						msp->mv_flag = B_MV_PROMOTE;
					} else {
						msp->mv_flag = 0x00;
					}
					if (test_hi_legalmove(msp) == RC_LEGAL) {
						return RC_ESCAPE;
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
			if (IS_HI_PC(sq) && sq != HI_OU) {
				if (hi_run_pc[i][KIND(sq)] != 0) {
					msp->ps_flag = 0x0000;
					msp->pc = sq;
					msp->from = from;
					if (sq <= HI_HI && sq != HI_KI && (RANK(from) <= 3 || RANK(to) <= 3)) {
						msp->mv_flag = B_MV_PROMOTE;
					} else {
						msp->mv_flag = 0x00;
					}
					if (test_hi_legalmove(msp) == RC_LEGAL) {
						return RC_ESCAPE;
					}
				}
			}
		}
	}
	return RC_NOTESCAPE;
}

// test to shut check-fire
static int
test_hi_shutto(mseq_t *msp, int to)
{
	board_t *bdp_par;
	uchar_t pc;

	bdp_par = msp->parp->BDP;
	msp->from = 0;
	msp->to = to;
	msp->capt = SQ_ROOM;
	msp->mv_flag = 0x00;
	for (pc = PC_HI; pc >= PC_FU; pc--) {
		if (bdp_par->hi_hand[pc] > 0) {
			msp->ps_flag = 0x0000;
			msp->pc = (uchar_t)pc | B_PC_HITHER;
			if (test_hi_legalmove(msp) == RC_LEGAL) {
				return RC_ESCAPE;
			}
		}
	}
	if (bdp_par->fire[to] != 0) {
		if (test_hi_moveto_fromanywhere(msp, to) == RC_ESCAPE) {
			return RC_ESCAPE;
		}
	}
	return RC_NOTESCAPE;
}

int
fm_test_hi_matemseq(mseq_t *msp_par)
{
	board_t *bdp_par;
	int to;
	int v_f, v_r, vec;
	mseq_t mseq;
	board_t board_tmp;

	mseq.parp = msp_par;
	mseq.BDP = &board_tmp;
	mseq.depth = msp_par->depth + 1;
	bdp_par = msp_par->BDP;
	memcpy(&board_tmp.board, &bdp_par->board, OFFSET_OF(board_t, fire, board));
	ASSERT(bdp_par->hi_ou_pos != 0);
	if (test_hi_ouescape(&mseq) == RC_ESCAPE) {
		// 玉が逃げられれば、逃れとする。
		return RC_ESCAPE;
	}
	if (msp_par->ps_flag & B_PS_YO_DBLCHECK) {
		// 両王手で玉が逃げられなければ、詰とする。
		return RC_MATE;
	}
	if (bdp_par->fire[bdp_par->yo_ck_pos] & (B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE)) {
		if (test_hi_moveto_fromanywhere(&mseq, bdp_par->yo_ck_pos) == RC_ESCAPE) {
			// 王手駒を取れれば、逃れとする。
			return RC_ESCAPE;
		}
	}
	if (msp_par->ps_flag & B_PS_YO_RUNCHECK) {
		v_f = FILE(bdp_par->hi_ou_pos) - FILE(bdp_par->yo_ck_pos);
		if (v_f > 0) {
			v_f = 1;
		} else if (v_f < 0) {
			v_f = -1;
		}
		v_r = RANK(bdp_par->hi_ou_pos) - RANK(bdp_par->yo_ck_pos);
		if (v_r > 0) {
			v_r = 1;
		} else if (v_r < 0) {
			v_r = -1;
		}
		vec = VEC(v_f, v_r);
		for (to = bdp_par->hi_ou_pos - vec; to != bdp_par->yo_ck_pos; to -= vec) {
			if (test_hi_shutto(&mseq, to) == RC_ESCAPE) {
				// 合駒が利けば、逃れとする。
				return RC_ESCAPE;
			}
		}
	}
	// 玉が逃げられず、王手駒を取れず、合駒が利かなければ、詰とする。
	return RC_MATE;
}

void
fm_test_hi_mate(long depth)
{
	mseq_t *msp_par;
	int rc;

	for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
		if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
			continue;
		}
		rc = fm_test_hi_matemseq(msp_par);
		ASSERT(rc == RC_MATE || rc == RC_ESCAPE);
		if (rc == RC_MATE) {
			msp_par->ps_flag |= B_PS_MATE;
			mseq_head[depth].flag |= B_MH_MATE;
		}
	}
	if (!(mseq_head[depth].flag & B_MH_MATE)) {
		mseq_head[depth].flag |= B_MH_ESCAPE;
	}
	return;
}

int
fm_test_targetmseq(mseq_t *msp_par)
{
	if (msp_par->hi_hash == mseq_target.hi_hash
		&& msp_par->yo_hash == mseq_target.yo_hash
		&& msp_par->hi_hand == mseq_target.hi_hand)
	{
		return RC_MATE;
	}
	return RC_ESCAPE;
}

void
fm_test_target(long depth)
{
	mseq_t *msp_par;
	int rc;

	for (msp_par = mseq_head[depth].brop; msp_par != NULL; msp_par = msp_par->brop) {
		if (msp_par->ps_flag & (B_PS_SAMEPHASE | B_PS_UFMATE)) {
			continue;
		}
		rc = fm_test_targetmseq(msp_par);
		ASSERT(rc == RC_MATE || rc == RC_ESCAPE);
		if (rc == RC_MATE) {
			msp_par->ps_flag |= B_PS_MATE;
			mseq_head[depth].flag |= B_MH_MATE;
		}
	}
	return;
}

