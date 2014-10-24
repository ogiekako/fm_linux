// zaco/print.c -- FM printing routines
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/print.c 1.19 2014/08/16 11:04:37 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<mbstring.h>
#include	<time.h>
#include	<signal.h>
#include	"sjis.h"
#include	"fm.h"
#include	"piece.h"

static void print_hand(FILE *Fp, short *hand, short *hand2, int hi_side);
static void make_M_str(char outbuf[]);
static void print_mate_move_tree(mate_move_t *mmp);
static int prune_mate_move_tree(mate_move_t *mmp, mate_move_t *mmq);
static void print_direct_mate_sub(phase_t *php);

void
print_phase(phase_t *php, FILE *Fp)
{
	phase_t *php2 = NULL;
	int pf, pr;
	uchar_t sq;

	if (print_2nd_phase == ON) {
		php2 = &phase[0];
	}

	print_hand(Fp, php->yo_hand, (php2 != NULL) ? php2->yo_hand : NULL, OFF);

	// print board
	fputs("  ÇX ÇW ÇV ÇU ÇT ÇS ÇR ÇQ ÇP", Fp);
	if (php2 != NULL) {
		fputs("             ÇX ÇW ÇV ÇU ÇT ÇS ÇR ÇQ ÇP", Fp);
	}
	fprintf(Fp, "\n");
	fputs("+---------------------------+", Fp);
	if (php2 != NULL) {
		fputs("          +---------------------------+", Fp);
	}
	fprintf(Fp, "\n");
	for (pr = 0; pr <= 8; pr++) {
		fputc('|', Fp);
		for (pf = 8; pf >= 0; pf--) {
			sq = php->board[POS(pf, pr)];
			fputs(b_graph[sq], Fp);
		}
		fprintf(Fp, "|%s", kanji_num[pr]);
		if (php2 != NULL) {
			fputs("        |", Fp);
			for (pf = 8; pf >= 0; pf--) {
				sq = php2->board[POS(pf, pr)];
				fputs(b_graph[sq], Fp);
			}
			fprintf(Fp, "|%s", kanji_num[pr]);
		}
		fprintf(Fp, "\n");
	}
	fputs("+---------------------------+", Fp);
	if (php2) {
		fputs("          +---------------------------+", Fp);
	}
	fprintf(Fp, "\n");

	// print hand pieces of hither side
	print_hand(Fp, php->hi_hand, (php2 != NULL) ? php2->hi_hand : NULL, ON);

	return;
}

void
print_mate(phase_t *last_php, FILE *Fp)
{
	phase_t *php;
	char *mb;
	int col, len, left_margin = 73;
	int i, pc, m, n;

	if (&phase[1] == last_php) {
		fprintf(Fp, "0éË\n");
		if (Fp == stdout) {
			fprintf(Fp, "\n");
		} else if (brief_output == 0) {
			fputc('\n', Fp);
		}
		return;
	}
	if (mate_print == ON && (brief_output == 0 || Fp == stdout)) {
		print_2nd_phase = ON;
		print_phase(&phase[1], Fp);
		print_2nd_phase = OFF;
	}
	if (depth < limit_depth && min_limit_depth != 0xffff) {
		fprintf(Fp, "ëÅãl\n");
	}
	m = 8;
	len = make_move(&phase[1], mate_buf);
	mate_buf[len] = ' ';
	n = 1;
	col = left_margin - len;
	mb = mate_buf + len;
	for (php = &phase[2]; php < last_php; php++) {
		len = make_move(php, mb + 1);
		col -= len;
		if (col < 0 || n >= m) {
			mb[0] = '\n';
			n = 0;
			col = left_margin - len;
		} else {
			mb[0] = ' ';
		}
		n++;
		mb += 1 + len;
		col--;
	}
	n = (int)(last_php - &phase[1]);
	sprintf(mb + 1, "Ç‹Ç≈ %déË", n);
	len = (int)strlen(mb + 1);
	col -= 1 + len;
	if (col <= 0) {
		mb[0] = '\n';
		col = 72 - len;
	} else {
		mb[0] = ' ';
	}
	mb += 1 + len;
	if (NEQUALM(php->hi_hand, null_phase.hi_hand, sizeof(null_phase.hi_hand))) {
		strcpy(mb + 1, "ãÓó]ÇË ");
		for (i = 0, n = 0; NOT_NUL(hand_pc_order[i]); i++) {
			pc = hand_pc_order[i];
			if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PLACABLE)
					&& php->hi_hand[pc] > 0) {
				len = (int)strlen(mb + 1);
				if (php->hi_hand[pc] > 1) {
					sprintf(mb + len + 1, "%s%d", graph[pc], php->hi_hand[pc]);
				} else {
					sprintf(mb + len + 1, "%s", graph[pc]);
				}
			}
		}
		len = (int)strlen(mb + 1);
		col -= 1 + len;
		if (col <= 0) {
			mb[0] = '\n';
			col = 72 - len;
		} else {
			mb[0] = ' ';
		}
		mb += 1 + len;
	}
	mb[0] = '\n';
	mb[1] = '\0';
	{
		char *s, *t = mate_buf;
		for (s = strchr(t, '\n'); s != NULL; t = s + 1, s = strchr(t, '\n')) {
			char work[128];
			work[0] = '\0';
			strncat(work, t, s - t + 1);
			fprintf(Fp, work);
		}
	}
	if (Fp == stdout) {
		fprintf(Fp, "\n");
	} else if (brief_output == 0) {
		fputc('\n', Fp);
	}

	if (ferror(Fp)) {
		fprintf(stderr, "\n\n%s: èoóÕÉtÉ@ÉCÉãÇÃèëçûÇ›ÉGÉâÅ[Ç≈Ç∑...\n\n", cmdname);
		fm_exit(2);
	}

	return;
}

