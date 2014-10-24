// za/mate.c -- zoo+single-change help(-self)-mate mate-testing subroutines
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/mate.c 1.7 2014/08/17 02:13:45 budai10 Exp budai10 $

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

static int test_hither_mate_exchange(phase_t *php);
static int test_yonder_mate_exchange(phase_t *php);
static int is_hi_legal_move(phase_t *php);
static int is_yo_legal_move(phase_t *php);

int
is_hither_mate_normal(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->hi_ou_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_hither_mate_place(php) == 0) {
				return FALSE;
			}
		} else if (IS_HI_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (piece[mvp->fpc].test_hither_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_hither_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

int
is_hither_mate_messigny(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->hi_ou_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_hither_mate_place(php) == 0) {
				return FALSE;
			}
		} else if (IS_HI_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (test_hither_mate_exchange(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_hither_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_hither_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

int
is_yonder_mate_normal(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_yonder_mate_place(php) == 0) {
				return FALSE;
			}
		} else if (IS_YO_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (piece[mvp->fpc].test_yonder_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_yonder_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

int
is_yonder_mate_messigny(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_yonder_mate_place(php) == 0) {
				return FALSE;
			}
		} else if (IS_YO_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (test_yonder_mate_exchange(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_yonder_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_yonder_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

int
is_hither_stalemate(phase_t *php)
{
	move_t *mvp;
	int pos;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_hither_mate_place_loose(php) == 0) {
				return FALSE;
			}
		} else if (IS_HI_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (piece[mvp->fpc].test_hither_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_hither_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

int
is_yonder_stalemate(phase_t *php)
{
	move_t *mvp;
	int pos;
	uchar_t bpc, pc;

	mvp = &php->move;
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			if (test_yonder_mate_place_loose(php) == 0) {
				return FALSE;
			}
		} else if (IS_YO_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = KIND(php->fpc_board[pos]);
			if (piece[mvp->fpc].test_yonder_mate_leap(php) == 0) {
				return FALSE;
			}
			if (piece[mvp->fpc].test_yonder_mate_run(php) == 0) {
				return FALSE;
			}
		}
	}

	return TRUE;
}

// 0: 逃れ
// 1: 詰
int
test_hither_mate_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
		if (php->hi_hand[pc] >= 1) {
			mvp->pc = mvp->fpc = pc;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_hither_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_hither_mate_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if (php->check_state == (B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK) &&
			(php->fire[mvp->to] & B_YO_FIRE_PATH)) {
		for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
			if (php->hi_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_hither_mate_place_taihaink2mad(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if ((php->check_state == (B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_YO_FIRE_PATH)) ||
			(php->fire[mvp->to] & B_HI_ESCAPE_SPECIAL)) {
		for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
			if (php->hi_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (piece[pc].attr & B_PA_CONSTRAINT_PM) {
					if (is_hi_legal_move(php)) {
						return 0;
					}
				} else {
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
			}
		}
	}

	return 1;
}

static int
test_hither_mate_exchange(phase_t *php)
{
	move_t *mvp = &php->move;
	int to;
	uchar_t to_sq;

	for (to = 0; to < 81; to++) {
		to_sq = KIND(php->board[to]);
		if (to_sq == mvp->pc && to != mvp->from) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = B_MV2_EXCHANGE;
			if (test_hither_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_hither_mate_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i;
	uchar_t to_sq;

	for (i = 0; ; i++) {
		to = hi_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
		if (IS_HI_MOVABLE(to_sq)) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_hither_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_hither_mate_shishi_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, to3, i, j, k;
	uchar_t to_sq, to_sq2;

	for (i = 0; ; i++) {
		to = hi_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
		if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_hither_mate_sub(php) == 0) {
				return 0;
			}
			if (IS_YO_PC(to_sq) && IS_AROUND(mvp->from, to)) {
				mvp->to_1st = to;
				for (j = 0; ; j++) {
					to2 = hi_leap_to_pos[PC_OU][to][j];
					if (to2 == EOP) {
						break;
					}
					if (_mbsstr(piece[mvp->pc].name, "獅子") == NULL) {
						for(k = 0; ; k++) {
							to3 = hi_leap_to_pos[mvp->fpc][mvp->from][k];
							if (to3 == EOP) {
								goto next_for;
							}
							if (to3 == to2) {
								break;
							}
						}
					}
					to_sq2 = php->board[to2];
					if (IS_HI_MOVABLE(to_sq2) || to2 == mvp->from) {
						mvp->to = to2;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_hither_mate_sub(php) == 0) {
							return 0;
						}
					}
				next_for:;
				}
				mvp->to_1st = NOP;
			}
		}
	}

	return 1;
}

int
test_hither_mate_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to_next, i, j;
	uchar_t to_sq;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0, to_next = hi_run_to_posv[mvp->fpc][mvp->from][j][0]; ; i++) {
			to = to_next;
			to_sq = php->board[to];
			to_next = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to_next == EOP) {
				if (IS_HI_MOVABLE(to_sq)) {
					mvp->to = to;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return 1;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
test_hither_mate_mao_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int div, to, i;
	uchar_t to_sq;

	for (i = 0; hi_run_to_posv[mvp->fpc][mvp->from][i] != NULL; i++) {
		div = hi_run_to_posv[mvp->fpc][mvp->from][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][0];
			if (to == EOP) {
				goto another_to;
			}
			to_sq = php->board[to];
			if (IS_HI_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
		another_to:
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
			if (to == EOP) {
				continue;
			}
			to_sq = php->board[to];
			if (IS_HI_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_hither_mate_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i, j;
	uchar_t to_sq;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (IS_HI_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return 1;
}

int
test_hither_mate_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				i++;
				to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
				if (to2 == EOP) {
					break;
				}
				to_sq2 = php->board[to2];
				if (IS_HI_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_hither_mate_decapt_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				if (IS_YO_PC(to_sq)) {
					i++;
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_ROOM(to_sq2)) {
						mvp->to = to2;
						mvp->to_1st = to;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_hither_mate_sub(php) == 0) {
							return 0;
						}
						mvp->to_1st = NOP;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_hither_mate_pao_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (IS_PASSABLE(to_sq)) {
				if (IS_ROOM(to_sq)) {
					mvp->to = to;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
			} else {
				for (i++; ; i++) {
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (NOT_PASSABLE(to_sq2)) {
						if (IS_YO_PC(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_hither_mate_sub(php) == 0) {
								return 0;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_hither_mate_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				for (i++; ; i++) {
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_PASSABLE(to_sq2)) {
						if (IS_ROOM(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_yonder_mate_sub(php) == 0) {
								return 0;
							}
						}
					} else {
						if (IS_YO_PC(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_yonder_mate_sub(php) == 0) {
								return 0;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_hither_mate_jump_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j, n;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				n++;
				if (n >= 2) {
					i++;
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_HI_MOVABLE(to_sq2)) {
						mvp->to = to2;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_yonder_mate_sub(php) == 0) {
							return 0;
						}
					}
					break;
				}
			}
		}
	}

	return 1;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
test_hither_mate_refl(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][to][0];
				to_sq2 = php->board[to2];
				if (IS_HI_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][to][1];
				if (to2 == EOP) {
					break;
				}
				to_sq2 = php->board[to2];
				if (IS_HI_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_hither_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
		}
	}

	return 1;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
int
test_hither_mate_equi_hop(phase_t *php)
{
	char *to, *point;
	move_t *mvp = &php->move;
	int i;
	uchar_t to_sq;

	to = hi_run_to_posv[mvp->fpc][mvp->from][0];
	point = yo_run_to_posv[mvp->fpc][mvp->from][0];
	for (i = 0; to[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[point[i]])) {
			to_sq = php->board[to[i]];
			if (IS_HI_MOVABLE(to_sq)) {
				mvp->to = to[i];
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_yonder_mate_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
		if (php->yo_hand[pc] >= 1) {
			mvp->pc = mvp->fpc = pc;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_yonder_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if (php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
			(php->fire[mvp->to] & B_HI_FIRE_PATH)) {
		for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
			if (php->yo_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_yonder_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_yonder_mate_place_taihaink2mad(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(php->fire[mvp->to] & B_YO_ESCAPE_SPECIAL)) {
		for (i = 0; pc = hand_pc[i], NOT_NUL(pc); i++) {
			if (php->yo_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (piece[pc].attr & B_PA_CONSTRAINT_PM) {
					if (is_yo_legal_move(php)) {
						return 0;
					}
				} else {
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
			}
		}
	}

	return 1;
}

static int
test_yonder_mate_exchange(phase_t *php)
{
	move_t *mvp = &php->move;
	int to;
	uchar_t to_sq;

	for (to = 0; to < 81; to++) {
		to_sq = KIND(php->board[to]);
		if (to_sq == mvp->pc && to != mvp->from) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = B_MV2_EXCHANGE;
			if (test_yonder_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i;
	uchar_t to_sq;

	for (i = 0; ; i++) {
		to = yo_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
		if (IS_YO_MOVABLE(to_sq)) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_yonder_mate_sub(php) == 0) {
				return 0;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_shishi_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, to3, i, j, k;
	uchar_t to_sq, to_sq2;

	for (i = 0; ; i++) {
		to = yo_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
		if (IS_YO_MOVABLE(to_sq)) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (test_yonder_mate_sub(php) == 0) {
				return 0;
			}
			if (IS_HI_PC(to_sq) && IS_AROUND(mvp->from, to)) {
				mvp->to_1st = to;
				for (j = 0; ; j++) {
					to2 = yo_leap_to_pos[PC_OU][to][j];
					if (to2 == EOP) {
						break;
					}
					if (_mbsstr(piece[mvp->pc].name, "獅子") == NULL) {
						for(k = 0; ; k++) {
							to3 = yo_leap_to_pos[mvp->fpc][mvp->from][k];
							if (to3 == EOP) {
								goto next_for;
							}
							if (to3 == to2) {
								break;
							}
						}
					}
					to_sq2 = php->board[to2];
					if (IS_YO_MOVABLE(to_sq2) || to2 == mvp->from) {
						mvp->to = to2;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_yonder_mate_sub(php) == 0) {
							return 0;
						}
					}
				next_for:;
				}
				mvp->to_1st = NOP;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to_next, i, j;
	uchar_t to_sq;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0, to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][0]; ; i++) {
			to = to_next;
			to_sq = php->board[to];
			to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to_next == EOP) {
				if (IS_YO_MOVABLE(to_sq)) {
					mvp->to = to;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return 1;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
test_yonder_mate_mao_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int div, to, i;
	uchar_t to_sq;

	for (i = 0; hi_run_to_posv[mvp->fpc][mvp->from][i] != NULL; i++) {
		div = hi_run_to_posv[mvp->fpc][mvp->from][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][0];
			if (to == EOP) {
				goto another_to;
			}
			to_sq = php->board[to];
			if (IS_YO_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_yonder_mate_sub(php) == 0) {
					return 0;
				}
			}
		another_to:
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
			if (to == EOP) {
				continue;
			}
			to_sq = php->board[to];
			if (IS_HI_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_hither_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_yonder_mate_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i, j;
	uchar_t to_sq;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (IS_YO_MOVABLE(to_sq)) {
				mvp->to = to;
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_yonder_mate_sub(php) == 0) {
					return 0;
				}
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				i++;
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
				if (to2 == EOP) {
					break;
				}
				to_sq2 = php->board[to2];
				if (IS_YO_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_decapt_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				if (IS_HI_PC(to_sq)) {
					i++;
					to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_ROOM(to_sq2)) {
						mvp->to = to2;
						mvp->to_1st = to;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_yonder_mate_sub(php) == 0) {
							return 0;
						}
						mvp->to_1st = NOP;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_pao_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (IS_PASSABLE(to_sq)) {
				if (IS_ROOM(to_sq)) {
					mvp->to = to;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
			} else {
				for (i++; ; i++) {
					to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (NOT_PASSABLE(to_sq2)) {
						if (IS_HI_PC(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_yonder_mate_sub(php) == 0) {
								return 0;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				for (i++; ; i++) {
					to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_PASSABLE(to_sq2)) {
						if (IS_ROOM(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_yonder_mate_sub(php) == 0) {
								return 0;
							}
						}
					} else {
						if (IS_HI_PC(to_sq2)) {
							mvp->to = to2;
							mvp->flag = 0x00;
							mvp->flag2 = 0x00;
							if (test_yonder_mate_sub(php) == 0) {
								return 0;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 1;
}

int
test_yonder_mate_jump_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j, n;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				n++;
				if (n >= 2) {
					i++;
					to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_YO_MOVABLE(to_sq2)) {
						mvp->to = to2;
						mvp->flag = 0x00;
						mvp->flag2 = 0x00;
						if (test_yonder_mate_sub(php) == 0) {
							return 0;
						}
					}
					break;
				}
			}
		}
	}

	return 1;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
test_yonder_mate_refl(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][to][0];
				to_sq2 = php->board[to2];
				if (IS_YO_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][to][1];
				if (to2 == EOP) {
					break;
				}
				to_sq2 = php->board[to2];
				if (IS_YO_MOVABLE(to_sq2)) {
					mvp->to = to2;
					mvp->flag = 0x00;
					mvp->flag2 = 0x00;
					if (test_yonder_mate_sub(php) == 0) {
						return 0;
					}
				}
				break;
			}
		}
	}

	return 1;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
int
test_yonder_mate_equi_hop(phase_t *php)
{
	char *to, *point;
	move_t *mvp = &php->move;
	int i;
	uchar_t to_sq;

	to = hi_run_to_posv[mvp->fpc][mvp->from][0];
	point = yo_run_to_posv[mvp->fpc][mvp->from][0];
	for (i = 0; to[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[point[i]])) {
			to_sq = php->board[to[i]];
			if (IS_YO_MOVABLE(to_sq)) {
				mvp->to = to[i];
				mvp->flag = 0x00;
				mvp->flag2 = 0x00;
				if (test_yonder_mate_sub(php) == 0) {
					return 0;
				}
			}
		}
	}

	return 1;
}

int
test_mate_null(phase_t *php)
{
	return -1;
}

// n: 平
int
test_hither_mate_sub_n(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
				return 0;
			}
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
			return 0;
		}
	}

	return 1;
}

// c: キルケ、アンチキルケ
int
test_hither_mate_sub_c(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
				return 0;
			}
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
				if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
					return 0;
				}
			}
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
			return 0;
		}
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (hi_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
				return 0;
			}
		}
	}

	return 1;
}

// f: 打歩詰
int
test_hither_mate_sub_f(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (is_hi_legal_move(php)) {
			return 0;
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (is_hi_legal_move(php)) {
		return 0;
	}

	return 1;
}

// mf: Messigny打歩詰
int
test_hither_mate_sub_mf(phase_t *php)
{
	move_t *mvp = &php->move;
	move_t *mvq = &(php - 2)->move;

	if (depth >= 3 && (mvp->flag2 & mvq->flag2 & B_MV2_EXCHANGE) &&
			mvp->pc == mvq->pc && mvp->from == mvq->from && mvp->to == mvq->to) {
		return 0;
	}
	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (is_hi_legal_move(php)) {
			return 0;
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (is_hi_legal_move(php)) {
		return 0;
	}

	return 1;
}

static int
is_hi_legal_move(phase_t *php)
{
	if (hi_move(php) == 0 && analyze_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
			if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
				return TRUE;
			}
			if (++phase_count >= check_point) {
				mile_stone();
			}
			depth++;
			if (NOT(is_yonder_mate(php + 1))) {
				depth--;
				return TRUE;
			}
			depth--;
		}
	}

	return FALSE;
}

// n: 平
int
test_yonder_mate_sub_n(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
				return 0;
			}
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
			return 0;
		}
	}

	return 1;
}

// c: キルケ、アンチキルケ
int
test_yonder_mate_sub_c(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
				return 0;
			}
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
				if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
					return 0;
				}
			}
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
			return 0;
		}
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (yo_move_leaf(php) == 0 && analyze_leaf_phase(php + 1) == 0) {
			if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
				return 0;
			}
		}
	}

	return 1;
}

// f: 打歩詰
int
test_yonder_mate_sub_f(phase_t *php)
{
	move_t *mvp = &php->move;

	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (is_yo_legal_move(php)) {
			return 0;
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (is_yo_legal_move(php)) {
		return 0;
	}

	return 1;
}

// mf: Messigny打歩詰
int
test_yonder_mate_sub_mf(phase_t *php)
{
	move_t *mvp = &php->move;
	move_t *mvq = &(php - 2)->move;

	if (depth >= 3 && (mvp->flag2 & mvq->flag2 & B_MV2_EXCHANGE) &&
			mvp->pc == mvq->pc && mvp->from == mvq->from && mvp->to == mvq->to) {
		return 0;
	}
	if (IS_PROMOTABLE_FE(mvp->pc) &&
			(mvp->from != NOP && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
		mvp->flag |= B_MV_PROMOTE;
		if (is_yo_legal_move(php)) {
			return 0;
		}
	}
	mvp->flag &= ~B_MV_PROMOTE;
	if (is_yo_legal_move(php)) {
		return 0;
	}

	return 1;
}

static int
is_yo_legal_move(phase_t *php)
{
	if (yo_move(php) == 0 && analyze_phase(php + 1) == 0) {
		if (NOT((php + 1)->check_state & B_CS_HITHER_CHECK)) {
			if (NOT((php + 1)->check_state & B_CS_YONDER_CHECK)) {
				return TRUE;
			}
			if (++phase_count >= check_point) {
				mile_stone();
			}
			depth++;
			if (NOT(is_hither_mate(php + 1))) {
				depth--;
				return TRUE;
			}
			depth--;
		}
	}

	return FALSE;
}

