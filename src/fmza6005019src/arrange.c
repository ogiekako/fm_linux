// zaco/arrange.c -- problem arranger of FM solver
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/arrange.c 1.21 2014/08/17 02:13:42 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<mbstring.h>
#include	<ctype.h>
#include	"fm.h"
#include	"piece.h"

static int check_constraint_sq(phase_t *php);
static int check_constraint_2f(phase_t *php);
static int check_constraint_3t(phase_t *php);

// ���ݒ�
int
arrange(FILE *Fp)
{
	phase_t *php;
	char *ib = NULL, *save_ib, *s;
	int mode = HI_PC_ON_BOARD;
	int free_pc_count = OFF;
	int pos, n, i;
	ushort_t work_limit_depth;
	uchar_t pc;

	if (Fp == NULL) {
		return -1;
	}
	init_phase = null_phase;
	php = &init_phase;

	LOOP {
		ib = fgets(inbuf, sizeof(inbuf), Fp);
		if (ib == NULL) {
			error_message("���̓f�[�^���������Ă��܂���i�����Ɂu.�v�Ɓu#�萔�v�̂ǂ�����Ȃ��j",
					NULL);
			return -1;
		}
		s = _mbschr(inbuf, '\n');
		if (s != NULL) {
			s[0] = '\0';
		}
		ib = skip_space(ib);
		if (ib[0] == '\n' || ib[0] == '\0') {
			// ��s
			continue;
		}
		if (ib[0] == ';') {
			// �R�����g
			continue;
		}
		if (ib[0] == '/') {
			// �I�v�V����
			ib++;
			toasciiuc(ib);
			switch (ib[0]) {
			case 'O':		// output file name
				ib++;
				strcpy(out_file, ib);
				fileFp = fopen(out_file, "a");
				if (fileFp == NULL) {
					fprintf(stderr, "%s: �o�̓t�@�C�� %s ���I�[�v���ł��܂���...\n\n",
							cmdname, out_file);
				}
				break;
			case 'B':
				ib++;
				switch (ib[0]) {
				case 'M':
					mate_print = OFF;
					break;
				default:
					brief_output = 2;
					break;
				}
				break;
			case 'L':
				ib++;
				option_L(ib);
				break;
			case 'E':
				ib++;
				option_E(ib);
				init_phase = null_phase;
				break;
			case 'M':
				ib++;
				option_M(ib);
				break;
			case 'I':
				ib++;
				strcpy(in_file, ib);
				break;
			case 'X':
				ib++;
				option_X(ib);
				break;
			}
			continue;
		}
		// ��i��
		ib = get_title(ib);
		ib = skip_space(ib);
		if (ib[0] == '\n' || ib[0] == '\0') {
			continue;
		}
		// ���[���E����
		toasciiuc(ib);
		ib = get_rule(ib);
		if (ib == NULL) {
			return -1;
		}
		ib = skip_space(ib);
		if (ib[0] == '\n' || ib[0] == '\0') {
			continue;
		}
		// ���
		while (ib != NULL && ib[0]) {
			if (ib - inbuf >= 512) {
				break;
			}
			switch (ib[0]) {
			case ' ':
			case '\n':
				ib++;
				continue;
			case '+':
				ib++;
				if (mode == HI_PC_ON_BOARD) {
					mode = HI_PC_IN_HAND;
				} else if (mode == YO_PC_ON_BOARD) {
					if (ib[0] == '*') {
						ib++;
					} else if (ib[0] == '0') {
						ib++;
						for (pc = PC_FU; pc < RAW_PC_END; pc++) {
							php->yo_hand[pc] = 0;
						}
					} else {
						save_ib = ib;
						pc = get_pc_code(&ib);
						ib = save_ib;
						if (NOT(IS_NUL(pc) || pc == ROOM || pc == ROCK || pc == PIT)) {
							for (pc = PC_FU; pc < RAW_PC_END; pc++) {
								php->yo_hand[pc] = 0;
							}
						}
					}
					free_pc_count = ON;
					mode = YO_PC_IN_HAND;
				}
				continue;
			case ',':
				ib++;
				mode = YO_PC_ON_BOARD;
				continue;
			case '#':
				ib++;
				if (ib[0] == '#') {
					ib++;
					max_limit_depth = 0xffff;
				} else {
					max_limit_depth = 0;
				}
				n = strtol(ib, &s, 10);
				ib = s;
				if (n > LIMIT_DEPTH) {
					error_message("�l�萔���傫�߂��܂�", NULL);
					return -1;
				}
				limit_depth = n;
				if (max_limit_depth == 0xffff) {
					if (ib[0] == '-') {
						ib++;
						min_limit_depth = limit_depth;
						n = strtol(ib, &s, 10);
						ib = s;
						if (n > LIMIT_DEPTH) {
							error_message("�l�萔���傫�߂��܂�", NULL);
							return -1;
						}
						limit_depth = n;
						if (limit_depth <= min_limit_depth) {
							error_message("���������w�肪�Ԉ���Ă��܂�", NULL);
							return -1;
						}
					} else {
						min_limit_depth = 0;
					}
					max_limit_depth = limit_depth;
				}
				goto problem_test;
			case '.':
				if (limit_depth == 0) {
					error_message("�l�萔���w�肳��Ă��܂���", NULL);
					return -1;
				}
				goto problem_test;
			}

			switch (mode) {
			case HI_PC_ON_BOARD:
			case YO_PC_ON_BOARD:
				save_ib = ib;
				pos = get_pos(&ib);
				if (pos < 0 || pos >= 81) {
					error_message("��ʒu�w�肪�Ԉ���Ă��܂�", save_ib);
					return -1;
				}
				if (NOT_ROOM(php->board[pos])) {
					error_message("��ʒu���d�����Ă��܂�", save_ib);
					return -1;
				}
				save_ib = ib;
				pc = get_pc_code(&ib);
				if (IS_NUL(pc) || IS_ROOM(pc)) {
					error_message("�Ջ�̋�����Ԉ���Ă��܂�", save_ib);
					return -1;
				}
				if (piece[pc].attr & B_PA_ROYAL) {
					// ��������
					if (mode == HI_PC_ON_BOARD) {
						php->hi_ou_pos = pos;
						php->board[pos] = pc | B_PC_HITHER;
					} else {
						php->yo_gy_pos = pos;
						php->board[pos] = pc | B_PC_YONDER;
					}
				} else if (pc == PIT) {
					// �v
					php->board[pos] = SQ_PIT;
				} else if (pc == ROCK) {
					// ��
					php->board[pos] = SQ_ROCK;
				} else {
					php->yo_hand[RAW_KIND(pc)]--;
					if (mode == HI_PC_ON_BOARD) {
						php->board[pos] = pc | B_PC_HITHER;
					} else {
						php->board[pos] = pc | B_PC_YONDER;
					}
				}
				break;
			case HI_PC_IN_HAND:
			case YO_PC_IN_HAND:
				save_ib = ib;
				pc = get_pc_code(&ib);
				if (IS_NUL(pc) || pc == ROOM || pc == ROCK || pc == PIT) {
					error_message("����̋�����Ԉ���Ă��܂�", save_ib);
					return -1;
				}
				if (NOT(piece[pc].attr & B_PA_PLACABLE)) {
					error_message("����ɂł��Ȃ���w�肳��Ă��܂�", save_ib);
					return -1;
				}
				save_ib = ib;
				if (isdigit((uchar_t)ib[0])) {
					n = strtol(ib, &s, 10);
					ib = s;
				} else {
					n = 1;
				}
				if (mode == HI_PC_IN_HAND) {
					php->hi_hand[pc] += n;
					php->yo_hand[pc] -= n;
				} else {
					php->yo_hand[pc] = n;
				}
				break;
			default:
				error_message("���̓f�[�^�������ł��܂���", ib);	// ����Ȃ��ƁA�Ȃ��͂�
				return -1;
			}
		}
	}

problem_test:
	n = 0;
	for (pos = 0; pos < 81; pos++) {
		pc = php->board[pos];
		if (IS_YO_PC(pc) && (piece[KIND(pc)].attr & B_PA_ROYAL)) {
			n++;
		}
	}
	if (n == 0) {
		error_message("����ɉ������̋����܂���", NULL);
		return -1;
	}
	if (n >= 2) {
		cond_flag |= B_C_W_GY;
	}
	for (i = 0; NOT_NUL(hand_pc_order[i]); i++) {
		pc = hand_pc_order[i];
		if ((piece[pc].attr & B_PA_DEFINED) && php->yo_hand[pc] < 0) {
			if (free_pc_count == OFF) {
				error_message("����W������O��Ă��܂�", NULL);
				return -1;
			}
			php->yo_hand[pc] = 0;
		}
	}
	correct_for_rule();
	if (cond_flag & BM_C_CIRCE_FAMILY) {
		for (pc = PC_FU; pc < PC_END; pc++) {
			if (piece[pc].move_style & B_MS_SHISHI_LEAP) {
				error_message("�L���P�n�ł́A���q���ы�͖��T�|�[�g�ł�", NULL);
				return -1;
			}
		}
	}
	if (cond_flag & B_C_ISARDAM) {
		for (pc = PC_FU; pc < PC_END; pc++) {
			if (piece[pc].move_style &
					(B_MS_SHISHI_LEAP | B_MS_MAO_STEP | B_MS_HOP | B_MS_DECAPT_HOP |
					 B_MS_JUMP_HOP | B_MS_REFL135 | B_MS_REFL90)) {
				error_message("Isardam�ł́A���q���ы�AMao���ы�A�f�n��A囌n��A�J���K���[�n��A"
						"���A�h�͖��T�|�[�g�ł�", NULL);
				return -1;
			}
		}
	}
	if (check_constraint_sq(php) != 0) {
		error_message("�s�����̂Ȃ������܂�", NULL);
		return -1;
	}
	if (check_constraint_2f(php) != 0) {
		error_message("���������܂�", NULL);
		return -1;
	}
	if (check_constraint_3t(php) != 0) {
		error_message("�O��������܂�", NULL);
		return -1;
	}
	// �g�p��v��
	setup_used_pc_count(&init_phase);
	// �l�萔��񏉊��ݒ�
	work_limit_depth = limit_depth;
	if (max_limit_depth != 0) {
		work_limit_depth = max_limit_depth;
		if (min_limit_depth != 0) {
			limit_depth = min_limit_depth;
		} else {
			limit_depth = max_limit_depth % 2 == 0 ? 2 : 1;
		}
	}
	// �ǖʍ\���̔z�񐶐�
	phase = calloc(work_limit_depth + xtra_depth, sizeof(*phase));
	if (phase == NULL) {
		fm_exit_mae("�ǖʍ\���̔z��̈�");
	}
	phase[0] = null_phase;
	phase[0].move.from = NOP;
	phase[1] = init_phase;
	if (cond_flag & B_C_NOCAPT) {
		init_phase_state |= B_MODE_ASCETIC;
		phase[1].state |= init_phase_state;
	}
	memcpy(phase[1].fpc_board, phase[1].board, sizeof(phase[1].fpc_board)); 
	// �l�菇�ҏW�̈搶��
	mate_buf = malloc(15 * work_limit_depth + 128);
	if (mate_buf == NULL) {
		fm_exit_mae("�l�菇�ҏW�̈�");
	}
	// �ŏI�����菇�p�l��\���̔z��̈搶��
	last_move = calloc(work_limit_depth + xtra_depth, sizeof(last_move[0]));
	if (last_move == NULL) {
		fm_exit_mae("�ŏI�����菇�p�l��\���̔z��̈�");
	}
	// �ǖʏ����ݒ�
	phase[2] = phase[1];	// phase[2] ����Ɨp�Ɏg�p
	if (rule_flag & B_R_HELP_MATE) {
		if (limit_depth % 2 == 0) {
			phase[2].state &= ~B_HITHER_TURN;
		} else {
			phase[2].state |= B_HITHER_TURN;
		}
	} else {
		if (limit_depth % 2 == 0) {
			phase[2].state |= B_HITHER_TURN;
		} else {
			phase[2].state &= ~B_HITHER_TURN;
		}
	}
	// �U������ǖʃw�b�_���ݒ�
	if (strategy & B_HI_ESCAPE) {
		if (no_hi_eh_head == 0) {
			no_hi_eh_head = (long)(hi_eh_lim / 10 > 0x40000000 ? 0x40000000 : hi_eh_lim / 10);
			for (i = 0; no_hi_eh_head > 0; i++) {
				no_hi_eh_head >>= 1;
			}
			no_hi_eh_head = 1 << i;
		}
	}
	// �������ǖʃw�b�_���ݒ�
	if (strategy & B_YO_ESCAPE) {
		if (no_yo_eh_head == 0) {
			no_yo_eh_head = (long)(yo_eh_lim / 10 > 0x40000000 ? 0x40000000 : yo_eh_lim / 10);
			for (i = 0; no_yo_eh_head > 0; i++) {
				no_yo_eh_head >>= 1;
			}
			no_yo_eh_head = 1 << i;
		}
	}
	// �����ǖʕ\��
	print_start(stdout, OFF);
	// �����ǖʌ���
	set_entry();
	is_symmetric(&phase[1]);
	if (analyze_phase(&phase[2]) != 0) {
		error_message("�Ֆʂ����炩�̐���Ɋ|�����Ă��܂�", NULL);
		return -1;
	}
	if (((rule_flag & B_R_HELP_MATE) && limit_depth % 2 != 0) ||
			((rule_flag & (B_R_HELP_SELF_MATE | B_R_HELP_SELF_STALEMATE)) &&
			 limit_depth % 2 == 0)) {
		if (phase[2].check_state & B_CS_HITHER_CHECK) {
			error_message("���ԂŊ��Ɍ��ʂɉ��肪�|�����Ă��܂�", NULL);
			return -1;
		}
	} else {
		if (phase[2].check_state & B_CS_YONDER_CHECK) {
			error_message("���ԂŊ��ɐ�艤�ɉ��肪�|�����Ă��܂�", NULL);
			return -1;
		}
		if (NOT(phase[2].check_state & B_CS_HITHER_CHECK)) {
			if (limit_depth <= 0) {
				error_message("�l�萔���Ԉ���Ă��܂�", NULL);
				return -1;
			}
		}
	}
	// �菇������ǖʊm�F�p�\���̔z��̈搶��
	sph_cell = calloc(work_limit_depth + xtra_depth, sizeof(sph_cell[0]));
	if (sph_cell == NULL) {
		fm_exit_mae("�菇���ǖʍ\���̔z��̈�");
	}
	// �U������ǖʕۑ��̈搶��
	if (strategy & B_HI_ESCAPE) {
		hi_eh_head = calloc(no_hi_eh_head, sizeof(hi_eh_head[0]));
		if (hi_eh_head == NULL) {
			fm_exit_mae("�U������ǖʍ\���̔z��̈�");
		}
		hi_eh_cell = calloc(hi_eh_lim, sizeof(hi_eh_cell[0]));
		if (hi_eh_cell == NULL) {
			fm_exit_mae("�U������ǖʍ\���̔z��̈�");
		}
		hi_eh_free = NULL;
		hi_eh_cell[hi_eh_lim - 1].next = hi_eh_free;
		hi_eh_free = &hi_eh_cell[0];
		for (i = 0; i < hi_eh_lim - 1; i++) {
			hi_eh_cell[i].next = &hi_eh_cell[i + 1];
		}
	}
	// �������ǖʕۑ��̈搶��
	if (strategy & B_YO_ESCAPE) {
		yo_eh_head = calloc(no_yo_eh_head, sizeof(yo_eh_head[0]));
		if (yo_eh_head == NULL) {
			fm_exit_mae("�������ǖʍ\���̔z��̈�");
		}
		yo_eh_cell = calloc(yo_eh_lim, sizeof(yo_eh_cell[0]));
		if (yo_eh_cell == NULL) {
			fm_exit_mae("�������ǖʍ\���̔z��̈�");
		}
		yo_eh_free = NULL;
		yo_eh_cell[yo_eh_lim - 1].next = yo_eh_free;
		yo_eh_free = &yo_eh_cell[0];
		for (i = 0; i < yo_eh_lim - 1; i++) {
			yo_eh_cell[i].next = &yo_eh_cell[i + 1];
		}
	}
	strategy |= B_HI_DENY_SAME | B_YO_DENY_SAME;
	if (strategy & B_HI_ALLOW_SAME) {
		strategy &= ~B_HI_DENY_SAME;
	}
	if (strategy & B_YO_ALLOW_SAME) {
		strategy &= ~B_YO_DENY_SAME;
	}
	if ((strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME)) || (strategy & B_YO_ESCAPE_2)) {
		make_hash_tab();
		hash_init(&phase[1]);
	}

	return 0;
}

