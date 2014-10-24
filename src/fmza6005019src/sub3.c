// za/sub3.c -- zoo+single-change help(-self)-mate entry-setup subroutines
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/sub3.c 1.38 2014/08/18 12:54:33 budai10 Exp budai10 $

#include	<stdlib.h>
#include	"fm.h"
#include	"piece.h"

void
set_entry(void)
{
	int pc, exist_ts = FALSE;

	// ★酔象成の二玉詰と同じと錯覚。二玉詰はどちらかを獲れば良いが、太子は両方詰める必要があった。
//	for (pc = PC_F1; pc <= PC_F7; pc++) {
//		if (piece[pc].pc_no == FP_SZ && (piece[pc].attr & B_PA_DEFINED) &&
//				(piece[pc].attr & BM_PA_PROMOTABLE)) {
//			exist_ts = TRUE;
//		}
//	}
	// 詰ルール対応
	if (rule_flag & B_R_DIRECT_MATE) {
		// かしこ詰
		if (cond_flag & BM_C_MESSIGNY_FAMILY) {
			check = direct_check_messigny;
			escape = direct_escape_messigny;
			is_hither_mate = is_hither_mate_messigny;
			is_yonder_mate = is_yonder_mate_messigny;
		} else if (cond_flag & B_C_GREED) {
			check = direct_check_greed;
			escape = direct_escape_greed;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else if (cond_flag & B_C_ASCETIC) {
			check = direct_check_ascetic;
			escape = direct_escape_ascetic;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else {
			check = direct_check_normal;
			escape = direct_escape_normal;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		}
		if (loose == ON || (cond_flag & B_C_W_GY) || exist_ts == TRUE) {
			// /XA指定または二玉詰または太子に成れる酔象入り
			direct_check_place = direct_check_place_loose;
			direct_check_place_sub = direct_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_check_move_sub = direct_check_move_sub_circe;
			} else {
				direct_check_move_sub = direct_check_move_sub_loose;
			}
			direct_escape_place = direct_escape_place_loose;
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (fairy_move_style & (BM_MS_HOP_FAMILY | B_MS_MAO_STEP)) {
			// Ｇ系フェアリー駒
			direct_check_place = direct_check_place_loose;
			direct_check_place_sub = direct_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_check_move_sub = direct_check_move_sub_circe;
			} else {
				direct_check_move_sub = direct_check_move_sub_loose;
			}
			direct_escape_place = direct_escape_place_loose;
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (cond_flag & BM_C_MADRASI_FAMILY) {
			// マドラシ系
			direct_check_place = direct_check_place_loose;
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				direct_check_place_sub = direct_check_place_sub_loose;
			} else {
				direct_check_place_sub = direct_check_place_sub_madrasi;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_check_move_sub = direct_check_move_sub_circe;
			} else {
				direct_check_move_sub = direct_check_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				direct_escape_place = direct_escape_place_loose;
			} else {
				direct_escape_place = direct_escape_place_taihaink2mad;
			}
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				test_hither_mate_place = test_hither_mate_place_loose;
				test_yonder_mate_place = test_yonder_mate_place_loose;
			} else {
				test_hither_mate_place = test_hither_mate_place_taihaink2mad;
				test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
			}
		} else if (cond_flag & BM_C_CHANGE_FAMILY) {
			// 安南、安北、対面、背面、ネコ鮮、ネコネコ鮮
			direct_check_place = direct_check_place_loose;
			if (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
				direct_check_place_sub = direct_check_place_sub_annanhoku;
			} else if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
				direct_check_place_sub = direct_check_place_sub_taihaimen;
			} else {
				direct_check_place_sub = direct_check_place_sub_nekosen;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_check_move_sub = direct_check_move_sub_circe;
			} else {
				direct_check_move_sub = direct_check_move_sub_loose;
			}
			direct_escape_place = direct_escape_place_taihaink2mad;
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_taihaink2mad;
			test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
		} else if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
			// 局面制約系
			direct_check_place = direct_check_place_loose;
			direct_check_place_sub = direct_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_check_move_sub = direct_check_move_sub_circe;
			} else {
				direct_check_move_sub = direct_check_move_sub_loose;
			}
			direct_escape_place = direct_escape_place_loose;
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else {
			// 純系
			direct_check_place = direct_check_place_normal;
			direct_check_place_sub = direct_check_place_sub_normal;
			if (cond_flag & B_C_CIRCE) {
				direct_check_move_sub = direct_check_move_sub_normal_circe;
			} else if (cond_flag & B_C_ANTICIRCE) {
				direct_check_move_sub = direct_check_move_sub_loose;
			} else {
				direct_check_move_sub = direct_check_move_sub_normal;
			}
			direct_escape_place = direct_escape_place_normal;
			direct_escape_place_sub = direct_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				direct_escape_move_sub = direct_escape_move_sub_circe;
			} else {
				direct_escape_move_sub = direct_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_normal;
			test_yonder_mate_place = test_yonder_mate_place_normal;
		}
	} else if (rule_flag & (B_R_HELP_SELF_MATE | B_R_HELP_SELF_STALEMATE)) {
		// 協力自玉詰、協力自玉ステイルメイト
		if (cond_flag & BM_C_MESSIGNY_FAMILY) {
			check = help_self_check_messigny;
			escape = help_self_escape_messigny;
			is_hither_mate = is_hither_mate_messigny;
			is_yonder_mate = is_yonder_mate_messigny;
		} else if (cond_flag & B_C_GREED) {
			check = help_self_check_greed;
			escape = help_self_escape_greed;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else if (cond_flag & B_C_ASCETIC) {
			check = help_self_check_ascetic;
			escape = help_self_escape_ascetic;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else {
			check = help_self_check_normal;
			escape = help_self_escape_normal;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		}
		if (loose == ON || (cond_flag & B_C_W_GY) || exist_ts == TRUE) {
			// /XA指定または二玉詰または太子に成れる酔象入り
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_self_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_check_place = help_self_escape_check_place_loose;
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			help_escape_check_place_sub = help_self_escape_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_circe;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (fairy_move_style & (BM_MS_HOP_FAMILY | B_MS_MAO_STEP)) {
			// Ｇ系フェアリー駒
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_self_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_check_place = help_self_escape_check_place_loose;
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			help_escape_check_place_sub = help_self_escape_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_circe;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (cond_flag & BM_C_MADRASI_FAMILY) {
			// マドラシ系
			help_check_place = help_check_place_loose;
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				help_check_place_sub = help_self_check_place_sub_loose;
			} else {
				help_check_place_sub = help_self_check_place_sub_madrasi;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				help_escape_place = help_escape_place_loose;
				help_escape_check_place = help_self_escape_check_place_loose;
			} else {
				help_escape_place = help_escape_place_taihaink2mad;
				help_escape_check_place = help_self_escape_check_place_taihaink2mad;
			}
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				help_escape_check_place_sub = help_self_escape_check_place_sub_loose;
			} else {
				help_escape_check_place_sub = help_self_escape_check_place_sub_madrasi;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_circe;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				test_hither_mate_place = test_hither_mate_place_loose;
				test_yonder_mate_place = test_yonder_mate_place_loose;
			} else {
				test_hither_mate_place = test_hither_mate_place_taihaink2mad;
				test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
			}
		} else if (cond_flag & BM_C_CHANGE_FAMILY) {
			// 安南、安北、対面、背面、ネコ鮮、ネコネコ鮮
			help_check_place = help_check_place_loose;
			if (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
				help_check_place_sub = help_self_check_place_sub_annanhoku;
			} else if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
				help_check_place_sub = help_self_check_place_sub_taihaimen;
			} else {
				help_check_place_sub = help_self_check_place_sub_nekosen;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_taihaink2mad;
			help_escape_check_place = help_self_escape_check_place_taihaink2mad;
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			if (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
				help_escape_check_place_sub = help_self_escape_check_place_sub_annanhoku;
			} else if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
				help_escape_check_place_sub = help_self_escape_check_place_sub_taihaimen;
			} else {
				help_escape_check_place_sub = help_self_escape_check_place_sub_nekosen;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_circe;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_taihaink2mad;
			test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
		} else if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
			// 局面制約系
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_self_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_check_place = help_self_escape_check_place_loose;
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			help_escape_check_place_sub = help_self_escape_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_circe;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else {
			// 純系
			help_check_place = help_check_place_normal;
			help_check_place_sub = help_self_check_place_sub_normal;
			if (cond_flag & B_C_CIRCE) {
				help_check_move_sub = help_self_check_move_sub_normal_circe;
			} else if (cond_flag & B_C_ANTICIRCE) {
				help_check_move_sub = help_self_check_move_sub_circe;
			} else {
				help_check_move_sub = help_self_check_move_sub_normal;
			}
			help_escape_place = help_escape_place_normal;
			help_escape_check_place = help_self_escape_check_place_normal;
			help_escape_place_sub = help_self_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_self_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_self_escape_move_sub_loose;
			}
			help_escape_check_place_sub = help_self_escape_check_place_sub_normal;
			if (cond_flag & B_C_CIRCE) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_normal_circe;
			} else if (cond_flag & B_C_ANTICIRCE) {
				help_escape_check_move_sub = help_self_escape_check_move_sub_loose;
			} else {
				help_escape_check_move_sub = help_self_escape_check_move_sub_normal;
			}
			test_hither_mate_place = test_hither_mate_place_normal;
			test_yonder_mate_place = test_yonder_mate_place_normal;
		}
	} else {
		// 協力詰
		if (cond_flag & BM_C_MESSIGNY_FAMILY) {
			check = help_check_messigny;
			escape = help_escape_messigny;
			is_hither_mate = is_hither_mate_messigny;
			is_yonder_mate = is_yonder_mate_messigny;
		} else if (cond_flag & B_C_GREED) {
			check = help_check_greed;
			escape = help_escape_greed;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else if (cond_flag & B_C_ASCETIC) {
			check = help_check_ascetic;
			escape = help_escape_ascetic;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		} else {
			check = help_check_normal;
			escape = help_escape_normal;
			is_hither_mate = is_hither_mate_normal;
			is_yonder_mate = is_yonder_mate_normal;
		}
		if (loose == ON || (cond_flag & B_C_W_GY) || exist_ts == TRUE) {
			// /XA指定または二玉詰または太子に成れる酔象入り
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (fairy_move_style & (BM_MS_HOP_FAMILY | B_MS_MAO_STEP)) {
			// Ｇ系フェアリー駒
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else if (cond_flag & BM_C_MADRASI_FAMILY) {
			// マドラシ系
			help_check_place = help_check_place_loose;
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				help_check_place_sub = help_check_place_sub_loose;
			} else {
				help_check_place_sub = help_check_place_sub_madrasi;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				help_escape_place = help_escape_place_loose;
			} else {
				help_escape_place = help_escape_place_taihaink2mad;
			}
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			if (cond_flag & BM_C_CHANGE_FAMILY) {
				test_hither_mate_place = test_hither_mate_place_loose;
				test_yonder_mate_place = test_yonder_mate_place_loose;
			} else {
				test_hither_mate_place = test_hither_mate_place_taihaink2mad;
				test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
			}
		} else if (cond_flag & BM_C_CHANGE_FAMILY) {
			// 安南、安北、対面、背面、ネコ鮮、ネコネコ鮮
			help_check_place = help_check_place_loose;
			if (cond_flag & (B_C_ANNAN | B_C_ANHOKU)) {
				help_check_place_sub = help_check_place_sub_annanhoku;
			} else if (cond_flag & (B_C_TAIMEN | B_C_HAIMEN)) {
				help_check_place_sub = help_check_place_sub_taihaimen;
			} else {
				help_check_place_sub = help_check_place_sub_nekosen;
			}
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_taihaink2mad;
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_taihaink2mad;
			test_yonder_mate_place = test_yonder_mate_place_taihaink2mad;
		} else if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
			// 局面制約系
			help_check_place = help_check_place_loose;
			help_check_place_sub = help_check_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_loose;
			}
			help_escape_place = help_escape_place_loose;
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_loose;
			test_yonder_mate_place = test_yonder_mate_place_loose;
		} else {
			// 純系
			help_check_place = help_check_place_normal;
			help_check_place_sub = help_check_place_sub_normal;
			if (cond_flag & B_C_CIRCE) {
				help_check_move_sub = help_check_move_sub_normal_circe;
			} else if (cond_flag & B_C_ANTICIRCE) {
				help_check_move_sub = help_check_move_sub_circe;
			} else {
				help_check_move_sub = help_check_move_sub_normal;
			}
			help_escape_place = help_escape_place_normal;
			help_escape_place_sub = help_escape_place_sub_loose;
			if (cond_flag & (B_C_CIRCE | B_C_ANTICIRCE)) {
				help_escape_move_sub = help_escape_move_sub_circe;
			} else {
				help_escape_move_sub = help_escape_move_sub_loose;
			}
			test_hither_mate_place = test_hither_mate_place_normal;
			test_yonder_mate_place = test_yonder_mate_place_normal;
		}
	}
	// 打歩詰対応
	if (cond_flag & B_C_UCHIFU) {
		// 打歩詰
		if (cond_flag & BM_C_MESSIGNY_FAMILY) {
			test_hither_mate_sub = test_hither_mate_sub_mf;
			test_yonder_mate_sub = test_yonder_mate_sub_mf;
		} else {
			test_hither_mate_sub = test_hither_mate_sub_f;
			test_yonder_mate_sub = test_yonder_mate_sub_f;
		}
	} else {
		// 打歩詰以外
		if ((cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) &&
				(cond_flag & (B_C_CIRCE | B_C_ANTICIRCE))) {
			test_hither_mate_sub = test_hither_mate_sub_c;
			test_yonder_mate_sub = test_yonder_mate_sub_c;
		} else {
			test_hither_mate_sub = test_hither_mate_sub_n;
			test_yonder_mate_sub = test_yonder_mate_sub_n;
		}
	}
	// マドラシ・Isardam・変身対応
	if ((cond_flag & BM_C_MADRASI_FAMILY) && NOT(cond_flag & BM_C_CHANGE_FAMILY)) {
		// マドラシ・非変身
		analyze_phase = analyze_phase_madrasi;
		analyze_leaf_phase = analyze_leaf_phase_madrasi;
	} else if ((cond_flag & BM_C_ISARDAM_FAMILY) && NOT(cond_flag & BM_C_CHANGE_FAMILY)) {
		// Isardam・非変身
		analyze_phase = analyze_phase_isardam;
		analyze_leaf_phase = analyze_leaf_phase_isardam;
	} else if ((cond_flag & BM_C_MADRASI_FAMILY) && (cond_flag & BM_C_CHANGE_FAMILY)) {
		// マドラシ・変身
		analyze_phase = analyze_phase_change_madrasi;
		analyze_leaf_phase = analyze_leaf_phase_change_madrasi;
	} else if (NOT(cond_flag & BM_C_MADRASI_FAMILY) && (cond_flag & BM_C_CHANGE_FAMILY)) {
		// 非マドラシ・非Isardam・変身
		analyze_phase = analyze_phase_change;
		analyze_leaf_phase = analyze_leaf_phase_change;
	} else {
		// 非マドラシ・非Isardam・非変身
		analyze_phase = analyze_phase_normal;
		analyze_leaf_phase = analyze_leaf_phase_normal;
	}
	// 変身対応
	if (cond_flag & B_C_ANNAN) {
		// 安南
		setup_fpc_board = setup_fpc_board_annan;
	} else if (cond_flag & B_C_ANHOKU) {
		// 安北
		setup_fpc_board = setup_fpc_board_anhoku;
	} else if (cond_flag & B_C_TAIMEN) {
		// 対面
		setup_fpc_board = setup_fpc_board_taimen;
	} else if (cond_flag & B_C_HAIMEN) {
		// 背面
		setup_fpc_board = setup_fpc_board_haimen;
	} else if (cond_flag & B_C_NEKOSEN) {
		// ネコ鮮
		setup_fpc_board = setup_fpc_board_nekosen;
	} else if (cond_flag & B_C_NEKO2SEN) {
		// ネコネコ鮮
		setup_fpc_board = setup_fpc_board_neko2;
	} else {
		// 非変身
		setup_fpc_board = setup_fpc_board_normal;
	}
	// キルケ系対応（含Andernach, AntiAndernach, Messigny）
	if (cond_flag & B_C_PWC) {
		hi_move_sub = hi_move_sub_pwc;
		yo_move_sub = yo_move_sub_pwc;
		hi_move_leaf_sub = hi_move_leaf_sub_pwc;
		yo_move_leaf_sub = yo_move_leaf_sub_pwc;
		test_before_move_test = test_before_move_test_others;
		hi_hash_diff_sub = hi_hash_diff_sub_pwc;
		yo_hash_diff_sub = yo_hash_diff_sub_pwc;
		make_reborn = make_reborn_pwc;
		is_symmetric = is_symmetric_normal;
		test_symmetric = test_symmetric_normal;
	} else if (cond_flag & B_C_CIRCE) {
		hi_move_sub = hi_move_sub_circe;
		yo_move_sub = yo_move_sub_circe;
		hi_move_leaf_sub = hi_move_leaf_sub_circe;
		yo_move_leaf_sub = yo_move_leaf_sub_circe;
		test_before_move_test = test_before_move_test_others;
		hi_hash_diff_sub = hi_hash_diff_sub_circe;
		yo_hash_diff_sub = yo_hash_diff_sub_circe;
		make_reborn = make_reborn_circe;
		is_symmetric = is_symmetric_circe;
		test_symmetric = test_symmetric_circe;
	} else if (cond_flag & B_C_ANTICIRCE) {
		hi_move_sub = hi_move_sub_anticirce;
		yo_move_sub = yo_move_sub_anticirce;
		hi_move_leaf_sub = hi_move_leaf_sub_anticirce;
		yo_move_leaf_sub = yo_move_leaf_sub_anticirce;
		test_before_move_test = test_before_move_test_anticirce_andernach;
		hi_hash_diff_sub = hi_hash_diff_sub_anticirce;
		yo_hash_diff_sub = yo_hash_diff_sub_anticirce;
		make_reborn = make_reborn_anticirce;
		is_symmetric = is_symmetric_circe;
		test_symmetric = test_symmetric_circe;
	} else if (cond_flag & B_C_ANDERNACH) {
		hi_move_sub = hi_move_sub_andernach;
		yo_move_sub = yo_move_sub_andernach;
		hi_move_leaf_sub = hi_move_leaf_sub_andernach;
		yo_move_leaf_sub = yo_move_leaf_sub_andernach;
		test_before_move_test = test_before_move_test_anticirce_andernach;
		hi_hash_diff_sub = hi_hash_diff_sub_andernach;
		yo_hash_diff_sub = yo_hash_diff_sub_andernach;
		make_reborn = NULL;
		is_symmetric = is_symmetric_normal;
		test_symmetric = test_symmetric_normal;
	} else if (cond_flag & B_C_ANTIANDERNACH) {
		hi_move_sub = hi_move_sub_antiandernach;
		yo_move_sub = yo_move_sub_antiandernach;
		hi_move_leaf_sub = hi_move_leaf_sub_antiandernach;
		yo_move_leaf_sub = yo_move_leaf_sub_antiandernach;
		test_before_move_test = test_before_move_test_antiandernach;
		hi_hash_diff_sub = hi_hash_diff_sub_andernach;
		yo_hash_diff_sub = yo_hash_diff_sub_andernach;
		make_reborn = NULL;
		is_symmetric = is_symmetric_normal;
		test_symmetric = test_symmetric_normal;
	} else if (cond_flag & BM_C_MESSIGNY_FAMILY) {
		hi_move_sub = hi_move_sub_exchange;
		yo_move_sub = yo_move_sub_exchange;
		hi_move_leaf_sub = hi_move_leaf_sub_exchange;
		yo_move_leaf_sub = yo_move_leaf_sub_exchange;
		test_before_move_test = test_before_move_test_others;
		hi_hash_diff_sub = hi_hash_diff_sub_exchange;
		yo_hash_diff_sub = yo_hash_diff_sub_exchange;
		make_reborn = make_reborn_exchange;
		is_symmetric = is_symmetric_normal;
		test_symmetric = test_symmetric_normal;
	} else {
		hi_move_sub = hi_move_sub_normal;
		yo_move_sub = yo_move_sub_normal;
		hi_move_leaf_sub = hi_move_leaf_sub_normal;
		yo_move_leaf_sub = yo_move_leaf_sub_normal;
		test_before_move_test = test_before_move_test_others;
		hi_hash_diff_sub = hi_hash_diff_sub_normal;
		yo_hash_diff_sub = yo_hash_diff_sub_normal;
		make_reborn = NULL;
		is_symmetric = is_symmetric_normal;
		test_symmetric = test_symmetric_normal;
	}
	if (restarting == ON) {
		hi_move = hi_move_restarting;
		yo_move = yo_move_restarting;
		hi_move_leaf = hi_move_leaf_normal;
		yo_move_leaf = yo_move_leaf_normal;
	} else if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
		switch (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
		case B_C_KOKO:
			hi_move = hi_move_phase_constraint;
			yo_move = yo_move_phase_constraint;
			hi_move_leaf = hi_move_leaf_phase_constraint;
			yo_move_leaf = yo_move_leaf_phase_constraint;
			hi_trial_move = hi_trial_move_normal;
			yo_trial_move = yo_trial_move_normal;
			hi_trial_move2 = hi_trial_move2_normal;
			yo_trial_move2 = yo_trial_move2_normal;
			break;
		case B_C_ISARDAM:
			hi_move = hi_move_normal;
			yo_move = yo_move_normal;
			hi_move_leaf = hi_move_leaf_normal;
			yo_move_leaf = yo_move_leaf_normal;
			hi_trial_move = hi_trial_move_isardam;
			yo_trial_move = yo_trial_move_isardam;
			// Isardamでは、二手移動駒をサポートしていない。
			break;
		case B_C_INVALID_CAPT_OU_2F:
			hi_move = hi_move_normal;
			yo_move = yo_move_normal;
			hi_move_leaf = hi_move_leaf_normal;
			yo_move_leaf = yo_move_leaf_normal;
			hi_trial_move = hi_trial_move_normal;
			yo_trial_move = yo_trial_move_normal;
			hi_trial_move2 = hi_trial_move2_normal;
			yo_trial_move2 = yo_trial_move2_normal;
			break;
		}
	} else {
		hi_move = hi_move_normal;
		yo_move = yo_move_normal;
		hi_move_leaf = hi_move_leaf_normal;
		yo_move_leaf = yo_move_leaf_normal;
		// 局面制約系でなければ、xx_trial_move*は呼ばれない。
	}
	// 駒毎利き対応
	for (pc = PC_FU; pc < PC_END; pc++) {
		piece_t *pap = &piece[pc];
		if ((pap->attr & B_PA_DEFINED) && ((pap->attr & B_PA_PIECE) || pc == ROCK)) {
			if (pap->move_style & B_MS_LEAP) {
				// 跳び駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_leap = analyze_hi_leap_phase_constraint;
					pap->analyze_yo_leap = analyze_yo_leap_phase_constraint;
				} else {
					pap->analyze_hi_leap = analyze_hi_leap;
					pap->analyze_yo_leap = analyze_yo_leap;
				}
				pap->analyze_hi_leap_madrasi = analyze_hi_leap_madrasi;
				pap->analyze_yo_leap_madrasi = analyze_yo_leap_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_leap_phase_constraint;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_leap_phase_constraint;
				} else {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_leap;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_leap;
				}
				pap->help_check_leap = help_check_leap;
				pap->help_escape_leap = help_escape_leap;
				pap->direct_check_leap = direct_check_leap;
				pap->direct_escape_leap = direct_escape_leap;
				pap->test_hither_mate_leap = test_hither_mate_leap;
				pap->test_yonder_mate_leap = test_yonder_mate_leap;
			} else if (pap->move_style & B_MS_SHISHI_LEAP) {
				// 獅子跳び駒
				// 単純な当たりについては、leapと同じ
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_leap = analyze_hi_leap_phase_constraint;
					pap->analyze_yo_leap = analyze_yo_leap_phase_constraint;
				} else {
					pap->analyze_hi_leap = analyze_hi_leap;
					pap->analyze_yo_leap = analyze_yo_leap;
				}
				pap->analyze_hi_leap_madrasi = analyze_hi_leap_madrasi;
				pap->analyze_yo_leap_madrasi = analyze_yo_leap_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_leap_phase_constraint;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_leap_phase_constraint;
				} else {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_leap;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_leap;
				}
				pap->help_check_leap = help_check_shishi_leap;
				pap->help_escape_leap = help_escape_shishi_leap;
				pap->direct_check_leap = direct_check_shishi_leap;
				pap->direct_escape_leap = direct_escape_shishi_leap;
				pap->test_hither_mate_leap = test_hither_mate_shishi_leap;
				pap->test_yonder_mate_leap = test_yonder_mate_shishi_leap;
			} else if (pap->move_style & B_MS_STEP) {
				// 跳々駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_leap = analyze_hi_step_phase_constraint;
					pap->analyze_yo_leap = analyze_yo_step_phase_constraint;
				} else {
					pap->analyze_hi_leap = analyze_hi_step;
					pap->analyze_yo_leap = analyze_yo_step;
				}
				pap->analyze_hi_leap_madrasi = analyze_hi_step_madrasi;
				pap->analyze_yo_leap_madrasi = analyze_yo_step_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_step_phase_constraint;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_step_phase_constraint;
				} else {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_step;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_step;
				}
				pap->help_check_leap = help_check_step;
				pap->help_escape_leap = help_escape_step;
				pap->direct_check_leap = direct_check_step;
				pap->direct_escape_leap = direct_escape_step;
				pap->test_hither_mate_leap = test_hither_mate_step;
				pap->test_yonder_mate_leap = test_yonder_mate_step;
			} else if (pap->move_style & B_MS_MAO_STEP) {
				// Mao跳び駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_leap = analyze_hi_mao_step_phase_constraint;
					pap->analyze_yo_leap = analyze_yo_mao_step_phase_constraint;
				} else {
					pap->analyze_hi_leap = analyze_hi_mao_step;
					pap->analyze_yo_leap = analyze_yo_mao_step;
				}
				pap->analyze_hi_leap_madrasi = analyze_hi_mao_step_madrasi;
				pap->analyze_yo_leap_madrasi = analyze_yo_mao_step_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_mao_step_phase_constraint;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_mao_step_phase_constraint;
				} else {
					pap->analyze_leaf_hi_leap = analyze_leaf_hi_mao_step;
					pap->analyze_leaf_yo_leap = analyze_leaf_yo_mao_step;
				}
				pap->help_check_leap = help_check_mao_step;
				pap->help_escape_leap = help_escape_mao_step;
				pap->direct_check_leap = direct_check_mao_step;
				pap->direct_escape_leap = direct_escape_mao_step;
				pap->test_hither_mate_leap = test_hither_mate_mao_step;
				pap->test_yonder_mate_leap = test_yonder_mate_mao_step;
			} else {
				// それら以外
				pap->analyze_hi_leap = analyze_null;
				pap->analyze_yo_leap = analyze_null;
				pap->analyze_hi_leap_madrasi = analyze_null_madrasi;
				pap->analyze_yo_leap_madrasi = analyze_null_madrasi;
				pap->analyze_leaf_hi_leap = analyze_leaf_null;
				pap->analyze_leaf_yo_leap = analyze_leaf_null;
				pap->help_check_leap = help_move_null;
				pap->help_escape_leap = help_move_null;
				pap->direct_check_leap = direct_move_null;
				pap->direct_escape_leap = direct_move_null;
				pap->test_hither_mate_leap = test_mate_null;
				pap->test_yonder_mate_leap = test_mate_null;
			}
			if (pap->move_style & B_MS_RUN) {
				// 走り駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_run_phase_constraint;
					pap->analyze_yo_run = analyze_yo_run_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_run;
					pap->analyze_yo_run = analyze_yo_run;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_run_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_run_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_run_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_run_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_run;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_run;
				}
				pap->help_check_run = help_check_run;
				pap->help_escape_run = help_escape_run;
				pap->direct_check_run = direct_check_run;
				pap->direct_escape_run = direct_escape_run;
				pap->test_hither_mate_run = test_hither_mate_run;
				pap->test_yonder_mate_run = test_yonder_mate_run;
			} else if (pap->move_style & B_MS_HOP) {
				// Ｇホップ駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_hop_phase_constraint;
					pap->analyze_yo_run = analyze_yo_hop_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_hop;
					pap->analyze_yo_run = analyze_yo_hop;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_hop_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_hop_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_hop_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_hop_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_hop;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_hop;
				}
				pap->help_check_run = help_check_hop;
				pap->help_escape_run = help_escape_hop;
				pap->direct_check_run = direct_check_hop;
				pap->direct_escape_run = direct_escape_hop;
				pap->test_hither_mate_run = test_hither_mate_hop;
				pap->test_yonder_mate_run = test_yonder_mate_hop;
			} else if (pap->move_style & B_MS_DECAPT_HOP) {
				// 蝗ホップ駒
				pap->analyze_hi_run = analyze_hi_decapt_hop;
				pap->analyze_yo_run = analyze_yo_decapt_hop;
				pap->analyze_hi_run_madrasi = analyze_hi_decapt_hop_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_decapt_hop_madrasi;
				pap->analyze_leaf_hi_run = analyze_leaf_hi_decapt_hop;
				pap->analyze_leaf_yo_run = analyze_leaf_yo_decapt_hop;
				pap->help_check_run = help_check_decapt_hop;
				pap->help_escape_run = help_escape_decapt_hop;
				pap->direct_check_run = direct_check_decapt_hop;
				pap->direct_escape_run = direct_escape_decapt_hop;
				pap->test_hither_mate_run = test_hither_mate_decapt_hop;
				pap->test_yonder_mate_run = test_yonder_mate_decapt_hop;
			} else if (pap->move_style & B_MS_PAO_JUMP) {
				// 包ジャンプ駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_jump_phase_constraint;
					pap->analyze_yo_run = analyze_yo_jump_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_jump;
					pap->analyze_yo_run = analyze_yo_jump;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_jump_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_jump_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump;
				}
				pap->help_check_run = help_check_pao_jump;
				pap->help_escape_run = help_escape_pao_jump;
				pap->direct_check_run = direct_check_pao_jump;
				pap->direct_escape_run = direct_escape_pao_jump;
				pap->test_hither_mate_run = test_hither_mate_pao_jump;
				pap->test_yonder_mate_run = test_yonder_mate_pao_jump;
			} else if (pap->move_style & B_MS_JUMP) {
				// 鬣ジャンプ駒
				// 当たりについては、pao_jumpと同じ
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_jump_phase_constraint;
					pap->analyze_yo_run = analyze_yo_jump_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_jump;
					pap->analyze_yo_run = analyze_yo_jump;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_jump_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_jump_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump;
				}
				pap->help_check_run = help_check_jump;
				pap->help_escape_run = help_escape_jump;
				pap->direct_check_run = direct_check_jump;
				pap->direct_escape_run = direct_escape_jump;
				pap->test_hither_mate_run = test_hither_mate_jump;
				pap->test_yonder_mate_run = test_yonder_mate_jump;
			} else if (pap->move_style & B_MS_JUMP_HOP) {
				// 考ホップ駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_jump_hop_phase_constraint;
					pap->analyze_yo_run = analyze_yo_jump_hop_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_jump_hop;
					pap->analyze_yo_run = analyze_yo_jump_hop;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_jump_hop_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_jump_hop_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump_hop_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump_hop_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_jump_hop;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_jump_hop;
				}
				pap->help_check_run = help_check_jump_hop;
				pap->help_escape_run = help_escape_jump_hop;
				pap->direct_check_run = direct_check_jump_hop;
				pap->direct_escape_run = direct_escape_jump_hop;
				pap->test_hither_mate_run = test_hither_mate_jump_hop;
				pap->test_yonder_mate_run = test_yonder_mate_jump_hop;
			} else if (pap->move_style & (B_MS_REFL135 | B_MS_REFL90)) {
				// 雀ホップ駒、鷲ホップ駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_refl_phase_constraint;
					pap->analyze_yo_run = analyze_yo_refl_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_refl;
					pap->analyze_yo_run = analyze_yo_refl;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_refl_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_refl_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_refl_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_refl_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_refl;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_refl;
				}
				pap->help_check_run = help_check_refl;
				pap->help_escape_run = help_escape_refl;
				pap->direct_check_run = direct_check_refl;
				pap->direct_escape_run = direct_escape_refl;
				pap->test_hither_mate_run = test_hither_mate_refl;
				pap->test_yonder_mate_run = test_yonder_mate_refl;
			} else if (pap->move_style & (B_MS_EQUI_JUMP | B_MS_NS_EQUI_JUMP)) {
				// 均等ジャンプ駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_equi_hop_phase_constraint;
					pap->analyze_yo_run = analyze_yo_equi_hop_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_equi_hop;
					pap->analyze_yo_run = analyze_yo_equi_hop;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_equi_hop_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_equi_hop_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_equi_hop_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_equi_hop_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_equi_hop;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_equi_hop;
				}
				pap->help_check_run = help_check_equi_hop;
				pap->help_escape_run = help_escape_equi_hop;
				pap->direct_check_run = direct_check_equi_hop;
				pap->direct_escape_run = direct_escape_equi_hop;
				pap->test_hither_mate_run = test_hither_mate_equi_hop;
				pap->test_yonder_mate_run = test_yonder_mate_equi_hop;
			} else if (pap->move_style & B_MS_CIRCLE_RUN) {
				// 回転走り駒
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_hi_run = analyze_hi_run_phase_constraint;
					pap->analyze_yo_run = analyze_yo_run_phase_constraint;
				} else {
					pap->analyze_hi_run = analyze_hi_run;
					pap->analyze_yo_run = analyze_yo_run;
				}
				pap->analyze_hi_run_madrasi = analyze_hi_run_madrasi;
				pap->analyze_yo_run_madrasi = analyze_yo_run_madrasi;
				if (cond_flag & BM_C_PHASE_CONSTRAINT_FAMILY) {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_run_phase_constraint;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_run_phase_constraint;
				} else {
					pap->analyze_leaf_hi_run = analyze_leaf_hi_run;
					pap->analyze_leaf_yo_run = analyze_leaf_yo_run;
				}
				pap->help_check_run = help_check_circle_run;
				pap->help_escape_run = help_escape_circle_run;
				pap->direct_check_run = direct_check_circle_run;
				pap->direct_escape_run = direct_escape_circle_run;
				pap->test_hither_mate_run = test_hither_mate_run;
				pap->test_yonder_mate_run = test_yonder_mate_run;
			} else {
				// それら以外
				pap->analyze_hi_run = analyze_null;
				pap->analyze_yo_run = analyze_null;
				pap->analyze_hi_run_madrasi = analyze_null_madrasi;
				pap->analyze_yo_run_madrasi = analyze_null_madrasi;
				pap->analyze_leaf_hi_run = analyze_leaf_null;
				pap->analyze_leaf_yo_run = analyze_leaf_null;
				pap->help_check_run = help_move_null;
				pap->help_escape_run = help_move_null;
				pap->direct_check_run = direct_move_null;
				pap->direct_escape_run = direct_move_null;
				pap->test_hither_mate_run = test_mate_null;
				pap->test_yonder_mate_run = test_mate_null;
			}
		}
	}
