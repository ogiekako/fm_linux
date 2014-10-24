// za/hs_escape.c -- zoo+single-change help-self-mate escape routines
// Copyright (c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/hs_escape.c 1.28 2014/08/18 12:54:32 budai10 Exp budai10 $

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

// ----------------------------------------------------------------------------- 
#define	HELP_SELF_ESCAPE_PART_1 \
	/* ｆｍの終了条件 */ \
	if (solution_count >= solution_limit || depth >= limit_depth) { \
		return 0; \
	} \
	depth++; \
 \
	/* 受方局面を解析 */ \
	if (analyze_phase(php) != 0) { \
		goto return_1; \
	} \
 \
	/* ルール上の検索打切り条件（王手回避チェック） */ \
	if (php->check_state & B_CS_YONDER_CHECK) { \
		goto return_1; \
	} \
 \
	/* ルール上の検索打切り条件（王手義務チェック） */ \
	/* ただし、受先の場合には初形では王手状態でないことを考慮 */ \
	if (NOT(php->check_state & B_CS_HITHER_CHECK) && depth >= 2) { \
		goto return_1; \
	} \
 \
	/* 解析状況をチェック（解析状況表示） */ \
	if (++phase_count >= check_point) { \
		mile_stone(); \
	} \
 \
	/* 詰手数を超えていれば、スキップ */ \
	if (depth > limit_depth) { \
		goto return_0; \
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
	/* 検索局面削減：攻方の同一局面をチェック、登録 */ \
	if (depth < limit_depth && (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME))) { \
		hi_hash_diff(php); \
		if (strategy & B_HI_DENY_SAME) { \
			/* 手順中同一局面をチェック、攻方の不詰同一局面をチェック */ \
			/* 同一局面でなかった場合に、手順中同一局面を登録 */ \
			int is_same = is_hi_same_phase(php); \
			if (is_same != 0) { \
				goto return_0; \
			} \
		} \
	}
// ----------------------------------------------------------------------------- 
#define	HELP_SELF_ESCAPE_PART_2 \
	/* 検索局面削減：受方の同一局面を登録、解除 */ \
	if ((strategy & B_HI_DENY_SAME) && depth < limit_depth) { \
		ulong_t index; \
 \
		/* 手順中同一局面チェック用ハッシュ値を解除 */ \
		index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands; \
		index &= NOSPHHEAD - 1; \
		hi_sph_head[index] = hi_sph_head[index]->next; \
		/* 詰手順中の局面でなければ、攻方の不詰同一局面を登録 */ \
		/* （解数上限超えの場合を除く） */ \
		if ((strategy & B_HI_ESCAPE) && NOT((php - 1)->move.flag & B_MV_MATESEQ) && \
				solution_count < solution_limit) { \
			set_hi_eh_cell(php); \
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

static void help_self_escape_check_place_FU(phase_t *php);

int
help_self_escape_normal(phase_t *php)
{
	HELP_SELF_ESCAPE_PART_1

	// 王手回避手検索
	if (depth >= limit_depth) {
		if (cond_flag & B_C_UCHIFU) {
			help_self_escape_check_place_FU(php);
		} else if (rule_flag & B_R_HELP_SELF_MATE) {
			void (* save_help_escape_place)(phase_t *php) = help_escape_place;
			void (* save_help_escape_move_sub)(phase_t *php) = help_escape_move_sub;
			help_escape_place = help_escape_check_place;
			help_escape_move_sub = help_escape_check_move_sub;
			help_escape_all(php);
			help_escape_place = save_help_escape_place;
			help_escape_move_sub = save_help_escape_move_sub;
		} else {
			help_escape_all(php);
		}
	} else {
		help_escape_all(php);
	}

	HELP_SELF_ESCAPE_PART_2
}

int
help_self_escape_messigny(phase_t *php)
{
	HELP_SELF_ESCAPE_PART_1

	// 王手回避手検索
	if (depth >= limit_depth) {
		if (cond_flag & B_C_UCHIFU) {
			help_self_escape_check_place_FU(php);
		} else if (rule_flag & B_R_HELP_SELF_MATE) {
			void (* save_help_escape_place)(phase_t *php) = help_escape_place;
			void (* save_help_escape_move_sub)(phase_t *php) = help_escape_move_sub;
			help_escape_place = help_escape_check_place;
			help_escape_move_sub = help_escape_check_move_sub;
			help_escape_all_messigny(php);
			help_escape_place = save_help_escape_place;
			help_escape_move_sub = save_help_escape_move_sub;
		} else {
			help_escape_all_messigny(php);
		}
	} else {
		help_escape_all_messigny(php);
	}

	HELP_SELF_ESCAPE_PART_2
}

int
help_self_escape_greed(phase_t *php)
{
	HELP_SELF_ESCAPE_PART_1

	// 王手回避手検索
	if (depth >= limit_depth) {
		if (rule_flag & B_R_HELP_SELF_MATE) {
			help_escape_all_greed(php);
			if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
				if (cond_flag & B_C_UCHIFU) {
					help_self_escape_check_place_FU(php);
				} else {
					help_escape_all_ascetic(php);
				}
			}
		} else {
			void (* save_help_escape_place)(phase_t *php) = help_escape_place;
			void (* save_help_escape_move_sub)(phase_t *php) = help_escape_move_sub;
			help_escape_place = help_escape_check_place;
			help_escape_move_sub = help_escape_check_move_sub;
			help_escape_all_greed(php);
			if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
				help_escape_all_ascetic(php);
			}
			help_escape_place = save_help_escape_place;
			help_escape_move_sub = save_help_escape_move_sub;
		}
	} else {
		help_escape_all_greed(php);
		if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
			help_escape_all_ascetic(php);
		}
	}

	HELP_SELF_ESCAPE_PART_2
}

