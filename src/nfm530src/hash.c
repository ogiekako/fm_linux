// nfm/nfm/hash.c - hash related functions
// Copyright (c) KAMINA Jiro 2006-2007.  All rights reserved.
// $Header: hash.cv  1.3  07/10/20 22:44:35  Exp $

#include	<stdio.h>
#include	<stdlib.h>
#include	"nfm.h"
#include	"funcdef.h"
#include	"macro.h"

static ulong_t rand_seed = 123456789;
static ulong_t board_hash[BOARD_SIZE][0xff + 1];

// mt19937i.c
extern void sgenrand();
extern ulong_t genrand();

// test hash value
// NULL: not found
// (mseq_t *)addr: found
mseq_t *
fm_test_hash(mseq_t *msp)
{
	mseq_t *msq, *msq_prev;
	long index;
	int n;

	index = (long)((msp->hi_hash ^ msp->yo_hash) & hashhead_count_1);
	if (IS_HI_PC(msp->pc)) {
		test_hash_check_count++;
		n = 0;
		msq_prev = NULL;
		for (msq = hashhead_check[index]; msq != NULL; msq = msq->amip) {
			ASSERT(msq != msq->amip);
			n++;
			if (msq->hi_hash == msp->hi_hash && msq->yo_hash == msp->yo_hash
				&& ((msq->ps_flag ^ msp->ps_flag) & B_PS_UFMATE) == 0x0000)
			{
				if (msq->hi_hand == msp->hi_hand) {
					comp_hash_check_found += n;
					found_hash_check_count++;
					return msq;
				}
				if (sc_tact_sameaslesspc == ON) {
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) >= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) >= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) >= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) >= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) >= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) >= (msp->hi_hand & M_HH_HI))
					{
						comp_hash_check_found += n;
						found_hash_check_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) <= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) <= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) <= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) <= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) <= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) <= (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_check[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameasmorepc == ON) {
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) <= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) <= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) <= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) <= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) <= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) <= (msp->hi_hand & M_HH_HI))
					{
						comp_hash_check_found += n;
						found_hash_check_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) >= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) >= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) >= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) >= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) >= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) >= (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_check[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameaslessfu == ON) {
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						comp_hash_check_found += n;
						found_hash_check_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_check[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameasmorefu == ON) {
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						comp_hash_check_found += n;
						found_hash_check_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_check[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				}
			}
			msq_prev = msq;
		}
		comp_hash_check_notfound += n;
		notfound_hash_check_count++;
	} else {
		test_hash_escape_count++;
		n = 0;
		msq_prev = NULL;
		for (msq = hashhead_escape[index]; msq != NULL; msq = msq->amip) {
			ASSERT(msq != msq->amip);
			n++;
			if (msq->hi_hash == msp->hi_hash && msq->yo_hash == msp->yo_hash
				&& ((msq->ps_flag ^ msp->ps_flag) & B_PS_UFMATE) == 0x0000)
			{
				if (msq->hi_hand == msp->hi_hand) {
					comp_hash_escape_found += n;
					found_hash_escape_count++;
					return msq;
				}
				if (sc_tact_sameaslesspc == ON) {
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) >= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) >= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) >= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) >= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) >= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) >= (msp->hi_hand & M_HH_HI))
					{
						comp_hash_escape_found += n;
						found_hash_escape_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) <= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) <= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) <= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) <= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) <= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) <= (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_escape[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameasmorepc == ON) {
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) <= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) <= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) <= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) <= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) <= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) <= (msp->hi_hand & M_HH_HI))
					{
						comp_hash_escape_found += n;
						found_hash_escape_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) >= (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) >= (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) >= (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) >= (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) >= (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) >= (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_escape[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameaslessfu == ON) {
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						comp_hash_escape_found += n;
						found_hash_escape_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_escape[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				} else if (sc_tact_sameasmorefu == ON) {
					if ((msq->hi_hand & M_HH_FU) <= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						comp_hash_escape_found += n;
						found_hash_escape_count++;
						return msq;
					}
					if ((msq->hi_hand & M_HH_FU) >= (msp->hi_hand & M_HH_FU)
						&& (msq->hi_hand & M_HH_KY) == (msp->hi_hand & M_HH_KY)
						&& (msq->hi_hand & M_HH_KE) == (msp->hi_hand & M_HH_KE)
						&& (msq->hi_hand & M_HH_GI) == (msp->hi_hand & M_HH_GI)
						&& (msq->hi_hand & M_HH_KI) == (msp->hi_hand & M_HH_KI)
						&& (msq->hi_hand & M_HH_KA) == (msp->hi_hand & M_HH_KA)
						&& (msq->hi_hand & M_HH_HI) == (msp->hi_hand & M_HH_HI))
					{
						if (msq_prev == NULL) {
							hashhead_escape[index] = msq->amip;
						} else {
							msq_prev->amip = msq->amip;
						}
						msq->amip = NULL;
						if (sc_tact_freesamemseq == ON && msq->depth == msp->depth) {
							fm_free_mseq(msq->depth, msq);
						}
						break;
					}
				}
			}
			msq_prev = msq;
		}
		comp_hash_escape_notfound += n;
		notfound_hash_escape_count++;
	}
	return NULL;
}

