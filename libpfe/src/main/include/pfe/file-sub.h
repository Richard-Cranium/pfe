#ifndef PFE_FILE_SUB_H
#define PFE_FILE_SUB_H 20091024
/* generated by make-header.py from ../../c/file-sub.c */

#include <pfe/pfe-sub.h>

/**
 *  Subroutines for file access
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




/**
 * Result: file length, -1 on error
 */
extern _p4_off_t p4_file_size (FILE * f); /*{*/

/**
 * Copies file, but at most limit characters.
 * Returns destination file length if successful, -1 otherwise.
 */
extern _p4_off_t p4_file_copy (const char *src, const char *dst, _p4_off_t limit); /*{*/

/**
 * Renames or moves file, returns 0 on success, -1 on error.
 */
extern int p4_file_move (const char *src, const char *dst); /*{*/

/**
 * Truncates or extends file.
 * Returns 0 if successful, -1 otherwise.
 */
extern int p4_file_resize (const char *fn, _p4_off_t new_size); /*{*/

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
