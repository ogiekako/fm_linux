// nfm/nfm/message.c -- Japanese messages
// Copyright(c) KAMINA Jiro 2006-2013.  All rights reserved.
// $Header$

#include	<stdlib.h>

const char *msg[] = {
	NULL,
	"%s %s - Copyright(c) KAMINA Jiro 2006-2013\n",
	"%s %s - %s\n",
	"解析しますか？ (Yes/No/eXit) ",
	"継続しますか？ (Yes/No) ",
	"中断...\n",
	"不詰...\n",
	"%s %s手目 %s まで\n",
	"別解：初手より\n",
	"別解：%s手目より\n",
	"非限定：初手より\n",
	"非限定：%s手目より\n",
	"早詰：\n",
	"統計情報:\n",
	"攻方ハッシュ:       合計 /       既出 /       新規\n",
	"    検査回数  %10s / %10s / %10s\n",
	"平均比較回数  %10.2f / %10.2f / %10.2f\n",
	"受方ハッシュ:       合計 /       既出 /       新規\n",
	"    検査回数  %10s / %10s / %10s\n",
	"平均比較回数  %10.2f / %10.2f / %10.2f\n",
	"メモリ割当てエラー...\n",
	"ファイル %s オープンエラー（%s モード）...\n",
	"入力形式: nfm [/I]入力ファイル [/O出力ファイル] [/T処理方式フラグ]\n"
		"              [/P出力フラグ] [/Sサイズフラグ] [/E動作環境フラグ]\n"
		"          処理方式フラグ: E, M, S, R, { F- | F+ | H- | H+ }\n"
		"          出力フラグ: C, E, H, L, M, N, T, S, V\n"
		"          サイズフラグ: B={最大盤面セル数|最大盤面セルメモリサイズMB}\n"
		"                        M={最大手順セル数|最大手順セルメモリサイズMB}\n"
		"          動作環境フラグ: A, R=物理メモリサイズMB, V=仮想メモリサイズMB\n",
	"入力データエラー（%s）...\n",
	"%s手目：%s：%s\n",
	"盤面セル溢れ: %s (%sMB) + %s (%sMB)\n手順セル状況: %s (%sMB)\n",
	"盤面セル溢れ: %s (%sMB)\n手順セル状況: %s (%sMB)\n",
	"手順セル溢れ: %s (%sMB) + %s (%sMB)\n盤面セル状況: %s (%sMB)\n",
	"手順セル溢れ: %s (%sMB)\n盤面セル状況: %s (%sMB)\n",
	"盤面セル追加: %11s (%5sMB) / 空き %5sMB\n",
	"手順セル追加: %11s (%5sMB) / 空き %5sMB\n",
	"手順セル解放: %11s / 解放深度 %5s / 到達深度 %5s\n",
	"全物理メモリサイズ = %13sB (%5sMB)\n",
	"空物理メモリサイズ = %13sB (%5sMB)\n",
	"全仮想メモリサイズ = %13sB (%5sMB)\n",
	"空仮想メモリサイズ = %13sB (%5sMB)\n",
	"    局面数:       生成 /       有効 /       新規 /       唯一\n",
	"% 6d手目: %10s / %10s / %10s / %10s\n",
	"リセットメモリサイズ = %sMB",
	"手順セル再利用開始...\n",
	"手順セル再利用（%ld）...\n",
};

