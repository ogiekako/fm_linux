// nfm/nfm/getprob.c -- get problem data
// Copyright(c) KAMINA Jiro 2006-2007.  All rights reserved.
// $Header: getprob.cv  1.2  07/10/20 22:45:32  Exp $

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<mbstring.h>
#include	<ctype.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"
#include	"message.h"

static void
reset_problem(void)
{
	memset(title, '\0', sizeof(title));
	rule = 0;
	cond = 0;
	depth_limit = 0;
	depth_start = 0;
	memcpy(board_prob.board, board_void, sizeof(board_prob.board));
	memcpy(board_prob.hi_hand, hand_void, sizeof(board_prob.hi_hand));
	memcpy(board_prob.yo_hand, yo_hand_ini, sizeof(board_prob.yo_hand));
	memset(board_prob.fire, '\0', sizeof(board_prob.fire));
	board_prob.hi_ou_pos = 0;
	board_prob.yo_gy_pos = 0;
	board_prob.hi_ck_pos = 0;
	board_prob.yo_ck_pos = 0;
	memcpy(board_target.board, board_void, sizeof(board_target.board));
	memcpy(board_target.hi_hand, hand_void, sizeof(board_target.hi_hand));
	memcpy(board_target.yo_hand, yo_hand_ini, sizeof(board_target.yo_hand));
	memset(board_target.fire, '\0', sizeof(board_target.fire));
	board_target.hi_ou_pos = 0;
	board_target.yo_gy_pos = 0;
	board_target.hi_ck_pos = 0;
	board_target.yo_ck_pos = 0;
	return;
}

static int
get_rule_and_depth(char *buf)
{
	char *s, *t, *r;
	int rule_len;

	r = NULL;
	if (*buf == '\0' || *buf == '\x07') {
		return 1;
	}
	s = strchr(buf, '\x07');
	if (s != NULL) {
		*s = '\0';
		r = s;
	}
	fm_ltrim(buf);
	fm_rtrim(buf);
	fm_str_toupper(buf);
	if (EQUALS(buf, str[STR_SECT_END]) || EQUALS(buf, str[STR_SECT_END_E])
		|| EQUALZ(buf, "END") || EQUALZ(buf, "EXIT"))
	{
		return 1;
	}
	if (EQUALZ(buf, "H#@")) {
		rule = RULE_NFMHMA;
		rule_len = STR_SIZE_OF("H#@");
	} else if (EQUALZ(buf, "H#")) {
		rule = RULE_NFMHM;
		rule_len = STR_SIZE_OF("H#");
	} else if (EQUALZ(buf, "HS#@")) {
		rule = RULE_NFMHSMA;
		rule_len = STR_SIZE_OF("HS#@");
	} else if (EQUALZ(buf, "HS#")) {
		rule = RULE_NFMHSM;
		rule_len = STR_SIZE_OF("HS#");
	} else if (EQUALZ(buf, "SH#")) {
		rule = RULE_NFMSHM;
		rule_len = STR_SIZE_OF("SH#");
	} else if (EQUALZ(buf, "SER-#")) {
		rule = RULE_NFMSERM;
		rule_len = STR_SIZE_OF("SER-#");
	} else if (EQUALZ(buf, "H@")) {
		rule = RULE_NFMHA;
		rule_len = STR_SIZE_OF("H@");
	} else {
		goto error_exit;
	}
	depth_limit = fm_strtol(buf + rule_len, NULL, 10);
	if (depth_limit == 0) {
		goto error_exit;
	}
	t = strchr(buf, '/');
	if (t != NULL) {
		t++;
		for ( ; ; ) {
			fm_ltrim(t);
			if (EQUALS(t, str[STR_COND_PWC])) {
				t += strlen(str[STR_COND_PWC]);
				cond |= COND_PWC;
			} else if (EQUALS(t, str[STR_COND_NOPROM])) {
				t += strlen(str[STR_COND_NOPROM]);
				cond |= COND_NO_PROM;
			} else if (EQUALS(t, str[STR_COND_NOPROM_E])) {
				t += strlen(str[STR_COND_NOPROM_E]);
				cond |= COND_NO_PROM;
			} else {
				break;
			}
		}
	}
	t = strchr(buf, ':');
	if (t != NULL) {
		t++;
		fm_ltrim(t);
		strcpy(title, t);
	}
	memmove(buf, s + 1, strlen(s + 1) + 1);
	return 0;

error_exit:;
	if (r != NULL) {
		*r = '\n';
	}
	return -1;
}

