// zaco/piece.c -- FM piece data
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/piece.c 1.29 2014/08/17 02:13:44 budai10 Exp budai10 $

#include	<mbstring.h>
#include	<time.h>
#include	<math.h>
#include	<conio.h>
#include	"fm.h"
#include	"piece.h"

#define	UNIT_NUM_OF_TO_POS		(10000)
#define	UNIT_NUM_OF_TO_POSV		(1000)
#define	UNIT_NUM_OF_TO_BOARD	(81)

static void debug_print_pc_attrs(int pc);

static char *to_pos;
static char **to_posv;
static int to_pos_index = UNIT_NUM_OF_TO_POS;
static int to_posv_index = UNIT_NUM_OF_TO_POSV;
static int to_board_index = UNIT_NUM_OF_TO_BOARD;
static uchar_t (*to_board)[81];
static char graph_fp[FP_EXEND][20];
static char e_fp_no[8] = { '-', 'P', '2', '3', '4', '5', '6', '7', };

static void
setup_normal_piece(int np, uchar_t pc, int promotable)
{
	char *s;
	int i;

	// piece no.
	piece[pc].pc_no = np;
	// attributes
	piece[pc].name = _strdup(piece_info[np].name);
	if (pc < RAW_PC_END && (piece_info[np].attr & B_PA_REUSABLE)) {
		pc_count[pc] = piece_info[np].max_count;
	}
	if (piece_info[np].attr & B_PA_PLACABLE) {
		for (i = 0; NOT_NUL(hand_pc[i]); i++)
			;
		hand_pc[i] = pc;
	}
	piece[pc].attr = piece_info[np].attr | B_PA_DEFINED;
	if (promotable == ON) {
		piece[pc].attr |= B_PA_PROMOTABLE;
	}
	piece[pc].move_style = piece_info[np].move_style;
	// graphs
	s = graph_fp[np];
	strcpy(s, piece_info[np].graphv[0]);
	graph[pc] = s;
	b_graph[pc] = s;
	s += strlen(s) + 1;
	strcat(strcpy(s, " "), piece_info[np].graphv[0]);
	b_graph[pc | B_PC_HITHER] = s;
	s += strlen(s) + 1;
	strcpy(s, "v");
	if (np == NP_OU) {
		strcat(s, piece_info[np].graphv[2]);
		graph_yo_gy = piece_info[np].graphv[2];
	} else {
		strcat(s, piece_info[np].graphv[0]);
	}
	b_graph[pc | B_PC_YONDER] = s;
	if (pc == ROOM || pc == ROCK || pc == PIT) {
		b_graph[pc] = b_graph[pc | B_PC_HITHER];
		b_graph[pc | B_PC_HITHER] = NULL;
		b_graph[pc | B_PC_YONDER] = NULL;
	}

	return;
}

static void
setup_normal_pieces(void)
{
	// 将棋
	setup_normal_piece(NP_FU, PC_FU, ON);
	setup_normal_piece(NP_KY, PC_KY, ON);
	setup_normal_piece(NP_KE, PC_KE, ON);
	setup_normal_piece(NP_GI, PC_GI, ON);
	setup_normal_piece(NP_KI, PC_KI, OFF);
	setup_normal_piece(NP_KA, PC_KA, ON);
	setup_normal_piece(NP_HI, PC_HI, ON);
	setup_normal_piece(NP_OU, PC_OU, OFF);
	// 将棋成駒
	setup_normal_piece(NP_TO, PC_TO, OFF);
	setup_normal_piece(NP_NY, PC_NY, OFF);
	setup_normal_piece(NP_NK, PC_NK, OFF);
	setup_normal_piece(NP_NG, PC_NG, OFF);
	setup_normal_piece(NP_UM, PC_UM, OFF);
	setup_normal_piece(NP_RY, PC_RY, OFF);
	// 特殊
	setup_normal_piece(SP_ROOM, ROOM, OFF);
	setup_normal_piece(SP_ROCK, ROCK, OFF);
	setup_normal_piece(SP_PIT, PIT, OFF);

	return;
}

// piece_infoからgraph_fpへ
// graph_fp：20バイトの領域に、駒面、盤駒面（攻方）、盤駒面（受方）を各文字列として格納
static void
setup_fairy_graphs(void)
{
	char *s;
	int fp;

	for (fp = FP_START; fp < FP_END; fp++) {
		if (piece_info[fp].name != NULL) {
			// graphs
			s = graph_fp[fp];
			strcpy(s, piece_info[fp].graphv[0]);
			s += strlen(s) + 1;
			strcat(strcpy(s, " "), piece_info[fp].graphv[0]);
			s += strlen(s) + 1;
			strcat(strcpy(s, "v"), piece_info[fp].graphv[0]);
		}
	}

	return;
}

static void
inc_to_pos_index(int n)
{
	to_pos_index++;
	if (to_pos_index >= UNIT_NUM_OF_TO_POS - n) {
		to_pos = calloc(UNIT_NUM_OF_TO_POS, sizeof(to_pos[0]));
		if (to_pos == NULL) {
			fm_exit_mae("跳び移動先領域");
		}
		to_pos_index = 0;
	}
	to_pos[to_pos_index] = EOP;

	return;
}

static void
inc_to_posv_index(int n)
{
	to_posv_index++;
	if (to_posv_index >= UNIT_NUM_OF_TO_POSV - n) {
		to_posv = calloc(UNIT_NUM_OF_TO_POSV, sizeof(to_posv[0]));
		if (to_posv == NULL) {
			fm_exit_mae("走り移動先領域");
		}
		to_posv_index = 0;
	}
	to_posv[to_posv_index] = NULL;

	return;
}

static uchar_t *
inc_to_board_index(void)
{
	to_board_index++;
	if (to_board_index >= UNIT_NUM_OF_TO_BOARD) {
		to_board = calloc(UNIT_NUM_OF_TO_BOARD, sizeof(to_board[0]));
		if (to_board == NULL) {
			fm_exit_mae("移動先盤面領域");
		}
		to_board_index = 0;
	}
	memset(to_board[to_board_index], OFF, sizeof(to_board[0]));

	return to_board[to_board_index];
}

static void
setup_leap_to_pos_sub(vec_t *leap_vec, char *leap_to_pos[81])
{
	int pos, vn, pf, pr, vf, vr, i;

	for (vn = 1; leap_vec[vn].file != 0 || leap_vec[vn].rank != 0; vn++)
		;
	for (pos = 0; pos < 81; pos++) {
		pf = POS_FILE(pos);
		pr = POS_RANK(pos);
		inc_to_pos_index(vn);
		leap_to_pos[pos] = to_pos + to_pos_index;
		for (i = 0; leap_vec[i].file != 0 || leap_vec[i].rank != 0; i++) {
			vf = leap_vec[i].file;
			vr = leap_vec[i].rank;
			if (pf + vf < 0 || pf + vf > 8 || pr + vr < 0 || pr + vr > 8) {
				continue;
			}
			to_pos[to_pos_index] = pos + VEC(vf, vr);
			inc_to_pos_index(0);
		}
	}

	return;
}

static void
copy_pos(char *from_pos[81], char *to_pos[81])
{
	int pos;

	for (pos = 0; pos < 81; pos++) {
		to_pos[pos] = from_pos[pos];
	}

	return;
}

static void
setup_hi_leap_to_pos(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & BM_MS_LEAP_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_pos(hi_leap_to_pos[apc], hi_leap_to_pos[pc]);
			return;
		}
	}
	setup_leap_to_pos_sub(piece_info[piece[pc].pc_no].hi_leap_vec, hi_leap_to_pos[pc]);

	return;
}

static void
setup_yo_leap_to_pos(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_leap_vec;
	ava = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_pos(hi_leap_to_pos[pc], yo_leap_to_pos[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & BM_MS_LEAP_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_pos(yo_leap_to_pos[apc], yo_leap_to_pos[pc]);
			return;
		}
	}
	setup_leap_to_pos_sub(piece_info[piece[pc].pc_no].yo_leap_vec, yo_leap_to_pos[pc]);

	return;
}

static void
setup_run_to_posv_sub(vec_t *run_vec, char **run_to_posv[81])
{
	int pos, vn, pf, pr, vf, vr, rg, i, j;

	for (vn = 1; run_vec[vn].file != 0 || run_vec[vn].rank != 0; vn++)
		;
	for (pos = 0; pos < 81; pos++) {
		pf = POS_FILE(pos);
		pr = POS_RANK(pos);
		inc_to_posv_index(vn);
		run_to_posv[pos] = to_posv + to_posv_index;
		for (i = 0; run_vec[i].file != 0 || run_vec[i].rank != 0; i++) {
			vf = run_vec[i].file;
			vr = run_vec[i].rank;
			rg = run_vec[i].range == 0 ? 9 : run_vec[i].range;
			for (j = 1; j <= rg && pf + vf * j >= 0 && pf + vf * j <= 8 &&
					pr + vr * j >= 0 && pr + vr * j <= 8; j++)
				;
			if (j >= 2) {
				inc_to_pos_index(j);
				to_posv[to_posv_index] = to_pos + to_pos_index;
				inc_to_posv_index(0);
				for (j = 1; j <= rg && pf + vf * j >= 0 && pf + vf * j <= 8 &&
						pr + vr * j >= 0 && pr + vr * j <= 8; j++) {
					to_pos[to_pos_index] = pos + VEC(vf * j, vr * j);
					inc_to_pos_index(0);
				}
			}
		}
	}

	return;
}

static void
copy_posv(char **from_posv[81], char **to_posv[81])
{
	int pos;

	for (pos = 0; pos < 81; pos++) {
		to_posv[pos] = from_posv[pos];
	}

	return;
}

static void
setup_hi_run_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_run_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & BM_MS_RUN_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(hi_run_to_posv[apc], hi_run_to_posv[pc]);
			return;
		}
	}
	setup_run_to_posv_sub(piece_info[piece[pc].pc_no].hi_run_vec, hi_run_to_posv[pc]);

	return;
}

static void
setup_yo_run_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_run_vec;
	ava = piece_info[piece[pc].pc_no].hi_run_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_posv(hi_run_to_posv[pc], yo_run_to_posv[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & BM_MS_RUN_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(yo_run_to_posv[apc], yo_run_to_posv[pc]);
			return;
		}
	}
	setup_run_to_posv_sub(piece_info[piece[pc].pc_no].yo_run_vec, yo_run_to_posv[pc]);

	return;
}

static void
setup_step_to_posv_sub(vec_t *leap_vec, char **run_to_posv[81])
{
	int pos, vn, pf, pr, vf, vr, dist, i, j;

	for (vn = 1; leap_vec[vn].file != 0 || leap_vec[vn].rank != 0; vn++)
		;
	for (pos = 0; pos < 81; pos++) {
		pf = POS_FILE(pos);
		pr = POS_RANK(pos);
		inc_to_posv_index(vn);
		run_to_posv[pos] = to_posv + to_posv_index;
		for (i = 0; leap_vec[i].file != 0 || leap_vec[i].rank != 0; i++) {
			vf = leap_vec[i].file;
			vr = leap_vec[i].rank;
			dist = leap_vec[i].range;
			if (pf + vf * dist >= 0 && pf + vf * dist <= 8 &&
					pr + vr * dist >= 0 && pr + vr * dist <= 8) {
				inc_to_pos_index(dist + 1);
				to_posv[to_posv_index] = to_pos + to_pos_index;
				inc_to_posv_index(0);
				for (j = 1; j <= dist; j++) {
					to_pos[to_pos_index] = pos + VEC(vf * j, vr * j);
					inc_to_pos_index(0);
				}
			}
		}
	}

	return;
}

static void
setup_hi_step_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & B_MS_STEP)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(hi_run_to_posv[apc], hi_run_to_posv[pc]);
			return;
		}
	}
	setup_step_to_posv_sub(piece_info[piece[pc].pc_no].hi_leap_vec, hi_run_to_posv[pc]);

	return;
}

static void
setup_yo_step_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_leap_vec;
	ava = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_posv(hi_run_to_posv[pc], yo_run_to_posv[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & B_MS_STEP)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(yo_run_to_posv[apc], yo_run_to_posv[pc]);
			return;
		}
	}
	setup_step_to_posv_sub(piece_info[piece[pc].pc_no].yo_leap_vec, yo_run_to_posv[pc]);

	return;
}

static void
setup_leap_to_board_sub(char *leap_to_pos[81], uchar_t *xx_to_board[81])
{
	int pos, to, i;

	for (pos = 0; pos < 81; pos++) {
		xx_to_board[pos] = inc_to_board_index();
		for (i = 0; ; i++) {
			to = leap_to_pos[pos][i];
			if (to == EOP) {
				break;
			}
			xx_to_board[pos][to] = ON;
		}
	}

	return;
}

static void
setup_run_to_board_sub(char **run_to_posv[81], uchar_t *xx_to_board[81])
{
	int pos, to, i, j;

	for (pos = 0; pos < 81; pos++) {
		if (xx_to_board[pos] != NULL) {
			xx_to_board[pos] = xx_to_board[pos];
		} else {
			xx_to_board[pos] = inc_to_board_index();
		}
		for (j = 0; run_to_posv[pos][j] != NULL; j++) {
			for (i = 0; ; i++) {
				to = run_to_posv[pos][j][i];
				if (to == EOP) {
					break;
				}
				xx_to_board[pos][to] = ON;
			}
		}
	}

	return;
}

static void
copy_board(uchar_t *from_board[81], uchar_t *xx_to_board[81])
{
	if (xx_to_board[0] == NULL) {
		int pos;
		for (pos = 0; pos < 81; pos++) {
			xx_to_board[pos] = from_board[pos];
		}
	} else {
		int pos, pos2;
		for (pos = 0; pos < 81; pos++) {
			for (pos2 = 0; pos2 < 81; pos2++) {
				xx_to_board[pos][pos2] |= from_board[pos][pos2];
			}
		}
	}

	return;
}

static void
setup_hi_leap_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) ||
				NOT(piece[apc].move_style & BM_MS_LEAP_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(hi_to_board[apc], hi_to_board[pc]);
			return;
		}
	}
	setup_leap_to_board_sub(hi_leap_to_pos[pc], hi_to_board[pc]);

	return;
}

static void
setup_yo_leap_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_leap_vec;
	ava = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_board(hi_to_board[pc], yo_to_board[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) ||
				NOT(piece[apc].move_style & BM_MS_LEAP_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(yo_to_board[apc], yo_to_board[pc]);
			return;
		}
	}
	setup_leap_to_board_sub(yo_leap_to_pos[pc], yo_to_board[pc]);

	return;
}

static void
setup_hi_run_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_run_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].attr & B_PA_PLACABLE) ||
				NOT(piece[apc].move_style & BM_MS_EXRUN_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(hi_to_board[apc], hi_to_board[pc]);
			return;
		}
	}
	setup_run_to_board_sub(hi_run_to_posv[pc], hi_to_board[pc]);

	return;
}

static void
setup_yo_run_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_run_vec;
	ava = piece_info[piece[pc].pc_no].hi_run_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_board(hi_to_board[pc], yo_to_board[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].attr & B_PA_PLACABLE) ||
				NOT(piece[apc].move_style & BM_MS_EXRUN_FAMILY)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0)
					|| va[i].file != ava[i].file || va[i].rank != ava[i].rank
					|| va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(yo_to_board[apc], yo_to_board[pc]);
			return;
		}
	}
	setup_run_to_board_sub(yo_run_to_posv[pc], yo_to_board[pc]);

	return;
}

static void
setup_hi_step_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].attr & B_PA_PLACABLE) ||
				NOT(piece[apc].move_style & B_MS_STEP)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(hi_to_board[apc], hi_to_board[pc]);
			return;
		}
	}
	setup_run_to_board_sub(hi_run_to_posv[pc], hi_to_board[pc]);

	return;
}

