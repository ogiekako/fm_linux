// za/analyze.c -- zoo+single-change help(-self)-mate phase-analyzer
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/analyze.c 1.24 2014/08/16 14:01:47 budai10 Exp budai10 $

#include	<stdio.h>
#include	<signal.h>
#include	<time.h>
#include	<math.h>
#include	"fm.h"
#include	"piece.h"

static void set_yo_escape_special(phase_t *php, int pos);
static void set_hi_escape_special(phase_t *php, int pos);
static void setup_fpc_board_madrasi(phase_t *php);

int
analyze_phase_normal(phase_t *php)
{
	int pos;
	uchar_t bpc, pc;

	call_analyze_count++;
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			php->fpc_board[pos] &= BM_SQ_PC;
			// 王の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->hi_ou_pos = pos;
			}
			// 王手について調べる
			piece[pc].analyze_hi_leap(php, pos, pc);
			piece[pc].analyze_hi_run(php, pos, pc);
			break;
		case B_PC_YONDER:
			php->fpc_board[pos] &= BM_SQ_PC;
			// 玉の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->yo_gy_pos = pos;
			}
			// 王手について調べる
			piece[pc].analyze_yo_leap(php, pos, pc);
			piece[pc].analyze_yo_run(php, pos, pc);
			break;
		}
	}

	return 0;
}

int
analyze_phase_change(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 王の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->hi_ou_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_hi_leap(php, pos, fpc);
			piece[fpc].analyze_hi_run(php, pos, fpc);
			break;
		case B_PC_YONDER:
			// 玉の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->yo_gy_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_yo_leap(php, pos, fpc);
			piece[fpc].analyze_yo_run(php, pos, fpc);
			break;
		}
	}

	return 0;
}

int
analyze_phase_madrasi(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				(void)piece[pc].analyze_hi_leap_madrasi(php, pos, pc, pc);
				(void)piece[pc].analyze_hi_run_madrasi(php, pos, pc, pc);
			}
			break;
		case B_PC_YONDER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				(void)piece[pc].analyze_yo_leap_madrasi(php, pos, pc, pc);
				(void)piece[pc].analyze_yo_run_madrasi(php, pos, pc, pc);
			}
			break;
		}
	}
	setup_fpc_board_madrasi(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 王の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->hi_ou_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_hi_leap(php, pos, fpc);
			piece[fpc].analyze_hi_run(php, pos, fpc);
			break;
		case B_PC_YONDER:
			// 玉の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->yo_gy_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_yo_leap(php, pos, fpc);
			piece[fpc].analyze_yo_run(php, pos, fpc);
			break;
		}
	}

	return 0;
}

int
analyze_phase_change_madrasi(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				fpc = php->fpc_board[pos];
				(void)piece[fpc].analyze_hi_leap_madrasi(php, pos, pc, fpc);
				(void)piece[fpc].analyze_hi_run_madrasi(php, pos, pc, fpc);
			}
			break;
		case B_PC_YONDER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				fpc = php->fpc_board[pos];
				(void)piece[fpc].analyze_yo_leap_madrasi(php, pos, pc, fpc);
				(void)piece[fpc].analyze_yo_run_madrasi(php, pos, pc, fpc);
			}
			break;
		}
	}
	setup_fpc_board_madrasi(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 王の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->hi_ou_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_hi_leap(php, pos, fpc);
			piece[fpc].analyze_hi_run(php, pos, fpc);
			break;
		case B_PC_YONDER:
			// 玉の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->yo_gy_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_yo_leap(php, pos, fpc);
			piece[fpc].analyze_yo_run(php, pos, fpc);
			break;
		}
	}

	return 0;
}

int
analyze_phase_isardam(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 石化について調べる
			pc = KIND(bpc);
			if (piece[pc].analyze_hi_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_hi_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		case B_PC_YONDER:
			// 石化について調べる
			pc = KIND(bpc);
			if (piece[pc].analyze_yo_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_yo_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		}
	}
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 王の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->hi_ou_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_hi_leap(php, pos, fpc);
			piece[fpc].analyze_hi_run(php, pos, fpc);
			break;
		case B_PC_YONDER:
			// 玉の位置を記録する
			pc = KIND(bpc);
			if (piece[pc].attr & B_PA_ROYAL) {
				php->yo_gy_pos = pos;
			}
			// 王手について調べる
			fpc = php->fpc_board[pos];
			piece[fpc].analyze_yo_leap(php, pos, fpc);
			piece[fpc].analyze_yo_run(php, pos, fpc);
			break;
		}
	}

	return 0;
}

void
analyze_hi_leap(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = hi_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
			php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
			if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
				set_yo_escape_special(php, pos);
			}
		}
	}

	return;
}

void
analyze_hi_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = hi_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
				(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
				 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
			php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
			if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
				set_yo_escape_special(php, pos);
			}
		}
	}

	return;
}

void
analyze_hi_step(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j, k;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		fire_next = hi_run_to_posv[fpc][pos][j][0];
		for (i = 0; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = hi_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return;
}

void
analyze_hi_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fnext, i, j, k;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		fnext = hi_run_to_posv[fpc][pos][j][0];
		for (i = 0; ; i++) {
			fpos = fnext;
			fsq = php->board[fpos];
			fnext = hi_run_to_posv[fpc][pos][j][i + 1];
			if (fnext == EOP) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
void
analyze_hi_mao_step(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_HI_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_yo_escape_special(php, pos);
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_HI_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_yo_escape_special(php, pos);
				}
			}
		}
	}

	return;
}

void
analyze_hi_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_HI_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_yo_escape_special(php, pos);
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_HI_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_yo_escape_special(php, pos);
				}
			}
		}
	}

	return;
}

void
analyze_hi_run(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j, k;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_run_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j, k;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = hi_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = hi_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_decapt_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = hi_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_HI_FIRE_PATH;
						}
						php->fire[fpos2] |= B_HI_FIRE_PATH;
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_yo_escape_special(php, pos);
						}
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_decapt_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 hi_trial_move2(php, KIND(php->board[pos]), pos, fpos, fpos2) == 0)) {
						php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = hi_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_HI_FIRE_PATH;
						}
						php->fire[fpos2] |= B_HI_FIRE_PATH;
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_yo_escape_special(php, pos);
						}
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_jump(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
							php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; k != i; k++) {
								fpos = hi_run_to_posv[fpc][pos][j][k];
								php->fire[fpos] |= B_HI_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_yo_escape_special(php, pos);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
								(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
								 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
							php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; k != i; k++) {
								fpos = hi_run_to_posv[fpc][pos][j][k];
								php->fire[fpos] |= B_HI_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_yo_escape_special(php, pos);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_jump_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k, n;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
						php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = hi_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_HI_FIRE_PATH;
						}
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_yo_escape_special(php, pos);
						}
					}
					break;
				}
			}
		}
	}

	return;
}

