/**
 * (C) 2000 - 2001 Guido Draheim
 *
 * lisence: GNU LGPL, non-substantial parts may promote to 
 *                    any opensource.org approved license.
 *
 * @description
 *      "struct" implements neon/mops/mpe-like structures.
 *      "structs" implements fsl/mforth-like structures.
 *
 *      the two wordsets are designed to let the sub-words 
 *      to be used interchangably both inside STRUCT and
 *      STRUCTURE definitions. They will also work inside
 *      pfe's class-definitions btw.
 *
 *      The provided words try to be compatible
 *      with the simple implementation guidelines as 
 *      provided in the survey at the comp.lang.forth.repository
 *      (http://forth.sourceforge.net/word/structure)
 *      and the documentation on MPE' forth's implementation 
 *      (/vol/c/Programme/PfwVfx/Doc/VfxMan.Htm/struct.html)
 *      and the structs-source of the Forth Scientific Library
 *      (lib/fsl/structs.fth)
 *
 *   field-layout
 *      PFA[0] has the offset (elsewhere for the method-table)
 *      PFA[1] has the sizeof (may serve as minimalistic type-id)
 *
 *   struct-layout
 *      PFA[0] unused (elswehere method-table or type-id)
 *      PFA[1] has the sizeof (that is instantiated)
 *    
 *   therefore SIZEOF is designed to give a nice result in
 *   both places.
 */

#define _P4_SOURCE 1

#include <pfe/pfe-base.h>
#include <pfe/struct-ext.h>

extern FCode (p4_offset_RT);

/* ----------------------------------------------------------------
 * the first part is the traditional stuff, and here's what
 * comes from the Forth Scientific Library - it's implemented
 * widely, including swiftforth
 */

/* CHAR: ( struct-offset "name" -- struct-offset' )
 : CHAR: CHAR FIELD ;
 */
FCode (p4_char_colon)
{
    p4_field (sizeof(p4char));
}

/* CELL: ( struct-offset "name" -- struct-offset' )
 : CELL: ALIGNED CELL FIELD ;
 */
FCode (p4_cell_colon)
{
    *SP = P4_ALIGNED(*SP);
    p4_field (sizeof(p4cell));
}

/* WCHAR: ( struct-offset "name" -- struct-offset' )
 : WCHAR: WALIGNED WCHAR FIELD ;
 */
FCode (p4_wchar_colon)
{
    *SP += *SP & 1;
    p4_field (sizeof(p4char) * 2);
}

/* DOUBLE: ( struct-offset "name" -- struct-offset' )
 : DOUBLE: ALIGNED DOUBLE FIELD ;
 */
FCode (p4_two_cell_colon)
{
    *SP = P4_ALIGNED(*SP);
    p4_field (sizeof(p4cell) * 2);
}

/* CHARS: ( struct-offset "name" -- struct-offset' )
 : CHARS: CHARS FIELD ;
 */
FCode (p4_chars_colon)
{
    p4_field (sizeof(p4char) * FX_POP);
}

/* CELLS: ( struct-offset "name" -- struct-offset' )
 : CELLS: CELLS SWAP ALIGNED SWAP FIELD ;
 */
FCode (p4_cells_colon)
{
    SP[1] = P4_ALIGNED(SP[1]);
    p4_field (sizeof(p4cell) * FX_POP);
}

/* WCHARS: ( struct-offset "name" -- struct-offset' )
 : WCHARS: WCHARS FIELD ;
 */
FCode (p4_wchars_colon)
{
    SP[1] += SP[1] & 1;
    p4_field (sizeof(p4char) * FX_POP);
}

/* FLOAT: ( struct-offset "name" -- struct-offset' )
 : FLOAT: FLOATS SWAP FALIGNED SWAP FIELD ;
 */
FCode (p4_float_colon)
{
    SP[1] = P4_SFALIGNED(SP[1]);
    p4_field (sizeof(double) * FX_POP);
}

P4_LISTWORDS(structs) =
{
    CO ("STRUCTURE:",		p4_structure),
    CO (";STRUCTURE",		p4_endstructure),

    CO ("CHAR:",		p4_char_colon),
    CO ("WCHAR:",		p4_wchar_colon),
    CO ("CELL:",		p4_cell_colon),
    CO ("DOUBLE:",		p4_two_cell_colon),
    CO ("FLOAT:",		p4_float_colon),

    CO ("CHARS:",		p4_chars_colon),
    CO ("WCHARS:",		p4_wchars_colon),
    CO ("CELLS:",		p4_cells_colon),

    CO ("INTEGER:",		p4_cell_colon),
    CO ("POINTER:",		p4_cell_colon),
    CO ("STRUCT:",		p4_field),
    CO ("ARRAY:",               p4_array_of),
};
P4_COUNTWORDS(structs, "STRUCTS - simple structure implementation 0");

P4_LOADLIST (structs) =
{
    P4_LOAD_INTO, "EXTENSIONS",
    &P4WORDS(structs),
    P4_LOAD_END
};
P4_MODULE_LIST (structs);

/*
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */