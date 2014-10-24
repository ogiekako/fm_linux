// zaco/move.c -- new phase generator of FM solver
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/move.c 1.24 2014/08/14 14:17:00 budai10 Exp budai10 $

#include	<stdio.h>
#include	<memory.h>
#include	"fm.h"
#include	"piece.h"

static int hi_pc_constraint(phase_t *ophp);
static int yo_pc_constraint(phase_t *ophp);
static int hi_pc_constraint_fpt(phase_t *ophp, int from, int to);
static int yo_pc_constraint_fpt(phase_t *ophp, int from, int to);
static int hi_pc_constraint_fptwr(phase_t *ophp, int from, int to);
static int yo_pc_constraint_fptwr(phase_t *ophp, int from, int to);
static int hi_pc_constraint_wt(phase_t *ophp);
static int yo_pc_constraint_wt(phase_t *ophp);
static int phase_constraint(phase_t *ophp, phase_t *nphp);
static int phase_constraint_koko(phase_t *nphp, int pos);

// test move validity and make new phase structure
//
// new: pointer to new phase structure
// old: pointer to original phase sturcture
//
// 0: valid move
// 1: invalid move
// 2: already tested (at restarting)

int
hi_move_normal(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
		nphp->hi_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (hi_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}

	nphp->state = init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
yo_move_normal(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
		nphp->yo_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (yo_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}

	nphp->state = B_HITHER_TURN | init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
hi_move_phase_constraint(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
		nphp->hi_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (hi_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
yo_move_phase_constraint(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
		nphp->yo_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (yo_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = B_HITHER_TURN | init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
hi_move_restarting(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (NEQUALM(omvp, &last_move[depth], 7) ||
			((omvp->flag ^ last_move[depth].flag) & B_MV_PROMOTE)) {
		return 2;
	}
	phase_count--;  // counted at stopping
	omvp->flag = last_move[depth].flag;
	omvp->flag2 = last_move[depth].flag2;
	if (last_move[depth + 1].pc == NUL) {
		restarting = OFF;
		phase_count--;  // count after now
		if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
			hi_move = hi_move_phase_constraint;
			yo_move = yo_move_phase_constraint;
			hi_move_leaf = hi_move_leaf_phase_constraint;
			yo_move_leaf = yo_move_leaf_phase_constraint;
		} else {
			hi_move = hi_move_normal;
			yo_move = yo_move_normal;
			hi_move_leaf = hi_move_leaf_normal;
			yo_move_leaf = yo_move_leaf_normal;
		}
	}

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
		nphp->hi_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (hi_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
yo_move_restarting(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_count++;

	if (NEQUALM(omvp, &last_move[depth], 7) ||
			((omvp->flag ^ last_move[depth].flag) & B_MV_PROMOTE)) {
		return 2;
	}
	phase_count--;  // counted at stopping
	omvp->flag = last_move[depth].flag;
	omvp->flag2 = last_move[depth].flag2;
	if (last_move[depth + 1].pc == NUL) {
		restarting = OFF;
		phase_count--;  // count after now
		if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
			hi_move = hi_move_phase_constraint;
			yo_move = yo_move_phase_constraint;
			hi_move_leaf = hi_move_leaf_phase_constraint;
			yo_move_leaf = yo_move_leaf_phase_constraint;
		} else {
			hi_move = hi_move_normal;
			yo_move = yo_move_normal;
			hi_move_leaf = hi_move_leaf_normal;
			yo_move_leaf = yo_move_leaf_normal;
		}
	}

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
		nphp->yo_hand[omvp->pc]--;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (yo_move_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = B_HITHER_TURN | init_phase_state;
	nphp->check_state = 0x00;
	nphp->hi_ou_pos = NOP;
	nphp->yo_gy_pos = NOP;
	nphp->move = null_move;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

// test move validity and make new phase structure (brief version)
//
// new: pointer to new phase structure
// old: pointer to original phase sturcture
//
// 0: valid move
// 1: invalid move

int
hi_move_leaf_normal(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_leaf_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (hi_move_leaf_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}

	nphp->state = nphp->check_state = 0x00;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
yo_move_leaf_normal(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_leaf_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (yo_move_leaf_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}

	nphp->state = nphp->check_state = 0x00;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
hi_move_leaf_phase_constraint(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_leaf_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (hi_move_leaf_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = nphp->check_state = 0x00;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
yo_move_leaf_phase_constraint(phase_t *ophp)
{
	phase_t *nphp;
	move_t *omvp = &ophp->move;

	call_move_leaf_count++;

	if (omvp->from == NOP) {
		if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
			return -1;
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
	} else {
		if (test_before_move_test(ophp, omvp) == TRUE) {
			if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
				return -1;
			}
		}
		nphp = ophp + 1;
		nphp[0] = ophp[0];
		if (yo_move_leaf_sub(ophp, omvp, nphp) != 0) {
			return -1;
		}
	}
	if (phase_constraint(ophp, nphp)) {
		return -1;
	}

	nphp->state = nphp->check_state = 0x00;
	memcpy(nphp->fire, null_phase.fire, sizeof(null_phase.fire)); 
	memcpy(nphp->fpc_board, nphp->board, sizeof(nphp->fpc_board)); 

	return 0;
}

int
test_before_move_test_others(phase_t *ophp, move_t *omvp)
{
	return TRUE;
}

int
test_before_move_test_anticirce_andernach(phase_t *ophp, move_t *omvp)
{
	return IS_ROOM(ophp->board[omvp->to]);
}

int
test_before_move_test_antiandernach(phase_t *ophp, move_t *omvp)
{
	return NOT_ROOM(ophp->board[omvp->to]);
}

static int
hi_pc_constraint_sub(phase_t *ophp, uchar_t pc, uchar_t bpc, int from, int to)
{
	if (piece[pc].attr & B_PA_CONSTRAINT_SQ) {
		// çsÇ´èàÇÃÇ»Ç¢ãÓ
		if (hi_valid_sq[pc][to] == OFF) {
			return -1;	// invalid sq
		}
	}
	if (piece[pc].attr & B_PA_CONSTRAINT_2F) {
		int pos, pos1;
		// ìÒï‡É`ÉFÉbÉN
		for (pos = POS(POS_FILE(to), 8), pos1 = pos - 8; pos >= pos1; pos--) {
			if (ophp->board[pos] == bpc && pos != from) {
				return -1;	// double FU pieces
			}
		}
	}
	if (piece[pc].attr & B_PA_CONSTRAINT_3T) {
		int pos, pos1, n;
		// éOâçÉ`ÉFÉbÉN
		n = 0;
		for (pos = POS(POS_FILE(to), 8), pos1 = pos - 8; pos >= pos1; pos--) {
			if (ophp->board[pos] == bpc && pos != from) {
				n++;
				if (n >= 2) {
					return -1;	// triple TTB pieces
				}
			}
		}
	}

	return 0;
}

static int
yo_pc_constraint_sub(phase_t *ophp, uchar_t pc, uchar_t bpc, int from, int to)
{
	if (piece[pc].attr & B_PA_CONSTRAINT_SQ) {
		// çsÇ´èàÇÃÇ»Ç¢ãÓ
		if (yo_valid_sq[pc][to] == OFF) {
			return -1;	// invalid sq
		}
	}
	if (piece[pc].attr & B_PA_CONSTRAINT_2F) {
		int pos, pos9;
		// ìÒï‡É`ÉFÉbÉN
		for (pos = POS(POS_FILE(to), 0), pos9 = pos + 8; pos <= pos9; pos++) {
			if (ophp->board[pos] == bpc && pos != from) {
				return -1;	// double FU pieces
			}
		}
	}
	if (piece[pc].attr & B_PA_CONSTRAINT_3T) {
		int pos, pos9, n;
		// éOâçÉ`ÉFÉbÉN
		n = 0;
		for (pos = POS(POS_FILE(to), 0), pos9 = pos + 8; pos <= pos9; pos++) {
			if (ophp->board[pos] == bpc && pos != from) {
				n++;
				if (n >= 2) {
					return -1;	// triple TTB pieces
				}
			}
		}
	}

	return 0;
}

static int
hi_pc_constraint(phase_t *ophp)
{
	move_t *omvp = &ophp->move;
	uchar_t pc, bpc;

	pc = omvp->pc;
	if (omvp->flag & B_MV_PROMOTE) {
		pc |= B_PC_PROMOTED;
	}
	bpc = omvp->pc | B_PC_HITHER;

	return hi_pc_constraint_sub(ophp, pc, bpc, omvp->from, omvp->to);
}

static int
yo_pc_constraint(phase_t *ophp)
{
	move_t *omvp = &ophp->move;
	uchar_t pc, bpc;

	pc = omvp->pc;
	if (omvp->flag & B_MV_PROMOTE) {
		pc |= B_PC_PROMOTED;
	}
	bpc = omvp->pc | B_PC_YONDER;

	return yo_pc_constraint_sub(ophp, pc, bpc, omvp->from, omvp->to);
}

// fpt: from piece to
static int
hi_pc_constraint_fpt(phase_t *ophp, int from, int to)
{
	uchar_t pc, bpc;

	pc = KIND(ophp->board[from]);
	bpc = pc | B_PC_HITHER;

	return hi_pc_constraint_sub(ophp, pc, bpc, from, to);
}

static int
yo_pc_constraint_fpt(phase_t *ophp, int from, int to)
{
	uchar_t pc, bpc;

	pc = KIND(ophp->board[from]);
	bpc = pc | B_PC_YONDER;

	return yo_pc_constraint_sub(ophp, pc, bpc, from, to);
}

// fpt: from piece to with raw
static int
hi_pc_constraint_fptwr(phase_t *ophp, int from, int to)
{
	uchar_t pc, bpc;

	pc = RAW_KIND(ophp->board[from]);
	bpc = pc | B_PC_HITHER;

	return hi_pc_constraint_sub(ophp, pc, bpc, from, to);
}

static int
yo_pc_constraint_fptwr(phase_t *ophp, int from, int to)
{
	uchar_t pc, bpc;

	pc = RAW_KIND(ophp->board[from]);
	bpc = pc | B_PC_YONDER;

	return yo_pc_constraint_sub(ophp, pc, bpc, from, to);
}

// wt: with turn
static int
hi_pc_constraint_wt(phase_t *ophp)
{
	move_t *omvp = &ophp->move;
	uchar_t pc, bpc;

	pc = omvp->pc;
	bpc = pc | B_PC_YONDER;

	return yo_pc_constraint_sub(ophp, pc, bpc, omvp->from, omvp->to);
}

static int
yo_pc_constraint_wt(phase_t *ophp)
{
	move_t *omvp = &ophp->move;
	uchar_t pc, bpc;

	pc = omvp->pc;
	bpc = pc | B_PC_HITHER;

	return hi_pc_constraint_sub(ophp, pc, bpc, omvp->from, omvp->to);
}

static int
phase_constraint_koko(phase_t *nphp, int pos)
{
	int pos2, i;

	for (i = 0, pos2 = around_to_pos[pos][i]; pos2 != NOP; i++, pos2 = around_to_pos[pos][i]) {
		if (IS_ANY_PC(nphp->board[pos2])) {
			return 0;
		}
	}

	return -1;
}

static int
phase_constraint(phase_t *ophp, phase_t *nphp)
{
	if (cond_flag & B_C_KOKO) {
		move_t *omvp = &ophp->move;
		if (cond_flag & B_C_ANTICIRCE) {
			if (omvp->flag2 & B_MV2_REBORN) {
				if (phase_constraint_koko(nphp, omvp->reborn) == -1) {
					return -1;
				}
			} else {
				if (phase_constraint_koko(nphp, omvp->to) == -1) {
					return -1;
				}
			}
		} else {
			if (phase_constraint_koko(nphp, omvp->to) == -1) {
				return -1;
			}
		}
	}

	return 0;
}

int
hi_move_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->hi_hand[rcapt]++;
			}
		}
	}
	if (omvp->to != omvp->from) {
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->hi_hand[rcapt]++;
			}
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	}

	return 0;
}

int
yo_move_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->yo_hand[rcapt]++;
			}
		}
	}
	if (omvp->to != omvp->from) {
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->yo_hand[rcapt]++;
			}
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	}

	return 0;
}

int
hi_move_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					yo_pc_constraint_fpt(ophp, omvp->to_1st, omvp->from)) {
				nphp->board[omvp->from] = SQ_ROOM;
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->from] = ophp->board[omvp->to_1st];
				omvp->flag2 |= B_MV2_REBORN_1ST;
			}
		} else {
			nphp->board[omvp->from] = SQ_ROOM;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					yo_pc_constraint_fpt(ophp, omvp->to, omvp->to_1st)) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to_1st] = ophp->board[omvp->to];
				omvp->flag2 |= B_MV2_REBORN;
			}
		} else {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	} else {
		if (omvp->to != omvp->from) {
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				capt = KIND(to_sq);
				if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
						yo_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
					nphp->board[omvp->from] = SQ_ROOM;
					if (piece[capt].attr & B_PA_REUSABLE) {
						rcapt = RAW_KIND(capt);
						nphp->hi_hand[rcapt]++;
					}
				} else {
					nphp->board[omvp->from] = ophp->board[omvp->to];
					omvp->flag2 |= B_MV2_REBORN;
				}
			} else {
				nphp->board[omvp->from] = SQ_ROOM;
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
yo_move_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					hi_pc_constraint_fpt(ophp, omvp->to_1st, omvp->from)) {
				nphp->board[omvp->from] = SQ_ROOM;
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->from] = ophp->board[omvp->to_1st];
				omvp->flag2 |= B_MV2_REBORN_1ST;
			}
		} else {
			nphp->board[omvp->from] = SQ_ROOM;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					hi_pc_constraint_fpt(ophp, omvp->to, omvp->to_1st)) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to_1st] = ophp->board[omvp->to];
				omvp->flag2 |= B_MV2_REBORN;
			}
		} else {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	} else {
		if (omvp->to != omvp->from) {
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				capt = KIND(to_sq);
				if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
						hi_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
					nphp->board[omvp->from] = SQ_ROOM;
					if (piece[capt].attr & B_PA_REUSABLE) {
						rcapt = RAW_KIND(capt);
						nphp->yo_hand[rcapt]++;
					}
				} else {
					nphp->board[omvp->from] = ophp->board[omvp->to];
					omvp->flag2 |= B_MV2_REBORN;
				}
			} else {
				nphp->board[omvp->from] = SQ_ROOM;
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
hi_move_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rcapt = RAW_KIND(to_sq);
			switch (rcapt) {
			case PC_FU:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = YO_FU;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[PC_FU].attr & B_PA_REUSABLE) {
						nphp->hi_hand[PC_FU]++;
					}
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = yo_reborn_pos[rcapt][0];
				} else if (pf > 4) {
					reborn = yo_reborn_pos[rcapt][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = yo_reborn_pos[rcapt][1];
					} else {
						reborn = yo_reborn_pos[rcapt][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = yo_reborn_pos[rcapt][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == yo_reborn_pos[rcapt][0] &&
							IS_ROOM(nphp->board[yo_reborn_pos[rcapt][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->hi_hand[rcapt]++;
					}
				}
				break;
			case PC_KA:
			case PC_HI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				reborn = yo_reborn_pos[rcapt][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->hi_hand[rcapt]++;
					}
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[rcapt][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->hi_hand[rcapt]++;
					}
				}
				break;
			}
			ophp->board[omvp->to] = ROOM;
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				rcapt = RAW_KIND(to_sq);
				switch (rcapt) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = YO_FU;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[PC_FU].attr & B_PA_REUSABLE) {
							nphp->hi_hand[PC_FU]++;
						}
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = yo_reborn_pos[rcapt][0];
					} else if (pf > 4) {
						reborn = yo_reborn_pos[rcapt][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = yo_reborn_pos[rcapt][1];
						} else {
							reborn = yo_reborn_pos[rcapt][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = yo_reborn_pos[rcapt][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == yo_reborn_pos[rcapt][0] &&
								IS_ROOM(nphp->board[yo_reborn_pos[rcapt][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->hi_hand[rcapt]++;
						}
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = yo_reborn_pos[rcapt][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->hi_hand[rcapt]++;
						}
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[rcapt][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->hi_hand[rcapt]++;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

int
yo_move_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rcapt = RAW_KIND(to_sq);
			switch (rcapt) {
			case PC_FU:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = HI_FU;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[PC_FU].attr & B_PA_REUSABLE) {
						nphp->yo_hand[PC_FU]++;
					}
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = hi_reborn_pos[rcapt][0];
				} else if (pf > 4) {
					reborn = hi_reborn_pos[rcapt][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = hi_reborn_pos[rcapt][1];
					} else {
						reborn = hi_reborn_pos[rcapt][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = hi_reborn_pos[rcapt][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == hi_reborn_pos[rcapt][0] &&
							IS_ROOM(nphp->board[hi_reborn_pos[rcapt][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->yo_hand[rcapt]++;
					}
				}
				break;
			case PC_KA:
			case PC_HI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				reborn = hi_reborn_pos[rcapt][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->yo_hand[rcapt]++;
					}
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[rcapt][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					if (piece[rcapt].attr & B_PA_REUSABLE) {
						nphp->yo_hand[rcapt]++;
					}
				}
				break;
			}
			ophp->board[omvp->to] = ROOM;
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				rcapt = RAW_KIND(to_sq);
				switch (rcapt) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = HI_FU;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[PC_FU].attr & B_PA_REUSABLE) {
							nphp->yo_hand[PC_FU]++;
						}
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = hi_reborn_pos[rcapt][0];
					} else if (pf > 4) {
						reborn = hi_reborn_pos[rcapt][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = hi_reborn_pos[rcapt][1];
						} else {
							reborn = hi_reborn_pos[rcapt][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = hi_reborn_pos[rcapt][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == hi_reborn_pos[rcapt][0] &&
								IS_ROOM(nphp->board[hi_reborn_pos[rcapt][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->yo_hand[rcapt]++;
						}
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = hi_reborn_pos[rcapt][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->yo_hand[rcapt]++;
						}
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[rcapt][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						if (piece[rcapt].attr & B_PA_REUSABLE) {
							nphp->yo_hand[rcapt]++;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

int
hi_move_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt, rpc;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rpc = RAW_KIND(omvp->pc);
			switch (rpc) {
			case PC_FU:
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[PC_FU][pf];
				if ((IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) ||
						omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = hi_reborn_pos[rpc][0];
				} else if (pf > 4) {
					reborn = hi_reborn_pos[rpc][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = hi_reborn_pos[rpc][1];
					} else {
						reborn = hi_reborn_pos[rpc][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = hi_reborn_pos[rpc][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == hi_reborn_pos[rpc][0] &&
							IS_ROOM(nphp->board[hi_reborn_pos[rpc][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_KA:
			case PC_HI:
				reborn = hi_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[rpc][pf];
				if ((IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) ||
						omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_OU:
				reborn = hi_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
				hi_not_reborn_1st:;
					if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
						ophp->board[omvp->to] = ROOM;
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
					if (omvp->flag & B_MV_PROMOTE) {
						nphp->board[omvp->to] |= B_PC_PROMOTED;
					}
				}
				break;
			}
			nphp->board[omvp->to_1st] = SQ_ROOM;
			rcapt = RAW_KIND(to_sq);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				nphp->hi_hand[rcapt]++;
			}
			ophp->board[omvp->to] = ROOM;
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				rpc = RAW_KIND(omvp->pc);
				switch (rpc) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[PC_FU][pf];
					if ((IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fpt(ophp, omvp->to, reborn)))) ||
							omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = hi_reborn_pos[rpc][0];
					} else if (pf > 4) {
						reborn = hi_reborn_pos[rpc][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = hi_reborn_pos[rpc][1];
						} else {
							reborn = hi_reborn_pos[rpc][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = hi_reborn_pos[rpc][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == hi_reborn_pos[rpc][0] &&
								IS_ROOM(nphp->board[hi_reborn_pos[rpc][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = hi_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[rpc][pf];
					if ((IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fpt(ophp, omvp->to, reborn)))) ||
							omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_OU:
					reborn = hi_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
					hi_not_reborn:;
						if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
							return -1;
						}
						nphp->board[omvp->to] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[omvp->to] |= B_PC_PROMOTED;
						}
					}
					break;
				}
				rcapt = RAW_KIND(to_sq);
				if (piece[rcapt].attr & B_PA_REUSABLE) {
					nphp->hi_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt, rpc;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rpc = RAW_KIND(omvp->pc);
			switch (rpc) {
			case PC_FU:
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[PC_FU][pf];
				if ((IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) ||
						omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = yo_reborn_pos[rpc][0];
				} else if (pf > 4) {
					reborn = yo_reborn_pos[rpc][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = yo_reborn_pos[rpc][1];
					} else {
						reborn = yo_reborn_pos[rpc][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = yo_reborn_pos[rpc][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == yo_reborn_pos[rpc][0] &&
							IS_ROOM(nphp->board[yo_reborn_pos[rpc][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				}
				break;
			case PC_KA:
			case PC_HI:
				reborn = yo_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[rpc][pf];
				if ((IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) ||
						omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_OU:
				reborn = yo_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
				yo_not_reborn_1st:;
					if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
						ophp->board[omvp->to] = ROOM;
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
					if (omvp->flag & B_MV_PROMOTE) {
						nphp->board[omvp->to] |= B_PC_PROMOTED;
					}
				}
				break;
			}
			nphp->board[omvp->to_1st] = SQ_ROOM;
			rcapt = RAW_KIND(to_sq);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				nphp->yo_hand[rcapt]++;
			}
			ophp->board[omvp->to] = ROOM;
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				rpc = RAW_KIND(omvp->pc);
				switch (rpc) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[PC_FU][pf];
					if ((IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) ||
							omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = yo_reborn_pos[rpc][0];
					} else if (pf > 4) {
						reborn = yo_reborn_pos[rpc][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = yo_reborn_pos[rpc][1];
						} else {
							reborn = yo_reborn_pos[rpc][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = yo_reborn_pos[rpc][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == yo_reborn_pos[rpc][0] &&
								IS_ROOM(nphp->board[yo_reborn_pos[rpc][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = yo_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[rpc][pf];
					if ((IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fpt(ophp, omvp->to, reborn)))) ||
							omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_OU:
					reborn = yo_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn]) || omvp->from == reborn) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
					yo_not_reborn:;
						if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
							return -1;
						}
						nphp->board[omvp->to] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[omvp->to] |= B_PC_PROMOTED;
						}
					}
					break;
				}
				rcapt = RAW_KIND(to_sq);
				if (piece[rcapt].attr & B_PA_REUSABLE) {
					nphp->yo_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (hi_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->hi_hand[rcapt]++;
			}
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (hi_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (hi_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->yo_hand[rcapt]++;
			}
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (hi_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->hi_hand[rcapt]++;
			}
		} else {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (hi_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			} else {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (hi_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			capt = KIND(to_sq);
			if (piece[capt].attr & B_PA_REUSABLE) {
				rcapt = RAW_KIND(capt);
				nphp->yo_hand[rcapt]++;
			}
		} else {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (yo_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			} else {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (yo_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->flag2 & B_MV2_EXCHANGE) {
		move_t *o2mvp = &(ophp - 1)->move;
		switch (cond_flag & BM_C_MESSIGNY_FAMILY) {
		case B_C_MESSIGNY_J:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE) &&
					o2mvp->from == omvp->from && o2mvp->to == omvp->to) {
				return -1;
			}
			break;
		case B_C_MESSIGNY_E:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE)) {
				return -1;
			}
			break;
		}
		capt = KIND(ophp->board[omvp->to]);
		if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
				yo_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
			return -1;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		nphp->board[omvp->from] = ophp->board[omvp->to];
	} else {
		if (omvp->to_1st != NOP) {
			nphp->board[omvp->to_1st] = SQ_ROOM;
			to_sq = ophp->board[omvp->to_1st];
			if (IS_YO_PC(to_sq)) {
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			}
		}
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->hi_hand[rcapt]++;
				}
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
yo_move_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt, rcapt;

	if (omvp->flag2 & B_MV2_EXCHANGE) {
		move_t *o2mvp = &(ophp - 1)->move;
		switch (cond_flag & BM_C_MESSIGNY_FAMILY) {
		case B_C_MESSIGNY_J:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE) &&
					o2mvp->from == omvp->from && o2mvp->to == omvp->to) {
				return -1;
			}
			break;
		case B_C_MESSIGNY_E:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE)) {
				return -1;
			}
			break;
		}
		capt = KIND(ophp->board[omvp->to]);
		if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
				hi_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
			return -1;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		nphp->board[omvp->from] = ophp->board[omvp->to];
	} else {
		if (omvp->to_1st != NOP) {
			nphp->board[omvp->to_1st] = SQ_ROOM;
			to_sq = ophp->board[omvp->to_1st];
			if (IS_HI_PC(to_sq)) {
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			}
		}
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				capt = KIND(to_sq);
				if (piece[capt].attr & B_PA_REUSABLE) {
					rcapt = RAW_KIND(capt);
					nphp->yo_hand[rcapt]++;
				}
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
	}
	if (omvp->to != omvp->from) {
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	}

	return 0;
}

int
yo_move_leaf_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
	}
	if (omvp->to != omvp->from) {
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	}

	return 0;
}

int
hi_move_leaf_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt;

	if (omvp->to_1st != NOP) {
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					yo_pc_constraint_fpt(ophp, omvp->to_1st, omvp->from)) {
				nphp->board[omvp->from] = SQ_ROOM;
			} else if (piece[capt].attr & B_PA_ROYAL) {
				nphp->board[omvp->from] = SQ_ROOM;
			} else {
				nphp->board[omvp->from] = ophp->board[omvp->to_1st];
			}
		} else {
			nphp->board[omvp->from] = SQ_ROOM;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					yo_pc_constraint_fpt(ophp, omvp->to, omvp->to_1st)) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
			} else if (piece[capt].attr & B_PA_ROYAL) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
			} else {
				nphp->board[omvp->to_1st] = ophp->board[omvp->to];
			}
		} else {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	} else {
		if (omvp->to != omvp->from) {
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				capt = KIND(to_sq);
				if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
						yo_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
					nphp->board[omvp->from] = SQ_ROOM;
				} else if (piece[capt].attr & B_PA_ROYAL) {
					nphp->board[omvp->from] = SQ_ROOM;
				} else {
					nphp->board[omvp->from] = ophp->board[omvp->to];
				}
			} else {
				nphp->board[omvp->from] = SQ_ROOM;
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt;

	if (omvp->to_1st != NOP) {
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					hi_pc_constraint_fpt(ophp, omvp->to_1st, omvp->from)) {
				nphp->board[omvp->from] = SQ_ROOM;
			} else if (piece[capt].attr & B_PA_ROYAL) {
				nphp->board[omvp->from] = SQ_ROOM;
			} else {
				nphp->board[omvp->from] = ophp->board[omvp->to_1st];
			}
		} else {
			nphp->board[omvp->from] = SQ_ROOM;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					hi_pc_constraint_fpt(ophp, omvp->to, omvp->to_1st)) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
			} else if (piece[capt].attr & B_PA_ROYAL) {
				nphp->board[omvp->to_1st] = SQ_ROOM;
			} else {
				nphp->board[omvp->to_1st] = ophp->board[omvp->to];
			}
		} else {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		if (omvp->flag & B_MV_PROMOTE) {
			nphp->board[omvp->to] |= B_PC_PROMOTED;
		}
	} else {
		if (omvp->to != omvp->from) {
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				capt = KIND(to_sq);
				if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
						hi_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
					nphp->board[omvp->from] = SQ_ROOM;
				} else if (piece[capt].attr & B_PA_ROYAL) {
					nphp->board[omvp->from] = SQ_ROOM;
				} else {
					nphp->board[omvp->from] = ophp->board[omvp->to];
				}
			} else {
				nphp->board[omvp->from] = SQ_ROOM;
			}
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rcapt = RAW_KIND(to_sq);
			switch (rcapt) {
			case PC_FU:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = YO_FU;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = yo_reborn_pos[rcapt][0];
				} else if (pf > 4) {
					reborn = yo_reborn_pos[rcapt][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = yo_reborn_pos[rcapt][1];
					} else {
						reborn = yo_reborn_pos[rcapt][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = yo_reborn_pos[rcapt][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == yo_reborn_pos[rcapt][0] &&
							IS_ROOM(nphp->board[yo_reborn_pos[rcapt][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				}
				break;
			case PC_KA:
			case PC_HI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				reborn = yo_reborn_pos[rcapt][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[rcapt][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = rcapt | B_PC_YONDER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			}
			ophp->board[omvp->to] = ROOM;
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				rcapt = RAW_KIND(to_sq);
				switch (rcapt) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = YO_FU;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = yo_reborn_pos[rcapt][0];
					} else if (pf > 4) {
						reborn = yo_reborn_pos[rcapt][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = yo_reborn_pos[rcapt][1];
						} else {
							reborn = yo_reborn_pos[rcapt][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = yo_reborn_pos[rcapt][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == yo_reborn_pos[rcapt][0] &&
								IS_ROOM(nphp->board[yo_reborn_pos[rcapt][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = yo_reborn_pos[rcapt][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[rcapt][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = rcapt | B_PC_YONDER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				}
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rcapt;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		nphp->board[omvp->to] = ophp->board[omvp->from];
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rcapt = RAW_KIND(to_sq);
			switch (rcapt) {
			case PC_FU:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = HI_FU;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = hi_reborn_pos[rcapt][0];
				} else if (pf > 4) {
					reborn = hi_reborn_pos[rcapt][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = hi_reborn_pos[rcapt][1];
					} else {
						reborn = hi_reborn_pos[rcapt][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = hi_reborn_pos[rcapt][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == hi_reborn_pos[rcapt][0] &&
							IS_ROOM(nphp->board[hi_reborn_pos[rcapt][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				}
				break;
			case PC_KA:
			case PC_HI:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				reborn = hi_reborn_pos[rcapt][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				nphp->board[omvp->to_1st] = SQ_ROOM;
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[rcapt][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fptwr(ophp, omvp->to_1st, reborn)))) {
					nphp->board[reborn] = rcapt | B_PC_HITHER;
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				}
				break;
			}
			ophp->board[omvp->to] = ROOM;
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				rcapt = RAW_KIND(to_sq);
				switch (rcapt) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = HI_FU;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = hi_reborn_pos[rcapt][0];
					} else if (pf > 4) {
						reborn = hi_reborn_pos[rcapt][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = hi_reborn_pos[rcapt][1];
						} else {
							reborn = hi_reborn_pos[rcapt][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = hi_reborn_pos[rcapt][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == hi_reborn_pos[rcapt][0] &&
								IS_ROOM(nphp->board[hi_reborn_pos[rcapt][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = hi_reborn_pos[rcapt][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[rcapt][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							(NOT(piece[rcapt].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fptwr(ophp, omvp->to, reborn)))) {
						nphp->board[reborn] = rcapt | B_PC_HITHER;
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					}
					break;
				}
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rpc;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rpc = RAW_KIND(omvp->pc);
			switch (rpc) {
			case PC_FU:
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = hi_reborn_pos[rpc][0];
				} else if (pf > 4) {
					reborn = hi_reborn_pos[rpc][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = hi_reborn_pos[rpc][1];
					} else {
						reborn = hi_reborn_pos[rpc][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = hi_reborn_pos[rpc][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == hi_reborn_pos[rpc][0] &&
							IS_ROOM(nphp->board[hi_reborn_pos[rpc][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_KA:
			case PC_HI:
				reborn = hi_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				pf = POS_FILE(omvp->to_1st);
				reborn = hi_reborn_pos[rpc][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(hi_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto hi_not_reborn_1st;
				}
				break;
			case PC_OU:
				reborn = hi_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
				hi_not_reborn_1st:;
					if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
						ophp->board[omvp->to] = ROOM;
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
					if (omvp->flag & B_MV_PROMOTE) {
						nphp->board[omvp->to] |= B_PC_PROMOTED;
					}
				}
				break;
			}
			nphp->board[omvp->to_1st] = SQ_ROOM;
			ophp->board[omvp->to] = ROOM;
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				rpc = RAW_KIND(omvp->pc);
				switch (rpc) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							((omvp->flag & B_MV_PROMOTE) ||
							 NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fpt(ophp, omvp->from, reborn)))) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = hi_reborn_pos[rpc][0];
					} else if (pf > 4) {
						reborn = hi_reborn_pos[rpc][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = hi_reborn_pos[rpc][1];
						} else {
							reborn = hi_reborn_pos[rpc][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = hi_reborn_pos[rpc][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == hi_reborn_pos[rpc][0] &&
								IS_ROOM(nphp->board[hi_reborn_pos[rpc][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = hi_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = hi_reborn_pos[rpc][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							((omvp->flag & B_MV_PROMOTE) ||
							 NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(hi_pc_constraint_fpt(ophp, omvp->to, reborn)))) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto hi_not_reborn;
					}
					break;
				case PC_OU:
					reborn = hi_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
					hi_not_reborn:;
						if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint(ophp)) {
							return -1;
						}
						nphp->board[omvp->to] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[omvp->to] |= B_PC_PROMOTED;
						}
					}
					break;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	int pf, reborn;
	uchar_t to_sq, rpc;

	if (omvp->to_1st != NOP) {
		// ÇQéËà⁄ìÆÅiÂõÅjÇÃèÍçá
		nphp->board[omvp->from] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			ophp->board[omvp->to] = ROCK;
			rpc = RAW_KIND(omvp->pc);
			switch (rpc) {
			case PC_FU:
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[PC_FU][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_KY:
			case PC_KE:
			case PC_GI:
			case PC_KI:
				pf = POS_FILE(omvp->to_1st);
				if (pf < 4) {
					reborn = yo_reborn_pos[rpc][0];
				} else if (pf > 4) {
					reborn = yo_reborn_pos[rpc][1];
				} else {
					if (omvp->flag2 & B_MV2_REBORN_MORE) {
						reborn = yo_reborn_pos[rpc][1];
					} else {
						reborn = yo_reborn_pos[rpc][0];
						if (NOT_ROOM(nphp->board[reborn])) {
							reborn = yo_reborn_pos[rpc][1];
						}
					}
				}
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
					if (pf == 4 && reborn == yo_reborn_pos[rpc][0] &&
							IS_ROOM(nphp->board[yo_reborn_pos[rpc][1]])) {
						omvp->flag2 |= B_MV2_REBORN_MORE;
					}
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_KA:
			case PC_HI:
				reborn = yo_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_F1:
			case PC_F2:
			case PC_F3:
			case PC_F4:
			case PC_F5:
			case PC_F6:
			case PC_F7:
				pf = POS_FILE(omvp->to_1st);
				reborn = yo_reborn_pos[rpc][pf];
				if (IS_ROOM(nphp->board[reborn]) &&
						(NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
						 NOT(yo_pc_constraint_fpt(ophp, omvp->to_1st, reborn)))) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
					goto yo_not_reborn_1st;
				}
				break;
			case PC_OU:
				reborn = yo_reborn_pos[rpc][0];
				if (IS_ROOM(nphp->board[reborn])) {
					nphp->board[omvp->to_1st] = SQ_ROOM;
					nphp->board[reborn] = ophp->board[omvp->from];
					omvp->flag2 |= B_MV2_REBORN_1ST;
					omvp->reborn_1st = reborn;
				} else {
				yo_not_reborn_1st:;
					if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
						ophp->board[omvp->to] = ROOM;
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
					if (omvp->flag & B_MV_PROMOTE) {
						nphp->board[omvp->to] |= B_PC_PROMOTED;
					}
				}
				break;
			}
			nphp->board[omvp->to_1st] = SQ_ROOM;
			ophp->board[omvp->to] = ROOM;
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		// ÇQéËà⁄ìÆÇ≈Ç»Ç¢èÍçá
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				rpc = RAW_KIND(omvp->pc);
				switch (rpc) {
				case PC_FU:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[PC_FU][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							((omvp->flag & B_MV_PROMOTE) ||
							 NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fpt(ophp, omvp->from, reborn)))) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_KY:
				case PC_KE:
				case PC_GI:
				case PC_KI:
					pf = POS_FILE(omvp->to);
					if (pf < 4) {
						reborn = yo_reborn_pos[rpc][0];
					} else if (pf > 4) {
						reborn = yo_reborn_pos[rpc][1];
					} else {
						if (omvp->flag2 & B_MV2_REBORN_MORE) {
							reborn = yo_reborn_pos[rpc][1];
						} else {
							reborn = yo_reborn_pos[rpc][0];
							if (NOT_ROOM(nphp->board[reborn])) {
								reborn = yo_reborn_pos[rpc][1];
							}
						}
					}
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
						if (pf == 4 && reborn == yo_reborn_pos[rpc][0] &&
								IS_ROOM(nphp->board[yo_reborn_pos[rpc][1]])) {
							omvp->flag2 |= B_MV2_REBORN_MORE;
						}
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_KA:
				case PC_HI:
					reborn = yo_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_F1:
				case PC_F2:
				case PC_F3:
				case PC_F4:
				case PC_F5:
				case PC_F6:
				case PC_F7:
					pf = POS_FILE(omvp->to);
					reborn = yo_reborn_pos[rpc][pf];
					if (IS_ROOM(nphp->board[reborn]) &&
							((omvp->flag & B_MV_PROMOTE) ||
							 NOT(piece[omvp->pc].attr & BM_PA_CONSTRAINT) ||
							 NOT(yo_pc_constraint_fpt(ophp, omvp->from, reborn)))) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[reborn] |= B_PC_PROMOTED;
						}
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
						goto yo_not_reborn;
					}
					break;
				case PC_OU:
					reborn = yo_reborn_pos[rpc][0];
					if (IS_ROOM(nphp->board[reborn])) {
						nphp->board[omvp->to] = SQ_ROOM;
						nphp->board[reborn] = ophp->board[omvp->from];
						omvp->flag2 |= B_MV2_REBORN;
						omvp->reborn = reborn;
					} else {
					yo_not_reborn:;
						if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint(ophp)) {
							return -1;
						}
						nphp->board[omvp->to] = ophp->board[omvp->from];
						if (omvp->flag & B_MV_PROMOTE) {
							nphp->board[omvp->to] |= B_PC_PROMOTED;
						}
					}
					break;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (hi_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (hi_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (yo_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		} else {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (yo_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			} else {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_YO_PC(to_sq)) {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		} else {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (hi_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_YO_PC(to_sq)) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			} else {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && hi_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (hi_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_YONDER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq;

	if (omvp->to_1st != NOP) {
		nphp->board[omvp->to_1st] = SQ_ROOM;
		to_sq = ophp->board[omvp->to_1st];
		if (IS_HI_PC(to_sq)) {
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		} else {
			if (piece[omvp->pc].attr & B_PA_ROYAL) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
				// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
				if (yo_pc_constraint(ophp)) {
					return -1;
				}
				nphp->board[omvp->to] = ophp->board[omvp->from];
			} else {
				// îΩì]ÇµÇΩèÍçá
				nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
				omvp->flag2 |= B_MV2_TURN;
			}
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	} else {
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			to_sq = ophp->board[omvp->to];
			if (IS_HI_PC(to_sq)) {
				nphp->board[omvp->to] = ophp->board[omvp->from];
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			} else {
				if (piece[omvp->pc].attr & B_PA_ROYAL) {
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else if ((piece[omvp->pc].attr & BM_PA_CONSTRAINT) && yo_pc_constraint_wt(ophp)) {
					// îΩì]Ç≈Ç´Ç»Ç¢èÍçá
					if (yo_pc_constraint(ophp)) {
						return -1;
					}
					nphp->board[omvp->to] = ophp->board[omvp->from];
				} else {
					// îΩì]ÇµÇΩèÍçá
					nphp->board[omvp->to] = omvp->pc | B_PC_HITHER;
					omvp->flag2 |= B_MV2_TURN;
				}
				if (omvp->flag & B_MV_PROMOTE) {
					nphp->board[omvp->to] |= B_PC_PROMOTED;
				}
			}
		}
	}

	return 0;
}

int
hi_move_leaf_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt;

	if (omvp->flag2 & B_MV2_EXCHANGE) {
		move_t *o2mvp = &(ophp - 1)->move;
		switch (cond_flag & BM_C_MESSIGNY_FAMILY) {
		case B_C_MESSIGNY_J:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE) &&
					o2mvp->from == omvp->from && o2mvp->to == omvp->to) {
				return -1;
			}
			break;
		case B_C_MESSIGNY_E:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE)) {
				return -1;
			}
			break;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_YO_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					yo_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
				return -1;
			}
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		nphp->board[omvp->from] = ophp->board[omvp->to];
	} else {
		if (omvp->to_1st != NOP) {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

int
yo_move_leaf_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp)
{
	uchar_t to_sq, capt;

	if (omvp->flag2 & B_MV2_EXCHANGE) {
		move_t *o2mvp = &(ophp - 1)->move;
		switch (cond_flag & BM_C_MESSIGNY_FAMILY) {
		case B_C_MESSIGNY_J:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE) &&
					o2mvp->from == omvp->from && o2mvp->to == omvp->to) {
				return -1;
			}
			break;
		case B_C_MESSIGNY_E:
			if (depth >= 3 && (o2mvp->flag2 & B_MV2_EXCHANGE)) {
				return -1;
			}
			break;
		}
		to_sq = ophp->board[omvp->to];
		if (IS_HI_PC(to_sq)) {
			capt = KIND(to_sq);
			if ((piece[capt].attr & BM_PA_CONSTRAINT) &&
					hi_pc_constraint_fpt(ophp, omvp->to, omvp->from)) {
				return -1;
			}
		}
		nphp->board[omvp->to] = ophp->board[omvp->from];
		nphp->board[omvp->from] = ophp->board[omvp->to];
	} else {
		if (omvp->to_1st != NOP) {
			nphp->board[omvp->to_1st] = SQ_ROOM;
		}
		if (omvp->to != omvp->from) {
			nphp->board[omvp->from] = SQ_ROOM;
			nphp->board[omvp->to] = ophp->board[omvp->from];
			if (omvp->flag & B_MV_PROMOTE) {
				nphp->board[omvp->to] |= B_PC_PROMOTED;
			}
		}
	}

	return 0;
}

#if 0
//if (PR_POS(omvp->to) == 12) {
//if (depth == 1) {
printf("depth=%d  ", depth);
if (omvp->from == NOP) {
	printf("%s-%d\n", graph[omvp->pc], PR_POS(omvp->to));
} else if (omvp->flag & B_MV_PROMOTE) {
	printf("%s%d-%d=%s\n", graph[omvp->pc], PR_POS(omvp->from), PR_POS(omvp->to),
			graph[omvp->pc | B_PC_PROMOTED]);
} else if (omvp->to_1st != NOP) {
	printf("%s%d-%d-%d\n", graph[omvp->pc], PR_POS(omvp->from), PR_POS(omvp->to_1st),
			PR_POS(omvp->to));
} else {
	printf("%s%d-%d\n", graph[omvp->pc], PR_POS(omvp->from), PR_POS(omvp->to));
	//printf("attr=%08x\n", piece[omvp->pc].attr);
}
//}
#endif

#if 0
//if ((ophp-1)->move.to == 51) {
if (omvp->from == NOP) {
	printf("%s-%d\n", graph[omvp->pc], omvp->to);
} else if (omvp->flag & B_MV_PROMOTE) {
	printf("%s%d-%d=%s\n", graph[omvp->pc], omvp->from, omvp->to, graph[omvp->pc | B_PC_PROMOTED]);
} else if (omvp->to_1st != NOP) {
	printf("%s%d-%d-%d\n", graph[omvp->pc], omvp->from, omvp->to_1st, omvp->to);
} else {
	printf("%s%d-%d\n", graph[omvp->pc], omvp->from, omvp->to);
}
//}
#endif
