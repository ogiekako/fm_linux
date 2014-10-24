// 6aco/diff.c -- FM group-5 subroutines
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/diff.c 1.3 2014/08/14 14:17:01 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	"fm.h"
#include	"piece.h"

static ulong_t rand_num = 0;
static ulong_t *board_hash[81];
static ulong_t *hand_hash[RAW_PC_END];

static ulong_t get_rand_num(void);

// 検索局面削減：（弱）同一局面チェック（攻方手番）
int
is_hi_same_phase(phase_t *php)
{
	seq_phase_hash_t *sph_ent;
	escape_hash_t *eh_ent;
	ulong_t index;

	index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands;
	sph_ent = hi_sph_head[index & (NOSPHHEAD - 1)];
	while (sph_ent != NULL) {
		if (sph_ent->php->hash_hi_pieces == php->hash_hi_pieces &&
				sph_ent->php->hash_yo_pieces == php->hash_yo_pieces &&
				sph_ent->php->hash_hi_hands == php->hash_hi_hands) {
			(sph_ent->php - 1)->move.flag |= B_MV_SAME;
			return 2;
		}
		sph_ent = sph_ent->next;
	}
	if (strategy & B_HI_ESCAPE) {
		eh_ent = hi_eh_head[index & (no_hi_eh_head - 1)];
		while (eh_ent != NULL) {
			if (eh_ent->depth <= depth &&
					eh_ent->hi_pieces == php->hash_hi_pieces &&
					eh_ent->yo_pieces == php->hash_yo_pieces &&
					eh_ent->hi_hands == php->hash_hi_hands) {
				if (eh_ent->ref_count < 0xfffe) {
					eh_ent->ref_count++;
				}
				return 1;
			}
			eh_ent = eh_ent->next;
		}
	}
	sph_cell[depth].next = hi_sph_head[index & (NOSPHHEAD - 1)];
	sph_cell[depth].php = php;
	hi_sph_head[index & (NOSPHHEAD - 1)] = &sph_cell[depth];

	return 0;
}

// 検索局面削減：（弱）同一局面チェック（受方手番）
int
is_yo_same_phase(phase_t *php)
{
	seq_phase_hash_t *sph_ent;
	escape_hash_t *eh_ent;
	ulong_t index;

	index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands;
	sph_ent = yo_sph_head[index & (NOSPHHEAD - 1)];
	while (sph_ent != NULL) {
		if (sph_ent->php->hash_hi_pieces == php->hash_hi_pieces &&
				sph_ent->php->hash_yo_pieces == php->hash_yo_pieces &&
				sph_ent->php->hash_hi_hands == php->hash_hi_hands) {
			(sph_ent->php - 1)->move.flag |= B_MV_SAME;
			return 2;
		}
		sph_ent = sph_ent->next;
	}
	if (strategy & B_YO_ESCAPE) {
		eh_ent = yo_eh_head[index & (no_yo_eh_head - 1)];
		while (eh_ent != NULL) {
			if (eh_ent->depth <= depth &&
					eh_ent->hi_pieces == php->hash_hi_pieces &&
					eh_ent->yo_pieces == php->hash_yo_pieces &&
					eh_ent->hi_hands == php->hash_hi_hands) {
				if (eh_ent->ref_count < 0xfffe) {
					eh_ent->ref_count++;
				}
				return 1;
			}
			eh_ent = eh_ent->next;
		}
	}
	sph_cell[depth].next = yo_sph_head[index & (NOSPHHEAD - 1)];
	sph_cell[depth].php = php;
	yo_sph_head[index & (NOSPHHEAD - 1)] = &sph_cell[depth];

	return 0;
}