static int
get_indata_pc(const char *s)
{
	int i;
	uchar_t pc;

	for (pc = PC_FU; pc < SQ_END; pc++) {
		for (i = 0; indata_pc[pc][i] != NULL; i++) {
			if (EQUALS(s, indata_pc[pc][i])) {
				return (pc);
			}
		}
	}
	return 0;
}

static void
del_space(char *buf)
{
	char *s;

	for (s = buf; *s != '\0';) {
		if (isspace((uchar_t)*s)) {
			memmove(s, s + 1, strlen(s + 1) + 1);
		} else if (EQUALZ(s, "　")) {
			memmove(s, s + STR_SIZE_OF("　"), strlen(s + STR_SIZE_OF("　")) + 1);
		} else {
			if (_ismbblead((uchar_t)*s)) {
				s += 2;
			} else {
				s++;
			}
		}
	}
	return;
}

static int
get_phase(char *buf, board_t *bdp)
{
	char *s, *t, *r;
	int pos;
	int n;
	uchar_t pc;

	r = NULL;
	del_space(buf);
	fm_str_toupper(buf);
	s = strchr(buf, '\x07');
	*s = '\0';
	r = s;
	for (s = buf; *s != '\0' && *s != '+'; s += 4) {
		if (!isdigit((uchar_t)*s) || *s == '0'
			|| !isdigit((uchar_t)*(s + 1)) || *(s + 1) == '0')
		{
			goto error_exit;
		}
		pos = POS(*s - '0', *(s + 1) - '0');
		if (pos < POS(1, 1) || pos > POS(9, 9) || !IS_ROOM(bdp->board[pos])) {
			goto error_exit;
		}
		pc = get_indata_pc(s + 2);
		if (pc < PC_FU || pc > PC_RY) {
			goto error_exit;
		}
		bdp->board[pos] = pc | B_PC_HITHER;
		if (pc == PC_OU) {
			bdp->hi_ou_pos = pos;
		} else {
			if (bdp->yo_hand[RAW_KIND(pc)] <= 0) {
				goto error_exit;
			}
			bdp->yo_hand[RAW_KIND(pc)]--;
		}
	}
	if (*s == '+') {
		for (s++; *s != '\0';) {
			pc = get_indata_pc(s);
			if (pc < PC_FU || pc > PC_RY) {
				goto error_exit;
			}
			n = atoi(s + 2);
			if (n < 0) {
				goto error_exit;
			}
			if (n == 0 && *(s + 2) != '0') {
				 n = 1;
			}
			if (bdp->yo_hand[RAW_KIND(pc)] < n) {
				goto error_exit;
			}
			bdp->hi_hand[pc] += n;
			bdp->yo_hand[RAW_KIND(pc)] -= n;
			for (s += 2; *s != '\0' && isdigit((uchar_t)*s); s++)
				;
		}
	}
	t = s + 1;
	s = strchr(t, '\x07');
	*s = '\0';
	for (s = t; *s != '\0' && *s != '+'; s += 4) {
		if (!isdigit((uchar_t)*s) || *s == '0'
			|| !isdigit((uchar_t)*(s + 1)) || *(s + 1) == '0')
		{
			goto error_exit;
		}
		pos = POS(*s - '0', *(s + 1) - '0');
		if (pos < POS(1, 1) || pos > POS(9, 9) || !IS_ROOM(bdp->board[pos])) {
			goto error_exit;
		}
		pc = get_indata_pc(s + 2);
		if (pc < PC_FU || pc > PC_RY) {
			goto error_exit;
		}
		bdp->board[pos] = pc | B_PC_YONDER;
		if (pc == PC_OU) {
			bdp->yo_gy_pos = pos;
		} else {
			if (bdp->yo_hand[RAW_KIND(pc)] <= 0) {
				goto error_exit;
			}
			bdp->yo_hand[RAW_KIND(pc)]--;
		}
	}
	if (*s == '+') {
		for (pc = PC_FU; pc <= PC_HI; pc++) {
			bdp->yo_hand[pc] = 0;
		}
		for (s++; *s != '\0';) {
			pc = get_indata_pc(s);
			if (pc < PC_FU || pc > PC_RY) {
				goto error_exit;
			}
			n = atoi(s + 2);
			if (n < 0) {
				goto error_exit;
			}
			if (n == 0 && *(s + 2) != '0') {
				 n = 1;
			}
			bdp->yo_hand[pc] += n;
			for (s += 2; *s != '\0' && isdigit((uchar_t)*s); s++)
				;
		}
	}
	memmove(buf, s + 1, strlen(s + 1) + 1);
	return 0;

error_exit:;
	if (r != NULL) {
		*r = '\n';
	}
	return -1;
}

