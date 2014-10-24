// za/hs_check.c -- zoo+single-change help-self-mate check routines
// Copyright (c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/hs_check.c 1.29 2014/08/18 12:54:32 budai10 Exp budai10 $

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

// ----------------------------------------------------------------------------- 
#define	HELP_SELF_CHECK_PART_1 \
	/* ｆｍの終了条件 */ \
	if (solution_count >= solution_limit || depth > limit_depth) { \
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
	if (trace_limit >= 1 && depth >= 2 && trace_move(php) != 0) { \
		goto return_0; \
	} \
 \
	/* 打歩詰のとき、指定詰手数の１手前で持歩ゼロかチェック */ \
	/* ★打歩詰の場合、燕も考慮する必要がある。 */ \
	if (depth == limit_depth - 1 && (cond_flag & B_C_UCHIFU) && php->yo_hand[PC_FU] == 0) { \
		if ((php - 1)->move.pc == PC_FU && (php - 1)->move.from == NOP && is_hither_mate(php)) { \
			(void)help_mate_rule_check(php); \
		} \
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
	} \
 \
	/* 詰手数を超えていれば、詰/ステイルメイトをチェック */ \
	if (depth > limit_depth) { \
		switch (rule_flag) { \
		case B_R_HELP_SELF_MATE: \
			if ((php->check_state & B_CS_YONDER_CHECK) && is_hither_mate(php) && \
					help_mate_rule_check(php) == RC_MATE) { \
				/* 王手が掛かっていれば、詰をチェック */ \
				goto return_0; \
			} \
			break; \
		case B_R_HELP_SELF_STALEMATE: \
			if (NOT(php->check_state & B_CS_YONDER_CHECK) && is_hither_stalemate(php) && \
					help_mate_rule_check(php) == RC_MATE) { \
				/* 王手が掛かっていなければ、ステイルメイトをチェック */ \
				goto return_0; \
			} \
			break; \
		} \
		goto return_0; \
	}
// ----------------------------------------------------------------------------- 
#define	HELP_SELF_CHECK_PART_2 \
	if (NOT(php->state & B_CHECK_MOVE_FOUND)) { \
		/* 王手が検出できなかった場合 */ \
		switch (rule_flag) { \
		case B_R_HELP_SELF_MATE: \
			/* 協力自玉詰の場合 */ \
			if ((php->check_state & B_CS_YONDER_CHECK) && is_hither_mate(php)) { \
				/* 王手が掛かっていれば、詰をチェック */ \
				(void)help_mate_rule_check(php); \
			} \
			break; \
		case B_R_HELP_SELF_STALEMATE: \
			/* 協力自玉ステイルメイトの場合 */ \
			if (NOT(php->check_state & B_CS_YONDER_CHECK) && is_hither_stalemate(php)) { \
				/* 王手が掛かっていなければ、ステイルメイトをチェック */ \
				(void)help_mate_rule_check(php); \
			} \
			break; \
		} \
	} \
 \
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

int
help_self_check_normal(phase_t *php)
{
	HELP_SELF_CHECK_PART_1

	// 攻方取禁
	if (cond_flag & B_C_HITHER_NOCAPT) {
		php->state |= B_MODE_ASCETIC;
	}

	// 王手検索
	help_check_all(php);

	HELP_SELF_CHECK_PART_2
}

int
help_self_check_messigny(phase_t *php)
{
	HELP_SELF_CHECK_PART_1

	// 攻方取禁
	if (cond_flag & B_C_HITHER_NOCAPT) {
		php->state |= B_MODE_ASCETIC;
	}

	// 王手検索
	help_check_all_messigny(php);

	HELP_SELF_CHECK_PART_2
}

int
help_self_check_greed(phase_t *php)
{
	HELP_SELF_CHECK_PART_1

	// 王手検索
	help_check_all_greed(php);
	if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
		help_check_all_ascetic(php);
	}

	HELP_SELF_CHECK_PART_2
}

int
help_self_check_ascetic(phase_t *php)
{
	HELP_SELF_CHECK_PART_1

	// 王手検索
	help_check_all_ascetic(php);
	if (NOT(php->state & B_CHECK_MOVE_FOUND)) {
		help_check_all_greed(php);
	}

	HELP_SELF_CHECK_PART_2
}

void
help_self_check_place_sub_loose(phase_t *php)
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
help_self_check_place_sub_normal(phase_t *php)
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
help_self_check_place_sub_madrasi(phase_t *php)
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
help_self_check_place_sub_annanhoku(phase_t *php)
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
help_self_check_place_sub_taihaimen(phase_t *php)
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
help_self_check_place_sub_nekosen(phase_t *php)
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

void
help_self_check_move_sub_loose(phase_t *php)
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
help_self_check_move_sub_circe(phase_t *php)
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

void
help_self_check_move_sub_normal(phase_t *php)
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

void
help_self_check_move_sub_normal_circe(phase_t *php)
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

