// za/h_check.c -- zoo+single-change help(-self)-mate check routines
// Copyright (c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/h_check.c 1.35 2014/08/18 12:54:30 budai10 Exp budai10 $

// 跳び、跳々、走り、Ｇ跳び、蝗跳び、包跳び、鬣跳び、考跳び、雀跳び、鷲跳び、Ｅ跳び
// 安南、安北、対面、背面、ネコ鮮、ネコネコ鮮、マドラシ、PWC

// 王手：
//   打ち：
//     1)  打駒の利き（自身移動先。変身後の移動先）で王手になる。
//         ※例外：Sparrow, Equihopper系（攻方、受方のデータ構造が非対称）
//     2)  安南、安北、ネコ鮮、ネコネコ鮮のとき、駒打による攻駒の変身で王手になる。
//     3)  打駒が、Ｇ跳び、包跳び、鬣跳び、考跳び、雀跳び、鷲跳び、Ｅ跳びの台駒になり、王手になる。
//           Ｇ跳び：玉の周囲
//           包跳び、鬣跳び：玉からの包利き路or鬣利き路
//           考跳び：玉の周囲か、既に埋まっていれば考利き路
//           雀跳び：玉の両側の雀利き路
//           鷲跳び：玉の両側の鷲利き路
//           Ｅ跳び：定義が難しい...
//     4)  打駒が、走駒のマドラシ痺れを解除し、王手になる。
//   移動：
//     1)  
// 王手回避手：
//   打ち：
//     1)  打駒が、跳々、走り、＊跳びの利き路を塞ぐ。（鬣跳びの跳び先を含む）
//         ※例外：Sparrow, Equihopper系（攻方、受方のデータ構造が非対称）
//     2)  対面、背面、ネコネコ鮮のとき、駒打による王手駒の変身で王手が回避される。
//     3)  打駒が、王手駒をマドラシ痺れにする。
//   移動：
//     1)  

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

// ----------------------------------------------------------------------------- 
#define	HELP_CHECK_PART_1 \
	/* ｆｍの終了条件 */ \
	if (solution_count >= solution_limit || depth >= limit_depth) { \
		return 0; \
	} \
	depth++; \
 \
	/* 攻方局面を解析 */ \
	if (analyze_phase(php) != 0) { \
		goto return_1; \
	} \
 \
	/* ルール上の検索打切り条件（王手回避チェック） */ \
	if (php->check_state & B_CS_HITHER_CHECK) { \
		goto return_1; \
	} \
 \
	/* 解析状況をチェック（解析状況表示） */ \
	if (++phase_count >= check_point) { \
		mile_stone(); \
	} \
 \
	/* 検索局面削減：局面の対称性チェック */ \
	if (((php - 1)->state & B_SYMMETRIC) && depth >= 2 && test_symmetric(php) == 0) { \
		goto return_0; \
	} \
 \
	/* トレース上限の検索打切り条件 */ \
	if (trace_limit >= 1 && ((depth >= 2 && trace_move(php) != 0) || depth > trace_limit)) { \
		goto return_0; \
	} \
 \
	/* 検索局面削減：受方の同一局面をチェック、登録 */ \
	if (depth < limit_depth && (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME))) { \
		yo_hash_diff(php); \
		if (strategy & B_YO_DENY_SAME) { \
			/* 手順中同一局面をチェック、受方の不詰同一局面をチェック */ \
			/* 同一局面でなかった場合に、手順中同一局面を登録 */ \
			int is_same = is_yo_same_phase(php); \
			if (is_same != 0) { \
				goto return_0; \
			} \
		} \
	}
// ----------------------------------------------------------------------------- 
#define	HELP_CHECK_PART_2 \
	/* 検索局面削減：受方の同一局面を登録、解除 */ \
	if ((strategy & B_YO_DENY_SAME) && depth < limit_depth) { \
		ulong_t index; \
 \
		/* 手順中同一局面チェック用ハッシュ値を解除 */ \
		index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands; \
		index &= NOSPHHEAD - 1; \
		yo_sph_head[index] = yo_sph_head[index]->next; \
		/* 詰手順中の局面でなければ、受方の不詰同一局面を登録 */ \
		/* （解数上限超えの場合を除く） */ \
		if ((strategy & B_YO_ESCAPE) && NOT((php - 1)->move.flag & B_MV_MATESEQ) && \
				solution_count < solution_limit) { \
			set_yo_eh_cell(php); \
		} \
	} \
 \
