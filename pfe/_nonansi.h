#ifndef __PFE__NONANSI_H
#define __PFE__NONANSI_H "%full_filespec: _nonansi.h~5.3:incl:bln_12xx!1 %"

/** 
 *  -- load platform-specific system-includes 
 *
 *  Copyright (C) Tektronix, Inc. 1998 - 2000. All rights reserved.
 *
 *  @see     GNU LGPL
 *  @author  Tektronix CTE           @(#) %derived_by: guidod %
 *  @version %version: 5.3 %
 *    (%date_modified: Mon Mar 12 10:33:14 2001 %)
 *
 *  @description
 *              Include file, gathers non-ANSI includes for low level
 *		file I/O and operating system access in general.
 *
 *              It will only work if _P4_SOURCE is defined.
 *              Use only for pfe-specific pfe-loadable third-party modules.
 *
 */
/*@{*/

#ifdef _P4_SOURCE

#include <pfe/def-config.h>

#if defined HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>	/* structs and constants for low level tty access */
#endif

#if defined HAVE_FCNTL_H
# include <fcntl.h>	/* structs and constants for low level file access */
#endif

#if defined HAVE_SYS_STAT_H
# include <sys/stat.h>	/* structs and constants for stat() and fstat() */
#endif

#if defined HAVE_DIRECT_H
# include <direct.h>	/* directory related */
#endif

#if defined HAVE_SYS_NDIR_H
# include <sys/ndir.h>	/* ??? */
#endif

#if defined HAVE_UTIME_H
# include <utime.h>	/* file access/modification time structure */
#endif

#if defined HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#if defined HAVE_SYS_UTIME_H
# include <sys/utime.h>
#endif

#if defined HAVE_POLL_H
# include <poll.h>	/* the poll() system call */
#elif defined HAVE_SYS_POLL_H
# include <sys/poll.h>
#endif

#if defined HAVE_SYS_SELECT_H
# include <sys/select.h> /* the select() system call */
#endif

#if defined HAVE_PROCESS_H
# include <process.h>	/* fork, exec */
#elif defined HAVE_SYS_PROCESS_H
# include <sys/process.h>
#endif

#if defined HAVE_UNISTD_H
# include <unistd.h>	/* most low level file functions on Posix systems */
#endif

#if defined HAVE_IO_H
# include <io.h>	/* equivalent to unistd.h on some DOS like systems */
#endif

#if defined HAVE_LIBC_H	/* NeXT has this */
# include <libc.h>
#endif

#if defined HAVE_CONIO_H
# include <conio.h>	/* DOS like systems define cputs() here */
#endif

#if defined HAVE_DOS_H
# include <dos.h>	/* DOS like systems define things like delay() here */
#endif

#if defined HAVE_MEM_H
# include <mem.h>	/* some systems have memcpy() here */
#endif

#if defined HAVE_MEMORY_H
# include <memory.h>	/* or here */
#endif

#if defined HAVE_STRINGS_H
# include <strings.h>	/* place where old BSD systems keep bcopy() */
#endif

#ifdef __PFE__MISSING_H
#error do not include pfe/_missing.h before pfe/_nonansi.h
#endif

#include <pfe/_missing.h>

#endif
  /* will only work if _P4_SOURCE defined */

/*@}*/
#endif 


