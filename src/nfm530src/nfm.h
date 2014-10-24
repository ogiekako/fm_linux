// Fairy Mate With Dr.NOSHITA's idea
// nfm/nfm/nfm.h -- main header file
// Copyright(c) KAMINA Jiro 2006-2010.  All rights reserved.
// $Header$

#if !defined(NFM_HM_)
#define NFM_HM_

#include	<stdio.h>
#include	<time.h>

// ユーザ定義型
typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned long ulong_t;
typedef struct board_ board_t;			// board pointer
typedef struct mseq_ mseq_t;			// move sequence pointer
typedef struct mseqhead_ mseqhead_t;	// move sequence header
#if _MSC_VER >= 1500
typedef unsigned long long ulonglong_t;
#else
typedef unsigned long ulonglong_t;
#endif

// 定数
#define	ON				(-1)
#define	OFF				(0)
#define	SIGQUIT			(3)
#define	SIG2BIG			(99)
#define	PATHMAX			(256)
#define	EWASIZE			(128)			// editing work area size
#define	BUFSIZE			(1024)
#define	BOARD_SIZE		(10 * 11 + 1)
#define	HAND_SIZE		(8)
#define	DEFAULT_HASHHEAD_COUNT	(0x80000)
#define	DEFAULT_MSEQ_INC_COUNT	((1024 * 1024 * 32) / sizeof(mseq_t))
#define	DEFAULT_BOARD_INC_COUNT	((1024 * 1024 * 32) / sizeof(board_t))

// 詰/逃れ/非逃れ値
#define	RC_MATE			(0)
#define	RC_ESCAPE		(-1)
#define	RC_NOTESCAPE	(1)

// 合法/非合法値
#define	RC_LEGAL		(0)
#define	RC_ILLEGAL		(-1)

// メッセージ出力種別コード
#define	B_PM_DISPLAY	(0x1)
#define	B_PM_FILE		(0x2)
#define	B_PM_BOTH		(B_PM_DISPLAY | B_PM_FILE)

// ルールフラグビット
#define	RULE_MATE			(0x8000)
#define	RULE_SELFMATE		(0x4000)
#define	RULE_SEREASEMATE	(0x2000)
#define	RULE_TARGETFIG		(0x1000)
#define	RULE_HELP			(0x0800)
#define	RULE_NFMHM			(RULE_HELP | RULE_MATE)
#define	RULE_NFMHMA			(RULE_HELP | RULE_MATE | RULE_TARGETFIG)
#define	RULE_NFMHSM			(RULE_HELP | RULE_SELFMATE)
#define	RULE_NFMHSMA		(RULE_HELP | RULE_SELFMATE | RULE_TARGETFIG)
#define	RULE_NFMSHM			(RULE_HELP | RULE_SEREASEMATE)
#define	RULE_NFMSERM		(RULE_SEREASEMATE)
#define	RULE_NFMHA			(RULE_HELP | RULE_TARGETFIG)

// 条件フラグビット
#define	COND_PWC		(0x8000)
#define	COND_NO_PROM	(0x0008)

// 手順移動方向フラグビット
#define	DIR_RIGHT		(0x01)
#define	DIR_LEFT		(0x02)
#define	DIR_STRAIGHT	(0x04)
#define	DIR_GO			(0x08)
#define	DIR_UP			(0x10)
#define	DIR_SIDE		(0x20)
#define	DIR_DOWN		(0x40)
#define	DIR_JUMP		(0x80)

// 構造体

// 駒成ビット
#define	B_PC_PROMOTE	(0x08)	// 成面

// 駒コード
#define	PC_FU		(0x01)
#define	PC_KY		(0x02)
#define	PC_KE		(0x03)
#define	PC_GI		(0x04)
#define	PC_KI		(0x05)
#define	PC_KA		(0x06)
#define	PC_HI		(0x07)
#define	PC_OU		(0x0d)		// == (PC_KI | B_PC_PROMOTE)
#define	PC_TO		(PC_FU | B_PC_PROMOTE)
#define	PC_NY		(PC_KY | B_PC_PROMOTE)
#define	PC_NK		(PC_KE | B_PC_PROMOTE)
#define	PC_NG		(PC_GI | B_PC_PROMOTE)
#define	PC_UM		(PC_KA | B_PC_PROMOTE)
#define	PC_RY		(PC_HI | B_PC_PROMOTE)
#define	PC_END		(16)
#define	SQ_ROOM		(0x10)
#define	SQ_ROCK		(0x11)		// 石（枡を占有し、不透過）
#define	SQ_HOLE		(0x12)		// 穴（枡を占有し、透過）
#define	SQ_END		(19)
#define	SQ_OofB		(0x00)

