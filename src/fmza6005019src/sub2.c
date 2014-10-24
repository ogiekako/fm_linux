// 6aco/sub2.c -- FM group-2 subroutines
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/sub2.c 1.19 2014/08/14 02:50:48 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<conio.h>
#include	"fm.h"
#include	"piece.h"

static int dance_timing = 0;

static int save_phase(int prompt);
static void in_key_check(void);
static void save_direct_mate(void);
static void set_mate_flag(mate_move_t *mmp, int d);

void
signal_exit(int sig)
{
	time_t now_time;

	signal(SIGINT, SIG_IGN);

	now_time = time((time_t *)NULL);
	total_elapse = now_time - start_time;

	sig += 6;	// SIGALRM(=1) -> 7, SIGINT(=2) -> 8, SIGQUIT(=3) -> 9
	switch (sig) {
	case 7:
		fprintf(stderr, "\n");
		break;
	}

	if (phase_1M == 0 && phase_count <= 0) {
		fprintf(stderr, "%s: シグナル受信 - %d\n", cmdname, sig);
		fm_exit(9);
	}

	while (phase_count >= 1000000) {
		phase_count -= 1000000;
		phase_1M++;
	}

	printf("\n");
	ERASE_EOS();

	if (fileFp != NULL && brief_output == 0) {
		fputs("中断\n\n", fileFp);
		print_last_moves(&phase[depth], 0, fileFp);
		putc('\n', fileFp);
		print_record(fileFp);
		fputs("\n\n", fileFp);
	}
	printf("中断\n\n");
	print_last_moves(&phase[depth], 0, stdout);
	printf("\n");
	print_record(stdout);
	printf("\n\n");

	if (save_phase(ON) == 9) {
		sig = 9;
	}

	fprintf(stderr, "%s: シグナル受信 - %d\n", cmdname, sig);
	fm_exit(9);
}

