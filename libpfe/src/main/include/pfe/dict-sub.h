#ifndef PFE_DICT_SUB_H
#define PFE_DICT_SUB_H 20091024
/* generated by make-header.py from ../../c/dict-sub.c */

#include <pfe/pfe-sub.h>

/**
 *  Implements dictionary and wordlists.
 *
 *  Copyright (C) Tektronix, Inc. 1998 - 2001.
 *  Copyright (C) 2005 - 2008 Guido U. Draheim <guidod@gmx.de>
 *
 *  @see     GNU LGPL
 *  @author  Guido U. Draheim            (modified by $Author: guidod $)
 *  @version $Revision: 514 $
 *     (modified $Date: 2009-10-22 16:18:28 +0200 (Do, 22. Okt 2009) $)
 */

#ifdef __cplusplus
extern "C" {
#endif




/** ((FORGET))
 * remove words from dictionary, free dictionary space, this is the
 * runtime helper of => (FORGET)
 */
extern void FXCode (p4_forget_dp);

extern void FXCode_RT (p4_destroyer_RT);

/** ONLY ( -- )
 * the only-vocabulary is special. Calling it will erase
 * the search => ORDER of vocabularies and only allows
 * to name some very basic vocabularies. Even => ALSO
 * is not available.
 example:
   ONLY FORTH ALSO EXTENSIONS ALSO DEFINITIONS
 */
extern void FXCode (p4_only_RT);

/** FORTH ( -- )
 : FORTH FORTH-WORDLIST CONTEXT ! ;
 */
extern void FXCode (p4_forth_RT);

/**
 * A vocabulary is organized as a mixture between hash-table and
 * linked list. (This is a practice you can observe in several
 * systems.) It works like this: Given a name, first a hash-code is
 * generated. This hash-code selects one of several linked lists
 * called threads. The hooks to these threads are stored in a table.
 *
 * The body of a WORDLIST is essentially such a table of pointers to
 * threads, while in FIG-Forth it was just a pointer to the one and
 * only linked list a VOCABULARY consists of in FIG-Forth.
 */
extern int p4_wl_hash (const p4_char_t *s, int l); /*/ * s s t r i n g , l l e n g t h o f s t r i n g , r e t u r n s h a s h - c o d e f o r t h a t n a m e * / {*/

/**
 * find the thread with the latest word in the given word list
 */
extern p4char ** p4_topmost (p4_Wordl *w); /*{*/

/** return the NFA of the latest definition in the CURRENT WORDLIST */
extern p4char * p4_latest (void); /*{*/

/**
 * create a word list in the dictionary
 */
extern p4_Wordl * p4_make_wordlist (p4char* nfa); /*{*/

/** runs p4_make_wordlist on the z-string */
extern p4_Wordl * p4_find_wordlist_str (const char* nm); /*{*/

/** FIND-WORDLIST is mostly required by => LOAD-WORDS  */
extern p4_Wordl * p4_find_wordlist (const p4_char_t* nm, int nmlen); /*{*/

/** (FORGET)
 * forget anything above address
 */
extern void p4_forget (p4_byte_t* above); /*{*/

/**
 * create a destroyer word. Upon =>'FORGET' the code will will be
 * run with the given argument. The structure is code/CFA and what/PFA.
 */
extern p4_namebuf_t* p4_forget_word (const char *name, p4cell id, p4code ccode, p4cell what); /*{*/

/** search headers in a wordlist */
extern p4_namebuf_t* p4_search_wordlist (const p4_char_t *nm, int l, const p4_Wordl *w); /*{*/

/** continue search headers in a wordlist */
extern p4_namebuf_t* p4_next_search_wordlist (p4_namebuf_t* last, const p4_char_t* nm, int l, const p4_Wordl* w); /*{*/

/** FIND
 *  search all word lists in the search order for name, return NFA
 * (we use the id speedup here - the first WLs have each a unique bitmask
 *  in the wl->id. Especially the FORTH wordlist can be present multiple
 *  time - even in being just search via wl->also. With w->id each is just
 *  searched once - atleast for each of the WLs that have gotten an id-bit
 *  which on a 32bit system are 32 WLs - enough for many system setups.
 *  It might be possible to use the old code even here (that walked the
 *  ORDER to see if the next WL is present in an earlier slot) but in a
 *  system with so many vocs it is quite improbable to find duplicates
 *  other than the basic vocs like FORTH in there anyway - so we use this
 *  one that might search a WL twice theoretically. Tell me of occasions
 *  where that is really a problem - in my setups it happens that the ORDER
 *  overflows much before getting duplicates other than the basic wordlists.
 */
extern p4char * p4_find (const p4_char_t *nm, int l); /*{*/

/**
 * tick next word,  and
 * return count byte pointer of name field (to detect immediacy)
 */
extern p4char * p4_tick_nfa (void); /*{*/

/**
 * tick next word,  and return xt
 */
extern p4xt p4_tick_cfa (void); /*{*/

/** writes counted string into dictionary, returns address */
extern p4_charbuf_t* p4_string_comma (const p4_char_t* s, int len); /*{*/

/**
 * Show words in word list matching pattern, and of one of the
 * categories in string `categories'. NULL pointer or zero length
 * string means all kinds of words.
 */
extern void p4_wild_words (const p4_Wordl *wl, const p4char *pattern, const char *categories); /*{*/

/** used in lined and edit-ext */
extern int p4_complete_dictionary (char *in, char *out, int display); /*{*/

/** used during boot-phase */
extern void p4_preload_only (void); /*{*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