void
analyze_hi_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k, n;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
						php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = hi_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_HI_FIRE_PATH;
						}
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_yo_escape_special(php, pos);
						}
					}
					break;
				}
			}
		}
	}

	return;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
void
analyze_hi_refl(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_hi_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_HI_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

// Equihopper:
//   hi_leap_to_pos: 対称点に対する利き位置
//   yo_leap_to_pos: 利き位置に対する対称点
//   hi_run_to_posv[0]: 利き位置リスト
//   hi_run_to_posv[1]: 合駒の利く利き位置リスト
//   hi_run_to_posv[2...]: 合駒の利く利き位置に対する合駒位置リスト
//   yo_run_to_posv[0]: 対称点リスト
//   yo_run_to_posv[1]: 合駒の利く対称点リスト
//   hi_to_board: 対称点があれば利く位置
//   yo_to_board: 利く位置がある対称点
void
analyze_hi_equi_hop(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	// ★何故、利き位置はこちらの情報[0]で、中間跳躍位置はあちらの情報[0]なのか？
	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			// 中間跳躍位置になんらかの駒があった場合
			fsq = php->board[fpos[i]];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				// 利き位置に相手王駒があった場合
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					// ただのEqui-hopperの場合
					// ★利き位置２のこちらの情報[1]とは、合駒の利く利き位置リスト
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
					} else {
						// ★利き位置３のこちらの情報[j + 2]とは、
						//   合駒の利く利き位置に対する合駒位置リスト
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; fpos3[k] != EOP; k++) {
								php->fire[fpos3[k]] |= B_HI_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_yo_escape_special(php, pos);
							}
						}
					}
				} else {
					// Non-stop-equi-hopperの場合
					php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
			}
		}
	}

	return;
}

void
analyze_hi_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	// ★何故、利き位置はこちらの情報[0]で、中間跳躍位置はあちらの情報[0]なのか？
	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			// 中間跳躍位置になんらかの駒があった場合
			fsq = php->board[fpos[i]];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 hi_trial_move(php, KIND(php->board[pos]), pos, fpos[i]) == 0)) {
				// 利き位置に相手王駒があった場合
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					// ただのEqui-hopperの場合
					// ★利き位置２のこちらの情報[1]とは、合駒の利く利き位置リスト
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
					} else {
						// ★利き位置３のこちらの情報[j + 2]とは、
						//   合駒の利く利き位置に対する合駒位置リスト
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; fpos3[k] != EOP; k++) {
								php->fire[fpos3[k]] |= B_HI_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_yo_escape_special(php, pos);
							}
						}
					}
				} else {
					// Non-stop-equi-hopperの場合
					php->check_state |= B_CS_HITHER_CHECK | B_CS_CLOSE_CHECK;
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_yo_escape_special(php, pos);
					}
				}
			}
		}
	}

	return;
}

void
analyze_yo_leap(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = yo_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
			php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
			if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
				set_hi_escape_special(php, pos);
			}
		}
	}

	return;
}

void
analyze_yo_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = yo_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
				(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
				 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
			php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
			if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
				set_hi_escape_special(php, pos);
			}
		}
	}

	return;
}

void
analyze_yo_step(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j, k;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = yo_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = yo_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return;
}

void
analyze_yo_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j, k;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = yo_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = yo_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
void
analyze_yo_mao_step(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_YO_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_hi_escape_special(php, pos);
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_YO_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_hi_escape_special(php, pos);
				}
			}
		}
	}

	return;
}

void
analyze_yo_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_YO_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_hi_escape_special(php, pos);
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
				php->fire[div] |= B_YO_FIRE_PATH;
				if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
					set_hi_escape_special(php, pos);
				}
			}
		}
	}

	return;
}

void
analyze_yo_run(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j, k;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_run_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j, k;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = yo_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = yo_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = yo_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_decapt_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = yo_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_YO_FIRE_PATH;
						}
						php->fire[fpos2] |= B_YO_FIRE_PATH;
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_hi_escape_special(php, pos);
						}
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_decapt_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 yo_trial_move2(php, KIND(php->board[pos]), pos, fpos, fpos2) == 0)) {
						php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = yo_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_YO_FIRE_PATH;
						}
						php->fire[fpos2] |= B_YO_FIRE_PATH;
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_hi_escape_special(php, pos);
						}
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_jump(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
							php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; k != i; k++) {
								fpos = yo_run_to_posv[fpc][pos][j][k];
								php->fire[fpos] |= B_YO_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_hi_escape_special(php, pos);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
								(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
								 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
							php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; k != i; k++) {
								fpos = yo_run_to_posv[fpc][pos][j][k];
								php->fire[fpos] |= B_YO_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_hi_escape_special(php, pos);
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_jump_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k, n;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
						php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = yo_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_YO_FIRE_PATH;
						}
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_hi_escape_special(php, pos);
						}
					}
					break;
				}
			}
		}
	}

	return;
}

void
analyze_yo_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k, n;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
						php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
						for (k = 0; k != i; k++) {
							fpos = yo_run_to_posv[fpc][pos][j][k];
							php->fire[fpos] |= B_YO_FIRE_PATH;
						}
						if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
							set_hi_escape_special(php, pos);
						}
					}
					break;
				}
			}
		}
	}

	return;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
void
analyze_yo_refl(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

void
analyze_yo_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, k;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
					for (k = 0; k != i; k++) {
						fpos = hi_run_to_posv[fpc][pos][j][k];
						php->fire[fpos] |= B_YO_FIRE_PATH;
					}
					if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
						set_hi_escape_special(php, pos);
					}
				}
				break;
			}
		}
	}

	return;
}

// Equihopper:
//   hi_leap_to_pos: 対称点に対する利き位置
//   yo_leap_to_pos: 利き位置に対する対称点
//   hi_run_to_posv[0]: 利き位置リスト
//   hi_run_to_posv[1]: 合駒の利く利き位置リスト
//   hi_run_to_posv[2...]: 合駒の利く利き位置に対する合駒位置リスト
//   yo_run_to_posv[0]: 対称点リスト
//   yo_run_to_posv[1]: 合駒の利く対称点リスト
//   hi_to_board: 対称点があれば利く位置
//   yo_to_board: 利く位置がある対称点
void
analyze_yo_equi_hop(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; fpos3[k] != EOP; k++) {
								php->fire[fpos3[k]] |= B_YO_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_hi_escape_special(php, pos);
							}
						}
					}
				} else {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
				}
			}
		}
	}

	return;
}