void
set_hi_eh_cell(phase_t *php)
{
	escape_hash_t *ent;
	ulong_t index;
	char work[32];

	index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands;
	index &= no_hi_eh_head - 1;
	ent = hi_eh_head[index];
	while (ent != NULL) {
		if (ent->hi_pieces == php->hash_hi_pieces && ent->yo_pieces == php->hash_yo_pieces &&
				ent->hi_hands == php->hash_hi_hands) {
			if (ent->depth > depth) {
				ent->depth = depth;
			}
			return;
		}
		ent = ent->next;
	}

	if (hi_eh_free == NULL) {
		int i, j = 1, n = 0;
	retry:
		for (i = 0; i < no_hi_eh_head; i++) {
			ent = (escape_hash_t *) &hi_eh_head[i];
			while (ent->next != NULL) {
				if (ent->next->ref_count <= 1) {
					escape_hash_t *ent2 = ent->next;
					ent->next = ent2->next;
					ent2->next = hi_eh_free;
					ent2->ref_count = 0;
					hi_eh_free = ent2;
					n++;
				} else {
					ent->next->ref_count--;
					ent = ent->next;
				}
			}
		}
		if (n < yo_eh_lim / 10 * 3) {
			j++;
			goto retry;
		}
		ERASE_EOS();
		printf("*** %s個の MHE テーブル（参照回数 %d以下）を再使用します...\n\n",
				make_num_str(0, n, work), j);
		total_elapse = time((time_t *)NULL) - start_time;
		while (phase_count >= 1000000) {
			phase_count -= 1000000;
			phase_1M++;
			check_point -= 1000000;
		}
		print_record(stdout);
	}
	ent = hi_eh_free;
	hi_eh_free = hi_eh_free->next;
	ent->depth = depth;
	ent->ref_count = 1;
	ent->hi_pieces = php->hash_hi_pieces;
	ent->yo_pieces = php->hash_yo_pieces;
	ent->hi_hands = php->hash_hi_hands;
	ent->next = hi_eh_head[index];
	hi_eh_head[index] = ent;

	return;
}

void
set_yo_eh_cell(phase_t *php)
{
	escape_hash_t *ent;
	ulong_t index;
	char work[32];

	index = php->hash_hi_pieces ^ php->hash_yo_pieces ^ php->hash_hi_hands;
	index &= no_yo_eh_head - 1;
	ent = yo_eh_head[index];
	while (ent != NULL) {
		if (ent->hi_pieces == php->hash_hi_pieces && ent->yo_pieces == php->hash_yo_pieces &&
				ent->hi_hands == php->hash_hi_hands) {
			if (ent->depth > depth) {
				ent->depth = depth;
			}
			return;
		}
		ent = ent->next;
	}
	if (yo_eh_free == NULL) {
		int i, j = 1, n = 0;
	retry:
		for (i = 0; i < no_yo_eh_head; i++) {
			ent = (escape_hash_t *) &yo_eh_head[i];
			while (ent->next != NULL) {
				if (ent->next->ref_count <= 1) {
					escape_hash_t *ent2 = ent->next;
					ent->next = ent2->next;
					ent2->next = yo_eh_free;
					ent2->ref_count = 0;
					yo_eh_free = ent2;
					n++;
				} else {
					ent->next->ref_count--;
					ent = ent->next;
				}
			}
		}
		if (n < yo_eh_lim / 10 * 3) {
			j++;
			goto retry;
		}
		ERASE_EOS();
		printf("*** %s個の MYE テーブル（参照回数 %d以下）を再使用します...\n\n",
				make_num_str(0, n, work), j);
		total_elapse = time((time_t *)NULL) - start_time;
		while (phase_count >= 1000000) {
			phase_count -= 1000000;
			phase_1M++;
			check_point -= 1000000;
		}
		print_record(stdout);
	}
	ent = yo_eh_free;
	yo_eh_free = yo_eh_free->next;
	ent->depth = depth;
	ent->ref_count = 1;
	ent->hi_pieces = php->hash_hi_pieces;
	ent->yo_pieces = php->hash_yo_pieces;
	ent->hi_hands = php->hash_hi_hands;
	ent->next = yo_eh_head[index];
	yo_eh_head[index] = ent;

	return;
}

void
hash_init(phase_t *php)
{
	int pos;
	uchar_t bpc;

	php->hash_hi_hands = hand_hash[PC_FU][php->hi_hand[PC_FU]] ^
						 hand_hash[PC_KY][php->hi_hand[PC_KY]] ^
						 hand_hash[PC_KE][php->hi_hand[PC_KE]] ^
						 hand_hash[PC_GI][php->hi_hand[PC_GI]] ^
						 hand_hash[PC_KI][php->hi_hand[PC_KI]] ^
						 hand_hash[PC_KA][php->hi_hand[PC_KA]] ^
						 hand_hash[PC_HI][php->hi_hand[PC_HI]] ^
						 hand_hash[PC_F1][php->hi_hand[PC_F1]] ^
						 hand_hash[PC_F2][php->hi_hand[PC_F2]] ^
						 hand_hash[PC_F3][php->hi_hand[PC_F3]] ^
						 hand_hash[PC_F4][php->hi_hand[PC_F4]] ^
						 hand_hash[PC_F5][php->hi_hand[PC_F5]] ^
						 hand_hash[PC_F6][php->hi_hand[PC_F6]] ^
						 hand_hash[PC_F7][php->hi_hand[PC_F7]];
	php->hash_hi_pieces = php->hash_yo_pieces = 0;
	for  (pos = 0; pos < 81; pos++) {
		bpc = php->board[pos];
		if (IS_HI_PC(bpc)) {
			php->hash_hi_pieces ^= board_hash[pos][KIND(bpc)];
		} else if (IS_YO_PC(bpc)) {
			php->hash_yo_pieces ^= board_hash[pos][KIND(bpc)];
		}
	}

	return;
}