return_0:; \
	depth--; \
	return 0; \
 \
return_1:; \
	depth--; \
	return 1;
// ----------------------------------------------------------------------------- 

static void	help_check_exchange(phase_t *php);
static void help_check_place_FU(phase_t *php);

int
help_check_normal(phase_t *php)
{
	HELP_CHECK_PART_1

	// 攻方取禁
	if (cond_flag & B_C_HITHER_NOCAPT) {
		php->state |= B_MODE_ASCETIC;
	}

	// 王手検索
	if (depth >= limit_depth) {
		if (rule_flag & B_R_HELP_MATE) {
			if (cond_flag & B_C_UCHIFU) {
				help_check_place_FU(php);
			} else {
				help_check_all(php);
			}
		} else {
			void (* save_help_check_place)(phase_t *php);
			void (* save_help_check_place_sub)(phase_t *php);
			void (* save_help_check_move_sub)(phase_t *php);
			save_help_check_place = help_check_place;
			help_check_place = help_check_place_loose;
			save_help_check_place_sub = help_check_place_sub;
			help_check_place_sub = help_check_place_sub_loose;
			save_help_check_move_sub = help_check_move_sub;
			help_check_move_sub = help_check_move_sub_loose;
			help_check_all(php);
			help_check_place = save_help_check_place;
			help_check_place_sub = save_help_check_place_sub;
			help_check_move_sub = save_help_check_move_sub;
		}
	} else {
		help_check_all(php);
	}

	HELP_CHECK_PART_2
}

int
help_check_messigny(phase_t *php)
{
	HELP_CHECK_PART_1

	// 攻方取禁
	if (cond_flag & B_C_HITHER_NOCAPT) {
		php->state |= B_MODE_ASCETIC;
	}

	// 王手検索
	if (depth >= limit_depth) {
		if (rule_flag & B_R_HELP_MATE) {
			if (cond_flag & B_C_UCHIFU) {
				help_check_place_FU(php);
			} else {
				help_check_all_messigny(php);
			}
		} else {
			void (* save_help_check_place)(phase_t *php);
			void (* save_help_check_place_sub)(phase_t *php);
			void (* save_help_check_move_sub)(phase_t *php);
			save_help_check_place = help_check_place;
			help_check_place = help_check_place_loose;
			save_help_check_place_sub = help_check_place_sub;
			help_check_place_sub = help_check_place_sub_loose;
			save_help_check_move_sub = help_check_move_sub;
			help_check_move_sub = help_check_move_sub_loose;
			help_check_all_messigny(php);
			help_check_place = save_help_check_place;
			help_check_place_sub = save_help_check_place_sub;
			help_check_move_sub = save_help_check_move_sub;
		}
	} else {
		help_check_all_messigny(php);
	}

	HELP_CHECK_PART_2
}

int
help_check_greed(phase_t *php)
{
	HELP_CHECK_PART_1

	// 王手検索
	if (depth >= limit_depth) {
		if (rule_flag & B_R_HELP_MATE) {
			help_check_all_greed(php);
			if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
				if (cond_flag & B_C_UCHIFU) {
					help_check_place_FU(php);
				} else {
					help_check_all_ascetic(php);
				}
			}
		} else {	// B_R_HELP_STALEMATE
			void (* save_help_check_place)(phase_t *php);
			void (* save_help_check_place_sub)(phase_t *php);
			void (* save_help_check_move_sub)(phase_t *php);
			save_help_check_place = help_check_place;
			help_check_place = help_check_place_loose;
			save_help_check_place_sub = help_check_place_sub;
			help_check_place_sub = help_check_place_sub_loose;
			save_help_check_move_sub = help_check_move_sub;
			help_check_move_sub = help_check_move_sub_loose;
			help_check_all_greed(php);
			if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
				help_check_all_ascetic(php);
			}
			help_check_place = save_help_check_place;
			help_check_place_sub = save_help_check_place_sub;
			help_check_move_sub = save_help_check_move_sub;
		}
	} else {
		help_check_all_greed(php);
		if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
			help_check_all_ascetic(php);
		}
	}

	HELP_CHECK_PART_2
}

