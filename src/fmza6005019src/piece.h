// 6aco/peice.c -- FM piece data
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/piece.h 1.34 2014/08/17 02:13:44 budai10 Exp budai10 $

#if !defined(PIECE_H)
#define PIECE_H

#include	"fm.h"

// 駒成ビット
#define	B_PC_PROMOTED		(0x10)	// 成面

// 駒コード
#define	NUL					(0x00)						// 非駒
#define	ROOM				(0x00)						// 空
#define	PC_FU				(0x01)						// 歩
#define	PC_KY				(0x02)						// 香
#define	PC_KE				(0x03)						// 桂
#define	PC_GI				(0x04)						// 銀
#define	PC_KI				(0x05)						// 金
#define	PC_KA				(0x06)						// 角
#define	PC_HI				(0x07)						// 飛
#define	PC_F1				(0x08)						// フェアリー駒１
#define	PC_F2				(0x09)						// フェアリー駒２
#define	PC_F3				(0x0a)						// フェアリー駒３
#define	PC_F4				(0x0b)						// フェアリー駒４
#define	PC_F5				(0x0c)						// フェアリー駒５
#define	PC_F6				(0x0d)						// フェアリー駒６
#define	PC_F7				(0x0e)						// フェアリー駒７
#define	PC_OU				(0x0f)						// 王/玉
#define	PIT					(ROOM | B_PC_PROMOTED)		// 穽（枡を占有し、透過）
#define	PC_TO				(PC_FU | B_PC_PROMOTED)		// と
#define	PC_NY				(PC_KY | B_PC_PROMOTED)		// 杏
#define	PC_NK				(PC_KE | B_PC_PROMOTED)		// 圭
#define	PC_NG				(PC_GI | B_PC_PROMOTED)		// 全
#define	ROCK				(PC_KI | B_PC_PROMOTED)		// 石（枡を占有し、不透過）
#define	PC_UM				(PC_KA | B_PC_PROMOTED)		// 馬
#define	PC_RY				(PC_HI | B_PC_PROMOTED)		// 龍
#define	PC_P1				(PC_F1 | B_PC_PROMOTED)		// フェアリー成駒１
#define	PC_P2				(PC_F2 | B_PC_PROMOTED)		// フェアリー成駒２
#define	PC_P3				(PC_F3 | B_PC_PROMOTED)		// フェアリー成駒３
#define	PC_P4				(PC_F4 | B_PC_PROMOTED)		// フェアリー成駒４
#define	PC_P5				(PC_F5 | B_PC_PROMOTED)		// フェアリー成駒５
#define	PC_P6				(PC_F6 | B_PC_PROMOTED)		// フェアリー成駒６
#define	PC_P7				(PC_F7 | B_PC_PROMOTED)		// フェアリー成駒７
#define	RAW_PC_END			(16)
#define	PC_END				(32)
#define	PC_F0				(PC_F1 - 1)
#define	PC_P0				(PC_P1 - 1)

// 駒攻受ビット
#define	B_PC_HITHER			(0x40)		// ■この値は、駒データと連携しているので
#define	B_PC_YONDER			(0x20)		//   変更時には同時に駒データも対応が必要
#define	BM_PC_TURN			(0x60)

// 攻方駒コード
#define	HI_FU				(PC_FU | B_PC_HITHER)
#define	HI_KY				(PC_KY | B_PC_HITHER)
#define	HI_KE				(PC_KE | B_PC_HITHER)
#define	HI_GI				(PC_GI | B_PC_HITHER)
#define	HI_KI				(PC_KI | B_PC_HITHER)
#define	HI_KA				(PC_KA | B_PC_HITHER)
#define	HI_HI				(PC_HI | B_PC_HITHER)
#define	HI_F1				(PC_F1 | B_PC_HITHER)
#define	HI_F2				(PC_F2 | B_PC_HITHER)
#define	HI_F3				(PC_F3 | B_PC_HITHER)
#define	HI_F4				(PC_F4 | B_PC_HITHER)
#define	HI_F5				(PC_F5 | B_PC_HITHER)
#define	HI_F6				(PC_F6 | B_PC_HITHER)
#define	HI_OU				(PC_OU | B_PC_HITHER)
#define	HI_TO				(PC_TO | B_PC_HITHER)
#define	HI_NY				(PC_NY | B_PC_HITHER)
#define	HI_NK				(PC_NK | B_PC_HITHER)
#define	HI_NG				(PC_NG | B_PC_HITHER)
#define	HI_UM				(PC_UM | B_PC_HITHER)
#define	HI_RY				(PC_RY | B_PC_HITHER)
#define	HI_P1				(PC_P1 | B_PC_HITHER)
#define	HI_P2				(PC_P2 | B_PC_HITHER)
#define	HI_P3				(PC_P3 | B_PC_HITHER)
#define	HI_P4				(PC_P4 | B_PC_HITHER)
#define	HI_P5				(PC_P5 | B_PC_HITHER)
#define	HI_P6				(PC_P6 | B_PC_HITHER)
#define	HI_P7				(PC_P7 | B_PC_HITHER)
#define	HI_F0				(HI_F1 - 1)
#define	HI_P0				(HI_P1 - 1)