static void
setup_yo_step_to_board(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_leap_vec;
	ava = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_board(hi_to_board[pc], yo_to_board[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].attr & B_PA_PLACABLE) ||
				NOT(piece[apc].move_style & B_MS_STEP)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_leap_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_board(yo_to_board[apc], yo_to_board[pc]);
			return;
		}
	}
	setup_run_to_board_sub(yo_run_to_posv[pc], yo_to_board[pc]);

	return;
}

static void
setup_close_check_board(char *leap_to_pos[81], uchar_t *close_check_board[81])
{
	int pos, to, i;

	if (close_check_board[0] == NULL) {
		for (pos = 0; pos < 81; pos++) {
			close_check_board[pos] = inc_to_board_index();
		}
	}
	for (pos = 0; pos < 81; pos++) {
		for (i = 0; ; i++) {
			to = leap_to_pos[pos][i];
			if (to == EOP) {
				break;
			}
			close_check_board[pos][to] = ON;
		}
	}

	return;
}

static void
setup_remote_check_board(char **run_to_posv[81], uchar_t *remote_check_board[81])
{
	int pos, to, i, j;

	if (remote_check_board[0] == NULL) {
		for (pos = 0; pos < 81; pos++) {
			remote_check_board[pos] = inc_to_board_index();
		}
	}
	for (pos = 0; pos < 81; pos++) {
		for (j = 0; run_to_posv[pos][j] != NULL; j++) {
			for (i = 0; ; i++) {
				to = run_to_posv[pos][j][i];
				if (to == EOP) {
					break;
				}
				remote_check_board[pos][to] = ON;
			}
		}
	}

	return;
}

// Mao, Moaは、Knightと同じ移動先だが、合駒が利く。
//
// hi_to_board: 合駒位置盤面マップ（攻方、受方共用）
// hi_run_to_posv[駒][位置][ベクトルインデクス]: 合駒点位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][合駒点位置]: 利き位置リスト（攻方、受方共用）
// hi_leap_to_pos: 移動先リスト（攻方用）
// yo_leap_to_pos: 移動先リスト（受方用）
static void
setup_mao_step_to_pos(uchar_t pc)
{
	char **run_to_posv;
	int pos, div, to, df, dr, tf, tr, vf, vr, i;

	// 跳々駒系の合駒点位置リスト（攻方、受方共用）を設定
	setup_hi_step_to_posv(pc);
	// 合駒位置盤面マップ（攻方、受方共用）を設定
	setup_hi_run_to_board(pc);
	// 跳々駒系の合駒候補位置用の盤面情報を設定
	setup_remote_check_board(hi_run_to_posv[pc], hi_remote_check_board);
	setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	// 跳々駒系の移動先リスト（攻方用）を作成
	for (pos = 0; pos < 81; pos++) {
		inc_to_posv_index(81);
		yo_run_to_posv[pc][pos] = to_posv + to_posv_index;
		for (div = 0; div < 81; div++) {
			inc_to_pos_index(2);
			to_posv[to_posv_index] = to_pos + to_pos_index;
			inc_to_posv_index(0);
			if (hi_to_board[pc][pos][div] == ON) {
				// Mao, Moaの移動先２箇所or１箇所
				df = POS19_FILE(div);
				dr = POS19_RANK(div);
				vf = df - POS19_FILE(pos);
				vr = dr - POS19_RANK(pos);
				if (vf == 0) {
					tf = df - 1;
					tr = dr + (vr > 0 ? 1 : -1);
					if (tf >= 1 && tf <= 9 && tr >= 1 && tr <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
					tf = df + 1;
					tr = dr + (vr > 0 ? 1 : -1);
					if (tf >= 1 && tf <= 9 && tr >= 1 && tr <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
				} else if (vr == 0) {
					tf = df + (vf > 0 ? 1 : -1);
					tr = dr - 1;
					if (tf >= 1 && tf <= 9 && tr >= 1 && tr <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
					tf = df + (vf > 0 ? 1 : -1);
					tr = dr + 1;
					if (tf >= 1 && tf <= 9 && tr >= 1 && tr <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
				} else {
					tf = df + (vf > 0 ? 1 : -1);
					tr = dr;
					if (tf >= 1 && tf <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
					tf = df;
					tr = dr + (vr > 0 ? 1 : -1);
					if (tr >= 1 && tr <= 9) {
						to_pos[to_pos_index] = POS19(tf, tr);
						inc_to_pos_index(0);
					}
				}
			}
		}
	}
	// 移動先リスト（攻方用）を作成
	for (pos = 0; pos < 81; pos++) {
		inc_to_pos_index(8);
		hi_leap_to_pos[pc][pos] = to_pos + to_pos_index;
		run_to_posv = yo_run_to_posv[pc][pos];
		for (to = 0; to < 81; to++) {
			if (hi_to_board[pc][pos][to] == ON) {
				for (i = 0; run_to_posv[to][i] != EOP; i++) {
					to_pos[to_pos_index] = run_to_posv[to][i];
					inc_to_pos_index(0);
				}
			}
		}
	}
	// 移動先リスト（受方用）を作成
	for (pos = 0; pos < 81; pos++) {
		inc_to_pos_index(8);
		yo_leap_to_pos[pc][pos] = to_pos + to_pos_index;
		run_to_posv = yo_run_to_posv[pc][pos];
		for (to = 0; to < 81; to++) {
			if (hi_to_board[pc][pos][to] == ON) {
				for (i = 0; run_to_posv[to][i] != EOP; i++) {
					to_pos[to_pos_index] = run_to_posv[to][i];
					inc_to_pos_index(0);
				}
			}
		}
	}
	// 跳び駒系の移動先盤面情報を設定
	setup_close_check_board(yo_leap_to_pos[pc], hi_close_check_board);
	setup_close_check_board(hi_leap_to_pos[pc], yo_close_check_board);
#if 0
{
	int i;
	pos = POS19(7, 8);
	printf("■位置 %d\n", PR_POS(pos));
	printf("■合駒位置リスト\n");
	for (i = 0; hi_run_to_posv[pc][pos][i] != NULL; i++) {
		printf("%d ", PR_POS(hi_run_to_posv[pc][pos][i][0]));
	}
	printf("\n");
	printf("■利き位置リスト\n");
	for (i = 0; hi_leap_to_pos[pc][pos][i] != EOP; i++) {
		printf("%d ", PR_POS(hi_leap_to_pos[pc][pos][i]));
	}
	printf("\n");
	for (i = 0; hi_run_to_posv[pc][pos][i] != NULL; i++) {
		if (yo_run_to_posv[pc][pos][hi_run_to_posv[pc][pos][i][0]][0] != EOP) {
			printf("%d ", PR_POS(yo_run_to_posv[pc][pos][hi_run_to_posv[pc][pos][i][0]][0]));
		}
		if (yo_run_to_posv[pc][pos][hi_run_to_posv[pc][pos][i][0]][1] != EOP) {
			printf("%d ", PR_POS(yo_run_to_posv[pc][pos][hi_run_to_posv[pc][pos][i][0]][1]));
		}
	}
	printf("\n");
	exit(0);
}
#endif

	return;
}

// Sparrowは、反射駒までＱ利きで進み、そこから進行方向に135°の角度で１枡反射する。
//
// 縦横の場合：  . . . . . . . .
//  @: 反射駒    . . . @ . . . .
//  *: 利き位置  . * * | * . * .
//               @ - * # * - - @
//               . * . @ . . * .
//               . . . . . . . .
//               . . . . . . . .
//
// 斜めの場合：  . . . . . * @ .
//  @: 反射駒    . . . . . / * .
//  *: 利き位置  . . @ * / . . .
//               . . * # . . . .
//               . * / . . . . .
//               . @ * . . . . .
//               . . . . . . . .
//
// hi_run_to_posv[駒][位置][ベクトルインデクス][位置リスト]: 反射駒位置リスト（攻方、受方共用）
// yo_run_to_posv[駒][位置][反射駒位置][利き位置リスト]: 利き位置リスト（攻方、受方共用）
// hi_to_board: 反射駒利き位置盤面マップ（攻方、受方共用）
static void
setup_refl135_to_pos(uchar_t pc)
{
	int pos, to;

	// 反射駒までの位置リスト（攻方、受方共用）を設定
	setup_hi_run_to_posv(pc);
	// 反射駒位置盤面情報（攻方、受方共用）を設定
	setup_hi_run_to_board(pc);
	// 合駒候補位置用の盤面情報を設定
	setup_remote_check_board(hi_run_to_posv[pc], hi_remote_check_board);
	setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	// 利き位置リスト（攻方、受方共用）を作成
	for (pos = 0; pos < 81; pos++) {
		inc_to_posv_index(81);
		yo_run_to_posv[pc][pos] = to_posv + to_posv_index;
		for (to = 0; to < 81; to++) {
			inc_to_pos_index(2);
			to_posv[to_posv_index] = to_pos + to_pos_index;
			inc_to_posv_index(0);
			if (hi_to_board[pc][pos][to] == ON) {
				// Sparrowの移動先２箇所or１箇所
				int tf, tr, vf, vr;
				tf = POS19_FILE(to);
				tr = POS19_RANK(to);
				vf = tf - POS19_FILE(pos);
				vr = tr - POS19_RANK(pos);
				if (vf == 0) {
					if (tf == 1) {
						to_pos[to_pos_index] = POS19(2, tr + (vr > 0 ? -1 : 1));
						inc_to_pos_index(0);
					} else if (tf == 9) {
						to_pos[to_pos_index] = POS19(8, tr + (vr > 0 ? -1 : 1));
						inc_to_pos_index(0);
					} else {
						to_pos[to_pos_index] = POS19(tf - 1, tr + (vr > 0 ? -1 : 1));
						inc_to_pos_index(0);
						to_pos[to_pos_index] = POS19(tf + 1, tr + (vr > 0 ? -1 : 1));
						inc_to_pos_index(0);
					}
				} else if (vr == 0) {
					if (tr == 1) {
						to_pos[to_pos_index] = POS19(tf + (vf > 0 ? -1 : 1), 2);
						inc_to_pos_index(0);
					} else if (tr == 9) {
						to_pos[to_pos_index] = POS19(tf + (vf > 0 ? -1 : 1), 8);
						inc_to_pos_index(0);
					} else {
						to_pos[to_pos_index] = POS19(tf + (vf > 0 ? -1 : 1), tr - 1);
						inc_to_pos_index(0);
						to_pos[to_pos_index] = POS19(tf + (vf > 0 ? -1 : 1), tr + 1);
						inc_to_pos_index(0);
					}
				} else {
					to_pos[to_pos_index] = POS19(tf + (vf > 0 ? -1 : 1), tr);
					inc_to_pos_index(0);
					to_pos[to_pos_index] = POS19(tf, tr + (vr > 0 ? -1 : 1));
					inc_to_pos_index(0);
				}
			}
		}
	}
#if 0
{
	int i, j;
	pos = POS19(7, 8);
	printf("\n");
	printf("■反射駒利き位置リスト\n");
	for (i = 0; i < 24 && hi_run_to_posv[pc][pos][i] != NULL; i++) {
		for (j = 0; j < 9 && hi_run_to_posv[pc][pos][i][j] != EOP; j++) {
			printf("%d ", PR_POS(hi_run_to_posv[pc][pos][i][j]));
		}
		printf("\n");
	}
	printf("■利き位置リスト\n");
	for (i = 0; i < 81; i++) {
		if (yo_run_to_posv[pc][pos][i][0] != EOP) {
			printf("%d: ", PR_POS(i));
			for (j = 0; j < 3 && yo_run_to_posv[pc][pos][i][j] != EOP; j++) {
				printf("%d ", PR_POS(yo_run_to_posv[pc][pos][i][j]));
			}
			printf("\n");
		}
	}
//	exit(0);
}
#endif

	return;
}

static void
setup_refl90_to_pos(uchar_t pc)
{
	int pos, to;

	// 走り駒系の移動先（攻方、受方共用）を設定
	setup_hi_run_to_posv(pc);
	// 走り駒系の移動先盤面情報（攻方、受方共用）を設定
	setup_hi_run_to_board(pc);
	// 合駒位置チェック用の走り駒系の移動先盤面情報を設定
	setup_remote_check_board(hi_run_to_posv[pc], hi_remote_check_board);
	setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	// 利き位置リスト（攻方、受方共用）を作成
	for (pos = 0; pos < 81; pos++) {
		inc_to_posv_index(81);
		yo_run_to_posv[pc][pos] = to_posv + to_posv_index;
		for (to = 0; to < 81; to++) {
			inc_to_pos_index(2);
			to_posv[to_posv_index] = to_pos + to_pos_index;
			inc_to_posv_index(0);
			if (hi_to_board[pc][pos][to] == ON) {
				// Eagleの移動先２箇所or１箇所
				int tf, tr, vf, vr;
				tf = POS19_FILE(to);
				tr = POS19_RANK(to);
				vf = tf - POS19_FILE(pos);
				vr = tr - POS19_RANK(pos);
				if (vf == 0) {
					if (tf == 1) {
						to_pos[to_pos_index] = POS19(2, tr);
						inc_to_pos_index(0);
					} else if (POS19_FILE(pos) == 9) {
						to_pos[to_pos_index] = POS19(8, tr);
						inc_to_pos_index(0);
					} else {
						to_pos[to_pos_index] = POS19(tf - 1, tr);
						inc_to_pos_index(0);
						to_pos[to_pos_index] = POS19(tf + 1, tr);
						inc_to_pos_index(0);
					}
				} else if (vr == 0) {
					if (tr == 1) {
						to_pos[to_pos_index] = POS19(tf, 2);
						inc_to_pos_index(0);
					} else if (tr == 9) {
						to_pos[to_pos_index] = POS19(tf, 8);
						inc_to_pos_index(0);
					} else {
						to_pos[to_pos_index] = POS19(tf, tr - 1);
						inc_to_pos_index(0);
						to_pos[to_pos_index] = POS19(tf, tr + 1);
						inc_to_pos_index(0);
					}
				} else {
					if ((vf > 0 && vr > 0) || (vf < 0 && vr < 0)) {
						if (tf - 1 >= 1 && tr + 1 <= 9) {
							to_pos[to_pos_index] = POS19(tf - 1, tr + 1);
							inc_to_pos_index(0);
						}
						if (tf + 1 <= 9 && tr - 1 >= 1) {
							to_pos[to_pos_index] = POS19(tf + 1, tr - 1);
							inc_to_pos_index(0);
						}
					} else {
						if (tf - 1 >= 1 && tr - 1 >= 1) {
							to_pos[to_pos_index] = POS19(tf - 1, tr - 1);
							inc_to_pos_index(0);
						}
						if (tf + 1 <= 9 && tr + 1 <= 9) {
							to_pos[to_pos_index] = POS19(tf + 1, tr + 1);
							inc_to_pos_index(0);
						}
					}
				}
			}
		}
	}
#if 0
{
	int i, j;
	pos = POS19(4, 2);
	printf("\n");
	printf("■反射駒利き位置リスト\n");
	for (i = 0; i < 24 && hi_run_to_posv[pc][pos][i] != NULL; i++) {
		for (j = 0; j < 9 && hi_run_to_posv[pc][pos][i][j] != EOP; j++) {
			printf("%d ", PR_POS(hi_run_to_posv[pc][pos][i][j]));
		}
		printf("\n");
	}
	printf("■利き位置リスト\n");
	for (i = 0; i < 81; i++) {
		if (yo_run_to_posv[pc][pos][i][0] != EOP) {
			printf("%d: ", PR_POS(i));
			for (j = 0; j < 3 && yo_run_to_posv[pc][pos][i][j] != EOP; j++) {
				printf("%d ", PR_POS(yo_run_to_posv[pc][pos][i][j]));
			}
			printf("\n");
		}
	}
//	exit(0);
}
#endif

	return;
}

// hi_leap_to_pos: 対称点に対する利き位置
// yo_leap_to_pos: 利き位置に対する対称点
// hi_run_to_posv[0]: 利き位置リスト
// hi_run_to_posv[1]: 合駒の利く利き位置リスト
// hi_run_to_posv[2...]: 合駒の利く利き位置に対する合駒位置リスト
// yo_run_to_posv[0]: 対称点リスト
// yo_run_to_posv[1]: 合駒の利く対称点リスト
// hi_to_board: 対称点があれば利く位置
// yo_to_board: 利く位置がある対称点
static void
setup_equi_hop_to_pos(uchar_t pc)
{
	char *equi_hop_to_posv;
	int pos;

	for (pos = 0; pos < 81; pos++) {
		int point, n, n2, i;
		hi_leap_to_pos[pc][pos] = inc_to_board_index();
		yo_leap_to_pos[pc][pos] = inc_to_board_index();
		for (point = 0; point < 81; point++) {
			hi_leap_to_pos[pc][pos][point] = NOP;
			yo_leap_to_pos[pc][pos][point] = NOP;
		}
		hi_to_board[pc][pos] = inc_to_board_index();
		yo_to_board[pc][pos] = inc_to_board_index();
		n = 1;
		for (point = 0; point < 81; point++) {
			int vf, vr, pf, pr;
			if (point == pos) {
				continue;
			}
			n++;
			pf = POS19_FILE(point);
			pr = POS19_RANK(point);
			vf = pf - POS19_FILE(pos);
			vr = pr - POS19_RANK(pos);
			if (pf + vf <= 9 && pf + vf >= 1 && pr + vr <= 9 && pr + vr >= 1) {
				hi_leap_to_pos[pc][pos][point] = POS19(pf + vf, pr + vr);
				yo_leap_to_pos[pc][pos][POS19(pf + vf, pr + vr)] = point;
				hi_to_board[pc][pos][POS19(pf + vf, pr + vr)] = ON;
				yo_to_board[pc][pos][point] = ON;
			}
		}
		n2 = 1;
		for (point = 0; point < 81; point++) {
			int vf, vr;
			if (hi_leap_to_pos[pc][pos][point] != NOP) {
				vf = abs(POS_FILE(point) - POS_FILE(pos));
				vr = abs(POS_RANK(point) - POS_RANK(pos));
				if ((vf == 0 && vr >= 2) || (vf >= 2 && vr == 0) || (vf == vr && vf >= 2) ||
						(vf == 2 && vr == 4) || (vf == 4 && vr == 2)) {
					n2++;
				}
			}
		}
		inc_to_posv_index(2 + n2 - 1);
		hi_run_to_posv[pc][pos] = to_posv + to_posv_index;
		inc_to_pos_index(n);
		to_posv[to_posv_index] = to_pos + to_pos_index;
		inc_to_posv_index(0);
		for (point = 0; point < 81; point++) {
			if (hi_leap_to_pos[pc][pos][point] != NOP) {
				to_pos[to_pos_index] = hi_leap_to_pos[pc][pos][point];
				inc_to_pos_index(0);
			}
		}
		inc_to_pos_index(n2);
		to_posv[to_posv_index] = to_pos + to_pos_index;
		inc_to_posv_index(0);
		for (point = 0; point < 81; point++) {
			int vf, vr;
			if (hi_leap_to_pos[pc][pos][point] != NOP) {
				vf = abs(POS_FILE(point) - POS_FILE(pos));
				vr = abs(POS_RANK(point) - POS_RANK(pos));
				if ((vf == 0 && vr >= 2) || (vf >= 2 && vr == 0) || (vf == vr && vf >= 2) ||
						(vf == 2 && vr == 4) || (vf == 4 && vr == 2)) {
					to_pos[to_pos_index] = hi_leap_to_pos[pc][pos][point];
					inc_to_pos_index(0);
				}
			}
		}
		equi_hop_to_posv = hi_run_to_posv[pc][pos][1];
		for (i = 0; equi_hop_to_posv[i] != EOP; i++) {
			int tvf, tvr, pvf, pvr, vf, vr, n3 = 1, j;
			tvf = POS_FILE(equi_hop_to_posv[i]) - POS_FILE(pos);
			tvr = POS_RANK(equi_hop_to_posv[i]) - POS_RANK(pos);
			pvf = tvf / 2;
			pvr = tvr / 2;
			if (pvf == 0 || pvr == 0 || abs(pvf) == abs(pvr)) {
				vf = pvf == 0 ? 0 : pvf / abs(pvf);
				vr = pvr == 0 ? 0 : pvr / abs(pvr);
				for (j = 1; vf * j != tvf || vr * j != tvr; j++) {
					if (vf * j != pvf || vr * j != pvr) {
						n3++;
					}
				}
			} else {
				n3 = 3;
			}
			inc_to_pos_index(n3);
			to_posv[to_posv_index] = to_pos + to_pos_index;
			inc_to_posv_index(0);
			if (pvf == 0 || pvr == 0 || abs(pvf) == abs(pvr)) {
				vf = pvf == 0 ? 0 : pvf / abs(pvf);
				vr = pvr == 0 ? 0 : pvr / abs(pvr);
				for (j = 1; vf * j != tvf || vr * j != tvr; j++) {
					if (vf * j != pvf || vr * j != pvr) {
						to_pos[to_pos_index] = pos + VEC(vf * j, vr * j);
						inc_to_pos_index(0);
					}
				}
			} else {
				vf = pvf / 2;
				vr = pvr / 2;
				to_pos[to_pos_index] = pos + VEC(vf, vr);
				inc_to_pos_index(0);
				to_pos[to_pos_index] = pos + VEC(pvf + vf, pvr + vr);
				inc_to_pos_index(0);
			}
		}
		inc_to_posv_index(2);
		yo_run_to_posv[pc][pos] = to_posv + to_posv_index;
		inc_to_pos_index(n);
		to_posv[to_posv_index] = to_pos + to_pos_index;
		inc_to_posv_index(0);
		for (point = 0; point < 81; point++) {
			if (hi_leap_to_pos[pc][pos][point] != NOP) {
				to_pos[to_pos_index] = point;
				inc_to_pos_index(0);
			}
		}
		inc_to_pos_index(n);
		to_posv[to_posv_index] = to_pos + to_pos_index;
		inc_to_posv_index(0);
		for (point = 0; point < 81; point++) {
			int vf, vr;
			if (hi_leap_to_pos[pc][pos][point] != NOP) {
				vf = abs(POS_FILE(point) - POS_FILE(pos));
				vr = abs(POS_RANK(point) - POS_RANK(pos));
				if ((vf == 0 && vr >= 2) || (vf >= 2 && vr == 0) || (vf == vr && vf >= 2) ||
						(vf == 2 && vr == 4) || (vf == 4 && vr == 2)) {
					to_pos[to_pos_index] = point;
					inc_to_pos_index(0);
				}
			}
		}
	}
#if 0
{
	int i, j;
	pos = POS19(5, 5);
	printf("■対称点に対する利き位置\n");
	for (i = 0; i < 81; i++) {
		if (hi_leap_to_pos[pc][pos][i] != NOP) {
			printf("%d/%d ", PR_POS(i), PR_POS(hi_leap_to_pos[pc][pos][i]));
		}
	}
	printf("\n");
	printf("■利き位置に対する対称点\n");
	for (i = 0; i < 81; i++) {
		if (yo_leap_to_pos[pc][pos][i] != NOP) {
			printf("%d/%d ", PR_POS(i), PR_POS(yo_leap_to_pos[pc][pos][i]));
		}
	}
	printf("\n");
	printf("■利き位置リスト\n");
	for (i = 0; i < 24 && hi_run_to_posv[pc][pos][0][i] != EOP; i++) {
		printf("%d ", PR_POS(hi_run_to_posv[pc][pos][0][i]));
	}
	printf("\n");
	printf("■合駒の利く利き位置リスト\n");
	for (i = 0; i < 24 && hi_run_to_posv[pc][pos][1][i] != EOP; i++) {
		printf("%d ", PR_POS(hi_run_to_posv[pc][pos][1][i]));
	}
	printf("\n");
	printf("■合駒の利く利き位置に対する合駒位置リスト\n");
	for (i = 0; i < 24 && hi_run_to_posv[pc][pos][1][i] != EOP; i++) {
		printf("%d: ", PR_POS(hi_run_to_posv[pc][pos][1][i]));
		for (j = 0; hi_run_to_posv[pc][pos][i + 2][j] != EOP; j++) {
			printf("%d ", PR_POS(hi_run_to_posv[pc][pos][i + 2][j]));
		}
		printf("\n");
	}
	printf("■対称点リスト\n");
	for (i = 0; i < 24 && yo_run_to_posv[pc][pos][0][i] != EOP; i++) {
		printf("%d ", PR_POS(yo_run_to_posv[pc][pos][0][i]));
	}
	printf("\n");
	printf("■合駒の利く対称点リスト\n");
	for (i = 0; i < 24 && yo_run_to_posv[pc][pos][1][i] != EOP; i++) {
		printf("%d ", PR_POS(yo_run_to_posv[pc][pos][1][i]));
	}
	printf("\n");
}
#endif

	return;
}

static void
setup_circle_run_to_posv_sub(vec_t *run_vec, char **run_to_posv[81])
{
	int pos, vn, pf, pr, i, j, k;
	char work[9];

	for (vn = 1; run_vec[vn].file != 0 || run_vec[vn].rank != 0; vn++)
		;
	for (pos = 0; pos < 81; pos++) {
		inc_to_posv_index(vn);
		run_to_posv[pos] = to_posv + to_posv_index;
		for (i = 0; run_vec[i].file != 0 || run_vec[i].rank != 0; i++) {
			pf = POS19_FILE(pos);
			pr = POS19_RANK(pos);
			for (j = 1; j <= run_vec[i].range; j++) {
				pf += run_vec[(i + j - 1) % 8].file;
				pr += run_vec[(i + j - 1) % 8].rank;
				if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
					break;
				}
			}
			if (j >= 2) {
				memset(work, 0x00, sizeof(work));
				pf = POS19_FILE(pos);
				pr = POS19_RANK(pos);
				for (j = 1; j <= run_vec[i].range; j++) {
					pf += run_vec[(i + j - 1) % 8].file;
					pr += run_vec[(i + j - 1) % 8].rank;
					if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
						break;
					}
					work[j - 1] = POS19(pf, pr);
				}
				work[j - 1] = NOP;
				for (k = 0; run_to_posv[pos][k] != NULL; k++) {
					if (memcmp(run_to_posv[pos][k], work, j) == 0) {
						goto ccw_run;
					}
				}
				inc_to_pos_index(j);
				to_posv[to_posv_index] = to_pos + to_pos_index;
				inc_to_posv_index(0);
				pf = POS19_FILE(pos);
				pr = POS19_RANK(pos);
				for (j = 1; j <= run_vec[i].range; j++) {
					pf += run_vec[(i + j - 1) % 8].file;
					pr += run_vec[(i + j - 1) % 8].rank;
					if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
						break;
					}
					to_pos[to_pos_index] = POS19(pf, pr);
					inc_to_pos_index(0);
				}
			}
		ccw_run:;
			pf = POS19_FILE(pos);
			pr = POS19_RANK(pos);
			for (j = 1; j <= run_vec[i].range; j++) {
				pf += run_vec[(i - j + 9) % 8].file;
				pr += run_vec[(i - j + 9) % 8].rank;
				if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
					break;
				}
			}
			if (j >= 2) {
				memset(work, 0x00, sizeof(work));
				pf = POS19_FILE(pos);
				pr = POS19_RANK(pos);
				for (j = 1; j <= run_vec[i].range; j++) {
					pf += run_vec[(i - j + 9) % 8].file;
					pr += run_vec[(i - j + 9) % 8].rank;
					if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
						break;
					}
					work[j - 1] = POS19(pf, pr);
				}
				work[j - 1] = NOP;
				for (k = 0; run_to_posv[pos][k] != NULL; k++) {
					if (memcmp(run_to_posv[pos][k], work, j) == 0) {
						goto next_run;
					}
				}
				inc_to_pos_index(j);
				to_posv[to_posv_index] = to_pos + to_pos_index;
				inc_to_posv_index(0);
				pf = POS19_FILE(pos);
				pr = POS19_RANK(pos);
				for (j = 1; j <= run_vec[i].range; j++) {
					pf += run_vec[(i - j + 9) % 8].file;
					pr += run_vec[(i - j + 9) % 8].rank;
					if (pf < 1 || pf > 9 || pr < 1 || pr > 9) {
						break;
					}
					to_pos[to_pos_index] = POS19(pf, pr);
					inc_to_pos_index(0);
				}
			}
		next_run:;
		}
	}

	return;
}

