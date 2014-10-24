// zaco/sub.c -- FM subroutine
// Copyright(c) KAMINA Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/sub.c 1.33 2014/08/17 02:13:44 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/stat.h>
#include	<mbstring.h>
#include	<conio.h>
#include	<windows.h>
#include	<stdarg.h>
#include	<time.h>
#include	<signal.h>
#include	"sjis.h"
#include	"fm.h"
#include	"piece.h"

typedef struct del_file_ del_file_t;
struct del_file_ {
	del_file_t *next;
	FILE *Fp;
	char *name;
};

static del_file_t *del_file_list = NULL;

static void setup_cond_position(char *s);
static void setup_depth(char *s);
static void reg_prob2(phase_t *php, char *s);

char *
get_title(char *ib)
{
	char *s;
	int i;

	ib = skip_space(ib);
	for (i = 0; quote_mark[i].start != NULL; i++) {
		if (EQUALS(ib, quote_mark[i].start)) {
			s = ib += strlen(quote_mark[i].start);
			ib = _mbsstr(s, quote_mark[i].end);
			if (ib == NULL) {
				ib = skip_nonspace(s);
			}
			strncpy(title, s, ib - s);
			title[ib - s] = '\0';
			if (EQUALS(ib, quote_mark[i].end)) {
				ib += strlen(quote_mark[i].end);
			}
			for (s = title; isspace((uchar_t)s[0]); s++);
			if (s[0] == '\0') {
				title[0] = '\0';
			}
			break;
		}
	}

	return ib;
}