// 受方駒コード
#define	YO_FU				(PC_FU | B_PC_YONDER)
#define	YO_KY				(PC_KY | B_PC_YONDER)
#define	YO_KE				(PC_KE | B_PC_YONDER)
#define	YO_GI				(PC_GI | B_PC_YONDER)
#define	YO_KI				(PC_KI | B_PC_YONDER)
#define	YO_KA				(PC_KA | B_PC_YONDER)
#define	YO_HI				(PC_HI | B_PC_YONDER)
#define	YO_F1				(PC_F1 | B_PC_YONDER)
#define	YO_F2				(PC_F2 | B_PC_YONDER)
#define	YO_F3				(PC_F3 | B_PC_YONDER)
#define	YO_F4				(PC_F4 | B_PC_YONDER)
#define	YO_F5				(PC_F5 | B_PC_YONDER)
#define	YO_F6				(PC_F6 | B_PC_YONDER)
#define	YO_GY				(PC_OU | B_PC_YONDER)
#define	YO_TO				(PC_TO | B_PC_YONDER)
#define	YO_NY				(PC_NY | B_PC_YONDER)
#define	YO_NK				(PC_NK | B_PC_YONDER)
#define	YO_NG				(PC_NG | B_PC_YONDER)
#define	YO_UM				(PC_UM | B_PC_YONDER)
#define	YO_RY				(PC_RY | B_PC_YONDER)
#define	YO_P1				(PC_P1 | B_PC_YONDER)
#define	YO_P2				(PC_P2 | B_PC_YONDER)
#define	YO_P3				(PC_P3 | B_PC_YONDER)
#define	YO_P4				(PC_P4 | B_PC_YONDER)
#define	YO_P5				(PC_P5 | B_PC_YONDER)
#define	YO_P6				(PC_P6 | B_PC_YONDER)
#define	YO_P7				(PC_P7 | B_PC_YONDER)
#define	YO_F0				(YO_F1 - 1)
#define	YO_P0				(YO_P1 - 1)

// 特殊駒コード
#define	SQ_ROOM				(ROOM)
#define	SQ_ROCK				(ROCK)
#define	SQ_PIT				(PIT)