static void
setup_hi_circle_run_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].hi_run_vec;
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & B_MS_CIRCLE_RUN)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].hi_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(hi_run_to_posv[apc], hi_run_to_posv[pc]);
			return;
		}
	}
	setup_circle_run_to_posv_sub(piece_info[piece[pc].pc_no].hi_run_vec, hi_run_to_posv[pc]);
#if 0
{
	int pos, i, j;
	pos = POS19(7, 7);
	printf("■移動先リスト\n");
	for (i = 0; hi_run_to_posv[pc][pos][i] != NULL; i++) {
		printf("%d: ", i);
		for (j = 0; hi_run_to_posv[pc][pos][i][j] != EOP; j++) {
			printf("%d ", PR_POS(hi_run_to_posv[pc][pos][i][j]));
		}
		printf("\n");
	}
	printf("\n");
}
#endif

	return;
}

static void
setup_yo_circle_run_to_posv(uchar_t pc)
{
	vec_t *va, *ava;
	int i;
	uchar_t apc;

	va = piece_info[piece[pc].pc_no].yo_run_vec;
	ava = piece_info[piece[pc].pc_no].hi_run_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			copy_posv(hi_run_to_posv[pc], yo_run_to_posv[pc]);
			return;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank) {
			break;
		}
	}
	for (apc = PC_FU; apc < pc; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED) || NOT(piece[apc].move_style & B_MS_CIRCLE_RUN)) {
			continue;
		}
		ava = piece_info[piece[apc].pc_no].yo_run_vec;
		for (i = 0; ; i++) {
			if ((va[i].file == 0 && va[i].rank == 0) || (ava[i].file == 0 && ava[i].rank == 0) ||
					va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				break;
			}
		}
		if (va[i].file == 0 && va[i].rank == 0 && ava[i].file == 0 && ava[i].rank == 0) {
			copy_posv(yo_run_to_posv[apc], yo_run_to_posv[pc]);
			return;
		}
	}
	setup_circle_run_to_posv_sub(piece_info[piece[pc].pc_no].yo_run_vec, yo_run_to_posv[pc]);

	return;
}

static int
setup_valid_sq(char *leap_to_pos[81], char **run_to_posv[81], uchar_t **valid_sq)
{
	int pos, invalid;

	invalid = 0;
	for (pos = 0; pos < 81; pos++) {
		if ((leap_to_pos[pos] == NULL || leap_to_pos[pos][0] == EOP) &&
				(run_to_posv[pos] == NULL || run_to_posv[pos][0] == NULL ||
					run_to_posv[pos][0][0] == EOP)) {
			invalid = -1;
			break;
		}
	}
	if (invalid) {
		valid_sq[0] = calloc(81, sizeof(uchar_t));
		if (valid_sq[0] == NULL) {
			fm_exit_mae("行き処のない駒領域");
		}
		memset(valid_sq[0], OFF, 81);
		for (pos = 0; pos < 81; pos++) {
			if ((leap_to_pos[pos] != NULL && leap_to_pos[pos][0] != EOP) ||
					(run_to_posv[pos] != NULL && run_to_posv[pos][0] != NULL &&
						run_to_posv[pos][0][0] != EOP)) {
				valid_sq[0][pos] = ON;
			}
		}
	}

	return invalid;
}