void
analyze_yo_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 yo_trial_move(php, KIND(php->board[pos]), pos, fpos[i]) == 0)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_YONDER_CHECK | B_CS_REMOTE_CHECK;
							for (k = 0; fpos3[k] != EOP; k++) {
								php->fire[fpos3[k]] |= B_YO_FIRE_PATH;
							}
							if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
								set_hi_escape_special(php, pos);
							}
						}
					}
				} else {
					php->check_state |= B_CS_YONDER_CHECK | B_CS_CLOSE_CHECK;
				}
			}
		}
	}

	return;
}

void
analyze_null(phase_t *php, int pos, uchar_t fpc)
{
	return;
}

// for Madrasi or Isardam
int
analyze_hi_leap_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = hi_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_YO_PC(fsq)) {
			if (KIND(fsq) == pc &&
					(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
				if (cond_flag & B_C_ISARDAM) {
					return -1;
				}
				php->fire[fpos] |= B_PALALYTIC;
			}
			if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
				php->fire[pos] |= B_CHECK;
			}
		}
	}

	return 0;
}

int
analyze_hi_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fnext, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fnext = hi_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fnext;
			fsq = php->board[fpos];
			if (hi_run_to_posv[fpc][pos][j][i + 1] == EOP) {
				if (IS_YO_PC(fsq)) {
					if (KIND(fsq) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
analyze_hi_mao_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq)) {
				if (KIND(fsq) == pc &&
						(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
					if (cond_flag & B_C_ISARDAM) {
						return -1;
					}
					php->fire[fpos] |= B_PALALYTIC;
				}
				if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
					php->fire[pos] |= B_CHECK;
				}
			}
		another_to:;
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq)) {
				if (KIND(fsq) == pc &&
						(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
					if (cond_flag & B_C_ISARDAM) {
						return -1;
					}
					php->fire[fpos] |= B_PALALYTIC;
				}
				if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
					php->fire[pos] |= B_CHECK;
				}
			}
		}
	}

	return 0;
}

int
analyze_hi_run_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_YO_PC(fsq)) {
					if (KIND(fsq) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_hi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = hi_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_hi_decapt_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq)) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						if (KIND(fsq) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_hi_jump_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_YO_PC(fsq2)) {
							if (KIND(fsq2) == pc &&
									(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
									 (cond_flag & B_C_K_MADRASI))) {
								if (cond_flag & B_C_ISARDAM) {
									return -1;
								}
								php->fire[fpos2] |= B_PALALYTIC;
							}
							if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
								php->fire[pos] |= B_CHECK;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_hi_jump_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_YO_PC(fsq2)) {
						if (KIND(fsq2) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos2] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
analyze_hi_refl_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

// Equihopper:
//   hi_leap_to_pos: 対称点に対する利き位置
//   yo_leap_to_pos: 利き位置に対する対称点
//   hi_run_to_posv[0]: 利き位置リスト
//   hi_run_to_posv[1]: 合駒の利く利き位置リスト
//   hi_run_to_posv[2...]: 合駒の利く利き位置に対する合駒位置リスト
//   yo_run_to_posv[0]: 対称点リスト
//   yo_run_to_posv[1]: 合駒の利く対称点リスト
//   hi_to_board: 対称点があれば利く位置
//   yo_to_board: 利く位置がある対称点
int
analyze_hi_equi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	// ★何故、利き位置はこちらの情報[0]で、中間跳躍位置はあちらの情報[0]なのか？
	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			// 中間跳躍位置になんらかの駒があった場合
			fsq = php->board[fpos[i]];
			if (IS_YO_PC(fsq)) {
				// 利き位置に相手王駒があった場合
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					// ただのEqui-hopperの場合
					// ★利き位置２のこちらの情報[1]とは、合駒の利く利き位置リスト
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						if (KIND(fsq) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos[i]] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							if (KIND(fsq) == pc &&
									(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
									 (cond_flag & B_C_K_MADRASI))) {
								if (cond_flag & B_C_ISARDAM) {
									return -1;
								}
								php->fire[fpos[i]] |= B_PALALYTIC;
							}
							if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
								php->fire[pos] |= B_CHECK;
							}
						}
					}
				} else {
					// Non-stop-equi-hopperの場合
					if (KIND(fsq) == pc) {
						php->fire[fpos[i]] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_yo_leap_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = yo_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_HI_PC(fsq)) {
			if (KIND(fsq) == pc &&
					(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
				if (cond_flag & B_C_ISARDAM) {
					return -1;
				}
				php->fire[fpos] |= B_PALALYTIC;
			}
			if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
				php->fire[pos] |= B_CHECK;
			}
		}
	}

	return 0;
}

int
analyze_yo_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fnext, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fnext = yo_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fnext;
			fsq = php->board[fpos];
			fnext = yo_run_to_posv[fpc][pos][j][i + 1];
			if (fnext == EOP) {
				if (IS_HI_PC(fsq)) {
					if (KIND(fsq) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
analyze_yo_mao_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq)) {
				if (KIND(fsq) == pc &&
						(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
					if (cond_flag & B_C_ISARDAM) {
						return -1;
					}
					php->fire[fpos] |= B_PALALYTIC;
				}
				if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
					php->fire[pos] |= B_CHECK;
				}
			}
		another_to:;
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq)) {
				if (KIND(fsq) == pc &&
						(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI))) {
					if (cond_flag & B_C_ISARDAM) {
						return -1;
					}
					php->fire[fpos] |= B_PALALYTIC;
				}
				if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
					php->fire[pos] |= B_CHECK;
				}
			}
		}
	}

	return 0;
}

int
analyze_yo_run_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (IS_ANY_PC(fsq)) {
				if (IS_HI_PC(fsq)) {
					if (KIND(fsq) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_yo_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = yo_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_yo_decapt_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq)) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						if (KIND(fsq) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_yo_jump_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_HI_PC(fsq2)) {
							if (KIND(fsq2) == pc &&
									(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
									 (cond_flag & B_C_K_MADRASI))) {
								if (cond_flag & B_C_ISARDAM) {
									return -1;
								}
								php->fire[fpos2] |= B_PALALYTIC;
							}
							if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
								php->fire[pos] |= B_CHECK;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_yo_jump_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_HI_PC(fsq2)) {
						if (KIND(fsq2) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos2] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
analyze_yo_refl_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2)) {
					if (KIND(fsq2) == pc &&
							(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
							 (cond_flag & B_C_K_MADRASI))) {
						if (cond_flag & B_C_ISARDAM) {
							return -1;
						}
						php->fire[fpos2] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq2)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
				break;
			}
		}
	}

	return 0;
}

