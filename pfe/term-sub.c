/** 
 * --   terminal i/o, system independent parts
 * 
 *  Copyright (C) Tektronix, Inc. 1998 - 2001. All rights reserved.
 *
 *  @see     GNU LGPL
 *  @author  Tektronix CTE              @(#) %derived_by: guidod %
 *  @version %version: 1.12 %
 *    (%date_modified: Mon Mar 12 10:33:00 2001 %)
 *
 *  @description
 *       functions to talk to the terminal driver of forth.
 *       Depending on the active terminal driver, not
 *       all of these words are fully functional.
 */
/*@{*/
#if defined(__version_control__) && defined(__GNUC__)
static char* id __attribute__((unused)) = 
"@(#) $Id: term-sub.c,v 0.30 2001-03-12 09:33:00 guidod Exp $";
#endif

#define _P4_SOURCE 1

#include <pfe/incl-ext.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <pfe/def-xtra.h>
#include <pfe/term-sub.h>
#include <pfe/_missing.h>

#ifdef _K12_SOURCE
#include <pfe/term-k12.h>
#endif

#ifndef isascii
#define isascii(X) ((unsigned char) (X) < 0x80)
#endif

#if !defined PFE_CHARSET_IBMPC && !defined PFE_CHARSET_ISO
# if PFE_PATH_STYLE & 16
# define PFE_CHARSET_IBMPC 1
# else
# define PFE_CHARSET_ISO 1
# endif
#endif

/* -----------------------------------------------------------------------
 * map function and cursor keys to wordstar key sequences or to EKEY codes: 
 */

/** 
 * replacement strings for the key codes, 
 * wordstar-like version for block-editor.
 */
static char
*raw2ws [P4_NUM_KEYS] =
{
    "\013L",			/* F1 -> ^KL i.e. interpret this line */
    "\013D",			/* F2 -> ^KD i.e. stamp screen */
    "\013U",			/* F3 -> ^KU i.e. use other block file */
    NULL,
    "\013X",			/* F5 -> ^KX i.e. push line */
    "\013E",			/* F6 -> ^KE i.e. pop line */
    "\013Y",			/* F7 -> ^KY i.e. push and delete line */
    "\013W",			/* F8 -> ^KW i.e. pop and insert line */
    "\013B",			/* F9 -> ^KB i.e. load this block */
    "\025",			/* F10 -> ^U i.e. quit editor */

    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,

    "\023", "\004", "\005", "\030",
    "\021S","\021D","\003", "\022",
    "\010", "\007", "\026", "\026",
    "\016", "\021Y","\031", "\021Z",
};

/** 
 * replacement strings for the key codes,
 * version for EKEY
 */
static char
*raw2ekey [P4_NUM_KEYS] = 
{
    "\000", "\001", "\002", "\003", "\004",
    "\005", "\006", "\007", "\010", "\011",
    
    "\012", "\013", "\014", "\015", "\016",
    "\017", "\020", "\021", "\022", "\023",
    
    "\024", "\025", "\026", "\027",
    "\030", "\031", "\032", "\033",
    "\034", "\035", "\036", "\037",
    "\040", "\041", "\042", "\043",
};

/**
 * some hardcoded simple sequences
 */
char *p4_dumbterm_controls[] = 
{
    "",			/* cm - cursor move */
    "\f",		/* ho - home position */
    
    "\b",		/* le - cursor left */
    " ",		/* nd - right one column */
    "",			/* up - up one column */
    "\n",		/* do - down one column */
    
    "",			/* cl - clear screen and home */
    "",			/* cd - clear down */
    "",			/* ce - clear to end of line */
    "\a",		/* bl - bell */
    
    "",			/* dc - delete character in line */
    "",			/* dl - delete line from screen */
    
    "",			/* sf - scroll screen up */
    "",			/* sr - scroll screen down */
    
    "",			/* so - enter standout mode */
    "",			/* se - leave standout mode */
    "",			/* us - turn on underline mode */
    "",			/* ue - turn off underline mode */
    
    "",			/* md - enter double bright mode */
    "",			/* mr - enter reverse video mode */
    "",			/* mb - enter blinking mode */
    "",			/* me - turn off all appearance modes */
    
    "",			/* ks - make function keys transmit */
    ""			/* ke - make function keys work locally */
};

/**
 * Strings sent by function keys
 */