void
fm_attach_hash(mseq_t *msp)
{
	long index;

	index = (long)((msp->hi_hash ^ msp->yo_hash) & hashhead_count_1);
	if (IS_HI_PC(msp->pc)) {
		ASSERT(hashhead_check[index] != msp);
		msp->amip = hashhead_check[index];
		hashhead_check[index] = msp;
	} else {
		ASSERT(hashhead_escape[index] != msp);
		msp->amip = hashhead_escape[index];
		hashhead_escape[index] = msp;
	}
	return;
}

void
fm_free_hash(mseq_t *msp, mseq_t **hashhead)
{
	mseq_t *msq, *msq_prev;
	long index;

	index = (long)((msp->hi_hash ^ msp->yo_hash) & hashhead_count_1);
	msq_prev = NULL;
	for (msq = hashhead[index]; msq != NULL; msq = msq->amip) {
		ASSERT(msq != msq->amip);
		if (msq == msp) {
			if (msq_prev == NULL) {
				hashhead[index] = msq->amip;
			} else {
				msq_prev->amip = msq->amip;
			}
			msq->amip = NULL;
			break;
		}
		msq_prev = msq;
	}
	return;
}

// calculate hash value
void
fm_calc_hash(mseq_t *msp)
{
	uchar_t pc;

	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			ASSERT(msp->pc >= HI_FU && msp->pc <= HI_HI);
			msp->hi_hash ^= board_hash[msp->to][msp->pc];
			pc = RAW_KIND(msp->pc);
			msp->hi_hand &= ~bmhh[pc];
			msp->hi_hand |= bphh[pc][msp->BDP->hi_hand[pc]];
		} else {
			msp->hi_hash ^= board_hash[msp->from][msp->pc];
			if (msp->mv_flag & B_MV_PROMOTE) {
				msp->hi_hash ^= board_hash[msp->to][msp->pc | B_PC_PROMOTE];
			} else {
				msp->hi_hash ^= board_hash[msp->to][msp->pc];
			}
			if (!IS_ROOM(msp->capt)) {
				ASSERT(msp->capt != YO_GY);
				msp->yo_hash ^= board_hash[msp->to][msp->capt];
				if (msp->mv_flag & B_MV_REBORN) {
					msp->yo_hash ^= board_hash[msp->from][msp->capt];
				} else {
					pc = RAW_KIND(msp->capt);
					msp->hi_hand &= ~bmhh[pc];
					msp->hi_hand |= bphh[pc][msp->BDP->hi_hand[pc]];
				}
			}
		}
	} else {
		if (msp->from == 0) {
			msp->yo_hash ^= board_hash[msp->to][msp->pc];
		} else {
			msp->yo_hash ^= board_hash[msp->from][msp->pc];
			if (msp->mv_flag & B_MV_PROMOTE) {
				msp->yo_hash ^= board_hash[msp->to][msp->pc | B_PC_PROMOTE];
			} else {
				msp->yo_hash ^= board_hash[msp->to][msp->pc];
			}
			if (!IS_ROOM(msp->capt)) {
				ASSERT(msp->capt != HI_OU);
				msp->hi_hash ^= board_hash[msp->to][msp->capt];
				if (msp->mv_flag & B_MV_REBORN) {
					msp->hi_hash ^= board_hash[msp->from][msp->capt];
				}
			}
		}
	}
	return;
}