int
help_check_ascetic(phase_t *php)
{
	HELP_CHECK_PART_1

	// 王手検索
	if (depth >= limit_depth) {
		if (rule_flag & B_R_HELP_MATE) {
			if (cond_flag & B_C_UCHIFU) {
				help_check_place_FU(php);
			} else {
				help_check_all_ascetic(php);
				if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
					help_check_all_greed(php);
				}
			}
		} else {
			void (* save_help_check_place)(phase_t *php);
			void (* save_help_check_place_sub)(phase_t *php);
			void (* save_help_check_move_sub)(phase_t *php);
			save_help_check_place = help_check_place;
			help_check_place = help_check_place_loose;
			save_help_check_place_sub = help_check_place_sub;
			help_check_place_sub = help_check_place_sub_loose;
			save_help_check_move_sub = help_check_move_sub;
			help_check_move_sub = help_check_move_sub_loose;
			help_check_all_ascetic(php);
			if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
				help_check_all_greed(php);
			}
			help_check_place = save_help_check_place;
			help_check_place_sub = save_help_check_place_sub;
			help_check_move_sub = save_help_check_move_sub;
		}
	} else {
		help_check_all_ascetic(php);
		if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
			help_check_all_greed(php);
		}
	}

	HELP_CHECK_PART_2
}

void
help_check_all(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc;

	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			help_check_place(php);
		} else if (IS_HI_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_check_leap(php);
			piece[mvp->fpc].help_check_run(php);
		}
	}

	return;
}

void
help_check_all_messigny(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc;

	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			help_check_place(php);
		} else if (IS_HI_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			help_check_exchange(php);
			piece[mvp->fpc].help_check_leap(php);
			piece[mvp->fpc].help_check_run(php);
		}
	}

	return;
}

void
help_check_all_greed(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc;

	php->state |= B_MODE_GREED;
	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_check_leap(php);
			piece[mvp->fpc].help_check_run(php);
		}
	}
	php->state &= ~B_MODE_GREED;

	return;
}

void
help_check_all_ascetic(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc;

	php->state |= B_MODE_ASCETIC;
	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			help_check_place(php);
		} else if (IS_HI_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_check_leap(php);
			piece[mvp->fpc].help_check_run(php);
		}
	}
	php->state &= ~B_MODE_ASCETIC;

	return;
}

void
help_check_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	for (i = 0; NOT_NUL(hand_pc[i]); i++) {
		pc = hand_pc[i];
		if (php->hi_hand[pc] >= 1) {
			mvp->pc = mvp->fpc = pc;
			help_check_place_sub(php);
		}
	}

	return;
}

// ホッパやジャンパなどのフェアリー駒がおらず、
// 変身系条件がなく、マドラシもない場合
void
help_check_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if (hi_close_check_board[php->yo_gy_pos][mvp->to] == ON ||
			hi_remote_check_board[php->yo_gy_pos][mvp->to] == ON) {
		// 跳び駒の逆利き位置、跳々駒、走り駒の逆走り道の場合
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->hi_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				help_check_place_sub(php);
			}
		}
	}

	return;
}

static void
help_check_place_FU(phase_t *php)
{
	if (php->hi_hand[PC_FU] >= 1) {
		move_t *mvp = &php->move;
		int pos;
		uchar_t bpc;
		mvp->pc = mvp->fpc = PC_FU;
		mvp->from = NOP;
		for (pos = 0; pos < 81; pos++) {
			bpc = php->board[pos];
			if (IS_ROOM(bpc)) {
				mvp->to = pos;
				help_check_place_sub(php);
			}
		}
	}

	return;
}

static void
help_check_exchange(phase_t *php)
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
			if (hi_move(php) == 0 && escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
		}
	}

	return;
}

void
help_check_leap(phase_t *php)
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
//		if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			help_check_move_sub(php);
		}
	}

	return;
}

void
help_check_shishi_leap(phase_t *php)
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
//		if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			help_check_move_sub(php);
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
//					if (IS_ROOM(to_sq2) || IS_YO_PC(to_sq2) || to2 == mvp->from) {
					if (((IS_ROOM(to_sq2) || to2 == mvp->from) && NOT(php->state & B_MODE_GREED)) ||
							(IS_YO_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						help_check_move_sub(php);
					}
				next_for:;
				}
				mvp->to_1st = NOP;
			}
		}
	}
	mvp->to = mvp->from;
	help_check_move_sub(php);

	return;
}