// 通常駒番号
#define	NP_FU				(1)		// 歩
#define	NP_KY				(2)		// 香
#define	NP_KE				(3)		// 桂
#define	NP_GI				(4)		// 銀
#define	NP_KI				(5)		// 金
#define	NP_KA				(6)		// 角
#define	NP_HI				(7)		// 飛
#define	NP_OU				(8)		// 王/玉
#define	NP_TO				(9)		// と
#define	NP_NY				(10)	// 杏
#define	NP_NK				(11)	// 圭
#define	NP_NG				(12)	// 全
#define	NP_UM				(13)	// 馬
#define	NP_RY				(14)	// 龍
#define	SP_ROOM				(15)	// ・/空
#define	SP_ROCK				(16)	// ●/石
#define	SP_PIT				(17)	// ◆/穽
// 通常駒番号終了印
#define	NP_END				(18)
// フェアリー駒番号
#define	FP_START			(20)
// フェアリーチェス - リーパー族
#define	FP_WA				(29)	// (0, 1)-Leaper Wazir
#define	FP_DA				(30)	// (0, 2)-Leaper Dabbaba
#define	FP_TL				(31)	// (0, 3)-Leaper Threeleaper
#define	FP_FE				(32)	// (1, 1)-Leaper Fers
#define	FP_KN				(21)	// (1, 2)-Leaper Knight
#define	FP_CM				(22)	// (1, 3)-Leaper Camel
#define	FP_GR				(23)	// (1, 4)-Leaper Giraffe
#define	FP_FL				(25)	// (1, 6)-Leaper Flamingo 
#define	FP_AL				(33)	// (2, 2)-Leaper Alfil
#define	FP_ZB				(27)	// (2, 3)-Leaper Zebra
#define	FP_TR				(34)	// (3, 3)-Leaper Tripper
#define	FP_L5				(24)	// (0, 5)+(3, 4)-Leaper Five-leaper
#define	FP_R4				(28)	// (2, 6)-Leaper Root-40-leaper
#define	FP_R5				(26)	// (1, 7)+(5, 5)-Leaper Root-50-leaper
#define	FP_R6				(27)	// (1, 8)+(4, 7)-Leaper Root-65-leaper
#define	FP_ZE				(20)	// (0, 0)-Leaper Zero
// フェアリーチェス - ライダー族
#define	FP_QU				(41)	// (0, 1)+(1, 1)-Rider Queen
#define	FP_NR				(42)	// (1, 2)-Rider Nightrider
#define	FP_CR				(43)	// (1, 3)-Rider Camelrider
#define	FP_ZR				(44)	// (2, 3)-Rider Zebrarider
#define	FP_RS				(48)	// (1, 2)-CircleRider Rose
// フェアリーチェス - 複合（リーパー、ライダー）族
#define	FP_EM				(50)	// (0, 1)-Rider+(1, 2)-Leaper Empress
#define	FP_PR				(49)	// (1, 1)-Rider+(1, 2)-Leaper Princess
// フェアリーチェス - ホッパー族
#define	FP_GH				(51)	// Grasshopper
#define	FP_LO				(52)	// Locust
#define	FP_LI				(53)	// Lion
#define	FP_RO				(54)	// Kangaroo
#define	FP_NE				(55)	// Non-stop equihopper
#define	FP_EH				(56)	// Equihopper
#define	FP_SP				(57)	// Sparrow
#define	FP_EA				(58)	// Eagle
// フェアリーチェス - 象棋族
#define	FP_PA				(61)	// PaO
#define	FP_VA				(62)	// Vao
#define	FP_LE				(63)	// Leo
#define	FP_SH				(64)	// Shan
#define	FP_MA				(65)	// Mao
#define	FP_MO				(66)	// Moa
// 中将棋族
#define	FP_SS				(71)	// 獅子
#define	FP_SZ				(72)	// 酔象
#define	FP_KR				(73)	// 麒麟
#define	FP_HO				(74)	// 鳳凰
#define	FP_MK				(75)	// 盲虎
#define	FP_DO				(76)	// 銅将
#define	FP_MH				(77)	// 猛豹
#define	FP_CN				(78)	// 仲人
#define	FP_HN				(79)	// 奔王
#define	FP_JG				(80)	// 竪行
#define	FP_OG				(81)	// 横行
#define	FP_HE				(82)	// 反車
#define	FP_TS				(83)	// 太子 <- 酔象
#define	FP_HR				(84)	// 飛鹿 <- 盲虎
#define	FP_HJ				(85)	// 飛鷲 <- 龍王
#define	FP_TK				(86)	// 角鷹 <- 龍馬
#define	FP_HG				(87)	// 飛牛 <- 竪行
#define	FP_HC				(88)	// 奔猪 <- 横行
#define	FP_KG				(89)	// 鯨鯢 <- 反車
#define	FP_HK				(90)	// 白駒 <- 香車
#define	FP_CHI				(91)	// 中将棋：飛
#define	FP_CKA				(92)	// 中将棋：角
#define	FP_CKI				(93)	// 中将棋：金
#define	FP_CGI				(94)	// 中将棋：銀
#define	FP_CRY				(95)	// 中将棋：龍
#define	FP_CUM				(96)	// 中将棋：馬
#define	FP_CKY				(97)	// 中将棋：香
#define	FP_PSS				(98)	// 師（成獅子）
#define	FP_PHN				(99)	// 本（成奔王）
#define	FP_PRY				(100)	// リ（成龍王）
#define	FP_PUM				(101)	// マ（成龍馬）
#define	FP_PHI				(102)	// ヒ（成飛車）
#define	FP_PKA				(103)	// ク（成角行）
#define	FP_PJG				(104)	// 立（成竪行）
#define	FP_POG				(105)	// 黄（成横行）
#define	FP_PSZ				(106)	// 豕（成酔象）
// 禽将棋族
#define	FP_THO				(111)	// 鵬
#define	FP_TTR				(112)	// 鶴
#define	FP_TKJ				(113)	// 雉
#define	FP_TLU				(114)	// 享（左鶉）
#define	FP_TRU				(115)	// 鳥（右鶉）
#define	FP_TTK				(116)	// 鷹
#define	FP_TKT				(117)	// 鵰（くまたか）<- 鷹
#define	FP_TTB				(118)	// 燕
#define	FP_TKR				(119)	// 鴈 <- 燕
// 大将棋族
#define	FP_DHR				(121)	// 飛龍 -> 金将（成金）
#define	FP_DMG				(122)	// 猛牛 -> 金将（成金）
#define	FP_DSC				(123)	// 嗔猪 -> 金将（成金）
#define	FP_DMJ				(124)	// 猫刃 -> 金将（成金）
#define	FP_DNK				(125)	// 金将（成金） <- 飛龍、猛牛、嗔猪、猫刃
// 摩訶大大将棋族
#define	FP_DBJ				(126)	// 蟠蛇
// フェアリー駒番号終了印
#define	FP_END				(131)
// 拡張フェアリー駒番号
#define	FP_EX1				(131)	// 拡張フェアリー駒１
#define	FP_EX2				(132)	// 拡張フェアリー駒２
#define	FP_EX3				(133)	// 拡張フェアリー駒３
#define	FP_EX4				(134)	// 拡張フェアリー駒４
#define	FP_EX5				(135)	// 拡張フェアリー駒５
#define	FP_EX6				(136)	// 拡張フェアリー駒６
#define	FP_EX7				(137)	// 拡張フェアリー駒７
// 拡張フェアリー駒番号終了印
#define	FP_EXEND			(138)

