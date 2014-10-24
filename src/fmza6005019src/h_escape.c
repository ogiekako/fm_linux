// za/h_escape.c -- zoo+single-change help-mate escape routines
// Copyright(c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/h_escape.c 1.34 2014/08/18 12:54:30 budai10 Exp budai10 $

#include	<stdlib.h>
#include	<mbstring.h>
#include	"fm.h"
#include	"piece.h"

// ----------------------------------------------------------------------------- 
#define	HELP_ESCAPE_PART_1 \
	/* �����̏I������ */ \
	if (solution_count >= solution_limit || depth > limit_depth) { \
		return 0; \
	} \
	depth++; \
 \
	/* ����ǖʂ���� */ \
	if (analyze_phase(php) != 0) { \
		goto return_1; \
	} \
 \
	/* ���[����̌����Ő؂�����i�������`�F�b�N�j */ \
	if (php->check_state & B_CS_YONDER_CHECK) { \
		goto return_1; \
	} \
 \
	/* ���[����̌����Ő؂�����i����`���`�F�b�N�j */ \
	/* �������A���̏ꍇ�ɂ͏��`�ł͉����ԂłȂ����Ƃ��l�� */ \
	/* �܂��A���̓X�e�C�����C�g�̍ŏI���ł͉����ԂłȂ����Ƃ��l�� */ \
	if (NOT(php->check_state & B_CS_HITHER_CHECK) && depth >= 2 && \
			(NOT(rule_flag & B_R_HELP_STALEMATE) || depth <= limit_depth)) { \
		goto return_1; \
	} \
 \
	/* ��͏󋵂��`�F�b�N�i��͏󋵕\���j */ \
	if (++phase_count >= check_point) { \
		mile_stone(); \
	} \
 \
	/* �����ǖʍ팸�F�ǖʂ̑Ώ̐��`�F�b�N */ \
	if (((php - 1)->state & B_SYMMETRIC) && depth >= 2 && test_symmetric(php) == 0) { \
		goto return_0; \
	} \
 \
	/* �g���[�X����̌����Ő؂���� */ \
	if (trace_limit >= 1 && depth >= 2 && trace_move(php) != 0) { \
		goto return_0; \
	} \
 \
	/* �ŕ��l�̂Ƃ��A�w��l�萔�̂P��O�Ŏ����[�����`�F�b�N */ \
	/* ���ŕ��l�̏ꍇ�A�����l������K�v������B */ \
	if (depth == limit_depth - 1 && (cond_flag & B_C_UCHIFU) && php->hi_hand[PC_FU] == 0) { \
		if ((php - 1)->move.pc == PC_FU && (php - 1)->move.from == NOP && is_yonder_mate(php)) { \
			help_mate_rule_check(php); \
		} \
		goto return_0; \
	} \
 \
	/* �����ǖʍ팸�F�U���̓���ǖʂ��`�F�b�N�A�o�^ */ \
	if (depth < limit_depth && (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME))) { \
		hi_hash_diff(php); \
		if ((strategy & B_HI_DENY_SAME)) { \
			/* �菇������ǖʂ��`�F�b�N�A�U���̕s�l����ǖʂ��`�F�b�N */ \
			/* ����ǖʂłȂ������ꍇ�ɁA�菇������ǖʂ�o�^ */ \
			int is_same = is_hi_same_phase(php); \
			if (is_same != 0) { \
				goto return_0; \
			} \
		} \
	} \
 \
	/* �l�萔�𒴂��Ă���΁A�l/�X�e�C�����C�g���`�F�b�N */ \
	if (depth > limit_depth) { \
		switch (rule_flag) { \
		case B_R_HELP_MATE: \
			if (is_yonder_mate(php)) { \
				help_mate_rule_check(php); \
			} \
			break; \
		case B_R_HELP_STALEMATE: \
			if (NOT(php->check_state & B_CS_HITHER_CHECK) && is_yonder_stalemate(php)) { \
				help_mate_rule_check(php); \
			} \
			break; \
		} \
		goto return_0; \
	}
