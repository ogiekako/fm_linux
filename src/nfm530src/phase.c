// nfm/nfm/phase.c -- phase processor
// Copyright(c) KAMINA Jiro 2006-2007.  All rights reserved.
// $Header: phase.cv  1.4  07/10/20 23:21:48  Exp $

#include	<stdlib.h>
#include	<string.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"

// go one ply
void
fm_set_phase(mseq_t *msp)
{
	board_t *bdp;
	int pos, pos_r_1, pos_r_9;

	ASSERT(msp->from == 0
		|| (msp->from >= POS(1, 1)
			&& msp->from <= POS(9, 9) && IS_ONBD(board_void[msp->from])));
	ASSERT(msp->to >= POS(1, 1) && msp->to <= POS(9, 9) && IS_ONBD(board_void[msp->to]));
	bdp = msp->BDP;
	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			bdp->hi_hand[RAW_KIND(msp->pc)]--;
			bdp->board[msp->to] = msp->pc;
		} else {
			bdp->board[msp->from] = SQ_ROOM;
			bdp->board[msp->to] = msp->pc;
			if (msp->pc == HI_OU) {
				bdp->hi_ou_pos = msp->to;
			} else if (msp->mv_flag & B_MV_PROMOTE) {
				bdp->board[msp->to] |= B_PC_PROMOTE;
			}
			if (!IS_ROOM(msp->capt)) {
				if (cond & COND_PWC) {
					switch (msp->capt) {
					case YO_HI: case YO_KA: case YO_KI: case YO_GI:
					case YO_RY: case YO_UM: case YO_NG: case YO_NK: case YO_NY: case YO_TO:
						bdp->board[msp->from] = msp->capt;
						msp->mv_flag |= B_MV_REBORN;
						break;
					case YO_KE:
						if (RANK(msp->from) <= 7) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case YO_KY:
						if (RANK(msp->from) <= 8) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case YO_FU:
						if (RANK(msp->from) <= 8) {
							pos_r_1 = FILE(msp->from) * 10 + 1;
							for (pos = FILE(msp->from) * 10 + 8; pos >= pos_r_1; pos--) {
								if (bdp->board[pos] == YO_FU && pos != msp->from) {
									goto double_yo_fu;
								}
							}
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
						double_yo_fu:;
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					}
				} else {
					bdp->hi_hand[RAW_KIND(msp->capt)]++;
				}
			}
		}
	} else {
		if (msp->from == 0) {
			bdp->yo_hand[RAW_KIND(msp->pc)]--;
			bdp->board[msp->to] = msp->pc;
		} else {
			bdp->board[msp->from] = SQ_ROOM;
			bdp->board[msp->to] = msp->pc;
			if (msp->pc == YO_GY) {
				bdp->yo_gy_pos = msp->to;
			} else if (msp->mv_flag & B_MV_PROMOTE) {
				bdp->board[msp->to] |= B_PC_PROMOTE;
			}
			if (!IS_ROOM(msp->capt)) {
				if (cond & COND_PWC) {
					switch (msp->capt) {
					case HI_HI: case HI_KA: case HI_KI: case HI_GI:
					case HI_RY: case HI_UM: case HI_NG: case HI_NK: case HI_NY: case HI_TO:
						bdp->board[msp->from] = msp->capt;
						msp->mv_flag |= B_MV_REBORN;
						break;
					case HI_KE:
						if (RANK(msp->from) >= 3) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case HI_KY:
						if (RANK(msp->from) >= 2) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case HI_FU:
						if (RANK(msp->from) >= 2) {
							pos_r_9 = FILE(msp->from) * 10 + 9;
							for (pos = FILE(msp->from) * 10 + 2; pos <= pos_r_9; pos++) {
								if (bdp->board[pos] == HI_FU && pos != msp->from) {
									goto double_hi_fu;
								}
							}
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
						double_hi_fu:;
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					}
				} else {
					bdp->yo_hand[RAW_KIND(msp->capt)]++;
				}
			}
		}
	}
	return;
}

