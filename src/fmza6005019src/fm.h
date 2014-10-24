// Fairy Mate 6.xxx.xxx
// 6aco/fm.h -- FM main header file
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/fm.h 1.49 2014/08/16 14:01:48 budai10 Exp budai10 $

#if !defined(FM_H)
#define FM_H

//   hi - hither（先手）
//   yo - yonder（後手）

#include	<stdio.h>
#include	<stdlib.h>
#include	<time.h>
#include	"ansiesc.h"
#undef	ERASE_EOS
#define ERASE_EOS()		erase_eos_win32(hStdout)
#include	"console.h"

#define	RAW_PC_END	(16)

typedef unsigned long long ulonglong_t;
typedef unsigned long ulong_t;
typedef unsigned short ushort_t;
typedef unsigned char uchar_t;

//#if defined(_DEBUG)
#if 1	// for debug
#define	ASSERT(b)		{\
	if (!(b)) {\
		fprintf(stderr, "\n%s(%d): " #b "\n", __FILE__, __LINE__);\
		exit(255);\
	}\
}
#else
#define	ASSERT(b)
#endif

// additional signal code
#define	SIGQUIT				(3)
#define	SIGALRM				(1)

// switch value
#define OFF					(0x00)
#define ON					(0xff)
#if !defined(TRUE) || !defined(FALSE)
#define TRUE				(-1)
#define FALSE				(0)
#endif

// return code (for direct)
#define	RC_FAIL				(0)		// 反則（処理対象外を含む）
#define	RC_ESCAPE			(1)		// 逃れ（中断を含む）
#define	RC_MATE				(2)		// 詰

// limit (or maximum) value
#define PROBSIZE			(1024)
#define LIMIT_DEPTH			(60000)
#define TITLESIZE			(256)
#define PNAMEMAX			(256)
#define FNAMEMAX			(256)
#define NOEHDEFAULT			(1000000)
#if defined(_M_X64)
#define NOEHLIMIT 			(2000000000L)
#else
#define NOEHLIMIT 			(80000000L)
#endif
#define NOSPHHEAD			(128)
#define NOMATEMOVE			(100000)

// arrange mode
#define HI_PC_ON_BOARD		(1)
#define HI_PC_IN_HAND		(2)
#define YO_PC_ON_BOARD		(3)
#define YO_PC_IN_HAND		(4)
#define MATE_MOVES			(5)

// mate-rule flags
#define B_R_DIRECT_MATE		(0x80000000)	// かしこ詰（手数指定、無駄合なし）
#define BM_R_DIRECT			(0x80000000)	// かしこ系
#define B_R_HELP_MATE		(0x08000000)	// 協力詰
#define B_R_HELP_STALEMATE	(0x04000000)	// 協力ステイルメイト
#define B_R_HELP_SELF_MATE	(0x02000000)	// 協力自玉詰
#define B_R_HELP_SELF_STALEMATE \
							(0x01000000)	// 協力自玉ステイルメイト
#define BM_R_HELP			(0x0f000000)	// 協力系

// mate-condition flags
#define	B_C_NOPROMOTE		(0x8000000000000000)	// 成禁
#define	B_C_ABSOLUTE_NOPROMOTE \
							(0x4000000000000000)	// 絶対成禁
#define	B_C_NOCAPT			(0x0800000000000000)	// 取禁
#define	B_C_ABSOLUTE_NOCAPT	(0x0400000000000000)	// 絶対取禁（現在、未実装）
#define	B_C_HITHER_NOCAPT	(0x0100000000000000)	// 攻方取禁
#define	B_C_UCHIFU			(0x0080000000000000)	// 打歩詰
#define	B_C_UCHIFU_OK		(0x0040000000000000)	// 打歩詰可
#define	B_C_UCHI			(0x0020000000000000)	// 打駒詰（現在、未実装）
#define	B_C_POSITION		(0x0008000000000000)	// 王位置
#define	B_C_JUST_MATE		(0x0004000000000000)	// 限定詰（現在、未実装）
#define	B_C_W_GY			(0x0002000000000000)	// 二玉詰
#define	B_C_GREED			(0x0000800000000000)	// 強欲
#define	B_C_ASCETIC 		(0x0000400000000000)	// 禁欲
#define	B_C_ANNAN			(0x0000080000000000)	// 安南
#define	B_C_ANHOKU			(0x0000040000000000)	// 安北
#define	B_C_TAIMEN			(0x0000020000000000)	// 対面
#define	B_C_HAIMEN			(0x0000010000000000)	// 背面
#define	B_C_NEKOSEN			(0x0000008000000000)	// ネコ鮮
#define	B_C_NEKO2SEN		(0x0000004000000000)	// ネコネコ鮮
#define	BM_C_CHANGE_FAMILY	(0x00000fc000000000)	// 変身系
#define	B_C_TENJIKU			(0x0000002000000000)	// 天竺（現在、未実装）
#define	B_C_CIRCE			(0x0000000800000000)	// キルケ
#define	B_C_ANTICIRCE		(0x0000000400000000)	// アンチキルケ
#define	B_C_PWC				(0x0000000200000000)	// PWC
#define	BM_C_CIRCE_FAMILY	(0x0000000e00000000)	// キルケ系
#define	B_C_ANDERNACH		(0x0000000080000000)	// Andernach
#define	B_C_ANTIANDERNACH	(0x0000000040000000)	// AntiAndernach
#define	B_C_MADRASI			(0x0000000008000000)	// マドラシ
#define	B_C_K_MADRASI		(0x0000000004000000)	// Ｋマドラシ
#define	BM_C_MADRASI_FAMILY	(0x000000000c000000)	// マドラシ系
#define	B_C_ISARDAM			(0x0000000000800000)	// Isardam
#define	B_C_ISARDAM_K		(0x0000000000400000)	// Isardam-K（現在、未実装）
#define	BM_C_ISARDAM_FAMILY	(0x0000000000c00000)	// Isardam系
#define	B_C_KOKO			(0x0000000000200000)	// Koko
#define	B_C_INVALID_CAPT_OU_2F \
							(0x0000000000100000)	// 二歩王獲無効
