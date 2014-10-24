// za/d_escape.c -- zoo+single-change direct-mate escape routines
// Copyright(c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/d_escape.c 1.20 2014/08/18 12:54:29 budai10 Exp budai10 $

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

// rc:
//   0:  反則（処理対象外を含む）
//   1:  逃れ（中断を含む）
//   2:  詰
// -----------------------------------------------------------------------------
#define	DIRECT_ESCAPE_PART_1 \
	depth++; \
	rc = RC_FAIL; \
 \
	/* 受方局面を解析 */ \
	if (analyze_phase(php) != 0) { \
		goto return_rc; \
	} \
 \
	/* ルール上の検索打切り条件（王手回避チェック） */ \
	if (php->check_state & B_CS_YONDER_CHECK) { \
		goto return_rc; \
	} \
 \
	/* ルール上の検索打切り条件（王手義務チェック） */ \
	/* ただし、受先の場合には初形では王手状態でないことを考慮 */ \
	if (NOT(php->check_state & B_CS_HITHER_CHECK) && depth >= 2) { \
		goto return_rc; \
	} \
 \
	/* 王手成立を設定 */ \
	if (depth >= 2) { \
		(php - 1)->state |= B_CHECK_MOVE_FOUND; \
	} \
 \
	/* 解析状況をチェック（解析状況表示） */ \
	if (++phase_count >= check_point) { \
		mile_stone(); \
	} \
 \
	/* 検索局面削減：局面の対称性チェック */ \
	if (((php - 1)->state & B_SYMMETRIC) && depth >= 2 && test_symmetric(php) == 0) { \
		goto return_rc; \
	} \
 \
	/* トレース上限の検索打切り条件 */ \
	if (trace_limit >= 1 && depth >= 2 && trace_move(php) != 0) { \
		goto return_rc; \
	} \
 \
	/* 打歩詰のとき、指定詰手数の１手前で持歩ゼロかチェック */ \
	/* ★打歩詰の場合、燕も考慮する必要がある。 */ \
	if (depth == limit_depth - 1 && (cond_flag & B_C_UCHIFU) && php->hi_hand[PC_FU] == 0) { \
		if ((php - 1)->move.pc == PC_FU && (php - 1)->move.from == NOP && is_yonder_mate(php)) { \
			rc = direct_mate_rule_check(php); \
		} else { \
			rc = RC_ESCAPE; \
		} \
		goto return_rc; \
	} \
 \
	/* 検索局面削減：攻方の同一局面をチェック、登録 */ \
	if (depth < limit_depth && (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME))) { \
		hi_hash_diff(php); \
		if ((strategy & B_HI_DENY_SAME)) { \
			/* 手順中同一局面をチェック、攻方の不詰同一局面をチェック */ \
			/* 同一局面でなかった場合に、手順中同一局面を登録 */ \
			int is_same = is_hi_same_phase(php); \
			if (is_same != 0) { \
				rc = RC_ESCAPE; \
				goto return_rc; \
			} \
		} \
	} \
 \
	/* 詰手数を超えていれば、詰をチェック */ \
	if (depth > limit_depth) { \
		if (is_yonder_mate(php)) { \
			rc = direct_mate_rule_check(php); \
		} else { \
			rc = RC_ESCAPE; \
		} \
		goto return_rc; \
	}
// -----------------------------------------------------------------------------
#define	DIRECT_ESCAPE_PART_2 \
	if (rc == RC_FAIL) { \
		/* 王手回避手が検出できなければ、詰をチェック */ \
		if (is_yonder_mate(php) && direct_mate_rule_check(php) == RC_MATE) { \
			rc = RC_MATE; \
		} else { \
			rc = RC_ESCAPE; \
		} \
	} \
 \
	/* 詰手順を出力 */ \
	if (depth == 1) { \
		if (rc == RC_MATE && trace_limit == 0) { \
			print_direct_mate(); \
			remove_mate_move_tree(php); \
		} \
	} \
 \
	/* 検索局面削減：攻方の同一局面を登録、解除 */ \
	if ((strategy & B_HI_DENY_SAME) && depth < limit_depth) { \
		ulong_t index; \
 \
		/* 手順中同一局面チェック用ハッシュ値を解除 */ \
		index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands; \
		index &= NOSPHHEAD - 1; \
		hi_sph_head[index] = hi_sph_head[index]->next; \
		/* 詰手順中の局面でなければ、攻方の不詰同一局面を登録 */ \
		/* （解数上限超えの場合を除く） */ \
		if ((strategy & B_HI_ESCAPE) && rc == RC_ESCAPE) { \
			set_hi_eh_cell(php); \
		} \
	} \
 \