// Equihopper:
//   hi_leap_to_pos: 対称点に対する利き位置
//   yo_leap_to_pos: 利き位置に対する対称点
//   hi_run_to_posv[0]: 利き位置リスト
//   hi_run_to_posv[1]: 合駒の利き位置リスト
//   hi_run_to_posv[2...]: 合駒の利き位置に対する合駒位置リスト
//   yo_run_to_posv[0]: 対称点リスト
//   yo_run_to_posv[1]: 合駒の利く対称点リスト
//   hi_to_board: 対称点があれば利く位置
//   yo_to_board: 利く位置がある対称点
int
analyze_yo_equi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_HI_PC(fsq)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						if (KIND(fsq) == pc &&
								(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
								 (cond_flag & B_C_K_MADRASI))) {
							if (cond_flag & B_C_ISARDAM) {
								return -1;
							}
							php->fire[fpos[i]] |= B_PALALYTIC;
						}
						if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
							php->fire[pos] |= B_CHECK;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							if (KIND(fsq) == pc &&
									(NOT(piece[KIND(pc)].attr & B_PA_ROYAL) ||
									 (cond_flag & B_C_K_MADRASI))) {
								if (cond_flag & B_C_ISARDAM) {
									return -1;
								}
								php->fire[fpos[i]] |= B_PALALYTIC;
							}
							if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
								php->fire[pos] |= B_CHECK;
							}
						}
					}
				} else {
					if (KIND(fsq) == pc) {
						php->fire[fpos[i]] |= B_PALALYTIC;
					}
					if (piece[KIND(fsq)].attr & B_PA_ROYAL) {
						php->fire[pos] |= B_CHECK;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_null_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc)
{
	return 0;
}

static void
set_yo_escape_special(phase_t *php, int pos)
{
	int pos1, pos2;

	switch (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
	case B_C_TAIMEN:
		if (fpc_pos_forward[pos] != NOP) {
			php->fire[yo_hi_ctrl_taimen[pos]] |= B_YO_ESCAPE_SPECIAL;
		}
		break;
	case B_C_HAIMEN:
		if (fpc_pos_back[pos] != NOP) {
			php->fire[yo_hi_ctrl_haimen[pos]] |= B_YO_ESCAPE_SPECIAL;
		}
		break;
	case B_C_NEKO2SEN:
		for (pos1 = pos; fpc_pos_forward[pos1] != NOP && IS_ANY_PC(php->board[pos1 - 1]); pos1--)
			;
		if (fpc_pos_forward[pos1] != NOP) {
			php->fire[fpc_pos_forward[pos1]] |= B_YO_ESCAPE_SPECIAL;
		}
		for (pos2 = pos; fpc_pos_back[pos2] != NOP && IS_ANY_PC(php->board[pos2 + 1]); pos2++)
			;
		if (fpc_pos_back[pos2] != NOP) {
			php->fire[fpc_pos_back[pos2]] |= B_YO_ESCAPE_SPECIAL;
		}
		break;
	}

	return;
}

static void
set_hi_escape_special(phase_t *php, int pos)
{
	int pos1, pos2;

	switch (cond_flag & (B_C_TAIMEN | B_C_HAIMEN | B_C_NEKO2SEN)) {
	case B_C_TAIMEN:
		if (fpc_pos_back[pos] != NOP) {
			php->fire[hi_yo_ctrl_taimen[pos]] |= B_HI_ESCAPE_SPECIAL;
		}
		break;
	case B_C_HAIMEN:
		if (fpc_pos_forward[pos] != NOP) {
			php->fire[hi_yo_ctrl_haimen[pos]] |= B_HI_ESCAPE_SPECIAL;
		}
		break;
	case B_C_NEKO2SEN:
		for (pos1 = pos; fpc_pos_forward[pos1] != NOP && IS_ANY_PC(php->board[pos1 - 1]); pos1--)
			;
		if (fpc_pos_forward[pos1] != NOP) {
			php->fire[fpc_pos_forward[pos1]] |= B_HI_ESCAPE_SPECIAL;
		}
		for (pos2 = pos; fpc_pos_back[pos2] != NOP && IS_ANY_PC(php->board[pos2 + 1]); pos2++)
			;
		if (fpc_pos_back[pos2] != NOP) {
			php->fire[fpc_pos_back[pos2]] |= B_HI_ESCAPE_SPECIAL;
		}
		break;
	}

	return;
}

int
analyze_leaf_phase_normal(phase_t *php)
{
	int pos;
	uchar_t bpc, pc;

	call_analyze_leaf_count++;
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			pc = KIND(bpc);
			switch (piece[pc].analyze_leaf_hi_leap(php, pos, pc)) {
			case -1:
				return 0;
			case 0:
				if (piece[pc].analyze_leaf_hi_run(php, pos, pc) == -1) {
					return 0;
				}
			}
			break;
		case B_PC_YONDER:
			pc = KIND(bpc);
			switch (piece[pc].analyze_leaf_yo_leap(php, pos, pc)) {
			case -1:
				return 0;
			case 0:
				if (piece[pc].analyze_leaf_yo_run(php, pos, pc) == -1) {
					return 0;
				}
			}
			break;
		}
	}

	return 0;
}

int
analyze_leaf_phase_change(phase_t *php)
{
	int pos;
	uchar_t bpc, fpc;

	call_analyze_leaf_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			fpc = php->fpc_board[pos];
			switch (piece[fpc].analyze_leaf_hi_leap(php, pos, fpc)) {
			case -1:
				return 0;
			case 0:
				if (piece[fpc].analyze_leaf_hi_run(php, pos, fpc) == -1) {
					return 0;
				}
			}
			break;
		case B_PC_YONDER:
			fpc = php->fpc_board[pos];
			switch (piece[fpc].analyze_leaf_yo_leap(php, pos, fpc)) {
			case -1:
				return 0;
			case 0:
				if (piece[fpc].analyze_leaf_yo_run(php, pos, fpc) == -1) {
					return 0;
				}
			}
			break;
		}
	}

	return 0;
}

int
analyze_leaf_phase_madrasi(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_leaf_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				piece[pc].analyze_hi_leap_madrasi(php, pos, pc, pc);
				piece[pc].analyze_hi_run_madrasi(php, pos, pc, pc);
			}
			break;
		case B_PC_YONDER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				piece[pc].analyze_yo_leap_madrasi(php, pos, pc, pc);
				piece[pc].analyze_yo_run_madrasi(php, pos, pc, pc);
			}
			break;
		}
	}
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
            if (NOT(php->fire[pos] & B_PALALYTIC)) {
                fpc = php->fpc_board[pos];
                switch (piece[fpc].analyze_leaf_hi_leap(php, pos, fpc)) {
                case -1:
                    return 0;
                case 0:
                    if (piece[fpc].analyze_leaf_hi_run(php, pos, fpc) == -1) {
                        return 0;
                    }
                }
            }
			break;
		case B_PC_YONDER:
            if (NOT(php->fire[pos] & B_PALALYTIC)) {
                fpc = php->fpc_board[pos];
                switch (piece[fpc].analyze_leaf_yo_leap(php, pos, fpc)) {
                case -1:
                    return 0;
                case 0:
                    if (piece[fpc].analyze_leaf_yo_run(php, pos, fpc) == -1) {
                        return 0;
                    }
                }
            }
			break;
		}
	}

	return 0;
}