// back-calculate hash value
void
fm_back_hash(mseq_t *msp)
{
	uchar_t pc;

	if (IS_HI_PC(msp->pc)) {
		if (msp->from == 0) {
			ASSERT(msp->pc >= HI_FU && msp->pc <= HI_HI);
			msp->hi_hash ^= board_hash[msp->to][msp->pc];
			pc = RAW_KIND(msp->pc);
			msp->hi_hand &= ~bmhh[pc];
			msp->hi_hand |= bphh[pc][msp->parp->BDP->hi_hand[pc]];
		} else {
			msp->hi_hash ^= board_hash[msp->from][msp->pc];
			if (msp->mv_flag & B_MV_PROMOTE) {
				msp->hi_hash ^= board_hash[msp->to][msp->pc | B_PC_PROMOTE];
			} else {
				msp->hi_hash ^= board_hash[msp->to][msp->pc];
			}
			if (!IS_ROOM(msp->capt)) {
				ASSERT(msp->capt != YO_GY);
				msp->yo_hash ^= board_hash[msp->to][msp->capt];
				if (msp->mv_flag & B_MV_REBORN) {
					msp->yo_hash ^= board_hash[msp->from][msp->capt];
				} else {
					pc = RAW_KIND(msp->capt);
					msp->hi_hand &= ~bmhh[pc];
					msp->hi_hand |= bphh[pc][msp->parp->BDP->hi_hand[pc]];
				}
			}
		}
	} else {
		if (msp->from == 0) {
			msp->yo_hash ^= board_hash[msp->to][msp->pc];
		} else {
			msp->yo_hash ^= board_hash[msp->from][msp->pc];
			if (msp->mv_flag & B_MV_PROMOTE) {
				msp->yo_hash ^= board_hash[msp->to][msp->pc | B_PC_PROMOTE];
			} else {
				msp->yo_hash ^= board_hash[msp->to][msp->pc];
			}
			if (!IS_ROOM(msp->capt)) {
				ASSERT(msp->capt != HI_OU);
				msp->hi_hash ^= board_hash[msp->to][msp->capt];
				if (msp->mv_flag & B_MV_REBORN) {
					msp->hi_hash ^= board_hash[msp->from][msp->capt];
				}
			}
		}
	}
	return;
}

// initialize phase hash
void
fm_init_hash(mseq_t *msp)
{
	board_t *bdp;
	int p_f, pos;

	bdp = msp->BDP;
	for  (p_f = 1; p_f <= 9; p_f++) {
		for  (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			if (IS_HI_PC(bdp->board[pos])) {
				msp->hi_hash ^= board_hash[pos][bdp->board[pos]];
			} else if (IS_YO_PC(bdp->board[pos])) {
				msp->yo_hash ^= board_hash[pos][bdp->board[pos]];
			}
		}
	}
	msp->hi_hand = bphh[PC_FU][bdp->hi_hand[PC_FU]]
					+ bphh[PC_KY][bdp->hi_hand[PC_KY]]
					+ bphh[PC_KE][bdp->hi_hand[PC_KE]]
					+ bphh[PC_GI][bdp->hi_hand[PC_GI]]
					+ bphh[PC_KI][bdp->hi_hand[PC_KI]]
					+ bphh[PC_KA][bdp->hi_hand[PC_KA]]
					+ bphh[PC_HI][bdp->hi_hand[PC_HI]];
	return;
}

void
fm_make_hashtable(void)
{
	int p_f, pos;
	uchar_t pc;

	sgenrand(rand_seed);
	for (p_f = 1; p_f <= 9; p_f++) {
		for (pos = POS(p_f, 1); pos <= POS(p_f, 9); pos++) {
			for (pc = HI_FU; pc <= HI_RY; pc++) {
				board_hash[pos][pc] = genrand();
			}
			for (pc = YO_FU; pc <= YO_RY; pc++) {
				board_hash[pos][pc] = genrand();
			}
		}
	}
	return;
}