static void
set_phase(board_t *bdp_to, board_t *bdp_from)
{
	memmove(bdp_to, bdp_from, sizeof(*bdp_from));
}

static int
set_extrapc(char *buf, board_t *bdp, board_t *bdq)
{
	char *s, *t;
	int pos;
	uchar_t pc;

	t = NULL;
	del_space(buf);
	fm_str_toupper(buf);
	s = strchr(buf, '\x07');
	*s = '\0';
	t = s;
	for (s = buf; *s != '\0'; s += 4) {
		if (!isdigit((uchar_t)*s) || *s == '0'
			|| !isdigit((uchar_t)*(s + 1)) || *(s + 1) == '0')
		{
			goto error_exit;
		}
		pos = POS(*s - '0', *(s + 1) - '0');
		if (pos < POS(1, 1) || pos > POS(9, 9) || !IS_ROOM(bdp->board[pos])) {
			goto error_exit;
		}
		pc = get_indata_pc(s + 2);
		if (IS_ROCK(pc)) {
			bdp->board[pos] = SQ_ROCK;
		} else if (IS_HOLE(pc)) {
			bdp->board[pos] = SQ_HOLE;
		} else {
			goto error_exit;
		}
		if (bdq != NULL) {
			if (!IS_ROOM(bdq->board[pos])) {
				goto error_exit;
			}
			if (IS_ROCK(pc)) {
				bdq->board[pos] = SQ_ROCK;
			} else {
				bdq->board[pos] = SQ_HOLE;
			}
		}
	}
	memmove(buf, s + 1, strlen(s + 1) + 1);
	return 0;

error_exit:;
	if (t != NULL) {
		*t = '\n';
	}
	return -1;
}

static int
test_targetmseq(mseq_t *msp_par)
{
	board_t *bdp_par;
	int p_f, pos;

	ASSERT(msp_par->BDP != NULL);
	bdp_par = msp_par->BDP;
	for (p_f = 1; p_f <= 9; p_f++) {
		for (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			if (IS_ANY_PC(bdp_par->board[pos])
				&& bdp_par->board[pos] != board_target.board[pos])
			{
				return RC_ESCAPE;
			}
		}
	}
	return RC_MATE;
}