// ----------------------------------------------------------------------------- 
#define	HELP_ESCAPE_PART_2 \
	if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) { \
		/* �������肪���o�ł��Ȃ������ꍇ */ \
		if ((rule_flag & B_R_HELP_MATE)&& is_yonder_mate(php)) { \
			/* ���͋l�Ȃ�΁A�l���`�F�b�N */ \
			help_mate_rule_check(php); \
		}\
	} \
 \
	/* �����ǖʍ팸�F�U���̓���ǖʂ�o�^�A���� */ \
	if ((strategy & B_HI_DENY_SAME) && depth < limit_depth) { \
		ulong_t index; \
 \
		/* �菇������ǖʃ`�F�b�N�p�n�b�V���l������ */ \
		index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands; \
		index &= NOSPHHEAD - 1; \
		hi_sph_head[index] = hi_sph_head[index]->next; \
		/* �l�菇���̋ǖʂłȂ���΁A�U���̕s�l����ǖʂ�o�^ */ \
		/* �i�𐔏�������̏ꍇ�������j */ \
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

static void help_escape_exchange(phase_t *php);

int
help_escape_normal(phase_t *php)
{
	HELP_ESCAPE_PART_1

	// �������茟��
	help_escape_all(php);

	HELP_ESCAPE_PART_2
}

int
help_escape_messigny(phase_t *php)
{
	HELP_ESCAPE_PART_1

	// �������茟��
	help_escape_all_messigny(php);

	HELP_ESCAPE_PART_2
}

int
help_escape_greed(phase_t *php)
{
	HELP_ESCAPE_PART_1

	// �������茟��
	help_escape_all_greed(php);
	if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
		help_escape_all_ascetic(php);
	}

	HELP_ESCAPE_PART_2
}

int
help_escape_ascetic(phase_t *php)
{
	HELP_ESCAPE_PART_1

	// �������茟��
	help_escape_all_ascetic(php);
	if (NOT(php->state & B_ESCAPE_MOVE_FOUND)) {
		help_escape_all_greed(php);
	}

	HELP_ESCAPE_PART_2
}

void
help_escape_all(phase_t *php)
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
			help_escape_place(php);
		} else if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_escape_leap(php);
			piece[mvp->fpc].help_escape_run(php);
		}
	}

	return;
}

void
help_escape_all_messigny(phase_t *php)
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
			help_escape_place(php);
		} else if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			help_escape_exchange(php);
			piece[mvp->fpc].help_escape_leap(php);
			piece[mvp->fpc].help_escape_run(php);
		}
	}

	return;
}

void
help_escape_all_greed(phase_t *php)
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
		if (IS_YO_PC(bpc)) {
			mvp->pc = KIND(bpc);
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_escape_leap(php);
			piece[mvp->fpc].help_escape_run(php);
		}
	}
	php->state &= ~B_MODE_GREED;

	return;
}

void
help_escape_all_ascetic(phase_t *php)
{
	move_t *mvp;
	int pos, i;
	uchar_t bpc, pc;

	php->state |= B_MODE_ASCETIC;
	mvp = &php->move;
	mvp->to_1st = NOP;
	for (i = 0; i < 81; i++) {
		pos = pos_order[php->yo_gy_pos][i];
		bpc = php->board[pos];
		if (IS_ROOM(bpc)) {
			mvp->to = pos;
			mvp->from = NOP;
			help_escape_place(php);
		} else if (IS_YO_PC(bpc)) {
			pc = KIND(bpc);
			mvp->pc = pc;
			mvp->from = pos;
			mvp->fpc = php->fpc_board[pos];
			piece[mvp->fpc].help_escape_leap(php);
			piece[mvp->fpc].help_escape_run(php);
		}
	}
	php->state &= ~B_MODE_ASCETIC;

	return;
}