// mode:
//   0: normal
//   1: for dancing
//   2: for saving
//   3: only 1 line output
void
print_last_moves(phase_t *last_php, int mode, FILE *Fp)
{
	phase_t *php;
	int col, len, line, n;
	char outbuf[16];

	switch (mode) {
	case 1:
		col = 0;
		n = 1;
		break;
	case 2:
		fprintf(Fp, "ç≈èIåüçıéËèáÅF\n");
		col = 0;
		n = 1;
		break;
	case 3:
		fprintf(Fp, "ç≈èIåüçıéËèáÅF");
		col = 14;
		n = 1 + 2;
		break;
	default:
		fprintf(Fp, "ç≈èIåüçıéËèá (%d)ÅF\n", (last_php - phase) - 1);
		fprintf(Fp, "\n");
		col = 0;
		n = 1;
		break;
	}
	line = 0;
	len = make_move(&phase[1], outbuf);
	fputs(outbuf, Fp);
	col += len;
	for (php = &phase[2]; php < last_php; php++) {
		len = make_move(php, outbuf);
		switch (mode) {
		case 2:
			if (col + len >= 72 || n >= 8) {
				fprintf(Fp, "\n");
				col = 0;
				n = 0;
				line++;
			} else {
				fputc(' ', Fp);
				col++;
			}
			break;
		case 3:
			if (col + len >= 75) {
				if ((php - &phase[0]) < limit_depth) {
					fputs(" ...", Fp);
				}
				goto loop_break;
			}
			fputc(' ', Fp);
			col++;
			break;
		default:
			if (line >= 12 && (col + len >= 75 || n >= 8) && Fp == stdout) {
				if ((php - &phase[0]) < limit_depth) {
					fputs(" ...", Fp);
				}
				goto loop_break;
			}
			if (col + len >= 72 || n >= 8) {
				fprintf(Fp, "\n");
				col = 0;
				n = 0;
				line++;
			} else {
				fputc(' ', Fp);
				col++;
			}
			break;
		}
		fputs(outbuf, Fp);
		col += len;
		n++;
	}
loop_break:;
	fprintf(Fp, "\n");

	return;
}

void
print_record(FILE *Fp)
{
	int hours, minutes, seconds;
	char work[32];

	seconds = (int)(total_elapse % 60);
	minutes = (int)((total_elapse / 60) % 60);
	hours = (int)((total_elapse / 60) / 60);

	fputs("âêÕéûä‘ÅF", Fp);
	if (hours > 0) {
		fprintf(Fp, "%déûä‘ %2dï™ %2dïb  ", hours, minutes, seconds);
	} else if (minutes > 0) {
		fprintf(Fp, "%dï™ %2dïb  ", minutes, seconds);
	} else {
		fprintf(Fp, "%dïb  ", seconds);
	}
	fprintf(Fp, "âêÕã«ñ êîÅF%s  åüèoâêîÅF%ld",
			make_num_str(phase_1M, phase_count, work), solution_count);

	return;
}