return_rc:; \
	if (rc == RC_ESCAPE && ((php - 1)->move.flag & B_MV_MATESEQ)) { \
		remove_mate_move_tree(php - 1); \
	} \
 \
	depth--; \
	return rc;
// -----------------------------------------------------------------------------

static int direct_escape_exchange(phase_t *php);

// rc:
//   0:  反則（処理対象外を含む）
//   1:  逃れ（中断を含む）
//   2:  詰
int
direct_escape_normal(phase_t *php)
{
	int rc;

	DIRECT_ESCAPE_PART_1
	
	// 王手回避手検索
	rc = direct_escape_all(php);

	DIRECT_ESCAPE_PART_2
}

int
direct_escape_messigny(phase_t *php)
{
	int rc;

	DIRECT_ESCAPE_PART_1

	// 王手回避手検索
	rc = direct_escape_all_messigny(php);

	DIRECT_ESCAPE_PART_2
}

int
direct_escape_greed(phase_t *php)
{
	int rc;

	DIRECT_ESCAPE_PART_1
	
	// 王手回避手検索
	rc = direct_escape_all_greed(php);
	if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
		rc = direct_escape_all_ascetic(php);
	}

	DIRECT_ESCAPE_PART_2
}

int
direct_escape_ascetic(phase_t *php)
{
	int rc;

	DIRECT_ESCAPE_PART_1
	
	// 王手回避手検索
	rc = direct_escape_all_ascetic(php);
	if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
		rc = direct_escape_all_greed(php);
	}

	DIRECT_ESCAPE_PART_2
}

int
direct_escape_all(phase_t *php)
{
	move_t *mvp;
	int pos, i, rc;
	uchar_t bpc;

	rc = RC_FAIL;
	mvp = &php->move;
	mvp->to_1st = NOP;
	php->state |= B_MODE_GREED;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_GREED;
	php->state |= B_MODE_ASCETIC;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			switch (direct_escape_place(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		} else if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_ASCETIC;

	return rc;
}

int
direct_escape_all_messigny(phase_t *php)
{
	move_t *mvp;
	int pos, i, rc;
	uchar_t bpc;

	rc = RC_FAIL;
	mvp = &php->move;
	mvp->to_1st = NOP;
	php->state |= B_MODE_GREED;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_GREED;
	php->state |= B_MODE_ASCETIC;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			switch (direct_escape_place(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		} else if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (direct_escape_exchange(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_ASCETIC;

	return rc;
}

int
direct_escape_all_greed(phase_t *php)
{
	move_t *mvp;
	int pos, i, rc;
	uchar_t bpc;

	rc = RC_FAIL;
	php->state |= B_MODE_GREED;
	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_GREED;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_GREED;

	return rc;
}

int
direct_escape_all_ascetic(phase_t *php)
{
	move_t *mvp;
	int pos, i, rc;
	uchar_t bpc;

	rc = RC_FAIL;
	php->state |= B_MODE_ASCETIC;
	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			switch (direct_escape_place(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		} else if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			switch (piece[mvp->fpc].direct_escape_leap(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			switch (piece[mvp->fpc].direct_escape_run(php)) {
			case RC_ESCAPE:
				php->state &= ~B_MODE_ASCETIC;
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}
	php->state &= ~B_MODE_ASCETIC;

	return rc;
}

int
direct_escape_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc, i;
	uchar_t pc;

	rc = RC_FAIL;
	for (i = 0; NOT_NUL(hand_pc[i]); i++) {
		pc = hand_pc[i];
		if (php->yo_hand[pc] > 0) {
			mvp->pc = mvp->fpc = pc;
			switch (direct_escape_place_sub(php)) {
			case RC_ESCAPE:
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc, i;
	uchar_t pc;

	rc = RC_FAIL;
	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(depth == 1 && NOT(php->check_state & B_CS_HITHER_CHECK))) {	// 受先の初手用ルート
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] > 0) {
				mvp->pc = mvp->fpc = pc;
				switch (direct_escape_place_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
				}
			}
		}
	}

	return rc;
}

// 対面、背面、ネコネコ鮮、マドラシ系
int
direct_escape_place_taihaink2mad(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc, i;
	uchar_t pc;

	rc = RC_FAIL;
	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(php->fire[mvp->to] & B_YO_ESCAPE_SPECIAL) ||
			(depth == 1 && NOT(php->check_state & B_CS_HITHER_CHECK))) {	// 受先の初手用ルート
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] > 0) {
				mvp->pc = mvp->fpc = pc;
				switch (direct_escape_place_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		}
	}

	return rc;
}

static int
direct_escape_exchange(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, rc;
	uchar_t to_sq;

	rc = RC_FAIL;
	for (to = 0; to < 81; to++) {
		to_sq = KIND(php->board[to]);
		if (to_sq == mvp->pc && to != mvp->from) {
			mvp->to = to;
			mvp->flag = 0x00;
			mvp->flag2 = B_MV2_EXCHANGE;
			if (yo_move(php) == 0) {
				switch (check(php + 1)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		}
	}

	return rc;
}

int
direct_escape_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, rc, i;
	uchar_t to_sq;

	rc = RC_FAIL;
	for (i = 0; ; i++) {
		to = yo_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
//		if (IS_YO_MOVABLE(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			switch (direct_escape_move_sub(php)) {
			case RC_ESCAPE:
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_shishi_leap(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, to3, rc, i, j, k;
	uchar_t to_sq, to_sq2;

	rc = RC_FAIL;
	for (i = 0; ; i++) {
		to = yo_leap_to_pos[mvp->fpc][mvp->from][i];
		if (to == EOP) {
			break;
		}
		to_sq = php->board[to];
//		if (IS_YO_MOVABLE(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			rc = direct_escape_move_sub(php);
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
//					if (IS_YO_MOVABLE(to_sq2) || to2 == mvp->from) {
					if (((IS_ROOM(to_sq2) || to2 == mvp->from) && NOT(php->state & B_MODE_GREED)) ||
							(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						switch (direct_escape_move_sub(php)) {
						case RC_ESCAPE:
							return RC_ESCAPE;
						case RC_MATE:
							rc = RC_MATE;
							break;
						}
					}
				next_for:;
				}
				mvp->to_1st = NOP;
			}
		}
	}
	mvp->to = mvp->from;
	switch (direct_escape_move_sub(php)) {
	case RC_ESCAPE:
		return RC_ESCAPE;
	case RC_MATE:
		rc = RC_MATE;
		break;
	}

	return rc;
}

int
direct_escape_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to_next, rc, i, j;
	uchar_t to_sq;

	rc = RC_FAIL;
	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0, to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][0]; ; i++) {
			to = to_next;
			to_sq = php->board[to];
			to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][i + 1];
			if (to_next == EOP) {
//				if (IS_YO_MOVABLE(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to;
					switch (direct_escape_move_sub(php)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
					}
				}
				break;
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return rc;
}

// Mao, Mor:
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
int
direct_escape_mao_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int div, to, rc, i;
	uchar_t to_sq;

	rc = RC_FAIL;
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
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				switch (direct_escape_move_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		another_to:
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
			if (to == EOP) {
				continue;
			}
			to_sq = php->board[to];
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				switch (direct_escape_move_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		}
	}

	return rc;
}