void
fm_back_phase(mseq_t *msp)
{
	board_t *bdp;

	ASSERT(msp->from == 0
		|| (msp->from >= POS(1, 1)
			&& msp->from <= POS(9, 9) && IS_ONBD(board_void[msp->from])));
	ASSERT(msp->to >= POS(1, 1) && msp->to <= POS(9, 9) && IS_ONBD(board_void[msp->to]));
	bdp = msp->BDP;
	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			bdp->board[msp->to] = SQ_ROOM;
			bdp->hi_hand[RAW_KIND(msp->pc)]++;
		} else {
			if (!IS_ROOM(msp->capt) && !(msp->mv_flag & B_MV_REBORN)) {
				bdp->hi_hand[RAW_KIND(msp->capt)]--;
			}
			bdp->board[msp->to] = msp->capt;
			bdp->board[msp->from] = msp->pc;
			if (msp->pc == HI_OU) {
				bdp->hi_ou_pos = msp->from;
			}
		}
	} else {
		if (msp->from == 0) {
			bdp->board[msp->to] = SQ_ROOM;
			bdp->yo_hand[RAW_KIND(msp->pc)]++;
		} else {
			if (!IS_ROOM(msp->capt) && !(msp->mv_flag & B_MV_REBORN)) {
				bdp->yo_hand[RAW_KIND(msp->capt)]--;
			}
			bdp->board[msp->to] = msp->capt;
			bdp->board[msp->from] = msp->pc;
			if (msp->pc == YO_GY) {
				bdp->yo_gy_pos = msp->from;
			}
		}
	}
	return;
}

void
fm_prev_phase(mseq_t *msp)
{
	board_t *bdp;

	ASSERT(msp->from == 0
		|| (msp->from >= POS(1, 1)
			&& msp->from <= POS(9, 9) && IS_ONBD(board_void[msp->from])));
	ASSERT(msp->to >= POS(1, 1) && msp->to <= POS(9, 9) && IS_ONBD(board_void[msp->to]));
	bdp = msp->parp->BDP;
	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			bdp->board[msp->to] = SQ_ROOM;
			bdp->hi_hand[RAW_KIND(msp->pc)]++;
		} else {
			if (!IS_ROOM(msp->capt) && !(msp->mv_flag & B_MV_REBORN)) {
				bdp->hi_hand[RAW_KIND(msp->capt)]--;
			}
			bdp->board[msp->to] = msp->capt;
			bdp->board[msp->from] = msp->pc;
			if (msp->pc == HI_OU) {
				bdp->hi_ou_pos = msp->from;
			}
		}
	} else {
		if (msp->from == 0) {
			bdp->board[msp->to] = SQ_ROOM;
			bdp->yo_hand[RAW_KIND(msp->pc)]++;
		} else {
			if (!IS_ROOM(msp->capt) && !(msp->mv_flag & B_MV_REBORN)) {
				bdp->yo_hand[RAW_KIND(msp->capt)]--;
			}
			bdp->board[msp->to] = msp->capt;
			bdp->board[msp->from] = msp->pc;
			if (msp->pc == YO_GY) {
				bdp->yo_gy_pos = msp->from;
			}
		}
	}
	return;
}