char *p4_dumbterm_rawkeys[] =
{
    "\033OP",			/* k1 - function keys 1 - 4 from vt100 */
    "\033OQ",			/* k2 */
    "\033OR",			/* k3 */
    "\033OS",			/* k4 */
    "\033[15~",			/* k5 - function keys 5 - 10 from xterm */
    "\033[17~",			/* k6 */
    "\033[18~",			/* k7 */
    "\033[19~",			/* k8 */
    "\033[20~",			/* k9 */
    "\033[21~",			/* k0 */
    
    "\033[23~",			/* F1 - function keys S-F1 thru S-F10 */
    "\033[24~",			/* F2 - stem from Linux console */
    "\033[25~",			/* F3 - or whoknowswhereelse */
    "\033[26~",			/* F4 */
    "\033[28~",			/* F5 */
    "\033[29~",			/* F6 */
    "\033[31~",			/* F7 */
    "\033[32~",			/* F8 */
    "\033[33~",			/* F9 */
    "\033[34~",			/* FA */
    
    "\033OD",			/* kl - arrow left */
    "\033OC",			/* kr - arrow right */
    "\033OA",			/* ku - arrow up */
    "\033OB",			/* kd - arrow down */
    
    "\033[1~",			/* kh - home key */
    "\033[4~",			/* kH - home down key (end key) */
    "\033[6~",			/* kN - next page */
    "\033[5~",			/* kP - previous page */
    
    "\b",			/* kb - backspace key */
    "\033[3~",			/* kD - delete character key */
    NULL,			/* kM - exit insert mode key */
    "\033[2~",			/* kI - insert character key */
    
    NULL,			/* kA - insert line key */
    NULL,			/* kE - clear end of line key */
    NULL,			/* kL - delete line key */
    NULL,			/* kC - clear screen key */
};

static int
get_remap_key (p4char *replace[])
{
    int i, n, m;

    if (PFE.keyptr)
    {  
        if (*PFE.keyptr == '\0')
            PFE.keyptr = NULL;
        else
            return *PFE.keyptr++;
    }
    n = 0;
    for (;;)
    {  
        m = -1;
	/* get a key from terminal */
	if (PFE.term->c_getvkey)
	{
	    i = PFE.term->c_getvkey();
	    if (i > 255)
 	    {   /* if special key, drop keybuf, return r-vkey */
		PFE.keyptr = replace[i&255];
		return *PFE.keyptr++ | 0x100;
	    }else{
		/* if no special key, continue as if we had called _getkey */
		PFE.keybuf[n++] = (p4char) i;
	    }
	}else{
	    PFE.keybuf[n++] = p4_getkey ();
	}
	/* scan for esc-sequence */
        if (PFE.rawkey_string)
        {  
            for (i = m; ++i < P4_NUM_KEYS;)
            {  
                if (!PFE.rawkey_string[i] || !replace[i])
                    continue;
                if (memcmp (PFE.keybuf, PFE.rawkey_string[i], n) == 0)
                {  
                    if (PFE.rawkey_string[i][n] == '\0')
                    {  
			/* esc-sequence completly matched, return r-vkey */
                        PFE.keyptr = replace[i];
                        return *PFE.keyptr++ | 0x100;
                    }else{
                        m = i;
                    }
                }
            }
        }
	/* if no start of esc-sequence found, return what we have in keybuf */
        if (m < 0)
        {  
            PFE.keybuf[n] = '\0';
            PFE.keyptr = PFE.keybuf;
            return *PFE.keyptr++;
        }
    }
}

int
p4_getekey (void)
{
    int key  = get_remap_key ((p4char **) raw2ekey);

    if (key == P4_KEY_kb)
        return '\b';
    return key;
}

int
p4_getwskey (void)
{
    return get_remap_key ((p4char **) raw2ws) & ~0x100;
}

int
p4_ekeypressed (void)
{
    return (PFE.keyptr && *PFE.keyptr) || p4_keypressed ();
}

int
p4_isprint (int c)
{
    return
#     if defined PFE_CHARSET_IBMPC
        !isascii (c) || !iscntrl (c)	/* almost anything can be displayed */
#     else
        isprint (c)
#     if defined PFE_CHARSET_ISO
        || (0xA0 <= (p4char) c && (p4char) c <= 0xFF)
#     endif
#     endif
        ;
}

void				/* write character to screen */
p4_putc_printable (int c)	/* ensure visible result */
{
    if (p4_isprint (c))
    {
        p4_putc (c);
    }else{
        p4_dot_standout_on ();
        p4_putc ((p4char) c < ' ' ? c + '@' : '?');
        p4_dot_standout_off ();
    }
}