int
direct_escape_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, rc, i, j;
	uchar_t to_sq;

	rc = RC_FAIL;
	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
//			if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				switch (direct_escape_move_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j;
	uchar_t to_sq, to_sq2;

	rc = RC_FAIL;
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
//				if (IS_YO_MOVABLE(to_sq2)) {
				if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to2;
					switch (direct_escape_move_sub(php)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
					}
				}
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_decapt_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j;
	uchar_t to_sq, to_sq2;

	rc = RC_FAIL;
	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (NOT_PASSABLE(to_sq)) {
//				if (IS_HI_PC(to_sq)) {
				if (IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC)) {
					i++;
					to2 = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_ROOM(to_sq2)) {
						mvp->to = to2;
						mvp->to_1st = to;
						switch (direct_escape_move_sub(php)) {
						case RC_ESCAPE:
							return RC_ESCAPE;
						case RC_MATE:
							rc = RC_MATE;
							break;
						}
						mvp->to_1st = NOP;
					}
				}
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_pao_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j;
	uchar_t to_sq, to_sq2;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
			if (IS_PASSABLE(to_sq)) {
//				if (IS_ROOM(to_sq)) {
				if (IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) {
					mvp->to = to;
					switch (direct_escape_move_sub(php)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
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
//						if (IS_HI_PC(to_sq2)) {
						if (IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC)) {
							mvp->to = to2;
							switch (direct_escape_move_sub(php)) {
							case RC_ESCAPE:
								return RC_ESCAPE;
							case RC_MATE:
								rc = RC_MATE;
								break;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_jump(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j;
	uchar_t to_sq, to_sq2;

	rc = RC_FAIL;
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
//						if (IS_ROOM(to_sq2)) {
						if (IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) {
							mvp->to = to2;
							switch (direct_escape_move_sub(php)) {
							case RC_ESCAPE:
								return RC_ESCAPE;
							case RC_MATE:
								rc = RC_MATE;
								break;
							}
						}
					} else {
//						if (IS_HI_PC(to_sq2)) {
						if (IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC)) {
							mvp->to = to2;
							switch (direct_escape_move_sub(php)) {
							case RC_ESCAPE:
								return RC_ESCAPE;
							case RC_MATE:
								rc = RC_MATE;
								break;
							}
						}
						break;
					}
				}
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_jump_hop(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j, n;
	uchar_t to_sq, to_sq2;

	rc = RC_FAIL;
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
//					if (IS_YO_MOVABLE(to_sq2)) {
					if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
							(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						switch (direct_escape_move_sub(php)) {
						case RC_ESCAPE:
							return RC_ESCAPE;
						case RC_MATE:
							rc = RC_MATE;
							break;
						}
					}
					break;
				}
			}
		}
	}

	return rc;
}

