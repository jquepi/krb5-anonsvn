/* strb2bitstr.c - string of bits to bit string */

#ifndef	lint
static char *rcsid = "$Header$";
#endif

/* 
 * $Header$
 *
 *
 * $Log$
 * Revision 1.1  1994/06/10 03:35:11  eichin
 * autoconfed isode for kerberos work
 *
 * Revision 1.1  1994/06/01 00:39:07  eichin
 * add psap too
 *
 * Revision 8.0  91/07/17  12:47:26  isode
 * Release 7.0
 * 
 * 
 */

/*
 *				  NOTICE
 *
 *    Acquisition, use, and distribution of this module and related
 *    materials are subject to the restrictions of a license agreement.
 *    Consult the Preface in the User's Manual for the full terms of
 *    this agreement.
 *
 */


/* LINTLIBRARY */

#include <stdio.h>
#include "psap.h"

/*  */

PE	strb2bitstr (cp, len, class, id)
register char  *cp;
register int     len;
PElementClass class;
PElementID id;
{
    register int    i,
		    j,
                    bit,
		    mask;
    register PE	    p;

    if ((p = pe_alloc (class, PE_FORM_PRIM, id)) == NULLPE)
	return NULLPE;

    p = prim2bit (p);
    if (len > 0 && bit_off (p, len - 1) == NOTOK) {
no_mem: ;
        pe_free (p);
        return NULLPE;
    }

    for (bit = (*cp & 0xff), i = 0, mask = 1 << (j = 7); i < len; i++) {
	if ((bit & mask) && bit_on (p, i) == NOTOK)
	    goto no_mem;
	if (j-- == 0)
	    bit = *++cp & 0xff, mask = 1 << (j = 7);
	else
	    mask >>= 1;
    }

    return p;
}
