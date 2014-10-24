// za/fmza.c -- zoo+single-change help(-self)-mate original data and routines
// Copyright(c) KAMINA, Jiro 2013-2014.  All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/fmza.c 1.29 2014/08/17 02:13:43 budai10 Exp budai10 $

#include	"fm.h"

char cmdname[] = "fmza";

rule_cond_t rule_cond[] = {
	{ "協力", B_R_HELP_MATE, 0, },
	{ "ばか", B_R_HELP_MATE, 0, },
	{ "H#", B_R_HELP_MATE, 0, },
	{ "協力ステイルメイト", B_R_HELP_STALEMATE, 0, },
	{ "ばかステイルメイト", B_R_HELP_STALEMATE, 0, },
	{ "H=", B_R_HELP_STALEMATE, 0, },
	{ "協力自玉", B_R_HELP_SELF_MATE, 0, },
	{ "ばか自殺", B_R_HELP_SELF_MATE, 0, },
	{ "HS#", B_R_HELP_SELF_MATE, 0, },
	{ "協力自玉ステイルメイト", B_R_HELP_SELF_STALEMATE, 0, },
	{ "ばか自殺ステイルメイト", B_R_HELP_SELF_STALEMATE, 0, },
	{ "HS=", B_R_HELP_SELF_STALEMATE, 0, },
	{ "かしこ", B_R_DIRECT_MATE, 0, },
	{ "#", B_R_DIRECT_MATE, 0, },
	{ "成禁", 0, B_C_NOPROMOTE, },
	{ "!NP", 0, B_C_NOPROMOTE, },
	{ "絶対成禁", 0, B_C_ABSOLUTE_NOPROMOTE, },
	{ "!ANP", 0, B_C_ABSOLUTE_NOPROMOTE, },
	{ "取禁", 0, B_C_NOCAPT, },
	{ "!NC", 0, B_C_NOCAPT, },
	{ "攻方取禁", 0, B_C_HITHER_NOCAPT, },
	{ "!HNC", 0, B_C_HITHER_NOCAPT, },
	{ "打歩", 0, B_C_UCHIFU, },
	{ "!F", 0, B_C_UCHIFU, },
	{ "打歩可", 0, B_C_UCHIFU_OK, },
	{ "!AF", 0, B_C_UCHIFU_OK, },
	{ "強欲", 0, B_C_GREED, },
	{ "!GR", 0, B_C_GREED, },
	{ "禁欲", 0, B_C_ASCETIC, },
	{ "!AS", 0, B_C_ASCETIC, },
	{ "安南", 0, B_C_ANNAN, },
	{ "!AN", 0, B_C_ANNAN, },
	{ "安北", 0, B_C_ANHOKU, },
	{ "!AH", 0, B_C_ANHOKU, },
	{ "対面", 0, B_C_TAIMEN, },
	{ "!TM", 0, B_C_TAIMEN, },
	{ "背面", 0, B_C_HAIMEN, },
	{ "!HM", 0, B_C_HAIMEN, },
	{ "ネコ鮮", 0, B_C_NEKOSEN, },
	{ "!NK", 0, B_C_NEKOSEN, },
	{ "ネコネコ鮮", 0, B_C_NEKO2SEN, },
	{ "!NK2", 0, B_C_NEKO2SEN, },
	{ "ANDERNACH", 0, B_C_ANDERNACH, },
	{ "!AD", 0, B_C_ANDERNACH, },
	{ "ANTIANDERNACH", 0, B_C_ANTIANDERNACH, },
	{ "!AA", 0, B_C_ANTIANDERNACH, },
	{ "PWC", 0, B_C_PWC, },
	{ "!PWC", 0, B_C_PWC, },
	{ "!CX", 0, B_C_PWC, },
	{ "キルケ", 0, B_C_CIRCE, },
	{ "CIRCE", 0, B_C_CIRCE, },
	{ "!CR", 0, B_C_CIRCE, },
	{ "アンチキルケ", 0, B_C_ANTICIRCE, },
	{ "ANTICIRCE", 0, B_C_ANTICIRCE, },
	{ "!AC", 0, B_C_ANTICIRCE, },
	{ "KOKO", 0, (B_C_KOKO | B_C_CONSTRAINT_1ST), },
	{ "KOEKO", 0, (B_C_KOKO | B_C_CONSTRAINT_1ST), },
	{ "!KO", 0, (B_C_KOKO | B_C_CONSTRAINT_1ST), },
	{ "MESSIGNY", 0, B_C_MESSIGNY, },
	{ "!MS", 0, B_C_MESSIGNY, },
	{ "MESSIGNY（欧州式）", 0, B_C_MESSIGNY_E, },
	{ "!ME", 0, B_C_MESSIGNY_E, },
	{ "MESSIGNY（日本式）", 0, B_C_MESSIGNY_J, },
	{ "!MJ", 0, B_C_MESSIGNY_J, },
	{ "マドラシ", 0, B_C_MADRASI, },
	{ "MADRASI", 0, B_C_MADRASI, },
	{ "!MD", 0, B_C_MADRASI, },
	{ "Ｋマドラシ", 0, B_C_K_MADRASI, },
	{ "Kマドラシ", 0, B_C_K_MADRASI, },
	{ "K-MADRASI", 0, B_C_K_MADRASI, },
	{ "!KM", 0, B_C_K_MADRASI, },
	{ "ISARDAM", 0, (B_C_ISARDAM | B_C_CONSTRAINT_1ST), },
	{ "!IS", 0, (B_C_ISARDAM | B_C_CONSTRAINT_1ST), },
	{ "ISARDAM（タイプB）", 0, B_C_ISARDAM, },
	{ "ISARDAM（タイプＢ）", 0, B_C_ISARDAM, },
	{ "!IB", 0, B_C_ISARDAM, },
	{ "二歩王取無効", 0, (B_C_INVALID_CAPT_OU_2F | B_C_CONSTRAINT_1ST), },
	{ "!2F", 0, (B_C_INVALID_CAPT_OU_2F | B_C_CONSTRAINT_1ST), },
	{ "位置[", 0, B_C_POSITION, },	// 「位置[fr]」を想定
	{ "![", 0, B_C_POSITION, },		// 「![fr]」を想定
	{ NULL, 0, 0, },
};