void
print_rule(FILE *Fp)
{
	int pc, ppc, x;
	char outbuf[256], work[32];

	reg_rule(outbuf);
	fputs(outbuf, Fp);
	fprintf(Fp, " %séË", make_num_str(0, limit_depth, work));
	if (max_limit_depth > 0) {
		fputs("ÅièáéüåüçıÅF", Fp);
		if (min_limit_depth > 0) {
			fprintf(Fp, "%s", make_num_str(0, min_limit_depth, work));
		} else {
			fprintf(Fp, "%s", make_num_str(0, ((max_limit_depth & 0x1) ? 1 : 2), work));
		}
		fprintf(Fp, "-%sÅj", make_num_str(0, max_limit_depth, work));
	}
	fprintf(Fp, "\n");

	for (pc = PC_F1; pc <= PC_F7; pc++) {
		if (piece[pc].attr & B_PA_DEFINED) {
			outbuf[0] = '\0';
			if (piece[pc].attr & B_PA_ROYAL) {
				sprintf(outbuf, "%s: %sâ§", graph[pc], piece[pc].name);
			} else {
				sprintf(outbuf, "%s: %s", graph[pc], piece[pc].name);
			}
			if (piece[pc].attr & B_PA_PROMOTABLE) {
				ppc = pc | B_PC_PROMOTED;
				x = (int)strlen(outbuf) - 1;
				if (outbuf[x] != ' ' && NEQUALZ(outbuf + x - 1, "Åj")) {
					strcat(outbuf, " ");
				}
				strcat(outbuf, ">> ");
				sprintf(outbuf + strlen(outbuf), "%s: %s", graph[ppc], piece[ppc].name);
				if (piece[ppc].attr & B_PA_ROYAL) {
					strcat(outbuf, "â§");
				}
			}
			x = (int)strlen(outbuf) - 1;
			if (outbuf[x] != ' ' && NEQUALZ(outbuf + x - 1, "Åj")) {
				strcat(outbuf, " ");
			}
			sprintf(outbuf + strlen(outbuf), "[%d+%d]",
					hi_used_pc_count[pc], yo_used_pc_count[pc]);
			if (NOT(piece[pc].attr & B_PA_ROYAL) && NOT(piece[pc].attr & B_PA_PLACABLE)) {
				x = (int)strlen(outbuf) - 1;
				if (outbuf[x] == ' ') {
					strcpy(outbuf + x, ", ");
				} else if (outbuf[x] == ']') {
					strcat(outbuf + x, ", ");
				} else {
					strcat(outbuf, "ÅA");
				}
				strcat(outbuf, "éÊéÃÇƒ");
			}
			fprintf(Fp, "%s\n", outbuf);
		}
	}

	fprintf(Fp, "\n");

	return;
}

