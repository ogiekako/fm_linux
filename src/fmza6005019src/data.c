// zaco/data.c -- global data of FM
// Copyright(c) KAMINA, Jiro 2013-2014. All rights reserved.
// $Header: C:/project/fm/src/6xx/za/RCS/data.c 1.18 2014/08/14 02:50:46 budai10 Exp budai10 $

#include	<stdio.h>
#include	<time.h>
#include	"fm.h"
#include	"piece.h"

// ä¬ã´ïœêî
ulonglong_t cond_flag = 0x0000000000000000ULL;
ulong_t seed = 0UL;				// seed of pseudorandom number
ulong_t strategy = 0x00000000UL;
ulong_t hint = 0x00000000UL;
ulong_t rule_flag = 0x00000000UL;
ushort_t limit_depth = 0;		// given depth
ushort_t max_limit_depth;		// max. mate depth
ushort_t min_limit_depth;		// start mate depth on give ranges
char title[TITLESIZE];
char in_file[FNAMEMAX + 1] = "(stdin)";
char out_file[FNAMEMAX + 1] = "(stdout)";
char restart_file[FNAMEMAX + 1] = "FM000.FMS";

// ÉIÉvÉVÉáÉìä¬ã´ïœêî
char *arg_x = NULL;
long solution_limit = 10L;		// limit # of solution to output
#if defined(_M_X64)
long long hi_eh_lim = NOEHDEFAULT;
long long yo_eh_lim = NOEHDEFAULT;
long long yo_eh2_lim = NOEHDEFAULT;
#else
long hi_eh_lim = NOEHDEFAULT;
long yo_eh_lim = NOEHDEFAULT;
long yo_eh2_lim = NOEHDEFAULT;
#endif
long no_hi_eh_head = 0L;
long no_yo_eh_head = 0L;
int free_sph_count = 0;
ushort_t trace_limit = 0;
char mate_position = NOP;		// 0: no condition
char mate_counter_position = NOP;
uchar_t init_phase_state = 0x00;
uchar_t mate_print = ON;
uchar_t brief_output = 0;		// { 0, 1, 2, 3, }
uchar_t gentle = ON;
uchar_t hi_eh_given = OFF;
uchar_t yo_eh_given = OFF;
uchar_t yo_eh2_given = OFF;
uchar_t dancing = OFF;
uchar_t loose = OFF;
uchar_t verbose = OFF;
uchar_t debug = 0x00;

// çÏã∆ïœêî
FILE *inFp = NULL;				// input file
FILE *fileFp = NULL;			// output file
char *mate_buf;
phase_t *phase = NULL;
move_t *last_move = NULL;		// last (displaying or at stopping) moves
time_t total_elapse = 0LL;		//(S)
time_t start_time = 0LL;
time_t restart_time = 0LL;
time_t last_elapse = 0LL;		// elapse time at calling mile_stone() last
time_t base_elapse = 0LL;		// elapse time at restarting
long long call_move_count = 0LL;
long long call_move_leaf_count = 0LL;
long long call_analyze_count = 0LL;
long long call_analyze_leaf_count = 0LL;
long solution_count = 0L;		//(S)
long phase_count = 0L;			//(S) # of phases tested
long phase_1M = 0L;				//(S) # of phases tested in 1,000,000
long save_count = 0L;
long save_1M = 0L;
long check_point = 100000L;		//(S) next
long check_interval = 100000L;	//(S) interval seconds for displaying
ushort_t depth = 0;				// current depth
ushort_t xtra_depth = 30;		// extra depth
phase_t init_phase;
uchar_t all_used_pc_count[RAW_PC_END];
uchar_t hi_used_pc_count[RAW_PC_END];
uchar_t yo_used_pc_count[RAW_PC_END];
char inbuf[PROBSIZE];

// ÉIÉvÉVÉáÉìçÏã∆ïœêî
FILE *restartFp = NULL;			// restart file
seq_phase_hash_t *hi_sph_head[NOSPHHEAD]; // sph: same phase hash
seq_phase_hash_t *yo_sph_head[NOSPHHEAD];
seq_phase_hash_t *sph_cell;
escape_hash_t **hi_eh_head;		// sia: same phase in all
escape_hash_t **yo_eh_head;
escape_hash_t *hi_eh_free = NULL;
escape_hash_t *yo_eh_free = NULL;
escape_hash_t *hi_eh_cell = NULL;
escape_hash_t *yo_eh_cell = NULL;
mate_move_t *mm_free = NULL;
mate_move_t *mm_root = NULL;
uchar_t restarting = OFF;
uchar_t print_2nd_phase = OFF;