// move.c
int (* hi_move)(phase_t *ophp);
int (* yo_move)(phase_t *ophp);
int (* hi_move_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
int (* yo_move_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
int (* hi_move_leaf)(phase_t *ophp);
int (* yo_move_leaf)(phase_t *ophp);
int (* hi_move_leaf_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
int (* yo_move_leaf_sub)(phase_t *ophp, move_t *omvp, phase_t *nphp);
int (* test_before_move_test)(phase_t *ophp, move_t *omvp);
// analyze.c
int (* analyze_phase)(phase_t *php);
int (* analyze_leaf_phase)(phase_t *php);
void (* setup_fpc_board)(phase_t *php);
int (* hi_trial_move)(phase_t *php, uchar_t pc, int from, int to);
int (* yo_trial_move)(phase_t *php, uchar_t pc, int from, int to);
int (* hi_trial_move2)(phase_t *php, uchar_t pc, int from, int to_1st, int to);
int (* yo_trial_move2)(phase_t *php, uchar_t pc, int from, int to_1st, int to);
// h_check.c, hs_check.c, d_check.c
int (* check)(phase_t *php);
// h_escape.c, hs_escape.c, d_escape.c
int (* escape)(phase_t *php);
// h_check.c, hs_check.c
void (* help_check_place)(phase_t *php);
void (* help_check_place_sub)(phase_t *php);
void (* help_check_move_sub)(phase_t *php);
// h_escape.c, hs_escape.c
void (* help_escape_place)(phase_t *php);
void (* help_escape_check_place)(phase_t *php);
void (* help_escape_place_sub)(phase_t *php);
void (* help_escape_move_sub)(phase_t *php);
void (* help_escape_check_place_sub)(phase_t *php);
void (* help_escape_check_move_sub)(phase_t *php);
// d_check.c
int (* direct_check_place)(phase_t *php);
int (* direct_check_place_sub)(phase_t *php);
int (* direct_check_move_sub)(phase_t *php);
// d_escape.c
int (* direct_escape_place)(phase_t *php);
int (* direct_escape_place_sub)(phase_t *php);
int (* direct_escape_move_sub)(phase_t *php);
// sub.c
void (* make_reborn)(phase_t *php, move_t *mvp, char *ob);
int (* is_symmetric)(phase_t *php);
// sub2.c
int (* test_symmetric)(phase_t *php);
// sub4.c
int (* is_hither_mate)(phase_t *php);
int (* is_yonder_mate)(phase_t *php);
int (* test_hither_mate_place)(phase_t *php);
int (* test_yonder_mate_place)(phase_t *php);
int (* test_hither_mate_sub)(phase_t *php);
int (* test_yonder_mate_sub)(phase_t *php);
// sub5.c
void (* hi_hash_diff_sub)(phase_t *php, phase_t *php_1, int from, int to);
void (* yo_hash_diff_sub)(phase_t *php, phase_t *php_1, int from, int to);