void
help_check_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to_next, i, j;
	uchar_t to_sq;

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0, to_next = hi_run_to_posv[mvp->fpc][mvp->from][j][0]; ; i++) {
			to = to_next;
			to_sq = php->board[to];
			to_next = hi_run_to_posv[mvp->fpc][mvp->from][j][i + 1];
			if (to_next == EOP) {
//				if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to;
					help_check_move_sub(php);
				}
				break;
			}
			if (NOT_PASSABLE(to_sq)) {
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
help_check_mao_step(phase_t *php)
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
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				help_check_move_sub(php);
			}
		another_to:
			to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
			if (to == EOP) {
				continue;
			}
			to_sq = php->board[to];
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				help_check_move_sub(php);
			}
		}
	}

	return;
}

void
help_check_run(phase_t *php)
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
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				help_check_move_sub(php);
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return;
}

void
help_check_hop(phase_t *php)
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
//				if (IS_ROOM(to_sq2) || IS_YO_PC(to_sq2)) {
				if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
						(IS_YO_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to2;
					help_check_move_sub(php);
				}
				break;
			}
		}
	}

	return;
}

void
help_check_decapt_hop(phase_t *php)
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
//				if (IS_YO_PC(to_sq)) {
				if (IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC)) {
					i++;
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (IS_ROOM(to_sq2)) {
						mvp->to = to2;
						mvp->to_1st = to;
						help_check_move_sub(php);
						mvp->to_1st = NOP;
					}
				}
				break;
			}
		}
	}

	return;
}

void
help_check_pao_jump(phase_t *php)
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
//			if (IS_ROOM(to_sq)) {
			if (IS_PASSABLE(to_sq)) {
				if (IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) {
					mvp->to = to;
					help_check_move_sub(php);
				}
			} else {
				for (i++; ; i++) {
					to2 = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
					if (to2 == EOP) {
						break;
					}
					to_sq2 = php->board[to2];
					if (NOT_PASSABLE(to_sq2)) {
//						if (IS_YO_PC(to_sq2)) {
						if (IS_YO_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC)) {
							mvp->to = to2;
							help_check_move_sub(php);
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
help_check_jump(phase_t *php)
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
//					if (IS_ROOM(to_sq2)) {
					if (IS_PASSABLE(to_sq2)) {
						if (IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) {
							mvp->to = to2;
							help_check_move_sub(php);
						}
					} else {
//						if (IS_YO_PC(to_sq2)) {
						if (IS_YO_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC)) {
							mvp->to = to2;
							help_check_move_sub(php);
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
help_check_jump_hop(phase_t *php)
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
//					if (IS_ROOM(to_sq2) || IS_YO_PC(to_sq2)) {
					if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
							(IS_YO_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						help_check_move_sub(php);
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
help_check_refl(phase_t *php)
{
	move_t *mvp = &php->move;
	int div, to, i, j, k;
	uchar_t sq, to_sq;
	char to_done[16] = {
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	};

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			div = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (div == EOP) {
				break;
			}
			sq = php->board[div];
			if (NOT_PASSABLE(sq)) {
				to = yo_run_to_posv[mvp->fpc][mvp->from][div][0];
				to_sq = php->board[to];
//				if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to) {
							goto another_to;
						}
					}
					to_done[k] = to;
					mvp->to = to;
					help_check_move_sub(php);
				}
			another_to:;
				to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
				if (to == EOP) {
					break;
				}
				to_sq = php->board[to];
//				if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to) {
							goto next_vec;
						}
					}
					to_done[k] = to;
					mvp->to = to;
					help_check_move_sub(php);
				}
			next_vec:;
				break;
			}
		}
	}

	return;
}

// Equihopper:
//   hi_run_to_posv[0]: 利き位置リスト
//   yo_run_to_posv[0]: 対称点リスト
void
help_check_equi_hop(phase_t *php)
{
	char *to, *sym_pos;
	move_t *mvp = &php->move;
	int i;
	uchar_t to_sq;

	to = hi_run_to_posv[mvp->fpc][mvp->from][0];
	sym_pos = yo_run_to_posv[mvp->fpc][mvp->from][0];
	for (i = 0; to[i] != EOP; i++) {
		if (IS_ANY_PC(php->board[sym_pos[i]])) {
			to_sq = php->board[to[i]];
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to[i];
				help_check_move_sub(php);
			}
		}
	}

	return;
}