int
p4_change_case (int ch)
{
    if (ch >= 0x100)  return ch;
    if (islower (ch)) return toupper (ch);
    if (isupper (ch)) return tolower (ch);
# if defined PFE_CHARSET_ISO
    if (0xC0 <= (p4char)ch && (p4char)ch <= 0xDE) return ch + 0x20;
    if (0xE0 <= (p4char)ch && (p4char)ch <= 0xFE) return ch - 0x20;
# elif defined PFE_CHARSET_IBMPC
    {
        static char lower [] =
            "\x81\x82\x83\x84\x85\x86\x87\x88" /* IBM 437 lower case chars */
            "\x89\x8A\x8B\x8C\x8D\x91\x93\x94"
            "\x95\x96\x97\x98\xA0\xA1\xA2\xA3"
            "\xA4";
        static char upper [] =
            "\x9A\x90\x41\x8E\x41\x8F\x80\x45" /* IBM 437 upper case chars */
            "\x45\x45\x49\x49\x49\x92\x4F\x99"
            "\x4F\x55\x55\x59\x41\x49\x4F\x55"
            "\xA5";
        char *p;
        
        if ((p = strchr (lower, (char)ch))) return upper [p - lower];
        if ((p = strchr (upper, (char)ch))) return lower [p - upper];
    }
# endif
    return ch;
}


/*
 * hooks for window size change and job control:
 */



/*
 *  new terminal driver implementation
 */

int  p4_tty_interrupt_key (char ch) { return (*PFE.term->tty_interrupt_key)(ch); }
void p4_interactive_terminal () { (*PFE.term->interactive_terminal)(); }
void p4_system_terminal ()      { (*PFE.term->system_terminal)(); }
void p4_query_winsize ()        { (*PFE.term->query_winsize)(); }
int  p4_keypressed ()           { return (*PFE.term->c_keypressed)(); }
int  p4_getkey ()		{ return (*PFE.term->c_getkey)(); }

void p4_putc_noflush (char c)   { (*PFE.term->c_putc_noflush)(c); }
void p4_putc (char c)           { (*PFE.term->c_putc)(c); }
void p4_puts (const char* s)    { (*PFE.term->c_puts)(s); }
void p4_put_flush ()            { (*PFE.term->c_put_flush) (); }

void p4_gotoxy(int x, int y)	{ (*PFE.term->c_gotoxy)(x,y); }
void p4_wherexy(int* x, int* y)	{ (*PFE.term->c_wherexy)(x,y); }

void p4_goleft (void)		{ (*PFE.term->tput) (P4_TERM_GOLEFT); }
void p4_goright (void)		{ (*PFE.term->tput) (P4_TERM_GORIGHT); }
void p4_goup (void)		{ (*PFE.term->tput) (P4_TERM_GOUP); }
void p4_godown (void) 		{ (*PFE.term->tput) (P4_TERM_GODOWN); }

void p4_dot_home (void)		{ (*PFE.term->tput) (P4_TERM_HOME); }
void p4_dot_clrscr (void)	{ (*PFE.term->tput) (P4_TERM_CLRSCR); }
void p4_dot_clreol (void)	{ (*PFE.term->tput) (P4_TERM_CLREOL); }
void p4_dot_clrdown (void)	{ (*PFE.term->tput) (P4_TERM_CLRDOWN); }
void p4_dot_bell (void)		{ (*PFE.term->tput) (P4_TERM_BELL); }

void p4_dot_normal (void)      	{ (*PFE.term->tput) (P4_TERM_NORMAL); }
void p4_dot_standout_on (void) 	{ (*PFE.term->tput) (P4_TERM_BOLD_ON); }
void p4_dot_standout_off (void) { (*PFE.term->tput) (P4_TERM_BOLD_OFF); }
void p4_dot_bright (void)      	{ (*PFE.term->tput) (P4_TERM_BRIGHT); }
void p4_dot_reverse (void)    	{ (*PFE.term->tput) (P4_TERM_REVERSE); }
void p4_dot_blinking (void)    	{ (*PFE.term->tput) (P4_TERM_BLINKING); }
void p4_dot_underline_on (void) { (*PFE.term->tput) (P4_TERM_UNDERLINE_ON); }
void p4_dot_underline_off (void){ (*PFE.term->tput) (P4_TERM_UNDERLINE_OFF); }



/* this one is called *before* loading other things in p4_main */
int p4_prepare_terminal ()
{
#   ifdef _K12_SOURCE
#   define p4_term_ios p4_term_k12
#   endif

    if (!PFE.term) PFE.term = &p4_term_ios;

    PFE.on_stop =	p4_system_terminal;
    PFE.on_continue =	p4_interactive_terminal;
    PFE.on_winchg =	p4_query_winsize;
  
    if (PFE.term->rawkey_string) 
        PFE.rawkey_string = PFE.term->rawkey_string;
    else 
        PFE.rawkey_string = p4_dumbterm_rawkeys;

    if (PFE.term->control_string) 
        PFE.control_string = PFE.term->control_string;
  
    if (PFE.term->init) return (*PFE.term->init)(); else return 1;
}

void p4_cleanup_terminal ()
{
    if (PFE.term->fini) (*PFE.term->fini)();
    
    PFE.term = 0;
}

/*@}*/