// 駒攻受ビット
#define	B_PC_HITHER		(0x80)
#define	B_PC_YONDER		(0x40)
#define	B_SQ_CONTROL	(0x10)

// 攻方駒コード
#define	HI_FU		(PC_FU | B_PC_HITHER)
#define	HI_KY		(PC_KY | B_PC_HITHER)
#define	HI_KE		(PC_KE | B_PC_HITHER)
#define	HI_GI		(PC_GI | B_PC_HITHER)
#define	HI_KI		(PC_KI | B_PC_HITHER)
#define	HI_KA		(PC_KA | B_PC_HITHER)
#define	HI_HI		(PC_HI | B_PC_HITHER)
#define	HI_OU		(PC_OU | B_PC_HITHER)
#define	HI_TO		(HI_FU | B_PC_PROMOTE)
#define	HI_NY		(HI_KY | B_PC_PROMOTE)
#define	HI_NK		(HI_KE | B_PC_PROMOTE)
#define	HI_NG		(HI_GI | B_PC_PROMOTE)
#define	HI_UM		(HI_KA | B_PC_PROMOTE)
#define	HI_RY		(HI_HI | B_PC_PROMOTE)

// 受方駒コード
#define	YO_FU		(PC_FU | B_PC_YONDER)
#define	YO_KY		(PC_KY | B_PC_YONDER)
#define	YO_KE		(PC_KE | B_PC_YONDER)
#define	YO_GI		(PC_GI | B_PC_YONDER)
#define	YO_KI		(PC_KI | B_PC_YONDER)
#define	YO_KA		(PC_KA | B_PC_YONDER)
#define	YO_HI		(PC_HI | B_PC_YONDER)
#define	YO_GY		(PC_OU | B_PC_YONDER)
#define	YO_TO		(YO_FU | B_PC_PROMOTE)
#define	YO_NY		(YO_KY | B_PC_PROMOTE)
#define	YO_NK		(YO_KE | B_PC_PROMOTE)
#define	YO_NG		(YO_GI | B_PC_PROMOTE)
#define	YO_UM		(YO_KA | B_PC_PROMOTE)
#define	YO_RY		(YO_HI | B_PC_PROMOTE)

// 盤面マスクパターン
#define	M_SQ_PIECE		(0x0f)
#define	M_SQ_RAW_PC		(0x07)
#define	M_SQ_ONBD		(0x1f)
#define	M_SQ_OofB		(0x00)

// 盤面セル構造体
struct board_ {
	board_t *next;				// 次盤面セルへのポインタ
	uchar_t board[BOARD_SIZE];	// 盤面
	uchar_t hi_hand[HAND_SIZE];	// 攻方持駒
	uchar_t yo_hand[HAND_SIZE];	// 受方持駒
	uchar_t hi_ou_pos;			// 攻方王位置
	uchar_t yo_gy_pos;			// 受方玉位置
	uchar_t fire[BOARD_SIZE];	// 火線マップ
	uchar_t hi_ck_pos;			// 攻方王手位置（両王手のときは、0xff)
	uchar_t yo_ck_pos;			// 受方王手位置（両王手のときは、0xff)
};

#define	B_BD_HI_STEPFIRE	(0x80)
#define	B_BD_HI_RUNFIRE		(0x40)
#define	B_BD_HI_OVERFIRE	(0x20)
#define	B_BD_HI_OUFIRE	 	(0x10)
#define	B_BD_HI_FUEXIST	 	(0x10)
#define	B_BD_HI_FIRE		(B_BD_HI_STEPFIRE | B_BD_HI_RUNFIRE | B_BD_HI_OVERFIRE)
#define	B_BD_YO_STEPFIRE	(0x08)
#define	B_BD_YO_RUNFIRE		(0x04)
#define	B_BD_YO_OVERFIRE	(0x02)
#define	B_BD_YO_GYFIRE	 	(0x01)
#define	B_BD_YO_FUEXIST	 	(0x01)
#define	B_BD_YO_FIRE		(B_BD_YO_STEPFIRE | B_BD_YO_RUNFIRE | B_BD_YO_OVERFIRE)
//#define	B_BD_YO_FIRE		(B_BD_YO_STEPFIRE | B_BD_YO_RUNFIRE)