void
hi_hash_diff(phase_t *php)
{
	phase_t *php_1 = php - 1;
	int from, to, n;
	uchar_t pc;

	if (depth == 1) {
		return;
	}
	from = php_1->move.from;
	to = php_1->move.to;
	if (from == NOP) {
		// 駒打ち
		pc = php_1->move.pc;
		php->hash_hi_pieces ^= board_hash[to][pc];
		n = php->hi_hand[pc];
		php->hash_hi_hands ^= hand_hash[pc][n + 1] ^ hand_hash[pc][n];
	} else {
		// 駒移動
		hi_hash_diff_sub(php, php_1, from, to);
	}

	return;
}

void
yo_hash_diff(phase_t *php)
{
	phase_t *php_1 = php - 1;
	int from, to;
	uchar_t pc;

	if (depth == 1) {
		return;
	}
	from = php_1->move.from;
	to = php_1->move.to;
	if (from == NOP) {
		// 駒打ち
		pc = php_1->move.pc;
		php->hash_yo_pieces ^= board_hash[to][pc];
	} else {
		// 駒移動
		yo_hash_diff_sub(php, php_1, from, to);
	}

	return;
}

void
make_hash_tab(void)
{
	ulong_t *hasha;
	int pos, bhx, i;
	uchar_t pc;

	bhx = 1;
	for (pc = 0; pc < RAW_PC_END; pc++) {
		bhx += null_phase.yo_hand[pc] + 1;
	}
	bhx += PC_END * 81;
	hasha = calloc(bhx, sizeof(board_hash[0][0]));
	if (hasha == NULL) {
		fm_exit_mae("ハッシュ盤面領域");
	}
	board_hash[0] = hasha;
	hasha++;
	for (pos = 0; pos < 81; pos++) {
		board_hash[pos] = hasha;
		hasha += PC_END;
		for (pc = 0; pc < PC_END; pc++) {
			board_hash[pos][pc] = get_rand_num();
		}
	}
	for (pc = PC_FU; pc < RAW_PC_END; pc++) {
		hand_hash[pc] = hasha;
		hasha += null_phase.yo_hand[pc] + 1;
		for (i = 0; i <= null_phase.yo_hand[pc]; i++) {
			hand_hash[pc][i] = get_rand_num();
		}
	}

	return;
}

static ulong_t
get_rand_num(void)
{
	int i;
	static int col = 0;
	static char rand_digit[12] = { '\0', };

	if (rand_digit[col] == '\0') {
		rand_num = rand_num * 504542181 + 453816693;
		sprintf(rand_digit, "%d", rand_num);
		col = 0;
	}
	for (i = rand_digit[col] - '0'; i >= 0; i--) {
		rand_num = rand_num * 504542181 + 453816693;
	}
	col++;

	return rand_num;
}

void
hi_hash_diff_sub_normal(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, n;
	uchar_t pc0, pc, capt;

	pc0 = KIND(php_1->board[from]);
	pc = KIND(php->board[to]);
	php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_yo_pieces ^= board_hash[to1][capt];
			capt = RAW_KIND(capt);
			if (piece[capt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[capt];
				php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
			}
		}
	}
	capt = KIND(php_1->board[to]);
	if (NOT_ROOM(capt)) {
		// 駒捕獲あり
		php->hash_yo_pieces ^= board_hash[to][capt];
		capt = RAW_KIND(capt);
		if (piece[capt].attr & B_PA_REUSABLE) {
			n = php->hi_hand[capt];
			php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
		}
	}

	return;
}

void
yo_hash_diff_sub_normal(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1;
	uchar_t pc0, pc, capt;

	pc0 = KIND(php_1->board[from]);
	pc = KIND(php->board[to]);
	php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			php->hash_hi_pieces ^= board_hash[to1][capt];
		}
	}
	capt = KIND(php_1->board[to]);
	if (NOT_ROOM(capt)) {
		// 駒捕獲あり
		php->hash_hi_pieces ^= board_hash[to][capt];
	}

	return;
}