#define	BM_C_PHASE_CONSTRAINT_FAMILY \
							(0x0000000000f00000)	// 局面制約系
#define	B_C_MESSIGNY		(0x0000000000080000)	// Messigny
#define	B_C_MESSIGNY_E		(0x0000000000040000)	// Messigny（欧州式）
#define	B_C_MESSIGNY_J		(0x0000000000020000)	// Messigny（日本式）
#define	BM_C_MESSIGNY_FAMILY \
							(0x00000000000e0000)	// Messigny系
#define	B_C_CONSTRAINT_1ST	(0x0000000000000001)	// 制約優先（玉獲りに優先）

// strategy flags group
#define	B_HI_DENY_SAME		(0x80000000)
#define	B_YO_DENY_SAME		(0x40000000)
#define	B_HI_ESCAPE			(0x20000000)
#define	B_YO_ESCAPE			(0x10000000)
#define	B_YO_ESCAPE_2		(0x02000000)
#define	B_HI_ALLOW_SAME		(0x00800000)
#define	B_YO_ALLOW_SAME		(0x00400000)

// hint flags（現在未実装）
#define	B_H_F_NO_HAND_FU	(0x80000000)
#define	B_H_F_NO_HAND		(0x40000000)
#define	B_H_F_NO_HAND_FU2	(0x20000000)

// phase states
#define B_HITHER_TURN		(0x80)		// ON: 攻方番、OFF: 受方番
#define B_SYMMETRIC			(0x40)
#define B_CHECK_MOVE_FOUND	(0x20)
#define B_ESCAPE_MOVE_FOUND	(0x10)
#define B_MODE_GREED		(0x08)
#define B_MODE_ASCETIC		(0x04)

// check states
#define B_CS_HITHER_CHECK	(0x20)		// 攻方の王手
#define B_CS_YONDER_CHECK	(0x10)		// 受方の王手
#define B_CS_CLOSE_CHECK	(0x08)
#define B_CS_REMOTE_CHECK	(0x04)
#define B_CS_CHECK_PALALYTIC \
							(0x02)

// fire states
#define B_HI_FIRE_PATH		(0x80)		// 攻方の利きの路（利きがあるわけではない）
#define B_YO_FIRE_PATH		(0x40)		// 受方の利きの路（利きがあるわけではない）
#define B_CHECK				(0x20)		// 当該位置の駒が相手王に利きをもつ
#define B_PALALYTIC			(0x10)		// 当該位置の駒がマヒ状態
#define B_HI_CHECK_SPECIAL	(0x08)		// 攻方駒打ち王手候補位置（変身系、マドラシ）
#define B_YO_CHECK_SPECIAL	(0x04)		// 受方駒打ち王手候補位置（変身系、マドラシ）
#define B_HI_ESCAPE_SPECIAL	(0x02)		// 攻方駒打ち王手回避手候補位置（変身系、マドラシ）
#define B_YO_ESCAPE_SPECIAL	(0x01)		// 受方駒打ち王手回避手候補位置（変身系、マドラシ）

// move flags
#define B_MV_PROMOTE		(0x80)
#define B_MV_SHUTOFF		(0x40)
#define B_MV_NOT_PROMOTE	(0x20)		// 指定手順で使用
#define B_MV_TESTED			(0x10)		// 指定手順で使用。現在未使用
#define B_MV_MATE			(0x08)
#define B_MV_MATESEQ		(0x04)
#define B_MV_SAME			(0x02)
//							(0x01)
#define B_MV2_REBORN		(0x80)		// 復活
#define B_MV2_REBORN_1ST	(0x40)		// 復活（２手移動の１手目に対する）
#define B_MV2_REBORN_MORE	(0x20)		// 選択復活残あり
#define B_MV2_TURN			(0x10)		// 駒反転
#define B_MV2_EXCHANGE		(0x08)		// 駒交換

// mate-move flags
#define B_MM_PROMOTE		(0x80)		// 成
#define B_MM_DEL			(0x10)		// 削除（予定）
#define B_MM_HITHER_TURN	(0x08)		// 攻方番
#define B_MM_MATE			(0x04)		// 詰
#define B_MM_SHORT_MATE		(0x02)		// 早詰
#define B_MM_EXCESS_MATE	(0x01)		// 駒余り詰