int
analyze_leaf_phase_change_madrasi(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_leaf_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				fpc = php->fpc_board[pos];
				piece[fpc].analyze_hi_leap_madrasi(php, pos, pc, fpc);
				piece[fpc].analyze_hi_run_madrasi(php, pos, pc, fpc);
			}
			break;
		case B_PC_YONDER:
			pc = KIND(bpc);
			if (NOT(piece[pc].attr & B_PA_ROYAL) || (cond_flag & B_C_K_MADRASI)) {
				// 石化について調べる
				fpc = php->fpc_board[pos];
				piece[fpc].analyze_yo_leap_madrasi(php, pos, pc, fpc);
				piece[fpc].analyze_yo_run_madrasi(php, pos, pc, fpc);
			}
			break;
		}
	}
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
            if (NOT(php->fire[pos] & B_PALALYTIC)) {
                fpc = php->fpc_board[pos];
                switch (piece[fpc].analyze_leaf_hi_leap(php, pos, fpc)) {
                case -1:
                    return 0;
                case 0:
                    if (piece[fpc].analyze_leaf_hi_run(php, pos, fpc) == -1) {
                        return 0;
                    }
                }
			}
			break;
		case B_PC_YONDER:
            if (NOT(php->fire[pos] & B_PALALYTIC)) {
                fpc = php->fpc_board[pos];
                switch (piece[fpc].analyze_leaf_yo_leap(php, pos, fpc)) {
                case -1:
                    return 0;
                case 0:
                    if (piece[fpc].analyze_leaf_yo_run(php, pos, fpc) == -1) {
                        return 0;
                    }
                }
			}
			break;
		}
	}

	return 0;
}

int
analyze_leaf_phase_isardam(phase_t *php)
{
	int pos;
	uchar_t bpc, pc, fpc;

	call_analyze_leaf_count++;
	setup_fpc_board(php);
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			// 石化について調べる
			pc = KIND(bpc);
			if (piece[pc].analyze_hi_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_hi_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		case B_PC_YONDER:
			// 石化について調べる
			pc = KIND(bpc);
			if (piece[pc].analyze_yo_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_yo_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		}
	}
	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		switch (bpc & BM_PC_TURN) {
		case B_PC_HITHER:
			fpc = php->fpc_board[pos];
			switch (piece[fpc].analyze_leaf_hi_leap(php, pos, fpc)) {
			case -1:
				return 0;
			case 0:
				if (piece[fpc].analyze_leaf_hi_run(php, pos, fpc) == -1) {
					return 0;
				}
			}
			break;
		case B_PC_YONDER:
			fpc = php->fpc_board[pos];
			switch (piece[fpc].analyze_leaf_yo_leap(php, pos, fpc)) {
			case -1:
				return 0;
			case 0:
				if (piece[fpc].analyze_leaf_yo_run(php, pos, fpc) == -1) {
					return 0;
				}
			}
			break;
		}
	}

	return 0;
}