// 手順セル構造体
struct mseq_ {
	mseq_t *parp;			// 前深度の手順セルへのポインタ
	mseq_t *brop;			// 同一深度の手順セルへのポインタ
	mseq_t *amip;			// ハッシュキューまたは詰手順ポインタ
	union {
		board_t *bdp_;		// 盤面セルへのポインタ
		mseq_t *sphp_;		// 同一局面の手順セルポインタ
	} u1_;
	long depth;				// 深度
	ulong_t hi_hash;		// 盤面攻方駒のハッシュ値
	ulong_t yo_hash;		// 盤面受方駒のハッシュ値
	ulong_t hi_hand;		// 攻方持駒ビットパターン
	ushort_t ps_flag;		// 局面状態フラグ
	uchar_t ref_count;		// 参照数
	uchar_t pc;				// 手順: 駒
	uchar_t from;			// 手順: 移動元
	uchar_t to;				// 手順: 移動先
	uchar_t capt;			// 手順: 捕獲駒
	uchar_t mv_flag;		// 手順: フラグ
};

#define	BDP 		u1_.bdp_
#define	SPHP 		u1_.sphp_

// 持駒種別マスク
#define	M_HH_FU 		(0x000000ff)	// 持駒マスク: 歩
#define	M_HH_KY 		(0x00000f00)	// 持駒マスク: 香
#define	M_HH_KE 		(0x0000f000)	// 持駒マスク: 桂
#define	M_HH_GI 		(0x000f0000)	// 持駒マスク: 銀
#define	M_HH_KI 		(0x00f00000)	// 持駒マスク: 金
#define	M_HH_KA 		(0x0f000000)	// 持駒マスク: 角
#define	M_HH_HI 		(0xf0000000)	// 持駒マスク: 飛

// 局面状態フラグビット
#define	B_PS_HI_CHECK		(0x8000)	// 攻方王手
#define	B_PS_HI_STEPCHECK	(0x4000)	// 攻方近接王手
#define	B_PS_HI_RUNCHECK	(0x2000)	// 攻方遠隔王手
#define	B_PS_HI_DBLCHECK	(0x1000)	// 攻方両王手
#define	B_PS_YO_CHECK		(0x0800)	// 受方王手
#define	B_PS_YO_STEPCHECK	(0x0400)	// 受方近接王手
#define	B_PS_YO_RUNCHECK	(0x0200)	// 受方遠隔王手
#define	B_PS_YO_DBLCHECK	(0x0100)	// 受方両王手
#define	B_PS_MATE			(0x0080)	// 詰
#define	B_PS_ESCAPE			(0x0040)	// 逃れ
#define	B_PS_UFMATE			(0x0020)	// 打歩詰
#define	B_PS_UFCHECK		(0x0010)	// 打歩王手
#define	B_PS_SAMEPHASE		(0x0008)	// 同一局面
#define	B_PS_SAMEBOARD		(0x0004)	// 同一盤面
#define	B_PS_GIVENMATEPATH	(0x0002)	// 指定詰手順

// 手順フラグビット
#define	B_MV_PROMOTE		(0x80)		// 成
#define	B_MV_CAPTIT			(0x40)		// 同
#define	B_MV_REBORN			(0x20)		// 再生
#define	B_MV_MATEPATH		(0x08)		// 詰手順
#define	B_MV_MATEBRANCH		(0x04)		// 詰手順分岐点
#define	B_MV_MATEJOIN		(0x02)		// 詰手順合流点
#define	B_MV_GIVEN			(0x01)		// 指定手順

// 手順セルヘッダ構造体
struct mseqhead_ {
	mseq_t *brop;				// 同一深度の手順セルへのポインタ
	long all_move_count;		// 全生成手数（盤面データ構造生成後）
	long legal_move_count;		// 合法手数
	long new_phase_count;		// 新規手数
	long unique_phase_count;	// 唯一手数
	ushort_t flag;				// フラグ
};