static int
get_matemseq_chesstyle(mseq_t *msp, mseq_t *msp_par, char **sp, uchar_t turn)
{
	const int *vcp;
	char *s;
	int vec, v_f, v_r;
	uchar_t pc;

	s = *sp;
	msp->pc = get_indata_pc(s);
	if (msp->pc < PC_FU || msp->pc > PC_RY) {
		return STR_ERR_GIVENMATEPATH_PC;
	}
	msp->pc |= turn;
	s += 2;
	if (*s == '-') {
		msp->from = 0;
		s++;
	} else {
		if (*s < '1' || *s > '9' || *(s + 1) < '1' || *(s + 1) > '9') {
			return STR_ERR_GIVENMATEPATH_FROM;
		}
		msp->from = POS(*s - '0', *(s + 1) - '0');
		s += 2;
		if (*s == '-') {
			s++;
		} else if (*s == 'X') {
			msp->mv_flag |= B_MV_CAPTIT;
			s++;
			msp->capt = get_indata_pc(s);
			if (msp->capt < PC_FU || msp->capt > PC_RY) {
				return STR_ERR_GIVENMATEPATH_CAPT;
			}
			msp->capt |= (turn == B_PC_HITHER)? B_PC_YONDER: B_PC_HITHER;
			s += 2;
		} else {
			return STR_ERR_GIVENMATEPATH;
		}
	}
	if (*s < '1' || *s > '9' || *(s + 1) < '1' || *(s + 1) > '9') {
		return STR_ERR_GIVENMATEPATH_TO;
	}
	msp->to = POS(*s - '0', *(s + 1) - '0');
	if (msp->to == msp->from) {
		return STR_ERR_GIVENMATEPATH;
	}
	if (msp->from != 0) {
		v_f = FILE(msp->to) - FILE(msp->from);
		v_r = RANK(msp->to) - RANK(msp->from);
		vec = 0;
		if (v_f == 0) {
			vec = (v_r > 0)? 1: -1;
		} else if (v_r == 0) {
			vec = (v_f > 0)? 10: -10;
		} else if (v_f == v_r || v_f == -v_r) {
			vec = (msp->to - msp->from) / abs(v_r);
		}
		if (turn == B_PC_HITHER) {
			for (vcp = vec_hi_step[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
				if (vcp[0] == vec) {
					break;
				}
			}
			if (vcp[0] == 0) {
				for (vcp = vec_hi_run[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
					if (vcp[0] == vec) {
						break;
					}
				}
			}
		} else {
			for (vcp = vec_yo_step[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
				if (vcp[0] == vec) {
					break;
				}
			}
			if (vcp[0] == 0) {
				for (vcp = vec_yo_run[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
					if (vcp[0] == vec) {
						break;
					}
				}
			}
		}
		if (vcp[0] == 0) {
			return STR_ERR_GIVENMATEPATH_MOVE;
		}
	}
	s += 2;
	if (*s == '=') {
		if ((turn == B_PC_HITHER && RANK(msp->from) > 3 && RANK(msp->to) > 3)
			|| (turn == B_PC_YONDER && RANK(msp->from) < 7 && RANK(msp->to) < 7))
		{
			return STR_ERR_GIVENMATEPATH_PROMPOS;
		}
		msp->mv_flag |= B_MV_PROMOTE;
		s++;
		pc = get_indata_pc(s);
		if (pc < PC_TO || pc > PC_RY || pc == PC_OU || pc != KIND(msp->pc | B_PC_PROMOTE)) {
			return STR_ERR_GIVENMATEPATH_PROMPC;
		}
		s += 2;
	}
	if (*s == '/') {
		msp->mv_flag |= B_MV_REBORN;
		s++;
		if (*s < '1' || *s > '9' || *(s + 1) < '1' || *(s + 1) > '9') {
			return STR_ERR_GIVENMATEPATH_REBORN;
		}
		s += 2;
		pc = get_indata_pc(s);
		if (pc < PC_FU || pc > PC_RY) {
			return STR_ERR_GIVENMATEPATH_REBORN;
		}
		s += 2;
	}
	*sp = s;
	return 0;
}

static int
get_matemseq_nfmstyle(mseq_t *msp, mseq_t *msp_par, char **sp, uchar_t turn)
{
	const int *vcp;
	char *s, *t;
	int vec, v_f, v_r;
	uchar_t pc;

	s = *sp;
	if (*s >= '1' && *s <= '9' && *(s + 1) >= '1' && *(s + 1) <= '9') {
		msp->to = POS(*s - '0', *(s + 1) - '0');
		s += 2;
	}
	if (EQUALS(s, str[STR_CAPTIT]) && (msp->depth >= 2 || msp->to != 0)) {
		if (msp->to == 0) {
			msp->to = msp_par->to;
		}
		s += strlen(str[STR_CAPTIT]);
	}
	if (msp->to == 0) {
		return STR_ERR_GIVENMATEPATH_TO;
	}
	msp->pc = get_indata_pc(s);
	if (msp->pc < PC_FU || msp->pc > PC_RY) {
		return STR_ERR_GIVENMATEPATH_PC;
	}
	msp->pc |= turn;
	s += 2;
	if (*s == '<' || *s == '=' || *s == 'X' || *s == '/') {
	// ｎｆｍ式（移動の場合のみ）
		if (*s == '=') {
			if (msp->pc & B_PC_PROMOTE) {
				return STR_ERR_GIVENMATEPATH;
			}
			msp->mv_flag |= B_MV_PROMOTE;
			s++;
			pc = get_indata_pc(s);
			if (pc < PC_TO || pc > PC_RY || pc == PC_OU || pc != KIND(msp->pc | B_PC_PROMOTE)) {
				return STR_ERR_GIVENMATEPATH_PROMPC;
			}
			s += 2;
		}
		if (*s == 'X' || *s == '/') {
			msp->mv_flag |= B_MV_CAPTIT;
			s++;
			msp->capt = get_indata_pc(s);
			if (msp->capt < PC_FU || msp->capt > PC_RY) {
				return STR_ERR_GIVENMATEPATH_CAPT;
			}
			msp->capt |= (turn == B_PC_HITHER)? B_PC_YONDER: B_PC_HITHER;
			s += 2;
		}
		if (*s == '<') {
			s++;
			if (*s < '1' || *s > '9' || *(s + 1) < '1' || *(s + 1) > '9') {
				return STR_ERR_GIVENMATEPATH_TO;
			}
			msp->from = POS(*s - '0', *(s + 1) - '0');
			if (msp->from == msp->to) {
				return STR_ERR_GIVENMATEPATH;
			}
			if ((msp->mv_flag & B_MV_PROMOTE)
				&& ((turn == B_PC_HITHER && RANK(msp->from) > 3 && RANK(msp->to) > 3)
					|| (turn == B_PC_YONDER && RANK(msp->from) < 7 && RANK(msp->to) < 7)))
			{
				return STR_ERR_GIVENMATEPATH_PROMPOS;
			}
			s += 2;
		}
	} else {
	// ｆｍ式（打の場合は、ｎｆｍ式もこちらを通る）
		if (EQUALS(s, str[STR_PROMOTE])) {
			if (msp->pc & B_PC_PROMOTE) {
				return STR_ERR_GIVENMATEPATH;
			}
			msp->mv_flag |= B_MV_PROMOTE;
			s += strlen(str[STR_PROMOTE]);
		} else if (EQUALS(s, str[STR_RAW])) {
			if (msp->pc & B_PC_PROMOTE) {
				return STR_ERR_GIVENMATEPATH;
			}
			s += strlen(str[STR_RAW]);
		} else if (EQUALS(s, str[STR_NOTPROMOTE])) {
			if (msp->pc & B_PC_PROMOTE) {
				return STR_ERR_GIVENMATEPATH;
			}
			s += strlen(str[STR_NOTPROMOTE]);
		}
		if (EQUALS(s, str[STR_PLACE])) {
			msp->from = 0;
			s += strlen(str[STR_PLACE]);
		} else if (*s == '(') {
			t = _mbschr(s, ')');
			if (t == NULL) {
				return STR_ERR_GIVENMATEPATH_FROM;
			}
			s++;
			if (*s < '1' || *s > '9' || *(s + 1) < '1' || *(s + 1) > '9') {
				return STR_ERR_GIVENMATEPATH_TO;
			}
			msp->from = POS(*s - '0', *(s + 1) - '0');
			if (msp->to == msp->from) {
				return STR_ERR_GIVENMATEPATH;
			}
			if ((msp->mv_flag & B_MV_PROMOTE)
				&& ((turn == B_PC_HITHER && RANK(msp->from) > 3 && RANK(msp->to) > 3)
					|| (turn == B_PC_YONDER && RANK(msp->from) < 7 && RANK(msp->to) < 7)))
			{
				return STR_ERR_GIVENMATEPATH_PROMPOS;
			}
			msp->capt = msp_par->BDP->board[msp->to];
			s = t + 1;
		} else {
			msp->from = 0;
		}
	}
	if (msp->from != 0) {
		v_f = FILE(msp->to) - FILE(msp->from);
		v_r = RANK(msp->to) - RANK(msp->from);
		vec = 0;
		if (v_f == 0) {
			vec = (v_r > 0)? 1: -1;
		} else if (v_r == 0) {
			vec = (v_f > 0)? 10: -10;
		} else if (v_f == v_r || v_f == -v_r) {
			vec = (msp->to - msp->from) / abs(v_r);
		}
		if (turn == B_PC_HITHER) {
			for (vcp = vec_hi_step[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
				if (vcp[0] == vec) {
					break;
				}
			}
			if (vcp[0] == 0) {
				for (vcp = vec_hi_run[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
					if (vcp[0] == vec) {
						break;
					}
				}
			}
		} else {
			for (vcp = vec_yo_step[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
				if (vcp[0] == vec) {
					break;
				}
			}
			if (vcp[0] == 0) {
				for (vcp = vec_yo_run[KIND(msp->pc)]; vcp[0] != 0; vcp++) {
					if (vcp[0] == vec) {
						break;
					}
				}
			}
		}
		if (vcp[0] == 0) {
			return STR_ERR_GIVENMATEPATH_MOVE;
		}
	}
	*sp = s;
	return 0;
}

static void
put_err_indata(int str_idx)
{
	if (phase_count > 0) {
		fm_put_display(MSG_DIRECT, "\n");
	}
	fm_put_display(MSG_ERR_INDATA, str[str_idx]);
	return;
}

static int
get_matemseq(void)
{
	char *s, *t;
	mseq_t *msp, *msp_par, *msp_brop;
	int str_idx_err;
	uchar_t turn;

	msp = fm_get_mseq(0);
	*msp = mseq_void;
	msp->BDP = fm_get_board(0);
	*msp->BDP = board_prob;
	fm_set_fire(msp);
	fm_init_hash(msp);
	msp_givenmate = msp;
	msp_par = msp;
	if (rule & RULE_TARGETFIG) {
		if (((mseq_target.ps_flag & B_PS_HI_CHECK) && depth_limit % 2 == 1)
			|| (!(mseq_target.ps_flag & B_PS_HI_CHECK) && depth_limit % 2 == 0))
		{
			turn = B_PC_YONDER;
		} else {
			turn = B_PC_HITHER;
		}
	} else {
		if ((rule == RULE_NFMHM && depth_limit % 2 == 1)
			|| (rule == RULE_NFMHSM && depth_limit % 2 == 0))
		{
			turn = B_PC_YONDER;
		} else {
			turn = B_PC_HITHER;
		}
	}
	FOREVER() {
		if (inbuf[0] == '\0') {
			if (fm_fgets(inbuf, sizeof(inbuf), inFp) != 0) {
				t = NULL;
				goto error_exit;
			}
		}
		s = inbuf;
		while (*s != '\0') {
			if (isspace((uchar_t)*s)) {
				s++;
				continue;
			}
			t = s;
			if (turn == B_PC_YONDER) {
				turn = B_PC_HITHER;
			} else {
				turn = B_PC_YONDER;
			}
			msp = fm_get_mseq(0);
			msp_par->brop = msp;
			*msp = mseq_void;
			msp->parp = msp_par;
			msp->depth = msp_par->depth + 1;
			msp->hi_hash = msp_par->hi_hash;
			msp->yo_hash = msp_par->yo_hash;
			msp->hi_hand = msp_par->hi_hand;
			msp->from = 0xff;
			msp->capt = SQ_ROOM;
			msp->mv_flag |= B_MV_MATEPATH | B_MV_GIVEN;
			str_idx_err = 0;
			if ((*s >= '1' && *s <= '9' && *(s + 1) >= '1' && *(s + 1) <= '9')
				|| EQUALS(s, str[STR_CAPTIT]))
			{
				str_idx_err = get_matemseq_nfmstyle(msp, msp_par, &s, turn);
			} else {
				str_idx_err = get_matemseq_chesstyle(msp, msp_par, &s, turn);
			}
			if (str_idx_err != 0) {
				goto error_exit;
			}
			if (fm_test_move(msp) == RC_ILLEGAL) {
				str_idx_err = STR_ERR_GIVENMATEPATH_MOVE;
				goto error_exit;
			}
			msp->BDP = fm_get_board(0);
			*msp->BDP = *msp_par->BDP;
			if (fm_set_givenphase(msp) == RC_ILLEGAL) {
				str_idx_err = STR_ERR_GIVENMATEPATH_PHASE;
				goto error_exit;
			}
			fm_set_fire(msp);
			fm_calc_hash(msp);
			if (turn == B_PC_HITHER) {
				if ((msp->ps_flag & B_PS_YO_CHECK) || !(msp->ps_flag & B_PS_HI_CHECK)) {
					str_idx_err = STR_ERR_GIVENMATEPATH_PHASE;
					goto error_exit;
				}
			} else {
				if (msp->ps_flag & B_PS_HI_CHECK) {
					str_idx_err = STR_ERR_GIVENMATEPATH_PHASE;
					goto error_exit;
				}
			}
			msp_par = msp;
			if (turn == B_PC_HITHER) {
				if (msp_par->ps_flag & B_PS_HI_CHECK) {
					if (fm_test_yo_matemseq(msp_par) == RC_MATE) {
						msp_par->ps_flag |= B_PS_MATE;
						break;
					}
				} else if ((rule & RULE_TARGETFIG) && mask_targetdepth == 1) {
					if (test_targetmseq(msp_par) == RC_MATE) {
						msp_par->ps_flag |= B_PS_MATE;
						break;
					}
				}
			} else {
				if (msp_par->ps_flag & B_PS_YO_CHECK) {
					if (fm_test_hi_matemseq(msp_par) == RC_MATE) {
						msp_par->ps_flag |= B_PS_MATE;
						break;
					}
				} else if ((rule & RULE_TARGETFIG) && mask_targetdepth == 0) {
					if (test_targetmseq(msp_par) == RC_MATE) {
						msp_par->ps_flag |= B_PS_MATE;
						break;
					}
				}
			}
		}
		if (msp_par->ps_flag & B_PS_MATE) {
			depth_limit = msp->depth;
			break;
		}
		inbuf[0] = '\0';
	}
	return 0;

error_exit:;
	put_err_indata(STR_GET_GIVENMATEPATH);
	if (t != NULL) {
		s = strchr(t, ' ');
		if (s != NULL) {
			*s = '\0';
		}
		fm_put_display(MSG_ERR_GIVENMATEPATH,
			fm_format_num(editbuf, msp->depth), t, str[str_idx_err]);
	}
	for (msp = msp_givenmate; msp != NULL; msp = msp_brop) {
		msp_brop = msp->brop;
		if (msp->BDP != NULL) {
			FREE_BOARD(msp->BDP);
		}
		FREE_MSEQ(msp);
	}
	return -1;
}

void
set_mseqprob(void)
{
	mseq_prob = mseq_void;
	mseq_prob.BDP = &board_prob;
	fm_set_fire(&mseq_prob);
	fm_init_hash(&mseq_prob);
}

int
fm_get_problem(void)
{
	int rc;
	int n;

	reset_problem();
	n = fm_get_rule_data(inbuf, sizeof(inbuf), inFp);
	if (n == 0) {
		return 1;		// end
	}
	rc = get_rule_and_depth(inbuf);
	if (rc != 0) {
		if (rc == 1) {
			return 1;		// end
		}
		if (rc == -1) {
			put_err_indata(STR_GET_RULE_AND_DEPTH);
			fm_put_indata(inbuf);
			return -1;
		}
	}
	n--;
	if (((rule == RULE_NFMHMA || rule == RULE_NFMHSMA) && n < 4) || n < 2) {
		n = fm_get_phase_data(inbuf, sizeof(inbuf), inFp, n);
		if (n == 0) {
			put_err_indata(STR_GET_PHASE_DATA);
			fm_put_indata(inbuf);
			return -1;
		}
	}
	if (get_phase(inbuf, &board_prob) != 0) {
		put_err_indata(STR_GET_PHASE);
		fm_put_indata(inbuf);
		return -1;
	}
	n -= 2;
	if (rule == RULE_NFMHMA || rule == RULE_NFMHSMA) {
		if (get_phase(inbuf, &board_target) != 0) {
			put_err_indata(STR_GET_TARGET_PHASE);
			fm_put_indata(inbuf);
			return -1;
		}
		n -= 2;
	} else if (rule == RULE_NFMHA) {
		set_phase(&board_target, &board_prob);
	}
	if (sc_tact_usextrapc == ON) {
		if (n < 1) {
			n = fm_get_extrapc_data(inbuf, sizeof(inbuf), inFp);
			if (n == 0) {
				put_err_indata(STR_GET_PHASE_DATA);
				fm_put_indata(inbuf);
				return -1;
			}
		}
		if (set_extrapc(inbuf, &board_prob, &board_target) != 0) {
			put_err_indata(STR_GET_EXTRAPC);
			fm_put_indata(inbuf);
			return -1;
		}
	}
	if (sc_tact_givenmatepath == ON
		&& (rule == RULE_NFMHM || rule == RULE_NFMHMA
			|| rule == RULE_NFMHSM || rule == RULE_NFMHSMA || rule == RULE_NFMHA))
	{
		if (get_matemseq() != 0) {
			return -1;
		}
	}
	set_mseqprob();
	return 0;
}

int
fm_change_givenmatepath(void)
{
	mseq_t *msp, *msp_brop, *msp_mate, *msq;
	long depth, depth_mate;

	depth_mate = -1;
	for (depth = 0; depth <= depth_limit + depth_rsv - 1; depth++) {
		if (mseq_head[depth].flag & B_MH_MATE) {
			depth_mate = depth;
			break;
		}
	}
	if (depth_mate == -1) {
		return -1;
	}
	msp_mate = NULL;
	for (msp = mseq_head[depth].brop; msp != NULL; msp = msp->brop) {
		if (msp->ps_flag & B_PS_GIVENMATEPATH) {
			msp_mate = msp;
		}
	}
	if (msp_mate == NULL) {
		return -1;
	}
	for (msp = msp_givenmate->brop; msp != NULL; msp = msp_brop) {
		msp_brop = msp->brop;
		if (msp->hi_hash == msp_mate->hi_hash
			&& msp->yo_hash == msp_mate->yo_hash
			&& msp->hi_hand == msp_mate->hi_hand)
		{
			msp->pc = msp_mate->pc;
			msp->from = msp_mate->from;
			msp->to = msp_mate->to;
			msp->capt = msp_mate->capt;
			msp->mv_flag = msp_mate->mv_flag | B_MV_GIVEN;
			msp_givenmate->brop = msp;
			break;
		}
		FREE_BOARD(msp->BDP);
		FREE_MSEQ(msp);
	}
	for (msp = msp_mate->parp; msp != NULL && msp->parp != NULL; msp = msp->parp) {
		msq = fm_get_mseq(0);
		*msq = *msp;
		msq->brop = msp_givenmate->brop;
		msp_givenmate->brop = msq;
		msq->BDP = fm_get_board(0);
		*msq->BDP = *msp->BDP;
		msq->ref_count = 0;
		msq->mv_flag |= B_MV_MATEPATH | B_MV_GIVEN;
	}
	depth = 1;
	for (msp = msp_givenmate->brop; msp != NULL; msp = msp->brop) {
		msp->depth = depth;
		depth++;
	}
	depth_limit = depth - 1;
	return 0;
}