int
analyze_leaf_hi_leap(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = hi_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
			php->check_state |= B_CS_HITHER_CHECK;
			if (php->check_state & B_CS_YONDER_CHECK) {
				return -1;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = hi_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
				(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
				 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
			php->check_state |= B_CS_HITHER_CHECK;
			if (php->check_state & B_CS_YONDER_CHECK) {
				return -1;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_step(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = hi_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = hi_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = hi_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = hi_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
analyze_leaf_hi_mao_step(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_HITHER_CHECK;
				if (php->check_state & B_CS_YONDER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_HITHER_CHECK;
				if (php->check_state & B_CS_YONDER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_HITHER_CHECK;
				if (php->check_state & B_CS_YONDER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_HITHER_CHECK;
				if (php->check_state & B_CS_YONDER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_run(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_run_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = hi_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = hi_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_decapt_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_decapt_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 hi_trial_move2(php, KIND(php->board[pos]), pos, fpos, fpos2) == 0)) {
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_jump(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
							php->check_state |= B_CS_HITHER_CHECK;
							if (php->check_state & B_CS_YONDER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
								(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
								 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
							php->check_state |= B_CS_HITHER_CHECK;
							if (php->check_state & B_CS_YONDER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_jump_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
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
analyze_leaf_hi_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = hi_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
analyze_leaf_hi_refl(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_YO_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 hi_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
int
analyze_leaf_hi_equi_hop(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_HITHER_CHECK;
							if (php->check_state & B_CS_YONDER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
					}
				} else {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_hi_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_YO_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 hi_trial_move(php, KIND(php->board[pos]), pos, fpos[i]) == 0)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_HITHER_CHECK;
						if (php->check_state & B_CS_YONDER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_HITHER_CHECK;
							if (php->check_state & B_CS_YONDER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
					}
				} else {
					php->check_state |= B_CS_HITHER_CHECK;
					if (php->check_state & B_CS_YONDER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_leap(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = yo_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
			php->check_state |= B_CS_YONDER_CHECK;
			if (php->check_state & B_CS_HITHER_CHECK) {
				return -1;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i;
	uchar_t fsq;

	for (i = 0; ; i++) {
		fpos = yo_leap_to_pos[fpc][pos][i];
		if (fpos == EOP) {
			break;
		}
		fsq = php->board[fpos];
		if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
				(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
				 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
			php->check_state |= B_CS_YONDER_CHECK;
			if (php->check_state & B_CS_HITHER_CHECK) {
				return -1;
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_step(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = yo_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = yo_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fire_next, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0, fire_next = yo_run_to_posv[fpc][pos][j][0]; ; i++) {
			fpos = fire_next;
			fsq = php->board[fpos];
			fire_next = yo_run_to_posv[fpc][pos][j][i + 1];
			if (fire_next == EOP) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
			if (NOT_PASSABLE(fsq)) {
				break;
			}
		}
	}

	return 0;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
analyze_leaf_yo_mao_step(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_YONDER_CHECK;
				if (php->check_state & B_CS_HITHER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				php->check_state |= B_CS_YONDER_CHECK;
				if (php->check_state & B_CS_HITHER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int div, fpos, i;
	uchar_t fsq;

	for (i = 0; hi_run_to_posv[fpc][pos][i] != NULL; i++) {
		div = hi_run_to_posv[fpc][pos][i][0];
		if (div == EOP) {
			break;
		}
		if (IS_PASSABLE(php->board[div])) {
			fpos = yo_run_to_posv[fpc][pos][div][0];
			if (fpos == EOP) {
				goto another_to;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_YONDER_CHECK;
				if (php->check_state & B_CS_HITHER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		another_to:
			fpos = yo_run_to_posv[fpc][pos][div][1];
			if (fpos == EOP) {
				continue;
			}
			fsq = php->board[fpos];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
				php->check_state |= B_CS_YONDER_CHECK;
				if (php->check_state & B_CS_HITHER_CHECK) {
					return -1;
				} else {
					return 1;
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_run(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_run_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, i, j;
	uchar_t fsq;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = yo_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				i++;
				fpos2 = yo_run_to_posv[fpc][pos][j][i];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_decapt_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2)) {
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_decapt_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_ROOM(fsq2) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 yo_trial_move2(php, KIND(php->board[pos]), pos, fpos, fpos2) == 0)) {
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_jump(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
							php->check_state |= B_CS_YONDER_CHECK;
							if (php->check_state & B_CS_HITHER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				for (i++; ; i++) {
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (NOT_PASSABLE(fsq2)) {
						if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
								(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
								 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
							php->check_state |= B_CS_YONDER_CHECK;
							if (php->check_state & B_CS_HITHER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_jump_hop(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
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
analyze_leaf_yo_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j, n;
	uchar_t fsq, fsq2;

	for (j = 0; yo_run_to_posv[fpc][pos][j] != NULL; j++) {
		n = 0;
		for (i = 0; ; i++) {
			fpos = yo_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				n++;
				if (n >= 2) {
					i++;
					fpos2 = yo_run_to_posv[fpc][pos][j][i];
					if (fpos2 == EOP) {
						break;
					}
					fsq2 = php->board[fpos2];
					if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
							(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
							 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					}
					break;
				}
			}
		}
	}

	return 0;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
analyze_leaf_yo_refl(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	int fpos, fpos2, i, j;
	uchar_t fsq, fsq2;

	for (j = 0; hi_run_to_posv[fpc][pos][j] != NULL; j++) {
		for (i = 0; ; i++) {
			fpos = hi_run_to_posv[fpc][pos][j][i];
			if (fpos == EOP) {
				break;
			}
			fsq = php->board[fpos];
			if (NOT_PASSABLE(fsq)) {
				fpos2 = yo_run_to_posv[fpc][pos][fpos][0];
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				fpos2 = yo_run_to_posv[fpc][pos][fpos][1];
				if (fpos2 == EOP) {
					break;
				}
				fsq2 = php->board[fpos2];
				if (IS_HI_PC(fsq2) && (piece[KIND(fsq2)].attr & B_PA_ROYAL) &&
						(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
						 yo_trial_move(php, KIND(php->board[pos]), pos, fpos) == 0)) {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
				break;
			}
		}
	}

	return 0;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
int
analyze_leaf_yo_equi_hop(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_YONDER_CHECK;
							if (php->check_state & B_CS_HITHER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
					}
				} else {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_yo_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc)
{
	char *fpos, *sym_pos;
	int i;
	uchar_t fsq;

	fpos = hi_run_to_posv[fpc][pos][0];
	sym_pos = yo_run_to_posv[fpc][pos][0];
	for (i = 0; fpos[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			fsq = php->board[fpos[i]];
			if (IS_HI_PC(fsq) && (piece[KIND(fsq)].attr & B_PA_ROYAL) &&
					(NOT(cond_flag & B_C_CONSTRAINT_1ST) ||
					 yo_trial_move(php, KIND(php->board[pos]), pos, fpos[i]) == 0)) {
				if (piece[fpc].move_style & B_MS_EQUI_JUMP) {
					char *fpos2 = hi_run_to_posv[fpc][pos][1];
					int j;
					for (j = 0; fpos2[j] != EOP; j++) {
						if (fpos2[j] == fpos[i]) {
							break;
						}
					}
					if (fpos2[j] == EOP) { 
						php->check_state |= B_CS_YONDER_CHECK;
						if (php->check_state & B_CS_HITHER_CHECK) {
							return -1;
						} else {
							return 1;
						}
					} else {
						char *fpos3 = hi_run_to_posv[fpc][pos][j + 2];
						int k;
						for (k = 0; fpos3[k] != EOP; k++) {
							if (NOT_PASSABLE(php->board[fpos3[k]])) {
								break;
							}
						}
						if (fpos3[k] == EOP) {
							php->check_state |= B_CS_YONDER_CHECK;
							if (php->check_state & B_CS_HITHER_CHECK) {
								return -1;
							} else {
								return 1;
							}
						}
					}
				} else {
					php->check_state |= B_CS_YONDER_CHECK;
					if (php->check_state & B_CS_HITHER_CHECK) {
						return -1;
					} else {
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int
analyze_leaf_null(phase_t *php, int pos, uchar_t fpc)
{
	return 0;
}

void
setup_fpc_board_normal(phase_t *php)
{
	int pos;

	for (pos = 0; pos < 81; pos++) {
		php->fpc_board[pos] &= BM_SQ_PC;
	}

	return;
}

void
setup_fpc_board_annan(phase_t *php)
{
	int pos, xpos;
	uchar_t bpc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos + 1;
//			if (POS19_RANK(pos) <= 8 && IS_HI_PC(php->board[xpos])) {
			if (POS_RANK(pos) <= 7) {
				if (IS_HI_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_HI_CHECK_SPECIAL;
			}
//			if (POS19_RANK(pos) >= 2) {
			if (POS_RANK(pos) >= 1) {
				php->fire[pos - 1] |= B_HI_CHECK_SPECIAL;
			}
		} else if (IS_YO_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos - 1;
//			if (POS19_RANK(pos) >= 2 && IS_YO_PC(php->board[xpos])) {
			if (POS_RANK(pos) >= 1) {
				if (IS_YO_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_YO_CHECK_SPECIAL;
			}
//			if (POS19_RANK(pos) <= 8) {
			if (POS_RANK(pos) <= 7) {
				php->fire[pos + 1] |= B_YO_CHECK_SPECIAL;
			}
		}
	}

	return;
}

void
setup_fpc_board_anhoku(phase_t *php)
{
	int pos, xpos;
	uchar_t bpc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos - 1;
//			if (POS19_RANK(pos) >= 2 && IS_HI_PC(php->board[xpos])) {
			if (POS_RANK(pos) >= 1) {
				if (IS_HI_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_HI_CHECK_SPECIAL;
			}
//			if (POS19_RANK(pos) <= 8) {
			if (POS_RANK(pos) <= 7) {
				php->fire[pos + 1] |= B_HI_CHECK_SPECIAL;
			}
		} else if (IS_YO_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos + 1;
//			if (POS19_RANK(pos) <= 8 && IS_YO_PC(php->board[xpos])) {
			if (POS_RANK(pos) <= 7) {
				if (IS_YO_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_YO_CHECK_SPECIAL;
			}
//			if (POS19_RANK(pos) >= 2) {
			if (POS_RANK(pos) >= 1) {
				php->fire[pos - 1] |= B_YO_CHECK_SPECIAL;
			}
		}
	}

	return;
}

void
setup_fpc_board_taimen(phase_t *php)
{
	int pos, xpos;
	uchar_t bpc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos - 1;
//			if (POS19_RANK(pos) >= 2 && IS_YO_PC(php->board[xpos])) {
			if (POS_RANK(pos) >= 1) {
				if (IS_YO_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_YO_CHECK_SPECIAL;
			}
		} else if (IS_YO_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos + 1;
//			if (POS19_RANK(pos) <= 8 && IS_HI_PC(php->board[xpos])) {
			if (POS_RANK(pos) <= 7) {
				if (IS_HI_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_HI_CHECK_SPECIAL;
			}
		}
	}

	return;
}

void
setup_fpc_board_haimen(phase_t *php)
{
	int pos, xpos;
	uchar_t bpc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos + 1;
//			if (POS19_RANK(pos) <= 8 && IS_YO_PC(php->board[xpos])) {
			if (POS_RANK(pos) <= 7) {
				if (IS_YO_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_YO_CHECK_SPECIAL;
			}
		} else if (IS_YO_PC(bpc)) {
			php->fpc_board[pos] &= BM_SQ_PC;
			xpos = pos - 1;
//			if (POS19_RANK(pos) >= 2 && IS_HI_PC(php->board[xpos])) {
			if (POS_RANK(pos) >= 1) {
				if (IS_HI_PC(php->board[xpos])) {
					php->fpc_board[pos] = KIND(php->board[xpos]);
				}
				php->fire[xpos] |= B_HI_CHECK_SPECIAL;
			}
		}
	}

	return;
}

void
setup_fpc_board_nekosen(phase_t *php)
{
	int pos, pf, pr, r, rr;
	uchar_t bpc;

	for (pos = 0; pos < 81; pos++) {
		if (NOT(php->fpc_board[pos] & (B_PC_HITHER | B_PC_YONDER))) {
			continue;
		}
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			pf = POS_FILE(pos);
			pr = POS_RANK(pos);
			for (r = pr + 1; r <= 8; r++) {
				if (NOT(IS_HI_PC(php->board[POS(pf, r)]))) {
					break;
				}
			}
			r--;
			for (rr = pr; rr <= r; rr++) {
				php->fpc_board[POS(pf, rr)] = KIND(php->board[POS(pf, pr + r - rr)]);
			}
			if (pr >= 1) {
				php->fire[POS(pf, pr - 1)] |= B_HI_CHECK_SPECIAL;
			}
			if (r <= 7) {
				php->fire[POS(pf, r + 1)] |= B_HI_CHECK_SPECIAL;
			}
		} else if (IS_YO_PC(bpc)) {
			pf = POS_FILE(pos);
			pr = POS_RANK(pos);
			for (r = pr + 1; r <= 8; r++) {
				if (NOT(IS_YO_PC(php->board[POS(pf, r)]))) {
					break;
				}
			}
			r--;
			for (rr = pr; rr <= r; rr++) {
				php->fpc_board[POS(pf, rr)] = KIND(php->board[POS(pf, pr + r - rr)]);
			}
			if (pr >= 1) {
				php->fire[POS(pf, pr - 1)] |= B_YO_CHECK_SPECIAL;
			}
			if (r <= 7) {
				php->fire[POS(pf, r + 1)] |= B_YO_CHECK_SPECIAL;
			}
		}
	}

	return;
}

void
setup_fpc_board_neko2(phase_t *php)
{
	int pos, pf, pr, r, rr;

	for (pos = 0; pos < 81; pos++) {
		if (NOT(php->fpc_board[pos] & (B_PC_HITHER | B_PC_YONDER))) {
			continue;
		}
		if (IS_ANY_PC(php->board[pos])) {
			pf = POS_FILE(pos);
			pr = POS_RANK(pos);
			for (r = pr + 1; r <= 8; r++) {
				if (IS_ROOM(php->board[POS(pf, r)])) {
					break;
				}
			}
			r--;
			for (rr = pr; rr <= r; rr++) {
				php->fpc_board[POS(pf, rr)] = KIND(php->board[POS(pf, pr + r - rr)]);
			}
			if (pr >= 1) {
				php->fire[POS(pf, pr - 1)] |= B_HI_CHECK_SPECIAL | B_YO_CHECK_SPECIAL;
			}
			if (pr <= 7) {
				php->fire[POS(pf, r + 1)] |= B_HI_CHECK_SPECIAL | B_YO_CHECK_SPECIAL;
			}
		}
	}

	return;
}

static void
setup_fpc_board_madrasi(phase_t *php)
{
	int pos;

	for (pos = 0; pos < 81; pos++) {
		if (php->fire[pos] & B_CHECK) {
			if (IS_HI_PC(php->board[pos])) {
				uchar_t pc = KIND(php->board[pos]);
				if (php->fire[pos] & B_PALALYTIC) {
					php->state |= B_CS_CHECK_PALALYTIC;
					if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
						int fpos, i, j;
						for (j = 0; hi_run_to_posv[pc][pos][j] != NULL; j++) {
							for (i = 0; ; i++) {
								fpos = hi_run_to_posv[pc][pos][j][i];
								if (fpos == EOP || NOT_PASSABLE(php->board[fpos])) {
									break;
								}
								php->fire[fpos] |= B_HI_CHECK_SPECIAL;
							}
						}
					}
				} else {
					if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
						int fpos, i;
						for (i = 0; ; i++) {
							fpos = hi_leap_to_pos[pc][pos][i];
							if (fpos == EOP) {
								break;
							}
							php->fire[fpos] |= B_YO_ESCAPE_SPECIAL;
						}
					}
					if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
						int fpos, i, j;
						for (j = 0; hi_run_to_posv[pc][pos][j] != NULL; j++) {
							for (i = 0; ; i++) {
								fpos = hi_run_to_posv[pc][pos][j][i];
								if (fpos == EOP || NOT_PASSABLE(php->board[fpos])) {
									break;
								}
								php->fire[fpos] |= B_YO_ESCAPE_SPECIAL;
							}
						}
					}
				}
			} else {	// IS_YO_PC
				uchar_t pc = KIND(php->board[pos]);
				if (php->fire[pos] & B_PALALYTIC) {
					php->state |= B_CS_CHECK_PALALYTIC;
					if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
						int fpos, i, j;
						for (j = 0; yo_run_to_posv[pc][pos][j] != NULL; j++) {
							for (i = 0; ; i++) {
								fpos = yo_run_to_posv[pc][pos][j][i];
								if (fpos == EOP || NOT_PASSABLE(php->board[fpos])) {
									break;
								}
								php->fire[fpos] |= B_YO_CHECK_SPECIAL;
							}
						}
					}
				} else {
					if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
						int fpos, i;
						for (i = 0; ; i++) {
							fpos = yo_leap_to_pos[pc][pos][i];
							if (fpos == EOP) {
								break;
							}
							php->fire[fpos] |= B_HI_ESCAPE_SPECIAL;
						}
					}
					if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
						int fpos, i, j;
						for (j = 0; yo_run_to_posv[pc][pos][j] != NULL; j++) {
							for (i = 0; ; i++) {
								fpos = yo_run_to_posv[pc][pos][j][i];
								if (fpos == EOP || NOT_PASSABLE(php->board[fpos])) {
									break;
								}
								php->fire[fpos] |= B_HI_ESCAPE_SPECIAL;
							}
						}
					}
				}
			}
		}
		if (php->fire[pos] & B_PALALYTIC) {
			php->fpc_board[pos] = ROCK;
		}
	}

	return;
}

static int
phase_constraint_isardam(phase_t *php)
{
	int pos;
	uchar_t bsq, pc;

	for (pos = 0; pos < 81; pos++) {
		bsq = php->board[pos];
		switch (bsq & BM_PC_TURN) {
		case B_PC_HITHER:
			// 石化について検査
			pc = KIND(bsq);
			if (piece[pc].analyze_hi_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_hi_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		case B_PC_YONDER:
			// 石化について検査
			pc = KIND(bsq);
			if (piece[pc].analyze_yo_leap_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			if (piece[pc].analyze_yo_run_madrasi(php, pos, pc, pc) != 0) {
				return -1;
			}
			break;
		}
	}

	return 0;
}

int
hi_trial_move_isardam(phase_t *php, uchar_t pc, int from, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = NOP;
	if (IS_PROMOTABLE(pc) && IS_HI_PROMOTABLE_MOVE(to, from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (hi_move_leaf(php) == 0) {
			if (phase_constraint_isardam(php + 1) == 0) {
				return 0;
			}
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (hi_move_leaf(php) == 0) {
				if (phase_constraint_isardam(php + 1) == 0) {
					return 0;
				}
			}
		}
	}
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move_leaf(php) == 0) {
		if (phase_constraint_isardam(php + 1) == 0) {
			return 0;
		}
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (yo_move_leaf(php) == 0) {
			if (phase_constraint_isardam(php + 1) == 0) {
				return 0;
			}
		}
	}

	return -1;
}

int
yo_trial_move_isardam(phase_t *php, uchar_t pc, int from, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = NOP;
	if (IS_PROMOTABLE(pc) && IS_YO_PROMOTABLE_MOVE(to, from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move_leaf(php) == 0) {
			if (phase_constraint_isardam(php + 1) == 0) {
				return 0;
			}
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move_leaf(php) == 0) {
				if (phase_constraint_isardam(php + 1) == 0) {
					return 0;
				}
			}
		}
	}
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move_leaf(php) == 0) {
		if (phase_constraint_isardam(php + 1) == 0) {
			return 0;
		}
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (yo_move_leaf(php) == 0) {
			if (phase_constraint_isardam(php + 1) == 0) {
				return 0;
			}
		}
	}

	return -1;
}

// ★Kokoでは獅子跳び系駒は未サポート
int
hi_trial_move_normal(phase_t *php, uchar_t pc, int from, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = NOP;
	if (IS_PROMOTABLE(pc) && IS_HI_PROMOTABLE_MOVE(to, from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (hi_move_leaf(php) == 0) {
			return 0;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (hi_move_leaf(php) == 0) {
				return 0;
			}
		}
	}
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move_leaf(php) == 0) {
		return 0;
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (hi_move_leaf(php) == 0) {
			return 0;
		}
	}

	return -1;
}

int
yo_trial_move_normal(phase_t *php, uchar_t pc, int from, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = NOP;
	if (IS_PROMOTABLE(pc) && IS_YO_PROMOTABLE_MOVE(to, from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move_leaf(php) == 0) {
			return 0;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move_leaf(php) == 0) {
				return 0;
			}
		}
	}
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move_leaf(php) == 0) {
		return 0;
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (yo_move_leaf(php) == 0) {
			return 0;
		}
	}

	return -1;
}

// ★獅子跳び系駒は未サポート
int
hi_trial_move2_normal(phase_t *php, uchar_t pc, int from, int to_1st, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = to_1st;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move_leaf(php) == 0) {
		return 0;
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (hi_move_leaf(php) == 0) {
			return 0;
		}
	}

	return -1;
}

int
yo_trial_move2_normal(phase_t *php, uchar_t pc, int from, int to_1st, int to)
{
	move_t *mvp = &php->move;

	mvp->pc = pc;
	mvp->from = from;
	mvp->to = to;
	mvp->to_1st = to_1st;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move_leaf(php) == 0) {
		return 0;
	}
	if (mvp->flag2 & B_MV2_REBORN_MORE) {
		if (yo_move_leaf(php) == 0) {
			return 0;
		}
	}

	return -1;
}

#if 0
if (OFF) {
	int pr, pf;
	uchar_t sq;
	printf("board fpc_board\n");
	        12345678901234567890123456789□234567890
	printf("+---------------------------+         +---------------------------+\n");
	for (pr = 1; pr <= 9; pr++) {
		printf("|");
		for (pf = 9; pf >= 1; pf--) {
			sq = php->board[POS19(pf, pr)];
			printf(" %02x", sq);
		}
		printf("|%s       |", kanji_num[pr - 1]);
		for (pf = 9; pf >= 1; pf--) {
			sq = php->fpc_board[POS19(pf, pr)];
			printf(" %02x", sq);
		}
		printf("|%s\n", kanji_num[pr - 1]);
	}
	printf("+---------------------------+         +---------------------------+\n");
//	printf("fpc_board\n");
//	printf("+---------------------------+\n");
//	for (pr = 1; pr <= 9; pr++) {
//		printf("|");
//		for (pf = 9; pf >= 1; pf--) {
//			sq = php->fpc_board[POS19(pf, pr)];
//			printf(" %02x", sq);
//		}
//		printf("|%s\n", kanji_num[pr - 1]);
//	}
//	printf("+---------------------------+\n");
	printf("fire\n");
	printf("+---------------------------+\n");
	for (pr = 1; pr <= 9; pr++) {
		printf("|");
		for (pf = 9; pf >= 1; pf--) {
			sq = php->fire[POS19(pf, pr)];
			printf(" %02x", sq);
		}
		printf("|%s\n", kanji_num[pr - 1]);
	}
	printf("+---------------------------+\n");
}
#endif