// 0: valid, 1: invalid
static int
check_constraint_sq(phase_t *php)
{
	int pos;
	uchar_t bpc, pc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		pc = KIND(bpc);
		if ((piece[pc].attr & B_PA_PIECE) && (piece[pc].attr & B_PA_CONSTRAINT_SQ)) {
			// �s�����̂Ȃ���
			if (IS_HI_PC(bpc)) {
				if (hi_valid_sq[pc][pos] == OFF) {
					return -1;	// invalid sq
				}
			} else {
				if (yo_valid_sq[pc][pos] == OFF) {
					return -1;	// invalid sq
				}
			}
		}
	}

	return 0;
}

// 0: valid, 1: invalid
static int
check_constraint_2f(phase_t *php)
{
	int pos, pf, pr, fpos;
	uchar_t bpc, pc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		pc = KIND(bpc);
		if ((piece[pc].attr & B_PA_PIECE) && (piece[pc].attr & B_PA_CONSTRAINT_2F)) {
			// ����`�F�b�N
			pf = POS_FILE(pos);
			for (pr = 0; pr <= 8; pr++) {
				fpos = POS(pf, pr);
				if (php->board[fpos] == bpc && fpos != pos) {
					return -1;	// double FU pieces
				}
			}
		}
	}

	return 0;
}

// 0: valid, 1: invalid
static int
check_constraint_3t(phase_t *php)
{
	int pos, pf, pr, fpos, n;
	uchar_t bpc, pc;

	for (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		pc = KIND(bpc);
		if ((piece[pc].attr & B_PA_PIECE) && (piece[pc].attr & B_PA_CONSTRAINT_3T)) {
			// �O���`�F�b�N
			pf = POS_FILE(pos);
			n = 0;
			for (pr = 0; pr <= 8; pr++) {
				fpos = POS(pf, pr);
				if (php->board[fpos] == bpc && fpos != pos) {
					n++;
					if (n >= 2) {
						return -1;	// triple TTB pieces
					}
				}
			}
		}
	}

	return 0;
}

