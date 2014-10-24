// 6aco/peice.c -- FM piece data
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/piece.h 1.34 2014/08/17 02:13:44 budai10 Exp budai10 $

#if !defined(PIECE_H)
#define PIECE_H

#include	"fm.h"

// ��r�b�g
#define	B_PC_PROMOTED		(0x10)	// ����

// ��R�[�h
#define	NUL					(0x00)						// ���
#define	ROOM				(0x00)						// ��
#define	PC_FU				(0x01)						// ��
#define	PC_KY				(0x02)						// ��
#define	PC_KE				(0x03)						// �j
#define	PC_GI				(0x04)						// ��
#define	PC_KI				(0x05)						// ��
#define	PC_KA				(0x06)						// �p
#define	PC_HI				(0x07)						// ��
#define	PC_F1				(0x08)						// �t�F�A���[��P
#define	PC_F2				(0x09)						// �t�F�A���[��Q
#define	PC_F3				(0x0a)						// �t�F�A���[��R
#define	PC_F4				(0x0b)						// �t�F�A���[��S
#define	PC_F5				(0x0c)						// �t�F�A���[��T
#define	PC_F6				(0x0d)						// �t�F�A���[��U
#define	PC_F7				(0x0e)						// �t�F�A���[��V
#define	PC_OU				(0x0f)						// ��/��
#define	PIT					(ROOM | B_PC_PROMOTED)		// �v�i�e���L���A���߁j
#define	PC_TO				(PC_FU | B_PC_PROMOTED)		// ��
#define	PC_NY				(PC_KY | B_PC_PROMOTED)		// ��
#define	PC_NK				(PC_KE | B_PC_PROMOTED)		// �\
#define	PC_NG				(PC_GI | B_PC_PROMOTED)		// �S
#define	ROCK				(PC_KI | B_PC_PROMOTED)		// �΁i�e���L���A�s���߁j
#define	PC_UM				(PC_KA | B_PC_PROMOTED)		// �n
#define	PC_RY				(PC_HI | B_PC_PROMOTED)		// ��
#define	PC_P1				(PC_F1 | B_PC_PROMOTED)		// �t�F�A���[����P
#define	PC_P2				(PC_F2 | B_PC_PROMOTED)		// �t�F�A���[����Q
#define	PC_P3				(PC_F3 | B_PC_PROMOTED)		// �t�F�A���[����R
#define	PC_P4				(PC_F4 | B_PC_PROMOTED)		// �t�F�A���[����S
#define	PC_P5				(PC_F5 | B_PC_PROMOTED)		// �t�F�A���[����T
#define	PC_P6				(PC_F6 | B_PC_PROMOTED)		// �t�F�A���[����U
#define	PC_P7				(PC_F7 | B_PC_PROMOTED)		// �t�F�A���[����V
#define	RAW_PC_END			(16)
#define	PC_END				(32)
#define	PC_F0				(PC_F1 - 1)
#define	PC_P0				(PC_P1 - 1)

// ��U��r�b�g
#define	B_PC_HITHER			(0x40)		// �����̒l�́A��f�[�^�ƘA�g���Ă���̂�
#define	B_PC_YONDER			(0x20)		//   �ύX���ɂ͓����ɋ�f�[�^���Ή����K�v
#define	BM_PC_TURN			(0x60)

// �U����R�[�h
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

// �����R�[�h
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

// �����R�[�h
#define	SQ_ROOM				(ROOM)
#define	SQ_ROCK				(ROCK)
#define	SQ_PIT				(PIT)