void
help_escape_place_loose(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	for (i = 0; NOT_NUL(hand_pc[i]); i++) {
		pc = hand_pc[i];
		if (php->yo_hand[pc] > 0) {
			mvp->pc = mvp->fpc = pc;
			help_escape_place_sub(php);
		}
	}

	return;
}

void
help_escape_place_normal(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(depth == 1 && NOT(php->check_state & B_CS_HITHER_CHECK))) {	// ���̏���p���[�g
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] > 0) {
				mvp->pc = mvp->fpc = pc;
				help_escape_place_sub(php);
			}
		}
	}

	return;
}

// �ΖʁA�w�ʁA�l�R�l�R�N�A�}�h���V�n
void
help_escape_place_taihaink2mad(phase_t *php)
{
	move_t *mvp = &php->move;
	int i;
	uchar_t pc;

	if ((php->check_state == (B_CS_HITHER_CHECK | B_CS_REMOTE_CHECK) &&
				(php->fire[mvp->to] & B_HI_FIRE_PATH)) ||
			(php->fire[mvp->to] & B_YO_ESCAPE_SPECIAL) ||
			(depth == 1 && NOT(php->check_state & B_CS_HITHER_CHECK))) {	// ���̏���p���[�g
		for (i = 0; NOT_NUL(hand_pc[i]); i++) {
			pc = hand_pc[i];
			if (php->yo_hand[pc] > 0) {
				mvp->pc = mvp->fpc = pc;
				help_escape_place_sub(php);
			}
		}
	}

	return;
}

static void
help_escape_exchange(phase_t *php)
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
			if (yo_move(php) == 0 && check(php + 1) == 0) {
				php->state |= B_ESCAPE_MOVE_FOUND;
			}
		}
	}

	return;
}

void
help_escape_leap(phase_t *php)
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
//		if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			help_escape_move_sub(php);
		}
	}

	return;
}

void
help_escape_shishi_leap(phase_t *php)
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
//		if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
		if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
				(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
			mvp->to = to;
			help_escape_move_sub(php);
			if (IS_HI_PC(to_sq) && IS_AROUND(mvp->from, to)) {
				mvp->to_1st = to;
				for (j = 0; ; j++) {
					to2 = yo_leap_to_pos[PC_OU][to][j];
					if (to2 == EOP) {
						break;
					}
					if (_mbsstr(piece[mvp->pc].name, "���q") == NULL) {
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
//					if (NOT(IS_YO_PC(to_sq2)) || to2 == mvp->from) {
					if (((IS_ROOM(to_sq2) || to2 == mvp->from) && NOT(php->state & B_MODE_GREED)) ||
							(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						help_escape_move_sub(php);
					}
				next_for:;
				}
				mvp->to_1st = NOP;
			}
		}
	}
	mvp->to = mvp->from;
	help_escape_move_sub(php);

	return;
}

void
help_escape_step(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, to_next, i, j;
	uchar_t to_sq;

	for (j = 0; yo_run_to_posv[mvp->fpc][mvp->from][j] != NULL; j++) {
		for (i = 0, to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][0]; ; i++) {
			to = to_next;
			to_sq = php->board[to];
			to_next = yo_run_to_posv[mvp->fpc][mvp->from][j][i + 1];
			if (to_next == EOP) {
//				if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to;
					help_escape_move_sub(php);
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
// hi_run_to_posv[��][�ʒu][�x�N�g���C���f�N�X]: ����_�ʒu���X�g�i�U���A������p�j
// yo_run_to_posv[��][�ʒu][����_�ʒu]: �����ʒu���X�g�i�U���A������p�j
void
help_escape_mao_step(phase_t *php)
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
//			if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				help_escape_move_sub(php);
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
help_escape_run(phase_t *php)
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
//			if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to;
				help_escape_move_sub(php);
			}
			if (NOT_PASSABLE(to_sq)) {
				break;
			}
		}
	}

	return;
}