void
help_check_circle_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i, j, k;
	uchar_t to_sq;
	char to_done[32] = {
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	};

	for (j = 0; hi_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0; ; i++) {
			to = hi_run_to_posv[mvp->fpc][mvp->from][j][i];
			if (to == EOP) {
				break;
			}
			to_sq = php->board[to];
//			if (IS_ROOM(to_sq) || IS_YO_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_YO_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				for (k = 0; to_done[k] != NOP; k++) {
					ASSERT(k < 32);
					if (to_done[k] == to) {
						goto next_to;
					}
				}
				to_done[k] = to;
				mvp->to = to;
				help_check_move_sub(php);
			}
		next_to:;
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return;
}

void
help_move_null(phase_t *php)
{
	return;
}

void
help_check_place_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move(php) == 0 && escape(php + 1) == 0) {
		php->state |= B_CHECK_MOVE_FOUND;
	}

	return;
}

void
help_check_place_sub_normal(phase_t *php)
{
	move_t *mvp = &php->move;

	if (hi_to_board[mvp->fpc][mvp->to][php->yo_gy_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}

	return;
}

void
help_check_place_sub_madrasi(phase_t *php)
{
	move_t *mvp = &php->move;

	if (hi_to_board[mvp->fpc][mvp->to][php->yo_gy_pos] == ON ||
			(php->fire[mvp->to] & B_HI_CHECK_SPECIAL)) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}

	return;
}