// å≈íËÉfÅ[É^
phase_t null_phase = {
	0x00000000,	// hash_hi_pieces
	0x00000000,	// hash_yo_pieces
	0x00000000,	// hash_hi_hands
	{			// hi_hand
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{			// yo_hand
		0, 18, 4, 4, 4, 4, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	0x00,		// state
	0x00,		// check_state
	NOP,		// hi_ou_pos
	NOP,		// yo_gy_pos
	{			// struct move
		NUL, NUL, NOP, NOP, NOP, NOP, NOP, 0x00, 0x00,
	},
	{			// board
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
	},
	{			// fire
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	},
	{			// fpc_board
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
		SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM, SQ_ROOM,
	},
};

move_t null_move = {
	NUL,	// pc (piece)
	NUL,	// fpc (fire-piece)
	NOP,	// to
	NOP,	// from
	NOP,	// reborn
	NOP,	// to_1st
	NOP,	// reborn_1st
	0x00,	// flag
	0x00,	// flag2
};

char fpc_pos_back[81] = {
	1, 2, 3, 4, 5, 6, 7, 8, NOP,
	10, 11, 12, 13, 14, 15, 16, 17, NOP,
	19, 20, 21, 22, 23, 24, 25, 26, NOP,
	28, 29, 30, 31, 32, 33, 34, 35, NOP,
	37, 38, 39, 40, 41, 42, 43, 44, NOP,
	46, 47, 48, 49, 50, 51, 52, 53, NOP,
	55, 56, 57, 58, 59, 60, 61, 62, NOP,
	64, 65, 66, 67, 68, 69, 70, 71, NOP,
	73, 74, 75, 76, 77, 78, 79, 80, NOP,
};

char fpc_pos_forward[81] = {
	NOP, 0, 1, 2, 3, 4, 5, 6, 7,
	NOP, 9, 10, 11, 12, 13, 14, 15, 16,
	NOP, 18, 19, 20, 21, 22, 23, 24, 25,
	NOP, 27, 28, 29, 30, 31, 32, 33, 34,
	NOP, 36, 37, 38, 39, 40, 41, 42, 43,
	NOP, 45, 46, 47, 48, 49, 50, 51, 52,
	NOP, 54, 55, 56, 57, 58, 59, 60, 61,
	NOP, 63, 64, 65, 66, 67, 68, 69, 70,
	NOP, 72, 73, 74, 75, 76, 77, 78, 79,
};

char *fpc_pos_hi_annan = fpc_pos_back;
char *fpc_pos_yo_annan = fpc_pos_forward;
char *fpc_pos_hi_anhoku = fpc_pos_forward;
char *fpc_pos_yo_anhoku = fpc_pos_back;
char *fpc_pos_hi_taimen = fpc_pos_forward;
char *fpc_pos_yo_taimen = fpc_pos_back;
char *fpc_pos_hi_haimen = fpc_pos_back;
char *fpc_pos_yo_haimen = fpc_pos_forward;
char *yo_hi_ctrl_taimen = fpc_pos_forward;
char *hi_yo_ctrl_taimen = fpc_pos_back;
char *yo_hi_ctrl_haimen = fpc_pos_back;
char *hi_yo_ctrl_haimen = fpc_pos_forward;

quote_mark_t quote_mark[] = {
	{ "\007\007", "\007\007", },
	{ "\"", "\"", },
	{ "Åu", "Åv", },
	{ "Åw", "Åx", },
	{ NULL, NULL, },
};

char *kanji_num[9] = {
	"àÍ", "ìÒ", "éO", "él", "å‹", "òZ", "éµ", "î™", "ã„",
};
char *zen_num[9] = {
	"ÇP", "ÇQ", "ÇR", "ÇS", "ÇT", "ÇU", "ÇV", "ÇW", "ÇX",
};