static void
print_hand(FILE *Fp, short *hand, short *hand2, int hi_side)
{
	int i, n;
	int len, len2;
	uchar_t pc;
	uchar_t buf[64], buf21[64], buf22[64];

	strcpy(buf, "éùãÓÅF");
	buf21[0] = '\0';
	for (i = 0, n = 0; NOT_NUL(hand_pc_order[i]); i++) {
		pc = hand_pc_order[i];
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PLACABLE) &&
				hand[pc] >= 1) {
			n++;
			strcat(buf, graph[pc]);
			if (hand[pc] >= 2) {
				sprintf(buf + strlen(buf), "%d", hand[pc]);
			}
		}
	}
	if (n == 0) {
		strcat(buf, "Ç»Çµ");
	}
	len = (int)strlen(buf);
	if (len >= 40 && hand2 != NULL) {
		for (i = 0; ; i++) {
			if (NOT(isgraph((uchar_t)buf[i]))) {
				if (i >= 35) {
					break;
				}
				i++;
			}
		}
		strcpy(buf21, "      ");
		strcat(buf21, buf + i);
		buf[i] = '\0';
		len = (int)strlen(buf);
	}
	fprintf(Fp, "%s", buf);
	if (hand2 != NULL) {
		fprintf(Fp, "%*c", (39 - len), ' ');
		strcpy(buf, "éùãÓÅF");
		buf22[0] = '\0';
		for (i = 0, n = 0; NOT_NUL(hand_pc_order[i]); i++) {
			pc = hand_pc_order[i];
			if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PLACABLE) &&
					hand2[pc] >= 1) {
				n++;
				strcat(buf, graph[pc]);
				if (hand2[pc] >= 2) {
					sprintf(buf + strlen(buf), "%d", hand2[pc]);
				}
			}
		}
		if (n == 0) {
			strcat(buf, "Ç»Çµ");
		}
		len2 = (int)strlen(buf);
		if (len2 >= 40) {
			for (i = 0; ; i++) {
				if (NOT(isgraph((uchar_t)buf[i]))) {
					if (i >= 35) {
						break;
					}
					i++;
				}
			}
			strcpy(buf22, "      ");
			strcat(buf22, buf + i);
			buf[i] = '\0';
			len2 = (int)strlen(buf);
		}
		fprintf(Fp, "%s", buf);
		if (buf21[0] != '\0' || buf22[0] != '\0') {
			fprintf(Fp, "\n%s%*c%s", buf21, (39 - strlen(buf21)), ' ', buf22);
		}
	}
	if (Fp == stdout) {
		ERASE_EOF();
	}
	if (hi_side == ON) {
		fprintf(Fp, "\n\n");
	} else {
		fprintf(Fp, "\n");
	}

	return;
}

void
print_record4(void)
{
	long long total_phase_count = phase_1M * 1000000LL + phase_count;
	char work[32], work2[32], work3[32];

	if (fileFp != NULL) {
		if (brief_output == 0) {
			if (solution_count == 0) {
				fputs("ïsãl\n\n", fileFp);
			}
			if (limit_depth > 0) {
				print_record(fileFp);
				fputs("  ", fileFp);
				if (solution_limit == -1) {
					fputs("ãÓó]ë≈êÿ\n", fileFp);
				} else if (solution_limit == -2) {
					fputs("ëÅãlë≈êÿ\n", fileFp);
				} else if (solution_limit == -3) {
					fputs("ï ãlë≈êÿ\n", fileFp);
				} else if (solution_limit == -4) {
					fputs("êßå¿éûä‘ë≈êÿ\n", fileFp);
				} else if (solution_count < solution_limit) {
					fputs("ëSåüäÆóπ\n", fileFp);
				} else {
					fputs("ëSåüñ¢óπ\n", fileFp);
				}
				if (verbose == ON) {
					fprintf(fileFp, "  ê∂ê¨éËêî êﬂÅ{ótÅF%s+%s=%s(%4.1f%%)\n",
							make_num_str((long)(call_move_count / 1000000),
								(long)(call_move_count % 1000000), work),
							make_num_str((long)(call_move_leaf_count / 1000000),
								(long)(call_move_leaf_count % 1000000), work2),
							make_num_str((long)((call_move_count + call_move_leaf_count) / 1000000),
								(long)((call_move_count + call_move_leaf_count) % 1000000), work3),
							(double)total_phase_count / (call_move_count + call_move_leaf_count) *
								100);
					fprintf(fileFp, "ê∂ê¨ã«ñ êî êﬂÅ{ótÅF%s+%s=%s(%4.1f%%)\n",
							make_num_str((long)(call_analyze_count / 1000000),
								(long)(call_analyze_count % 1000000), work),
							make_num_str((long)(call_analyze_leaf_count / 1000000),
								(long)(call_analyze_leaf_count % 1000000), work2),
							make_num_str((long)((call_analyze_count + call_analyze_leaf_count) /
								1000000),
								(long)((call_analyze_count + call_analyze_leaf_count) % 1000000),
									work3),
							(double)total_phase_count /
								(call_analyze_count + call_analyze_leaf_count) * 100);
				}
				fputs("\n", fileFp);
			}
		}
	}
	ERASE_EOS();
	if (solution_count == 0) {
		printf("ïsãl\n\n");
	}
	if (limit_depth > 0) {
		print_record(stdout);
		printf("  ");
		if (solution_limit == -1) {
			printf("ãÓó]ë≈êÿ\n");
		} else if (solution_limit == -2) {
			printf("ëÅãlë≈êÿ\n");
		} else if (solution_limit == -3) {
			printf("ï ãlë≈êÿ\n");
		} else if (solution_limit == -4) {
			printf("êßå¿éûä‘ë≈êÿ\n");
		} else if (solution_count < solution_limit) {
			printf("ëSåüäÆóπ\n");
		} else {
			printf("ëSåüñ¢óπ\n");
		}
		if (verbose == ON) {
			printf("  ê∂ê¨éËêî êﬂÅ{ótÅF%s+%s=%s(%4.1f%%)\n",
					make_num_str((long)(call_move_count / 1000000),
						(long)(call_move_count % 1000000), work),
					make_num_str((long)(call_move_leaf_count / 1000000),
						(long)(call_move_leaf_count % 1000000), work2),
					make_num_str((long)((call_move_count + call_move_leaf_count) / 1000000),
						(long)((call_move_count + call_move_leaf_count) % 1000000), work3),
					(double)total_phase_count / (call_move_count + call_move_leaf_count) * 100);
			printf("ê∂ê¨ã«ñ êî êﬂÅ{ótÅF%s+%s=%s(%4.1f%%)\n",
					make_num_str((long)(call_analyze_count / 1000000),
						(long)(call_analyze_count % 1000000), work),
					make_num_str((long)(call_analyze_leaf_count / 1000000),
						(long)(call_analyze_leaf_count % 1000000), work2),
					make_num_str((long)((call_analyze_count + call_analyze_leaf_count) / 1000000),
						(long)((call_analyze_count + call_analyze_leaf_count) % 1000000), work3),
					(double)total_phase_count / (call_analyze_count + call_analyze_leaf_count) *
						100);
		}
		printf("\n");
	}

	return;
}