int
help_self_escape_ascetic(phase_t *php)
{
	HELP_SELF_ESCAPE_PART_1

	// 王手回避手検索
	if (depth >= limit_depth) {
		if (cond_flag & B_C_UCHIFU) {
			help_self_escape_check_place_FU(php);
		} else if (rule_flag & B_R_HELP_SELF_MATE) {
			void (* save_help_escape_place)(phase_t *php) = help_escape_place;
			void (* save_help_escape_move_sub)(phase_t *php) = help_escape_move_sub;
			help_escape_place = help_escape_check_place;
			help_escape_move_sub = help_escape_check_move_sub;
			help_escape_all_ascetic(php);
			if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
				help_escape_all_greed(php);
			}
			help_escape_place = save_help_escape_place;
			help_escape_move_sub = save_help_escape_move_sub;
		} else {
			help_escape_all_ascetic(php);
			if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
				help_escape_all_greed(php);
			}
		}
	} else {
		help_escape_all_ascetic(php);
		if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
			help_escape_all_greed(php);
		}
	}

	HELP_SELF_ESCAPE_PART_2
}

void
help_self_escape_check_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	for (i = 0; NOT_NUL(hand_pc[i]); i++) {
		pc = hand_pc[i];
		if (php->yo_hand[pc] >= 1) {
			mvp->pc = mvp->fpc = pc;
			help_escape_check_place_sub(php);
		}
	}

	return;
}

// ホッパやジャンパなどのフェアリー駒がおらず、
// 変身系条件がない場合
void
help_self_escape_check_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if (php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) {
//	if (yo_close_check_board[php->hi_ou_pos][mvp->to] == ON ||
//			yo_remote_check_board[php->hi_ou_pos][mvp->to] == ON) {
//		// 跳び駒の逆利き位置、跳々駒、走り駒の逆走り道の場合
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] >= 1) {
				mvp->pc = mvp->fpc = pc;
				help_escape_check_place_sub(php);
			}
		}
	}

	return;
}

// 対面、背面、ネコネコ鮮、マドラシ系
void
help_self_escape_check_place_taihaink2mad(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(php->fire[mvp->to] & B_YO_ESCAPE_SPECIAL) ||
			(depth == 1 && NOT(php->check_state & B_CS_HITHER_CHECK))) {	// 受先の初手用ルート
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] > 0) {
				mvp->pc = mvp->fpc = pc;
				help_escape_check_place_sub(php);
			}
		}
	}

	return;
}