void
fm_set_fire(mseq_t *msp)
{
	board_t *bdp;
	const int *vcp;
	int p_f, pos, posxv;
	uchar_t pc;

	bdp = msp->BDP;
	memset(bdp->fire, '\0', sizeof(bdp->fire));
	bdp->hi_ck_pos = 0;
	bdp->yo_ck_pos = 0;
	for (p_f = 1; p_f <= 9; p_f++) {
		for (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			pc = bdp->board[pos];
			if (IS_HI_PC(pc)) {
				for (vcp = vec_hi_step[KIND(pc)]; vcp[0] != 0; vcp++) {
					posxv = pos + vcp[0];
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if (IS_ONBD(bdp->board[posxv])) {
						bdp->fire[posxv] |= B_BD_HI_STEPFIRE;
						if (bdp->board[posxv] == YO_GY) {
							bdp->hi_ck_pos = pos;
							if (msp->ps_flag & B_PS_HI_RUNCHECK) {
								msp->ps_flag |= B_PS_HI_DBLCHECK;
								bdp->hi_ck_pos = 0xff;
							}
							msp->ps_flag |= B_PS_HI_STEPCHECK | B_PS_HI_CHECK;
//							if (pc == HI_FU && msp->to == pos && msp->from == 0) {
//								msp->ps_flag |= B_PS_UFCHECK;
//							}
						}
					}
				}
				for (vcp = vec_hi_run[KIND(pc)]; vcp[0] != 0; vcp++) {
					for (posxv = pos + vcp[0]; IS_THRUABLE(bdp->board[posxv]);
						posxv += vcp[0])
					{
						ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
						bdp->fire[posxv] |= B_BD_HI_RUNFIRE;
					}
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if (IS_ONBD(bdp->board[posxv])) {
						bdp->fire[posxv] |= B_BD_HI_RUNFIRE;
						if (bdp->board[posxv] == YO_GY) {
							bdp->hi_ck_pos = pos;
							if (msp->ps_flag & (B_PS_HI_STEPCHECK | B_PS_HI_RUNCHECK)) {
								msp->ps_flag |= B_PS_HI_DBLCHECK;
								bdp->hi_ck_pos = 0xff;
							}
							if (posxv == pos + vcp[0]) {
								msp->ps_flag |= B_PS_HI_STEPCHECK | B_PS_HI_CHECK;
							} else {
								msp->ps_flag |= B_PS_HI_RUNCHECK | B_PS_HI_CHECK;
							}
							if (IS_ONBD(bdp->board[posxv + vcp[0]])) {
								if (cond & COND_PWC) {
									if (IS_ROOM(bdp->board[posxv + vcp[0]])) {
										bdp->fire[posxv + vcp[0]] |= B_BD_HI_OVERFIRE;
									}
								} else {
									bdp->fire[posxv + vcp[0]] |= B_BD_HI_OVERFIRE;
								}
							}
						}
					}
				}
				if (pc == HI_FU) {
					bdp->fire[POS(p_f, 0)] |= B_BD_HI_FUEXIST;
				}
			} else if (IS_YO_PC(pc)) {
				for (vcp = vec_yo_step[KIND(pc)]; vcp[0] != 0; vcp++) {
					posxv = pos + vcp[0];
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if (IS_ONBD(bdp->board[posxv])) {
						bdp->fire[posxv] |= B_BD_YO_STEPFIRE;
						if (bdp->board[posxv] == HI_OU) {
							bdp->yo_ck_pos = pos;
							if (msp->ps_flag & B_PS_YO_RUNCHECK) {
								msp->ps_flag |= B_PS_YO_DBLCHECK;
								bdp->yo_ck_pos = 0xff;
							}
							msp->ps_flag |= B_PS_YO_STEPCHECK | B_PS_YO_CHECK;
							if (pc == YO_FU && msp->to == pos && msp->from == 0) {
								msp->ps_flag |= B_PS_UFCHECK;
							}
						}
						if (pc == YO_GY) {
							bdp->fire[posxv] |= B_BD_YO_GYFIRE;
						}
					}
				}
				for (vcp = vec_yo_run[KIND(pc)]; vcp[0] != 0; vcp++) {
					for (posxv = pos + vcp[0]; IS_THRUABLE(bdp->board[posxv]);
						posxv += vcp[0])
					{
						ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
						bdp->fire[posxv] |= B_BD_YO_RUNFIRE;
					}
					ASSERT(posxv >= 0 && posxv < BOARD_SIZE);
					if (IS_ONBD(bdp->board[posxv])) {
						bdp->fire[posxv] |= B_BD_YO_RUNFIRE;
						if (bdp->board[posxv] == HI_OU) {
							bdp->yo_ck_pos = pos;
							if (msp->ps_flag & (B_PS_YO_STEPCHECK | B_PS_YO_RUNCHECK)) {
								msp->ps_flag |= B_PS_YO_DBLCHECK;
								bdp->yo_ck_pos = 0xff;
							}
							if (posxv == pos + vcp[0]) {
								msp->ps_flag |= B_PS_YO_STEPCHECK | B_PS_YO_CHECK;
							} else {
								msp->ps_flag |= B_PS_YO_RUNCHECK | B_PS_YO_CHECK;
							}
							if (IS_ONBD(bdp->board[posxv + vcp[0]])) {
								if (cond & COND_PWC) {
									if (IS_ROOM(bdp->board[posxv + vcp[0]])) {
										bdp->fire[posxv + vcp[0]] |= B_BD_YO_OVERFIRE;
									}
								} else {
									bdp->fire[posxv + vcp[0]] |= B_BD_YO_OVERFIRE;
								}
							}
						}
					}
				}
				if (pc == YO_FU) {
					bdp->fire[POS(p_f, 0)] |= B_BD_YO_FUEXIST;
				}
			}
		}
	}
	return;
}

