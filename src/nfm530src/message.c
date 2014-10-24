// nfm/nfm/message.c -- Japanese messages
// Copyright(c) KAMINA Jiro 2006-2013.  All rights reserved.
// $Header$

#include	<stdlib.h>

const char *msg[] = {
	NULL,
	"%s %s - Copyright(c) KAMINA Jiro 2006-2013\n",
	"%s %s - %s\n",
	"��͂��܂����H (Yes/No/eXit) ",
	"�p�����܂����H (Yes/No) ",
	"���f...\n",
	"�s�l...\n",
	"%s %s��� %s �܂�\n",
	"�ʉ��F������\n",
	"�ʉ��F%s��ڂ��\n",
	"�����F������\n",
	"�����F%s��ڂ��\n",
	"���l�F\n",
	"���v���:\n",
	"�U���n�b�V��:       ���v /       ���o /       �V�K\n",
	"    ������  %10s / %10s / %10s\n",
	"���ϔ�r��  %10.2f / %10.2f / %10.2f\n",
	"����n�b�V��:       ���v /       ���o /       �V�K\n",
	"    ������  %10s / %10s / %10s\n",
	"���ϔ�r��  %10.2f / %10.2f / %10.2f\n",
	"�����������ăG���[...\n",
	"�t�@�C�� %s �I�[�v���G���[�i%s ���[�h�j...\n",
	"���͌`��: nfm [/I]���̓t�@�C�� [/O�o�̓t�@�C��] [/T���������t���O]\n"
		"              [/P�o�̓t���O] [/S�T�C�Y�t���O] [/E������t���O]\n"
		"          ���������t���O: E, M, S, R, { F- | F+ | H- | H+ }\n"
		"          �o�̓t���O: C, E, H, L, M, N, T, S, V\n"
		"          �T�C�Y�t���O: B={�ő�ՖʃZ����|�ő�ՖʃZ���������T�C�YMB}\n"
		"                        M={�ő�菇�Z����|�ő�菇�Z���������T�C�YMB}\n"
		"          ������t���O: A, R=�����������T�C�YMB, V=���z�������T�C�YMB\n",
	"���̓f�[�^�G���[�i%s�j...\n",
	"%s��ځF%s�F%s\n",
	"�ՖʃZ�����: %s (%sMB) + %s (%sMB)\n�菇�Z����: %s (%sMB)\n",
	"�ՖʃZ�����: %s (%sMB)\n�菇�Z����: %s (%sMB)\n",
	"�菇�Z�����: %s (%sMB) + %s (%sMB)\n�ՖʃZ����: %s (%sMB)\n",
	"�菇�Z�����: %s (%sMB)\n�ՖʃZ����: %s (%sMB)\n",
	"�ՖʃZ���ǉ�: %11s (%5sMB) / �� %5sMB\n",
	"�菇�Z���ǉ�: %11s (%5sMB) / �� %5sMB\n",
	"�菇�Z�����: %11s / ����[�x %5s / ���B�[�x %5s\n",
	"�S�����������T�C�Y = %13sB (%5sMB)\n",
	"�󕨗��������T�C�Y = %13sB (%5sMB)\n",
	"�S���z�������T�C�Y = %13sB (%5sMB)\n",
	"�󉼑z�������T�C�Y = %13sB (%5sMB)\n",
	"    �ǖʐ�:       ���� /       �L�� /       �V�K /       �B��\n",
	"% 6d���: %10s / %10s / %10s / %10s\n",
	"���Z�b�g�������T�C�Y = %sMB",
	"�菇�Z���ė��p�J�n...\n",
	"�菇�Z���ė��p�i%ld�j...\n",
};