void
help_check_place_sub_annanhoku(phase_t *php)
{
	move_t *mvp = &php->move;
	int bpos, fpos;

	mvp->fpc = mvp->pc;
	fpos = NOP;
	switch (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
	case B_C_ANNAN:
		bpos = fpc_pos_hi_annan[mvp->to];
		if (bpos != NOP && IS_HI_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		fpos = fpc_pos_forward[mvp->to];
		if (fpos == NOP || NOT(IS_HI_PC(php->board[fpos]))) {
			fpos = NOP;
		}
		break;
	case B_C_ANHOKU:
		bpos = fpc_pos_hi_anhoku[mvp->to];
		if (bpos != NOP && IS_HI_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		fpos = fpc_pos_back[mvp->to];
		if (fpos == NOP || NOT(IS_HI_PC(php->board[fpos]))) {
			fpos = NOP;
		}
		break;
	}
	if (hi_to_board[mvp->fpc][mvp->to][php->yo_gy_pos] == ON ||
			(fpos != NOP && hi_to_board[mvp->pc][fpos][php->yo_gy_pos] == ON)) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}

	return;
}

void
help_check_place_sub_taihaimen(phase_t *php)
{
	move_t *mvp = &php->move;
	int bpos;

	mvp->fpc = mvp->pc;
	bpos = NOP;
	switch (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
	case B_C_TAIMEN:
		bpos = fpc_pos_hi_taimen[mvp->to];
		if (bpos != NOP && IS_YO_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		break;
	case B_C_HAIMEN:
		bpos = fpc_pos_hi_haimen[mvp->to];
		if (bpos != NOP && IS_YO_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		break;
	}
	if (hi_to_board[mvp->fpc][mvp->to][php->yo_gy_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}

	return;
}

void
help_check_place_sub_nekosen(phase_t *php)
{
	move_t *mvp = &php->move;
	int pos1, pos2, i;
	int fpos[9] = { NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, };
	uchar_t fpc[9] = { NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, };

	switch (cond_flag & (B_C_NEKOSEN | B_C_NEKO2SEN)) {
	case B_C_NEKOSEN:
		for (pos1 = mvp->to; fpc_pos_forward[pos1] != NOP && IS_HI_PC(php->board[pos1 - 1]); pos1--)
			;
		for (pos2 = mvp->to; fpc_pos_back[pos2] != NOP && IS_HI_PC(php->board[pos2 + 1]); pos2++)
			;
		break;
	case B_C_NEKO2SEN:
		for (pos1 = mvp->to; fpc_pos_forward[pos1] != NOP && IS_ANY_PC(php->board[pos1 - 1]);
				pos1--)
			;
		for (pos2 = mvp->to; fpc_pos_back[pos2] != NOP && IS_ANY_PC(php->board[pos2 + 1]); pos2++)
			;
		break;
	}
	if (pos1 == pos2) {
		mvp->fpc = mvp->pc;
		fpos[0] = pos1;
		fpc[0] = mvp->fpc;
	} else {
		i = 0;
		LOOP {
			if (pos1 == mvp->to) {
				fpc[i] = mvp->fpc = KIND(php->board[pos2]);
				fpc[i + 1] = mvp->pc;
			} else if (pos2 == mvp->to) {
				fpc[i] = mvp->pc;
				fpc[i + 1] = mvp->fpc = KIND(php->board[pos1]);
			} else {
				fpc[i] = KIND(php->board[pos2]);
				fpc[i + 1] = KIND(php->board[pos1]);
			}
			fpos[i] = pos1;
			fpos[i + 1] = pos2;
			pos1++;
			pos2--;
			i += 2;
			if (pos1 > pos2) {
				break;
			}
			if (pos1 == pos2) {
				if (pos1 == mvp->to) {
					fpc[i] = mvp->fpc = mvp->pc;
				} else {
					fpc[i] = php->fpc_board[pos1];
				}
				fpos[i] = pos1;
				break;
			}
		}
	}
	for (i = 0; fpos[i] != NOP; i++) {
		if ((IS_HI_PC(php->board[fpos[i]]) || fpos[i] == mvp->to) &&
				hi_to_board[fpc[i]][fpos[i]][php->yo_gy_pos] == ON) {
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (hi_move(php) == 0 && escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
			break;
		}
	}

	return;
}

// 全合法手生成
void
help_check_move_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move(php) == 0 && escape(php + 1) == 0) {
		php->state |= B_CHECK_MOVE_FOUND;
	}
	if (mvp->from != NOP && IS_PROMOTABLE(mvp->pc) && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}

	return;
}

// キルケ対応
void
help_check_move_sub_circe(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (hi_move(php) == 0) {
		if (escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (hi_move(php) == 0 && escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
		}
	}
	if (mvp->from != NOP && IS_PROMOTABLE(mvp->pc) && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0) {
			if (escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (hi_move(php) == 0 && escape(php + 1) == 0) {
					php->state |= B_CHECK_MOVE_FOUND;
				}
			}
		}
	}

	return;
}

// 通常対応（王手の掛かる位置、空き王手になり得る位置）
void
help_check_move_sub_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int indirect_check = FALSE;

	if (hi_to_board[mvp->pc][mvp->to][php->yo_gy_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	} else if (hi_remote_check_board[php->yo_gy_pos][mvp->from] == ON) {
		indirect_check = TRUE;
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0 && escape(php + 1) == 0) {
			php->state |= B_CHECK_MOVE_FOUND;
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		if (hi_to_board[mvp->pc | B_PC_PROMOTED][mvp->to][php->yo_gy_pos] == ON ||
				indirect_check == TRUE) {
			mvp->flag |= B_MV_PROMOTE;
			mvp->flag2 = 0x00;
			if (hi_move(php) == 0 && escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
		}
	}

	return;
}

// 通常対応のキルケ対応
void
help_check_move_sub_normal_circe(phase_t *php)
{
	move_t *mvp = &php->move;
	int indirect_check = FALSE;

	if (hi_to_board[mvp->pc][mvp->to][php->yo_gy_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0) {
			if (escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (hi_move(php) == 0 && escape(php + 1) == 0) {
					php->state |= B_CHECK_MOVE_FOUND;
				}
			}
		}
	} else if (hi_remote_check_board[php->yo_gy_pos][mvp->from] == ON) {
		indirect_check = TRUE;
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (hi_move(php) == 0) {
			if (escape(php + 1) == 0) {
				php->state |= B_CHECK_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (hi_move(php) == 0 && escape(php + 1) == 0) {
					php->state |= B_CHECK_MOVE_FOUND;
				}
			}
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		if (hi_to_board[mvp->pc | B_PC_PROMOTED][mvp->to][php->yo_gy_pos] == ON ||
				indirect_check == TRUE) {
			mvp->flag |= B_MV_PROMOTE;
			mvp->flag2 = 0x00;
			if (hi_move(php) == 0) {
				if (escape(php + 1) == 0) {
					php->state |= B_CHECK_MOVE_FOUND;
				}
				if (mvp->flag2 & B_MV2_REBORN_MORE) {
					if (hi_move(php) == 0 && escape(php + 1) == 0) {
						php->state |= B_CHECK_MOVE_FOUND;
					}
				}
			}
		}
	}

	return;
}