const char *str[] = {
	NULL,
	"経過時間: %d秒",
	"経過時間: %d分 %d秒",
	"経過時間: %ld時間 %d分 %d秒",
	"解析局面数: %s",
	"解析深度: %s",
	"最終深度: %s",
	"解析終了",
	"解析中断",
	"協力詰",
	"協力詰（目標図）",
	"協力詰（目標図・受先）",
	"協力自玉詰",
	"協力自玉詰（目標図）",
	"協力自玉詰（目標図・受先）",
	"連続協力詰",
	"連続詰",
	"協力千日手",
	"PWC",
	"成禁",
	"NOPROM",
	"  ９ ８ ７ ６ ５ ４ ３ ２ １",
	"まで %s手",
	"まで %s+1手",
	"余り",
	"以下",
	"%s手",
	"%s+1手",
	"持駒：",
	"なし",
	"同",
	"打",
	"成",
	"生",
	"不成",
	"右",
	"左",
	"上",
	"引",
	"寄",
	"跳",
	"直",
	"行",
	"同一局面",
	"『",
	"』",
	"より",
	"まで",
	"開始図",
	"終了図",
	"詰上図",
	"目標図",
	"ルールと詰手数の解析",
	"継続データの読込み",
	"初形データの解析",
	"目標図データの解析",
	"拡張データの解析",
	"指定詰手順の解析",
	"手順",
	"駒",
	"移動元",
	"移動先",
	"捕獲駒",
	"復活",
	"成位置",
	"成駒",
	"移動",
	"局面",
	"[問題]",
	"[PROBLEM]",
	"[終了]",
	"[END]",
};

static const char *indata_00_pc[] = { NULL, };
static const char *indata_fu_pc[] =	{ "歩", "FU", NULL, };
static const char *indata_ky_pc[] =	{ "香", "KY", NULL, };
static const char *indata_ke_pc[] =	{ "桂", "KE", NULL, };
static const char *indata_gi_pc[] =	{ "銀", "GI", NULL, };
static const char *indata_ki_pc[] =	{ "金", "KI", NULL, };
static const char *indata_ka_pc[] =	{ "角", "KA", NULL, };
static const char *indata_hi_pc[] =	{ "飛", "HI", NULL, };
static const char *indata_08_pc[] =	{ NULL, };
static const char *indata_to_pc[] =	{ "と", "TO", NULL, };
static const char *indata_ny_pc[] =	{ "杏", "NY", NULL, };
static const char *indata_nk_pc[] =	{ "圭", "NK", NULL, };
static const char *indata_ng_pc[] =	{ "全", "NG", NULL, };
static const char *indata_ou_pc[] =	{ "王", "玉", "OU", "GY", NULL, };
static const char *indata_um_pc[] =	{ "馬", "UM", NULL, };
static const char *indata_ry_pc[] =	{ "龍", "竜", "RY", NULL, };
static const char *indata_16_pc[] =	{ NULL, };
static const char *indata_rock[] =	{ "石", "●", "RK", NULL, };
static const char *indata_hole[] =	{ "穴", "○", "HL", NULL, };
const char **indata_pc[19] = {
	indata_00_pc, indata_fu_pc, indata_ky_pc, indata_ke_pc,
	indata_gi_pc, indata_ki_pc, indata_ka_pc, indata_hi_pc,
	indata_08_pc, indata_to_pc, indata_ny_pc, indata_nk_pc,
	indata_ng_pc, indata_ou_pc, indata_um_pc, indata_ry_pc,
	indata_16_pc, indata_rock,  indata_hole,
};
const char *graph_pc[] = {
	NULL, "歩", "香", "桂", "銀", "金", "角", "飛",
	"□", "と", "杏", "圭", "全", "玉", "馬", "龍",
	"□", "●", "○",
};
const char *graph_mv_pc[] = {
	NULL, "歩", "香", "桂", "銀", "金", "角", "飛",
	"□", "と", "杏", "圭", "全", "玉", "馬", "龍",
};
const char *graph_hi_pc[] = {
	" ・", " 歩", " 香", " 桂", " 銀", " 金", " 角", " 飛",
	" □", " と", " 杏", " 圭", " 全", " 王", " 馬", " 龍",
};
const char *graph_yo_pc[] = {
	" ・", "v歩", "v香", "v桂", "v銀", "v金", "v角", "v飛",
	"v□", "vと", "v杏", "v圭", "v全", "v玉", "v馬", "v龍",
};
const char *graph_ex_pc[] = {
	" ・", " ●", " ○", " □", " □", " □", " □", " □",
};
const char *file_num[] = {
	NULL, "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
const char *rank_num[] = {
	NULL, "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
const char *board_rank_num[] = {
	NULL, "一", "二", "三", "四", "五", "六", "七", "八", "九",
};