int
fm_test_move(mseq_t *msp)
{
	board_t *bd_parp;
	int my_vec, v_f, v_r;
	int pos;
	const int *vcp;
	uchar_t pc;

	if ((cond & COND_NO_PROM) && (msp->mv_flag & B_MV_PROMOTE)) {
		return RC_ILLEGAL;
	}
	bd_parp = msp->parp->BDP;
	if (IS_HI_PC(msp->pc)) {
		// test piece hung - FU, KY, KE
		if (RANK(msp->to) == 1 && msp->pc <= HI_KE && !(msp->mv_flag & B_MV_PROMOTE)) {
			return RC_ILLEGAL;
		}
		// test piece hung - KE
		if (RANK(msp->to) == 2 && msp->pc == HI_KE && !(msp->mv_flag & B_MV_PROMOTE)) {
			return RC_ILLEGAL;
		}
		// test double-FU
		if (msp->pc == HI_FU && msp->from == 0
			&& (bd_parp->fire[POS(FILE(msp->to), 0)] & B_BD_HI_FUEXIST))
		{
			return RC_ILLEGAL;
		}
		// test suicide-open-check
		if (msp->to != bd_parp->yo_ck_pos && bd_parp->hi_ou_pos != 0 && msp->pc != HI_OU
			&& (bd_parp->fire[msp->from] & B_BD_YO_RUNFIRE))
		{
			v_f = FILE(bd_parp->hi_ou_pos) - FILE(msp->from);
			v_r = RANK(bd_parp->hi_ou_pos) - RANK(msp->from);
			my_vec = 0;
			if (v_f == 0) {
				my_vec = (v_r > 0)? 1: -1;
			} else if (v_r == 0) {
				my_vec = (v_f > 0)? 10: -10;
			} else if (v_f == v_r || v_f == -v_r) {
				my_vec = (bd_parp->hi_ou_pos - msp->from) / abs(v_r);
			}
			if (my_vec != 0) {
				for (pos = msp->from + my_vec; pos != bd_parp->hi_ou_pos; pos += my_vec) {
					if (pos == msp->to || !IS_THRUABLE(bd_parp->board[pos])) {
						goto hi_test_suicide_open_ok;
					}
				}
				for (pos = msp->from - my_vec; IS_THRUABLE(bd_parp->board[pos]);
					pos -= my_vec)
				{
					if (pos == msp->to || !(bd_parp->fire[pos] & B_BD_YO_RUNFIRE)) {
						goto hi_test_suicide_open_ok;
					}
				}
				if (pos != msp->to && IS_YO_PC(bd_parp->board[pos])
					&& yo_runv_pc[my_vec + 11][KIND(bd_parp->board[pos])] != 0)
				{
					return RC_ILLEGAL;
				}
			}
		hi_test_suicide_open_ok:;
		}
		if (rule == RULE_NFMSERM && flag_docheck == OFF) {
			if (bd_parp->fire[msp->to] & B_BD_YO_GYFIRE) {
				if (msp->mv_flag & B_MV_PROMOTE) {
					pc = msp->pc | B_PC_PROMOTE;
				} else {
					pc = msp->pc;
				}
				for (vcp = vec_hi_step[KIND(pc)]; vcp[0] != 0; vcp++) {
					if (bd_parp->board[msp->to + vcp[0]] == YO_GY) {
						return RC_ILLEGAL;
					}
				}
			}
		}
	} else {
		// test piece hung - FU, KY, KE
		if (RANK(msp->to) == 9 && msp->pc <= YO_KE && !(msp->mv_flag & B_MV_PROMOTE)) {
			return RC_ILLEGAL;
		}
		// test piece hung - KE
		if (RANK(msp->to) == 8 && msp->pc == YO_KE && !(msp->mv_flag & B_MV_PROMOTE)) {
			return RC_ILLEGAL;
		}
		// test double-FU
		if (msp->pc == YO_FU && msp->from == 0
			&& (bd_parp->fire[POS(FILE(msp->to), 0)] & B_BD_YO_FUEXIST))
		{
			return RC_ILLEGAL;
		}
		// test suicide-open-check
		if (msp->to != bd_parp->hi_ck_pos && msp->pc != YO_GY
			&& (bd_parp->fire[msp->from] & B_BD_HI_RUNFIRE))
		{
			v_f = FILE(bd_parp->yo_gy_pos) - FILE(msp->from);
			v_r = RANK(bd_parp->yo_gy_pos) - RANK(msp->from);
			my_vec = 0;
			if (v_f == 0) {
				my_vec = (v_r > 0)? 1: -1;
			} else if (v_r == 0) {
				my_vec = (v_f > 0)? 10: -10;
			} else if (v_f == v_r || v_f == -v_r) {
				my_vec = (bd_parp->yo_gy_pos - msp->from) / abs(v_r);
			}
			if (my_vec != 0) {
				for (pos = msp->from + my_vec; pos != bd_parp->yo_gy_pos; pos += my_vec) {
					if (pos == msp->to || !IS_THRUABLE(bd_parp->board[pos])) {
						goto yo_test_suicide_open_ok;
					}
				}
				for (pos = msp->from - my_vec; IS_THRUABLE(bd_parp->board[pos]);
					pos -= my_vec)
				{
					if (pos == msp->to || !(bd_parp->fire[pos] & B_BD_HI_RUNFIRE)) {
						goto yo_test_suicide_open_ok;
					}
				}
				if (pos != msp->to && IS_HI_PC(bd_parp->board[pos])
					&& hi_runv_pc[my_vec + 11][KIND(bd_parp->board[pos])] != 0)
				{
					return RC_ILLEGAL;
				}
			}
		yo_test_suicide_open_ok:;
		}
	}
	return RC_LEGAL;
}