const char *str[] = {
	NULL,
	"�o�ߎ���: %d�b",
	"�o�ߎ���: %d�� %d�b",
	"�o�ߎ���: %ld���� %d�� %d�b",
	"��͋ǖʐ�: %s",
	"��͐[�x: %s",
	"�ŏI�[�x: %s",
	"��͏I��",
	"��͒��f",
	"���͋l",
	"���͋l�i�ڕW�}�j",
	"���͋l�i�ڕW�}�E���j",
	"���͎��ʋl",
	"���͎��ʋl�i�ڕW�}�j",
	"���͎��ʋl�i�ڕW�}�E���j",
	"�A�����͋l",
	"�A���l",
	"���͐����",
	"PWC",
	"����",
	"NOPROM",
	"  �X �W �V �U �T �S �R �Q �P",
	"�܂� %s��",
	"�܂� %s+1��",
	"�]��",
	"�ȉ�",
	"%s��",
	"%s+1��",
	"����F",
	"�Ȃ�",
	"��",
	"��",
	"��",
	"��",
	"�s��",
	"�E",
	"��",
	"��",
	"��",
	"��",
	"��",
	"��",
	"�s",
	"����ǖ�",
	"�w",
	"�x",
	"���",
	"�܂�",
	"�J�n�}",
	"�I���}",
	"�l��}",
	"�ڕW�}",
	"���[���Ƌl�萔�̉��",
	"�p���f�[�^�̓Ǎ���",
	"���`�f�[�^�̉��",
	"�ڕW�}�f�[�^�̉��",
	"�g���f�[�^�̉��",
	"�w��l�菇�̉��",
	"�菇",
	"��",
	"�ړ���",
	"�ړ���",
	"�ߊl��",
	"����",
	"���ʒu",
	"����",
	"�ړ�",
	"�ǖ�",
	"[���]",
	"[PROBLEM]",
	"[�I��]",
	"[END]",
};

static const char *indata_00_pc[] = { NULL, };
static const char *indata_fu_pc[] =	{ "��", "FU", NULL, };
static const char *indata_ky_pc[] =	{ "��", "KY", NULL, };
static const char *indata_ke_pc[] =	{ "�j", "KE", NULL, };
static const char *indata_gi_pc[] =	{ "��", "GI", NULL, };
static const char *indata_ki_pc[] =	{ "��", "KI", NULL, };
static const char *indata_ka_pc[] =	{ "�p", "KA", NULL, };
static const char *indata_hi_pc[] =	{ "��", "HI", NULL, };
static const char *indata_08_pc[] =	{ NULL, };
static const char *indata_to_pc[] =	{ "��", "TO", NULL, };
static const char *indata_ny_pc[] =	{ "��", "NY", NULL, };
static const char *indata_nk_pc[] =	{ "�\", "NK", NULL, };
static const char *indata_ng_pc[] =	{ "�S", "NG", NULL, };
static const char *indata_ou_pc[] =	{ "��", "��", "OU", "GY", NULL, };
static const char *indata_um_pc[] =	{ "�n", "UM", NULL, };
static const char *indata_ry_pc[] =	{ "��", "��", "RY", NULL, };
static const char *indata_16_pc[] =	{ NULL, };
static const char *indata_rock[] =	{ "��", "��", "RK", NULL, };
static const char *indata_hole[] =	{ "��", "��", "HL", NULL, };
const char **indata_pc[19] = {
	indata_00_pc, indata_fu_pc, indata_ky_pc, indata_ke_pc,
	indata_gi_pc, indata_ki_pc, indata_ka_pc, indata_hi_pc,
	indata_08_pc, indata_to_pc, indata_ny_pc, indata_nk_pc,
	indata_ng_pc, indata_ou_pc, indata_um_pc, indata_ry_pc,
	indata_16_pc, indata_rock,  indata_hole,
};
const char *graph_pc[] = {
	NULL, "��", "��", "�j", "��", "��", "�p", "��",
	"��", "��", "��", "�\", "�S", "��", "�n", "��",
	"��", "��", "��",
};
const char *graph_mv_pc[] = {
	NULL, "��", "��", "�j", "��", "��", "�p", "��",
	"��", "��", "��", "�\", "�S", "��", "�n", "��",
};
const char *graph_hi_pc[] = {
	" �E", " ��", " ��", " �j", " ��", " ��", " �p", " ��",
	" ��", " ��", " ��", " �\", " �S", " ��", " �n", " ��",
};
const char *graph_yo_pc[] = {
	" �E", "v��", "v��", "v�j", "v��", "v��", "v�p", "v��",
	"v��", "v��", "v��", "v�\", "v�S", "v��", "v�n", "v��",
};
const char *graph_ex_pc[] = {
	" �E", " ��", " ��", " ��", " ��", " ��", " ��", " ��",
};
const char *file_num[] = {
	NULL, "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
const char *rank_num[] = {
	NULL, "1", "2", "3", "4", "5", "6", "7", "8", "9",
};
const char *board_rank_num[] = {
	NULL, "��", "��", "�O", "�l", "��", "�Z", "��", "��", "��",
};