int
print_search_options(FILE *Fp, int use_brief_problem_output)
{
	int rc = 0;
	char outbuf[128] = { '/', '\0', '\0', };

	// /L...
	if (solution_limit != 10) {
		fprintf(Fp, "/L%d\n", solution_limit);
		rc++;
		outbuf[1] = '\0';
		outbuf[2] = '*';
	}
	// /M...
	if ((strategy & (B_HI_ESCAPE | B_YO_ESCAPE))
			|| (strategy & (B_HI_ALLOW_SAME | B_YO_ALLOW_SAME))) {
		make_M_str(outbuf);
		outbuf[1] = 'M';
		fprintf(Fp, "%s\n", outbuf);
		rc++;
		outbuf[1] = '\0';
	}

	return rc;
}

static void
make_M_str(char outbuf[])
{
	long m;
	char work[32], work2[32];

	if (strategy & B_HI_ESCAPE) {
		strcat(outbuf, ",HE");
		if (hi_eh_given == ON) {
			sprintf(work, "=%s",
					make_num_str((long)(hi_eh_lim / 1000000), (long)(hi_eh_lim % 1000000), work2));
			strcat(outbuf, work);
			m = (long)((hi_eh_lim * sizeof(escape_hash_t) + 0x100000L - 1) / 0x100000L);
			sprintf(work, "(%sMB)", make_num_str(0, m, work2));
			strcat(outbuf, work);
			sprintf(work, "/%s", make_num_str(0, no_hi_eh_head, work2));
			strcat(outbuf, work);
		}
	}
	if (strategy & B_YO_ESCAPE) {
		strcat(outbuf, ",YE");
		if (yo_eh_given == ON) {
			sprintf(work, "=%s",
					make_num_str((long)(yo_eh_lim / 1000000), (long)(yo_eh_lim % 1000000), work2));
			strcat(outbuf, work);
			m = (long)((yo_eh_lim * sizeof(escape_hash_t) + 0x100000L - 1) / 0x100000L);
			sprintf(work, "(%sMB)", make_num_str(0, m, work2));
			strcat(outbuf, work);
			sprintf(work, "/%s", make_num_str(0, no_yo_eh_head, work2));
			strcat(outbuf, work);
		}
	}
	if (strategy & B_HI_ALLOW_SAME) {
		strcat(outbuf, ",HAS");
	}
	if (strategy & B_YO_ALLOW_SAME) {
		strcat(outbuf, ",YAS");
	}

	return;
}