void
help_escape_hop(phase_t *php)
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
//				if (IS_ROOM(to_sq2) || IS_HI_PC(to_sq2)) {
				if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
					mvp->to = to2;
					help_escape_move_sub(php);
				}
				break;
			}
		}
	}

	return;
}

void
help_escape_decapt_hop(phase_t *php)
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
						help_escape_move_sub(php);
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
help_escape_pao_jump(phase_t *php)
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
//				if (IS_ROOM(to_sq)) {
				if (IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) {
					mvp->to = to;
					help_escape_move_sub(php);
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
							help_escape_move_sub(php);
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
help_escape_jump(phase_t *php)
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
//						if (IS_ROOM(to_sq2)) {
						if (IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) {
							mvp->to = to2;
							help_escape_move_sub(php);
						}
					} else {
//						if (IS_HI_PC(to_sq2)) {
						if (IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC)) {
							mvp->to = to2;
							help_escape_move_sub(php);
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
help_escape_jump_hop(phase_t *php)
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
//					if (IS_ROOM(to_sq2) || IS_HI_PC(to_sq2)) {
					if ((IS_ROOM(to_sq2) && NOT(php->state & B_MODE_GREED)) ||
							(IS_HI_PC(to_sq2) && NOT(php->state & B_MODE_ASCETIC))) {
						mvp->to = to2;
						help_escape_move_sub(php);
					}
					break;
				}
			}
		}
	}

	return;
}

// Sparrow, Eagle:
//   hi_run_to_posv[��][�ʒu][�x�N�g��][�����ʒu���X�g]: ���ˋ���ʒu���X�g�i�U���A������p�j
//   yo_run_to_posv[��][�ʒu][���ˋ�ʒu][�����ʒu���X�g]: �����ʒu���X�g�i�U���A������p�j
//   hi_to_board: ���ˋ���ʒu�Ֆʃ}�b�v�i�U���A������p�j
void
help_escape_refl(phase_t *php)
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
//				if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to) {
							goto another_to;
						}
					}
					to_done[k] = to;
					mvp->to = to;
					help_escape_move_sub(php);
				}
			another_to:;
				to = yo_run_to_posv[mvp->fpc][mvp->from][div][1];
				if (to == EOP) {
					break;
				}
				to_sq = php->board[to];
//				if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
				if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
						(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
					for (k = 0; to_done[k] != NOP; k++) {
						if (to_done[k] == to) {
							goto next_vec;
						}
					}
					to_done[k] = to;
					mvp->to = to;
					help_escape_move_sub(php);
				}
			next_vec:;
				break;
			}
		}
	}

	return;
}

// Equihopper:
//   hi_run_to_posv[0]: �����ʒu���X�g
//   yo_run_to_posv[0]: �Ώ̓_���X�g
void
help_escape_equi_hop(phase_t *php)
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
//			if (IS_ROOM(to_sq) || IS_HI_PC(to_sq)) {
			if ((IS_ROOM(to_sq) && NOT(php->state & B_MODE_GREED)) ||
					(IS_HI_PC(to_sq) && NOT(php->state & B_MODE_ASCETIC))) {
				mvp->to = to[i];
				help_escape_move_sub(php);
			}
		}
	}

	return;
}

void
help_escape_circle_run(phase_t *php)
{
	move_t *mvp = &php->move;
	int to, i, j, k;
	uchar_t to_sq;
	char to_done[32] = {
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
		NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	};

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
					ASSERT(k < 32);
					if (to_done[k] == to) {
						goto next_to;
					}
				}
				to_done[k] = to;
				mvp->to = to;
				help_escape_move_sub(php);
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
help_escape_place_sub_loose(phase_t *php)
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
help_escape_move_sub_loose(phase_t *php)
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

// �L���P�Ή�
void
help_escape_move_sub_circe(phase_t *php)
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