// L option switch flags
#define	B_LF_SOL_LIMIT		(0x80)
#define	B_LF_PIECE_LEFT		(0x40)
#define	B_LF_SHORT_MATE		(0x20)
#define	B_LF_NO_LIMIT		(0x10)
#define	B_LF_NO_PRINT		(0x08)
#define	B_LF_TIME_LIMIT		(0x04)

// processing constants
#define DANCE_TIMING		(5)

// macros
#define LOOP				for ( ; ; )
#define NOT(a)				(!(a))
#define	ARRAY_SIZE_OF(a)	(sizeof(a)/sizeof(*(a)))
#define	STR_SIZE_OF(s)		(sizeof(s) - 1)

#define EQUAL(s, t)			(strcmp((s), (t)) == 0)
#define EQUALN(s, t, n)		(strncmp((s), (t), (n)) == 0)
#define EQUALZ(s, t)		(strncmp((s), (t), STR_SIZE_OF(t)) == 0)
#define EQUALS(s, t)		(strncmp((s), (t), strlen(t)) == 0)
#define EQUALM(a, b, n)		(memcmp((a), (b), (n)) == 0)
#define NEQUAL(s, t)		(strcmp((s), (t)) != 0)
#define NEQUALN(s, t, n)	(strncmp((s), (t), (n)) != 0)
#define NEQUALZ(s, t)		(strncmp((s), (t), STR_SIZE_OF(t)) != 0)
#define NEQUALS(s, t)		(strncmp((s), (t), strlen(t)) != 0)
#define NEQUALM(a, b, n)	(memcmp((a), (b), (n)) != 0)

#define IS_HI_TURN(ph)		((ph)->state & B_HITHER_TURN)
#define IS_YO_TURN(ph)		(NOT((ph)->state & B_HITHER_TURN))

#define	SPACE()				printf("\n")
#define	SPACE2()			printf("\n\n")

// 詰手構造体
typedef struct {
	uchar_t pc;
	uchar_t fpc;				// 利き駒種
	char to;
	char from;
	char reborn;
	char to_1st;				// 獅子、蝗用
	char reborn_1st;
	uchar_t flag;
	uchar_t flag2;
} move_t;

// 局面構造体
typedef struct {
	ulong_t hash_hi_pieces;		// 局面情報
	ulong_t hash_yo_pieces;		// 局面情報
	ulong_t hash_hi_hands;		// 局面情報
	short hi_hand[RAW_PC_END];	// 局面情報
	short yo_hand[RAW_PC_END];	// 局面情報
	uchar_t state;				// 局面情報
	uchar_t check_state;		// 局面情報
	char hi_ou_pos;				// 局面情報
	char yo_gy_pos;				// 局面情報
	move_t move;				// 局面情報
	uchar_t board[81];			// 局面情報
	uchar_t fire[81];			// 解析情報
	uchar_t fpc_board[81];		// 解析情報（現局面の利き情報）
} phase_t;

// 引用印構造体
typedef struct {
	char *start;
	char *end;
} quote_mark_t;

// ルール・条件構造体
typedef struct {
	char *name;
	ulong_t rule_flag;
	ulonglong_t cond_flag;
} rule_cond_t;

// 逃れ局面構造体
typedef struct escape_hash_ escape_hash_t;
struct escape_hash_ {
	escape_hash_t *next;
	ulong_t hi_pieces;
	ulong_t yo_pieces;
	ulong_t hi_hands;
	ushort_t depth;
	ushort_t ref_count;
};

// 手順中局面構造体
typedef struct seq_phase_hash_ seq_phase_hash_t;
struct seq_phase_hash_ {
	seq_phase_hash_t *next;
	phase_t *php;
};

// 詰手順木用詰手構造体
typedef struct mate_move_ mate_move_t;
typedef struct mate_move_ {
	mate_move_t *next;
	mate_move_t *prev;
	mate_move_t *side;
	ushort_t depth;
	ushort_t max_mate_depth;
	uchar_t pc;
	char to;
	char from;
	char reborn;
	char to_1st;				// 獅子、蝗用
	char reborn_1st;
	uchar_t flag;
	uchar_t flag2;
};

// main.c
extern void fm_exit(int rc);
extern void fm_exit_mae(uchar_t *s);

