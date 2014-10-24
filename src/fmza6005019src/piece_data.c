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
//  �ړ��x�N�g��
// --------------
// �ړ��Ȃ�
static vec_t vec_nul[] = { { 0, 0 }, };
// �O��  . . . . . | . . . . .
//       . . * . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fw[] = { { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_fw[] = { { 0, 1 }, { 0, 0 }, };
// �j�n  . . . . . . . | . . . . . . .
//       . . L . L . . | . . . . . . .
//       . . . . . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . . . . .
//       . . . . . . . | . . L . L . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_ke[] = { { -1, -2 }, { 1, -2 }, { 0, 0 }, };
static vec_t yo_vec_ke[] = { { -1, 2 }, { 1, 2 }, { 0, 0 }, };
// �⏫  . . . . . | . . . . .
//       . L L L . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . L L * .
//       . . . . . | . . . . .
static vec_t hi_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, -1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_gi[] = { { -1, -1 }, { -1, 1 }, { 0, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// ����  . . . . . | . . . . .
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
// �p�s  . . . . .
//       . * . * .
//       . . # . .
//       . * . * .
//       . . . . .
static vec_t vec_ka[] = { { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }, { 0, 0 }, };
// ���  . . . . .
//       . . * . .
//       . * # * .
//       . . * . .
//       . . . . .
static vec_t vec_hi[] = { { -1, 0 }, { 0, -1 }, { 0, 1 }, { 1, 0 }, { 0, 0 }, };
// ����  . . . . .
// ����  . * * * .
// Queen . * # * .
// �z��  . * * * .
// ���q  . . . . .
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
// Knight�i2014-08-05 Rose�p�Ƀf�[�^�����E�ォ�玞�v���ɕύX�j
// . . . . . . .   �E�E�E�E�E�E�E
// . . * . * . .   �E�E�F�E�G�E�E
// . * . . . * .   �E�E�E�E�E�@�E
// . . . # . . .   �E�E�E�K�E�E�E
// . * . . . * .   �E�D�E�E. �A�E
// . . * . * . .   �E�E�C�E�B�E�E
// . . . . . . .   �E�E�E�E�E�E�E
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
// Shan�i���j
// . . . . . . .
// . S . . . S .
// . . S . S . .
// . . . # . . .
// . . S . S . .
// . S . . . S .
// . . . . . . .
static vec_t vec_sh[] = { { -1, -1, 2 }, { -1, 1, 2 }, { 1, -1, 2 }, { 1, 1, 2 }, { 0, 0 }, };
// Mao�i�}�j
// . . . . . . .
// . . M . M . .
// . M . m . M .
// . . m # m . .
// . M . m . M .
// . . M . M . .
// . . . . . . .
static vec_t vec_ma[] = { { -1, 0, 1 }, { 0, 1, 1 }, { 1, 0, 1 }, { 0, -1, 1 }, { 0, 0 }, };
// Moa�i���j
// . . . . . . .
// . . M . M . .
// . M m . m M .
// . . . # . . .
// . M m . m M .
// . . M . M . .
// . . . . . . .
static vec_t vec_mo[] = { { -1, -1, 1 }, { -1, 1, 1 }, { 1, 1, 1 }, { 1, -1, 1 }, { 0, 0 }, };
// ���q  . . . . . . .
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
// ����  . . . . . | . . . . .
// ��    . L L L . | . L . L .
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
// �i��  . . . . . . .
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
// �P��  . . . . . . .
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
// �ӌ�  . . . . . | . . . . .
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
// �O3�� . . . . . | . . . . .
// ����  . * * * . | . . * . .
// ����  . . # . . | . . # . .
//       . . * . . | . * * * .
//       . . . . . | . . . . .
static vec_t hi_vec_f3b[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
static vec_t yo_vec_f3b[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
// �O��3 . . . . . | . . . . .
// �~��  . . * . . | . * * * .
//       . . # . . | . . # . .
//       . * * * . | . . * . .
//       . . . . . | . . . . .
static vec_t hi_vec_fb3[] = { { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_fb3[] = { { -1, -1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// �O3��3. . . . .
// �ҕ^  . * * * .
// ��  . . # . .
// ��    . * * * .
//       . . . . .
static vec_t vec_f3b3[] = {
	{ -1, -1 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 1, },
	{ 0, 0 },
};
// �O��  . . . . .
// ���l  . . * . .
// ����  . . # . .
//       . . * . .
//       . . . . .
static vec_t vec_fb[] = { { 0, -1 }, { 0, 1 }, { 0, 0 }, };
// ���E  . . . . .
// �G�s  . . . . .
//       . * # * .
//       . . . . .
//       . . . . .
static vec_t vec_lr[] = { { -1, 0 }, { 1, 0 }, { 0, 0 }, };
// ����  . . . . .
// ��  . * . * .
// �z��  . * # * .
//       . * . * .
//       . . . . .
static vec_t vec_l3r3[] = {
	{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 },
	{ 0, 0 },
};
// ��h  . . . . . . . | . . . . . . . | . . . R . . . | R . . R . . R
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
// �p��  . . . . . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . 2 . . . | . . . . . . .
//       . . . # . . . | . . . # . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . 2 . . .
//       . . . . . . . | . . . . . . .
static vec_t hi_vec_tk[] = { { 0, -2 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_tk[] = { { 0, 1 }, { 0, 2 }, { 0, 0 }, };
// �    . . L . . | . . . . .
//       . . . . . | . L . L .
//       . . # . . | . . # . .
//       . L . L . | . . . . .
//       . . . . . | . . L . .
static vec_t hi_vec_tkj[] = { { -1, 1 }, { 0, -2 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_tkj[] = { { -1, -1 }, { 0, 2 }, { 1, -1 }, { 0, 0 }, };
// ��    . . R . . | R . . . .
// ���G  . . R . . | . R . L .
//       . . # . . | . . # . .
//       . L . R . | . . R . .
//       . . . . R | . . R . .
static vec_t hi_vec_leap_tlu[] = { { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tlu[] = { { -1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tlu[] = { { -1, 1 }, { 0, -1 }, { 0, 0 }, };
static vec_t yo_vec_run_tlu[] = { { 0, 1 }, { 1, -1 }, { 0, 0 }, };
// ��    . . R . . | . . . . R
// �E�G  . . R . . | . L . R .
//       . . # . . | . . # . .
//       . R . L . | . . R . .
//       R . . . . | . . R . .
static vec_t hi_vec_leap_tru[] = { { -1, 1 }, { 0, 0 }, };
static vec_t yo_vec_leap_tru[] = { { 1, -1 }, { 0, 0 }, };
static vec_t hi_vec_run_tru[] = { { 0, -1 }, { 1, 1 }, { 0, 0 }, };
static vec_t yo_vec_run_tru[] = { { -1, -1 }, { 0, 1 }, { 0, 0 }, };
// �G    R . . . . . R | . . . R . . .
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
// ��    L . . . L | . . L . .
//       . . . . . | . . . . .
//       . . # . . | . . # . .
//       . . . . . | . . . . .
//       . . L . . | L . . . L
static vec_t hi_vec_tkr[] = { { -2, -2 }, { 0, 2 }, { 2, -2 }, { 0, 0 }, };
static vec_t yo_vec_tkr[] = { { -2, 2 }, { 0, -2 }, { 2, 2 }, { 0, 0 }, };
// ��  . . . . . . .
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
// �ҋ�  . . . . . . .
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
//  ���
// ------
// ������
static char *graphv_fu[] = { "��", "FU", NULL, };
static char *graphv_ky[] = { "��", "KY", NULL, };
static char *graphv_ke[] = { "�j", "KE", NULL, };
static char *graphv_gi[] = { "��", "GI", NULL, };
static char *graphv_ki[] = { "��", "KI", NULL, };
static char *graphv_ka[] = { "�p", "KA", NULL, };
static char *graphv_hi[] = { "��", "HI", NULL, };
static char *graphv_ou[] = { "��", "OU", "��", "GY", NULL, };
static char *graphv_to[] = { "��", "TO", NULL, };
static char *graphv_ny[] = { "��", "NY", NULL, };
static char *graphv_nk[] = { "�\", "NK", NULL, };
static char *graphv_ng[] = { "�S", "NG", NULL, };
static char *graphv_um[] = { "�n", "UM", NULL, };
static char *graphv_ry[] = { "��", "RY", NULL, };
// ����
static char *graphv_room[] = { "�E", "  ", "�@", NULL, };
static char *graphv_rock[] = { "��", "IS", "��", NULL, };
static char *graphv_pit[] = { "��", "PT", "�v", NULL, };
// �t�F�A���[�`�F�X - ���[�p�[��
static char *graphv_ze[] = { "��", "ZE", NULL, };	// (0, 0)-Leaper Zero
static char *graphv_wa[] = { "�b", "WA", NULL, };	// (0, 1)-Leaper Wazir
static char *graphv_da[] = { "��", "DA", NULL, };	// (0, 2)-Leaper Dabbaba
static char *graphv_tl[] = { "�Q", "TL", NULL, };	// (0, 3)-Leaper Threeleaper
static char *graphv_fe[] = { "��", "FE", NULL, };	// (1, 1)-Leaper Fers
static char *graphv_kn[] = { "�R", "KN", NULL, };	// (1, 2)-Leaper Knight
static char *graphv_cm[] = { "�p", "CM", NULL, };	// (1, 3)-Leaper Camel
static char *graphv_gr[] = { "��", "GR", NULL, };	// (1, 4)-Leaper Giraffe
static char *graphv_fl[] = { "�g", "FL", NULL, };	// (1, 6)-Leaper Flamingo
static char *graphv_al[] = { "��", "AL", NULL, };	// (2, 2)-Leaper Alfil
static char *graphv_zb[] = { "��", "ZB", NULL, };	// (2, 3)-Leaper Zebra
static char *graphv_tr[] = { "�V", "TR", NULL, };	// (3, 3)-Leaper Tripper
static char *graphv_l5[] = { "��", "L5", NULL, };	// (1, 5)+(3, 4)-Leaper 5-Leaper 
static char *graphv_r4[] = { "��", "R4", NULL, };	// (2, 6)-Leaper Root-40-Leaper 
static char *graphv_r5[] = { "��", "R5", NULL, };	// (1, 7)+(5, 5)-Leaper Root-50-Leaper
static char *graphv_r6[] = { "�O", "R6", NULL, };	// (1, 8)+(4, 7)-Leaper Root-65-Leaper
// �t�F�A���[�`�F�X - ���C�_�[��
static char *graphv_qu[] = { "�p", "QU", NULL, };	// (0, 1)+(1, 1)-Rider Queen
static char *graphv_nr[] = { "��", "NR", NULL, };	// (1, 2)-Rider Nightrider
static char *graphv_cr[] = { "�p", "CR", NULL, };	// (1, 3)-Rider Camelrider
static char *graphv_zr[] = { "��", "ZR", NULL, };	// (2, 3)-Rider Zebrarider
static char *graphv_rs[] = { "�K", "RS", NULL, };	// (1, 2)-CircleRider Rose
// �t�F�A���[�`�F�X - �����i���[�p�[�A���C�_�[�j��
static char *graphv_em[] = { "�@", "EM", NULL, };	// (0, 1)-Rider+(1, 2)-Leaper Empress
static char *graphv_pr[] = { "�P", "PR", NULL, };	// (1, 1)-Rider+(1, 2)-Leaper Princess
// �t�F�A���[�`�F�X - �z�b�p�[��
static char *graphv_gh[] = { "�f", "GH", NULL, };	// Grasshopper
static char *graphv_lo[] = { "�", "LO", NULL, };	// Locust
static char *graphv_li[] = { "�", "LI", NULL, };	// Lion
static char *graphv_ro[] = { "�l", "RO", NULL, };	// Kangaroo
static char *graphv_ne[] = { "�d", "NE", "��", NULL, };	// Non-stop equihopper
static char *graphv_eh[] = { "�d", "EH", "��", NULL, };	// Equihopper
static char *graphv_sp[] = { "��", "SP", NULL, };	// Sparrow
static char *graphv_ea[] = { "�h", "EA", NULL, };	// Eagle
// �t�F�A���[�`�F�X - �ۊ���
static char *graphv_pa[] = { "��", "PA", NULL, };	// �ۊ��F��E�{
static char *graphv_va[] = { "��", "VA", NULL, };
static char *graphv_le[] = { "��", "LE", NULL, };
static char *graphv_sh[] = { "��", "SH", NULL, };	// �ۊ��F���E��
static char *graphv_ma[] = { "�}", "MA", NULL, };	// �ۊ��F�n
static char *graphv_mo[] = { "��", "MO", NULL, };	// �ۊ��F�n��45�x��]
// ��������
static char *graphv_ss[] = { "��", "SS", NULL, };
static char *graphv_sz[] = { "��", "SZ", NULL, };	// ->��
static char *graphv_kr[] = { "�i", "KR", NULL, };	// ->�t�i���j
static char *graphv_ho[] = { "�P", "HO", NULL, };	// ->�{�i�z�j
static char *graphv_mk[] = { "��", "MK", NULL, };	// ->��
static char *graphv_do[] = { "��", "DO", NULL, };	// ->���i���j
static char *graphv_mh[] = { "�^", "MH", NULL, };	// ->�N�i�p�j
static char *graphv_cn[] = { "��", "CN", NULL, };	// ->況i�ہj
static char *graphv_hn[] = { "�z", "HN", NULL, };
static char *graphv_jg[] = { "�G", "JG", NULL, };	// ->��
static char *graphv_og[] = { "��", "OG", NULL, };	// ->��
static char *graphv_he[] = { "��", "HE", NULL, };	// ->�~
static char *graphv_ts[] = { "��", "TS", NULL, };
static char *graphv_hr[] = { "��", "HR", NULL, };	// <-��
static char *graphv_hj[] = { "�h", "HJ", NULL, };	// <-�������F��
static char *graphv_tk[] = { "��", "TK", NULL, };	// <-�������F�n
static char *graphv_hg[] = { "��", "HG", NULL, };	// <-�G
static char *graphv_hc[] = { "��", "HC", NULL, };	// <-��
static char *graphv_kg[] = { "�~", "KG", NULL, };	// <-��
static char *graphv_hk[] = { "��", "HK", NULL, };	// <-�������F��
static char *graphv_chi[] = { "��", "CHI", NULL, };	// ->���i���j
static char *graphv_cka[] = { "�p", "CKA", NULL, };	// ->�}�i�n�j
static char *graphv_cki[] = { "��", "CKI", NULL, };	// ->�q�i��j
static char *graphv_cgi[] = { "��", "CGI", NULL, };	// ->���i�G�j
static char *graphv_cry[] = { "��", "CRY", NULL, };	// ->�h
static char *graphv_cum[] = { "�n", "CUM", NULL, };	// ->��
static char *graphv_cky[] = { "��", "CKY", NULL, };	// ->��
static char *graphv_pss[] = { "�t", "PSS", NULL, };	// <-�i
static char *graphv_phn[] = { "�{", "PHO", NULL, };	// <-�P
static char *graphv_pry[] = { "��", "PRY", NULL, };	// <-�������F��
static char *graphv_pum[] = { "�}", "PUM", NULL, };	// <-�������F�p
static char *graphv_phi[] = { "�q", "PHI", NULL, };	// <-�������F��
static char *graphv_pka[] = { "�N", "PKA", NULL, };	// <-�^
static char *graphv_pjg[] = { "��", "PJG", NULL, };	// <-�������F��
static char *graphv_pog[] = { "��", "POG", NULL, };	// <-��
static char *graphv_psz[] = { "�", "PSZ", NULL, };	// <-��
// �׏�����
static char *graphv_tho[] = { "�Q", "THO", NULL, };	// �Q
static char *graphv_ttr[] = { "��", "TTR", NULL, };	// ��
static char *graphv_tkj[] = { "�", "TKJ", NULL, };	// �
static char *graphv_tlu[] = { "��", "TLU", NULL, };	// ���G
static char *graphv_tru[] = { "��", "TRU", NULL, };	// �E�G
static char *graphv_ttk[] = { "��", "TTK", NULL, };	// ��
static char *graphv_tkt[] = { "�G", "TKT", NULL, };	// �G
static char *graphv_ttb[] = { "��", "TTB", NULL, };	// ��
static char *graphv_tkr[] = { "��", "TKR", NULL, };	// ��
// �叫����
static char *graphv_dhr[] = { "�C", "DHR", NULL, };	// �� -> �����i�����j
static char *graphv_dmg[] = { "�N", "DMG", NULL, };	// �ҋ� -> �����i�����j
static char *graphv_dsc[] = { "��", "DSC", NULL, };	// �p�� -> �����i�����j
static char *graphv_dmj[] = { "�L", "DMJ", NULL, };	// �L�n -> �����i�����j
static char *graphv_dnk[] = { "��", "DNK", NULL, };	// �����i�����j <- �򗴁A�ҋ��A�p���A�L�n
// ���d��叫����
static char *graphv_dbj[] = { "��", "DBJ", NULL, };	// 崎ցi�o���W���j
// �g���t�F�A���[��
static char *graphv_f1[] = { "�@", "F1", NULL, };	// �g���t�F�A���[��P
static char *graphv_f2[] = { "�A", "F2", NULL, };	// �g���t�F�A���[��Q
static char *graphv_f3[] = { "�B", "F3", NULL, };	// �g���t�F�A���[��R
static char *graphv_f4[] = { "�C", "F4", NULL, };	// �g���t�F�A���[��S
static char *graphv_f5[] = { "�D", "F5", NULL, };	// �g���t�F�A���[��T
static char *graphv_f6[] = { "�E", "F6", NULL, };	// �g���t�F�A���[��U
static char *graphv_f7[] = { "�F", "F7", NULL, };	// �g���t�F�A���[��V
// Pao, Vao, Leo �̃f�t�H�[���g�\�L���u��v�Ƃ���B
// �g���\�L�Ƃ��āA�u�C�v�u�{�v��p�ӂ���B
// Pao, Vao, Leo �������ɂQ�ȏ�w�肳���ƁA�����u�C�v�u�{�v�����蓖�Ă�B
char graph_pa1[] = "��";
char graph_pa2[] = "�C";
char graph_pa3[] = "�{";
char hi_b_graph_pa2[] = " �C";
char yo_b_graph_pa2[] = "v�C";
char hi_b_graph_pa3[] = " �{";
char yo_b_graph_pa3[] = "v�{";

// ------------
//  ���ۋ���
// ------------
piece_t piece[PC_END] = {
	{	// 0x00 ��
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x01 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x02 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x03 �j�n
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x04 �⏫
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x05 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x06 �p�s
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x07 ���
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x08 �t�F�A���[��P
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x09 �t�F�A���[��Q
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0a �t�F�A���[��R
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0b �t�F�A���[��S
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0c �t�F�A���[��T
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0d �t�F�A���[��U
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0e �t�F�A���[��V
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x0f ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x10 �v
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x11 �Ƌ�
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x12 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x13 ���j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x14 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x15 ��
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x16 ���n
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x17 ����
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x18 ���t�F�A���[��P�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x19 ���t�F�A���[��Q�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1a ���t�F�A���[��R�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1b ���t�F�A���[��S�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1c ���t�F�A���[��T�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1d ���t�F�A���[��U�i���ݖ��T�|�[�g�j
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		NULL, NULL, NULL, NULL, NULL,
		0x00000000, 0x00000000, 0,
	},
	{	// 0x1e ���t�F�A���[��V�i���ݖ��T�|�[�g�j
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
//  ��{����
// ------------
piece_info_t piece_info[FP_EXEND] = {
	{	// 0
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 1
		"����", graphv_fu, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 18, 
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_2F | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 2
		"����", graphv_ky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_RUN),
	},
	{	// 3
		"�j�n", graphv_ke, hi_vec_ke, yo_vec_ke, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 4
		"�⏫", graphv_gi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 5
		"����", graphv_ki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 6
		"�p�s", graphv_ka, vec_nul, vec_nul, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 7
		"���", graphv_hi, vec_nul, vec_nul, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_RUN,
	},
	{	// 8 
		"����", graphv_ou, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 9
		"�Ƌ�", graphv_to, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 18,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 10
		"����", graphv_ny, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 11
		"���j", graphv_nk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 12
		"����", graphv_ng, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 13
		"���n", graphv_um, vec_hi, vec_hi, vec_ka, vec_ka, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 14
		"����", graphv_ry, vec_ka, vec_ka, vec_hi, vec_hi, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 15
		"��", graphv_room, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 16
		"��", graphv_rock, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
	},
	{	// 17
		"�v", graphv_pit, vec_nul, vec_nul, vec_nul, vec_nul, 81, 0x00000000, 0x00000000,
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
		"Pao�i�ۊ��̕�j", graphv_pa, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 62
		"Vao�i�p�����̕�j", graphv_va, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 63
		"Leo�i�p�����̕�j", graphv_le, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_PAO_JUMP,
	},
	{	// 64
		"Shan�i�ۊ��̑��j", graphv_sh, vec_sh, vec_sh, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_STEP,
	},
	{	// 65
		"Mao�i�ۊ��̔n�j", graphv_ma, vec_ma, vec_ma, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_MAO_STEP,
	},
	{	// 66
		"Moa�i�ۊ��̔n��45�x��]�j", graphv_mo, vec_mo, vec_mo, vec_nul, vec_nul, 4,
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
		"���q�i�������j", graphv_ss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 72
		"���ہi�������j", graphv_sz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 73
		"�i�فi�������j", graphv_kr, vec_kr, vec_kr, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 74
		"�P���i�������j", graphv_ho, vec_ho, vec_ho, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 75
		"�ӌՁi�������j", graphv_mk, hi_vec_mk, yo_vec_mk, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 76
		"�����i�������j", graphv_do, hi_vec_f3b, yo_vec_f3b, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 77
		"�ҕ^�i�������j", graphv_mh, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 78
		"���l�i�������j", graphv_cn, vec_fb, vec_fb, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 79
		"�z���i�������j", graphv_hn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 80
		"�G�s�i�������j", graphv_jg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 81
		"���s�i�������j", graphv_og, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 82
		"���ԁi�������j", graphv_he, vec_nul, vec_nul, vec_fb, vec_fb, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 83
		"���q�i�������A���ۂ̐���j", graphv_ts, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 84
		"�򎭁i�������A�ӌՂ̐���j", graphv_hr, vec_l3r3, vec_l3r3, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 85
		"��h�i�������A�����̐���j", graphv_hj, hi_leap_vec_hj, yo_leap_vec_hj,
		hi_run_vec_hj, yo_run_vec_hj, 4, B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 86
		"�p��i�������A���n�̐���j", graphv_tk, hi_vec_tk, yo_vec_tk, hi_vec_mk, yo_vec_mk, 4,
		B_PA_PIECE, (B_MS_SHISHI_LEAP | B_MS_RUN),
	},
	{	// 87
		"�򋍁i�������A�G�s�̐���j", graphv_hg, vec_nul, vec_nul, vec_f3b3, vec_f3b3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 88
		"�z���i�������A���s�̐���j", graphv_hc, vec_nul, vec_nul, vec_l3r3, vec_l3r3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 89
		"�~�ǁi�������A���Ԃ̐���j", graphv_kg, vec_nul, vec_nul, hi_vec_fb3, yo_vec_fb3, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 90
		"����i�������A���Ԃ̐���j", graphv_hk, vec_nul, vec_nul, hi_vec_f3b, yo_vec_f3b, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 91
		"��ԁi�������j", graphv_chi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 92
		"�p�s�i�������j", graphv_cka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 93
		"�����i�������j", graphv_cki, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 94
		"�⏫�i�������j", graphv_cgi, hi_vec_gi, yo_vec_gi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 95
		"�����i�������j", graphv_cry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 96
		"���n�i�������j", graphv_cum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 97
		"���ԁi�������j", graphv_cky, vec_nul, vec_nul, hi_vec_fw, yo_vec_fw, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 98
		"���q�i�������A�i�ق̐���j", graphv_pss, vec_ar2, vec_ar2, vec_nul, vec_nul, 2,
		B_PA_PIECE, B_MS_SHISHI_LEAP,
	},
	{	// 99
		"�z���i�������A�P���̐���j", graphv_phn, vec_nul, vec_nul, vec_ar, vec_ar, 2,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 100
		"�����i�������A��Ԃ̐���j", graphv_pry, vec_ka, vec_ka, vec_hi, vec_hi, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 101
		"���n�i�������A�p�s�̐���j", graphv_pum, vec_hi, vec_hi, vec_ka, vec_ka, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 102
		"��ԁi�������A�����̐���j", graphv_phi, vec_nul, vec_nul, vec_hi, vec_hi, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 103
		"�p�s�i�������A�ҕ^�̐���j", graphv_pka, vec_nul, vec_nul, vec_ka, vec_ka, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 104
		"�G�s�i�������A�⏫�̐���j", graphv_pjg, vec_lr, vec_lr, vec_fb, vec_fb, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 105
		"���s�i�������A�����̐���j", graphv_pog, vec_fb, vec_fb, vec_lr, vec_lr, 4,
		B_PA_PIECE, (B_MS_LEAP | B_MS_RUN),
	},
	{	// 106
		"���ہi�������A���l�̐���j", graphv_psz, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 4,
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
		"�Q�i�׏����̉��j", graphv_tho, vec_ar, vec_ar, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_ROYAL), B_MS_LEAP,
	},
	{	// 112
		"�߁i�׏����j", graphv_ttr, vec_f3b3, vec_f3b3, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 113
		"賁i�׏����j", graphv_tkj, hi_vec_tkj, yo_vec_tkj, vec_nul, vec_nul, 4,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 114
		"���i�׏����A���G�j",
		graphv_tlu, hi_vec_leap_tlu, yo_vec_leap_tlu, hi_vec_run_tlu, yo_vec_run_tlu, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 115
		"���i�׏����A�E�G�j",
		graphv_tru, hi_vec_leap_tru, yo_vec_leap_tru, hi_vec_run_tru, yo_vec_run_tru, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 116
		"��i�׏����j", graphv_ttk, hi_vec_sz, yo_vec_sz, vec_nul, vec_nul, 2,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 117
		"�G�i�׏����A��̐���j",
		graphv_tkt, hi_vec_leap_tkt, yo_vec_leap_tkt, hi_vec_run_tkt, yo_vec_run_tkt, 2,
		(B_PA_PIECE | B_PA_REUSABLE), (B_MS_LEAP | B_MS_RUN),
	},
	{	// 118
		"���i�׏����j", graphv_ttb, hi_vec_fw, yo_vec_fw, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_PLACABLE | B_PA_REUSABLE | B_PA_CONSTRAINT_3T | B_PA_CONSTRAINT_PM),
		B_MS_LEAP,
	},
	{	// 119
		"��i�׏����A���̐���j", graphv_tkr, hi_vec_tkr, yo_vec_tkr, vec_nul, vec_nul, 16,
		(B_PA_PIECE | B_PA_REUSABLE), B_MS_LEAP,
	},
	{	// 120
		NULL, NULL, NULL, NULL, NULL, NULL, 0, 0x00000000, 0x00000000,
	},
	{	// 121
		"�򗴁i�叫���j", graphv_dhr, vec_nul, vec_nul, vec_run_dhr, vec_run_dhr, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 122
		"�ҋ��i�叫���j", graphv_dmg, vec_nul, vec_nul, vec_run_dmg, vec_run_dmg, 4,
		B_PA_PIECE, B_MS_RUN,
	},
	{	// 123
		"�p���i�叫���j", graphv_dsc, vec_hi, vec_hi, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 124
		"�L�n�i�叫���j", graphv_dmj, vec_ka, vec_ka, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 125
		"�����i�叫���̐����j", graphv_dnk, hi_vec_ki, yo_vec_ki, vec_nul, vec_nul, 4,
		B_PA_PIECE, B_MS_LEAP,
	},
	{	// 126
		"崎ցi���d��叫���j", graphv_dbj, hi_vec_fb3, yo_vec_fb3, vec_nul, vec_nul, 4,
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
	{	// 131 �g���t�F�A���[��P
		"�g���t�F�A���[��P", graphv_f1, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 132 �g���t�F�A���[��Q
		"�g���t�F�A���[��Q", graphv_f2, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 133 �g���t�F�A���[��R
		"�g���t�F�A���[��R", graphv_f3, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 134 �g���t�F�A���[��S
		"�g���t�F�A���[��S", graphv_f4, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 135 �g���t�F�A���[��T
		"�g���t�F�A���[��T", graphv_f5, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 136 �g���t�F�A���[��U
		"�g���t�F�A���[��U", graphv_f6, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
	{	// 137 �g���t�F�A���[��V
		"�g���t�F�A���[��V", graphv_f7, vec_nul, vec_nul, vec_nul, vec_nul, 1,
		B_PA_PIECE, 0x00000000,
	},
};

// ----------
//  ������
// ----------
int fp_promoted[FP_EXEND] = {
	-1,		//  0: -
	 9,		//  1: ���� -> �Ɓi�Ƌ��j
	10,		//  2: ���� -> �ǁi�����j
	11,		//  3: �j�n -> �\�i���j�j
	12,		//  4: �⏫ -> �S�i����j
	-1,		//  5: ����
	13,		//  6: �p�s -> �n�i���n�j
	14,		//  7: ��� -> ���i�����j
	-1,		//  8: ��/��
	-1,		//  9: �Ƌ�
	-1,		// 10: ����
	-1,		// 11: ���j
	-1,		// 12: ����
	-1,		// 13: ���n
	-1,		// 14: ����
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
	-1,		// 61: Pao�i�ۊ��F��j
	-1,		// 62: Vao
	-1,		// 63: Leo
	-1,		// 64: Shan�i�ۊ��F���j
	-1,		// 65: Mao�i�ۊ��F�n�j
	-1,		// 66: Moa�i�ۊ��F�n��45�x��]�j
	-1,		// 67: -
	-1,		// 68: -
	-1,		// 69: -
	-1,		// 70: -
	-1,		// 71: ���q
	83,		// 72: ���� -> ���q
	98,		// 73: �i�� -> �t�i�����q�j
	99,		// 74: �P�� -> �{�i���z���j
	84,		// 75: �ӌ� -> ��
	105,	// 76: ���� -> ���s�i���j
	103,	// 77: �ҕ^ -> �N�i���p�s�j
	106,	// 78: ���l -> 況i�����ہj
	-1,		// 79: �z��
	87,		// 80: �G�s -> ��
	88,		// 81: ���s -> �z��
	89,		// 82: ���� -> �~��
	-1,		// 83: ���q
	-1,		// 84: ��
	-1,		// 85: ��h
	-1,		// 86: �p��
	-1,		// 87: ��
	-1,		// 88: �z��
	-1,		// 89: �~��
	-1,		// 90: ����
	100,	// 91: �������F�� -> ���i�������j
	101,	// 92: �������F�p -> �}�i�����n�j
	102,	// 93: �������F�� -> �q�i����ԁj
	104,	// 94: �������F�� -> ���i���G�s�j
	85,		// 95: �������F�� -> ��h
	86,		// 96: �������F�n -> �p��
	90,		// 97: �������F�� -> ����
	-1,		// 98: �t�i�����q�j
	-1,		// 99: �{�i���z���j
	-1,		// 100: ���i�������j
	-1,		// 101: �}�i�����n�j
	-1,		// 102: �q�i����ԁj
	-1,		// 103: �N�i���p�s�j
	-1,		// 104: ���i���G�s�j
	-1,		// 105: ���i�����s�j
	-1,		// 106: 況i�����ہj
	-1,		// 107: -
	-1,		// 108: -
	-1,		// 109: -
	-1,		// 110: -
	-1,		// 111: �Q
	-1,		// 112: ��
	-1,		// 113: �
	-1,		// 114: ���i���G�j
	-1,		// 115: ���i�E�G�j
	117,	// 116: �� -> �G
	-1,		// 117: �G
	119,	// 118: �� -> ��
	-1,		// 119: ��
	-1,		// 120: -
	125,	// 121: �� -> ���i�����j
	125,	// 122: �ҋ� -> ���i�����j
	125,	// 123: �p�� -> ���i�����j
	125,	// 124: �L�n -> ���i�����j
	-1,		// 125: ���i�����j
	-1,		// 126: 崎ցi���{���͖z�ւɐ����̂����A�~�ǂƓ��������̋�j
	-1,		// 127: -
	-1,		// 128: -
	-1,		// 129: -
	-1,		// 130: -
	-1,		// 131: �g���t�F�A���[��P
	-1,		// 132: �g���t�F�A���[��Q
	-1,		// 133: �g���t�F�A���[��R
	-1,		// 134: �g���t�F�A���[��S
	-1,		// 135: �g���t�F�A���[��T
	-1,		// 136: �g���t�F�A���[��U
	-1,		// 137: �g���t�F�A���[��V
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
	{ NOP, },						// ���
	{								// ��
		POS19(1, 7), POS19(2, 7), POS19(3, 7), POS19(4, 7), POS19(5, 7),
		POS19(6, 7), POS19(7, 7), POS19(8, 7), POS19(9, 7),
	},
	{ POS19(1, 9), POS19(9, 9), },	// ��
	{ POS19(2, 9), POS19(8, 9), },	// �j
	{ POS19(3, 9), POS19(7, 9), },	// ��
	{ POS19(4, 9), POS19(6, 9), },	// ��
	{ POS19(8, 8), },				// �p
	{ POS19(2, 8), },				// ��
	{								// �t�F�A���[��P
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��Q
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��R
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��S
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��T
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��U
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{								// �t�F�A���[��V
		POS19(1, 8), POS19(2, 8), POS19(3, 8), POS19(4, 8), POS19(5, 8),
		POS19(6, 8), POS19(7, 8), POS19(8, 8), POS19(9, 8),
	},
	{ POS19(5, 9), },				// ��
};
char yo_reborn_pos[RAW_PC_END][9] = {
	{ NOP, },						// ���
	{								// ��
		POS19(1, 3), POS19(2, 3), POS19(3, 3), POS19(4, 3), POS19(5, 3),
		POS19(6, 3), POS19(7, 3), POS19(8, 3), POS19(9, 3),
	},
	{ POS19(1, 1), POS19(9, 1), },	// ��
	{ POS19(2, 1), POS19(8, 1), },	// �j
	{ POS19(3, 1), POS19(7, 1), },	// ��
	{ POS19(4, 1), POS19(6, 1), },	// ��
	{ POS19(2, 2), },				// �p
	{ POS19(8, 2), },				// ��
	{								// �t�F�A���[��P
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��Q
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��R
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��S
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��T
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��U
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{								// �t�F�A���[��V
		POS19(1, 2), POS19(2, 2), POS19(3, 2), POS19(4, 2), POS19(5, 2),
		POS19(6, 2), POS19(7, 2), POS19(8, 2), POS19(9, 2),
	},
	{ POS19(5, 1), },				// ��
};