static void
setup_hi_valid_sq(uchar_t pc)
{
	if (setup_valid_sq(hi_leap_to_pos[pc], hi_run_to_posv[pc], &hi_valid_sq[pc]) == -1) {
		if (piece_info[piece[pc].pc_no].hi_leap_vec[0].file != 0 ||
				piece_info[piece[pc].pc_no].hi_leap_vec[0].rank != 0 ||
				piece_info[piece[pc].pc_no].hi_run_vec[0].file != 0 ||
				piece_info[piece[pc].pc_no].hi_run_vec[0].rank != 0) {
			piece[pc].attr |= B_PA_CONSTRAINT_SQ;
		}
	}

	return;
}

static void
setup_yo_valid_sq(uchar_t pc)
{
	if (setup_valid_sq(yo_leap_to_pos[pc], yo_run_to_posv[pc], &yo_valid_sq[pc]) == -1) {
		int pc_no = piece[pc].pc_no;
		if (piece_info[pc_no].yo_leap_vec[0].file != 0 ||
				piece_info[pc_no].yo_leap_vec[0].rank != 0 ||
				piece_info[pc_no].yo_run_vec[0].file != 0 ||
				piece_info[pc_no].yo_run_vec[0].rank != 0) {
			piece[pc].attr |= B_PA_CONSTRAINT_SQ;
		}
	}

	return;
}

static void
setup_pos_order(void)
{
	int pos, pf, pr, qf, qr, lvi, rvi, i, j, k, n;
	uchar_t pc;
	vec_t leap_vec[128], run_vec[128];
	uchar_t work[81];

	lvi = 0;
	rvi = 0;
	for (pc = PC_FU; pc < PC_END; pc++) {
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PIECE)) {
			piece_info_t *pip = &piece_info[piece[pc].pc_no];
			for (i = 0; pip->yo_leap_vec[i].file != 0 || pip->yo_leap_vec[i].rank != 0; i++) {
				for (j = 0; j < lvi; j++) {
					if (leap_vec[j].file == pip->yo_leap_vec[i].file &&
							leap_vec[j].rank == pip->yo_leap_vec[i].rank) {
						break;
					}
				}
				if (j >= lvi) {
					leap_vec[lvi].file = pip->yo_leap_vec[i].file;
					leap_vec[lvi].rank = pip->yo_leap_vec[i].rank;
					lvi++;
				}
			}
			leap_vec[lvi].file = 0;
			leap_vec[lvi].rank = 0;
			for (i = 0; pip->yo_run_vec[i].file != 0 || pip->yo_run_vec[i].rank != 0; i++) {
				for (j = 0; j < rvi; j++) {
					if (run_vec[j].file == pip->yo_run_vec[i].file &&
							run_vec[j].rank == pip->yo_run_vec[i].rank) {
						break;
					}
				}
				if (j >= rvi) {
					run_vec[rvi].file = pip->yo_run_vec[i].file;
					run_vec[rvi].rank = pip->yo_run_vec[i].rank;
					rvi++;
				}
			}
			run_vec[rvi].file = 0;
			run_vec[rvi].rank = 0;
		}
	}
//	for (i = 0; i < rvi; i++) {
//		printf("[%d,%d], ", run_vec[i].file, run_vec[i].rank);
//	}
//	printf("\n");
	for (pos = 0; pos < 81; pos++) {
		memset(pos_order[pos], 0, 81);
		pos_order[pos][pos] = 1;
		n = 2;
		pf = POS_FILE(pos);
		pr = POS_RANK(pos);
		// 玉の８近傍
		for (i = 1; i >= -1; i--) {
			qr = pr + i;
			for (j = -1; j <= 1; j++) {
				qf = pf + j;
				if (qf >= 0 && qf < 9 && qr >= 0 && qr < 9 && pos_order[pos][POS(qf, qr)] == 0) {
					pos_order[pos][POS(qf, qr)] = n;
					n++;
				}
			}
		}
		// 大跳び系の利き、走り駒系の利き筋
		for (k = 2; k <= 8; k++) {
			for (i = 0; i < lvi; i++) {
				if ((abs(leap_vec[i].file) == k && abs(leap_vec[i].rank) <= k) ||
						(abs(leap_vec[i].file) <= k && abs(leap_vec[i].rank) == k)) {
					qf = pf + leap_vec[i].file;
					qr = pr + leap_vec[i].rank;
					if (qf >= 0 && qf < 9 && qr >= 0 && qr < 9 &&
							pos_order[pos][POS(qf, qr)] == 0) {
						pos_order[pos][POS(qf, qr)] = n;
						n++;
					}
				}
			}
			for (i = 0; i < rvi; i++) {
				if (abs(run_vec[i].file) <= k && abs(run_vec[i].rank) <= k) {
					for (j = 1; j <= 8; j++) {
						if ((abs(run_vec[i].file) * j == k && abs(run_vec[i].rank) * j <= k) ||
								(abs(run_vec[i].file) * j <= k && abs(run_vec[i].rank) * j == k)) {
							qf = pf + run_vec[i].file * j;
							qr = pr + run_vec[i].rank * j;
							if (qf >= 0 && qf < 9 && qr >= 0 && qr < 9 &&
									pos_order[pos][POS(qf, qr)] == 0) {
								pos_order[pos][POS(qf, qr)] = n;
								n++;
							}
						}
					}
				}
			}
		}
		// その他
		for (k = 2; k <= 8; k++) {
			for (i = k; i >= k * -1; i--) {
				qr = pr + i;
				for (j = k * -1; j <= k; j++) {
					qf = pf + j;
					if (qf >= 0 && qf < 9 && qr >= 0 && qr < 9 &&
							pos_order[pos][POS(qf, qr)] == 0) {
						pos_order[pos][POS(qf, qr)] = n;
						n++;
					}
				}
			}
		}
	}
	for (pos = 0; pos < 81; pos++) {
		memcpy(work, pos_order[pos], 81);
		for (i = 0; i < 81; i++) {
			pos_order[pos][work[i] - 1] = i;
		}
	}
#if 0
if (ON) {
	int pr, pf;
	uchar_t sq;
	printf("+---------------------------+\n");
	for (pr = 1; pr <= 9; pr++) {
		printf("|");
		for (pf = 9; pf >= 1; pf--) {
			sq = pos_order[20][POS19(pf, pr)];
			printf(" %02d", PR_POS(sq));
		}
		printf("|%s\n", kanji_num[pr - 1]);
	}
	printf("+---------------------------+\n");
	exit(0);
}
#endif

	return;
}

void
setup_piece_attrs(void)
{
	int i;
	uchar_t pc;

	// 初期化
	for (pc = 0; pc < ARRAY_SIZE_OF(graph); pc++) {
		graph[pc] = NULL;
	}
	for (i = 0; i < ARRAY_SIZE_OF(hand_pc); i++) {
		hand_pc[i] = NUL;
	}

	// 通常駒情報、駒面を設定
	setup_normal_pieces();

	// フェアリー駒面を設定
	setup_fairy_graphs();

	// 移動先情報、移動先盤面情報
	for (pc = PC_FU; pc < PC_END; pc++) {
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PIECE)) {
			if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
				// 跳び駒の移動先を設定
				setup_hi_leap_to_pos(pc);
				setup_yo_leap_to_pos(pc);
				// 跳び駒の移動先盤面情報を設定
				setup_hi_leap_to_board(pc);
				setup_yo_leap_to_board(pc);
				// 跳び駒系の移動先盤面情報を設定
				setup_close_check_board(yo_leap_to_pos[pc], hi_close_check_board);
				setup_close_check_board(hi_leap_to_pos[pc], yo_close_check_board);
			} else if (piece[pc].move_style & B_MS_STEP) {
				// 跳々駒の移動先を設定
				setup_hi_step_to_posv(pc);	// setup_hi_run_to_posv() と同様の処理
				setup_yo_step_to_posv(pc);	// setup_yo_run_to_posv() と同様の処理
				// 跳々駒の移動先盤面情報を設定
				setup_hi_step_to_board(pc);	// setup_hi_run_to_board() と同様の処理
				setup_yo_step_to_board(pc);	// setup_yo_run_to_board() と同様の処理
				// 合駒位置チェック用の跳々駒の移動先盤面情報を設定
				setup_remote_check_board(yo_run_to_posv[pc], hi_remote_check_board);
				setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
			}
			if (piece[pc].move_style & BM_MS_RUN_FAMILY) {
				// 走り駒の移動先を設定
				setup_hi_run_to_posv(pc);
				setup_yo_run_to_posv(pc);
				// 走り駒の移動先盤面情報を設定
				setup_hi_run_to_board(pc);
				setup_yo_run_to_board(pc);
				// 合駒位置チェック用の走り駒の移動先盤面情報を設定
				setup_remote_check_board(yo_run_to_posv[pc], hi_remote_check_board);
				setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
			}
			// 行き処のない駒情報を設定
			setup_hi_valid_sq(pc);
			setup_yo_valid_sq(pc);
		}
	}

	setup_leap_to_board_sub(hi_leap_to_pos[PC_OU], around_sq);
	memcpy(around_to_pos, hi_leap_to_pos[PC_OU], sizeof(around_to_pos));
	setup_pos_order();
	fairy_move_style = 0x00000000;

//debug
#if 0
	{
		int f, r;
		for (r = 0; r <= 8; r++) {
			for (f = 8; f >= 0; f--) {
				printf(" %02x", hi_close_check_board[POS19(5, 5)][POS(f, r)]);
			}
			printf("\n");
		}
		printf("---\n");
		for (r = 0; r <= 8; r++) {
			for (f = 8; f >= 0; f--) {
				printf(" %02x", yo_close_check_board[POS19(5, 5)][POS(f, r)]);
			}
			printf("\n");
		}
	}
#endif

	return;
}

static void
setup_piece_info(int fp, uchar_t pc)
{
	char *s;
	int dual_pc = FALSE;
	uchar_t apc;

	// piece no.
	piece[pc].pc_no = fp;
	for (apc = PC_FU; apc < PC_END; apc++) {
		if (NOT(piece[apc].attr & B_PA_DEFINED)) {
			continue;
		}
		if (piece[apc].pc_no == fp) {
			dual_pc = TRUE;
			break;
		}
	}
	// attributes
	piece[pc].name = piece_info[fp].name;
	if (pc < RAW_PC_END) {
		pc_count[pc] = piece_info[fp].max_count;
	}
	piece[pc].attr = piece_info[fp].attr | B_PA_DEFINED;
	piece[pc].move_style = piece_info[fp].move_style;
	fairy_move_style |= piece[pc].move_style &
			(B_MS_HOP | B_MS_DECAPT_HOP | B_MS_PAO_JUMP | B_MS_JUMP | B_MS_JUMP_HOP |
				B_MS_REFL135 | B_MS_REFL90 | B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP | B_MS_MAO_STEP);
	if ((piece[pc].move_style & (B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP)) && equi_hop_pc == NUL) {
		equi_hop_pc = pc;
	}
	// graphs
	if (dual_pc == TRUE) {
		s = malloc(sizeof(graph_fp[fp]));
		memcpy(s, graph_fp[fp], sizeof(graph_fp[fp]));
	} else {
		s = graph_fp[fp];
	}
	strcpy(s, piece_info[fp].graphv[0]);
	graph[pc] = s;
	b_graph[pc] = s;
	s += strlen(s) + 1;
	strcat(strcpy(s, " "), graph[pc]);
	b_graph[pc | B_PC_HITHER] = s;
	s += strlen(s) + 1;
	strcat(strcpy(s, "v"), graph[pc]);
	b_graph[pc | B_PC_YONDER] = s;

	return;
}

static int
get_fp_no(char *argstr, int keep)
{
	char *s;
	int fp, i;
	char work[256], n;

	memcpy(work, argstr, 255);
	work[255] = '\0';
	if (work[0] == '=') {
		s = work + 1;
	} else {
		_mbstok(work, "=,#@:{?_");
		n = work[1];
		memset(work, '_', strlen(work) + 1);
		s = _mbstok(NULL, ",#@:?{|}_");
		if (s[0] == '*' && s[1] == '*') {
			s[0] = 'F';
			s[1] = n == 'P' ? '1' : n;
		}
	}
	if (strlen(s) == 3) {
		for (fp = NP_END; fp < FP_END; fp++) {
			if (NOT(piece_info[fp].attr & B_PA_PIECE)) {
				continue;
			}
			for (i = 0; piece_info[fp].graphv[i] != NULL; i++) {
				if (EQUAL(s, piece_info[fp].graphv[i])) {
					memset(s, '_', 3);
					goto get_fp_no_exit;
				}
			}
		}
		return FP_EXEND;
	}
	for (fp = 0; fp < FP_END; fp++) {
		if (NOT(piece_info[fp].attr & B_PA_PIECE)) {
			continue;
		}
		for (i = 0; piece_info[fp].graphv[i] != NULL; i++) {
			if (EQUAL(s, piece_info[fp].graphv[i])) {
				memset(s, '_', strlen(piece_info[fp].graphv[i]));
				goto get_fp_no_exit;
			}
		}
	}
	for (fp = FP_END; fp < FP_EXEND; fp++) {
		if (NOT(piece_info[fp].attr & B_PA_PIECE)) {
			continue;
		}
		for (i = 0; piece_info[fp].graphv[i] != NULL; i++) {
			if (EQUAL(s, piece_info[fp].graphv[i])) {
				memset(s, '_', strlen(piece_info[fp].graphv[i]));
				goto get_fp_no_exit;
			}
		}
	}
	return FP_EXEND;

get_fp_no_exit:
	if (NOT(keep)) {
		memcpy(argstr, work, strlen(work));
	}
	return fp;
}

static void
setup_fairy_pc_count(char *argstr, uchar_t pc)
{
	char *s, *t;
	int n;

	s = argstr;
	s[0] = '_';  // for '#'
	s++;
	n = strtol(s, &t, 10);
	pc_count[pc] = n;
	for ( ; s < t; s++) {
		s[0] = '_';
	}

	return;
}