#if 0
	for (pc = PC_FU; pc < PC_END; pc++) {
		piece_t *pap = &piece[pc];
		if ((pap->attr & B_PA_DEFINED) && ((pap->attr & B_PA_PIECE) || pc == ROCK)) {
			printf("pc = %s ", graph[pc]);
			printf("ahl = %p ", pap->analyze_hi_leap);
			printf("ahr = %p ", pap->analyze_hi_run);
			printf("ayl = %p ", pap->analyze_yo_leap);
			printf("ayr = %p\n", pap->analyze_yo_run);
		}
	}
#endif
#if 0
	if (rule_flag & B_R_DIRECT_MATE) {
		printf("dcp = %p ", direct_check_place);
		printf("dcps = %p ", direct_check_place_sub);
		printf("dcms = %p ", direct_check_move_sub);
		printf("dep = %p ", direct_escape_place);
		printf("deps = %p ", direct_escape_place_sub);
		printf("dems = %p ", direct_escape_move_sub);
		printf("ihmi = %p ", is_hi_mate_in);
		printf("iymi = %p\n", is_yo_mate_in);
	} else if (rule_flag & (B_R_HELP_SELF_MATE | B_R_HELP_SELF_STALEMATE)) {
		printf("hcp = %p ", help_check_place);
		printf("hcps = %p ", help_check_place_sub);
		printf("hcms = %p ", help_check_move_sub);
		printf("hep = %p ", help_escape_place);
		printf("hecp = %p ", help_escape_check_place);
		printf("heps = %p ", help_escape_place_sub);
		printf("hems = %p ", help_escape_move_sub);
		printf("hecps = %p ", help_escape_check_place_sub);
		printf("hecms = %p ", help_escape_check_move_sub);
		printf("ihmi = %p ", is_hi_mate_in);
		printf("iymi = %p\n", is_yo_mate_in);
	} else {
		printf("hcp = %p ", help_check_place);
		printf("hcps = %p ", help_check_place_sub);
		printf("hcms = %p ", help_check_move_sub);
		printf("hep = %p ", help_escape_place);
		printf("heps = %p ", help_escape_place_sub);
		printf("hems = %p ", help_escape_move_sub);
		printf("ihmi = %p ", is_hi_mate_in);
		printf("iymi = %p\n", is_yo_mate_in);
	}
#endif

	return;
}