static int
save_phase(int prompt)
{
	FILE *saveFp;
	char *s;
	phase_t *php;
	time_t now_time = time((time_t *)NULL);
	extern void make_FP_E_info(char *s);

	total_elapse = now_time - start_time;

	printf("待避ファイル：%s\n\n", restart_file);
	if (fileFp != NULL && brief_output == 0) {
		fprintf(fileFp, "待避ファイル：%s\n\n", restart_file);
	}

	saveFp = fopen(restart_file, "w");
	if (saveFp == NULL) {
		fprintf(stderr, "\n\n%s: 待避ファイル %s がオープンできません...\n\n",
				cmdname, restart_file);
		fm_exit(2);
	}
	setvbuf(saveFp, NULL, _IOFBF, 0x40000);

	fprintf(saveFp, ";%s %s - %s", cmdname, release, ctime(&now_time));
	fprintf(saveFp, "/I%s\n", in_file);
	if (fileFp != NULL) {
		fprintf(saveFp, "/O%s\n", out_file);
	}
	print_option_L(saveFp);
	print_option_M(saveFp);
	print_option_B(saveFp);
	print_option_X(saveFp);
	make_FP_E_str(inbuf);
	if (NEQUAL(inbuf, "/E")) {
		fprintf(saveFp, "%s\n", inbuf);
	}
	if (title[0] != '\0') {
		fprintf(saveFp, "『%s』\n", title);
	}
	s = inbuf;
	reg_prob(&phase[1], &phase[0], s, -1);
	fputs(s, saveFp);
	print_record(saveFp);
	fputc('\n', saveFp);
	print_last_moves(&phase[depth], 2, saveFp);
	// 以下、バイナリデータ退避
	freopen(restart_file, "ab", saveFp);
	fputc('\x1a', saveFp);
	// 最終検索手順退避
	fwrite("#MV#", 1, STR_SIZE_OF("#MV#"), saveFp);
	fwrite(&depth, 1, sizeof(ushort_t), saveFp);
	for (php = &phase[1]; php < &phase[depth]; php++) {
		fwrite(&php->move, 1, sizeof(move_t), saveFp);
	}
	fwrite("#MV#", 1, STR_SIZE_OF("#MV#"), saveFp);
	if (strategy & B_HI_ESCAPE) {
		// 攻方逃れ局面退避
		escape_hash_t *ent;
		long i;
		ushort_t eoq = 0;
		fwrite("#HE#", 1, STR_SIZE_OF("#HE#"), saveFp);
		for (i = 0; i < no_hi_eh_head; i++) {
			if (hi_eh_head[i] != NULL) {
				fwrite(&i, 1, sizeof(i), saveFp);
				ent = hi_eh_head[i];
				while (ent != NULL) {
					fwrite(&ent->depth, 1, sizeof(ushort_t), saveFp);
					fwrite(&ent->hi_pieces, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->yo_pieces, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->hi_hands, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->ref_count, 1, sizeof(ushort_t), saveFp);
					ent = ent->next;
				}
				fwrite(&eoq, 1, sizeof(ushort_t), saveFp);
			}
		}
		fwrite("#HE#", 1, STR_SIZE_OF("#HE#"), saveFp);
	}
	if (strategy & B_YO_ESCAPE) {
		// 受方逃れ局面退避
		escape_hash_t *ent;
		long i;
		ushort_t eoq = 0;
		fwrite("#YE#", 1, STR_SIZE_OF("#YE#"), saveFp);
		for (i = 0; i < no_yo_eh_head; i++) {
			if (yo_eh_head[i] != NULL) {
				fwrite(&i, 1, sizeof(i), saveFp);
				ent = yo_eh_head[i];
				while (ent != NULL) {
					fwrite(&ent->depth, 1, sizeof(ushort_t), saveFp);
					fwrite(&ent->hi_pieces, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->yo_pieces, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->hi_hands, 1, sizeof(ulong_t), saveFp);
					fwrite(&ent->ref_count, 1, sizeof(ushort_t), saveFp);
					ent = ent->next;
				}
				fwrite(&eoq, 1, sizeof(ushort_t), saveFp);
			}
		}
		fwrite("#YE#", 1, STR_SIZE_OF("#YE#"), saveFp);
	}

	fclose(saveFp);

	return 0;
}

void
mile_stone(void)
{
	time_t now_time;

	now_time = time((time_t *)NULL);
	total_elapse = now_time - start_time;
	while (phase_count >= 1000000) {
		phase_count -= 1000000;
		phase_1M++;
		check_point -= 1000000;
	}
	check_point += check_interval;
	in_key_check();
	if (total_elapse - last_elapse > 2) {
		check_interval -= 10000;
		check_point -= 10000;
	} else if (total_elapse - last_elapse < 2) {
		check_interval += 10000;
		check_point += 10000;
	}
	CARRIAGE_RETURN();
	if (dancing == ON) {
		dance_timing++;
		if (dance_timing >= DANCE_TIMING) {
			dance_timing = 0;
			phase[0] = phase[depth];
			print_2nd_phase = ON;
			print_phase(&phase[1], stdout);
			print_2nd_phase = OFF;
			print_last_moves(&phase[depth], 1, stdout);
			SPACE();
		}
	}
	print_record(stdout);
	last_elapse = total_elapse;

	return;
}

