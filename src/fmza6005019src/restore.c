// zaco/restore.c -- save file restoring routines of FM solver
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/restore.c 1.4 2014/08/07 07:47:50 budai10 Exp budai10 $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<mbstring.h>
#include	<time.h>
#include	<sys/stat.h>
#include	"fm.h"
#include	"piece.h"

static void analyze_last_record(char *buf);

// 再開用設定 & 配置

int
restore(void)
{
	int c, d, i;

	setvbuf(restartFp, NULL, _IOFBF, 0x40000);
	arrange(restartFp);
	if (fgets(inbuf, sizeof(inbuf), restartFp) == NULL || NEQUALZ(inbuf, "解析時間：")) {
		fprintf(stderr, "退避ファイル異常（解析時間）...\n");
		exit(1);
	}
	analyze_last_record(inbuf);
	if (fgets(inbuf, sizeof(inbuf), restartFp) == NULL || NEQUALZ(inbuf, "最終検索手順：")) {
		fprintf(stderr, "退避ファイル異常（最終検索手順）...\n");
printf("%s\n", inbuf);
		exit(1);
	}
	// 以下、バイナリデータ復元
	freopen(restart_file, "rb", restartFp);
	while ((c = fgetc(restartFp)) != '\x1a' && c != EOF)
		;
	// 最終検索手順復元
	fread(inbuf, 1, STR_SIZE_OF("#MV#"), restartFp);
	if (NEQUALZ(inbuf, "#MV#")) {
		fprintf(stderr, "退避ファイル異常（最終検索手順復元前）...\n");
		exit(1);
	}
	last_move[0] = null_move; 
	last_move[0].from = -1;
	last_move[0].to = 0;
	fread(inbuf, 1, sizeof(ushort_t), restartFp);
	d = *(ushort_t *)inbuf;
	for (i = 1; i < d; i++) {
		fread(&last_move[i], 1, sizeof(move_t), restartFp);
	}
	fread(inbuf, 1, STR_SIZE_OF("#MV#"), restartFp);
	if (NEQUALZ(inbuf, "#MV#")) {
		fprintf(stderr, "退避ファイル異常（最終検索手順復元後）...\n");
		exit(1);
	}
	fread(inbuf, 1, STR_SIZE_OF("#XX#"), restartFp);
	if (EQUALZ(inbuf, "#HE#")) {
		// 攻方逃れ局面復元
		escape_hash_t *ent, *prev;
		long i;
		LOOP {
			fread(inbuf, 1, sizeof(i), restartFp);
			if (EQUALZ(inbuf, "#HE#")) {
				break;
			}
			i = *(long *)inbuf;
			prev = (escape_hash_t *)&hi_eh_head[i];
			LOOP {
				fread(inbuf, 1, sizeof(ushort_t), restartFp);
				if (*(ushort_t *)inbuf == 0) {
					break;
				}
				ent = hi_eh_free;
				hi_eh_free = ent->next;
				ent->next = prev->next;
				prev->next = ent;
				ent->depth = *(ushort_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->hi_pieces = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->yo_pieces = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->hi_hands = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ushort_t), restartFp);
				ent->ref_count = *(ushort_t *)inbuf;
			}
		}
		fread(inbuf, 1, STR_SIZE_OF("#XX#"), restartFp);
	}
	if (EQUALZ(inbuf, "#YE#")) {
		// 受方逃れ局面復元
		escape_hash_t *ent, *prev;
		long i;
		LOOP {
			fread(inbuf, 1, sizeof(i), restartFp);
			if (EQUALZ(inbuf, "#YE#")) {
				break;
			}
			i = *(long *)inbuf;
			prev = (escape_hash_t *)&yo_eh_head[i];
			LOOP {
				fread(inbuf, 1, sizeof(ushort_t), restartFp);
				if (*(ushort_t *)inbuf == 0) {
					break;
				}
				ent = yo_eh_free;
				yo_eh_free = ent->next;
				ent->next = prev->next;
				prev->next = ent;
				ent->depth = *(ushort_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->hi_pieces = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->yo_pieces = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ulong_t), restartFp);
				ent->hi_hands = *(ulong_t *)inbuf;
				fread(inbuf, 1, sizeof(ushort_t), restartFp);
				ent->ref_count = *(ushort_t *)inbuf;
			}
		}
		fread(inbuf, 1, STR_SIZE_OF("#XX#"), restartFp);
	}
	fclose(restartFp);

	return 0;
}

static void 
analyze_last_record(char *buf)
{
	char *s, *t;
	int n;

	s = buf + STR_SIZE_OF("解析時間：");
	n = strtol(s, &t, 10);
	if (EQUALZ(t, "時間")) {
		total_elapse += n * 60 * 60;
		s = t + STR_SIZE_OF("時間 ");
		n = strtol(s, &t, 10);
	}
	if (EQUALZ(t, "分")) {
		total_elapse += n * 60;
		s = t + STR_SIZE_OF("分 ");
		n = strtol(s, &t, 10);
	}
	if (EQUALZ(t, "秒")) {
		total_elapse += n;
		s = t + STR_SIZE_OF("秒 ");
	}
	base_elapse = last_elapse = total_elapse;
	s = skip_space(s) + STR_SIZE_OF("解析局面数：");
	phase_count += strtol(s, &t, 10);
	while (t[0] == ',') {
		phase_1M *= 1000;
		phase_count *= 1000;
		s = t + 1;
		phase_count += strtol(s, &t, 10);
		phase_1M += phase_count / (1000 * 1000);
		phase_count %= 1000 * 1000;
	}
	s = skip_space(t) + STR_SIZE_OF("検出解数：");
	solution_count += strtol(s, &t, 10);

	return;
}

