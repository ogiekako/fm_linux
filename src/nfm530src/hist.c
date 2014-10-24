// nfm/nfm/hist.c -- history data
// Copyright(c) KAMINA Jiro 2006-2013.  All rights reserved.
// $Header$

char cmdname[] =
#if defined(_MSC_VER)	// VC++
#if defined(_M_X64)
"nfm64"
#elif _MSC_VER >= 1500
"nfm"
#else
"vnfm"
#endif
#endif
;
char release[] = "5.30"	// 2013-06-09
#if defined(_MSC_VER)	// VC++
#if _MSC_VER >= 1600
" (VC++ 2010)"
#elif _MSC_VER >= 1500
" (VC++ 2008)"
#elif _MSC_VER >= 1200
" (VC++ 6.0)"
#elif _MSC_VER >= 1100
" (VC++ 5.0)"
#else
" (VC++)"
#endif
#endif
;

// 2013-06-09 - 5.30; release version
// 2010-06-04 - 5.29; fix for over 2gb memory allocation
//         05         fix for displaying record
// 2010-06-02 - 5.28; support vc++ 2010
// 2010-05-23 - 5.27; put hma problem input only for start depth
//         24         modify /tf to /tf+ & /tf- and /tw to /tw+ & /tw-
// 2010-05-17 - 5.26; improve free mseq message
//                    improve maximum used memory option specificaion
//         18         bug fix for mseq memory allocation
//         19         add /sr=... for setting mseq-reuse unit
//         22         support [problem]-[end]
//                    put hma problem input for /tr
//                    support /tf & /tw for selfmate
// 2010-05-12 - 5.25; buf fix for fat mseq
// 2010-04-30 - 5.24; buf fix for getting nfm style move
// 2010-04-29 - 5.23; buf fix for memsize variables
// 2008-03-15 - 5.22; buf fix and improve
// 2007-10-27 - 5.21; modify pwc chess-style score
// 2007-10-21 - 5.20; improve output, bug fix for pwc
// 2007-10-20 - 5.19; merge pwc version, add noprom
// 2007-10-17 - 5.18; modify nfm-stype move output, and bug fix
// 2007-01-20 - 5.17; modify nfmhma and nfmhsma
// 2007-01-09 - 5.16; nfma
// 2007-01-06 - 5.15; bug fix
// 2007-01-05 - 5.14; use given mate sequence (nfmhma, nfmhsma)
// 2007-01-04 - 5.13; use given mate sequence (nfmhm, nfmhsm)
// 2007-01-04 - 5.12; bug fix
// 2007-01-03 - 5.11; bug fix
// 2007-01-02 - 5.10; bug fix
// 2007-01-01 - 5.09; merge all (nfmhm, nfmhsm, nfmshm, nfmserm, nfmhma, and nfmhsma)
// 2006-12-30 - 5.08; add nfmhma and nfmhsma
// 2006-12-28 - 5.07; bug fix and improve
// 2006-12-27 - 5.06; bug fix and improve
// 2006-12-25 - 5.05; bug fix and improve
// 2006-12-22 - 5.04; operation caterpiller
// 2006-12-20 - 5.03; operation ufm-2
// 2006-12-18 - 5.02; operation octopus
// 2006-12-10 - 5.01; operation ufm
// 2006-11-19 - 5.00; transplant from fmodx 4.01