int
help_mate_rule_check(phase_t *php)
{
	phase_t *php_1 = php - 1;
	move_t *mvp, *mvp_1;

	mvp_1 = &php_1->move;
	if (cond_flag & B_C_POSITION) {
		if (mate_position > 0) {
			if (IS_HI_TURN(php)) {
				if(php->hi_ou_pos != mate_position) {
					return RC_ESCAPE;
				}
			} else {
				if (php->yo_gy_pos != mate_position) {
					return RC_ESCAPE;
				}
			}
		}
		if (mate_counter_position != NOP) {
			if (IS_HI_TURN(php)) {
				if (php->yo_gy_pos != mate_counter_position) {
					return RC_ESCAPE;
				}
			} else {
				if(php->hi_ou_pos != mate_counter_position) {
					return RC_ESCAPE;
				}
			}
		}
	}
	if (NOT(rule_flag & (B_R_HELP_STALEMATE | B_R_HELP_SELF_STALEMATE)) &&
			NOT(NOT(cond_flag & B_C_UCHIFU) &&
				(NOT(piece[mvp_1->pc].attr & B_PA_CONSTRAINT_PM) ||
				 mvp_1->from != NOP ||
				 (cond_flag & B_C_UCHIFU_OK))) &&
			NOT((cond_flag & B_C_UCHIFU) &&
				((mvp_1->from == NOP && (piece[mvp_1->pc].attr & B_PA_CONSTRAINT_PM)) ||
				 limit_depth == 0))) {
		mvp_1->flag |= B_MV_MATESEQ;	// 同一局面チェック用に保存しないように
		return RC_ESCAPE;
	}

	solution_count++;
	mvp = &php->move;
	mvp->flag |= B_MV_MATE;
	if (trace_limit > 0) {
		int e = (depth - 1) % 10 + 1;
		if (fileFp != NULL) {
			if (e == 1) {
				fprintf(fileFp, "%2d: *** 詰 ***\n", depth);
				fflush(fileFp);
			} else {
				fprintf(fileFp, "%*c%2d: *** 詰 ***\n", ((e - 1) * 6), ' ', depth);
				fflush(fileFp);
			  }
		} else {
			CLEAR_LINE();
			if (e == 1) {
				printf("%2d: *** 詰 ***\n", depth);
			} else {
				printf("%*c%2d: *** 詰 ***\n", ((e - 1) * 6), ' ', depth);
			}
		}
	} else {
		total_elapse = time((time_t *)NULL) - start_time;
		phase[0] = *php;	// save mate phase
		phase[0].move.from = NOP;
		phase[0].move.to = NOP;
		while (phase_count >= 1000000) {
			phase_count -= 1000000;
			phase_1M++;
			check_point -= 1000000;
		}
		if (fileFp != NULL) {
			if (brief_output >= 1) {
				char *s = inbuf;
				reg_prob(&phase[1], &phase[0], s, -1);
				fputs(s, fileFp);
			}
			if (brief_output == 0) {
				print_record(fileFp);
				fputs("\n\n", fileFp);
			}
			print_mate(php, fileFp);
			fflush(fileFp);
		}
		ERASE_EOS();
		if (mate_print == ON && brief_output == 0) {
			print_record(stdout);
			printf("\n\n");
		}
		print_mate(php, stdout);
		if (solution_count >= solution_limit) {
			// 解数上限
			fprintf(stderr, "解数が指定された上限に達しました...\n\n");
		} else {
			if (mate_print == ON && brief_output == 0) {
				print_record(stdout);
			}
		}
	}
	dancing = OFF;
	if (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME)) {
		int i;
		for (i = 1; &phase[i] < php; i++) {
			phase[i].move.flag |= B_MV_MATESEQ;
		}
	}

	return RC_MATE;
}