// fmza.c
extern char cmdname[];
extern rule_cond_t rule_cond[];
extern int (* hi_move)(phase_t *ophp);
extern int (* yo_move)(phase_t *ophp);
extern int (* hi_move_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int (* yo_move_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int (* hi_move_leaf)(phase_t *ophp);
extern int (* yo_move_leaf)(phase_t *ophp);
extern int (* hi_move_leaf_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int (* yo_move_leaf_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int (* test_before_move_test)(phase_t *ophp, move_t *omvp);
extern int (* analyze_phase)(phase_t *php);
extern int (* analyze_leaf_phase)(phase_t *php);
extern void (* setup_fpc_board)(phase_t *php);
extern int (* hi_trial_move)(phase_t *php, uchar_t pc, int from, int to);
extern int (* yo_trial_move)(phase_t *php, uchar_t pc, int from, int to);
extern int (* hi_trial_move2)(phase_t *php, uchar_t pc, int from, int to_1st, int to);
extern int (* yo_trial_move2)(phase_t *php, uchar_t pc, int from, int to_1st, int to);
extern int (* check)(phase_t *php);
extern int (* escape)(phase_t *php);
extern void (* help_check_place)(phase_t *php);
extern void (* help_check_place_sub)(phase_t *php);
extern void (* help_check_move_sub)(phase_t *php);
extern void (* help_escape_place)(phase_t *php);
extern void (* help_escape_check_place)(phase_t *php);
extern void (* help_escape_place_sub)(phase_t *php);
extern void (* help_escape_move_sub)(phase_t *php);
extern void (* help_escape_check_place_sub)(phase_t *php);
extern void (* help_escape_check_move_sub)(phase_t *php);
extern int (* direct_check_place)(phase_t *php);
extern int (* direct_check_place_sub)(phase_t *php);
extern int (* direct_check_move_sub)(phase_t *php);
extern int (* direct_escape_place)(phase_t *php);
extern int (* direct_escape_place_sub)(phase_t *php);
extern int (* direct_escape_move_sub)(phase_t *php);
extern void (* make_reborn)(phase_t *php, move_t *mvp, char *ob);
extern int (* is_symmetric)(phase_t *php);
extern int (* test_symmetric)(phase_t *php);
extern int (* is_hither_mate)(phase_t *php);
extern int (* is_yonder_mate)(phase_t *php);
extern int (* test_hither_mate_place)(phase_t *php);
extern int (* test_yonder_mate_place)(phase_t *php);
extern int (* test_hither_mate_sub)(phase_t *php);
extern int (* test_yonder_mate_sub)(phase_t *php);
extern void (* hi_hash_diff_sub)(phase_t *php, phase_t *php_1, int from, int to);
extern void (* yo_hash_diff_sub)(phase_t *php, phase_t *php_1, int from, int to);

// h_check.c
extern int help_check_normal(phase_t *php);
extern int help_check_messigny(phase_t *php);
extern int help_check_greed(phase_t *php);
extern int help_check_ascetic(phase_t *php);
extern void help_check_all(phase_t *php);
extern void help_check_all_messigny(phase_t *php);
extern void help_check_all_greed(phase_t *php);
extern void help_check_all_ascetic(phase_t *php);
extern void help_check_place_loose(phase_t *php);
extern void help_check_place_normal(phase_t *php);
extern void help_check_leap(phase_t *php);
extern void help_check_shishi_leap(phase_t *php);
extern void help_check_step(phase_t *php);
extern void help_check_mao_step(phase_t *php);
extern void help_check_run(phase_t *php);
extern void help_check_hop(phase_t *php);
extern void help_check_decapt_hop(phase_t *php);
extern void help_check_pao_jump(phase_t *php);
extern void help_check_jump(phase_t *php);
extern void help_check_jump_hop(phase_t *php);
extern void help_check_refl(phase_t *php);
extern void help_check_equi_hop(phase_t *php);
extern void help_check_circle_run(phase_t *php);
extern void help_move_null(phase_t *php);
extern void help_check_place_sub_loose(phase_t *php);
extern void help_check_place_sub_normal(phase_t *php);
extern void help_check_place_sub_madrasi(phase_t *php);
extern void help_check_place_sub_annanhoku(phase_t *php);
extern void help_check_place_sub_taihaimen(phase_t *php);
extern void help_check_place_sub_nekosen(phase_t *php);
extern void help_check_move_sub_loose(phase_t *php);
extern void help_check_move_sub_circe(phase_t *php);
extern void help_check_move_sub_normal(phase_t *php);
extern void help_check_move_sub_normal_circe(phase_t *php);

// h_escape.c
extern int help_escape_normal(phase_t *php);
extern int help_escape_messigny(phase_t *php);
extern int help_escape_greed(phase_t *php);
extern int help_escape_ascetic(phase_t *php);
extern void help_escape_all(phase_t *php);
extern void help_escape_all_messigny(phase_t *php);
extern void help_escape_all_greed(phase_t *php);
extern void help_escape_all_ascetic(phase_t *php);
extern void help_escape_place_loose(phase_t *php);
extern void help_escape_place_normal(phase_t *php);
extern void help_escape_place_taihaink2mad(phase_t *php);
extern void help_escape_leap(phase_t *php);
extern void help_escape_shishi_leap(phase_t *php);
extern void help_escape_step(phase_t *php);
extern void help_escape_mao_step(phase_t *php);
extern void help_escape_run(phase_t *php);
extern void help_escape_hop(phase_t *php);
extern void help_escape_decapt_hop(phase_t *php);
extern void help_escape_pao_jump(phase_t *php);
extern void help_escape_jump(phase_t *php);
extern void help_escape_jump_hop(phase_t *php);
extern void help_escape_refl(phase_t *php);
extern void help_escape_equi_hop(phase_t *php);
extern void help_escape_circle_run(phase_t *php);
extern void help_escape_place_sub_loose(phase_t *php);
extern void help_escape_move_sub_loose(phase_t *php);
extern void help_escape_move_sub_circe(phase_t *php);

// hs_check.c
extern int help_self_check_normal(phase_t *php);
extern int help_self_check_messigny(phase_t *php);
extern int help_self_check_greed(phase_t *php);
extern int help_self_check_ascetic(phase_t *php);
extern void help_self_check_place_sub_loose(phase_t *php);
extern void help_self_check_place_sub_normal(phase_t *php);
extern void help_self_check_place_sub_madrasi(phase_t *php);
extern void help_self_check_place_sub_annanhoku(phase_t *php);
extern void help_self_check_place_sub_taihaimen(phase_t *php);
extern void help_self_check_place_sub_nekosen(phase_t *php);
extern void help_self_check_move_sub_loose(phase_t *php);
extern void help_self_check_move_sub_circe(phase_t *php);
extern void help_self_check_move_sub_normal(phase_t *php);
extern void help_self_check_move_sub_normal_circe(phase_t *php);

// hs_escape.c
extern int help_self_escape_normal(phase_t *php);
extern int help_self_escape_messigny(phase_t *php);
extern int help_self_escape_greed(phase_t *php);
extern int help_self_escape_ascetic(phase_t *php);
extern void help_self_escape_check_place_loose(phase_t *php);
extern void help_self_escape_check_place_normal(phase_t *php);
extern void help_self_escape_check_place_taihaink2mad(phase_t *php);
extern void help_self_escape_place_sub_loose(phase_t *php);
extern void help_self_escape_check_place_sub_loose(phase_t *php);
extern void help_self_escape_check_place_sub_normal(phase_t *php);
extern void help_self_escape_check_place_sub_madrasi(phase_t *php);
extern void help_self_escape_check_place_sub_annanhoku(phase_t *php);
extern void help_self_escape_check_place_sub_taihaimen(phase_t *php);
extern void help_self_escape_check_place_sub_nekosen(phase_t *php);
extern void help_self_escape_move_sub_loose(phase_t *php);
extern void help_self_escape_move_sub_circe(phase_t *php);
extern void help_self_escape_check_move_sub_loose(phase_t *php);
extern void help_self_escape_check_move_sub_circe(phase_t *php);
extern void help_self_escape_check_move_sub_normal(phase_t *php);
extern void help_self_escape_check_move_sub_normal_circe(phase_t *php);

// d_check.c
extern int direct_check_normal(phase_t *php);
extern int direct_check_messigny(phase_t *php);
extern int direct_check_greed(phase_t *php);
extern int direct_check_ascetic(phase_t *php);
extern int direct_check_all(phase_t *php);
extern int direct_check_all_messigny(phase_t *php);
extern int direct_check_all_greed(phase_t *php);
extern int direct_check_all_ascetic(phase_t *php);
extern int direct_check_place_loose(phase_t *php);
extern int direct_check_place_normal(phase_t *php);
extern int direct_check_leap(phase_t *php);
extern int direct_check_shishi_leap(phase_t *php);
extern int direct_check_step(phase_t *php);
extern int direct_check_mao_step(phase_t *php);
extern int direct_check_run(phase_t *php);
extern int direct_check_hop(phase_t *php);
extern int direct_check_decapt_hop(phase_t *php);
extern int direct_check_pao_jump(phase_t *php);
extern int direct_check_jump(phase_t *php);
extern int direct_check_jump_hop(phase_t *php);
extern int direct_check_refl(phase_t *php);
extern int direct_check_equi_hop(phase_t *php);
extern int direct_check_circle_run(phase_t *php);
extern int direct_move_null(phase_t *php);
extern int direct_check_place_sub_loose(phase_t *php);
extern int direct_check_place_sub_normal(phase_t *php);
extern int direct_check_place_sub_madrasi(phase_t *php);
extern int direct_check_place_sub_annanhoku(phase_t *php);
extern int direct_check_place_sub_taihaimen(phase_t *php);
extern int direct_check_place_sub_nekosen(phase_t *php);
extern int direct_check_move_sub_loose(phase_t *php);
extern int direct_check_move_sub_circe(phase_t *php);
extern int direct_check_move_sub_normal(phase_t *php);
extern int direct_check_move_sub_normal_circe(phase_t *php);

// d_escape.c
extern int direct_escape_normal(phase_t *php);
extern int direct_escape_messigny(phase_t *php);
extern int direct_escape_greed(phase_t *php);
extern int direct_escape_ascetic(phase_t *php);
extern int direct_escape_all(phase_t *php);
extern int direct_escape_all_messigny(phase_t *php);
extern int direct_escape_all_greed(phase_t *php);
extern int direct_escape_all_ascetic(phase_t *php);
extern int direct_escape_place_loose(phase_t *php);
extern int direct_escape_place_normal(phase_t *php);
extern int direct_escape_place_taihaink2mad(phase_t *php);
extern int direct_escape_leap(phase_t *php);
extern int direct_escape_shishi_leap(phase_t *php);
extern int direct_escape_step(phase_t *php);
extern int direct_escape_mao_step(phase_t *php);
extern int direct_escape_run(phase_t *php);
extern int direct_escape_hop(phase_t *php);
extern int direct_escape_decapt_hop(phase_t *php);
extern int direct_escape_pao_jump(phase_t *php);
extern int direct_escape_jump(phase_t *php);
extern int direct_escape_jump_hop(phase_t *php);
extern int direct_escape_refl(phase_t *php);
extern int direct_escape_equi_hop(phase_t *php);
extern int direct_escape_circle_run(phase_t *php);
extern int direct_escape_place_sub_loose(phase_t *php);
extern int direct_escape_move_sub_loose(phase_t *php);
extern int direct_escape_move_sub_circe(phase_t *php);

// sub3.c
extern void set_entry(void);

// mate.c
extern int is_hither_mate_normal(phase_t *php);
extern int is_hither_mate_messigny(phase_t *php);
extern int is_yonder_mate_normal(phase_t *php);
extern int is_yonder_mate_messigny(phase_t *php);
extern int is_hither_stalemate(phase_t *php);
extern int is_yonder_stalemate(phase_t *php);
extern int test_hither_mate_place_loose(phase_t *php);
extern int test_hither_mate_place_normal(phase_t *php);
extern int test_hither_mate_place_taihaink2mad(phase_t *php);
extern int test_hither_mate_leap(phase_t *php);
extern int test_hither_mate_shishi_leap(phase_t *php);
extern int test_hither_mate_step(phase_t *php);
extern int test_hither_mate_mao_step(phase_t *php);
extern int test_hither_mate_run(phase_t *php);
extern int test_hither_mate_hop(phase_t *php);
extern int test_hither_mate_decapt_hop(phase_t *php);
extern int test_hither_mate_pao_jump(phase_t *php);
extern int test_hither_mate_jump(phase_t *php);
extern int test_hither_mate_jump_hop(phase_t *php);
extern int test_hither_mate_refl(phase_t *php);
extern int test_hither_mate_equi_hop(phase_t *php);
extern int test_yonder_mate_place_loose(phase_t *php);
extern int test_yonder_mate_place_normal(phase_t *php);
extern int test_yonder_mate_place_taihaink2mad(phase_t *php);
extern int test_yonder_mate_leap(phase_t *php);
extern int test_yonder_mate_shishi_leap(phase_t *php);
extern int test_yonder_mate_step(phase_t *php);
extern int test_yonder_mate_mao_step(phase_t *php);
extern int test_yonder_mate_run(phase_t *php);
extern int test_yonder_mate_hop(phase_t *php);
extern int test_yonder_mate_decapt_hop(phase_t *php);
extern int test_yonder_mate_pao_jump(phase_t *php);
extern int test_yonder_mate_jump(phase_t *php);
extern int test_yonder_mate_jump_hop(phase_t *php);
extern int test_yonder_mate_refl(phase_t *php);
extern int test_yonder_mate_equi_hop(phase_t *php);
extern int test_mate_null(phase_t *php);
extern int test_hither_mate_sub_n(phase_t *php);
extern int test_hither_mate_sub_c(phase_t *php);
extern int test_hither_mate_sub_f(phase_t *php);
extern int test_hither_mate_sub_mf(phase_t *php);
extern int test_yonder_mate_sub_n(phase_t *php);
extern int test_yonder_mate_sub_c(phase_t *php);
extern int test_yonder_mate_sub_f(phase_t *php);
extern int test_yonder_mate_sub_mf(phase_t *php);

// analyze.c
extern int analyze_phase_normal(phase_t *php);
extern int analyze_phase_change(phase_t *php);
extern int analyze_phase_madrasi(phase_t *php);
extern int analyze_phase_change_madrasi(phase_t *php);
extern int analyze_phase_isardam(phase_t *php);
extern int analyze_phase_change_isardam(phase_t *php);
extern void analyze_hi_leap(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_step(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_mao_step(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_run(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_run_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_decapt_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_jump(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_jump_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_refl(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_equi_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_hi_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_leap(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_step(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_mao_step(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_run(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_run_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_decapt_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_jump(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_jump_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_refl(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_equi_hop(phase_t *php, int pos, uchar_t fpc);
extern void analyze_yo_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern void analyze_null(phase_t *php, int pos, uchar_t fpc);
extern int analyze_hi_leap_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_mao_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_run_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_decapt_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_jump_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_jump_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_refl_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_hi_equi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_leap_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_mao_step_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_run_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_decapt_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_jump_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_jump_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_refl_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_yo_equi_hop_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_null_madrasi(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
extern int analyze_leaf_phase_normal(phase_t *php);
extern int analyze_leaf_phase_change(phase_t *php);
extern int analyze_leaf_phase_madrasi(phase_t *php);
extern int analyze_leaf_phase_change_madrasi(phase_t *php);
extern int analyze_leaf_phase_isardam(phase_t *php);
extern int analyze_leaf_phase_change_isardam(phase_t *php);
extern int analyze_leaf_hi_leap(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_step(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_mao_step(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_run(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_run_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_decapt_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_jump(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_jump_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_refl(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_equi_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_hi_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_leap(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_leap_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_step(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_mao_step(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_mao_step_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_run(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_run_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_decapt_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_jump(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_jump_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_jump_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_jump_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_refl(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_refl_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_equi_hop(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_yo_equi_hop_phase_constraint(phase_t *php, int pos, uchar_t fpc);
extern int analyze_leaf_null(phase_t *php, int pos, uchar_t fpc);
extern void setup_fpc_board_normal(phase_t *php);
extern void setup_fpc_board_annan(phase_t *php);
extern void setup_fpc_board_anhoku(phase_t *php);
extern void setup_fpc_board_taimen(phase_t *php);
extern void setup_fpc_board_haimen(phase_t *php);
extern void setup_fpc_board_nekosen(phase_t *php);
extern void setup_fpc_board_neko2(phase_t *php);
extern int hi_trial_move_isardam(phase_t *php, uchar_t pc, int from, int to);
extern int yo_trial_move_isardam(phase_t *php, uchar_t pc, int from, int to);
extern int hi_trial_move_normal(phase_t *php, uchar_t pc, int from, int to);
extern int yo_trial_move_normal(phase_t *php, uchar_t pc, int from, int to);
extern int hi_trial_move2_normal(phase_t *php, uchar_t pc, int from, int to_1st, int to);
extern int yo_trial_move2_normal(phase_t *php, uchar_t pc, int from, int to_1st, int to);

// option.c
extern void option_L(char *optarg);
extern void option_E(char *optarg);
extern void option_M(char *optarg);
extern void option_X(char *optarg);

// arrange.c
extern int arrange(FILE *Fp);

// restore.c
extern int restore(void);

// sub.c
extern char *get_title(char *ib);
extern char *get_rule(char *ib);
extern void toasciiuc(char *ib);
extern char *skip_space(char *ib);
extern char *skip_nonspace(char *ib);
extern void error_message(const char *em, const char *ib);
extern int is_symmetric_normal(phase_t *php);
extern int is_symmetric_circe(phase_t *php);
extern int make_move(phase_t *php, char *ob);
extern void reg_rule(char *b);
extern char *get_move(char *ib, move_t *mvp);
extern int get_pos(char **ib);
extern void del_file(void);
extern void reg_prob(phase_t *php, phase_t *php2, char *b, int use_brief_problem_output);
extern void setup_used_pc_count(phase_t *php);
extern char *make_num_str(long unum, long lnum, char *buf);
extern void make_reborn_pwc(phase_t *php, move_t *mvp, char *ob);
extern void make_reborn_circe(phase_t *php, move_t *mvp, char *ob);
extern void make_reborn_anticirce(phase_t *php, move_t *mvp, char *ob);
extern void make_reborn_exchange(phase_t *php, move_t *mvp, char *ob);
extern int bit_count_ulong(ulong_t x);
extern int bit_count_ulonglong(ulonglong_t x);

// sub2.c
extern void signal_exit(int);
extern void mile_stone(void);
extern int help_mate_rule_check(phase_t *php);
extern int direct_mate_rule_check(phase_t *php);
extern int trace_move(phase_t *php);
extern int test_symmetric_normal(phase_t *php);
extern int test_symmetric_circe(phase_t *php);
extern mate_move_t *get_mate_move(void);
extern void free_mate_move_tree(mate_move_t *mmp);
extern void free_mate_move(mate_move_t *mmp);
extern void remove_mate_move_tree(phase_t *php);
extern void copy_move_2_mate_move(move_t *mvp, int d, mate_move_t *mmp);
extern void copy_mate_move_2_move(mate_move_t *mmp, move_t *mvp);
extern int comp_mate_move_vs_move(mate_move_t *mmp, move_t *mvp);

// diff.c
extern int is_hi_same_phase(phase_t *php);
extern int is_yo_same_phase(phase_t *php);
extern void set_hi_eh_cell(phase_t *php);
extern void set_yo_eh_cell(phase_t *php);
extern void hash_init(phase_t *php);
extern void hi_hash_diff(phase_t *php);
extern void yo_hash_diff(phase_t *php);
extern void make_hash_tab(void);
extern void hi_hash_diff_sub_normal(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_normal(phase_t *php, phase_t *php_1, int from, int to);
extern void hi_hash_diff_sub_pwc(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_pwc(phase_t *php, phase_t *php_1, int from, int to);
extern void hi_hash_diff_sub_circe(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_circe(phase_t *php, phase_t *php_1, int from, int to);
extern void hi_hash_diff_sub_anticirce(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_anticirce(phase_t *php, phase_t *php_1, int from, int to);
extern void hi_hash_diff_sub_andernach(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_andernach(phase_t *php, phase_t *php_1, int from, int to);
extern void hi_hash_diff_sub_exchange(phase_t *php, phase_t *php_1, int from, int to);
extern void yo_hash_diff_sub_exchange(phase_t *php, phase_t *php_1, int from, int to);

// move.c
extern int hi_move_normal(phase_t *ophp);
extern int yo_move_normal(phase_t *ophp);
extern int hi_move_phase_constraint(phase_t *ophp);
extern int yo_move_phase_constraint(phase_t *ophp);
extern int hi_move_restarting(phase_t *ophp);
extern int yo_move_restarting(phase_t *ophp);
extern int hi_move_leaf_normal(phase_t *ophp);
extern int yo_move_leaf_normal(phase_t *ophp);
extern int hi_move_leaf_phase_constraint(phase_t *ophp);
extern int yo_move_leaf_phase_constraint(phase_t *ophp);
extern int test_before_move_test_others(phase_t *ophp, move_t *omvp);
extern int test_before_move_test_anticirce_andernach(phase_t *ophp, move_t *omvp);
extern int test_before_move_test_antiandernach(phase_t *ophp, move_t *omvp);
extern int hi_move_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_normal(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_pwc(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_circe(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_anticirce(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_andernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_antiandernach(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int hi_move_leaf_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp);
extern int yo_move_leaf_sub_exchange(phase_t *ophp, move_t *omvp, phase_t *nphp);

// print.c
extern void print_phase(phase_t *php, FILE *Fp);
extern void print_mate(phase_t *php, FILE *Fp);
extern void print_last_moves(phase_t *php, int stop, FILE *Fp);
extern void print_record(FILE *Fp);
extern void print_rule(FILE *Fp);
extern void print_record4(void);
extern int print_search_options(FILE *Fp, int use_brief_problem_output);
extern void print_start(FILE *Fp, int restart);
extern void print_option_L(FILE *Fp);
extern void print_option_M(FILE *Fp);
extern void print_option_B(FILE *Fp);
extern void print_option_X(FILE *Fp);
extern void print_direct_mate(void);
extern int walk_mate_move_tree(mate_move_t *mmp, mate_move_t *mmq);

// hist.c
extern char release[];

// data.c
// 環境変数
extern ulonglong_t cond_flag;
extern ulong_t seed;
extern ulong_t strategy;
extern ulong_t hint;
extern ulong_t rule_flag;
extern ushort_t limit_depth;
extern ushort_t max_limit_depth;
extern ushort_t min_limit_depth;
extern char title[TITLESIZE];
extern char in_file[FNAMEMAX + 1];
extern char out_file[FNAMEMAX + 1];
extern char restart_file[FNAMEMAX + 1];
// オプション環境変数
extern char *arg_x;
extern long solution_limit;
#if defined(_M_X64)
long long hi_eh_lim;
long long yo_eh_lim;
long long yo_eh2_lim;
#else
extern long hi_eh_lim;
extern long yo_eh_lim;
extern long yo_eh2_lim;
#endif
extern long no_hi_eh_head;
extern long no_yo_eh_head;
extern int free_sph_count;
extern ushort_t trace_limit;
extern char mate_position;
extern char mate_counter_position;
extern uchar_t init_phase_state;
extern uchar_t mate_print;
extern uchar_t brief_output;
extern uchar_t gentle;
extern uchar_t hi_eh_given;
extern uchar_t yo_eh_given;
extern uchar_t yo_eh2_given;
extern uchar_t dancing;
extern uchar_t loose;
extern uchar_t verbose;
extern uchar_t debug;
// 作業変数
extern FILE *inFp;
extern FILE *fileFp;
extern char *mate_buf;
extern phase_t *phase;
extern move_t *last_move;
extern time_t total_elapse;
extern time_t start_time;
extern time_t restart_time;
extern time_t last_elapse;
extern time_t base_elapse;
extern long long call_move_count;
extern long long call_move_leaf_count;
extern long long call_analyze_count;
extern long long call_analyze_leaf_count;
extern long solution_count;
extern long phase_count;
extern long phase_1M;
extern long save_count;
extern long save_1M;
extern long check_point;
extern long check_interval;
extern ushort_t depth;
extern ushort_t xtra_depth;
extern phase_t init_phase;
extern uchar_t all_used_pc_count[RAW_PC_END];
extern uchar_t hi_used_pc_count[RAW_PC_END];
extern uchar_t yo_used_pc_count[RAW_PC_END];
extern char inbuf[PROBSIZE];
// オプション作業変数
extern FILE *restartFp;
extern seq_phase_hash_t *hi_sph_head[NOSPHHEAD];
extern seq_phase_hash_t *yo_sph_head[NOSPHHEAD];
extern seq_phase_hash_t *sph_cell;
extern escape_hash_t **hi_eh_head;
extern escape_hash_t **yo_eh_head;
extern escape_hash_t *hi_eh_free;
extern escape_hash_t *yo_eh_free;
extern escape_hash_t *hi_eh_cell;
extern escape_hash_t *yo_eh_cell;
extern mate_move_t *mm_free;
extern mate_move_t *mm_root;
extern uchar_t restarting;
extern uchar_t print_2nd_phase;
// 固定データ
extern phase_t null_phase;
extern char fpc_pos_back[81];
extern char fpc_pos_forward[81];
extern char *fpc_pos_hi_annan;
extern char *fpc_pos_yo_annan;
extern char *fpc_pos_hi_anhoku;
extern char *fpc_pos_yo_anhoku;
extern char *fpc_pos_hi_taimen;
extern char *fpc_pos_yo_taimen;
extern char *fpc_pos_hi_haimen;
extern char *fpc_pos_yo_haimen;
extern char *yo_hi_ctrl_taimen;
extern char *hi_yo_ctrl_taimen;
extern char *yo_hi_ctrl_haimen;
extern char *hi_yo_ctrl_haimen;
extern move_t null_move;
extern quote_mark_t quote_mark[];
extern char *kanji_num[];
extern char *zen_num[];

#undef	RAW_PC_END

#endif // !FM_H
