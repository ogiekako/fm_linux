// nfm/nfm/data.c -- global data
// Copyright(c) KAMINA Jiro 2006-2010.  All rights reserved.
// $Header$

#include	<stdio.h>
#include	<time.h>
#include	<setjmp.h>
#include	"nfm.h"

// FILE pointers
FILE *inFp;
FILE *fileFp;

// ポインタデータ
mseqhead_t *mseq_head;
mseq_t **hashhead_check;
mseq_t **hashhead_escape;
mseq_t *msp_free = NULL;
mseq_t *msp_start_last;
mseq_t *msp_givenmate;
board_t *bdp_free = NULL;
void (* fm_attach_mseq)(mseq_t *);

// 数値データ
double comp_hash_check_found = 0;
double comp_hash_check_notfound = 0;
double comp_hash_escape_found = 0;
double comp_hash_escape_notfound = 0;
time_t start_time;
time_t total_elapse;
size_t mseq_inc_memsize = sizeof(mseq_t) * DEFAULT_MSEQ_INC_COUNT;
size_t mseq_max_memsize;
size_t board_inc_memsize = sizeof(board_t) * DEFAULT_BOARD_INC_COUNT;
size_t board_max_memsize;
size_t free_memsize;
size_t real_memsize = 0xffffffff;
size_t virtual_memsize = 0xffffffff;
long phase_count;
long check_point;
long check_interval = 10000;
long mseq_count = 0;
long mseq_inc_count = DEFAULT_MSEQ_INC_COUNT;
long mseq_max_count = -1;
long mseqpool_count = 0;
long mseq_reuse_count = 0;
long hashhead_count = DEFAULT_HASHHEAD_COUNT;
long hashhead_count_1 = DEFAULT_HASHHEAD_COUNT - 1;
long board_count = 0;
long board_inc_count = DEFAULT_BOARD_INC_COUNT;
long board_max_count = -1;
long boardpool_count = 0;
long depth_limit;
long depth_start;
long test_hash_check_count = 0;
long found_hash_check_count = 0;
long notfound_hash_check_count = 0;
long test_hash_escape_count = 0;
long found_hash_escape_count = 0;
long notfound_hash_escape_count = 0;
int sc_print_chesstyle = OFF;
int sc_print_debug = OFF;
int sc_print_endfig = OFF;
int sc_print_hashinfo = OFF;
int sc_print_interrupted = 0;
int sc_print_loosemate = OFF;
int sc_print_matefig = OFF;
int sc_print_notdisplay = OFF;
int sc_print_tradstyle = OFF;
int sc_print_startfig = OFF;
int sc_print_verbose = OFF;
int sc_env_autostart = OFF;
int sc_tact_freemseq = OFF;
int sc_tact_givenmatepath = OFF;
int sc_tact_sameaslesspc = OFF;
int sc_tact_sameasmorepc = OFF;
int sc_tact_sameaslessfu = OFF;
int sc_tact_sameasmorefu = OFF;
int sc_tact_freesamemseq = OFF;
int sc_tact_usextrapc = OFF;
int sc_tact_reusemseq = OFF;
int save_sc_tact_freemseq;
int save_sc_tact_sameaslesspc;
int save_sc_tact_sameasmorepc;
int save_sc_tact_sameaslessfu;
int save_sc_tact_sameasmorefu;
int mate_count;
int flag_loose_mate;
int flag_docheck;
int mask_targetdepth;
int depth_rsv;
int str_mate_depth;
int one_line_title_limit;
int retry;
ushort_t rule;
ushort_t cond;

// 文字列データ
char in_file[PATHMAX];
char out_file[PATHMAX];
char title[256];
char inbuf[BUFSIZE];
char outbuf[BUFSIZE];
char editbuf[EWASIZE];

// 構造体データ
mseq_t mseq_prob;
board_t board_prob;
mseq_t mseq_target;
board_t board_target;

