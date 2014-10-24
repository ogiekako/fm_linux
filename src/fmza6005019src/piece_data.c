// zaco/piece_data.c -- FM piece data
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/piece_data.c 1.28 2014/08/18 12:54:31 budai10 Exp budai10 $

#include	<time.h>
#include	<conio.h>
#include	"fm.h"
#include	"piece.h"

uchar_t hand_pc_order[] = {
	PC_HI, PC_KA, PC_KI, PC_GI, PC_KE, PC_KY, PC_FU,
	PC_F1, PC_F2, PC_F3, PC_F4, PC_F5, PC_F6, PC_F7, NUL,
};

// --------------
//  移動ベクトル
// --------------
// 移動なし
static vec_t vec_nul[] = { { 0, 0 }, };
// 前面  . . . . . | . . . . .
//       . . * . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fw[] = { { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_fw[] = { { 0, 1 }, { 0, 0 }, };
// 桂馬  . . . . . . . | . . . . . . .
//       . . L . L . . | . . . . . . .
//       . . . . . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . . . . .
//       . . . . . . . | . . L . L . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_ke[] = { { -1, -2 }, { 1, -2 }, { 0, 0 }, };
static vec_t yo_vec_ke[] = { { -1, 2 }, { 1, 2 }, { 0, 0 }, };
// 銀将  . . . . . | . . . . .
//       . L L L . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . L L * .
//       . . . . . | . . . . .
static vec_t hi_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, -1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// 金将  . . . . . | . . . . .
//       . L L L . | . . L . .
//       . L # L . | . L # L .
//       . . L . . | . L L L .
//       . . . . . | . . . . .
static vec_t hi_vec_ki[] = {
	{ -1, -1 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 },
	{ 0, 0 },
};
static vec_t yo_vec_ki[] = {
	{ -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// 角行  . . . . .
//       . * . * .
//       . . # . .
//       . * . * .
//       . . . . .
static vec_t vec_ka[] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// 飛車  . . . . .
//       . . * . .
//       . * # * .
//       . . * . .
//       . . . . .
static vec_t vec_hi[] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, };
// 周囲  . . . . .
// 王将  . * * * .
// Queen . * # * .
// 奔王  . * * * .
// 太子  . . . . .
static vec_t vec_ar[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// Dabbaba
// . . . . . . .
// . . . L . . .
// . . . . . . .
// . L . # . L .
// . . . . . . .
// . . . L . . .
// . . . . . . .
static vec_t vec_da[] = { { -2, 0 }, { 0, -2 }, { 0, 2 }, { 2, 0 }, { 0, 0 }, };
// Threeleaper
// . . . . . . . . .
// . . . . L . . . .
// . . . . . . . . .
// . . . . . . . . .
// . L . . # . . L .
// . . . . . . . . .
// . . . . . . . . .
// . . . . L . . . .
// . . . . . . . . .
static vec_t vec_tl[] = { { -3, 0 }, { 0, -3 }, { 0, 3 }, { 3, 0 }, { 0, 0 }, };
// Knight（2014-08-05 Rose用にデータ順を右上から時計回りに変更）
// . . . . . . .   ・・・・・・・
// . . * . * . .   ・・⑦・⑧・・
// . * . . . * .   ・⑥・・・①・
// . . . # . . .   ・・・薔・・・
// . * . . . * .   ・⑤・・. ②・
// . . * . * . .   ・・④・③・・
// . . . . . . .   ・・・・・・・
static vec_t vec_kn[] = {
	{ -2, -1, 8 }, { -2, 1, 8 }, { -1, 2, 8 }, { 1, 2, 8 },
	{ 2, 1, 8 }, { 2, -1, 8 }, { 1, -2, 8 }, { -1, -2, 8 },
	{ 0, 0 },
};
// Camel
// . . . . . . . . .
// . . . L . L . . .
// . . . . . . . . .
// . L . . . . . L .
// . . . . # . . . .
// . L . . . . . L .
// . . . . . . . . .
// . . . L . L . . .
// . . . . . . . . .
static vec_t vec_cm[] = {
	{ -3, -1 }, { -3, 1 }, { -1, -3 }, { -1, 3 }, { 1, -3 }, { 1, 3 }, { 3, -1 }, { 3, 1 },
	{ 0, 0 },
};
// Giraffe
// . . . . . . . . . . .
// . . . . L . L . . . .
// . . . . . . . . . . .
// . . . . . . . . . . .
// . L . . . . . . . L .
// . . . . . # . . . . .
// . L . . . . . . . L .
// . . . . . . . . . . .
// . . . . . . . . . . .
// . . . . L . L . . . .
// . . . . . . . . . . .
static vec_t vec_gr[] = {
	{ -4, -1 }, { -4, 1 }, { -1, -4 }, { -1, 4 }, { 1, -4 }, { 1, 4 }, { 4, -1 }, { 4, 1 },
	{ 0, 0 },
};
// Flamingo
// . . . . . . . . . . . . . . .
// . . . . . . L . L . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . L . . . . . . . . . . . L .
// . . . . . . . # . . . . . . .
// . L . . . . . . . . . . . L .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . L . L . . . . . .
// . . . . . . . . . . . . . . .
static vec_t vec_fl[] = {
	{ -6, -1 }, { -6, 1 }, { -1, -6 }, { -1, 6 }, { 1, -6 }, { 1, 6 }, { 6, -1 }, { 6, 1 },
	{ 0, 0 },
};
// Alfil
// . . . . . . .
// . L . . . L .
// . . . . . . .
// . . . # . . .
// . . . . . . .
// . L . . . L .
// . . . . . . .
static vec_t vec_al[] = { { -2, -2 }, { -2, 2 }, { 2, -2 }, { 2, 2 }, { 0, 0 }, };
// Zebra
// . . . . . . . . .
// . . L . . . L . .
// . L . . . . . L .
// . . . . . . . . .
// . . . . # . . . .
// . . . . . . . . .
// . L . . . . . L .
// . . L . . . L . .
// . . . . . . . . .
static vec_t vec_zb[] = {
	{ -3, -2 }, { -3, 2 }, { -2, -3 }, { -2, 3 }, { 2, -3 }, { 2, 3 }, { 3, -2 }, { 3, 2 },
	{ 0, 0 },
};
// Tripper
// . . . . . . . . .
// . L . . . . . L .
// . . . . . . . . .
// . . . . . . . . .
// . . . . # . . . .
// . . . . . . . . .
// . . . . . . . . .
// . L . . . . . L .
// . . . . . . . . .
static vec_t vec_tr[] = { { -3, -3 }, { -3, 3 }, { 3, -3 }, { 3, 3 }, { 0, 0 }, };
// Five-Leaper
// . . . . . . . . . . . . .
// . . . . . . L . . . . . .
// . . . L . . . . . L . . .
// . . L . . . . . . . L . .
// . . . . . . . . . . . . .
// . . . . . . . . . . . . .
// . L . . . . # . . . . L .
// . . . . . . . . . . . . .
// . . . . . . . . . . . . .
// . . L . . . . . . . L . .
// . . . L . . . . . L . . .
// . . . . . . L . . . . . .
// . . . . . . . . . . . . .
static vec_t vec_l5[] = {
	{ -5, 0 }, { -4, -3 }, { -4, 3 }, { -3, -4 }, { -3, 4 }, { 0, -5 },
	{ 0, 5 }, { 3, -4 }, { 3, 4 }, { 4, -3 }, { 4, 3 }, { 5, 0 },
	{ 0, 0 },
};
// Root-40-Leaper
// . . . . . . . . . . . . . . .
// . . . . . L . . . L . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . L . . . . . . . . . . . L .
// . . . . . . . . . . . . . . .
// . . . . . . . # . . . . . . .
// . . . . . . . . . . . . . . .
// . L . . . . . . . . . . . L .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . .
// . . . . . L . . . L . . . . .
// . . . . . . . . . . . . . . .
static vec_t vec_r4[] = {
	{ -6, -2 }, { -6, 2 }, { -2, -6 }, { -2, 6 }, { 2, -6 }, { 2, 6 }, { 6, -2 }, { 6, 2 },
	{ 0, 0 },
};
// Root-50-Leaper
// . . . . . . . . . . . . . . . . .
// . . . . . . . L . L . . . . . . .
// . . . . . . . . . . . . . . . . .
// . . . L . . . . . . . . . L . . .
// . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . .
// . L . . . . . . . . . . . . . L .
// . . . . . . . . # . . . . . . . .
// . L . . . . . . . . . . . . . L .
// . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . .
// . . . L . . . . . . . . . L . . .
// . . . . . . . . . . . . . . . . .
// . . . . . . . L . L . . . . . . .
// . . . . . . . . . . . . . . . . .
static vec_t vec_r5[] = {
	{ -7, -1 }, { -7, 1 }, { -5, -5 }, { -5, 5 }, { -1, -7 }, { -1, 7 },
	{ 1, -7 }, { 1, 7 }, { 5, -5 }, { 5, 5 }, { 7, -1 }, { 7, 1 },
	{ 0, 0 },
};
// Root-65-Leaper
// . . . . . . . . . . . . . . . . . . .
// . . . . . . . . L . L . . . . . . . .
// . . . . . L . . . . . . . L . . . . .
// . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . .
// . . L . . . . . . . . . . . . . L . .
// . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . .
// . L . . . . . . . . . . . . . . . L .
// . . . . . . . . . # . . . . . . . . .
// . L . . . . . . . . . . . . . . . L .
// . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . .
// . . L . . . . . . . . . . . . . L . .
// . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . .
// . . . . . L . . . . . . . L . . . . .
// . . . . . . . . L . L . . . . . . . .
// . . . . . . . . . . . . . . . . . . .
static vec_t vec_r6[] = {
	{ -8, -1 }, { -8, 1 }, { -7, -4 }, { -7, 4 }, { -4, -7 }, { -4, 7 }, { -1, -8 }, { -1, 8 },
	{ 1, -8 }, { 1, 8 }, { 4, -7 }, { 4, 7 }, { 7, -4 }, { 7, 4 }, { 8, -1 }, { 8, 1 },
	{ 0, 0 },
};
// Shan（相）
// . . . . . . .
// . S . . . S .
// . . S . S . .
// . . . # . . .
// . . S . S . .
// . S . . . S .
// . . . . . . .
static vec_t vec_sh[] = { { -1, -1, 2 }, { -1, 1, 2 }, { 1, -1, 2 }, { 1, 1, 2 }, { 0, 0 }, };
// Mao（マ）
// . . . . . . .
// . . M . M . .
// . M . m . M .
// . . m # m . .
// . M . m . M .
// . . M . M . .
// . . . . . . .
static vec_t vec_ma[] = { { -1, 0, 1 }, { 0, 1, 1 }, { 1, 0, 1 }, { 0, -1, 1 }, { 0, 0 }, };
// Moa（モ）
// . . . . . . .
// . . M . M . .
// . M m . m M .
// . . . # . . .
// . M m . m M .
// . . M . M . .
// . . . . . . .
static vec_t vec_mo[] = { { -1, -1, 1 }, { -1, 1, 1 }, { 1, 1, 1 }, { 1, -1, 1 }, { 0, 0 }, };
// 獅子  . . . . . . .
//       . 2 2 2 2 2 .
//       . 2 2 2 2 2 .
//       . 2 2 # 2 2 .
//       . 2 2 2 2 2 .
//       . 2 2 2 2 2 .
//       . . . . . . .
static vec_t vec_ar2[] = {
	{ -2, -2 }, { -2, -1 }, { -2, 0 }, { -2, 1 }, { -2, 2 },
	{ -1, -2 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, { -1, 2 },
	{ 0, -2 }, { 0, -1 }, { 0, 1 }, { 0, 2 },
	{ 1, -2 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 2, -2 }, { 2, -1 }, { 2, 0 }, { 2, 1 }, { 2, 2 },
	{ 0, 0 },
};
// 酔象  . . . . . | . . . . .
// 鷹    . L L L . | . L . L .
//       . L # L . | . L # L .
//       . L . L . | . L L L .
//       . . . . . | . . . . .
static vec_t hi_vec_sz[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
static vec_t yo_vec_sz[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// 麒麟  . . . . . . .
//       . . . L . . .
//       . . L . L . .
//       . L . # . L .
//       . . L . L . .
//       . . . L . . .
//       . . . . . . .
static vec_t vec_kr[] = {
	{ -2, 0 }, { -1, -1 }, { -1, 1 }, { 0, -2 }, { 0, 2 }, { 1, -1 }, { 1, 1 }, { 2, 0 },
	{ 0, 0 },
};
// 鳳凰  . . . . . . .
//       . L . . . L .
//       . . . L . . .
//       . . L # L . .
//       . . . L . . .
//       . L . . . L .
//       . . . . . . .
static vec_t vec_ho[] = {
	{ -2, -2 }, { -2, 2 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 2, -2 }, { 2, 2 },
	{ 0, 0 },
};
// 盲虎  . . . . . | . . . . .
//       . L . L . | . L L L .
//       . L # L . | . L # L .
//       . L L L . | . L . L .
//       . . . . . | . . . . .
static vec_t hi_vec_mk[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
static vec_t yo_vec_mk[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// 前3後 . . . . . | . . . . .
// 銅将  . * * * . | . . * . .
// 白駒  . . # . . | . . # . .
//       . . * . . | . * * * .
//       . . . . . | . . . . .
static vec_t hi_vec_f3b[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
static vec_t yo_vec_f3b[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
// 前後3 . . . . . | . . . . .
// 鯨鯢  . . * . . | . * * * .
//       . . # . . | . . # . .
//       . * * * . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fb3[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_fb3[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// 前3後3. . . . .
// 猛豹  . * * * .
// 飛牛  . . # . .
// 鶴    . * * * .
//       . . . . .
static vec_t vec_f3b3[] = {
	{ -1, -1 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 1, },
	{ 0, 0 },
};
// 前後  . . . . .
// 仲人  . . * . .
// 反車  . . # . .
//       . . * . .
//       . . . . .
static vec_t vec_fb[] = { { 0, -1 }, { 0, 1 }, { 0, 0 }, };
// 左右  . . . . .
// 竪行  . . . . .
//       . * # * .
//       . . . . .
//       . . . . .
static vec_t vec_lr[] = { { -1, 0 }, { 1, 0 }, { 0, 0 }, };
// 側面  . . . . .
// 飛鹿  . * . * .
// 奔猪  . * # * .
//       . * . * .
//       . . . . .
static vec_t vec_l3r3[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// 飛鷲  . . . . . . . | . . . . . . . | . . . R . . . | R . . R . . R
//       . 2 . . . 2 . | . . . . . . . | . . . R . . . | . R . R . R .
//       . . 2 . 2 . . | . . . . . . . | . . . R . . . | . . R R R . .
//       . . . # . . . | . . . # . . . | R R R # R R R | R R R # R R R
//       . . . . . . . | . . 2 . 2 . . | . . R R R . . | . . . R . . .
//       . . . . . . . | . 2 . . . 2 . | . R . R . R . | . . . R . . .
//       . . . . . . . | . . . . . . . | R . . R . . R | . . . R . . .
static vec_t hi_leap_vec_hj[] = { { -2, -2 }, { -1, -1 }, { 1, -1 }, { 2, -2 }, { 0, 0 }, };
static vec_t yo_leap_vec_hj[] = { { -2, 2 }, { -1, 1 }, { 1, 1 }, { 2, 2 }, { 0, 0 }, };
static vec_t hi_run_vec_hj[] = {
	{ -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
static vec_t yo_run_vec_hj[] = {
	{ -1, -1 }, { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 },
	{ 0, 0 },
};
// 角鷹  . . . . . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_tk[] = { { 0, -2 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_tk[] = { { 0, 1 }, { 0, 2 }, { 0, 0 }, };
// 雉    . . L . . | . . . . .
//       . . . . . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . . . . .
//       . . . . . | . . L . .
static vec_t hi_vec_tkj[] = { { -1, 1 }, { 0, -2 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_tkj[] = { { -1, -1 }, { 0, 2 }, { 1, -1 }, { 0, 0 }, };
// 享    . . R . . | R . . . .
// 左鶉  . . R . . | . R . L .
//       . . # . . | . . # . .
//       . L . R . | . . R . .
//       . . . . R | . . R . .
static vec_t hi_vec_leap_tlu[] = { { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tlu[] = { { -1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tlu[] = { { -1, 1 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_run_tlu[] = { { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// 鳥    . . R . . | . . . . R
// 右鶉  . . R . . | . L . R .
//       . . # . . | . . # . .
//       . R . L . | . . R . .
//       R . . . . | . . R . .
static vec_t hi_vec_leap_tru[] = { { -1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tru[] = { { 1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tru[] = { { 0, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_run_tru[] = { { -1, -1 }, { 0, 1 }, { 0, 0 }, };
// 鵰    R . . . . . R | . . . R . . .
//       . R . . . R . | . R . R . R .
//       . . R L R . . | . . R R R . .
//       . . L # L . . | . . L # L . .
//       . . R R R . . | . . R L R . .
//       . R . R . R . | . R . . . R .
//       . . . R . . . | R . . . . . R
static vec_t hi_vec_leap_tkt[] = { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 0 }, };
static vec_t yo_vec_leap_tkt[] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, };
static vec_t hi_vec_run_tkt[] = {
	{ -1, -1 }, { -1, 1, 2 }, { 0, 1 }, { 1, -1 }, { 1, 1, 2 },
	{ 0, 0 },
};
static vec_t yo_vec_run_tkt[] = {
	{ -1, -1, 2 }, { -1, 1 }, { 0, -1 }, { 1, -1, 2 }, { 1, 1 },
	{ 0, 0 },
};
// 鴈    L . . . L | . . L . .
//       . . . . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . . . .
//       . . L . . | L . . . L
static vec_t hi_vec_tkr[] = { { -2, -2 }, { 0, 2 }, { 2, -2 }, { 0, 0 }, };
static vec_t yo_vec_tkr[] = { { -2, 2 }, { 0, -2 }, { 2, 2 }, { 0, 0 }, };
// 飛龍  . . . . . . .
//       . R . . . R .
//       . . R . R . .
//       . . . # . . .
//       . . R . R . .
//       . R . . . R .
//       . . . . . . .
static vec_t vec_run_dhr[] = {
	{ -1, -1, 2 }, { -1, 1, 2 }, { 1, -1, 2 }, { 1, 1, 2 },
	{ 0, 0 },
};
// 猛牛  . . . . . . .
//       . . . R . . .
//       . . . R . . .
//       . R R # R R .
//       . . . R . . .
//       . . . R . . .
//       . . . . . . .
static vec_t vec_run_dmg[] = {
	{ -1, 0, 2 }, { 0, -1, 2 }, { 0, 1, 2 }, { 1, 0, 2 },
	{ 0, 0 },
};

// ------
//  駒面
// ------
// 将棋族
static char *graphv_fu[] = { "歩", "FU", NULL, };
static char *graphv_ky[] = { "香", "KY", NULL, };
static char *graphv_ke[] = { "桂", "KE", NULL, };
static char *graphv_gi[] = { "銀", "GI", NULL, };
static char *graphv_ki[] = { "金", "KI", NULL, };
static char *graphv_ka[] = { "角", "KA", NULL, };
static char *graphv_hi[] = { "飛", "HI", NULL, };
static char *graphv_ou[] = { "王", "OU", "玉", "GY", NULL, };
static char *graphv_to[] = { "と", "TO", NULL, };
static char *graphv_ny[] = { "杏", "NY", NULL, };
static char *graphv_nk[] = { "圭", "NK", NULL, };
static char *graphv_ng[] = { "全", "NG", NULL, };
static char *graphv_um[] = { "馬", "UM", NULL, };
static char *graphv_ry[] = { "龍", "RY", NULL, };
// 特殊
static char *graphv_room[] = { "・", "  ", "　", NULL, };
static char *graphv_rock[] = { "●", "IS", "石", NULL, };
static char *graphv_pit[] = { "◆", "PT", "穽", NULL, };
// フェアリーチェス - リーパー族
static char *graphv_ze[] = { "零", "ZE", NULL, };	// (0, 0)-Leaper Zero
static char *graphv_wa[] = { "臣", "WA", NULL, };	// (0, 1)-Leaper Wazir
static char *graphv_da[] = { "戦", "DA", NULL, };	// (0, 2)-Leaper Dabbaba
static char *graphv_tl[] = { "参", "TL", NULL, };	// (0, 3)-Leaper Threeleaper
static char *graphv_fe[] = { "賢", "FE", NULL, };	// (1, 1)-Leaper Fers
static char *graphv_kn[] = { "騎", "KN", NULL, };	// (1, 2)-Leaper Knight
static char *graphv_cm[] = { "駱", "CM", NULL, };	// (1, 3)-Leaper Camel
static char *graphv_gr[] = { "き", "GR", NULL, };	// (1, 4)-Leaper Giraffe
static char *graphv_fl[] = { "紅", "FL", NULL, };	// (1, 6)-Leaper Flamingo
static char *graphv_al[] = { "ぞ", "AL", NULL, };	// (2, 2)-Leaper Alfil
static char *graphv_zb[] = { "縞", "ZB", NULL, };	// (2, 3)-Leaper Zebra
static char *graphv_tr[] = { "桟", "TR", NULL, };	// (3, 3)-Leaper Tripper
static char *graphv_l5[] = { "伍", "L5", NULL, };	// (1, 5)+(3, 4)-Leaper 5-Leaper 
static char *graphv_r4[] = { "栖", "R4", NULL, };	// (2, 6)-Leaper Root-40-Leaper 
static char *graphv_r5[] = { "梧", "R5", NULL, };	// (1, 7)+(5, 5)-Leaper Root-50-Leaper
static char *graphv_r6[] = { "楼", "R6", NULL, };	// (1, 8)+(4, 7)-Leaper Root-65-Leaper
// フェアリーチェス - ライダー族
static char *graphv_qu[] = { "Ｑ", "QU", NULL, };	// (0, 1)+(1, 1)-Rider Queen
static char *graphv_nr[] = { "夜", "NR", NULL, };	// (1, 2)-Rider Nightrider
static char *graphv_cr[] = { "駱", "CR", NULL, };	// (1, 3)-Rider Camelrider
static char *graphv_zr[] = { "縞", "ZR", NULL, };	// (2, 3)-Rider Zebrarider
static char *graphv_rs[] = { "薔", "RS", NULL, };	// (1, 2)-CircleRider Rose
// フェアリーチェス - 複合（リーパー、ライダー）族
static char *graphv_em[] = { "后", "EM", NULL, };	// (0, 1)-Rider+(1, 2)-Leaper Empress
static char *graphv_pr[] = { "姫", "PR", NULL, };	// (1, 1)-Rider+(1, 2)-Leaper Princess
// フェアリーチェス - ホッパー族
static char *graphv_gh[] = { "Ｇ", "GH", NULL, };	// Grasshopper
static char *graphv_lo[] = { "蝗", "LO", NULL, };	// Locust
static char *graphv_li[] = { "鬣", "LI", NULL, };	// Lion
static char *graphv_ro[] = { "考", "RO", NULL, };	// Kangaroo
static char *graphv_ne[] = { "Ｅ", "NE", "均", NULL, };	// Non-stop equihopper
static char *graphv_eh[] = { "Ｅ", "EH", "等", NULL, };	// Equihopper
static char *graphv_sp[] = { "雀", "SP", NULL, };	// Sparrow
static char *graphv_ea[] = { "鷲", "EA", NULL, };	// Eagle
// フェアリーチェス - 象棋族
static char *graphv_pa[] = { "包", "PA", NULL, };	// 象棋：包・炮
static char *graphv_va[] = { "包", "VA", NULL, };
static char *graphv_le[] = { "包", "LE", NULL, };
static char *graphv_sh[] = { "相", "SH", NULL, };	// 象棋：相・象
static char *graphv_ma[] = { "マ", "MA", NULL, };	// 象棋：馬
static char *graphv_mo[] = { "モ", "MO", NULL, };	// 象棋：馬を45度回転
// 中将棋族
static char *graphv_ss[] = { "獅", "SS", NULL, };
static char *graphv_sz[] = { "象", "SZ", NULL, };	// ->太
static char *graphv_kr[] = { "麒", "KR", NULL, };	// ->師（獅）
static char *graphv_ho[] = { "鳳", "HO", NULL, };	// ->本（奔）
static char *graphv_mk[] = { "虎", "MK", NULL, };	// ->鹿
static char *graphv_do[] = { "銅", "DO", NULL, };	// ->黄（横）
static char *graphv_mh[] = { "豹", "MH", NULL, };	// ->ク（角）
static char *graphv_cn[] = { "仲", "CN", NULL, };	// ->豕（象）
static char *graphv_hn[] = { "奔", "HN", NULL, };
static char *graphv_jg[] = { "竪", "JG", NULL, };	// ->牛
static char *graphv_og[] = { "横", "OG", NULL, };	// ->猪
static char *graphv_he[] = { "反", "HE", NULL, };	// ->鯨
static char *graphv_ts[] = { "太", "TS", NULL, };
static char *graphv_hr[] = { "鹿", "HR", NULL, };	// <-虎
static char *graphv_hj[] = { "鷲", "HJ", NULL, };	// <-中将棋：龍
static char *graphv_tk[] = { "鷹", "TK", NULL, };	// <-中将棋：馬
static char *graphv_hg[] = { "牛", "HG", NULL, };	// <-竪
static char *graphv_hc[] = { "猪", "HC", NULL, };	// <-横
static char *graphv_kg[] = { "鯨", "KG", NULL, };	// <-反
static char *graphv_hk[] = { "駒", "HK", NULL, };	// <-中将棋：香
static char *graphv_chi[] = { "飛", "CHI", NULL, };	// ->リ（龍）
static char *graphv_cka[] = { "角", "CKA", NULL, };	// ->マ（馬）
static char *graphv_cki[] = { "金", "CKI", NULL, };	// ->ヒ（飛）
static char *graphv_cgi[] = { "銀", "CGI", NULL, };	// ->立（竪）
static char *graphv_cry[] = { "龍", "CRY", NULL, };	// ->鷲
static char *graphv_cum[] = { "馬", "CUM", NULL, };	// ->鷹
static char *graphv_cky[] = { "香", "CKY", NULL, };	// ->駒
static char *graphv_pss[] = { "師", "PSS", NULL, };	// <-麒
static char *graphv_phn[] = { "本", "PHO", NULL, };	// <-鳳
static char *graphv_pry[] = { "リ", "PRY", NULL, };	// <-中将棋：飛
static char *graphv_pum[] = { "マ", "PUM", NULL, };	// <-中将棋：角
static char *graphv_phi[] = { "ヒ", "PHI", NULL, };	// <-中将棋：金
static char *graphv_pka[] = { "ク", "PKA", NULL, };	// <-豹
static char *graphv_pjg[] = { "立", "PJG", NULL, };	// <-中将棋：銀
static char *graphv_pog[] = { "黄", "POG", NULL, };	// <-銅
static char *graphv_psz[] = { "豕", "PSZ", NULL, };	// <-仲
// 禽将棋族
static char *graphv_tho[] = { "鵬", "THO", NULL, };	// 鵬
static char *graphv_ttr[] = { "鶴", "TTR", NULL, };	// 鶴
static char *graphv_tkj[] = { "雉", "TKJ", NULL, };	// 雉
static char *graphv_tlu[] = { "享", "TLU", NULL, };	// 左鶉
static char *graphv_tru[] = { "鳥", "TRU", NULL, };	// 右鶉
static char *graphv_ttk[] = { "鷹", "TTK", NULL, };	// 鷹
static char *graphv_tkt[] = { "鵰", "TKT", NULL, };	// 鵰
static char *graphv_ttb[] = { "燕", "TTB", NULL, };	// 燕
static char *graphv_tkr[] = { "鴈", "TKR", NULL, };	// 鴈
// 大将棋族
static char *graphv_dhr[] = { "辰", "DHR", NULL, };	// 飛龍 -> 金将（成金）
static char *graphv_dmg[] = { "丑", "DMG", NULL, };	// 猛牛 -> 金将（成金）
static char *graphv_dsc[] = { "亥", "DSC", NULL, };	// 嗔猪 -> 金将（成金）
static char *graphv_dmj[] = { "猫", "DMJ", NULL, };	// 猫刃 -> 金将（成金）
static char *graphv_dnk[] = { "成", "DNK", NULL, };	// 金将（成金） <- 飛龍、猛牛、嗔猪、猫刃
// 摩訶大大将棋族
static char *graphv_dbj[] = { "蛇", "DBJ", NULL, };	// 蟠蛇（バンジャ）
// 拡張フェアリー駒
static char *graphv_f1[] = { "①", "F1", NULL, };	// 拡張フェアリー駒１
static char *graphv_f2[] = { "②", "F2", NULL, };	// 拡張フェアリー駒２
static char *graphv_f3[] = { "③", "F3", NULL, };	// 拡張フェアリー駒３
static char *graphv_f4[] = { "④", "F4", NULL, };	// 拡張フェアリー駒４
static char *graphv_f5[] = { "⑤", "F5", NULL, };	// 拡張フェアリー駒５
static char *graphv_f6[] = { "⑥", "F6", NULL, };	// 拡張フェアリー駒６
static char *graphv_f7[] = { "⑦", "F7", NULL, };	// 拡張フェアリー駒７
// Pao, Vao, Leo のデフォールト表記を「包」とする。
// 拡張表記として、「砲」「炮」を用意する。
// Pao, Vao, Leo が同時に２つ以上指定されると、順次「砲」「炮」を割り当てる。
char graph_pa1[] = "包";
char graph_pa2[] = "砲";
char graph_pa3[] = "炮";
char hi_b_graph_pa2[] = " 砲";
char yo_b_graph_pa2[] = "v砲";
char hi_b_graph_pa3[] = " 炮";
char yo_b_graph_pa3[] = "v炮";

// ------------
//  実際駒情報
// ------------
piece_t piece[PC_END] = {
	{	// 0x00 空
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x01 歩兵
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x02 香車
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x03 桂馬
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x04 銀将
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x05 金将
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x06 角行
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x07 飛車
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x08 フェアリー駒１
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x09 フェアリー駒２
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0a フェアリー駒３
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0b フェアリー駒４
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0c フェアリー駒５
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0d フェアリー駒６
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0e フェアリー駒７
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0f 王将
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x10 穽
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x11 と金
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x12 成香
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x13 成桂
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x14 成銀
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x15 石
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x16 龍馬
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x17 龍王
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x18 成フェアリー駒１（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x19 成フェアリー駒２（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1a 成フェアリー駒３（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1b 成フェアリー駒４（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1c 成フェアリー駒５（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1d 成フェアリー駒６（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1e 成フェアリー駒７（現在未サポート）
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1f
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
};

// ------------
//  基本駒情報
// ------------
piece_info_t piece_info[FP_EXEND] = {
	{	// 0
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 1
		"歩兵", graphv_fu, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 18, 
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_2F | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 2
		"香車", graphv_ky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_RUN),
	},
	{	// 3
		"桂馬", graphv_ke, hi_vec_ke, yo_vec_ke, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 4
		"銀将", graphv_gi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 5
		"金将", graphv_ki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 6
		"角行", graphv_ka, vec_nul, vec_nul, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 7
		"飛車", graphv_hi, vec_nul, vec_nul, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 8 
		"王将", graphv_ou, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 9
		"と金", graphv_to, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 18,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 10
		"成香", graphv_ny, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 11
		"成桂", graphv_nk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 12
		"成銀", graphv_ng, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 13
		"龍馬", graphv_um, vec_hi, vec_hi, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 14
		"龍王", graphv_ry, vec_ka, vec_ka, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 15
		"空", graphv_room, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 16
		"石", graphv_rock, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 17
		"穽", graphv_pit, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 18
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 19
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 20
		"Zero {(0, 0)-Leaper} ", graphv_ze, vec_nul, vec_nul, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 21
		"Knight {(1, 2)-Leaper} ", graphv_kn, vec_kn, vec_kn, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 22
		"Camel {(1, 3)-Leaper} ", graphv_cm, vec_cm, vec_cm, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 23
		"Giraffe {(1, 4)-Leaper} ", graphv_gr, vec_gr, vec_gr, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 24
		"Five-leaper {(0, 5)+(3, 4)-Leaper} ", graphv_l5, vec_l5, vec_l5, vec_nul, vec_nul, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_LONG_LEAP),
	},
	{	// 25
		"Flamingo {(1, 6)-Leaper} ", graphv_fl, vec_fl, vec_fl, vec_nul, vec_nul, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_LONG_LEAP),
	},
	{	// 26
		"Root-50-leaper {(1, 7)+(5, 5)-Leaper} ", graphv_r5, vec_r5, vec_r5, vec_nul, vec_nul, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_LONG_LEAP),
	},
	{	// 27
		"Zebra {(2, 3)-Leaper} ", graphv_zb, vec_zb, vec_zb, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 28
		"Root-40-leaper {(2, 6)-Leaper}", graphv_r4, vec_r4, vec_r4, vec_nul, vec_nul, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_LONG_LEAP),
	},
	{	// 29
		"Wazir {(0, 1)-Leaper}", graphv_wa, vec_hi, vec_hi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 30
		"Dabbaba {(0, 2)-Leaper}", graphv_da, vec_da, vec_da, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 31
		"Threeleaper {(0, 3)-Leaper}", graphv_tl, vec_tl, vec_tl, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 32
		"Fers {(1, 1)-Leaper}", graphv_fe, vec_ka, vec_ka, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 33
		"Alfil {(2, 2)-Leaper}", graphv_al, vec_al, vec_al, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 34
		"Tripper {(3, 3)-Leaper}", graphv_tr, vec_tr, vec_tr, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 35
		"Root-65-leaper {(1, 8)+(4, 7)-Leaper} ", graphv_r6, vec_r6, vec_r6, vec_nul, vec_nul, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_LONG_LEAP),
	},
	{	// 36
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 37
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 38
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 39
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 40
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 41
		"Queen ", graphv_qu, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 42
		"Nightrider {(1, 2)-Rider} ", graphv_nr, vec_nul, vec_nul, vec_kn, vec_kn, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 43
		"Camelrider {(1, 3)-Rider} ", graphv_cr, vec_nul, vec_nul, vec_cm, vec_cm, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 44
		"Zebrarider {(2, 3)-Rider} ", graphv_zr, vec_nul, vec_nul, vec_zb, vec_zb, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 45
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 46
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 47
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 48
		"Rose {(1, 2)-CircleRider} ", graphv_rs, vec_nul, vec_nul, vec_kn, vec_kn, 2,
		B_PA_PIECE, B_MS_CIRCLE_RUN,
	},
	{	// 49
		"Princess {(1, 1)-Rider+{1, 2}-Leaper}", graphv_pr, vec_kn, vec_kn, vec_ka, vec_ka, 2,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 50
		"Empress {(0, 1)-Rider+{1, 2}-Leaper}", graphv_em, vec_kn, vec_kn, vec_hi, vec_hi, 2,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 51
		"Grasshopper ", graphv_gh, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_HOP,
	},
	{	// 52
		"Locust ", graphv_lo, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_DECAPT_HOP,
	},
	{	// 53
		"Lion ", graphv_li, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_JUMP,
	},
	{	// 54
		"Kangaroo", graphv_ro, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_JUMP_HOP,
	},
	{	// 55
		"Non-stop Equihopper", graphv_ne, vec_nul, vec_nul, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_NS_EQUI_JUMP,
	},
	{	// 56
		"Equihopper", graphv_eh, vec_nul, vec_nul, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_EQUI_JUMP,
	},
	{	// 57
		"Sparrow", graphv_sp, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_REFL135,
	},
	{	// 58
		"Eagle", graphv_ea, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_REFL90,
	},
	{	// 59
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 60
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 61
		"Pao（象棋の包）", graphv_pa, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 62
		"Vao（角利きの包）", graphv_va, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 63
		"Leo（Ｑ利きの包）", graphv_le, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 64
		"Shan（象棋の相）", graphv_sh, vec_sh, vec_sh, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_STEP,
	},
	{	// 65
		"Mao（象棋の馬）", graphv_ma, vec_ma, vec_ma, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_MAO_STEP,
	},
	{	// 66
		"Moa（象棋の馬を45度回転）", graphv_mo, vec_mo, vec_mo, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_MAO_STEP,
	},
	{	// 67
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 68
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 69
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 70
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 71
		"獅子（中将棋）", graphv_ss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 72
		"酔象（中将棋）", graphv_sz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 73
		"麒麟（中将棋）", graphv_kr, vec_kr, vec_kr, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 74
		"鳳凰（中将棋）", graphv_ho, vec_ho, vec_ho, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 75
		"盲虎（中将棋）", graphv_mk, hi_vec_mk, yo_vec_mk, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 76
		"銅将（中将棋）", graphv_do, hi_vec_f3b, yo_vec_f3b, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 77
		"猛豹（中将棋）", graphv_mh, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 78
		"仲人（中将棋）", graphv_cn, vec_fb, vec_fb, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 79
		"奔王（中将棋）", graphv_hn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 80
		"竪行（中将棋）", graphv_jg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 81
		"横行（中将棋）", graphv_og, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 82
		"反車（中将棋）", graphv_he, vec_nul, vec_nul, vec_fb, vec_fb, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 83
		"太子（中将棋、酔象の成駒）", graphv_ts, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 84
		"飛鹿（中将棋、盲虎の成駒）", graphv_hr, vec_l3r3, vec_l3r3, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 85
		"飛鷲（中将棋、龍王の成駒）", graphv_hj, hi_leap_vec_hj, yo_leap_vec_hj,
		hi_run_vec_hj, yo_run_vec_hj, 4, B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 86
		"角鷹（中将棋、龍馬の成駒）", graphv_tk, hi_vec_tk, yo_vec_tk, hi_vec_mk, yo_vec_mk, 4,
		B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 87
		"飛牛（中将棋、竪行の成駒）", graphv_hg, vec_nul, vec_nul, vec_f3b3, vec_f3b3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 88
		"奔猪（中将棋、横行の成駒）", graphv_hc, vec_nul, vec_nul, vec_l3r3, vec_l3r3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 89
		"鯨鯢（中将棋、反車の成駒）", graphv_kg, vec_nul, vec_nul, hi_vec_fb3, yo_vec_fb3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 90
		"白駒（中将棋、香車の成駒）", graphv_hk, vec_nul, vec_nul, hi_vec_f3b, yo_vec_f3b, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 91
		"飛車（中将棋）", graphv_chi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 92
		"角行（中将棋）", graphv_cka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 93
		"金将（中将棋）", graphv_cki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 94
		"銀将（中将棋）", graphv_cgi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 95
		"龍王（中将棋）", graphv_cry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 96
		"龍馬（中将棋）", graphv_cum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 97
		"香車（中将棋）", graphv_cky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 98
		"獅子（中将棋、麒麟の成駒）", graphv_pss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 99
		"奔王（中将棋、鳳凰の成駒）", graphv_phn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 100
		"龍王（中将棋、飛車の成駒）", graphv_pry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 101
		"龍馬（中将棋、角行の成駒）", graphv_pum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 102
		"飛車（中将棋、金将の成駒）", graphv_phi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 103
		"角行（中将棋、猛豹の成駒）", graphv_pka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 104
		"竪行（中将棋、銀将の成駒）", graphv_pjg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 105
		"横行（中将棋、銅将の成駒）", graphv_pog, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 106
		"酔象（中将棋、仲人の成駒）", graphv_psz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 107
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 108
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 109
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 110
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 111
		"鵬（禽将棋の王）", graphv_tho, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 112
		"鶴（禽将棋）", graphv_ttr, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 113
		"雉（禽将棋）", graphv_tkj, hi_vec_tkj, yo_vec_tkj, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 114
		"享（禽将棋、左鶉）",
		graphv_tlu, hi_vec_leap_tlu, yo_vec_leap_tlu, hi_vec_run_tlu, yo_vec_run_tlu, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 115
		"鳥（禽将棋、右鶉）",
		graphv_tru, hi_vec_leap_tru, yo_vec_leap_tru, hi_vec_run_tru, yo_vec_run_tru, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 116
		"鷹（禽将棋）", graphv_ttk, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 117
		"鵰（禽将棋、鷹の成駒）",
		graphv_tkt, hi_vec_leap_tkt, yo_vec_leap_tkt, hi_vec_run_tkt, yo_vec_run_tkt, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 118
		"燕（禽将棋）", graphv_ttb, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_3T | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 119
		"鴈（禽将棋、燕の成駒）", graphv_tkr, hi_vec_tkr, yo_vec_tkr, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 120
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 121
		"飛龍（大将棋）", graphv_dhr, vec_nul, vec_nul, vec_run_dhr, vec_run_dhr, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 122
		"猛牛（大将棋）", graphv_dmg, vec_nul, vec_nul, vec_run_dmg, vec_run_dmg, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 123
		"嗔猪（大将棋）", graphv_dsc, vec_hi, vec_hi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 124
		"猫刃（大将棋）", graphv_dmj, vec_ka, vec_ka, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 125
		"金将（大将棋の成金）", graphv_dnk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 126
		"蟠蛇（摩訶大大将棋）", graphv_dbj, hi_vec_fb3, yo_vec_fb3, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 127
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 128
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 129
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 130
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 131 拡張フェアリー駒１
		"拡張フェアリー駒１", graphv_f1, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 132 拡張フェアリー駒２
		"拡張フェアリー駒２", graphv_f2, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 133 拡張フェアリー駒３
		"拡張フェアリー駒３", graphv_f3, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 134 拡張フェアリー駒４
		"拡張フェアリー駒４", graphv_f4, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 135 拡張フェアリー駒５
		"拡張フェアリー駒５", graphv_f5, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 136 拡張フェアリー駒６
		"拡張フェアリー駒６", graphv_f6, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 137 拡張フェアリー駒７
		"拡張フェアリー駒７", graphv_f7, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
};

// ----------
//  成駒情報
// ----------
int fp_promoted[FP_EXEND] = {
	-1,		//  0: -
	 9,		//  1: 歩兵 -> と（と金）
	10,		//  2: 香車 -> 杏（成香）
	11,		//  3: 桂馬 -> 圭（成桂）
	12,		//  4: 銀将 -> 全（成銀）
	-1,		//  5: 金将
	13,		//  6: 角行 -> 馬（龍馬）
	14,		//  7: 飛車 -> 龍（龍王）
	-1,		//  8: 王/玉
	-1,		//  9: と金
	-1,		// 10: 成香
	-1,		// 11: 成桂
	-1,		// 12: 成銀
	-1,		// 13: 龍馬
	-1,		// 14: 龍王
	-1,		// 15: -
	-1,		// 16: -
	-1,		// 17: -
	-1,		// 18: -
	-1,		// 19: -
	-1,		// 20: Zero (0, 0)-Leaper
	-1,		// 21: Knight (1, 2)-Leaper
	-1,		// 22: Camel (1, 3)-Leaper
	-1,		// 23: Giraffe (1, 4)-Leaper
	-1,		// 24: Five-Leaper (0, 5)+(3, 4)-Leaper
	-1,		// 25: Flamingo (1, 6)-Leaper
	-1,		// 26: Root-50-Leaper (1, 7)+(5, 5)-Leaper
	-1,		// 27: Zebra (2, 3)-Leaper
	-1,		// 28: Root-40-Leaper (2, 6)-Leaper
	-1,		// 29: Wazir (0-1)-Leaper
	-1,		// 30: Dabbaba (0-2)-Leaper
	-1,		// 31: Threeleaper (0-3)-Leaper
	-1,		// 32: Fers (1-1)-Leaper
	-1,		// 33: Alfil (2-2)-Leaper
	-1,		// 34: Tripper (3-3)-Leaper
	-1,		// 35: Root-65-Leaper (1, 8)+(4, 7)-Leaper
	-1,		// 36: -
	-1,		// 37: -
	-1,		// 38: -
	-1,		// 39: -
	-1,		// 40: -
	-1,		// 41: Queen (0, 1)+(1, 1)-Rider
	-1,		// 42: Nightrider (1, 2)-Rider
	-1,		// 43: Camelrider (1, 3)-Rider
	-1,		// 44: Zebrarider (2, 3)-Rider
	-1,		// 45: -
	-1,		// 46: -
	-1,		// 47: -
	-1,		// 48: Rose (1, 2)-CircleRider
	-1,		// 49: Princess (1, 1)-Rider+(1, 2)-Leaper
	-1,		// 50: Empress (0, 1)-Rider+(1, 2)-Leaper
	-1,		// 51: Grasshopper
	-1,		// 52: Locust
	-1,		// 53: Lion
	-1,		// 54: Kangaroo
	-1,		// 55: Non-stop Equihopper
	-1,		// 56: Equihopper
	-1,		// 57: Sparrow
	-1,		// 58: Eagle
	-1,		// 59: -
	-1,		// 60: -
	-1,		// 61: Pao（象棋：包）
	-1,		// 62: Vao
	-1,		// 63: Leo
	-1,		// 64: Shan（象棋：相）
	-1,		// 65: Mao（象棋：馬）
	-1,		// 66: Moa（象棋：馬を45度回転）
	-1,		// 67: -
	-1,		// 68: -
	-1,		// 69: -
	-1,		// 70: -
	-1,		// 71: 獅子
	83,		// 72: 酔象 -> 太子
	98,		// 73: 麒麟 -> 師（成獅子）
	99,		// 74: 鳳凰 -> 本（成奔王）
	84,		// 75: 盲虎 -> 飛鹿
	105,	// 76: 銅将 -> 横行（黄）
	103,	// 77: 猛豹 -> ク（成角行）
	106,	// 78: 仲人 -> 豕（成酔象）
	-1,		// 79: 奔王
	87,		// 80: 竪行 -> 飛牛
	88,		// 81: 横行 -> 奔猪
	89,		// 82: 反車 -> 鯨鯢
	-1,		// 83: 太子
	-1,		// 84: 飛鹿
	-1,		// 85: 飛鷲
	-1,		// 86: 角鷹
	-1,		// 87: 飛牛
	-1,		// 88: 奔猪
	-1,		// 89: 鯨鯢
	-1,		// 90: 白駒
	100,	// 91: 中将棋：飛 -> リ（成龍王）
	101,	// 92: 中将棋：角 -> マ（成龍馬）
	102,	// 93: 中将棋：金 -> ヒ（成飛車）
	104,	// 94: 中将棋：銀 -> 立（成竪行）
	85,		// 95: 中将棋：龍 -> 飛鷲
	86,		// 96: 中将棋：馬 -> 角鷹
	90,		// 97: 中将棋：香 -> 白駒
	-1,		// 98: 師（成獅子）
	-1,		// 99: 本（成奔王）
	-1,		// 100: リ（成龍王）
	-1,		// 101: マ（成龍馬）
	-1,		// 102: ヒ（成飛車）
	-1,		// 103: ク（成角行）
	-1,		// 104: 立（成竪行）
	-1,		// 105: 黄（成横行）
	-1,		// 106: 豕（成酔象）
	-1,		// 107: -
	-1,		// 108: -
	-1,		// 109: -
	-1,		// 110: -
	-1,		// 111: 鵬
	-1,		// 112: 鶴
	-1,		// 113: 雉
	-1,		// 114: 享（左鶉）
	-1,		// 115: 鳥（右鶉）
	117,	// 116: 鷹 -> 鵰
	-1,		// 117: 鵰
	119,	// 118: 燕 -> 鴈
	-1,		// 119: 鴈
	-1,		// 120: -
	125,	// 121: 飛龍 -> 成（金将）
	125,	// 122: 猛牛 -> 成（金将）
	125,	// 123: 嗔猪 -> 成（金将）
	125,	// 124: 猫刃 -> 成（金将）
	-1,		// 125: 成（金将）
	-1,		// 126: 蟠蛇（※本当は奔蛇に成れるのだが、鯨鯢と同じ利きの駒）
	-1,		// 127: -
	-1,		// 128: -
	-1,		// 129: -
	-1,		// 130: -
	-1,		// 131: 拡張フェアリー駒１
	-1,		// 132: 拡張フェアリー駒２
	-1,		// 133: 拡張フェアリー駒３
	-1,		// 134: 拡張フェアリー駒４
	-1,		// 135: 拡張フェアリー駒５
	-1,		// 136: 拡張フェアリー駒６
	-1,		// 137: 拡張フェアリー駒７
};

char *graph[128];
char *b_graph[128];
char *graph_yo_gy;
char *hi_leap_to_pos[PC_END][81];
char *yo_leap_to_pos[PC_END][81];
char **hi_run_to_posv[PC_END][81];
char **yo_run_to_posv[PC_END][81];
char *around_to_pos[81];
uchar_t *hi_to_board[PC_END][81];
uchar_t *yo_to_board[PC_END][81];
uchar_t *hi_close_check_board[81];
uchar_t *yo_close_check_board[81];
uchar_t *hi_remote_check_board[81];
uchar_t *yo_remote_check_board[81];
uchar_t *around_sq[81];
ulong_t fairy_move_style;
uchar_t *hi_valid_sq[PC_END];
uchar_t *yo_valid_sq[PC_END];
int normal_pc_count[RAW_PC_END] = { 0, 18, 4, 4, 4, 4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, };
int pc_count[RAW_PC_END] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
uchar_t hand_pc[RAW_PC_END] = {
	NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, NUL, 
};
uchar_t pos_order[81][81];
uchar_t equi_hop_pc = NUL;
char hi_reborn_pos[RAW_PC_END][9] = {
	{ NOP, },						// 非駒
	{								// 歩
		POS19(1, 7), POS19(2, 7), POS19(3, 7), POS19(4, 7), POS19(5, 7),
		POS19(6, 7), POS19(7, 7), POS19(8, 7), POS19(9, 7),
	},
	{ POS19(1, 9), POS19(9, 9), },	// 香
	{ POS19(2, 9), POS19(8, 9), },	// 桂
	{ POS19(3, 9), POS19(7, 9), },	// 銀
	{ POS19(4, 9), POS19(6, 9), },	// 金
	{ POS19(8, 8), },				// 角
	{ POS19(2, 8), },				// 飛
	{								// フェアリー駒１
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒２
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒３
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒４
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒５
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒６
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// フェアリー駒７
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{ POS19(5, 9), },				// 王
};
char yo_reborn_pos[RAW_PC_END][9] = {
	{ NOP, },						// 非駒
	{								// 歩
		POS19(1, 3), POS19(2, 3), POS19(3, 3), POS19(4, 3), POS19(5, 3),
		POS19(6, 3), POS19(7, 3), POS19(8, 3), POS19(9, 3),
	},
	{ POS19(1, 1), POS19(9, 1), },	// 香
	{ POS19(2, 1), POS19(8, 1), },	// 桂
	{ POS19(3, 1), POS19(7, 1), },	// 銀
	{ POS19(4, 1), POS19(6, 1), },	// 金
	{ POS19(2, 2), },				// 角
	{ POS19(8, 2), },				// 飛
	{								// フェアリー駒１
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒２
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒３
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒４
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒５
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒６
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// フェアリー駒７
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{ POS19(5, 1), },				// 玉
};