char *
get_rule(char *ib)
{
	char *s, *max_s;
	ulonglong_t tmp_cond_flag = 0;
	ulong_t tmp_rule_flag = 0;
	size_t len;
	int i;
	char work[256];

	memset(work, '\0', sizeof(work));
	strncpy(work, ib, sizeof(work));
	s = strtok(work, " \t");
	len = 0;
	for (i = 0; rule_cond[i].name != NULL; i++) {
		if (strlen(rule_cond[i].name) > len) {
			len = strlen(rule_cond[i].name);
		}
	}
	max_s = work;
	while (len >= 1) {
		for (i = 0; rule_cond[i].name != NULL; i++) {
			if (strlen(rule_cond[i].name) != len) {
				continue;
			}
			s = _mbsstr(work, rule_cond[i].name);
			if (s == NULL) {
				continue;
			}
			tmp_rule_flag |= rule_cond[i].rule_flag;
			tmp_cond_flag |= rule_cond[i].cond_flag;
			if (rule_cond[i].rule_flag & (BM_R_HELP | BM_R_DIRECT)) {
				setup_depth(s + len);
			}
			if (rule_cond[i].cond_flag & B_C_POSITION) {
				setup_cond_position(s);
			}
			memset(s, '_', len);
			if (s + len > max_s) {
				max_s = s + len;
			}
		}
		len--;
	}
	// 排他条件チェック
	// 1) 成禁系	
	if ((tmp_cond_flag & B_C_NOPROMOTE) && (tmp_cond_flag & B_C_ABSOLUTE_NOPROMOTE)) {
		error_message("成禁と絶対成禁は排他です", NULL);
		return NULL;
	}
	// 1') 取禁系	
	if ((tmp_cond_flag & B_C_NOCAPT) && (tmp_cond_flag & B_C_HITHER_NOCAPT)) {
		error_message("取禁と攻方取禁は排他です", NULL);
		return NULL;
	}
	// 2) 打歩詰系	
	if ((tmp_cond_flag & B_C_UCHIFU) && (tmp_cond_flag & B_C_UCHIFU_OK)) {
		error_message("打歩詰と打歩詰可は排他です", NULL);
		return NULL;
	}
	// 3) 強欲・禁欲
	if ((tmp_cond_flag & B_C_GREED) && (tmp_cond_flag & B_C_ASCETIC)) {
		error_message("強欲と禁欲は排他です", NULL);
		return NULL;
	}
	if ((tmp_cond_flag & (B_C_NOCAPT | B_C_HITHER_NOCAPT)) &&
			(tmp_cond_flag & (B_C_GREED | B_C_ASCETIC))) {
		error_message("取禁・攻方取禁と強欲・禁欲は排他です", NULL);
		return NULL;
	}
	// 4) 変身系
	if (bit_count_ulonglong(tmp_cond_flag & BM_C_CHANGE_FAMILY) >= 2) {
		error_message("安南・安北・対面・背面・ネコ鮮・ネコネコ鮮は排他です", NULL);
		return NULL;
	}
	if ((tmp_cond_flag & B_C_INVALID_CAPT_OU_2F) &&
			bit_count_ulonglong(tmp_cond_flag & BM_C_CHANGE_FAMILY) == 0) {
		error_message("二歩王取無効は、安南・安北・対面・背面・ネコ鮮・ネコネコ鮮の"
				"いずれかと同時に指定します", NULL);
		return NULL;
	}
	// 5) キルケ系（＋Andernach＋AntiAndernach）	
	if (bit_count_ulonglong(tmp_cond_flag &
			(BM_C_CIRCE_FAMILY | B_C_ANDERNACH | B_C_ANTIANDERNACH)) >= 2) {
		error_message("キルケ・アンチキルケ・PWC・Andernach・AntiAndernach・Messignyは排他です",
				NULL);
		return NULL;
	}
	// 6) マドラシ・Isardam系	
	if (bit_count_ulonglong(tmp_cond_flag & (BM_C_MADRASI_FAMILY | BM_C_ISARDAM_FAMILY)) >= 2) {
		error_message("マドラシ・Ｋマドラシ・Isardam・Isardam（タイプB）は排他です", NULL);
		return NULL;
	}
	if ((tmp_cond_flag & BM_C_ISARDAM_FAMILY) &&
			(tmp_cond_flag & (BM_C_CHANGE_FAMILY | B_C_KOKO))) {
		error_message("Isardam・Isardam（タイプB）と"
				"安南・安北・対面・背面・ネコ鮮・ネコネコ鮮・Kokoは排他です", NULL);
		return NULL;
	}
	if (bit_count_ulonglong(tmp_cond_flag & (BM_C_MADRASI_FAMILY | BM_C_ISARDAM_FAMILY)) >= 2) {
		error_message("Isardam・Isardam（タイプB）とは排他です", NULL);
		return NULL;
	}
	// 7) 未サポート
	if ((tmp_cond_flag & BM_C_MESSIGNY_FAMILY) && (tmp_cond_flag & (B_C_GREED | B_C_ASCETIC))) {
		error_message("強欲・禁欲のMessignyは未サポートです", NULL);
		return NULL;
	}
	if (tmp_rule_flag == 0 && tmp_cond_flag == 0) {
		return ib;
	}
	if (tmp_rule_flag & B_R_HELP_SELF_STALEMATE) {
		tmp_rule_flag &= ~(B_R_HELP_MATE | B_R_HELP_STALEMATE | B_R_HELP_SELF_MATE);
	} else if (tmp_rule_flag & B_R_HELP_SELF_MATE) {
		tmp_rule_flag &= ~(B_R_HELP_MATE | B_R_HELP_STALEMATE | B_R_HELP_SELF_STALEMATE);
	} else if (tmp_rule_flag & B_R_HELP_STALEMATE) {
		tmp_rule_flag &= ~(B_R_HELP_MATE | B_R_HELP_SELF_MATE | B_R_HELP_SELF_STALEMATE);
	} else {
		tmp_rule_flag |= B_R_HELP_MATE;
	}
	rule_flag = tmp_rule_flag;
	if (tmp_cond_flag != 0) {
		cond_flag = tmp_cond_flag;
	}

	return ib + strlen(work) + 1;
}

static void
setup_depth(char *s)
{
	char *t;
	int n;

	if (NOT(isdigit((uchar_t)s[0]))) {
		return;
	}
	n = strtol(s, &t, 10);
	memset(s, '_', t - s);
	if (n > LIMIT_DEPTH) {
		error_message("詰手数が大き過ぎます", NULL);
		return;
	}
	limit_depth = n;

	return;
}

