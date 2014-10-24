// Fairy Mate 6.xxx.xxx
// zaco/main.c -- main routines of FM solver
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/main.c 1.12 2014/08/14 02:50:46 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<time.h>
#include	<signal.h>
#include	<sys/stat.h>
#include	<windows.h>
#include	"fm.h"
#include	"piece.h"

static void solve(void);
//static void in_key_check(void);

static char copyright[] = "Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.";

int
main(int argc, char *argv[])
{
	int c, rc = 0;
	extern char *optarg;
	extern int dosopt(char *argv[], const char *optarg);

#if 0
	{
		char **av;
		for (av = argv; *av != NULL; av++) {
			DISPLAY1("%s ", *av);
		}
		SPACE2();
//		getch();
	}
#endif
	argc = argc;	// warning message を避けるためのダミーコード
	setvbuf(stdout, NULL, _IONBF, 0);
	signal(SIGINT, SIG_IGN);

	atexit(del_file);  // 一時ファイルの削除
	init_console_win32();
	atexit(reset_console_win32);

	setup_piece_attrs();

	while ((c = dosopt(argv, "R:O[FM000.FMO]B[ ]L:E:M:I:T[2]X[ ]")) != EOF) {
		switch (c) {
		case 'R':		// restart
			strcpy(restart_file, optarg);
			restartFp = fopen(restart_file, "r");
			if (restartFp != NULL) {
				restarting = ON;
			}
			break;
		case 'O':		// output file name
			strcpy(out_file, optarg);
			fileFp = fopen(out_file, "a");
			if (fileFp == NULL) {
				fprintf(stderr, "%s: 出力ファイル %s がオープンできません...\n\n",
						cmdname, out_file);
				fm_exit(1);
			}
			break;
		case 'B':		// brief output mode
			switch (*optarg) {
			case 'M':
				mate_print = OFF;
				break;
			default:
				brief_output = 2;
				break;
			}
			break;
		case 'L':		// any limits
			option_L(optarg);
			break;
		case 'E':		// extra option switches
			option_E(optarg);
			break;
		case 'M':		// 特殊条件高速化モード
			option_M(optarg);
			break;
		case 'I':		// input file name
		case '*':
			strcpy(in_file, optarg);
			inFp = fopen(in_file, "r");
			if (inFp == NULL) {
				fprintf(stderr, "%s: 入力ファイル %s がオープンできません...\n\n",
						cmdname, in_file);
				fm_exit(1);
			}
			break;
		case 'T':		// trace mode
			trace_limit = atoi(optarg);
			break;
		case 'X':		// temporary option switches
			option_X(optarg);
			break;
		}
	}
	CURSOR_OFF(0, 0);
	strategy |= B_HI_DENY_SAME | B_YO_DENY_SAME;

	printf("%s %s - Copyright(c) KAMINA, Jiro 2013-2014\n\n", cmdname, release);

	if (restarting == ON) {
		if (restore()) {
			rc = 1;
			goto end_proc;
		}
		if (fileFp != NULL && brief_output == 0) {
			print_start(fileFp, ON);
		}
		print_start(stdout, ON);
	} else {
		if (arrange(inFp) != 0) {
			rc = 1;
			goto end_proc;
		}
		if (fileFp != NULL && brief_output == 0) {
			print_start(fileFp, OFF);
		}
	}
	solve();

end_proc:
	if (fileFp != NULL) {
		if (brief_output == 0) {
			fputs("以上\n\n", fileFp);
		}
		fclose(fileFp);
		if (brief_output > 0) {
			struct stat stat_buf;
			if (stat(out_file, &stat_buf) == 0 && stat_buf.st_size == 0) {
				remove(out_file);
			}
		}
	}

	return rc;
}