// @: 駒属性
//    R: ROYAL（王/玉に代わる）
//    P: 成（予め成が登録されている場合に限る）
//    H: 持駒にできる。
//    X: 同じ駒面の駒と入れ替える。例：将棋の金将を中将棋の飛車に成れる金将と入替え
static char *
setup_fairy_attr(char *argstr, int fp, uchar_t pc)
{
	char *s, *t;
	int pfp = NUL, i;
	uchar_t ppc = (pc | B_PC_PROMOTED), apc, appc;
	char work[256];

	memcpy(work, argstr, 255);
	work[15] = '\0';
	_mbstok(work, ",#@:{?_");
	work[0] = '_';  // for '@'	
	// P: Promote
	s = _mbschr(work, 'P');
	if (s != NULL) {
		pfp = fp_promoted[fp];
		if (pfp != -1) {
			piece[pc].attr |= B_PA_PROMOTABLE;
			// attributes
			piece[ppc].name = piece_info[pfp].name;
			piece[ppc].attr = piece_info[pfp].attr | B_PA_DEFINED;
			piece[ppc].move_style = piece_info[pfp].move_style;
			piece[ppc].pc_no = pfp;
			// graphs
			t = graph_fp[pfp];
			strcpy(t, piece_info[pfp].graphv[0]);
			graph[ppc] = t;
			b_graph[ppc] = t;
			t += strlen(t) + 1;
			strcat(strcpy(t, " "), graph[ppc]);
			b_graph[ppc | B_PC_HITHER] = t;
			t += strlen(t) + 1;
			strcat(strcpy(t, "v"), graph[ppc]);
			b_graph[ppc | B_PC_YONDER] = t;
		}
		s[0] = '_';
	}
	// R: Royal
	s = _mbschr(work, 'R');
	if (s != NULL) {
		piece[pc].attr |= B_PA_ROYAL;
		piece[pc].attr &= ~(B_PA_PLACABLE | B_PA_REUSABLE);
		b_graph[pc | B_PC_HITHER][0] = '_';
		b_graph[pc | B_PC_YONDER][0] = '~';
		if (piece[pc].attr & B_PA_PROMOTABLE) {
			piece[ppc].attr |= B_PA_ROYAL;
			piece[ppc].attr &= ~B_PA_REUSABLE;
			b_graph[ppc | B_PC_HITHER][0] = '_';
			b_graph[ppc | B_PC_YONDER][0] = '~';
		}
		for (i = 0; i < RAW_PC_END && NOT_NUL(hand_pc[i]) && hand_pc[i] != pc; i++)
			;
		if (hand_pc[i] == pc) {
			for ( ; i < RAW_PC_END && NOT_NUL(hand_pc[i]); i++) {
				hand_pc[i] = hand_pc[i + 1];
			}
		}
		s[0] = '_';
	}
	// H: Handing
	s = _mbschr(work, 'H');
	if (s != NULL) {
		piece[pc].attr |= B_PA_PLACABLE | B_PA_REUSABLE;
		if (piece[pc].attr & B_PA_PROMOTABLE) {
			piece[ppc].attr |= B_PA_REUSABLE;
		}
		for (i = 0; i < RAW_PC_END && NOT_NUL(hand_pc[i]); i++)
			;
		hand_pc[i] = pc;
		s[0] = '_';
	}
	// X: eXchange
	s = _mbschr(work, 'X');
	if (s != NULL) {
		// ★とりあえず、王は除外
		for (apc = PC_FU; apc < PC_F1; apc++) {
			if (EQUAL(graph[apc], graph[pc])) {
				piece[apc].attr &= ~B_PA_DEFINED;
				for (i = 0; i < RAW_PC_END && NOT_NUL(hand_pc[i]) && hand_pc[i] != apc; i++)
					;
				if (hand_pc[i] == apc) {
					for ( ; i < RAW_PC_END && NOT_NUL(hand_pc[i]); i++) {
						hand_pc[i] = hand_pc[i + 1];
					}
				}
				if (pfp != NUL) {
					appc = apc | B_PC_PROMOTED;
					piece[appc].attr &= ~B_PA_DEFINED;
				}
				break;
			}
		}
	}
	memcpy(argstr, work, strlen(work));

	return s;
}

//   攻方駒：  駒、  受方駒： v駒
// 攻方王駒： _駒、受方王駒： ~駒
static void
setup_given_fairy_graph_and_name(char *argstr, int fp, uchar_t pc)
{
	char *s;

	s = argstr;
	s[0] = '_';  // for ':'
	s++;
	if (s[0] != '/') {
		char *t = malloc(20);
		if (t == NULL) {
			fm_exit_mae("フェアリー駒面領域");
		}
		strncpy(t, s, 2);
		t[2] = '\0';
		graph[pc] = t;
		b_graph[pc] = t;
		t += strlen(t) + 1;
		strcat(strcpy(t, ((piece[pc].attr & B_PA_ROYAL) ? "_" : " ")), graph[pc]);
		b_graph[pc | B_PC_HITHER] = t;
		t += strlen(t) + 1;
		strcat(strcpy(t, ((piece[pc].attr & B_PA_ROYAL) ? "~" : "v")), graph[pc]);
		b_graph[pc | B_PC_YONDER] = t;
		s[0] = s[1] = '_';
		s += 2;
	}
	if (s[0] == '/') {
		char *t = malloc(20);
		uchar_t ppc = pc | B_PC_PROMOTED;
		s[0] = '_';
		s++;
		strncpy(t, s, 2);
		t[2] = '\0';
		graph[ppc] = t;
		b_graph[ppc] = t;
		t += strlen(t) + 1;
		strcat(strcpy(t, ((piece[ppc].attr & B_PA_ROYAL) ? "_" : " ")), graph[ppc]);
		b_graph[ppc | B_PC_HITHER] = t;
		t += strlen(t) + 1;
		strcat(strcpy(t, ((piece[ppc].attr & B_PA_ROYAL) ? "~" : "v")), graph[ppc]);
		b_graph[ppc | B_PC_YONDER] = t;
		s[0] = s[1] = '_';
		s += 2;
	}
	if (s[0] == '`') {
		char *t, *r;
		t = _mbschr(s + 1, '`');
		if (t != NULL) {
			// 0123456
			// `abcde`
			// s=0, t=6, t-s=6-0=6
			r = malloc(t - s);
			strncpy(r, s + 1, t - s - 1);
			r[t - s] = '\0';
			piece[pc].name = r;
			memset(s, '_', t - s + 1);
		}
	}

	return;
}

static int
merge_hi_vec(vec_t **vecp, vec_t *from, vec_t *diff)
{
	vec_t *vec = vecp[0], *work;
	int nv, nf, nd, i, j;

	// マージ相手のベクトル数を取得
	for (nf = 0; from[nf].file != 0 || from[nf].rank != 0; nf++)
		;
	if (nf == 0) {
		// マージ相手のベクトル数がゼロなら何もしない。
		return 0;
	}
	if (vec[0].file == 0 && vec[0].rank == 0) {
		// マージ元のベクトル数がゼロなら、マージ相手を全複写
		*vecp = calloc(nf + 1, sizeof(from[0]));
		vec = vecp[0];
		memcpy(vec, from, sizeof(from[0]) * (nf + 1));
	} else {
		// マージ元のベクトルと差異のあるものの数を抽出
		nv = nd = 0;
		for (i = 0; i < nf; i++) {
			for (nv = 0; vec[nv].file != 0 || vec[nv].rank != 0; nv++) {
				if (vec[nv].file == from[i].file && vec[nv].rank == from[i].rank) {
					if (vec[nv].range != from[i].range) {
						return -2;
					}
					break;
				}
			}
			if (vec[nv].file == 0 && vec[nv].rank == 0) {
				nd++;
			}
		}
		// マージ元のベクトルと差異のあるものをマージ
		work = calloc(nv + nd + 1, sizeof(vec[0]));
		memcpy(work, vec, sizeof(vec[0]) * (nv + 1));
		nv += nd;
		*vecp = work;
		vec = vecp[0];
		for (i = 0; i < nf; i++) {
			for (j = 0; vec[j].file != 0 || vec[j].rank != 0; j++) {
				if (vec[j].file == from[i].file && vec[j].rank == from[i].rank) {
					break;
				}
			}
			if (vec[j].file == 0 && vec[j].rank == 0) {
				vec[j + 1] = vec[j];
				vec[j] = from[i];
			}
		}
	}
	if (diff != NULL) {
		// 跳びと走りの同ベクトル値チェック
		for (i = 0; vec[i].file != 0 || vec[i].rank != 0; i++) {
			for (j = 0; diff[j].file != 0 || diff[j].rank != 0; j++) {
				if (diff[j].file == vec[i].file && diff[j].rank == vec[i].rank &&
						diff[j].range == vec[i].range) {
					return -1;
				}
			}
		}
	}

	return 0;
}

static void
merge_yo_vec(vec_t **vecp, vec_t *from, vec_t *diff)
{
	vec_t *vec = vecp[0], *work;
	int nv, nf, nd, i, j;

	// マージ相手のベクトル数を取得
	for (nf = 0; from[nf].file != 0 || from[nf].rank != 0; nf++)
		;
	if (nf == 0) {
		// マージ相手のベクトル数がゼロなら何もしない。
		return;
	}
	// マージ元の比較先とでベクトルと差異のあるものの数を抽出
	nd = 0;
	for (i = 0; vec[i].file != 0 || vec[i].rank != 0; i++) {
		for (j = 0; diff[j].file != 0 || diff[j].rank != 0; j++) {
			if (diff[j].file == vec[i].file && diff[j].rank == vec[i].rank &&
					diff[j].range == vec[i].range) {
				break;
			}
		}
		if (diff[j].file == 0 && diff[j].rank == 0) {
			nd++;
		}
	}
	if (nd == 0) {
		// マージ相手の比較先とでベクトルと差異のあるものの数を抽出
		for (i = 0; i < nf; i++) {
			for (j = 0; diff[j].file != 0 || diff[j].rank != 0; j++) {
				if (diff[j].file == vec[i].file && diff[j].rank == vec[i].rank &&
						diff[j].range == vec[i].range) {
					break;
				}
			}
			if (diff[j].file == 0 && diff[j].rank == 0) {
				nd++;
			}
		}
	}
	if (nd == 0) {
		// 比較先との間に差異がない場合、比較先を全複写
		*vecp = diff;
	} else {
		// 比較先との間に差異がある場合、マージ元にマージ相手をマージ
		if (vec[0].file == 0 && vec[0].rank == 0) {
			// マージ元のベクトル数がゼロなら、マージ相手を全複写
			*vecp = calloc(nf + 1, sizeof(from[0]));
			vec = vecp[0];
			memcpy(vec, from, sizeof(from[0]) * (nf + 1));
		} else {
			// マージ元のベクトルと差異のあるものの数を抽出
			nv = nd = 0;
			for (i = 0; i < nf; i++) {
				for (nv = 0; vec[nv].file != 0 || vec[nv].rank != 0; nv++) {
					if (vec[nv].file == from[i].file && vec[nv].rank == from[i].rank) {
						break;
					}
				}
				if (vec[nv].file == 0 && vec[nv].rank == 0) {
					nd++;
				}
			}
			// マージ元のベクトルと差異のあるものをマージ
			work = calloc(nv + nd + 1, sizeof(vec[0]));
			memcpy(work, vec, sizeof(vec[0]) * (nv + 1));
			nv += nd;
			*vecp = work;
			vec = vecp[0];
			for (i = 0; i < nf; i++) {
				for (j = 0; vec[j].file != 0 || vec[j].rank != 0; j++) {
					if (vec[j].file == from[i].file && vec[j].rank == from[i].rank) {
						break;
					}
				}
				if (vec[j].file == 0 && vec[j].rank == 0) {
					vec[j + 1] = vec[j];
					vec[j] = from[i];
				}
			}
		}
	}

	return;
}

// {<v...>}  駒の移動ベクトル。「=<pc>」で登録済の駒の利きを表す。
//           「|」で複数のベクトルを区切る。
//    例：「{=KA|=KN}」＝Princess. 角＋騎
static int
setup_fairy_vecs_merge(char *argstr, uchar_t pc)
{
	char *s;
	piece_info_t *pip = &piece_info[piece[pc].pc_no], *vpip;
	int vfp;
	char exn = '1' + piece[pc].pc_no - FP_EX1;

	vfp = get_fp_no(argstr, TRUE);
	s = argstr + 1;
	if (vfp == FP_EXEND) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"未登録フェアリー駒\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	}
	vpip = &piece_info[vfp];
	if (vpip->move_style & (B_MS_SHISHI_LEAP | BM_MS_HOP_FAMILY | B_MS_CIRCLE_RUN)) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"獅子系、ホップ系、Rose\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	}
	if (((pip->move_style & (B_MS_LEAP | B_MS_RUN)) && (vpip->move_style & B_MS_STEP)) ||
		((pip->move_style & B_MS_STEP) && (vpip->move_style & (B_MS_LEAP | B_MS_RUN)))) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"跳び・跳々または跳々・走り混在不可\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	}
	if (vpip->hi_leap_vec[0].file == 0 && vpip->hi_leap_vec[0].rank == 0 &&
			vpip->hi_run_vec[0].file == 0 && vpip->hi_run_vec[0].rank == 0) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"不動駒\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	}
	pip->move_style |= vpip->move_style;
	(void)merge_hi_vec(&pip->hi_leap_vec, vpip->hi_leap_vec, NULL);
	switch (merge_hi_vec(&pip->hi_run_vec, vpip->hi_run_vec, pip->hi_leap_vec)) {
	case -1:
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"跳びと走りとで同じベクトル値\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	case -2:
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトル流用駒=%s "
				"同じベクトル値で異なる有効距離\n", cmdname, exn, strlen(cmdname) + 2, "", s);
		return -1;
	}
	merge_yo_vec(&pip->yo_leap_vec, vpip->yo_leap_vec, pip->hi_leap_vec);
	merge_yo_vec(&pip->yo_run_vec, vpip->yo_run_vec, pip->hi_run_vec);

	return 0;
}

// {<v...>}  駒の移動ベクトル。横方向、縦方向、種類で１組のベクトルを表す。
//           「|」で複数のベクトルを区切る。
//    横方向  「-<n>」右、「+<n>」左、「+0or-0」横方向の利きなし（縦方向のみ）
//    縦方向  「-<n>」前、「+<n>」後ろ、「+0or-0」縦方向の利きなし（横方向のみ）
//    種類    「L」跳び（省略時解釈）、「S<n>」跳々（跳び系。混在不可。
//            跳々の<n>は方向毎に異なる値を設定可）
//            「R」走り、「R<n>」限定走り、「G」Ｇホップ、「C」蝗ホップ、
//            「P」包ジャンプ、「J」ライオンジャンプ、「K」カンガルーホップ
//            （走り系。走りと限定走りのみ混在可。それ以外は混在不可。
//            跳び系の跳々とも混在不可。限定走りの<n>は方向毎に異なる値を設定可）
//            ※「R」と「R8」は等価
//    例：「-2-1」＝北北東方向の桂馬の跳び。「{-2-1|-2+1}」で桂馬
//    例：「-1+2R」＝西北西方向のナイトライダーの走り。「{-1-2R|+1-2R|+2-1R|
//         +2+1R|+1+2R|-1+2R|-2+1R|-2-1R}」でナイトライダー
//    例：「-1-1S2」＝東北方向の相の跳々（相は合駒が利く）
//    例：「+1+0R2」＝南方向の牛の限定走り（牛は最大２枡走る）
//    例：「{-1-0R|+1-1R|+1+0R|+1+1R}」＝奔蛇（鯨鯢と同じ）。前後と斜め後ろの走り
static int
setup_fairy_vecs_sub(char *argstr, uchar_t pc, vec_t *leap_vec, vec_t *run_vec, int *move_style)
{
	char *s;
	piece_info_t *pip = &piece_info[piece[pc].pc_no];
	int lvi, rvi, file, rank;
	char exn = '1' + piece[pc].pc_no - FP_EX1;

	for (lvi = 0; leap_vec[lvi].file != 0 || leap_vec[lvi].rank != 0; lvi++)
		;
	for (rvi = 0; run_vec[rvi].file != 0 || run_vec[rvi].rank != 0; rvi++)
		;
	s = argstr;
	if (s[0] != '+' && s[0] != '-') {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"ベクトル符号異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	if (s[1] < '0' || s[1] > '8') {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"ベクトル値異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	file = s[0] == '+' ? s[1] - '0' : -(s[1] - '0');
	if (s[2] != '+' && s[2] != '-') {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"ベクトル符号異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	if (s[3] < '0' || s[3] > '8') {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"ベクトル値異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	rank = s[2] == '+' ? s[3] - '0' : -(s[3] - '0');
	if (s[4] == '\0') {
		*move_style |= B_MS_LEAP;
		leap_vec[lvi].file = file;
		leap_vec[lvi].rank = rank;
	} else {
		switch (s[4]) {
		case 'L':
			if (s[5] != '\0') {
				fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
						"種類異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
				return -1;
			}
			*move_style |= B_MS_LEAP;
			leap_vec[lvi].file = file;
			leap_vec[lvi].rank = rank;
			break;
		case 'S':
			if (s[5] < '2' || s[5] > '8') {
				fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
						"跳々値異常\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
				return -1;
			}
			*move_style |= B_MS_STEP;
			leap_vec[lvi].file = file;
			leap_vec[lvi].rank = rank;
			leap_vec[lvi].range = s[5] - '0';
			break;
		case 'R':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定走り値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_RUN;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		case 'G':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定Ｇホップ値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_HOP;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		case 'C':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定蝗ホップ値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_DECAPT_HOP;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		case 'P':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定包ジャンプ値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_PAO_JUMP;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		case 'J':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定ジャンプ値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_JUMP;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		case 'K':
			if (s[5] != '\0') {
				if (s[5] < '2' || s[5] > '8') {
					fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s"
							"移動ベクトルデータ=%s 限定カンガルーホップ値異常\n",
							cmdname, exn, strlen(cmdname) + 2, "", argstr);
					return -1;
				}
				run_vec[rvi].range = s[5] - '0';
			}
			*move_style |= B_MS_JUMP_HOP;
			run_vec[rvi].file = file;
			run_vec[rvi].rank = rank;
			break;
		default:
			fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
					"種類異常（有効：L/S/R/H/C/P/J/K）\n",
					cmdname, exn, strlen(cmdname) + 2, "", argstr);
			return -1;
		}
	}
	if (bit_count_ulong((pip->move_style | *move_style) & BM_MS_RUN_FAMILY) >= 2) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"走りとホップ系の混在不可\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	if (((pip->move_style | *move_style) & (B_MS_LEAP | B_MS_STEP)) &&
		((pip->move_style | *move_style) & (BM_MS_RUN_FAMILY & ~B_MS_RUN))) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"跳び・跳々とホップ系混在不可\n", cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}
	if (((pip->move_style & (B_MS_LEAP | B_MS_RUN)) && (*move_style & B_MS_STEP)) ||
		((pip->move_style & B_MS_STEP) && (*move_style & (B_MS_LEAP | B_MS_RUN)))) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ=%s "
				"跳び・跳々または跳々・走り混在不可\n",
				cmdname, exn, strlen(cmdname) + 2, "", argstr);
		return -1;
	}

	return 0;
}