static void
setup_cond_position(char *s)
{
	char *t;

	t = _mbschr(s, '[');
	if (t[0] != '[') {
		return;
	}
	t++;
	if (t[0] == '0' || NOT(isdigit((uchar_t)t[0])) ||
			t[1] == '0' || NOT(isdigit((uchar_t)t[1]))) {
		return;
	}
	mate_position = get_pos(&t);
	if (t[0] == '/') {
		t++;
		if (t[0] == '0' || NOT(isdigit((uchar_t)t[0])) ||
				t[1] == '0' || NOT(isdigit((uchar_t)t[1]))) {
			return;
		}
		mate_counter_position = get_pos(&t);
		if (t[0] == ']') {
			t++;
		}
	} else {
		if (t[0] == ']') {
			t++;
		}
	}
	memset(s, '_', t - s);

	return;
}

void
toasciiuc(char *ib)
{
	char *ib_start = ib;	// save value of ib for nthctype()
	char *s;

	while (ib[0]) {
		if (isascii((uchar_t)ib[0])) {
			if (ib[0] == '"') {
				s = _mbschr(ib + 1, '"');
				if (s != NULL) {
					ib = s + 1;
					continue;
				}
			}
			// ★SJIS
			if (nthctype(ib_start, (int)(ib - ib_start)) == CT_ANK) {
				ib[0] = toupper((uchar_t)ib[0]);
			}
			ib++;
		} else {
			ib += 2;
		}
	}

	return;
}

char *
skip_space(char *ib)
{
	while (ib[0] && (isspace((uchar_t)ib[0]) || EQUALZ(ib, "　"))) {
		// ★SJIS
		ib += isspace((uchar_t)ib[0]) ? 1 : 2;
	}

	return ib;
}

char *
skip_nonspace(char *ib)
{
	while (ib[0] && NOT(isspace((uchar_t)ib[0])) && NEQUALZ(ib, "　")) {
		// ★SJIS
		ib += (isascii((uchar_t)ib[0]) || ISKANA((uchar_t)ib[0])) ? 1 : 2;
	}

	return ib;
}

void
error_message(const char *em, const char *ib)
{
	int epos, len, sum, i;
	char work[256];

	if (ib == NULL) {
		fprintf(stderr, "？？？　%s...\n", em);
	} else {
		epos = (int)(ib - inbuf + 1);
		if (epos < 75) {
			fprintf(stderr, "？？？  %s...\n%s\n%*c\n", em, inbuf, (ib - inbuf + 1), '*');
		} else {
			fprintf(stderr, "？？？  %s...\n", em);
			sum = 0;
			while (epos >= 75) {
				for (i = 35; i < 75; i++) {
					_mbsncpy(work, inbuf + sum, i);
					work[i] = '\0';
					len = (int)strlen(work);
					if (len > 74) {
						break;
					}
				}
				fprintf(stderr, "%s\n", work);
				epos -= len;
				sum += len;
			}
			fprintf(stderr, "%s\n%*c\n", inbuf + sum, epos, '*');
		}
	}

	return;
}

int
is_symmetric_normal(phase_t *php)
{
	if (NEQUALM(&php->board[27], &php->board[45], sizeof(php->board[0]) * 9) ||
			NEQUALM(&php->board[18], &php->board[54], sizeof(php->board[0]) * 9) ||
			NEQUALM(&php->board[9], &php->board[63], sizeof(php->board[0]) * 9) ||
			NEQUALM(&php->board[0], &php->board[72], sizeof(php->board[0]) * 9)) {
		php->state &= ~B_SYMMETRIC;

		return FALSE;
	}
	php->state |= B_SYMMETRIC;

	return TRUE;
}

int
is_symmetric_circe(phase_t *php)
{
	return FALSE;
}