int
direct_mate_rule_check(phase_t *php)
{
	phase_t *php_1 = php - 1;
	move_t *mvp, *mvp_1;

	mvp_1 = &php_1->move;
	if (cond_flag & B_C_POSITION) {
		if (mate_position > 0) {
			if (IS_HI_TURN(php)) {
				if(php->hi_ou_pos != mate_position) {
					return RC_ESCAPE;
				}
			} else {
				if (php->yo_gy_pos != mate_position) {
					return RC_ESCAPE;
				}
			}
		}
		if (mate_counter_position != NOP) {
			if (IS_HI_TURN(php)) {
				if (php->yo_gy_pos != mate_counter_position) {
					return RC_ESCAPE;
				}
			} else {
				if(php->hi_ou_pos != mate_counter_position) {
					return RC_ESCAPE;
				}
			}
		}
	}
	if (NOT(rule_flag & B_R_HELP_SELF_STALEMATE) &&
			NOT(NOT(cond_flag & B_C_UCHIFU) && (mvp_1->pc != PC_FU || mvp_1->from != NOP ||
				(cond_flag & B_C_UCHIFU_OK))) &&
			NOT((cond_flag & B_C_UCHIFU) && (mvp_1->pc == PC_FU && mvp_1->from == NOP) ||
				limit_depth == 0)) {
		mvp_1->flag |= B_MV_MATESEQ;	// 同一局面チェック用に保存しないように
		return RC_ESCAPE;
	}

	mvp = &php->move;
	mvp->flag |= B_MV_MATE;
	if (trace_limit > 0) {
		int e = (depth - 1) % 10 + 1;
		if (fileFp != NULL) {
			if (e == 1) {
				fprintf(fileFp, "%2d: *** 詰 ***\n", depth);
				fflush(fileFp);
			} else {
				fprintf(fileFp, "%*c%2d: *** 詰 ***\n", ((e - 1) * 6), ' ', depth);
				fflush(fileFp);
			  }
		} else {
			CLEAR_LINE();
			if (e == 1) {
				printf("%2d: *** 詰 ***\n", depth);
			} else {
				printf("%*c%2d: *** 詰 ***\n", ((e - 1) * 6), ' ', depth);
			}
		}
	} else {
		save_direct_mate();
	}
	if (strategy & (B_HI_DENY_SAME | B_YO_DENY_SAME)) {
		int i;
		for (i = 1; &phase[i] < php; i++) {
			phase[i].move.flag |= B_MV_MATESEQ;
		}
	}

	return RC_MATE;
}

int
trace_move(phase_t *php)
{
	int d = depth - 1, e;
	char move_buf[32];

	if (d <= trace_limit) {
		make_move((php - 1), move_buf);
		e = (d - 1) % 10 + 1;
		if (fileFp != NULL) {
			if (e == 1) {
				fprintf(fileFp, "%2d: %s\n", d, move_buf);
				fflush(fileFp);
			} else {
				fprintf(fileFp, "%*c%2d: %s\n", ((e - 1) * 6), ' ', d, move_buf);
				fflush(fileFp);
			}
		} else {
			CLEAR_LINE();
			if (e == 1) {
				printf("%2d: %s\n", d, move_buf);
			} else {
				printf("%*c%2d: %s\n", ((e - 1) * 6), ' ', d, move_buf);
			}
		}
		if (d == trace_limit && ((trace_limit ^ limit_depth) & 0x1)) {
			return 1;
		}
	} else {
		if (d > (trace_limit + 1)) {
			return 1;
		}
	}

	return 0;
}

static void
in_key_check(void)
{
	int c;

	while (kbhit() != 0) {
		c = getch();
		switch (c) {
		case 0x03:	// ^C (Cancel a problem)
			fprintf(stderr, "^C\n");
			while (kbhit() != 0) {
				getch();
			}
			signal_exit(SIGINT);
			break;
		case 0x11:	// ^Q (Quit fm)
			fprintf(stderr, "^Q\n");
			while (kbhit() != 0) {
				getch();
			}
			signal_exit(SIGQUIT);
			break;
		case 'D':	// Dancing (toggle)
		case 'd':
			if (dancing == ON) {
				dancing = OFF;
			} else {
				dancing = ON;
				dance_timing = DANCE_TIMING;
			}
			break;
		}
	}

	return;
}