static int
setup_fairy_vecs_merge2(uchar_t pc, vec_t *hi_leap_vec, vec_t *hi_run_vec, ulong_t move_style)
{
	piece_info_t *pip = &piece_info[piece[pc].pc_no];
	vec_t *yo_leap_vec, *yo_run_vec;
	int i;
	char exn = '1' + piece[pc].pc_no - FP_EX1;

	pip->move_style |= move_style;
	(void)merge_hi_vec(&pip->hi_leap_vec, hi_leap_vec, NULL);
	switch (merge_hi_vec(&pip->hi_run_vec, hi_run_vec, pip->hi_leap_vec)) {
	case -1:
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ "
				"跳びと走りとで同じベクトル値\n", cmdname, exn, strlen(cmdname) + 2, "");
		return -1;
	case -2:
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです...\n%*s移動ベクトルデータ "
				"同じベクトル値で異なる有効距離\n", cmdname, exn, strlen(cmdname) + 2, "");
		return -1;
	}
	for (i = 0; hi_leap_vec[i].file != 0 || hi_leap_vec[i].rank != 0; i++)
		;
	yo_leap_vec = calloc(i + 1, sizeof(yo_leap_vec[0]));
	for (i = 0; hi_leap_vec[i].file != 0 || hi_leap_vec[i].rank != 0; i++) {
		yo_leap_vec[i].file = -(hi_leap_vec[i].file);
		yo_leap_vec[i].rank = -(hi_leap_vec[i].rank);
		yo_leap_vec[i].range = hi_leap_vec[i].range;
	}
	merge_yo_vec(&pip->yo_leap_vec, yo_leap_vec, pip->hi_leap_vec);
	for (i = 0; hi_leap_vec[i].file != 0 || hi_leap_vec[i].rank != 0; i++)
		;
	yo_run_vec = calloc(i + 1, sizeof(yo_run_vec[0]));
	for (i = 0; hi_leap_vec[i].file != 0 || hi_leap_vec[i].rank != 0; i++) {
		yo_run_vec[i].file = -(hi_run_vec[i].file);
		yo_run_vec[i].rank = -(hi_run_vec[i].rank);
		yo_run_vec[i].range = hi_run_vec[i].range;
	}
	merge_yo_vec(&pip->yo_run_vec, yo_run_vec, pip->hi_run_vec);

	return 0;
}

static int
setup_fairy_vecs(char *argstr, uchar_t pc)
{
	char *s, *t;
	vec_t *leap_vec, *run_vec;
	ulong_t move_style = 0;
	int n;

	s = argstr;
	s[0] = '_';  // for '{'
	s++;
	n = 0;
	for (t = s; t[0] != '}' && t[0] != '\0'; t++) {
		if (t[0] == '|') {
			n++;
		}
	}
	leap_vec = calloc(n + 2, sizeof(leap_vec[0]));
	run_vec = calloc(n + 2, sizeof(run_vec[0]));
	for (t = _mbstok(s, "|}"); t != NULL; t = _mbstok(NULL, "|}")) {
		if (t[0] == '_') {
			continue;
		}
		if (t[0] == '=') {
			if (setup_fairy_vecs_merge(t, pc) != 0) {
				return -1;
			}
		} else {
			if (setup_fairy_vecs_sub(t, pc, leap_vec, run_vec, &move_style) != 0) {
				return -1;
			}
		}
		memset(t, '_', strlen(t) + 1);
	}
	if (leap_vec[0].file != 0 || leap_vec[0].rank != 0 ||
			run_vec[0].file != 0 || run_vec[0].rank != 0) {
		if (setup_fairy_vecs_merge2(pc, leap_vec, run_vec, move_style) != 0) {
			return -1;
		}
	}
	piece[pc].move_style = piece_info[piece[pc].pc_no].move_style;

	return 0;
}

static void
resetup_paos(int fp, int i, uchar_t pc, int face)
{
	if (i >= 2 && face == OFF && (fp == FP_PA || fp == FP_VA || fp == FP_LE)) {
		// ２つ目以降のフェアリー駒で、駒面未指定で、Pao, Vao, Leoの場合
		char *s;
		int j;
		uchar_t k;
		for (j = 1, k = 0x00; j < i; j++) {
			if (EQUALS(graph[PC_F0 + j], graph_pa1)) {
				k |= 0x80;
			} else if (EQUALS(graph[PC_F0 + j], graph_pa2)) {
				k |= 0x40;
			} else if (EQUALS(graph[PC_F0 + j], graph_pa3)) {
				k |= 0x20;
			}
		}
		if (k == 0x80) {
			s = graph_fp[fp];
			strcpy(s, graph_pa2);
			graph[pc] = s;
			b_graph[pc] = s;
			s += strlen(s) + 1;
			strcpy(s, hi_b_graph_pa2);
			b_graph[pc | B_PC_HITHER] = s;
			s += strlen(s) + 1;
			strcpy(s, yo_b_graph_pa2);
			b_graph[pc | B_PC_YONDER] = s;
		} else if (k == 0xc0) {
			s = graph_fp[fp];
			strcpy(s, graph_pa3);
			graph[pc] = s;
			b_graph[pc] = s;
			s += strlen(s) + 1;
			strcpy(s, hi_b_graph_pa3);
			b_graph[pc | B_PC_HITHER] = s;
			s += strlen(s) + 1;
			strcpy(s, yo_b_graph_pa3);
			b_graph[pc | B_PC_YONDER] = s;
		}
	}

	return;
}

static int
test_both_royal_or_not(uchar_t pc, uchar_t apc, char e_fp_no_c)
{
	if (((piece[pc].attr & B_PA_ROYAL) && (piece[apc].attr & B_PA_ROYAL))
			|| (NOT(piece[pc].attr & B_PA_ROYAL) && NOT(piece[apc].attr & B_PA_ROYAL))) {
		fprintf(stderr, "%s: /EF%cオプションと同じ駒面のフェアリー駒が既に存在します...\n",
				cmdname, e_fp_no_c);
		return -1;
	}
	if (piece[pc].attr & B_PA_ROYAL) {
		piece[pc].attr |= B_PA_SAME_FACE;
	} else {
		piece[apc].attr |= B_PA_SAME_FACE;
	}

	return 0;
}

static int
check_same_face(uchar_t pc, uchar_t ppc, char e_fp_no_c)
{
	uchar_t apc;

	for (apc = PC_FU; apc < RAW_PC_END; apc++) {
		uchar_t appc = apc | B_PC_PROMOTED;
		if (NOT(piece[apc].attr & B_PA_DEFINED) || apc == pc) {
			continue;
		}
		if (EQUAL(graph[apc], graph[pc])) {
			if (apc >= PC_F1 && apc <= PC_F7) {
				if (test_both_royal_or_not(pc, apc, e_fp_no_c) == -1) {
					return -1;
				}
			}
		}
		if ((piece[pc].attr & B_PA_PROMOTABLE) && EQUAL(graph[apc], graph[ppc])) {
			if (apc >= PC_F1 && apc <= PC_F7) {
				if (test_both_royal_or_not(ppc, apc, e_fp_no_c) == -1) {
					return -1;
				}
			}
		}
		if ((piece[apc].attr & B_PA_PROMOTABLE) && EQUAL(graph[appc], graph[pc])) {
			if (apc >= PC_F1 && apc <= PC_F7) {
				if (test_both_royal_or_not(pc, appc, e_fp_no_c) == -1) {
					return -1;
				}
			}
		}
		if ((piece[apc].attr & B_PA_PROMOTABLE) && (piece[pc].attr & B_PA_PROMOTABLE)
				&& EQUAL(graph[appc], graph[ppc])) {
			if (apc >= PC_F1 && apc <= PC_F7) {
				if (test_both_royal_or_not(ppc, appc, e_fp_no_c) == -1) {
					return -1;
				}
			}
		}
	}

	return 0;
}

static void
make_equi_to_board(uchar_t pc, int pos, uchar_t *board)
{
	char *to, *mid;
	int i;

	memset(board, '.', 81);
	board[pos] = '#';
	to = hi_run_to_posv[pc][pos][0];
	for (i = 0; to[i] != EOP; i++) {
		board[to[i]] = 'E';
	}
	mid = yo_run_to_posv[pc][pos][0];
	for (i = 0; mid[i] != EOP; i++) {
		if (board[mid[i]] != 'E') {
			board[mid[i]] = 'e';
		}
	}

	return;
}

static void
make_to_board(int side, uchar_t pc, int pos, uchar_t *board)
{
	memset(board, '.', 81);
	board[pos] = '#';
	if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
		char *to;
		int i;
		if (side == B_PC_HITHER) {
			to = hi_leap_to_pos[pc][pos];
		} else {
			to = yo_leap_to_pos[pc][pos];
		}
		if (to == NULL) {
			printf("to (leap) == NULL\n");
			return;
		}
		for (i = 0; to[i] != EOP; i++) {
			if (piece[pc].move_style & B_MS_LEAP) {
				// Leap
				board[to[i]] = 'L';
			} else {
				// 獅子Leap
				board[to[i]] = '@';
			}
		}
	}
	if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
		char **tov, *to;
		int i, j;
		if (side == B_PC_HITHER) {
			tov = hi_run_to_posv[pc][pos];
		} else {
			tov = yo_run_to_posv[pc][pos];
		}
		if (tov == NULL) {
			printf("tov (run) == NULL\n");
			return;
		}
		for (i = 0; tov[i] != NULL; i++) {
			to = tov[i];
			for (j = 0; to[j] != EOP; j++) {
				if (piece[pc].move_style & (B_MS_RUN | B_MS_CIRCLE_RUN)) {
					// Run or CircleRun
					board[to[j]] = 'R';
				} else if (piece[pc].move_style & B_MS_HOP) {
					// Hop
					board[to[j]] = (j == 0) ? 'h' : 'H';
				} else if (piece[pc].move_style & B_MS_DECAPT_HOP) {
					// Decapt-Hop
					board[to[j]] = (j == 0) ? 'd' : 'D';
				} else if (piece[pc].move_style & B_MS_PAO_JUMP) {
					// Pao-Move
					board[to[j]] = (j == 0) ? 'p' : 'P';
				} else if (piece[pc].move_style & B_MS_JUMP) {
					// Jump
					board[to[j]] = (j == 0) ? 'j' : 'J';
				} else if (piece[pc].move_style & B_MS_JUMP_HOP) {
					// Jump-Hop (jump 2-pieces)
					board[to[j]] = (j <= 1) ? '1' : '2';
				} else {
					// Step
					board[to[j]] = 'S';
				}
			}
		}
	}

	return;
}