// 手順セルヘッダフラグビット
#define	B_MH_MATE			(0x8000)	// 詰
#define	B_MH_ESCAPE			(0x4000)	// 逃れ
#define	B_MH_NOCELL			(0x2000)	// 手順セル解放済

// 変数
// hist.c
extern char cmdname[];
extern char release[];

// data.c
extern FILE *inFp;
extern FILE *fileFp;
extern mseqhead_t *mseq_head;
extern mseq_t **hashhead_check;
extern mseq_t **hashhead_escape;
extern mseq_t *msp_free;
extern mseq_t *msp_start_last;
extern mseq_t *msp_givenmate;
extern board_t *bdp_free;
extern void (* fm_attach_mseq)(mseq_t *);
extern double comp_hash_check_found;
extern double comp_hash_check_notfound;
extern double comp_hash_escape_found;
extern double comp_hash_escape_notfound;
extern time_t start_time;
extern time_t total_elapse;
extern size_t mseq_inc_memsize;
extern size_t mseq_max_memsize;
extern size_t board_inc_memsize;
extern size_t board_max_memsize;
extern size_t free_memsize;
extern size_t real_memsize;
extern size_t virtual_memsize;
extern long phase_count;
extern long check_point;
extern long check_interval;
extern long mseq_count;
extern long mseq_inc_count;
extern long mseq_max_count;
extern long mseqpool_count;
extern long mseq_reuse_count;
extern long hashhead_count;
extern long hashhead_count_1;
extern long board_count;
extern long board_inc_count;
extern long board_max_count;
extern long boardpool_count;
extern long depth_limit;
extern long depth_start;
extern long test_hash_check_count;
extern long found_hash_check_count;
extern long notfound_hash_check_count;
extern long test_hash_escape_count;
extern long found_hash_escape_count;
extern long notfound_hash_escape_count;
extern int sc_print_chesstyle;
extern int sc_print_debug;
extern int sc_print_endfig;
extern int sc_print_hashinfo;
extern int sc_print_interrupted;
extern int sc_print_loosemate;
extern int sc_print_matefig;
extern int sc_print_notdisplay;
extern int sc_print_tradstyle;
extern int sc_print_startfig;
extern int sc_print_verbose;
extern int sc_env_autostart;
extern int sc_tact_freemseq;
extern int sc_tact_givenmatepath;
extern int sc_tact_sameaslesspc;
extern int sc_tact_sameasmorepc;
extern int sc_tact_sameaslessfu;
extern int sc_tact_sameasmorefu;
extern int sc_tact_freesamemseq;
extern int sc_tact_usextrapc;
extern int sc_tact_reusemseq;
extern int save_sc_tact_freemseq;
extern int save_sc_tact_sameaslesspc;
extern int save_sc_tact_sameasmorepc;
extern int save_sc_tact_sameaslessfu;
extern int save_sc_tact_sameasmorefu;
extern int mate_count;
extern int flag_loose_mate;
extern int flag_docheck;
extern int mask_targetdepth;
extern int depth_rsv;
extern int str_mate_depth;
extern int one_line_title_limit;
extern int retry;
extern ushort_t rule;
extern ushort_t cond;
extern char in_file[PATHMAX];
extern char out_file[PATHMAX];
extern char title[256];
extern char inbuf[BUFSIZE];
extern char outbuf[BUFSIZE];
extern char editbuf[EWASIZE];
extern mseq_t mseq_prob;
extern board_t board_prob;
extern mseq_t mseq_target;
extern board_t board_target;
extern const uchar_t board_void[BOARD_SIZE];
extern const uchar_t hand_void[HAND_SIZE];
extern const uchar_t yo_hand_ini[HAND_SIZE];
extern const mseq_t mseq_void;
extern const mseqhead_t mseqhead_void;
extern const int *vec_hi_step[PC_END];
extern const int *vec_hi_run[PC_END];
extern const int *vec_yo_step[PC_END];
extern const int *vec_yo_run[PC_END];
extern const int hi_step_vec[11];
extern const int yo_step_vec[11];
extern const int run_vec[9];
extern const int *hi_step_pc[10];
extern const int *yo_step_pc[10];
extern const int *hi_run_pc[8];
extern const int *yo_run_pc[8];
extern const int *hi_runv_pc[23];
extern const int *yo_runv_pc[23];
extern const ulong_t *bphh[];
extern const ulong_t bmhh[];

#endif // !NFM_HM_