int
make_move(phase_t *php, char *ob)
{
	move_t *mvp = &php->move;
	int len;

	if (mvp->to == NOP || mvp->pc == NUL) {
		return 0;
	}
	if (mvp->to == (php - 1)->move.to && mvp->from != NOP) {
		strcpy(ob, "同");
	} else if (mvp->to != mvp->from || mvp->to_1st != NOP) {
		if (mvp->to_1st == NOP) {
			sprintf(ob, "%d", PR_POS(mvp->to));
		} else {
			sprintf(ob, "%d-%d", PR_POS(mvp->to_1st), PR_POS(mvp->to));
		}
	} else {
		sprintf(ob, "%d", PR_POS(mvp->to));
	}

	if (IS_YO_TURN(php) && mvp->pc == PC_OU) {
		strcat(ob, graph_yo_gy);
	} else {
		if ((piece[mvp->pc].attr & B_PA_ROYAL) && (piece[mvp->pc].attr & B_PA_SAME_FACE)) {
			strcat(strcat(ob, "$"), graph[mvp->pc]);
		} else {
			strcat(ob, graph[mvp->pc]);
		}
	}

	if (mvp->from != NOP) {
		if (mvp->flag & B_MV_PROMOTE) {
			strcat(ob, "成");
		} else if (IS_PROMOTABLE(mvp->pc) && NOT(mvp->flag2 & B_MV2_EXCHANGE) &&
				(IS_HI_TURN(php) ?
                    IS_HI_PROMOTABLE_MOVE(mvp->to, mvp->from) :
                    IS_YO_PROMOTABLE_MOVE(mvp->to, mvp->from))) {
			strcat(ob, "生");
		}
		if (mvp->to != mvp->from || mvp->to_1st != NOP) {
			if (mvp->flag2 & (B_MV2_REBORN | B_MV2_REBORN_1ST | B_MV2_EXCHANGE)) {
				make_reborn(php, mvp, ob);
			} else {
				if (mvp->flag2 & B_MV2_TURN) {
					strcat(ob, "転");
				}
				len = (int)strlen(ob);
				sprintf(ob + len, "<%d", PR_POS(mvp->from));
			}
		} else {
			strcat(ob, "居");
		}
	} else {
		strcat(ob, "打");
	}
	len = (int)strlen(ob);

	return len;
}

void
reg_rule(char *b)
{
	b[0] = '\0';
	if (cond_flag & B_C_GREED) {
		strcat(b, "強欲");
	} else if (cond_flag & B_C_ASCETIC) {
		strcat(b, "禁欲");
	} else if (cond_flag & B_C_NOCAPT) {
		strcat(b, "取禁");
	} else if (cond_flag & B_C_HITHER_NOCAPT) {
		strcat(b, "攻方取禁");
	}
	if (cond_flag & B_C_NOPROMOTE) {
		strcat(b, "成禁");
	} else if (cond_flag & B_C_ABSOLUTE_NOPROMOTE) {
		strcat(b, "絶対成禁");
	}
	if (cond_flag & B_C_ANNAN) {
		strcat(b, "安南");
	} else if (cond_flag & B_C_ANHOKU) {
		strcat(b, "安北");
	} else if (cond_flag & B_C_TAIMEN) {
		strcat(b, "対面");
	} else if (cond_flag & B_C_HAIMEN) {
		strcat(b, "背面");
	} else if (cond_flag & B_C_NEKOSEN) {
		strcat(b, "ネコ鮮");
	} else if (cond_flag & B_C_NEKO2SEN) {
		strcat(b, "ネコネコ鮮");
	}
	if (cond_flag & B_C_CIRCE) {
		strcat(b, "キルケ");
	} else if (cond_flag & B_C_ANTICIRCE) {
		strcat(b, "アンチキルケ");
	} else if (cond_flag & B_C_PWC) {
		strcat(b, "PWC");
	}
	if (cond_flag & B_C_KOKO) {
		if (isalnum((uchar_t)b[strlen(b) - 1])) {
			strcat(b, "-");
		}
		strcat(b, "Koko");
	}
	if (cond_flag & BM_C_MESSIGNY_FAMILY) {
		if (isalnum((uchar_t)b[strlen(b) - 1])) {
			strcat(b, "-");
		}
		strcat(b, "Messigny");
		if (cond_flag & B_C_MESSIGNY_E) {
			strcat(b, "（欧州式）");
		} else if (cond_flag & B_C_MESSIGNY_J) {
			strcat(b, "（日本式）");
		}
	}
	if (cond_flag & B_C_ANDERNACH) {
		if (isalnum((uchar_t)b[strlen(b) - 1])) {
			strcat(b, "-");
		}
		strcat(b, "Andernach");
	} else if (cond_flag & B_C_ANTIANDERNACH) {
		if (isalnum((uchar_t)b[strlen(b) - 1])) {
			strcat(b, "-");
		}
		strcat(b, "AntiAndernach");
	}
	if (cond_flag & B_C_MADRASI) {
		strcat(b, "マドラシ");
	} else if (cond_flag & B_C_K_MADRASI) {
		strcat(b, "Ｋマドラシ");
	} else if (cond_flag & B_C_ISARDAM) {
		if (isalnum((uchar_t)b[strlen(b) - 1])) {
			strcat(b, "-");
		}
		strcat(b, "Isardam");
		if (NOT(cond_flag & B_C_CONSTRAINT_1ST)) {
			strcat(b, "（タイプB）");
		}
	}
	if (cond_flag & B_C_UCHIFU) {
		strcat(b, "打歩");
	} else if (cond_flag & B_C_UCHIFU_OK) {
		strcat(b, "打歩可");
	}
	if (cond_flag & B_C_W_GY) {
		strcat(b, "二玉");
	}
	if (rule_flag & B_R_DIRECT_MATE) {
		strcat(b, "かしこ詰");
	} else if (rule_flag & B_R_HELP_SELF_STALEMATE) {
		strcat(b, "協力自玉ステイルメイト");
	} else if (rule_flag & B_R_HELP_SELF_MATE) {
		strcat(b, "協力自玉詰");
	} else if (rule_flag & B_R_HELP_STALEMATE) {
		strcat(b, "協力ステイルメイト");
	} else {
		strcat(b, "協力詰");
	}
	if (cond_flag & B_C_INVALID_CAPT_OU_2F) {
		strcat(b, "（二歩玉獲無効）");
	}
	if (cond_flag & B_C_POSITION) {
		sprintf(b + strlen(b), "王位置[%d", PR_POS(mate_position));
		if (mate_counter_position != NOP) {
			sprintf(b + strlen(b), "/%d", PR_POS(mate_counter_position));
		}
		strcat(b, "]");
	}

	return;
}