void
solve(void)
{
	phase_t *php;
	uchar_t save_phase_state = 0x00;
#if 0
{
	int i, pf, pr;
	uchar_t x;
	for (pr = 0; pr <= 8; pr++) {
		for (pf = 8; pf >= 0; pf--) {
			//i = hi_leap_to_pos[PC_F1][POS(2, 2)][POS(pf, pr)];
			x = hi_to_board[PC_F1][POS(4, 4)][POS(pf, pr)];
			printf("%s ", x == ON ? "o" : ".");
		}
		printf("  ");
		for (pf = 8; pf >= 0; pf--) {
			//i = hi_leap_to_pos[PC_F1][POS(2, 2)][POS(pf, pr)];
			x = yo_to_board[PC_F1][POS(4, 4)][POS(pf, pr)];
			printf("%s ", x == ON ? "o" : ".");
		}
		printf("\n");
	}
	#if 0
	for (i = 0; hi_run_to_posv[PC_F1][POS(2, 2)][0][i] != EOP; i++) {
		printf("%d-%d, ",
				PR_POS(hi_run_to_posv[PC_F1][POS(2, 2)][0][i]),
				PR_POS(hi_run_to_posv[PC_F1][POS(2, 2)][1][i]));
	}
	printf("\n");
	#endif
}
#endif

	if (max_limit_depth > 0 && trace_limit == 0) {
		save_phase_state = phase[1].state;

	test_deep:
		if (fileFp != NULL && brief_output == 0) {
			fprintf(fileFp, "%d手検索\n\n", limit_depth);
		}
		CLEAR_LINE();
		printf("%d手検索\n\n", limit_depth);
	}

	if (trace_limit == 0 || fileFp != NULL) {
		print_record(stdout);
	}

	depth = 0;

	signal(SIGINT, signal_exit);
	restart_time = time((time_t *)NULL);
	start_time = restart_time - total_elapse;

	php = &phase[1];
	if (rule_flag & B_R_DIRECT_MATE) {
		// かしこ詰
		if (limit_depth % 2 == 0) {
			php->state &= ~B_HITHER_TURN;
			escape(php);
		} else {
			php->state |= B_HITHER_TURN;
			check(php);
		}
	} else if (rule_flag & (B_R_HELP_SELF_MATE | B_R_HELP_SELF_STALEMATE)) {
		// 協力自玉詰または協力自玉ステイルメイト
		if (limit_depth % 2 == 0) {
			php->state |= B_HITHER_TURN;
			check(php);
		} else {
			php->state &= ~B_HITHER_TURN;
			escape(php);
		}
	} else {
		// その他（協力詰または協力ステイルメイト）
		if (limit_depth % 2 == 0) {
			php->state &= ~B_HITHER_TURN;
			escape(php);
		} else {
			php->state |= B_HITHER_TURN;
			check(php);
		}
	}

	total_elapse = time((time_t *)NULL) - start_time;
	signal(SIGINT, SIG_IGN);

	if (solution_count == 0 && max_limit_depth > 0 && trace_limit == 0) {
		int d;
		while (phase_count >= 1000000) {
			phase_count -= 1000000;
			phase_1M++;
		}
		if (fileFp != NULL && brief_output == 0) {
			print_record(fileFp);
			fputs("\n\n", fileFp);
		}
		ERASE_EOS();
		print_record(stdout);
		printf("\n\n");
		save_count += phase_count;
		save_1M += phase_1M;
		d = 2;
		if (limit_depth + 2 <= max_limit_depth) {
			int i;
			limit_depth += 2;
			phase_count = 0;
			phase_1M = 0;
			check_point = check_interval = 100000;
			base_elapse = last_elapse = total_elapse;
			last_move[1] = null_move;
			phase[1].state = save_phase_state;
			phase[1].check_state = 0x00;

			if (strategy & B_HI_ESCAPE) {
				for (i = 0; i < hi_eh_lim; i++) {
					// TODO 未使用のエントリも無視して処理している。
					hi_eh_cell[i].depth += 2;
				}
			}
			if (strategy & B_YO_ESCAPE) {
				for (i = 0; i < yo_eh_lim; i++) {
					// TODO 未使用のエントリも無視して処理している。
					yo_eh_cell[i].depth += 2;
				}
			}
			goto test_deep;
		}
		phase_count = save_count;
		phase_1M = save_1M;
	}

	if (phase_1M == 0 && phase_count <= 0) {
		phase_count = 1;
	}
	while (phase_count >= 1000000) {
		phase_count -= 1000000;
		phase_1M++;
	}
	if (trace_limit > 0) {
		if (fileFp != NULL) {
			if (brief_output == 0) {
				fputc('\n', fileFp);
			}
			print_record(fileFp);
			fputs("  トレース終了\n\n", fileFp);
			CLEAR_LINE();
		} else {
			printf("\n");
		}
		print_record(stdout);
		printf("  トレース終了\n\n");
	} else {
		print_record4();
	}

	start_time = 0;

	return;
}

// rc:
//   1:  初期化エラー
//   2:  処理中エラー
//   3:  メモリエラー
//   9:  シグナル受信
void
fm_exit(int rc)
{
	CURSOR_ON(0, 0);
	exit(rc);
}

void
fm_exit_mae(uchar_t *s)
{
	if (start_time > 0) {
		fprintf(stderr, "\n\n");
	}
	fprintf(stderr, "%s: %sにメモリが割り当てられません...\n", cmdname, s);
	fm_exit(3);
}