static void
make_equi_hop_map(uchar_t pc, uchar_t *map)
{
	int pf, pr, pos, i, j;
	uchar_t board[81];

	make_equi_to_board(pc, POS(8, 8), board);
	for (pr = 0, i = 0; pr <= 8; pr++, i++) {
		for (pf = 8, j = 8; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_equi_to_board(pc, POS(8, 0), board);
	for (pr = 0, i = 8; pr <= 8; pr++, i++) {
		for (pf = 8, j = 8; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_equi_to_board(pc, POS(0, 8), board);
	for (pr = 0, i = 0; pr <= 8; pr++, i++) {
		for (pf = 8, j = 16; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_equi_to_board(pc, POS(0, 0), board);
	for (pr = 0, i = 8; pr <= 8; pr++, i++) {
		for (pf = 8, j = 16; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}

	return;
}

static void
make_map(int side, uchar_t pc, uchar_t *map)
{
	int pf, pr, pos, i, j;
	uchar_t board[81];

	make_to_board(side, pc, POS(8, 8), board);
	for (pr = 0, i = 0; pr <= 8; pr++, i++) {
		for (pf = 8, j = 8; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_to_board(side, pc, POS(8, 0), board);
	for (pr = 0, i = 8; pr <= 8; pr++, i++) {
		for (pf = 8, j = 8; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_to_board(side, pc, POS(0, 8), board);
	for (pr = 0, i = 0; pr <= 8; pr++, i++) {
		for (pf = 8, j = 16; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}
	make_to_board(side, pc, POS(0, 0), board);
	for (pr = 0, i = 8; pr <= 8; pr++, i++) {
		for (pf = 8, j = 16; pf >= 0; pf--, j--) {
			pos = POS(pf, pr);
			if (board[pos] != '.') {
				map[i + j * 17] = board[pos];
			}
		}
	}

	return;
}

static void
show_equi_hop_map(uchar_t pc)
{
	int i, j;
	uchar_t map[17 * 17];

	memset(map, '.', sizeof(map));
	printf("%s\n", graph[pc]);
	make_equi_hop_map(pc, map);
	for (i = 0; i <= 16; i++) {
		for (j = 16; j >= 0; j--) {
			printf(" %c", map[i + j * 17]);
		}
		printf("\n");
	}

	return;
}

static void
show_piece_vec_map(uchar_t pc, uchar_t ppc)
{
	vec_t *va, *ava;
	int i, j;
	uchar_t same;
	uchar_t map[17 * 17];
	uchar_t prom_map[17 * 17];

	if (piece[pc].move_style & (B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP)) {
		show_equi_hop_map(pc);
		goto stop;
	}
	same = ON;
	va = piece_info[piece[pc].pc_no].yo_leap_vec;
	ava = piece_info[piece[pc].pc_no].hi_leap_vec;
	for (i = 0; ; i++) {
		if (va[i].file == 0 && va[i].rank == 0) {
			break;
		}
		if (va[i].file != ava[i].file || va[i].rank != ava[i].rank || va[i].range != ava[i].range) {
			same = OFF;
			break;
		}
	}
	if (same == ON) {
		va = piece_info[piece[pc].pc_no].yo_run_vec;
		ava = piece_info[piece[pc].pc_no].hi_run_vec;
		for (i = 0; ; i++) {
			if (va[i].file == 0 && va[i].rank == 0) {
				break;
			}
			if (va[i].file != ava[i].file || va[i].rank != ava[i].rank ||
					va[i].range != ava[i].range) {
				same = OFF;
				break;
			}
		}
	}

	memset(map, '.', sizeof(map));
	memset(prom_map, '.', sizeof(prom_map));
	if ((piece[pc].attr & B_PA_PROMOTABLE) && (piece[ppc].attr & B_PA_DEFINED)) {
		printf("%s %s >> %s %s\n", graph[pc], piece[pc].name, graph[ppc], piece[ppc].name);
		make_map(B_PC_HITHER, pc, map);
		make_map(B_PC_HITHER, ppc, prom_map);
		if (piece[pc].move_style & (B_MS_LONG_LEAP | B_MS_RUN | B_MS_CIRCLE_RUN)) {
			for (i = 0; i <= 16; i++) {
				for (j = 16; j >= 0; j--) {
					printf(" %c", map[i + j * 17]);
				}
				printf(" ");
				for (j = 16; j >= 0; j--) {
					printf(" %c", prom_map[i + j * 17]);
				}
				printf("\n");
			}
		} else {
			for (i = 4; i <= 12; i++) {
				for (j = 12; j >= 4; j--) {
					printf(" %c", map[i + j * 17]);
				}
				printf(" ");
				for (j = 12; j >= 4; j--) {
					printf(" %c", prom_map[i + j * 17]);
				}
				printf("\n");
			}
		}
	} else {
		printf("%s %s\n", graph[pc], piece[pc].name);
		make_map(B_PC_HITHER, pc, map);
		if (piece[pc].move_style & (B_MS_LONG_LEAP | B_MS_RUN | B_MS_CIRCLE_RUN)) {
			for (i = 0; i <= 16; i++) {
				for (j = 16; j >= 0; j--) {
					printf(" %c", map[i + j * 17]);
				}
				printf("\n");
			}
		} else {
			for (i = 4; i <= 12; i++) {
				for (j = 12; j >= 4; j--) {
					printf(" %c", map[i + j * 17]);
				}
				printf("\n");
			}
		}
	}
	if (same == OFF) {
		memset(map, '.', sizeof(map));
		memset(prom_map, '.', sizeof(prom_map));
		if ((piece[pc].attr & B_PA_PROMOTABLE) && (piece[ppc].attr & B_PA_DEFINED)) {
			make_map(B_PC_YONDER, pc, map);
			make_map(B_PC_YONDER, ppc, prom_map);
			if (piece[pc].move_style & (B_MS_LONG_LEAP | B_MS_RUN | B_MS_CIRCLE_RUN)) {
				printf("----------------------------------  ---------------------------------- \n");
				for (i = 0; i <= 16; i++) {
					for (j = 16; j >= 0; j--) {
						printf(" %c", map[i + j * 17]);
					}
					printf(" ");
					for (j = 16; j >= 0; j--) {
						printf(" %c", prom_map[i + j * 17]);
					}
					printf("\n");
				}
			} else {
				printf("------------------  ------------------\n");
				for (i = 4; i <= 12; i++) {
					for (j = 12; j >= 4; j--) {
						printf(" %c", map[i + j * 17]);
					}
					printf(" ");
					for (j = 12; j >= 4; j--) {
						printf(" %c", prom_map[i + j * 17]);
					}
					printf("\n");
				}
			}
		} else {
			make_map(B_PC_YONDER, pc, map);
			if (piece[pc].move_style & (B_MS_LONG_LEAP | B_MS_RUN | B_MS_CIRCLE_RUN)) {
				printf("----------------------------------\n");
				for (i = 0; i <= 16; i++) {
					for (j = 16; j >= 0; j--) {
						printf(" %c", map[i + j * 17]);
					}
					printf("\n");
				}
			} else {
				printf("------------------\n");
				for (i = 4; i <= 12; i++) {
					for (j = 12; j >= 4; j--) {
						printf(" %c", map[i + j * 17]);
					}
					printf("\n");
				}
			}
		}
	}

stop:
	printf("***\n");
	getch();
	printf("\n");

	return;
}

// /EF<x>=<pc>[#<n>][@<y...>|][:<f>[/<b>]]
//   F<x>  P,1,2,3,4,5,6,7（「P」は「1」と同等）
//   <pc>  駒を示す文字列。英数字2or3文字または漢字、片仮名、平仮名１文字。必須
//   #<n>  駒の枚数。任意（デフォールト値は、基本的に本来のルールに従う）
//   @<y...>  駒の属性
//      R  王属性（詰の対象を示す。持駒にはできない。成ることはできる）   
//      H  持駒属性（持駒にできる）   
//      P  成属性（本来のルールで成れる駒に成れる）
//      X  入替え
//   :[<f>][/<b>]  駒面。成属性を指定したとき、「/」と共に成駒の面を指定できる。
// /EF<x>=**#<n>@<y...>:<f>{<v...>}
//   駒面は「F<x>」となる。「<x>」が「P」の場合は「F1」になる。
//   「#<n>」  「@<y...>」は上記と同じ「:<f>[`<d>`]」は後ろに説明を付加することが
//             できる。
//   {<v...>}  駒の移動ベクトル。横方向、縦方向、種類で１組のベクトルを表す。
//             または「=<pc>」で登録済の駒の利きを表す。
//             「|」で複数のベクトルを区切る。
//      横方向  「-<n>」右、「+<n>」左、「+0or-0」横方向の利きなし（縦方向のみ）
//      縦方向  「-<n>」前、「+<n>」後ろ、「+0or-0」縦方向の利きなし（横方向のみ）
//      種類    「L」跳び（省略時解釈）、「S<n>」跳々（跳び系。混在不可。
//              跳々の<n>は方向毎に異なる値を設定可）
//              「R」走り、「R<n>」限定走り、「G」Ｇホップ、「C」蝗ホップ、
//              「P」包ジャンプ、「J」ライオンジャンプ、「K」カンガルーホップ
//              （走り系。走りと限定走りのみ混在可。それ以外は混在不可。
//              限定走りの<n>は方向毎に異なる値を設定可）
//              ※「R」と「R8」は等価
//      例：「-2-1」＝北北東方向の桂馬の跳び。「{-2-1|-2+1}」で桂馬
//      例：「-1+2R」＝西北西方向のナイトライダーの走り。「{-1-2R|+1-2R|+2-1R|
//           +2+1R|+1+2R|-1+2R|-2+1R|-2-1R}」でナイトライダー
//      例：「-1-1S2」＝東北方向の相の跳々（相は合駒が利く）
//      例：「+1+0R2」＝南方向の牛の限定走り（牛は最大２枡走る）
//      例：「{-1-0R|+1-1R|+1+0R|+1+1R}」＝奔蛇（鯨鯢と同じ）。前後と斜め後ろの走り
//      例：「{=KA|=KN}」＝Princess. 角＋騎

int
option_E_FP(char *argstr, int i)
{
	char *s, *save;
	int fp, show, face;
	uchar_t pc = PC_F0 + i, ppc = (PC_F0 + i) | B_PC_PROMOTED;

	show = OFF;
	face = OFF;

	save = _strdup(argstr);
	strtok(save, ",");
	if (piece[pc].attr & B_PA_DEFINED) {
		fprintf(stderr, "%s: /EF%cオプションは、既に指定されています... %s\n",
				cmdname, e_fp_no[i], save);
		return -1;
	}

	fp = get_fp_no(save, TRUE);
	if (fp == FP_EXEND) {
		fprintf(stderr, "%s: /EF%cオプション指定のエラーです... %s\n", cmdname, e_fp_no[i], save);
		return -1;
	}
	get_fp_no(save, FALSE);

	setup_piece_info(fp, pc);

	s = _mbschr(save, '#');
	if (s != NULL) {
		setup_fairy_pc_count(s, pc);
	}
	s = _mbschr(save, '@');
	if (s != NULL) {
		setup_fairy_attr(s, fp, pc);
	}
	s = _mbschr(save, ':');
	if (s != NULL) {
		setup_given_fairy_graph_and_name(s, fp, pc);
		face = ON;
	}
	s = _mbschr(save, '?');
	if (s != NULL) {
		s[0] = '_';
		show = ON;
	}
	if (fp >= FP_EX1 && fp <= FP_EX7) {
		s = _mbschr(save, '{');
		if (s != NULL) {
			if (setup_fairy_vecs(s, pc) < 0) {
				return -1;
			}
		}
	}

	// 移動先情報、移動先盤面情報
	if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
		// 跳び駒の移動先を設定
		setup_hi_leap_to_pos(pc);
		setup_yo_leap_to_pos(pc);
		// 跳び駒の移動先盤面情報を設定
		setup_hi_leap_to_board(pc);
		setup_yo_leap_to_board(pc);
		// 跳び駒系の移動先盤面情報を設定
		setup_close_check_board(yo_leap_to_pos[pc], hi_close_check_board);
		setup_close_check_board(hi_leap_to_pos[pc], yo_close_check_board);
	} else if (piece[pc].move_style & B_MS_STEP) {
		// 跳々駒の移動先を設定
		setup_hi_step_to_posv(pc);	// setup_hi_run_to_posv() と同様の処理
		setup_yo_step_to_posv(pc);	// setup_yo_run_to_posv() と同様の処理
		// 跳々駒の移動先盤面情報を設定
		setup_hi_step_to_board(pc);	// setup_hi_leap_to_board() と同様の処理
		setup_yo_step_to_board(pc);	// setup_yo_leap_to_board() と同様の処理
		// 合駒位置チェック用の跳々駒の移動先盤面情報を設定
		setup_remote_check_board(yo_run_to_posv[pc], hi_remote_check_board);
		setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	} else if (piece[pc].move_style & B_MS_MAO_STEP) {
		// MAO_STEP(Mao, Moa)の移動先を設定
		setup_mao_step_to_pos(pc);
	}
	if (piece[pc].move_style & BM_MS_RUN_FAMILY) {
		// 走り駒系の移動先を設定
		setup_hi_run_to_posv(pc);
		setup_yo_run_to_posv(pc);
		// 走り駒系の移動先盤面情報を設定
		setup_hi_run_to_board(pc);
		setup_yo_run_to_board(pc);
		// 合駒位置チェック用の走り駒系の移動先盤面情報を設定
		setup_remote_check_board(yo_run_to_posv[pc], hi_remote_check_board);
		setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	} else if (piece[pc].move_style & B_MS_REFL135) {
		// REFL135(Sparrow)の移動先を設定
		setup_refl135_to_pos(pc);
	} else if (piece[pc].move_style & B_MS_REFL90) {
		// REFL90(Eagle)の移動先を設定
		setup_refl90_to_pos(pc);
	} else if (piece[pc].move_style & (B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP)) {
		// Equi-jump系の移動先を設定
		setup_equi_hop_to_pos(pc);
	} else if (piece[pc].move_style & B_MS_CIRCLE_RUN) {
		// CIRCLE_RUN(Rose)の移動先を設定
		setup_hi_circle_run_to_posv(pc);
		setup_yo_circle_run_to_posv(pc);
		// 走り駒系の移動先盤面情報を設定
		setup_hi_run_to_board(pc);
		setup_yo_run_to_board(pc);
		// 合駒位置チェック用の走り駒系の移動先盤面情報を設定
		setup_remote_check_board(yo_run_to_posv[pc], hi_remote_check_board);
		setup_remote_check_board(hi_run_to_posv[pc], yo_remote_check_board);
	}
	if (NOT(piece[pc].move_style &
			(B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP | B_MS_REFL135 | B_MS_REFL90))) {
		// 行き処のない駒情報を設定
		setup_hi_valid_sq(pc);
		setup_yo_valid_sq(pc);
	}
	if ((piece[pc].attr & B_PA_PROMOTABLE) && (piece[ppc].attr & B_PA_DEFINED)) {
		if (piece[ppc].move_style & BM_MS_LEAP_FAMILY) {
			// 成駒の跳び駒の移動先を設定
			setup_hi_leap_to_pos(ppc);
			setup_yo_leap_to_pos(ppc);
			// 成駒の跳び駒の移動先盤面情報を設定
			setup_hi_leap_to_board(ppc);
			setup_yo_leap_to_board(ppc);
			// 跳び駒系の移動先盤面情報を設定
			setup_close_check_board(yo_leap_to_pos[ppc], hi_close_check_board);
			setup_close_check_board(hi_leap_to_pos[ppc], yo_close_check_board);
		} else if (piece[ppc].move_style & B_MS_STEP) {
			// 成駒の跳々駒の移動先を設定
			setup_hi_step_to_posv(ppc);	// setup_hi_run_to_posv() と同様の処理
			setup_yo_step_to_posv(ppc);	// setup_yo_run_to_posv() と同様の処理
			// 成駒の跳々駒の移動先盤面情報を設定
			setup_hi_step_to_board(ppc);	// setup_hi_leap_to_board() と同様の処理
			setup_yo_step_to_board(ppc);	// setup_yo_leap_to_board() と同様の処理
			// 合駒位置チェック用の成駒の跳々駒の移動先盤面情報を設定
			setup_remote_check_board(yo_run_to_posv[ppc], hi_remote_check_board);
			setup_remote_check_board(hi_run_to_posv[ppc], yo_remote_check_board);
		}
		if (piece[ppc].move_style & BM_MS_RUN_FAMILY) {
			// 成駒の走り駒系の移動先を設定
			setup_hi_run_to_posv(ppc);
			setup_yo_run_to_posv(ppc);
			// 成駒の走り駒系の移動先盤面情報を設定
			setup_hi_run_to_board(ppc);
			setup_yo_run_to_board(ppc);
			// 合駒位置チェック用の成駒の走り駒系の移動先盤面情報を設定
			setup_remote_check_board(yo_run_to_posv[ppc], hi_remote_check_board);
			setup_remote_check_board(hi_run_to_posv[ppc], yo_remote_check_board);
		}
		// 成駒の行き処のない駒情報を設定
		setup_hi_valid_sq(ppc);
		setup_yo_valid_sq(ppc);
	}

	// 包の駒面の自動設定
	resetup_paos(fp, i, pc, face);

	// 駒面をチェック（標準セットに同じ駒面でどちらも王駒でないものがあれば削除、
	// 追加セットにあればエラー）
	if (check_same_face(pc, ppc, e_fp_no[i]) == -1) {
		return -1;
	}

	// 駒の属性をチェック
	check_piece_attrs();

	// 駒の利きを表示
	if (show == ON) {
//		debug_print_pc_attrs(pc);
		show_piece_vec_map(pc, ppc);
	}

	null_phase.yo_hand[pc] = pc_count[pc];

	return 0;
}

uchar_t
get_pc_code(char **ib)
{
	char **pca;
	uchar_t pc, royal;

	royal = OFF;
	if (EQUALS(*ib, "$")) {
		// =Royal
		royal = ON;
		*ib += STR_SIZE_OF("$");
	}
	if ((*ib)[0] == 'C' || (*ib)[0] == 'P' || (*ib)[0] == 'T' || (*ib)[0] == 'D') {
		for (pc = PC_FU; pc < PC_END; pc++) {
			if (NOT(piece[pc].attr & B_PA_DEFINED)) {
				continue;
			}
			if ((royal == ON && NOT(piece[pc].attr & B_PA_ROYAL)) ||
				(royal == OFF && (piece[pc].attr & B_PA_ROYAL))) {
				continue;
			}
			for (pca = piece_info[piece[pc].pc_no].graphv + 1; pca[0] != NULL; pca++) {
				if (strlen(pca[0]) <= 2) {
					continue;
				}
				if (EQUALS(*ib, pca[0])) {
					*ib += strlen(pca[0]);
					return pc;
				}
			}
		}
		for (pc = PC_FU; pc < PC_END; pc++) {
			if (NOT(piece[pc].attr & B_PA_DEFINED)) {
				continue;
			}
			for (pca = piece_info[piece[pc].pc_no].graphv + 1; pca[0] != NULL; pca++) {
				if (strlen(pca[0]) <= 2) {
					continue;
				}
				if (EQUALS(*ib, pca[0])) {
					*ib += strlen(pca[0]);
					return pc;
				}
			}
		}
	}
	for (pc = PC_FU; pc < PC_END; pc++) {
		if (NOT(piece[pc].attr & B_PA_DEFINED)) {
			continue;
		}
		if ((royal == ON && NOT(piece[pc].attr & B_PA_ROYAL)) ||
			(royal == OFF && (piece[pc].attr & B_PA_ROYAL))) {
			continue;
		}
		if (EQUALS(*ib, graph[pc])) {
			*ib += strlen(graph[pc]);
			return pc;
		}
		for (pca = piece_info[piece[pc].pc_no].graphv + 1; pca[0] != NULL; pca++) {
			if (EQUALS(*ib, pca[0])) {
				*ib += strlen(pca[0]);
				return pc;
			}
		}
	}
	for (pc = PC_FU; pc < PC_END; pc++) {
		if (NOT(piece[pc].attr & B_PA_DEFINED)) {
			continue;
		}
		if (EQUALS(*ib, graph[pc])) {
			*ib += strlen(graph[pc]);
			return pc;
		}
		for (pca = piece_info[piece[pc].pc_no].graphv + 1; pca[0] != NULL; pca++) {
			if (EQUALS(*ib, pca[0])) {
				*ib += strlen(pca[0]);
				return pc;
			}
		}
	}

	return NUL;
}

static void
correct_for_nopromote(void)
{
	int pc;

	for (pc = PC_FU; pc < RAW_PC_END; pc++) {
		if (piece[pc].attr & B_PA_PROMOTABLE) {
			piece[pc].attr &= ~B_PA_PROMOTABLE;
			piece[pc].attr |= B_PA_PROMOTABLE_FE;
		}
	}

	return;
}

static void
correct_for_absolute_nopromote(void)
{
	int pc;

	for (pc = PC_FU; pc < RAW_PC_END; pc++) {
		if (piece[pc].attr & B_PA_PROMOTABLE) {
			piece[pc].attr &= ~B_PA_PROMOTABLE;
		}
	}

	return;
}

static void
correct_for_constraint_sq(void)
{
	int pc;

	for (pc = PC_FU; pc < PC_END; pc++) {
		piece[pc].attr &= ~B_PA_CONSTRAINT_SQ;
	}

	return;
}

static void
correct_partial_for_constraint_sq(void)
{
	int pc;
	int pf, pr;

	for (pc = PC_FU; pc < PC_END; pc++) {
		if (piece[pc].attr & B_PA_CONSTRAINT_SQ) {
			for (pf = 1; pf <= 9; pf++) {
				for (pr = 2; pr <= 9; pr++) {
					if (hi_valid_sq[pc][POS19(pf, pr)] == OFF) {
						hi_valid_sq[pc][POS19(pf, pr)] = ON;
					}
				}
				for (pr = 8; pr >= 1; pr--) {
					if (yo_valid_sq[pc][POS19(pf, pr)] == OFF) {
						yo_valid_sq[pc][POS19(pf, pr)] = ON;
					}
				}
			}
		}
	}

	return;
}

static void
correct_partial_for_constraint_pm(void)
{
	int pc;

	for (pc = PC_FU; pc < PC_END; pc++) {
		if (piece[pc].attr & B_PA_CONSTRAINT_PM) {
			piece[pc].attr &= ~B_PA_CONSTRAINT_PM;
		}
	}

	return;
}

void
correct_for_rule(void)
{
	if (cond_flag & B_C_NOPROMOTE) {
		correct_for_nopromote();
	} else if (cond_flag & B_C_ABSOLUTE_NOPROMOTE) {
		correct_for_absolute_nopromote();
	}
	if (cond_flag & (B_C_ANNAN | B_C_HAIMEN | B_C_NEKOSEN | B_C_NEKO2SEN)) {
		correct_for_constraint_sq();
	}
	if (cond_flag & (B_C_ANHOKU | B_C_TAIMEN)) {
		correct_partial_for_constraint_sq();
	}
	if (cond_flag & B_C_UCHIFU_OK) {
		correct_partial_for_constraint_pm();
	}

	return;
}

void
make_FP_E_str(char *s)
{
	int fp, i;

	strcpy(s, "/E");
	for (i = 1; i <= 7; i++) {
		fp = PC_F0 + i;
		if (NOT(piece[fp].attr & B_PA_DEFINED)) {
			continue;
		}
		if (strlen(s) != STR_SIZE_OF("/E")) {
			strcat(s, ",");
		}
		sprintf(s + strlen(s), "F%d=%s", i, piece_info[piece[fp].pc_no].graphv[1]);
		if (piece[fp].attr & (B_PA_PLACABLE | B_PA_PROMOTABLE | B_PA_ROYAL)) {
			strcat(s, "@");
			if (piece[fp].attr & B_PA_PLACABLE) {
				strcat(s, "H");
			}
			if (piece[fp].attr & B_PA_PROMOTABLE) {
				strcat(s, "P");
			}
			if (piece[fp].attr & B_PA_ROYAL) {
				strcat(s, "R");
			}
		}
		if (NOT(piece[fp].attr & B_PA_ROYAL)) {
			sprintf(s + strlen(s), "#%d", all_used_pc_count[fp]);
		}
		if (NEQUALS(graph[fp], piece_info[piece[fp].pc_no].graphv[0])) {
			strcat(strcat(s, ":"), graph[fp]);
		}
		// ★name
		// ★vecs
	}
}

// for debug
void
print_graph(void)
{
	int sq;

	for (sq = PC_FU; sq < ARRAY_SIZE_OF(graph); sq++) {
		if (graph[sq] != NULL) {
			printf("%03d %s\n", sq, graph[sq]);
		}
	}

	return;
}

void
print_board(phase_t *php)
{
	int i, j;

	printf("+---------------------------+\n");
	for (i = 1; i <= 9; i++) {
		printf("|");
		for (j = 90; j >= 10; j -= 10) {
			printf(" %02x", php->board[j + i]);
		}
		printf("|\n");
	}
	printf("+---------------------------+\n");
	printf("\n");

	return;
}

void
print_to_pos(int side, uchar_t pc)
{
	char *to;
	int pos, i;

	for (pos = 0; pos < 81; pos++) {
		printf("%d  ", pos);
		if (side == B_PC_HITHER) {
			to = hi_leap_to_pos[pc][pos];
		} else {
			to = yo_leap_to_pos[pc][pos];
		}
		for (i = 0; to[i] != EOP; i++) {
			printf("%d/", PR_POS(to[i]));
		}
		printf("\n");
	}
	printf("\n");

	return;
}

void
print_to_posv(int side, uchar_t pc)
{
	char **tov, *to;
	int pos, i, j;

	for (pos = 0; pos < 81; pos++) {
		printf("%d\n", pos);
		if (side == B_PC_HITHER) {
			tov = hi_run_to_posv[pc][pos];
		} else {
			tov = yo_run_to_posv[pc][pos];
		}
		for (i = 0; tov[i] != NULL; i++) {
			to = tov[i];
			for (j = 0; to[j] != EOP; j++) {
				printf("%d/", PR_POS(to[j]));
			}
			printf("\n");
		}
	}
	printf("\n");

	return;
}

void
print_to_board(int side, uchar_t pc, int from)
{
	uchar_t *tov;
	int pf, pr, pos;

	printf("%d\n", from);
	if (side == B_PC_HITHER) {
		tov = hi_to_board[pc][from];
	} else {
		tov = yo_to_board[pc][from];
	}
	if (tov == NULL) {
		printf("no data...\n");
		return;
	}
	for (pr = 0; pr <= 8; pr++) {
		for (pf = 8; pf >= 0; pf--) {
			pos = POS(pf, pr);
			if (tov[pos] == ON) {
				printf(" *");
			} else {
				printf(" .");
			}
		}
		printf("\n");
	}
	printf("\n");

	return;
}

void
print_to_board2(int side, uchar_t pc, int pos)
{
	int pr, pf;
	char board[81];

	memset(board, '.', sizeof(board));
	board[pos] = '#';
	if (piece[pc].move_style & BM_MS_LEAP_FAMILY) {
		char *to;
		int i;
		if (side == B_PC_HITHER) {
			to = hi_leap_to_pos[pc][pos];
		} else {
			to = yo_leap_to_pos[pc][pos];
		}
		if (to == NULL) {
			printf("to (leap) == NULL\n");
			return;
		}
		for (i = 0; to[i] != EOP; i++) {
			if (piece[pc].move_style & B_MS_LEAP) {
				// Leap
				board[to[i]] = 'L';
			} else {
				// 獅子Leap
				board[to[i]] = '@';
			}
		}
	}
	if (piece[pc].move_style & (BM_MS_EXRUN_FAMILY | B_MS_STEP)) {
		char **tov, *to;
		int i, j;
		if (side == B_PC_HITHER) {
			tov = hi_run_to_posv[pc][pos];
		} else {
			tov = yo_run_to_posv[pc][pos];
		}
		if (tov == NULL) {
			printf("tov (run) == NULL\n");
			return;
		}
		for (i = 0; tov[i] != NULL; i++) {
			to = tov[i];
			for (j = 0; to[j] != EOP; j++) {
				if (piece[pc].move_style & (B_MS_RUN | B_MS_CIRCLE_RUN)) {
					// Run or CircleRun
					board[to[j]] = 'R';
				} else if (piece[pc].move_style & B_MS_HOP) {
					// Hop
					board[to[j]] = 'H';
				} else if (piece[pc].move_style & B_MS_DECAPT_HOP) {
					// Decapt-Hop
					board[to[j]] = 'D';
				} else if (piece[pc].move_style & B_MS_PAO_JUMP) {
					// Pao-Move
					board[to[j]] = 'P';
				} else if (piece[pc].move_style & B_MS_JUMP) {
					// Jump
					board[to[j]] = 'J';
				} else if (piece[pc].move_style & B_MS_JUMP_HOP) {
					// Jump-2-Pieces
					board[to[j]] = '2';
				} else {
					// Step
					board[to[j]] = 'S';
				}
			}
		}
	}
	for (pr = 0; pr <= 8; pr++) {
		for (pf = 8; pf >= 0; pf--) {
			printf(" %c", board[POS(pf, pr)]);
		}
		printf("\n");
	}
	printf("\n");

	return;
}

static void
debug_print_pc_attrs(int pc)
{
	piece_t *pcp = &piece[pc];

	printf("pc=%02x/%s pc_no=%d name=%s\n", pc, graph[pc], pcp->pc_no, pcp->name);
	printf("attr=0x%08x move_style=0x%08x\n", pcp->attr, pcp->move_style);

	return;
}

void
check_piece_attrs(void)
{
	piece_t *pcp;
	uchar_t pc, bad_state = OFF;

	for (pc = PC_FU; pc < PC_END; pc++) {
		pcp = &piece[pc];
		if ((pcp->attr & B_PA_DEFINED) && (pcp->move_style & BM_MS_LEAP_FAMILY)) {
//			if (piece_info[pcp->pc_no].hi_leap_vec == NULL) {
//				bad_state = ON;
//				fprintf(stderr, "%sの先手移動先ベクトルが未設定です...\n", graph[pc]);
//			}
//			if (piece_info[pcp->pc_no].yo_leap_vec == NULL) {
//				bad_state = ON;
//				fprintf(stderr, "%sの後手移動先ベクトルが未設定です...\n", graph[pc]);
//			}
			if (hi_leap_to_pos[pc][0] == NULL) {
				bad_state = ON;
				fprintf(stderr, "%sの先手移動先が未設定です...\n", graph[pc]);
			}
			if (yo_leap_to_pos[pc][0] == NULL) {
				bad_state = ON;
				fprintf(stderr, "%sの後手移動先が未設定です...\n", graph[pc]);
			}
			if ((pcp->attr & B_PA_PLACABLE)) {
				if (hi_to_board[pc][0] == NULL) {
					bad_state = ON;
					fprintf(stderr, "%sの先手移動先盤面データが未設定です...\n", graph[pc]);
				}
				if (yo_to_board[pc][0] == NULL) {
					bad_state = ON;
					fprintf(stderr, "%sの後手移動先盤面データが未設定です...\n", graph[pc]);
				}
			}
		}
		if ((pcp->attr & B_PA_DEFINED) &&
				(pcp->move_style & BM_MS_EXRUN_FAMILY)) {
//			if (piece_info[piece_info[pcp->pc_no].pc_no].hi_run_vec == NULL) {
//				bad_state = ON;
//				fprintf(stderr, "%sの先手走り系移動先ベクトルが未設定です...\n", graph[pc]);
//			}
//			if (piece_info[piece_info[pcp->pc_no].pc_no].yo_run_vec == NULL) {
//				bad_state = ON;
//				fprintf(stderr, "%sの後手走り系移動先ベクトルが未設定です...\n", graph[pc]);
//			}
			if (hi_run_to_posv[pc][0] == NULL) {
				bad_state = ON;
				fprintf(stderr, "%sの先手走り系移動先が未設定です...\n", graph[pc]);
			}
			if (yo_run_to_posv[pc][0] == NULL) {
				bad_state = ON;
				fprintf(stderr, "%sの後手走り系移動先が未設定です...\n", graph[pc]);
			}
			if ((pcp->attr & B_PA_PLACABLE)) {
				if (hi_to_board[pc][0] == NULL) {
					bad_state = ON;
					fprintf(stderr, "%sの先手走り系移動先盤面データが未設定です...\n", graph[pc]);
				}
				if (yo_to_board[pc][0] == NULL) {
					bad_state = ON;
					fprintf(stderr, "%sの後手走り系移動先盤面データが未設定です...\n", graph[pc]);
				}
			}
		}
	}
	if (bad_state == ON) {
		exit(1);
	}

	return;
}

void
print_board_on(int my_pos)
{
	int pf, pr, pos;

	for (pr = 0; pr <= 8; pr++) {
		for (pf = 8; pf >= 0; pf--) {
			pos = POS(pf, pr);
			if (around_sq[my_pos][pos] == ON) {
				printf(" *");
			} else {
				printf(" .");
			}
		}
		printf("\n");
	}
}

#if 0
{
	uchar_t pc;
	for (pc = PC_FU; pc < PC_END; pc++) {
		piece_t *pcp = &piece[pc];
		if (pcp->attr & B_PA_DEFINED) {
			printf("%02x %s %08x\n", pc, graph[pc], pcp->attr);
		}
	}
}
printf("fp=%d pc=%d save=%s\n", fp, pc, save), exit(0);
	{
		int i;
		printf("initial leap_vecs:\n");
		for (i = 0; leap_vecs[i].file != 0 || leap_vecs[i].rank != 0; i++) {
			printf("<%d,%d,%d>\n", leap_vecs[i].file, leap_vecs[i].rank, leap_vecs[i].range);
		}
		printf("initial run_vecs:\n");
		for (i = 0; run_vecs[i].file != 0 || run_vecs[i].rank != 0; i++) {
			printf("<%d,%d,%d>\n", run_vecs[i].file, run_vecs[i].rank, run_vecs[i].range);
		}
	}
	{
		int i;
		printf("leap_vec:\n");
		for (i = 0; pip->hi_leap_vec[i].file != 0 || pip->hi_leap_vec[i].rank != 0; i++) {
			printf("<%d,%d,%d>\n", pip->hi_leap_vec[i].file, pip->hi_leap_vec[i].rank, pip->hi_leap_vec[i].range);
		}
		printf("run_vec:\n");
		for (i = 0; pip->hi_run_vec[i].file != 0 || pip->hi_run_vec[i].rank != 0; i++) {
			printf("<%d,%d,%d>\n", pip->hi_run_vec[i].file, pip->hi_run_vec[i].rank, pip->hi_run_vec[i].range);
		}
	}
#endif
