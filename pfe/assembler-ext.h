#ifndef _PFE_ASSEMBLER_EXT_H
#define _PFE_ASSEMBLER_EXT_H 1159310733
/* generated 2006-0927-0045 ../../pfe/../mk/Make-H.pl ../../pfe/assembler-ext.c */

#include <pfe/pfe-ext.h>

/** 
 * -- Assembler Words for the Optional Programming-Tools Word Set
 *
 *  Copyright (C) 2008 Guido U. Draheim <guidod@gmx.de>
 *
 *  @see     GNU LGPL
 *  @author  Guido U. Draheim            (modified by $Author: guidod $)
 *  @version $Revision: 1.3 $
 *     (modified $Date: 2008-04-20 04:46:30 $)
 *
 *  @description
 *      The ANS Forth defines some "Programming Tools" containing
 *      also an ASSEMBLER wordlist plus CODE and ;CODE to create
 *      callable words with a machine-level text inbetween. This
 *      wordset.
 *
 *      Unlike most systems, the PFE is a portable system - therefore
 *      it is asserted that the machine-level words for CODE / END-CODE
 *      are known for the specific machine that this Forth is being
 *      compiled for. Hence you must explicitly require the assembler
 *      to make it working.
 */

#ifdef __cplusplus
extern "C" {
#endif




/** CODE ( "name" -- )
 * call => ALSO and add ASSEMBLER wordlist if available. Add PROC ENTER
 * assembler snippet as needed for the architecture into the PFA. The
 * CFA is setup (a) with the PFA adress in traditional ITC or (b)
 * with an infoblock as for sbr-coded colon words.
 * 
 * Remember that not all architectures are support and that the
 * ASSEMBLER wordset is not compiled into pfe by default. Use always
 * the corresponding => END-CODE for each => CODE start. The new
 * word name is not smudged.
 */
extern P4_CODE (p4_asm_create_code);

extern P4_CODE (p4_asm_semicolon_code);

/** END-CODE ( "name" -- )
 * call => PREVIOUS and  add PROC LEAVE assembler snippet as needed
 * for the architecture -  usually includes bits to "return from
 * subroutine". Remember that not all architectures are support and
 * PFE usually does only do variants of call-threading with a separate
 * loop for the inner interpreter that does "call into subroutine".
 * Some forth implementations do "jump into routine" and the PROC
 * LEAVE part would do "jump to next routine" also known as 
 * next-threading. The sbr-call-threading is usually similar to the
 * native subroutine-coding of the host operating system. See => CODE
 */
extern P4_CODE (p4_asm_end_code);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