// Sparrow, Eagle:
//   hi_run_to_posv[駒][位置][ベクトル][利き位置リスト]: 反射駒利き位置リスト（攻方、受方共用）
//   yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
//   hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
int
direct_escape_refl(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to2, rc, i, j, k;
	uchar_t to_sq, to_sq2;
	char to_done[16] = {
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	};

	rc = RC_FAIL;
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
//				if (IS_YO_MOVABLE(to_sq2)) {
				if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to2) {
							goto another_to2;
						}
					}
					to_done[k] = to2;
					mvp->to = to2;
					switch (direct_escape_move_sub(php)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
					}
				}
			another_to2:;
				to2 = yo_run_to_posv[mvp->fpc][mvp->from][to][1];
				if (to2 == EOP) {
					break;
				}
				to_sq2 = php->board[to2];
//				if (IS_YO_MOVABLE(to_sq2)) {
				if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to2) {
							goto next_vec;
						}
					}
					to_done[k] = to2;
					mvp->to = to2;
					switch (direct_escape_move_sub(php)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
					}
				}
			next_vec:;
				break;
			}
		}
	}

	return rc;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
int
direct_escape_equi_hop(phase_t *php)
{
	char *to, *sym_pos;
	move_t *mvp = &php->move;
	int rc, i;
	uchar_t to_sq;

	rc = RC_FAIL;
	to = hi_run_to_posv[mvp->fpc][mvp->from][0];
	sym_pos = yo_run_to_posv[mvp->fpc][mvp->from][0];
	for (i = 0; to[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			to_sq = php->board[to[i]];
//			if (IS_YO_MOVABLE(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to[i];
				switch (direct_escape_move_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		}
	}

	return rc;
}

int
direct_escape_circle_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, rc, i, j, k;
	uchar_t to_sq;
	char to_done[16] = {
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	};

	rc = RC_FAIL;
	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = yo_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
//			if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				for (k = 0; to_done[k] != NOP; k++) {
					if (to_done[k] == to) {
						goto next_to;
					}
				}
				to_done[k] = to;
				mvp->to = to;
				switch (direct_escape_move_sub(php)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		next_to:;
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_place_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc;

	rc = RC_FAIL;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0) {
		switch (check(php + 1)) {
		case RC_ESCAPE:
			return RC_ESCAPE;
		case RC_MATE:
			rc = RC_MATE;
			break;
		}
	}

	return rc;
}

int
direct_escape_move_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc;

	rc = RC_FAIL;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0) {
		switch (check(php + 1)) {
		case RC_ESCAPE:
			return RC_ESCAPE;
		case RC_MATE:
			rc = RC_MATE;
			break;
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag = B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			switch (check(php + 1)) {
			case RC_ESCAPE:
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
		}
	}

	return rc;
}

int
direct_escape_move_sub_circe(phase_t *php)
{
	move_t *mvp = &php->move;
	int rc;

	rc = RC_FAIL;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0) {
		switch (check(php + 1)) {
		case RC_ESCAPE:
			return RC_ESCAPE;
		case RC_MATE:
			rc = RC_MATE;
			break;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move(php) == 0) {
				switch (check(php + 1)) {
				case RC_ESCAPE:
					return RC_ESCAPE;
				case RC_MATE:
					rc = RC_MATE;
					break;
				}
			}
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag = B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			switch (check(php + 1)) {
			case RC_ESCAPE:
				return RC_ESCAPE;
			case RC_MATE:
				rc = RC_MATE;
				break;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (yo_move(php) == 0) {
					switch (check(php + 1)) {
					case RC_ESCAPE:
						return RC_ESCAPE;
					case RC_MATE:
						rc = RC_MATE;
						break;
					}
				}
			}
		}
	}

	return rc;
}

#if 0
		if (NOT(php->state & B_ESCAPE_MOVE_FOUND) && is_yonder_mate(php)) { \
			rc = direct_mate_rule_check(php); \
		} else { \
			rc = RC_ESCAPE; \
		} \

#endif