// board masks
#define	BM_SQ_PC			(0x1f)
#define	BM_SQ_RAW_PC		(0x0f)

// piece attribute bits & masks
#define	B_PA_DEFINED		(0x80000000)	// 定義済
#define	B_PA_PIECE   		(0x40000000)	// 駒
#define	B_PA_ROYAL			(0x20000000)	// 王属性（Royal属性）
#define	B_PA_PROMOTABLE		(0x08000000)	// 成れる駒
#define	B_PA_PROMOTABLE_FE	(0x04000000)	// 成禁のとき、詰回避で成れる駒 (For Escape)
#define	BM_PA_PROMOTABLE	(0x0c000000)	// 詰回避で成れる駒
#define	B_PA_PLACABLE  		(0x00800000)	// 打てる駒（＝再使用できる駒。生駒に使用）
#define	B_PA_REUSABLE  		(0x00400000)	// 再使用できる駒（成駒に使用）
#define	B_PA_SAME_FACE 		(0x00010000)	// 同じ駒面あり
#define	B_PA_CONSTRAINT_SQ	(0x00008000)	// 移動先（打先）制約あり
#define	B_PA_CONSTRAINT_2F	(0x00004000)	// 二歩制約あり
#define	B_PA_CONSTRAINT_3T	(0x00002000)	// 三燕制約あり
#define	BM_PA_CONSTRAINT	(0x0000e000)	// 制約
#define	B_PA_CONSTRAINT_PM	(0x00001000)	// 打詰 (Place Mate) 制約（打歩詰制約、打燕詰制約）
#define	B_PA_PASSED			(0x00000800)	// 透過性あり（駒が通り抜けられる）