#if 0
char *
get_move(char *ib, move_t *mvp)
{
	char *save_ib = ib;

	mvp->to_1st = NOP;
	mvp->flag = 0x00;
	mvp->flag2 = 0x00;
	if (EQUALZ(ib, "同")) {
		mvp->to = NOP;
		mvp->flag |= B_MV_SAME;
		ib += STR_SIZE_OF("同");
	} else {
		mvp->to = get_pos(&ib);
	}
	if (ib[0] == '-') {
		ib++;
		mvp->to_1st = mvp->to;
		mvp->to = get_pos(&ib);
	}
	mvp->pc = get_pc_code(&ib);
	if (EQUALZ(ib, "打")) {
		mvp->from = NOP;
		ib += STR_SIZE_OF("打");
	} else if (EQUALZ(ib, "居")) {
		mvp->from = mvp->to;
		ib += STR_SIZE_OF("居");
	} else {
		if (EQUALZ(ib, "成")) {
			mvp->flag |= B_MV_PROMOTE;
			ib += STR_SIZE_OF("成");
		} else if (EQUALZ(ib, "生")) {
			mvp->flag |= B_MV_NOT_PROMOTE;
			ib += STR_SIZE_OF("生");
		}
		if (ib[0] == '<') {
			ib++;
			mvp->from = get_pos(&ib);
		}
	}

	return skip_nonspace(ib);
}
#endif

int
get_pos(char **ib)
{
	int pos;

	pos = ((*ib)[0] - '1') * 9 + (*ib)[1] - '1';
	*ib += 2;

	return pos;
}

void
del_file(void)
{
	del_file_t *ent = del_file_list;

	while (ent != NULL) {
		remove(ent->name);
		ent = ent->next;
	}

	return;
}