void
print_start(FILE *Fp, int restart)
{
	int rc;

	if (Fp == fileFp && brief_output == 0) {
		time_t t = time((time_t *)NULL);
		fprintf(Fp, "%s %s - %s\n", cmdname, release, ctime(&t));
	}
	if (restart == ON) {
		fprintf(Fp, "çƒäJÅi%sÅj\n\n", restart_file);
	}
	rc = print_search_options(Fp, 0);
	if (rc >= 1 && (brief_output == 0 || Fp == stdout)) {
		fprintf(Fp, "\n");
	}
	if (title[0] != '\0') {
		fprintf(Fp, "%s\n\n", title);
	}
	print_rule(Fp);
	print_phase(&phase[1], Fp);	// èâê}ñ ï\é¶

	return;
}

void
print_option_L(FILE *Fp)
{
	if (solution_limit != 10) {
		fprintf(Fp, "/L%d\n", solution_limit);
	}

	return;
}

void
print_option_M(FILE *Fp)
{
	uchar_t first = ON;

	if (strategy & B_HI_ESCAPE) {
		if (first == ON) {
			first = OFF;
			fprintf(Fp, "/MHE");
		} else {
			fprintf(Fp, ",HE");
		}
		if (hi_eh_given == ON) {
			fprintf(Fp, "=%d", hi_eh_lim);
		}
	}
	if (strategy & B_YO_ESCAPE) {
		if (first == ON) {
			first = OFF;
			fprintf(Fp, "/MYE");
		} else {
			fprintf(Fp, ",YE");
		}
		if (yo_eh_given == ON) {
			fprintf(Fp, "=%d", yo_eh_lim);
		}
	}
	if (strategy & B_HI_ALLOW_SAME) {
		if (first == ON) {
			first = OFF;
			fprintf(Fp, "/MHAS");
		} else {
			fprintf(Fp, ",HAS");
		}
	}
	if (strategy & B_YO_ALLOW_SAME) {
		if (first == ON) {
			first = OFF;
			fprintf(Fp, "/MYAS");
		} else {
			fprintf(Fp, ",HYS");
		}
	}
	if (first == OFF) {
		fprintf(Fp, "\n");
	}

	return;
}

void
print_option_B(FILE *Fp)
{
	if (brief_output == 2) {
		fprintf(Fp, "/B\n");
	}
	if (mate_print == OFF) {
		fprintf(Fp, "/BM\n");
	}

	return;
}

void
print_option_X(FILE *Fp)
{
	if (arg_x != NULL) {
		fprintf(Fp, "%s\n", arg_x);
	}

	return;
}

int fmmt_cc;

void
print_direct_mate(void)
{
	int save_depth = depth;
	phase_t save_phase = phase[1];
	phase_t save_phase2 = phase[2];

//	if ((debug & 0x80) && fileFp != NULL) {
//		fprintf(fileFp, "wmmt - before prune start\n");
//		walk_mate_move_tree(mm_root, mm_root);
//		fprintf(fileFp, "\n");
//	}
	fmmt_cc = 0;
	(void)prune_mate_move_tree(mm_root, mm_root);
//	if ((debug & 0x80) && fileFp != NULL) {
//		fprintf(fileFp, "wmmt - after prune start\n");
//		walk_mate_move_tree(mm_root, mm_root);
//		fprintf(fileFp, "\n");
//	}
	solution_count++;
	print_mate_move_tree(mm_root);
	if (mate_print == ON && brief_output == 0) {
		print_record(stdout);
	}

	depth = save_depth;
	phase[1] = save_phase;
	phase[2] = save_phase2;

	return;
}