// �ʏ��ԍ�
#define	NP_FU				(1)		// ��
#define	NP_KY				(2)		// ��
#define	NP_KE				(3)		// �j
#define	NP_GI				(4)		// ��
#define	NP_KI				(5)		// ��
#define	NP_KA				(6)		// �p
#define	NP_HI				(7)		// ��
#define	NP_OU				(8)		// ��/��
#define	NP_TO				(9)		// ��
#define	NP_NY				(10)	// ��
#define	NP_NK				(11)	// �\
#define	NP_NG				(12)	// �S
#define	NP_UM				(13)	// �n
#define	NP_RY				(14)	// ��
#define	SP_ROOM				(15)	// �E/��
#define	SP_ROCK				(16)	// ��/��
#define	SP_PIT				(17)	// ��/�v
// �ʏ��ԍ��I����
#define	NP_END				(18)
// �t�F�A���[��ԍ�
#define	FP_START			(20)
// �t�F�A���[�`�F�X - ���[�p�[��
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
// �t�F�A���[�`�F�X - ���C�_�[��
#define	FP_QU				(41)	// (0, 1)+(1, 1)-Rider Queen
#define	FP_NR				(42)	// (1, 2)-Rider Nightrider
#define	FP_CR				(43)	// (1, 3)-Rider Camelrider
#define	FP_ZR				(44)	// (2, 3)-Rider Zebrarider
#define	FP_RS				(48)	// (1, 2)-CircleRider Rose
// �t�F�A���[�`�F�X - �����i���[�p�[�A���C�_�[�j��
#define	FP_EM				(50)	// (0, 1)-Rider+(1, 2)-Leaper Empress
#define	FP_PR				(49)	// (1, 1)-Rider+(1, 2)-Leaper Princess
// �t�F�A���[�`�F�X - �z�b�p�[��
#define	FP_GH				(51)	// Grasshopper
#define	FP_LO				(52)	// Locust
#define	FP_LI				(53)	// Lion
#define	FP_RO				(54)	// Kangaroo
#define	FP_NE				(55)	// Non-stop equihopper
#define	FP_EH				(56)	// Equihopper
#define	FP_SP				(57)	// Sparrow
#define	FP_EA				(58)	// Eagle
// �t�F�A���[�`�F�X - �ۊ���
#define	FP_PA				(61)	// PaO
#define	FP_VA				(62)	// Vao
#define	FP_LE				(63)	// Leo
#define	FP_SH				(64)	// Shan
#define	FP_MA				(65)	// Mao
#define	FP_MO				(66)	// Moa
// ��������
#define	FP_SS				(71)	// ���q
#define	FP_SZ				(72)	// ����
#define	FP_KR				(73)	// �i��
#define	FP_HO				(74)	// �P��
#define	FP_MK				(75)	// �ӌ�
#define	FP_DO				(76)	// ����
#define	FP_MH				(77)	// �ҕ^
#define	FP_CN				(78)	// ���l
#define	FP_HN				(79)	// �z��
#define	FP_JG				(80)	// �G�s
#define	FP_OG				(81)	// ���s
#define	FP_HE				(82)	// ����
#define	FP_TS				(83)	// ���q <- ����
#define	FP_HR				(84)	// �� <- �ӌ�
#define	FP_HJ				(85)	// ��h <- ����
#define	FP_TK				(86)	// �p�� <- ���n
#define	FP_HG				(87)	// �� <- �G�s
#define	FP_HC				(88)	// �z�� <- ���s
#define	FP_KG				(89)	// �~�� <- ����
#define	FP_HK				(90)	// ���� <- ����
#define	FP_CHI				(91)	// �������F��
#define	FP_CKA				(92)	// �������F�p
#define	FP_CKI				(93)	// �������F��
#define	FP_CGI				(94)	// �������F��
#define	FP_CRY				(95)	// �������F��
#define	FP_CUM				(96)	// �������F�n
#define	FP_CKY				(97)	// �������F��
#define	FP_PSS				(98)	// �t�i�����q�j
#define	FP_PHN				(99)	// �{�i���z���j
#define	FP_PRY				(100)	// ���i�������j
#define	FP_PUM				(101)	// �}�i�����n�j
#define	FP_PHI				(102)	// �q�i����ԁj
#define	FP_PKA				(103)	// �N�i���p�s�j
#define	FP_PJG				(104)	// ���i���G�s�j
#define	FP_POG				(105)	// ���i�����s�j
#define	FP_PSZ				(106)	// 況i�����ہj
// �׏�����
#define	FP_THO				(111)	// �Q
#define	FP_TTR				(112)	// ��
#define	FP_TKJ				(113)	// �
#define	FP_TLU				(114)	// ���i���G�j
#define	FP_TRU				(115)	// ���i�E�G�j
#define	FP_TTK				(116)	// ��
#define	FP_TKT				(117)	// �G�i���܂����j<- ��
#define	FP_TTB				(118)	// ��
#define	FP_TKR				(119)	// �� <- ��
// �叫����
#define	FP_DHR				(121)	// �� -> �����i�����j
#define	FP_DMG				(122)	// �ҋ� -> �����i�����j
#define	FP_DSC				(123)	// �p�� -> �����i�����j
#define	FP_DMJ				(124)	// �L�n -> �����i�����j
#define	FP_DNK				(125)	// �����i�����j <- �򗴁A�ҋ��A�p���A�L�n
// ���d��叫����
#define	FP_DBJ				(126)	// 崎�
// �t�F�A���[��ԍ��I����
#define	FP_END				(131)
// �g���t�F�A���[��ԍ�
#define	FP_EX1				(131)	// �g���t�F�A���[��P
#define	FP_EX2				(132)	// �g���t�F�A���[��Q
#define	FP_EX3				(133)	// �g���t�F�A���[��R
#define	FP_EX4				(134)	// �g���t�F�A���[��S
#define	FP_EX5				(135)	// �g���t�F�A���[��T
#define	FP_EX6				(136)	// �g���t�F�A���[��U
#define	FP_EX7				(137)	// �g���t�F�A���[��V
// �g���t�F�A���[��ԍ��I����
#define	FP_EXEND			(138)