static void
help_self_escape_check_place_FU(phase_t *php)
{
	if (php->yo_hand[PC_FU] >= 1) {
		move_t *mvp = &php->move;
		int pos;
		uchar_t bpc;
		mvp->pc = mvp->fpc = PC_FU;
		mvp->from = NOP;
		for (pos = 0; pos < 81; pos++) {
			bpc = php->board[pos];
			if (IS_ROOM(bpc)) {
				mvp->to = pos;
				help_escape_check_move_sub(php);
			}
		}
	}

	return;
}

void
help_self_escape_place_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0 && check(php + 1) == 0) {
		php->state |= B_ESCAPE_MOVE_FOUND;
	}

	return;
}

void
help_self_escape_move_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0 && check(php + 1) == 0) {
		php->state |= B_ESCAPE_MOVE_FOUND;
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag = B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
            php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

// キルケ対応
void
help_self_escape_move_sub_circe(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0) {
		if (check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move(php) == 0 && check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag = B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			if (check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (yo_move(php) == 0 && check(php + 1) == 0) {
					php->state |= B_ESCAPE_MOVE_FOUND;
				}
			}
		}
	}

	return;
}

void
help_self_escape_check_place_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0 && check(php + 1) == 0) {
		php->state |= B_ESCAPE_MOVE_FOUND;
	}

	return;
}