void
reg_prob(phase_t *php, phase_t *php2, char *b, int use_brief_problem_output)
{
	char *s;	// start addr.

	s = b;
	reg_rule(s);
	strcat(s, " ");
	reg_prob2(php, s);
	strcat(s, "#");
	if (max_limit_depth > 0) {
		sprintf(s + strlen(s), "#%d-%d", limit_depth, max_limit_depth);
	} else {
		sprintf(s + strlen(s), "%d", limit_depth);
	}
	strcat(s, "\n");

	return;
}

static void
reg_prob2(phase_t *php, char *s)
{
	int first, pos, pc, i;

	for (pos = 0; pos < 81; pos++) {
		if (IS_HI_PC(php->board[pos])) {
			pc = KIND(php->board[pos]);
			if ((piece[pc].attr & B_PA_ROYAL) && (piece[pc].attr & B_PA_SAME_FACE)) {
				sprintf(s + strlen(s), "%d$%s", PR_POS(pos), graph[pc]);
			} else {
				sprintf(s + strlen(s), "%d%s", PR_POS(pos), graph[pc]);
			}
		}
	}
	for (i = 0, first = -1; NOT_NUL(hand_pc_order[i]); i++) {
		pc = hand_pc_order[i];
		if (php->hi_hand[pc] >= 1) {
			if (first != 0) {
				first = 0;
				strcat(s, " +");
			}
			strcat(s, graph[pc]);
			if (php->hi_hand[pc] >= 2) {
				sprintf(s + strlen(s), "%d", php->hi_hand[pc]);
			}
		}
	}
	strcat(s, ", ");
	for (pos = 0; pos < 81; pos++) {
		if (IS_YO_PC(php->board[pos]) || IS_ROCK(php->board[pos]) || IS_PIT(php->board[pos])) {
			pc = KIND(php->board[pos]);
			if ((piece[pc].attr & B_PA_ROYAL)
					&& (piece[pc].attr & B_PA_SAME_FACE)) {
				sprintf(s + strlen(s), "%d$%s", PR_POS(pos), graph[pc]);
			} else {
				sprintf(s + strlen(s), "%d%s", PR_POS(pos), graph[pc]);
			}
		}
	}
	strcat(s, " +");
	for (i = 0, first = -1; NOT_NUL(hand_pc_order[i]); i++) {
		pc = hand_pc_order[i];
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PLACABLE)) {
			strcat(s, graph[pc]);
			if (php->yo_hand[pc] != 1) {
				sprintf(s + strlen(s), "%d", php->yo_hand[pc]);
			}
		}
	}
	strcat(s, " ");

	return;
}

void
setup_used_pc_count(phase_t *php)
{
	int pos, pc;

	memset(all_used_pc_count, 0, sizeof(all_used_pc_count));
	memset(hi_used_pc_count, 0, sizeof(hi_used_pc_count));
	memset(yo_used_pc_count, 0, sizeof(yo_used_pc_count));
	for (pos = 0; pos < 81; pos++) {
		pc = RAW_KIND(KIND(php->board[pos]));
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PIECE)) {
			all_used_pc_count[pc]++;
			if (IS_HI_PC(php->board[pos])) {
				hi_used_pc_count[pc]++;
			} else {
				yo_used_pc_count[pc]++;
			}
		}
	}
	for (pc = PC_FU; pc < RAW_PC_END; pc++) {
		if ((piece[pc].attr & B_PA_DEFINED) && (piece[pc].attr & B_PA_PIECE)) {
			if (piece[pc].attr & B_PA_PLACABLE) {
				all_used_pc_count[pc] += php->hi_hand[pc] + php->yo_hand[pc];
				hi_used_pc_count[pc] += php->hi_hand[pc];
				yo_used_pc_count[pc] += php->yo_hand[pc];
			} else {
				php->yo_hand[pc] = 0;
			}
			null_phase.yo_hand[pc] = all_used_pc_count[pc];
		}
	}

	return;
}

char *
make_num_str(long unum, long lnum, char *buf)
{
	int len, i;

	if (unum == 0) {
		sprintf(buf, "%ld", lnum);
	} else {
		sprintf(buf, "%ld%06ld", unum, lnum);
	}
	len = (int)strlen(buf);
	for (i = len - 3; i > 0; i -= 3) {
		memmove(buf + i + 1, buf + i, strlen(buf + i) + 1);
		buf[i] = ',';
	}

	return buf;
}