// ãléËèáñÿôííË
//   mmp:  èàóùëŒè€ãléË
//   mmq:  ï™äÚãléËÅiNULLÇÃèÍçáÇÕÅAmmpÇ™èâéËÇÃç≈èâÇ≈Ç†ÇÈÇ±Ç∆Çé¶Ç∑Åj
static int
prune_mate_move_tree(mate_move_t *mmp, mate_move_t *mmq)
{
	mate_move_t *mmr, *mms, *mmt;
	int mmd, mmf, fmmt = 0;

	fmmt_cc++;
	if (mmp->next == NULL) {
		// ññí[ÇÃèÍçáÅAç≈ëÂãléËêîÇê›íË
		mmp->max_mate_depth = mmp->depth;
	} else {
		// ññí[Ç≈Ç»ÇØÇÍÇŒÅAÇPíiêÊÇÃóvëfÇèàóù
		(void)prune_mate_move_tree(mmp->next, mmq);
	}
	if (mmp->side != NULL) {
		// ìØÉåÉxÉãóvëfÇ™Ç†ÇÍÇŒÅAÇªÇÍÇèàóù
		(void)prune_mate_move_tree(mmp->side, mmp->side);
	} else {
		// ìØÉåÉxÉãóvëfÇÇ∑Ç◊ÇƒèàóùÇµÇΩå„ÅAÇ‹ÇΩÇÕÅAìØÉåÉxÉãóvëfÇ™Ç»Ç©Ç¡ÇΩèÍçá
		if (mmq != mmp) {
			// ìØÉåÉxÉãóvëfÇ™Ç»Ç©Ç¡ÇΩèÍçá
			if (mmp->prev != NULL) {
				mmp->prev->max_mate_depth = mmp->max_mate_depth;
				mmp->prev->flag |= mmp->flag & B_MM_EXCESS_MATE;
			}
		} else {
			// ìØÉåÉxÉãóvëfÇÇ∑Ç◊ÇƒèàóùÇµÇΩå„
			if (mmp->flag & B_MM_HITHER_TURN) {
				// çUï˚î‘ÇÃèÍçáÅAç≈íZéËêîÇÃÇ›ÇécÇ∑ÅB
				mmd = 0;
				mmr = mmp->prev == NULL ? mm_root : mmp->prev->next;
				for ( ; mmr != NULL; mmr = mmr->side) {
					if (mmd == 0) {
						mmd = mmr->max_mate_depth;
					} else if (mmd < mmr->max_mate_depth) {
						if (mmr->next != NULL) {
							free_mate_move_tree(mmr->next);
							mmr->next = NULL;
							fmmt++;
						}
						mmr->flag |= B_MM_DEL;
					} else if (mmd > mmr->max_mate_depth) {
						mmd = mmr->max_mate_depth;
						mms = mmp->prev == NULL ? mm_root : mmp->prev->next;
						for ( ; mms != mmr; mms = mms->side) {
							if (mms->next != NULL) {
								free_mate_move_tree(mms->next);
								mms->next = NULL;
								fmmt++;
							}
							mms->flag |= B_MM_DEL;
						}
					}
				}
			} else {
				// éÛï˚î‘ÇÃèÍçáÅAç≈í∑éËêîÇÃÇ›ÇécÇ∑ÅB
				mmd = 0;
				mmr = mmp->prev == NULL ? mm_root : mmp->prev->next;
				for ( ; mmr != NULL; mmr = mmr->side) {
					if (mmd == 0) {
						mmd = mmr->max_mate_depth;
					} else if (mmd > mmr->max_mate_depth) {
						if (mmr->next != NULL) {
							free_mate_move_tree(mmr->next);
							mmr->next = NULL;
							fmmt++;
						}
						mmr->flag |= B_MM_DEL;
					} else if (mmd < mmr->max_mate_depth) {
						mmd = mmr->max_mate_depth;
						mms = mmp->prev == NULL ? mm_root : mmp->prev->next;
						for ( ; mms != mmr; mms = mms->side) {
							if (mms->next != NULL) {
								free_mate_move_tree(mms->next);
								mms->next = NULL;
								fmmt++;
							}
							mms->flag |= B_MM_DEL;
						}
					}
				}
			}
			mmf = 0x00;
			mmr = mmp->prev == NULL ? mm_root : mmp->prev->next;
			for ( ; mmr != NULL; mmr = mmr->side) {
				if (NOT(mmr->flag & B_MM_DEL)) {
					mmf |= (mmr->flag & B_MM_EXCESS_MATE) ? B_MM_EXCESS_MATE : B_MM_MATE;
				}
			}
			if ((mmf & B_MM_MATE) && (mmf & B_MM_EXCESS_MATE)) {
				mmf = 0x00;
				mmr = mmp->prev == NULL ? mm_root : mmp->prev->next;
				for ( ; mmr != NULL; mmr = mmr->side) {
					if ((mmr->flag & B_MM_EXCESS_MATE) && NOT(mmr->flag & B_MM_DEL)) {
						if (mmr->next != NULL) {
							free_mate_move_tree(mmr->next);
							mmr->next = NULL;
							fmmt++;
						}
						mmr->flag |= B_MM_DEL;
					}
				}
			}
			mmr = mms = mmp->prev == NULL ? mm_root : mmp->prev->next;
			for ( ; mmr != NULL; mmr = mmr->side) {
				if (NOT(mmr->flag & B_MM_DEL)) {
					if (mmr->prev == NULL) {
						mm_root = mmr;
					} else {
						mmr->prev->next = mmr;
						if (mmr->prev->max_mate_depth == 0) {
							mmr->prev->max_mate_depth = mmr->max_mate_depth;
						}
						mmr->prev->flag |= mmf & B_MM_EXCESS_MATE;
					}
					break;
				}
			}
			for (mmt = NULL, mmr = mms; mmr != NULL; mmt = mmr, mmr = mms) {
				mms = mmr->side;
				if (mmr->flag & B_MM_DEL) {
					if (mmt != NULL) {
						mmt->side = mms;
					}
					free_mate_move(mmr);
					fmmt++;
					mmr = mmt;
				}
			}
//			if (fmmt > 0 && (debug & 0x80) && fileFp != NULL) {
//				fprintf(fileFp, "pmmt - after free mate-move %d\n", fmmt_cc);
//				walk_mate_move_tree(mm_root, mm_root);
//				fprintf(fileFp, "\n");
//			}
		}
	}

	return 0;
}