// piece move-style bits & masks
#define	B_MS_LEAP			(0x80000000)	// 跳び（合駒の利かない移動）
#define	B_MS_SHISHI_LEAP	(0x40000000)	// 獅子風の跳び（合駒の利かない移動）
#define	B_MS_LONG_LEAP		(0x10000000)	// 長跳び（跳び補足。55から跳べない利きがある場合）
#define	BM_MS_LEAP_FAMILY	(0xc0000000)	// 跳び系
#define	B_MS_STEP			(0x08000000)	// 跳々移動（合駒の利く移動。象棋の象）
#define	B_MS_MAO_STEP		(0x04000000)	// Mao風の跳々（合駒の利く移動。象棋の馬）
#define	BM_MS_STEP_FAMILY	(0x0c000000)	// 跳々系
#define	B_MS_RUN			(0x00800000)	// 走り
#define	B_MS_HOP			(0x00400000)	// Grasshopper move
#define	B_MS_DECAPT_HOP		(0x00200000)	// Locust move
#define	B_MS_PAO_JUMP		(0x00100000)	// Pao move
#define	B_MS_JUMP			(0x00080000)	// Lion move
#define	B_MS_JUMP_HOP		(0x00040000)	// Kangaroo move
#define	BM_MS_RUN_FAMILY	(0x00fc0000)	// 走り系
#define	B_MS_REFL135		(0x00020000)	// Sparrow move
#define	B_MS_REFL90			(0x00010000)	// Eagle move
#define	B_MS_EQUI_JUMP		(0x00008000)	// Equihopper move
#define	B_MS_NS_EQUI_JUMP	(0x00004000)	// Non-stop Equihopper move
#define	BM_MS_HOP_FAMILY	(0x007fc000)	// Hop-jump系
#define	B_MS_CIRCLE_RUN		(0x00002000)	// Rose move
#define	BM_MS_EXRUN_FAMILY	(0x00fc2000)	// 拡張走り系

#define	EOP					(-1)	// 位置の配列の終端記号
#define	NOP					(-1)	// 盤上でない位置（位置の初期値に使用）

// macros
#define	POS(f, r)			((f) * 9 + (r))
#define	POS_FILE(pos)		((pos) / 9)
#define	POS_RANK(pos)		((pos) % 9)
#define	POS19(f, r)			(((f) - 1) * 9 + ((r) - 1))
#define	POS19_FILE(pos)		((pos) / 9 + 1)
#define	POS19_RANK(pos)		((pos) % 9 + 1)
#define	PR_POS(pos)			((((pos) / 9) + 1) * 10 + ((pos) % 9) + 1)
#define	VEC(f, r)			((f) * 9 + (r))
#define	KIND(pc)			((pc) & BM_SQ_PC)
#define	RAW_KIND(pc)		((pc) & BM_SQ_RAW_PC)
#define	IS_ANY_PC(pc)		(RAW_KIND(pc) != 0x00)
#define IS_PROMOTABLE(pc)	(piece[KIND(pc)].attr & B_PA_PROMOTABLE)
#define IS_PROMOTABLE_FE(pc) \
							(piece[KIND(pc)].attr & BM_PA_PROMOTABLE)	// For Escape
#define	IS_HI_PC(pc)		((pc) & B_PC_HITHER)
#define	IS_YO_PC(pc)		((pc) & B_PC_YONDER)
#define	IS_ROOM(sq)			((sq) == SQ_ROOM)
#define	IS_ROCK(sq)			((sq) == SQ_ROCK)
#define	IS_PIT(sq)			((sq) == SQ_PIT)
#define	IS_NUL(pc)			((pc) == NUL)
#define	NOT_ROOM(sq)		((sq) != SQ_ROOM)
#define	NOT_PIT(sq)			((sq) != SQ_PIT)
#define	NOT_NUL(pc)			((pc) != NUL)
#define	IS_AROUND(pos, to)	(around_sq[pos][to] == ON)
#define	IS_HI_MOVABLE(pc)	(IS_ROOM(pc) || IS_YO_PC(pc))
#define	IS_YO_MOVABLE(pc)	(IS_ROOM(pc) || IS_HI_PC(pc))
#define	IS_HI_PROMOTABLE_MOVE(fr, to) \
                            (((fr) % 9) <= 2 || ((to) % 9) <= 2)
#define	IS_YO_PROMOTABLE_MOVE(fr, to) \
                            (((fr) % 9) >= 6 || ((to) % 9) >= 6)
#define	IS_PASSABLE(sq)		(IS_ROOM(sq) || IS_PIT(sq))
#define	NOT_PASSABLE(sq)	(NOT_ROOM(sq) && NOT_PIT(sq))

// Vector
typedef struct {
	int file;
	int rank;
	int range;		// 跳々、限定走り、回転走りのとき指定
} vec_t;