int
test_symmetric_normal(phase_t *php)
{
	int last_to = (php - 1)->move.to;
	int last_from = (php - 1)->move.from;

//	if (last_to >= POS19(6, 1) || (last_to >= POS19(5, 1) && last_from >= POS19(6, 1))) {
	if (last_to >= POS(5, 0) || (last_to >= POS(4, 0) && last_from >= POS(5, 0))) {
		return 0;
	}
//	if (POS19_FILE(last_to) == 5 && (last_from == NOP || POS_FILE19(last_from) == 5)) {
	if (POS_FILE(last_to) == 4 && (last_from == NOP || POS_FILE(last_from) == 4)) {
		php->state |= B_SYMMETRIC;
	}

	return -1;
}

int
test_symmetric_circe(phase_t *php)
{
	return -1;
}

static void
save_direct_mate(void)
{
	mate_move_t *mmp, *mmq;
	int d;

	if (mm_root == NULL) {
		// 根元から詰手順を設定
		mmq = (mate_move_t *)&mm_root;
		for (d = 1; d <= depth - 1 && d <= limit_depth; d++) {
			mmp = get_mate_move();
			mmp->prev = d == 1 ? NULL : mmq;
			copy_move_2_mate_move(&phase[d].move, d, mmp);
			mmq->next = mmp;
			mmq = mmp;
		}
		set_mate_flag(mmp, d);
	} else {
		mmq = (mate_move_t *)&mm_root;
		for (d = 1; d <= depth - 1 && d <= limit_depth; d++) {
			if (mmq->next == NULL) {
				// 途中迄の枝に詰手順を接続
				for ( ; d <= depth - 1 && d <= limit_depth; d++) {
					mmp = get_mate_move();
					mmp->prev = d == 1 ? NULL : mmq;
					copy_move_2_mate_move(&phase[d].move, d, mmp);
					mmq->next = mmp;
					mmq = mmp;
				}
				set_mate_flag(mmp, d);
				break;
			}
			for (mmq = mmq->next; ; mmq = mmq->side) {
				if (comp_mate_move_vs_move(mmq, &phase[d].move) == 0) {
					mmp = mmq;
					break;
				}
				if (mmq->side == NULL) {
					mmp = NULL;
					break;
				}
			}
			if (mmp == NULL) {
				// 詰手順の途中で分岐
				mmp = get_mate_move();
				mmp->prev = d == 1 ? NULL : mmq->prev;	// ★
				copy_move_2_mate_move(&phase[d].move, d, mmp);
				mmq->side = mmp;						// ★
				mmq = mmp;
				for (d++; d <= depth - 1 && d <= limit_depth; d++) {
					mmp = get_mate_move();
					mmp->prev = mmq;
					copy_move_2_mate_move(&phase[d].move, d, mmp);
					mmq->next = mmp;
					mmq = mmp;
				}
				set_mate_flag(mmp, d);
				break;
			}
		}
	}

	return;
}

mate_move_t *
get_mate_move(void)
{
	mate_move_t *mmp;
	int i;

	if (mm_free == NULL) {
		mmp = calloc(NOMATEMOVE, sizeof(*mm_free));
		if (mmp == NULL) {
			fm_exit_mae("詰手順用手構造体");
		}
		mm_free = &mmp[0];
		for (i = 0; i < NOMATEMOVE - 1; i++) {
			mmp[i].next = &mmp[i + 1];
			mmp[i].prev = i == 0 ? NULL : &mmp[i - 1];
			mmp[i].side = NULL;
		}
		mmp[i].next = NULL;
		mmp[i].prev = &mmp[i - 1];
		mmp[i].side = NULL;
	}
	mmp = mm_free;
	mm_free = mm_free->next;
	if (mm_free != NULL && mm_free->next != NULL) {
		mm_free->next->prev = NULL;
	}
	mmp->next = NULL;

	return mmp;
}

static void
set_mate_flag(mate_move_t *mmp, int d)
{
	mmp->flag |= B_MM_MATE;
	if (d <= limit_depth) {
		mmp->flag |= B_MM_SHORT_MATE;
	}
	if (memcmp(&phase[d].hi_hand[PC_FU], &null_phase.hi_hand[PC_FU],
			sizeof(null_phase.hi_hand[0]) * 14) != 0) {
		mmp->flag |= B_MM_EXCESS_MATE;
	}

	return;
}