// 定数データ
const uchar_t board_void[BOARD_SIZE] = {
//           一       二       三       四       五       六       七       八       九
	SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB,
// １
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ２
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ３
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ４
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ５
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ６
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ７
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ８
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// ９
	SQ_OofB, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
// 10
	SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB, SQ_OofB,
	SQ_OofB,
};
const uchar_t hand_void[HAND_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, };
const uchar_t yo_hand_ini[HAND_SIZE] = { 0, 18, 4, 4, 4, 4, 2, 2, };
const mseq_t mseq_void = { NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
const mseqhead_t mseqhead_void = { NULL, 0, 0, 0, 0, 0, };
static const int vec_void[] = { 0, };
static const int vec_ou[] = { -11, -10, -9, -1, 1, 9, 10, 11, 0, };
static const int vec_ka[] = { -11, -9, 9, 11, 0, };
static const int vec_hi[] = { -10, -1, 1, 10, 0, };
static const int vec_hi_fu[] = { -1, 0, };
static const int vec_hi_ke[] = { -12, 8, 0, };
static const int vec_hi_gi[] = { -11, -9, -1, 9, 11, 0, };
static const int vec_hi_ki[] = { -11, -10, -1, 1, 9, 10, 0, };
static const int vec_yo_fu[] = { 1, 0, };
static const int vec_yo_ke[] = { -8, 12, 0, };
static const int vec_yo_gi[] = { -11, -9, 1, 9, 11, 0, };
static const int vec_yo_ki[] = { -10, -9, -1, 1, 10, 11, 0, };
const int *vec_hi_step[PC_END] = {
	vec_void,  vec_hi_fu, vec_void,  vec_hi_ke, vec_hi_gi, vec_hi_ki, vec_void,  vec_void,
	vec_void,  vec_hi_ki, vec_hi_ki, vec_hi_ki, vec_hi_ki, vec_ou,    vec_hi,    vec_ka,
};
const int *vec_hi_run[PC_END] = {
	vec_void,  vec_void,  vec_hi_fu, vec_void,  vec_void,  vec_void,  vec_ka,    vec_hi,
	vec_void,  vec_void,  vec_void,  vec_void,  vec_void,  vec_void,  vec_ka,    vec_hi,
};
const int *vec_yo_step[PC_END] = {
	vec_void,  vec_yo_fu, vec_void,  vec_yo_ke, vec_yo_gi, vec_yo_ki, vec_void,  vec_void,
	vec_void,  vec_yo_ki, vec_yo_ki, vec_yo_ki, vec_yo_ki, vec_ou,    vec_hi,    vec_ka,
};
const int *vec_yo_run[PC_END] = {
	vec_void,  vec_void,  vec_yo_fu, vec_void,  vec_void,  vec_void,  vec_ka,    vec_hi,
	vec_void,  vec_void,  vec_void,  vec_void,  vec_void,  vec_void,  vec_ka,    vec_hi,
};
const int hi_step_vec[11] = {
	-1, -12, 8, -11, 9, -10, 10, -9, 11, 1, 0,
};
const int yo_step_vec[11] = {
	1, -8, 12, -9, 11, -10, 10, -11, 9, -1, 0,
};
const int run_vec[9] = {
	-1, -11, 9, -10, 10, -9, 11, 1, 0,
};
static const int pc_set_step_79[PC_END] = {	// 銀,金,王,と,杏,圭,全,龍
	0, 0, 0, 0, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 0, 1,
};
static const int pc_set_step_46[PC_END] = {	// 金,王,と,杏,圭,全,馬
	0, 0, 0, 0, 0, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
};
static const int pc_set_step_13[PC_END] = {	// 銀,王,龍
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 1,
};
static const int pc_set_step_8[PC_END] = {	// 歩,銀,金,王,と,杏,圭,全,馬
	0, 1, 0, 0, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
};
static const int pc_set_step_2[PC_END] = {	// 金,王,と,杏,圭,全,馬
	0, 0, 0, 0, 0, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
};
static const int pc_set_step_ke[PC_END] = {	// 桂
	0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};
const int *hi_step_pc[10] = {
	pc_set_step_8,		//  -1 歩,銀,金,王,と,杏,圭,全,馬
	pc_set_step_ke,		// -12 桂
	pc_set_step_ke,		//   8 桂
	pc_set_step_79,		// -11 銀,金,王,と,杏,圭,全,龍
	pc_set_step_79,		//   9 銀,金,王,と,杏,圭,全,龍
	pc_set_step_46,		// -10 金,王,と,杏,圭,全,馬
	pc_set_step_46,		//  10 金,王,と,杏,圭,全,馬
	pc_set_step_13,		//  -9 銀,王,龍
	pc_set_step_13,		//  11 銀,王,龍
	pc_set_step_2,		//   1 金,王,と,杏,圭,全,馬
};
const int *yo_step_pc[10] = {
	pc_set_step_8,		//   1 歩,銀,金,王,と,杏,圭,全,馬
	pc_set_step_ke,		//  -8 桂
	pc_set_step_ke,		//  12 桂
	pc_set_step_79,		//  -9 銀,金,王,と,杏,圭,全,龍
	pc_set_step_79,		//  11 銀,金,王,と,杏,圭,全,龍
	pc_set_step_46,		// -10 金,王,と,杏,圭,全,馬
	pc_set_step_46,		//  10 金,王,と,杏,圭,全,馬
	pc_set_step_13,		// -11 銀,王,龍
	pc_set_step_13,		//   9 銀,王,龍
	pc_set_step_2,		//  -1 金,王,と,杏,圭,全,馬
};
static const int pc_set_run_79[PC_END] = {	// 角,馬
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
};
static const int pc_set_run_246[PC_END] = {	// 飛,龍
	0, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1,
};
static const int pc_set_run_13[PC_END] = {	// 角,馬
	0, 0, 0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 0,
};
static const int pc_set_run_8[PC_END] = {	// 香,飛,龍
	0, 0, 1, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 1,
};
const int *hi_run_pc[8] = {
	pc_set_run_8,		//  -1 香,飛,龍
	pc_set_run_79,		// -11 角,馬
	pc_set_run_79,		//   9 角,馬
	pc_set_run_246,		// -10 飛,龍
	pc_set_run_246,		//  10 飛,龍
	pc_set_run_13,		//  -9 角,馬
	pc_set_run_13,		//  11 角,馬
	pc_set_run_246,		//   1 飛,龍
};
const int *yo_run_pc[8] = {
	pc_set_run_246,		//  -1 飛,龍
	pc_set_run_13,		// -11 角,馬
	pc_set_run_13,		//   9 角,馬
	pc_set_run_246,		// -10 飛,龍
	pc_set_run_246,		//  10 飛,龍
	pc_set_run_79,		//  -9 角,馬
	pc_set_run_79,		//  11 角,馬
	pc_set_run_8,		//   1 香,飛,龍
};
const int *hi_runv_pc[23] = {
	pc_set_run_79,		// -11 角,馬
	pc_set_run_246,		// -10 飛,龍
	pc_set_run_13,		//  -9 角,馬
	NULL,				//  -8
	NULL,				//  -7
	NULL,				//  -6
	NULL,				//  -5
	NULL,				//  -4
	NULL,				//  -3
	NULL,				//  -2
	pc_set_run_8,		//  -1 香,飛,龍
	NULL,				//   0
	pc_set_run_246,		//   1 飛,龍
	NULL,				//   2
	NULL,				//   3
	NULL,				//   4
	NULL,				//   5
	NULL,				//   6
	NULL,				//   7
	NULL,				//   8
	pc_set_run_79,		//   9 角,馬
	pc_set_run_246,		//  10 飛,龍
	pc_set_run_13,		//  11 角,馬
};
const int *yo_runv_pc[23] = {
	pc_set_run_13,		// -11 角,馬
	pc_set_run_246,		// -10 飛,龍
	pc_set_run_79,		//  -9 角,馬
	NULL,				//  -8
	NULL,				//  -7
	NULL,				//  -6
	NULL,				//  -5
	NULL,				//  -4
	NULL,				//  -3
	NULL,				//  -2
	pc_set_run_246,		//  -1 飛,龍
	NULL,				//   0
	pc_set_run_8,		//   1 香,飛,龍
	NULL,				//   2
	NULL,				//   3
	NULL,				//   4
	NULL,				//   5
	NULL,				//   6
	NULL,				//   7
	NULL,				//   8
	pc_set_run_13,		//   9 角,馬
	pc_set_run_246,		//  10 飛,龍
	pc_set_run_79,		//  11 角,馬
};

static const ulong_t bphh_fu[] = {
	0x00000000, 0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006, 0x00000007,
	0x00000008, 0x00000009, 0x0000000a, 0x0000000b, 0x0000000c, 0x0000000d, 0x0000000e, 0x0000000f,
	0x00000010, 0x00000011, 0x00000012, 0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017,
	0x00000018, 0x00000019, 0x0000001a, 0x0000001b, 0x0000001c, 0x0000001d, 0x0000001e, 0x0000001f,
};
static const ulong_t bphh_ky[] = {
	0x00000000, 0x00000100, 0x00000200, 0x00000300, 0x00000400, 0x00000500, 0x00000600, 0x00000700,
};
static const ulong_t bphh_ke[] = {
	0x00000000, 0x00001000, 0x00002000, 0x00003000, 0x00004000, 0x00005000, 0x00006000, 0x00007000,
};
static const ulong_t bphh_gi[] = {
	0x00000000, 0x00010000, 0x00020000, 0x00030000, 0x00040000, 0x00050000, 0x00060000, 0x00070000,
};
static const ulong_t bphh_ki[] = {
	0x00000000, 0x00100000, 0x00200000, 0x00300000, 0x00400000, 0x00500000, 0x00600000, 0x00700000,
};
static const ulong_t bphh_ka[] = {
	0x00000000, 0x01000000, 0x02000000, 0x03000000, 0x04000000, 0x05000000, 0x06000000, 0x07000000,
};
static const ulong_t bphh_hi[] = {
	0x00000000, 0x10000000, 0x20000000, 0x30000000, 0x40000000, 0x50000000, 0x60000000, 0x70000000,
};
const ulong_t *bphh[] = {
	NULL, bphh_fu, bphh_ky, bphh_ke, bphh_gi, bphh_ki, bphh_ka, bphh_hi,
};
const ulong_t bmhh[] = {
	0, M_HH_FU, M_HH_KY, M_HH_KE, M_HH_GI, M_HH_KI, M_HH_KA, M_HH_HI,
};

