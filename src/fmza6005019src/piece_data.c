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
//  Ú®xNg
// --------------
// Ú®Èµ
static vec_t vec_nul[] = { { 0, 0 }, };
// OÊ  . . . . . | . . . . .
//       . . * . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fw[] = { { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_fw[] = { { 0, 1 }, { 0, 0 }, };
// jn  . . . . . . . | . . . . . . .
//       . . L . L . . | . . . . . . .
//       . . . . . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . . . . .
//       . . . . . . . | . . L . L . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_ke[] = { { -1, -2 }, { 1, -2 }, { 0, 0 }, };
static vec_t yo_vec_ke[] = { { -1, 2 }, { 1, 2 }, { 0, 0 }, };
// â«  . . . . . | . . . . .
//       . L L L . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . L L * .
//       . . . . . | . . . . .
static vec_t hi_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, -1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// à«  . . . . . | . . . . .
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
// ps  . . . . .
//       . * . * .
//       . . # . .
//       . * . * .
//       . . . . .
static vec_t vec_ka[] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// òÔ  . . . . .
//       . . * . .
//       . * # * .
//       . . * . .
//       . . . . .
static vec_t vec_hi[] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, };
// üÍ  . . . . .
// ¤«  . * * * .
// Queen . * # * .
// z¤  . * * * .
// ¾q  . . . . .
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
// Knighti2014-08-05 RosepÉf[^ðEã©çvñèÉÏXj
// . . . . . . .   EEEEEEE
// . . * . * . .   EEFEGEE
// . * . . . * .   EEEEE@E
// . . . # . . .   EEEåKEEE
// . * . . . * .   EDEE. AE
// . . * . * . .   EECEBEE
// . . . . . . .   EEEEEEE
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
// Shanij
// . . . . . . .
// . S . . . S .
// . . S . S . .
// . . . # . . .
// . . S . S . .
// . S . . . S .
// . . . . . . .
static vec_t vec_sh[] = { { -1, -1, 2 }, { -1, 1, 2 }, { 1, -1, 2 }, { 1, 1, 2 }, { 0, 0 }, };
// Maoi}j
// . . . . . . .
// . . M . M . .
// . M . m . M .
// . . m # m . .
// . M . m . M .
// . . M . M . .
// . . . . . . .
static vec_t vec_ma[] = { { -1, 0, 1 }, { 0, 1, 1 }, { 1, 0, 1 }, { 0, -1, 1 }, { 0, 0 }, };
// Moaij
// . . . . . . .
// . . M . M . .
// . M m . m M .
// . . . # . . .
// . M m . m M .
// . . M . M . .
// . . . . . . .
static vec_t vec_mo[] = { { -1, -1, 1 }, { -1, 1, 1 }, { 1, 1, 1 }, { 1, -1, 1 }, { 0, 0 }, };
// q  . . . . . . .
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
// Û  . . . . . | . . . . .
// é    . L L L . | . L . L .
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
// êiÙ  . . . . . . .
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
// P  . . . . . . .
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
// ÓÕ  . . . . . | . . . . .
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
// O3ã . . . . . | . . . . .
// º«  . * * * . | . . * . .
// î  . . # . . | . . # . .
//       . . * . . | . * * * .
//       . . . . . | . . . . .
static vec_t hi_vec_f3b[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
static vec_t yo_vec_f3b[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
// Oã3 . . . . . | . . . . .
// ~éÇ  . . * . . | . * * * .
//       . . # . . | . . # . .
//       . * * * . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fb3[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_fb3[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// O3ã3. . . . .
// Ò^  . * * * .
// ò  . . # . .
// ß    . * * * .
//       . . . . .
static vec_t vec_f3b3[] = {
	{ -1, -1 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 1, },
	{ 0, 0 },
};
// Oã  . . . . .
// l  . . * . .
// ½Ô  . . # . .
//       . . * . .
//       . . . . .
static vec_t vec_fb[] = { { 0, -1 }, { 0, 1 }, { 0, 0 }, };
// ¶E  . . . . .
// Gs  . . . . .
//       . * # * .
//       . . . . .
//       . . . . .
static vec_t vec_lr[] = { { -1, 0 }, { 1, 0 }, { 0, 0 }, };
// ¤Ê  . . . . .
// ò­  . * . * .
// z  . * # * .
//       . * . * .
//       . . . . .
static vec_t vec_l3r3[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// òh  . . . . . . . | . . . . . . . | . . . R . . . | R . . R . . R
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
// pé  . . . . . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_tk[] = { { 0, -2 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_tk[] = { { 0, 1 }, { 0, 2 }, { 0, 0 }, };
// è³    . . L . . | . . . . .
//       . . . . . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . . . . .
//       . . . . . | . . L . .
static vec_t hi_vec_tkj[] = { { -1, 1 }, { 0, -2 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_tkj[] = { { -1, -1 }, { 0, 2 }, { 1, -1 }, { 0, 0 }, };
//     . . R . . | R . . . .
// ¶êG  . . R . . | . R . L .
//       . . # . . | . . # . .
//       . L . R . | . . R . .
//       . . . . R | . . R . .
static vec_t hi_vec_leap_tlu[] = { { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tlu[] = { { -1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tlu[] = { { -1, 1 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_run_tlu[] = { { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// ¹    . . R . . | . . . . R
// EêG  . . R . . | . L . R .
//       . . # . . | . . # . .
//       . R . L . | . . R . .
//       R . . . . | . . R . .
static vec_t hi_vec_leap_tru[] = { { -1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tru[] = { { 1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tru[] = { { 0, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_run_tru[] = { { -1, -1 }, { 0, 1 }, { 0, 0 }, };
// üG    R . . . . . R | . . . R . . .
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
// éì    L . . . L | . . L . .
//       . . . . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . . . .
//       . . L . . | L . . . L
static vec_t hi_vec_tkr[] = { { -2, -2 }, { 0, 2 }, { 2, -2 }, { 0, 0 }, };
static vec_t yo_vec_tkr[] = { { -2, 2 }, { 0, -2 }, { 2, 2 }, { 0, 0 }, };
// ò´  . . . . . . .
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
// Ò  . . . . . . .
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
//  îÊ
// ------
// «û°
static char *graphv_fu[] = { "à", "FU", NULL, };
static char *graphv_ky[] = { "", "KY", NULL, };
static char *graphv_ke[] = { "j", "KE", NULL, };
static char *graphv_gi[] = { "â", "GI", NULL, };
static char *graphv_ki[] = { "à", "KI", NULL, };
static char *graphv_ka[] = { "p", "KA", NULL, };
static char *graphv_hi[] = { "ò", "HI", NULL, };
static char *graphv_ou[] = { "¤", "OU", "Ê", "GY", NULL, };
static char *graphv_to[] = { "Æ", "TO", NULL, };
static char *graphv_ny[] = { "Ç", "NY", NULL, };
static char *graphv_nk[] = { "\", "NK", NULL, };
static char *graphv_ng[] = { "S", "NG", NULL, };
static char *graphv_um[] = { "n", "UM", NULL, };
static char *graphv_ry[] = { "´", "RY", NULL, };
// Áê
static char *graphv_room[] = { "E", "  ", "@", NULL, };
static char *graphv_rock[] = { "", "IS", "Î", NULL, };
static char *graphv_pit[] = { "", "PT", "âv", NULL, };
// tFA[`FX - [p[°
static char *graphv_ze[] = { "ë", "ZE", NULL, };	// (0, 0)-Leaper Zero
static char *graphv_wa[] = { "b", "WA", NULL, };	// (0, 1)-Leaper Wazir
static char *graphv_da[] = { "í", "DA", NULL, };	// (0, 2)-Leaper Dabbaba
static char *graphv_tl[] = { "Q", "TL", NULL, };	// (0, 3)-Leaper Threeleaper
static char *graphv_fe[] = { "«", "FE", NULL, };	// (1, 1)-Leaper Fers
static char *graphv_kn[] = { "R", "KN", NULL, };	// (1, 2)-Leaper Knight
static char *graphv_cm[] = { "ép", "CM", NULL, };	// (1, 3)-Leaper Camel
static char *graphv_gr[] = { "«", "GR", NULL, };	// (1, 4)-Leaper Giraffe
static char *graphv_fl[] = { "g", "FL", NULL, };	// (1, 6)-Leaper Flamingo
static char *graphv_al[] = { "¼", "AL", NULL, };	// (2, 2)-Leaper Alfil
static char *graphv_zb[] = { "È", "ZB", NULL, };	// (2, 3)-Leaper Zebra
static char *graphv_tr[] = { "V", "TR", NULL, };	// (3, 3)-Leaper Tripper
static char *graphv_l5[] = { "Þ", "L5", NULL, };	// (1, 5)+(3, 4)-Leaper 5-Leaper 
static char *graphv_r4[] = { "²", "R4", NULL, };	// (2, 6)-Leaper Root-40-Leaper 
static char *graphv_r5[] = { "æ", "R5", NULL, };	// (1, 7)+(5, 5)-Leaper Root-50-Leaper
static char *graphv_r6[] = { "O", "R6", NULL, };	// (1, 8)+(4, 7)-Leaper Root-65-Leaper
// tFA[`FX - C_[°
static char *graphv_qu[] = { "p", "QU", NULL, };	// (0, 1)+(1, 1)-Rider Queen
static char *graphv_nr[] = { "é", "NR", NULL, };	// (1, 2)-Rider Nightrider
static char *graphv_cr[] = { "ép", "CR", NULL, };	// (1, 3)-Rider Camelrider
static char *graphv_zr[] = { "È", "ZR", NULL, };	// (2, 3)-Rider Zebrarider
static char *graphv_rs[] = { "åK", "RS", NULL, };	// (1, 2)-CircleRider Rose
// tFA[`FX - ¡i[p[AC_[j°
static char *graphv_em[] = { "@", "EM", NULL, };	// (0, 1)-Rider+(1, 2)-Leaper Empress
static char *graphv_pr[] = { "P", "PR", NULL, };	// (1, 1)-Rider+(1, 2)-Leaper Princess
// tFA[`FX - zbp[°
static char *graphv_gh[] = { "f", "GH", NULL, };	// Grasshopper
static char *graphv_lo[] = { "å", "LO", NULL, };	// Locust
static char *graphv_li[] = { "é¥", "LI", NULL, };	// Lion
static char *graphv_ro[] = { "l", "RO", NULL, };	// Kangaroo
static char *graphv_ne[] = { "d", "NE", "Ï", NULL, };	// Non-stop equihopper
static char *graphv_eh[] = { "d", "EH", "", NULL, };	// Equihopper
static char *graphv_sp[] = { "", "SP", NULL, };	// Sparrow
static char *graphv_ea[] = { "h", "EA", NULL, };	// Eagle
// tFA[`FX - Ûû°
static char *graphv_pa[] = { "ï", "PA", NULL, };	// ÛûFïEà{
static char *graphv_va[] = { "ï", "VA", NULL, };
static char *graphv_le[] = { "ï", "LE", NULL, };
static char *graphv_sh[] = { "", "SH", NULL, };	// ÛûFEÛ
static char *graphv_ma[] = { "}", "MA", NULL, };	// ÛûFn
static char *graphv_mo[] = { "", "MO", NULL, };	// ÛûFnð45xñ]
// «û°
static char *graphv_ss[] = { "", "SS", NULL, };
static char *graphv_sz[] = { "Û", "SZ", NULL, };	// ->¾
static char *graphv_kr[] = { "êi", "KR", NULL, };	// ->tij
static char *graphv_ho[] = { "P", "HO", NULL, };	// ->{izj
static char *graphv_mk[] = { "Õ", "MK", NULL, };	// ->­
static char *graphv_do[] = { "º", "DO", NULL, };	// ->©i¡j
static char *graphv_mh[] = { "^", "MH", NULL, };	// ->Nipj
static char *graphv_cn[] = { "", "CN", NULL, };	// ->æ³iÛj
static char *graphv_hn[] = { "z", "HN", NULL, };
static char *graphv_jg[] = { "G", "JG", NULL, };	// ->
static char *graphv_og[] = { "¡", "OG", NULL, };	// ->
static char *graphv_he[] = { "½", "HE", NULL, };	// ->~
static char *graphv_ts[] = { "¾", "TS", NULL, };
static char *graphv_hr[] = { "­", "HR", NULL, };	// <-Õ
static char *graphv_hj[] = { "h", "HJ", NULL, };	// <-«ûF´
static char *graphv_tk[] = { "é", "TK", NULL, };	// <-«ûFn
static char *graphv_hg[] = { "", "HG", NULL, };	// <-G
static char *graphv_hc[] = { "", "HC", NULL, };	// <-¡
static char *graphv_kg[] = { "~", "KG", NULL, };	// <-½
static char *graphv_hk[] = { "î", "HK", NULL, };	// <-«ûF
static char *graphv_chi[] = { "ò", "CHI", NULL, };	// ->i´j
static char *graphv_cka[] = { "p", "CKA", NULL, };	// ->}inj
static char *graphv_cki[] = { "à", "CKI", NULL, };	// ->qiòj
static char *graphv_cgi[] = { "â", "CGI", NULL, };	// ->§iGj
static char *graphv_cry[] = { "´", "CRY", NULL, };	// ->h
static char *graphv_cum[] = { "n", "CUM", NULL, };	// ->é
static char *graphv_cky[] = { "", "CKY", NULL, };	// ->î
static char *graphv_pss[] = { "t", "PSS", NULL, };	// <-êi
static char *graphv_phn[] = { "{", "PHO", NULL, };	// <-P
static char *graphv_pry[] = { "", "PRY", NULL, };	// <-«ûFò
static char *graphv_pum[] = { "}", "PUM", NULL, };	// <-«ûFp
static char *graphv_phi[] = { "q", "PHI", NULL, };	// <-«ûFà
static char *graphv_pka[] = { "N", "PKA", NULL, };	// <-^
static char *graphv_pjg[] = { "§", "PJG", NULL, };	// <-«ûFâ
static char *graphv_pog[] = { "©", "POG", NULL, };	// <-º
static char *graphv_psz[] = { "æ³", "PSZ", NULL, };	// <-
// ×«û°
static char *graphv_tho[] = { "Q", "THO", NULL, };	// Q
static char *graphv_ttr[] = { "ß", "TTR", NULL, };	// ß
static char *graphv_tkj[] = { "è³", "TKJ", NULL, };	// è³
static char *graphv_tlu[] = { "", "TLU", NULL, };	// ¶êG
static char *graphv_tru[] = { "¹", "TRU", NULL, };	// EêG
static char *graphv_ttk[] = { "é", "TTK", NULL, };	// é
static char *graphv_tkt[] = { "üG", "TKT", NULL, };	// üG
static char *graphv_ttb[] = { "", "TTB", NULL, };	// 
static char *graphv_tkr[] = { "éì", "TKR", NULL, };	// éì
// å«û°
static char *graphv_dhr[] = { "C", "DHR", NULL, };	// ò´ -> à«i¬àj
static char *graphv_dmg[] = { "N", "DMG", NULL, };	// Ò -> à«i¬àj
static char *graphv_dsc[] = { "å", "DSC", NULL, };	// p -> à«i¬àj
static char *graphv_dmj[] = { "L", "DMJ", NULL, };	// Ln -> à«i¬àj
static char *graphv_dnk[] = { "¬", "DNK", NULL, };	// à«i¬àj <- ò´AÒApALn
// ædåå«û°
static char *graphv_dbj[] = { "Ö", "DBJ", NULL, };	// å´ÖioWj
// g£tFA[î
static char *graphv_f1[] = { "@", "F1", NULL, };	// g£tFA[îP
static char *graphv_f2[] = { "A", "F2", NULL, };	// g£tFA[îQ
static char *graphv_f3[] = { "B", "F3", NULL, };	// g£tFA[îR
static char *graphv_f4[] = { "C", "F4", NULL, };	// g£tFA[îS
static char *graphv_f5[] = { "D", "F5", NULL, };	// g£tFA[îT
static char *graphv_f6[] = { "E", "F6", NULL, };	// g£tFA[îU
static char *graphv_f7[] = { "F", "F7", NULL, };	// g£tFA[îV
// Pao, Vao, Leo ÌftH[g\LðuïvÆ·éB
// g£\LÆµÄAuCvuà{vðpÓ·éB
// Pao, Vao, Leo ª¯ÉQÂÈãwè³êéÆAuCvuà{vðèÄéB
char graph_pa1[] = "ï";
char graph_pa2[] = "C";
char graph_pa3[] = "à{";
char hi_b_graph_pa2[] = " C";
char yo_b_graph_pa2[] = "vC";
char hi_b_graph_pa3[] = " à{";
char yo_b_graph_pa3[] = "và{";

// ------------
//  ÀÛîîñ
// ------------
piece_t piece[PC_END] = {
	{	// 0x00 ó
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x01 àº
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x02 Ô
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x03 jn
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x04 â«
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x05 à«
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x06 ps
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x07 òÔ
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x08 tFA[îP
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x09 tFA[îQ
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0a tFA[îR
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0b tFA[îS
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0c tFA[îT
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0d tFA[îU
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0e tFA[îV
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0f ¤«
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x10 âv
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x11 Æà
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x12 ¬
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x13 ¬j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x14 ¬â
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x15 Î
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x16 ´n
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x17 ´¤
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x18 ¬tFA[îPi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x19 ¬tFA[îQi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1a ¬tFA[îRi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1b ¬tFA[îSi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1c ¬tFA[îTi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1d ¬tFA[îUi»Ý¢T|[gj
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1e ¬tFA[îVi»Ý¢T|[gj
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
//  î{îîñ
// ------------
piece_info_t piece_info[FP_EXEND] = {
	{	// 0
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 1
		"àº", graphv_fu, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 18, 
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_2F | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 2
		"Ô", graphv_ky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_RUN),
	},
	{	// 3
		"jn", graphv_ke, hi_vec_ke, yo_vec_ke, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 4
		"â«", graphv_gi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 5
		"à«", graphv_ki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 6
		"ps", graphv_ka, vec_nul, vec_nul, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 7
		"òÔ", graphv_hi, vec_nul, vec_nul, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 8 
		"¤«", graphv_ou, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 9
		"Æà", graphv_to, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 18,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 10
		"¬", graphv_ny, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 11
		"¬j", graphv_nk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 12
		"¬â", graphv_ng, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 13
		"´n", graphv_um, vec_hi, vec_hi, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 14
		"´¤", graphv_ry, vec_ka, vec_ka, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 15
		"ó", graphv_room, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 16
		"Î", graphv_rock, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 17
		"âv", graphv_pit, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
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
		"PaoiÛûÌïj", graphv_pa, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 62
		"Vaoip«Ìïj", graphv_va, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 63
		"Leoip«Ìïj", graphv_le, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 64
		"ShaniÛûÌj", graphv_sh, vec_sh, vec_sh, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_STEP,
	},
	{	// 65
		"MaoiÛûÌnj", graphv_ma, vec_ma, vec_ma, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_MAO_STEP,
	},
	{	// 66
		"MoaiÛûÌnð45xñ]j", graphv_mo, vec_mo, vec_mo, vec_nul, vec_nul, 4,
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
		"qi«ûj", graphv_ss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 72
		"Ûi«ûj", graphv_sz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 73
		"êiÙi«ûj", graphv_kr, vec_kr, vec_kr, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 74
		"Pi«ûj", graphv_ho, vec_ho, vec_ho, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 75
		"ÓÕi«ûj", graphv_mk, hi_vec_mk, yo_vec_mk, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 76
		"º«i«ûj", graphv_do, hi_vec_f3b, yo_vec_f3b, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 77
		"Ò^i«ûj", graphv_mh, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 78
		"li«ûj", graphv_cn, vec_fb, vec_fb, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 79
		"z¤i«ûj", graphv_hn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 80
		"Gsi«ûj", graphv_jg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 81
		"¡si«ûj", graphv_og, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 82
		"½Ôi«ûj", graphv_he, vec_nul, vec_nul, vec_fb, vec_fb, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 83
		"¾qi«ûAÛÌ¬îj", graphv_ts, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 84
		"ò­i«ûAÓÕÌ¬îj", graphv_hr, vec_l3r3, vec_l3r3, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 85
		"òhi«ûA´¤Ì¬îj", graphv_hj, hi_leap_vec_hj, yo_leap_vec_hj,
		hi_run_vec_hj, yo_run_vec_hj, 4, B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 86
		"péi«ûA´nÌ¬îj", graphv_tk, hi_vec_tk, yo_vec_tk, hi_vec_mk, yo_vec_mk, 4,
		B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 87
		"òi«ûAGsÌ¬îj", graphv_hg, vec_nul, vec_nul, vec_f3b3, vec_f3b3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 88
		"zi«ûA¡sÌ¬îj", graphv_hc, vec_nul, vec_nul, vec_l3r3, vec_l3r3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 89
		"~éÇi«ûA½ÔÌ¬îj", graphv_kg, vec_nul, vec_nul, hi_vec_fb3, yo_vec_fb3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 90
		"îi«ûAÔÌ¬îj", graphv_hk, vec_nul, vec_nul, hi_vec_f3b, yo_vec_f3b, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 91
		"òÔi«ûj", graphv_chi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 92
		"psi«ûj", graphv_cka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 93
		"à«i«ûj", graphv_cki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 94
		"â«i«ûj", graphv_cgi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 95
		"´¤i«ûj", graphv_cry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 96
		"´ni«ûj", graphv_cum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 97
		"Ôi«ûj", graphv_cky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 98
		"qi«ûAêiÙÌ¬îj", graphv_pss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 99
		"z¤i«ûAPÌ¬îj", graphv_phn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 100
		"´¤i«ûAòÔÌ¬îj", graphv_pry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 101
		"´ni«ûApsÌ¬îj", graphv_pum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 102
		"òÔi«ûAà«Ì¬îj", graphv_phi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 103
		"psi«ûAÒ^Ì¬îj", graphv_pka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 104
		"Gsi«ûAâ«Ì¬îj", graphv_pjg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 105
		"¡si«ûAº«Ì¬îj", graphv_pog, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 106
		"Ûi«ûAlÌ¬îj", graphv_psz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 4,
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
		"Qi×«ûÌ¤j", graphv_tho, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 112
		"ßi×«ûj", graphv_ttr, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 113
		"è³i×«ûj", graphv_tkj, hi_vec_tkj, yo_vec_tkj, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 114
		"i×«ûA¶êGj",
		graphv_tlu, hi_vec_leap_tlu, yo_vec_leap_tlu, hi_vec_run_tlu, yo_vec_run_tlu, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 115
		"¹i×«ûAEêGj",
		graphv_tru, hi_vec_leap_tru, yo_vec_leap_tru, hi_vec_run_tru, yo_vec_run_tru, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 116
		"éi×«ûj", graphv_ttk, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 117
		"üGi×«ûAéÌ¬îj",
		graphv_tkt, hi_vec_leap_tkt, yo_vec_leap_tkt, hi_vec_run_tkt, yo_vec_run_tkt, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 118
		"i×«ûj", graphv_ttb, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_3T | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 119
		"éìi×«ûAÌ¬îj", graphv_tkr, hi_vec_tkr, yo_vec_tkr, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 120
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 121
		"ò´iå«ûj", graphv_dhr, vec_nul, vec_nul, vec_run_dhr, vec_run_dhr, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 122
		"Òiå«ûj", graphv_dmg, vec_nul, vec_nul, vec_run_dmg, vec_run_dmg, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 123
		"piå«ûj", graphv_dsc, vec_hi, vec_hi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 124
		"Lniå«ûj", graphv_dmj, vec_ka, vec_ka, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 125
		"à«iå«ûÌ¬àj", graphv_dnk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 126
		"å´Öiædåå«ûj", graphv_dbj, hi_vec_fb3, yo_vec_fb3, vec_nul, vec_nul, 4,
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
	{	// 131 g£tFA[îP
		"g£tFA[îP", graphv_f1, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 132 g£tFA[îQ
		"g£tFA[îQ", graphv_f2, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 133 g£tFA[îR
		"g£tFA[îR", graphv_f3, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 134 g£tFA[îS
		"g£tFA[îS", graphv_f4, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 135 g£tFA[îT
		"g£tFA[îT", graphv_f5, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 136 g£tFA[îU
		"g£tFA[îU", graphv_f6, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 137 g£tFA[îV
		"g£tFA[îV", graphv_f7, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
};

// ----------
//  ¬îîñ
// ----------
int fp_promoted[FP_EXEND] = {
	-1,		//  0: -
	 9,		//  1: àº -> ÆiÆàj
	10,		//  2: Ô -> Çi¬j
	11,		//  3: jn -> \i¬jj
	12,		//  4: â« -> Si¬âj
	-1,		//  5: à«
	13,		//  6: ps -> ni´nj
	14,		//  7: òÔ -> ´i´¤j
	-1,		//  8: ¤/Ê
	-1,		//  9: Æà
	-1,		// 10: ¬
	-1,		// 11: ¬j
	-1,		// 12: ¬â
	-1,		// 13: ´n
	-1,		// 14: ´¤
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
	-1,		// 61: PaoiÛûFïj
	-1,		// 62: Vao
	-1,		// 63: Leo
	-1,		// 64: ShaniÛûFj
	-1,		// 65: MaoiÛûFnj
	-1,		// 66: MoaiÛûFnð45xñ]j
	-1,		// 67: -
	-1,		// 68: -
	-1,		// 69: -
	-1,		// 70: -
	-1,		// 71: q
	83,		// 72: Û -> ¾q
	98,		// 73: êiÙ -> ti¬qj
	99,		// 74: P -> {i¬z¤j
	84,		// 75: ÓÕ -> ò­
	105,	// 76: º« -> ¡si©j
	103,	// 77: Ò^ -> Ni¬psj
	106,	// 78: l -> æ³i¬Ûj
	-1,		// 79: z¤
	87,		// 80: Gs -> ò
	88,		// 81: ¡s -> z
	89,		// 82: ½Ô -> ~éÇ
	-1,		// 83: ¾q
	-1,		// 84: ò­
	-1,		// 85: òh
	-1,		// 86: pé
	-1,		// 87: ò
	-1,		// 88: z
	-1,		// 89: ~éÇ
	-1,		// 90: î
	100,	// 91: «ûFò -> i¬´¤j
	101,	// 92: «ûFp -> }i¬´nj
	102,	// 93: «ûFà -> qi¬òÔj
	104,	// 94: «ûFâ -> §i¬Gsj
	85,		// 95: «ûF´ -> òh
	86,		// 96: «ûFn -> pé
	90,		// 97: «ûF -> î
	-1,		// 98: ti¬qj
	-1,		// 99: {i¬z¤j
	-1,		// 100: i¬´¤j
	-1,		// 101: }i¬´nj
	-1,		// 102: qi¬òÔj
	-1,		// 103: Ni¬psj
	-1,		// 104: §i¬Gsj
	-1,		// 105: ©i¬¡sj
	-1,		// 106: æ³i¬Ûj
	-1,		// 107: -
	-1,		// 108: -
	-1,		// 109: -
	-1,		// 110: -
	-1,		// 111: Q
	-1,		// 112: ß
	-1,		// 113: è³
	-1,		// 114: i¶êGj
	-1,		// 115: ¹iEêGj
	117,	// 116: é -> üG
	-1,		// 117: üG
	119,	// 118:  -> éì
	-1,		// 119: éì
	-1,		// 120: -
	125,	// 121: ò´ -> ¬ià«j
	125,	// 122: Ò -> ¬ià«j
	125,	// 123: p -> ¬ià«j
	125,	// 124: Ln -> ¬ià«j
	-1,		// 125: ¬ià«j
	-1,		// 126: å´Öi¦{ÍzÖÉ¬êéÌ¾ªA~éÇÆ¯¶«Ìîj
	-1,		// 127: -
	-1,		// 128: -
	-1,		// 129: -
	-1,		// 130: -
	-1,		// 131: g£tFA[îP
	-1,		// 132: g£tFA[îQ
	-1,		// 133: g£tFA[îR
	-1,		// 134: g£tFA[îS
	-1,		// 135: g£tFA[îT
	-1,		// 136: g£tFA[îU
	-1,		// 137: g£tFA[îV
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
	{ NOP, },						// ñî
	{								// à
		POS19(1, 7), POS19(2, 7), POS19(3, 7), POS19(4, 7), POS19(5, 7),
		POS19(6, 7), POS19(7, 7), POS19(8, 7), POS19(9, 7),
	},
	{ POS19(1, 9), POS19(9, 9), },	// 
	{ POS19(2, 9), POS19(8, 9), },	// j
	{ POS19(3, 9), POS19(7, 9), },	// â
	{ POS19(4, 9), POS19(6, 9), },	// à
	{ POS19(8, 8), },				// p
	{ POS19(2, 8), },				// ò
	{								// tFA[îP
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îQ
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îR
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îS
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îT
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îU
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// tFA[îV
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{ POS19(5, 9), },				// ¤
};
char yo_reborn_pos[RAW_PC_END][9] = {
	{ NOP, },						// ñî
	{								// à
		POS19(1, 3), POS19(2, 3), POS19(3, 3), POS19(4, 3), POS19(5, 3),
		POS19(6, 3), POS19(7, 3), POS19(8, 3), POS19(9, 3),
	},
	{ POS19(1, 1), POS19(9, 1), },	// 
	{ POS19(2, 1), POS19(8, 1), },	// j
	{ POS19(3, 1), POS19(7, 1), },	// â
	{ POS19(4, 1), POS19(6, 1), },	// à
	{ POS19(2, 2), },				// p
	{ POS19(8, 2), },				// ò
	{								// tFA[îP
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îQ
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îR
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îS
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îT
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îU
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// tFA[îV
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{ POS19(5, 1), },				// Ê
};