void
remove_mate_move_tree(phase_t *php)
{
	mate_move_t *mmp, *mmq;
	int d;

	if (mm_root == NULL) {
		return;
	}
	for (d = 1, mmp = mm_root; mmp != NULL; d++, mmp = mmp->next) {
		LOOP {
			if (comp_mate_move_vs_move(mmp, &phase[d].move) == 0) {
				break;
			}
			mmp = mmp->side;
			if (mmp == NULL) {
				return;
			}
		}
		if (d == (php - phase)) {
			break;
		}
	}
	if (mmp == NULL) {
		return;
	}
	mmq = NULL;
	if (mmp->prev == NULL) {
		if (mm_root == mmp) {
			mm_root = NULL;
		} else {
			mmq = mm_root;
		}
	} else {
		if (mmp->prev->next == mmp) {
			mmp->prev->next = NULL;
		} else {
			mmq = mmp->prev->next;
		}
	}
	if (mmq != NULL) {
		for ( ; mmq != NULL; mmq = mmq->side) {
			if (mmq->side == mmp) {
				mmq->side = NULL;
				break;
			}
		}
	}
	free_mate_move_tree(mmp);

	return;
}

void
free_mate_move_tree(mate_move_t *mmp)
{
	mate_move_t *mmq;

	if (mmp->next != NULL) {
		free_mate_move_tree(mmp->next);
		mmp->next = NULL;
	}
	mmq = mmp->side;
	free_mate_move(mmp);
	if (mmq != NULL) {
		free_mate_move_tree(mmq);
	}

	return;
}

void
free_mate_move(mate_move_t *mmp)
{
	mmp->next = mm_free;
	mmp->prev = NULL;
	mmp->side = NULL;
	if (mmp->next != NULL) {
		mmp->next->prev = mmp;
	}
#if 0
	mmp->pc = ROOM;
	mmp->to = 0;
	mmp->from = 0;
	mmp->to_1st = 0;
	mmp->flag = 0x00;
#endif
	mm_free = mmp;

	return;
}

void
copy_move_2_mate_move(move_t *mvp, int d, mate_move_t *mmp)
{
	mmp->depth = d;
	mmp->pc = mvp->pc;
	mmp->to = mvp->to;
	mmp->from = mvp->from;
	mmp->reborn = mvp->reborn;
	mmp->to_1st = mvp->to_1st;
	mmp->reborn_1st = mvp->reborn_1st;
	mmp->flag = 0x00;
	if (mvp->flag & B_MV_PROMOTE) {
		mmp->flag |= B_MM_PROMOTE;
	}
	if (phase[d].state & B_HITHER_TURN) {
		mmp->flag |= B_MM_HITHER_TURN;
	}
	mmp->max_mate_depth = 0;

	return;
}

void
copy_mate_move_2_move(mate_move_t *mmp, move_t *mvp)
{
	mvp->pc = mmp->pc;
	mvp->to = mmp->to;
	mvp->from = mmp->from;
	mvp->reborn = mmp->reborn;
	mvp->to_1st = mmp->to_1st;
	mvp->reborn_1st = mmp->reborn_1st;
	mvp->flag = 0x00;
	if (mmp->flag & B_MM_PROMOTE) {
		mvp->flag |= B_MV_PROMOTE;
	}

	return;
}

int
comp_mate_move_vs_move(mate_move_t *mmp, move_t *mvp)
{
	return (mmp->pc == mvp->pc &&
			mmp->to == mvp->to &&
			mmp->from == mvp->from &&
			mmp->reborn == mvp->reborn &&
			mmp->to_1st == mvp->to_1st &&
			mmp->reborn_1st == mvp->reborn_1st &&
			(mmp->flag & B_MM_PROMOTE) == (mvp->flag & B_MV_PROMOTE)) ? 0 : -1;
}