int
fm_set_givenphase(mseq_t *msp)
{
	board_t *bdp;
	int pos, pos_r_1, pos_r_9;

	bdp = msp->BDP;
	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			if (!IS_ROOM(bdp->board[msp->to]) || bdp->hi_hand[RAW_KIND(msp->pc)] <= 0) {
				return RC_ILLEGAL;
			}
			bdp->hi_hand[RAW_KIND(msp->pc)]--;
			bdp->board[msp->to] = msp->pc;
		} else {
			if (!IS_HI_PC(bdp->board[msp->from]) || !IS_HI_MOVABLE(bdp->board[msp->to])) {
				return RC_ILLEGAL;
			}
			bdp->board[msp->from] = SQ_ROOM;
			bdp->board[msp->to] = msp->pc;
			if (msp->pc == HI_OU) {
				bdp->hi_ou_pos = msp->to;
			} else if (msp->mv_flag & B_MV_PROMOTE) {
				if (RANK(msp->from) > 3 && RANK(msp->to) > 3) {
					return RC_ILLEGAL;
				}
				bdp->board[msp->to] |= B_PC_PROMOTE;
			}
			if (!IS_ROOM(msp->capt)) {
				if (cond & COND_PWC) {
					switch (msp->capt) {
					case YO_HI: case YO_KA: case YO_KI: case YO_GI:
					case YO_RY: case YO_UM: case YO_NG: case YO_NK: case YO_NY: case YO_TO:
						bdp->board[msp->from] = msp->capt;
						msp->mv_flag |= B_MV_REBORN;
						break;
					case YO_KE:
						if (RANK(msp->from) <= 7) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case YO_KY:
						if (RANK(msp->from) <= 8) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case YO_FU:
						if (RANK(msp->from) <= 8) {
							pos_r_1 = FILE(msp->from) * 10 + 1;
							for (pos = FILE(msp->from) * 10 + 8; pos >= pos_r_1; pos--) {
								if (bdp->board[pos] == YO_FU && pos != msp->from) {
									goto double_yo_fu;
								}
							}
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
						double_yo_fu:;
							bdp->hi_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					}
				} else {
					bdp->hi_hand[RAW_KIND(msp->capt)]++;
				}
			}
		}
	} else {
		if (msp->from == 0) {
			if (!IS_ROOM(bdp->board[msp->to]) || bdp->yo_hand[RAW_KIND(msp->pc)] <= 0) {
				return RC_ILLEGAL;
			}
			bdp->yo_hand[RAW_KIND(msp->pc)]--;
			bdp->board[msp->to] = msp->pc;
		} else {
			if (!IS_YO_PC(bdp->board[msp->from]) || !IS_YO_MOVABLE(bdp->board[msp->to])) {
				return RC_ILLEGAL;
			}
			bdp->board[msp->from] = SQ_ROOM;
			bdp->board[msp->to] = msp->pc;
			if (msp->pc == YO_GY) {
				bdp->yo_gy_pos = msp->to;
			} else if (msp->mv_flag & B_MV_PROMOTE) {
				if (RANK(msp->from) < 7 && RANK(msp->to) < 7) {
					return RC_ILLEGAL;
				}
				bdp->board[msp->to] |= B_PC_PROMOTE;
			}
			if (!IS_ROOM(msp->capt)) {
				if (cond & COND_PWC) {
					switch (msp->capt) {
					case HI_HI: case HI_KA: case HI_KI: case HI_GI:
					case HI_RY: case HI_UM: case HI_NG: case HI_NK: case HI_NY: case HI_TO:
						bdp->board[msp->from] = msp->capt;
						msp->mv_flag |= B_MV_REBORN;
						break;
					case HI_KE:
						if (RANK(msp->from) >= 3) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case HI_KY:
						if (RANK(msp->from) >= 2) {
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					case HI_FU:
						if (RANK(msp->from) >= 2) {
							pos_r_9 = FILE(msp->from) * 10 + 9;
							for (pos = FILE(msp->from) * 10 + 2; pos <= pos_r_9; pos++) {
								if (bdp->board[pos] == HI_FU && pos != msp->from) {
									goto double_hi_fu;
								}
							}
							bdp->board[msp->from] = msp->capt;
							msp->mv_flag |= B_MV_REBORN;
						} else {
						double_hi_fu:;
							bdp->yo_hand[RAW_KIND(msp->capt)]++;
						}
						break;
					}
				} else {
					bdp->yo_hand[RAW_KIND(msp->capt)]++;
				}
			}
		}
	}
	return RC_LEGAL;
}