static void
print_mate_move_tree(mate_move_t *mmp)
{
	mate_move_t *mmq;
	int d;

	if (mmp->next == NULL) {
		for (mmq = mmp; mmq != NULL; mmq = mmq->prev) {
			copy_mate_move_2_move(mmq, &last_move[mmq->depth]);
		}
		for (d = 1; d <= mmp->depth; d++) {
			phase[d].move = last_move[d];
			depth = d;
			if (IS_HI_TURN(&phase[d])) {
				(void)hi_move(&phase[d]);
			} else {
				(void)yo_move(&phase[d]);
			}
		}
		depth = d;
		print_direct_mate_sub(&phase[depth]);
	} else {
		print_mate_move_tree(mmp->next);
	}
	if (mmp->side != NULL) {
		if (mmp->flag & B_MM_HITHER_TURN) {
			solution_count++;
		}
		print_mate_move_tree(mmp->side);
	}

	return;
}

static void
print_direct_mate_sub(phase_t *php)
{
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
#if 0
	if (solution_count >= solution_limit) {
		// âêîè„å¿
		fprintf(stderr, "âêîÇ™éwíËÇ≥ÇÍÇΩè„å¿Ç…íBÇµÇ‹ÇµÇΩ...\n\n");
	}
#endif

	return;
}

//int count = 0;
// mmp:  èàóùëŒè€ãléË
// mmq:  ï™äÚãléË
int
walk_mate_move_tree(mate_move_t *mmp, mate_move_t *mmq)
{
	if (mmp == NULL) {
		return 0;
	}
//	if ((debug & 0x80) && fileFp != NULL) {
//		fprintf(fileFp, "wmmt - %02d %02d %02d-%02d %s %02x p=%p n=%p v=%p s=%p q=%p\n",
//				mmp->depth, mmp->max_mate_depth, mmp->from == NOP ? 0 : PR_POS(mmp->from),
//				PR_POS(mmp->to), graph[mmp->pc], mmp->flag,
//				mmp, mmp->next, mmp->prev, mmp->side, mmq);
//	}
	if (mmp->next != NULL) {
		(void)walk_mate_move_tree(mmp->next, mmq);
	}
	if (mmp->side != NULL) {
		(void)walk_mate_move_tree(mmp->side, mmp->side);
	}

	return 0;
}