void
hi_hash_diff_sub_pwc(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, n;
	uchar_t pc0, pc, capt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		pc = KIND(php_1->board[from]);
		php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				php->hash_yo_pieces ^= board_hash[to1][capt] ^ board_hash[from][capt];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[to1][capt];
				capt = RAW_KIND(capt);
				if (piece[capt].attr & B_PA_REUSABLE) {
					n = php->hi_hand[capt];
					php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
				}
			}
		}
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				php->hash_yo_pieces ^= board_hash[to][capt] ^ board_hash[to1][capt];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[to][capt];
				capt = RAW_KIND(capt);
				if (piece[capt].attr & B_PA_REUSABLE) {
					n = php->hi_hand[capt];
					php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
				}
			}
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				php->hash_yo_pieces ^= board_hash[to][capt] ^ board_hash[from][capt];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[to][capt];
				capt = RAW_KIND(capt);
				if (piece[capt].attr & B_PA_REUSABLE) {
					n = php->hi_hand[capt];
					php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
				}
			}
		}
	}

	return;
}

void
yo_hash_diff_sub_pwc(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1;
	uchar_t pc0, pc, capt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		pc = KIND(php_1->board[from]);
		php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				php->hash_hi_pieces ^= board_hash[to1][capt] ^ board_hash[from][capt];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[to1][capt];
			}
		}
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				php->hash_hi_pieces ^= board_hash[to][capt] ^ board_hash[to1][capt];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[to][capt];
			}
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				php->hash_hi_pieces ^= board_hash[to][capt] ^ board_hash[from][capt];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[to][capt];
			}
		}
	}

	return;
}

void
hi_hash_diff_sub_circe(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, rbn, n;
	uchar_t pc0, pc, capt, rcapt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		pc = KIND(php_1->board[from]);
		php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		// 蝗による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			rcapt = RAW_KIND(capt);
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				rbn = php_1->move.reborn_1st;
				php->hash_yo_pieces ^= board_hash[to1][capt] ^ board_hash[rbn][rcapt];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[to1][capt];
				if (piece[rcapt].attr & B_PA_REUSABLE) {
					n = php->hi_hand[rcapt];
					php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
				}
			}
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			rcapt = RAW_KIND(capt);
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				rbn = php_1->move.reborn;
				php->hash_yo_pieces ^= board_hash[to][capt] ^ board_hash[rbn][rcapt];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[to][capt];
				if (piece[rcapt].attr & B_PA_REUSABLE) {
					n = php->hi_hand[rcapt];
					php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
				}
			}
		}
	}

	return;
}

void
yo_hash_diff_sub_circe(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, rbn;
	uchar_t pc0, pc, capt, rcapt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		pc = KIND(php_1->board[from]);
		php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		// 蝗による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				rbn = php_1->move.reborn_1st;
				rcapt = RAW_KIND(capt);
				php->hash_hi_pieces ^= board_hash[to1][capt] ^ board_hash[rbn][rcapt];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[to1][capt];
			}
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				rbn = php_1->move.reborn;
				rcapt = RAW_KIND(capt);
				php->hash_hi_pieces ^= board_hash[to][capt] ^ board_hash[rbn][rcapt];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[to][capt];
			}
		}
	}

	return;
}

void
hi_hash_diff_sub_anticirce(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, rbn, n;
	uchar_t pc0, pc, capt, rcapt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 蝗による駒捕獲あり
		pc = KIND(php_1->board[from]);
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				rbn = php_1->move.reborn_1st;
				php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[rbn][pc];
			} else {
				// 再生なし
				php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
			}
			php->hash_yo_pieces ^= board_hash[to1][capt];
			rcapt = RAW_KIND(capt);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[rcapt];
				php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
			}
		} else {
			// 駒捕獲なし
			php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				rbn = php_1->move.reborn;
				pc = KIND(php->board[rbn]);
				php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[rbn][pc];
			} else {
				// 再生なし
				pc = KIND(php->board[to]);
				php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
			}
			php->hash_yo_pieces ^= board_hash[to][capt];
			rcapt = RAW_KIND(capt);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[rcapt];
				php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
			}
		} else {
			// 駒捕獲なし
			pc = KIND(php->board[to]);
			php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		}
	}

	return;
}

void
yo_hash_diff_sub_anticirce(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, rbn;
	uchar_t pc0, pc, capt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 蝗による駒捕獲あり
		pc = KIND(php_1->board[from]);
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN_1ST) {
				// 再生あり
				rbn = php_1->move.reborn_1st;
				php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[rbn][pc];
			} else {
				// 再生なし
				php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
			}
			php->hash_hi_pieces ^= board_hash[to1][capt];
		} else {
			// 駒捕獲なし
			php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			if (php_1->move.flag2 & B_MV2_REBORN) {
				// 再生あり
				rbn = php_1->move.reborn;
				pc = KIND(php->board[rbn]);
				php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[rbn][pc];
			} else {
				// 再生なし
				pc = KIND(php->board[to]);
				php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
			}
			php->hash_hi_pieces ^= board_hash[to][capt];
		} else {
			// 駒捕獲なし
			pc = KIND(php->board[to]);
			php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		}
	}

	return;
}