void
help_self_escape_check_place_sub_normal(phase_t *php)
{
	move_t *mvp = &php->move;

	if (yo_to_board[mvp->fpc][mvp->to][php->hi_ou_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

void
help_self_escape_check_place_sub_madrasi(phase_t *php)
{
	move_t *mvp = &php->move;

	if (yo_to_board[mvp->fpc][mvp->to][php->hi_ou_pos] == ON ||
			(php->fire[mvp->to] & B_YO_CHECK_SPECIAL)) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

void
help_self_escape_check_place_sub_annanhoku(phase_t *php)
{
	move_t *mvp = &php->move;
	int bpos, fpos;

	mvp->fpc = mvp->pc;
	fpos = NOP;
	switch (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
	case B_C_ANNAN:
		bpos = fpc_pos_yo_annan[mvp->to];
		if (bpos != NOP && IS_YO_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		fpos = fpc_pos_forward[mvp->to];
		if (fpos == NOP || NOT(IS_YO_PC(php->board[fpos]))) {
			fpos = NOP;
		}
		break;
	case B_C_ANHOKU:
		bpos = fpc_pos_yo_anhoku[mvp->to];
		if (bpos != NOP && IS_YO_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		fpos = fpc_pos_back[mvp->to];
		if (fpos == NOP || NOT(IS_YO_PC(php->board[fpos]))) {
			fpos = NOP;
		}
		break;
	}
	if (yo_to_board[mvp->fpc][mvp->to][php->hi_ou_pos] == ON ||
			(fpos != NOP && yo_to_board[mvp->pc][fpos][php->hi_ou_pos] == ON)) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

void
help_self_escape_check_place_sub_taihaimen(phase_t *php)
{
	move_t *mvp = &php->move;
	int bpos;

	mvp->fpc = mvp->pc;
	bpos = NOP;
	switch (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
	case B_C_TAIMEN:
		bpos = fpc_pos_yo_taimen[mvp->to];
		if (bpos != NOP && IS_HI_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		break;
	case B_C_HAIMEN:
		bpos = fpc_pos_yo_haimen[mvp->to];
		if (bpos != NOP && IS_HI_PC(php->board[bpos])) {
			mvp->fpc = KIND(php->board[bpos]);
		}
		break;
	}
	if (yo_to_board[mvp->fpc][mvp->to][php->hi_ou_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

void
help_self_escape_check_place_sub_nekosen(phase_t *php)
{
	move_t *mvp = &php->move;
	int pos1, pos2, i;
	int fpos[9] = { NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, };
	uchar_t fpc[9] = { NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, };

	switch (cond_flag & (B_C_NEKOSEN | B_C_NEKO2SEN)) {
	case B_C_NEKOSEN:
		for (pos1 = mvp->to; fpc_pos_forward[pos1] != NOP && IS_YO_PC(php->board[pos1 - 1]); pos1--)
			;
		for (pos2 = mvp->to; fpc_pos_back[pos2] != NOP && IS_YO_PC(php->board[pos2 + 1]); pos2++)
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
		if ((IS_YO_PC(php->board[fpos[i]]) || fpos[i] == mvp->to) &&
				yo_to_board[fpc[i]][fpos[i]][php->hi_ou_pos] == ON) {
			mvp->flag = 0x00;
			mvp->flag2 = 0x00;
			if (yo_move(php) == 0 && check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
			break;
		}
	}

	return;
}

void
help_self_escape_check_move_sub_loose(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0 && check(php + 1) == 0) {
		php->state |= B_ESCAPE_MOVE_FOUND;
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
            php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}

	return;
}

// キルケ対応
void
help_self_escape_check_move_sub_circe(phase_t *php)
{
	move_t *mvp = &php->move;

	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (yo_move(php) == 0) {
		if (check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
		if (mvp->flag2 & B_MV2_REBORN_MORE) {
			if (yo_move(php) == 0 && check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		mvp->flag |= B_MV_PROMOTE;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			if (check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (yo_move(php) == 0 && check(php + 1) == 0) {
					php->state |= B_ESCAPE_MOVE_FOUND;
				}
			}
		}
	}

	return;
}

void
help_self_escape_check_move_sub_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int indirect_check = FALSE;

	if (yo_to_board[mvp->pc][mvp->to][php->hi_ou_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
            php->state |= B_ESCAPE_MOVE_FOUND;
		}
	} else if (yo_remote_check_board[php->hi_ou_pos][mvp->from] == ON) {
		indirect_check = TRUE;
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0 && check(php + 1) == 0) {
			php->state |= B_ESCAPE_MOVE_FOUND;
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		if (yo_to_board[mvp->pc | B_PC_PROMOTED][mvp->to][php->hi_ou_pos] == ON ||
				indirect_check == TRUE) {
			mvp->flag |= B_MV_PROMOTE;
			mvp->flag2 = 0x00;
			if (yo_move(php) == 0 && check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
		}
	}

	return;
}

// キルケ対応
void
help_self_escape_check_move_sub_normal_circe(phase_t *php)
{
	move_t *mvp = &php->move;
	int indirect_check = FALSE;

	if (yo_to_board[mvp->pc][mvp->to][php->hi_ou_pos] == ON) {
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			if (check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (yo_move(php) == 0 && check(php + 1) == 0) {
					php->state |= B_ESCAPE_MOVE_FOUND;
				}
			}
		}
	} else if (yo_remote_check_board[php->hi_ou_pos][mvp->from] == ON) {
		indirect_check = TRUE;
		mvp->flag = 0x00;
		mvp->flag2 = 0x00;
		if (yo_move(php) == 0) {
			if (check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
			if (mvp->flag2 & B_MV2_REBORN_MORE) {
				if (yo_move(php) == 0 && check(php + 1) == 0) {
					php->state |= B_ESCAPE_MOVE_FOUND;
				}
			}
		}
	}
	if (IS_PROMOTABLE(mvp->pc) && IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from)) {
		if (yo_to_board[mvp->pc | B_PC_PROMOTED][mvp->to][php->hi_ou_pos] == ON ||
				indirect_check == TRUE) {
			mvp->flag |= B_MV_PROMOTE;
			mvp->flag2 = 0x00;
			if (yo_move(php) == 0) {
				if (check(php + 1) == 0) {
					php->state |= B_ESCAPE_MOVE_FOUND;
				}
				if (mvp->flag2 & B_MV2_REBORN_MORE) {
					if (yo_move(php) == 0 && check(php + 1) == 0) {
						php->state |= B_ESCAPE_MOVE_FOUND;
					}
				}
			}
		}
	}

	return;
}