int
bit_count_ulong(ulong_t x)
{
	ulong_t y = x;
	int n = 0;

	while (y != 0x0) {
		if (y & 1) {
			n++;
		}
		y >>= 1;
	}

	return n;
}

int
bit_count_ulonglong(ulonglong_t x)
{
	ulonglong_t y = x;
	int n = 0;

	while (y != 0x0) {
		if (y & 1) {
			n++;
		}
		y >>= 1;
	}

	return n;
}

void
make_reborn_pwc(phase_t *php, move_t *mvp, char *ob)
{
	int len, from;
	uchar_t pc;

	len = (int)strlen(ob);
	sprintf(ob + len, "<%d", PR_POS(mvp->from));
	if (mvp->flag2 & B_MV2_REBORN_1ST) {
		len = (int)strlen(ob);
		pc = KIND(php->board[mvp->to_1st]);
		sprintf(ob + len, "/%d%s", PR_POS(mvp->from), graph[pc]);
	}
	if (mvp->flag2 & B_MV2_REBORN) {
		len = (int)strlen(ob);
		from = mvp->to_1st == NOP ? mvp->from : mvp->to_1st;
		pc = KIND(php->board[mvp->to]);
		sprintf(ob + len, "/%d%s", PR_POS(from), graph[pc]);
	}

	return;
}

void
make_reborn_circe(phase_t *php, move_t *mvp, char *ob)
{
	int len;
	uchar_t pc;

	len = (int)strlen(ob);
	sprintf(ob + len, "<%d", PR_POS(mvp->from));
	if (mvp->flag2 & B_MV2_REBORN_1ST) {
		len = (int)strlen(ob);
		pc = RAW_KIND(php->board[mvp->to_1st]);
		sprintf(ob + len, "/%d%s", PR_POS(mvp->reborn_1st), graph[pc]);
	}
	if (mvp->flag2 & B_MV2_REBORN) {
		len = (int)strlen(ob);
		pc = RAW_KIND(php->board[mvp->to]);
		sprintf(ob + len, "/%d%s", PR_POS(mvp->reborn), graph[pc]);
	}

	return;
}

void
make_reborn_anticirce(phase_t *php, move_t *mvp, char *ob)
{
	int len;
	uchar_t pc;

	len = (int)strlen(ob);
	sprintf(ob + len, "<%d", PR_POS(mvp->from));
	if (mvp->flag2 & B_MV2_REBORN_1ST) {
		pc = mvp->pc;
		if (mvp->flag & B_MV_PROMOTE) {
			pc |= B_PC_PROMOTED;
		}
		len = (int)strlen(ob);
		if ((piece[pc].attr & B_PA_ROYAL) && (piece[pc].attr & B_PA_SAME_FACE)) {
			sprintf(ob + len, "/%d$%s", PR_POS(mvp->reborn_1st), graph[pc]);
		} else {
			sprintf(ob + len, "/%d%s", PR_POS(mvp->reborn_1st), graph[pc]);
		}
	}
	if (mvp->flag2 & B_MV2_REBORN) {
		pc = mvp->pc;
		if (mvp->flag & B_MV_PROMOTE) {
			pc |= B_PC_PROMOTED;
		}
		len = (int)strlen(ob);
		if ((piece[pc].attr & B_PA_ROYAL) && (piece[pc].attr & B_PA_SAME_FACE)) {
			sprintf(ob + len, "/%d$%s", PR_POS(mvp->reborn), graph[pc]);
		} else {
			sprintf(ob + len, "/%d%s", PR_POS(mvp->reborn), graph[pc]);
		}
	}

	return;
}

void
make_reborn_exchange(phase_t *php, move_t *mvp, char *ob)
{
	int len;

	len = (int)strlen(ob);
	if (mvp->flag2 & B_MV2_EXCHANGE) {
		sprintf(ob + len, "<>%d", PR_POS(mvp->from));
	} else {
		sprintf(ob + len, "<%d", PR_POS(mvp->from));
	}

	return;
}