void
hi_hash_diff_sub_andernach(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, n;
	uchar_t pc0, pc, capt, rcapt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 蝗による駒捕獲あり
		pc = KIND(php_1->board[from]);
		// 駒捕獲あり
		if (php_1->move.flag2 & B_MV2_TURN) {
			// 反転あり
			php->hash_hi_pieces ^= board_hash[from][pc];
			php->hash_yo_pieces ^= board_hash[to][pc];
		} else {
			// 反転なし
			php->hash_hi_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		}
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			php->hash_yo_pieces ^= board_hash[to1][capt];
			rcapt = RAW_KIND(capt);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[rcapt];
				php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
			}
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		if (php_1->move.flag2 & B_MV2_TURN) {
			// 反転あり
			php->hash_hi_pieces ^= board_hash[from][pc0];
			php->hash_yo_pieces ^= board_hash[to][pc];
		} else {
			// 反転なし
			php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		}
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_yo_pieces ^= board_hash[to][capt];
			rcapt = RAW_KIND(capt);
			if (piece[rcapt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[rcapt];
				php->hash_hi_hands ^= hand_hash[rcapt][n - 1] ^ hand_hash[rcapt][n];
			}
		}
	}

	return;
}

void
yo_hash_diff_sub_andernach(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1;
	uchar_t pc0, pc, capt;

	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 蝗による駒捕獲あり
		pc = KIND(php_1->board[from]);
		// 駒捕獲あり
		if (php_1->move.flag2 & B_MV2_TURN) {
			// 反転あり
			php->hash_yo_pieces ^= board_hash[from][pc];
			php->hash_hi_pieces ^= board_hash[to][pc];
		} else {
			// 反転なし
			php->hash_yo_pieces ^= board_hash[from][pc] ^ board_hash[to][pc];
		}
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			php->hash_hi_pieces ^= board_hash[to1][capt];
		}
	} else {
		pc0 = KIND(php_1->board[from]);
		pc = KIND(php->board[to]);
		if (php_1->move.flag2 & B_MV2_TURN) {
			// 反転あり
			php->hash_yo_pieces ^= board_hash[from][pc0];
			php->hash_hi_pieces ^= board_hash[to][pc];
		} else {
			// 反転なし
			php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
		}
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_hi_pieces ^= board_hash[to][capt];
		}
	}

	return;
}

void
hi_hash_diff_sub_exchange(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1, n;
	uchar_t pc0, pc, capt;

	pc0 = KIND(php_1->board[from]);
	pc = KIND(php->board[to]);
	php->hash_hi_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_yo_pieces ^= board_hash[to1][capt];
			capt = RAW_KIND(capt);
			if (piece[capt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[capt];
				php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
			}
		}
	}
	if (php_1->move.flag2 & B_MV2_EXCHANGE) {
		php->hash_yo_pieces ^= board_hash[to][pc0] ^ board_hash[from][pc0];
	} else {
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_yo_pieces ^= board_hash[to][capt];
			capt = RAW_KIND(capt);
			if (piece[capt].attr & B_PA_REUSABLE) {
				n = php->hi_hand[capt];
				php->hash_hi_hands ^= hand_hash[capt][n - 1] ^ hand_hash[capt][n];
			}
		}
	}

	return;
}

void
yo_hash_diff_sub_exchange(phase_t *php, phase_t *php_1, int from, int to)
{
	int to1;
	uchar_t pc0, pc, capt;

	pc0 = KIND(php_1->board[from]);
	pc = KIND(php->board[to]);
	php->hash_yo_pieces ^= board_hash[from][pc0] ^ board_hash[to][pc];
	to1 = php_1->move.to_1st;
	if (to1 != NOP) { 
		// 獅子、蝗等による駒捕獲あり
		capt = KIND(php_1->board[to1]);
		if (NOT_ROOM(capt)) {
			php->hash_hi_pieces ^= board_hash[to1][capt];
		}
	}
	if (php_1->move.flag2 & B_MV2_EXCHANGE) {
		php->hash_hi_pieces ^= board_hash[to][pc0] ^ board_hash[from][pc0];
	} else {
		capt = KIND(php_1->board[to]);
		if (NOT_ROOM(capt)) {
			// 駒捕獲あり
			php->hash_hi_pieces ^= board_hash[to][capt];
		}
	}

	return;
}