// Active Piece Information
typedef struct {
	char *name;
	void (* analyze_hi_leap)(phase_t *php, int pos, uchar_t fpc);
	void (* analyze_hi_run)(phase_t *php, int pos, uchar_t fpc);
	void (* analyze_yo_leap)(phase_t *php, int pos, uchar_t fpc);
	void (* analyze_yo_run)(phase_t *php, int pos, uchar_t fpc);
	int (* analyze_hi_leap_madrasi)(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
	int (* analyze_hi_run_madrasi)(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
	int (* analyze_yo_leap_madrasi)(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
	int (* analyze_yo_run_madrasi)(phase_t *php, int pos, uchar_t pc, uchar_t fpc);
	int (* analyze_leaf_hi_leap)(phase_t *php, int pos, uchar_t fpc);
	int (* analyze_leaf_hi_run)(phase_t *php, int pos, uchar_t fpc);
	int (* analyze_leaf_yo_leap)(phase_t *php, int pos, uchar_t fpc);
	int (* analyze_leaf_yo_run)(phase_t *php, int pos, uchar_t fpc);
	void (* help_check_leap)(phase_t *php);
	void (* help_check_run)(phase_t *php);
	void (* help_escape_leap)(phase_t *php);
	void (* help_escape_run)(phase_t *php);
	int (* direct_check_leap)(phase_t *php);
	int (* direct_check_run)(phase_t *php);
	int (* direct_escape_leap)(phase_t *php);
	int (* direct_escape_run)(phase_t *php);
	int (* test_hither_mate_leap)(phase_t *php);
	int (* test_hither_mate_run)(phase_t *php);
	int (* test_yonder_mate_leap)(phase_t *php);
	int (* test_yonder_mate_run)(phase_t *php);
	ulong_t attr;
	ulong_t move_style;
	int pc_no;
} piece_t;

// Base Piece Information
typedef struct {
	char *name;
	char **graphv;
	vec_t *hi_leap_vec;
	vec_t *yo_leap_vec;
	vec_t *hi_run_vec;
	vec_t *yo_run_vec;
	int max_count;
	ulong_t attr;
	ulong_t move_style;
} piece_info_t;

extern uchar_t hand_pc_order[];
extern char graph_pa1[];
extern char graph_pa2[];
extern char graph_pa3[];
extern char hi_b_graph_pa2[];
extern char yo_b_graph_pa2[];
extern char hi_b_graph_pa3[];
extern char yo_b_graph_pa3[];
extern piece_t piece[PC_END];
extern piece_info_t piece_info[FP_EXEND];
extern int fp_promoted[FP_EXEND];
extern char *graph[128];
extern char *b_graph[128];
extern char *graph_yo_gy;
extern char *hi_leap_to_pos[PC_END][81];
extern char *yo_leap_to_pos[PC_END][81];
extern char **hi_run_to_posv[PC_END][81];
extern char **yo_run_to_posv[PC_END][81];
extern char *around_to_pos[81];
extern uchar_t *hi_to_board[PC_END][81];		// 攻方移動先リスト
extern uchar_t *yo_to_board[PC_END][81];		// 攻方移動先リスト
extern uchar_t *hi_close_check_board[81];
extern uchar_t *yo_close_check_board[81];
extern uchar_t *hi_remote_check_board[81];
extern uchar_t *yo_remote_check_board[81];
extern uchar_t *around_sq[81];					// 周囲リスト
extern ulong_t fairy_move_style;
extern uchar_t *hi_valid_sq[PC_END];
extern uchar_t *yo_valid_sq[PC_END];
extern int normal_pc_count[RAW_PC_END];
extern int pc_count[RAW_PC_END];
extern uchar_t hand_pc[RAW_PC_END];
extern uchar_t pos_order[81][81];
extern uchar_t equi_hop_pc;
extern char hi_reborn_pos[RAW_PC_END][9];
extern char yo_reborn_pos[RAW_PC_END][9];

extern void setup_piece_attrs(void);
extern int option_E_FP(char *argstr, int i);
extern uchar_t get_pc_code(char **ib);
extern void correct_for_rule(void);
extern void make_FP_E_str(char *s);
extern void print_graph(void);
extern void print_board(phase_t *php);
extern void print_to_pos(int side, uchar_t pc);
extern void print_to_posv(int side, uchar_t pc);
extern void print_leap_to_board(int side, uchar_t pc);
extern void print_to_board(int side, uchar_t pc, int pos);
extern void print_to_board2(int side, uchar_t pc, int from);
extern void check_piece_attrs(void);
extern void print_board_on(int my_pos);

#endif // !PIECE_H