// board masks
#define	BM_SQ_PC			(0x1f)
#define	BM_SQ_RAW_PC		(0x0f)

// piece attribute bits & masks
#define	B_PA_DEFINED		(0x80000000)	// ��`��
#define	B_PA_PIECE   		(0x40000000)	// ��
#define	B_PA_ROYAL			(0x20000000)	// �������iRoyal�����j
#define	B_PA_PROMOTABLE		(0x08000000)	// ������
#define	B_PA_PROMOTABLE_FE	(0x04000000)	// ���ւ̂Ƃ��A�l����Ő����� (For Escape)
#define	BM_PA_PROMOTABLE	(0x0c000000)	// �l����Ő�����
#define	B_PA_PLACABLE  		(0x00800000)	// �łĂ��i���Ďg�p�ł����B����Ɏg�p�j
#define	B_PA_REUSABLE  		(0x00400000)	// �Ďg�p�ł����i����Ɏg�p�j
#define	B_PA_SAME_FACE 		(0x00010000)	// ������ʂ���
#define	B_PA_CONSTRAINT_SQ	(0x00008000)	// �ړ���i�Ő�j���񂠂�
#define	B_PA_CONSTRAINT_2F	(0x00004000)	// ������񂠂�
#define	B_PA_CONSTRAINT_3T	(0x00002000)	// �O�����񂠂�
#define	BM_PA_CONSTRAINT	(0x0000e000)	// ����
#define	B_PA_CONSTRAINT_PM	(0x00001000)	// �ŋl (Place Mate) ����i�ŕ��l����A�ŉ��l����j
#define	B_PA_PASSED			(0x00000800)	// ���ߐ�����i��ʂ蔲������j

// piece move-style bits & masks
#define	B_MS_LEAP			(0x80000000)	// ���сi����̗����Ȃ��ړ��j
#define	B_MS_SHISHI_LEAP	(0x40000000)	// ���q���̒��сi����̗����Ȃ��ړ��j
#define	B_MS_LONG_LEAP		(0x10000000)	// �����сi���ѕ⑫�B55���璵�ׂȂ�����������ꍇ�j
#define	BM_MS_LEAP_FAMILY	(0xc0000000)	// ���ьn
#define	B_MS_STEP			(0x08000000)	// ���X�ړ��i����̗����ړ��B�ۊ��̏ہj
#define	B_MS_MAO_STEP		(0x04000000)	// Mao���̒��X�i����̗����ړ��B�ۊ��̔n�j
#define	BM_MS_STEP_FAMILY	(0x0c000000)	// ���X�n
#define	B_MS_RUN			(0x00800000)	// ����
#define	B_MS_HOP			(0x00400000)	// Grasshopper move
#define	B_MS_DECAPT_HOP		(0x00200000)	// Locust move
#define	B_MS_PAO_JUMP		(0x00100000)	// Pao move
#define	B_MS_JUMP			(0x00080000)	// Lion move
#define	B_MS_JUMP_HOP		(0x00040000)	// Kangaroo move
#define	BM_MS_RUN_FAMILY	(0x00fc0000)	// ����n
#define	B_MS_REFL135		(0x00020000)	// Sparrow move
#define	B_MS_REFL90			(0x00010000)	// Eagle move
#define	B_MS_EQUI_JUMP		(0x00008000)	// Equihopper move
#define	B_MS_NS_EQUI_JUMP	(0x00004000)	// Non-stop Equihopper move
#define	BM_MS_HOP_FAMILY	(0x007fc000)	// Hop-jump�n
#define	B_MS_CIRCLE_RUN		(0x00002000)	// Rose move
#define	BM_MS_EXRUN_FAMILY	(0x00fc2000)	// �g������n

#define	EOP					(-1)	// �ʒu�̔z��̏I�[�L��
#define	NOP					(-1)	// �Տ�łȂ��ʒu�i�ʒu�̏����l�Ɏg�p�j

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
	int range;		// ���X�A���葖��A��]����̂Ƃ��w��
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
extern uchar_t *hi_to_board[PC_END][81];		// �U���ړ��惊�X�g
extern uchar_t *yo_to_board[PC_END][81];		// �U���ړ��惊�X�g
extern uchar_t *hi_close_check_board[81];
extern uchar_t *yo_close_check_board[81];
extern uchar_t *hi_remote_check_board[81];
extern uchar_t *yo_remote_check_board[81];
extern uchar_t *around_sq[81];					// ���̓��X�g
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
