/*
 * $Source$
 * $Author$
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Glue between Kerberos version and ISODE 6.0 version of structures.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_arep2karep_c[] =
"$Id$";
#endif	/* lint || saber */

#include <krb5/copyright.h>
#include <krb5/krb5.h>

/*#include <time.h> */
#include <isode/psap.h>
#include <krb5/asn1.h>
#include "asn1glue.h"

#include <krb5/ext-proto.h>

/* ISODE defines max(a,b) */

krb5_ap_rep_enc_part *
KRB5_EncAPRepPart2krb5_ap_rep_enc_part(val, error)
const register struct type_KRB5_EncAPRepPart *val;
register int *error;
{
    register krb5_ap_rep_enc_part *retval;
 
    retval = (krb5_ap_rep_enc_part *)xmalloc(sizeof(*retval));
    if (!retval) {
	*error = ENOMEM;
	return(0);
    }
    xbzero(retval, sizeof(*retval));

    retval->ctime = gentime2unix(val->ctime, error);
    if (*error) {
	xfree(retval);
	return(0);
    }	
    retval->cusec = val->cusec;

    if (val->subkey) {
	retval->subkey = KRB5_EncryptionKey2krb5_keyblock(val->subkey,
							  error);
	if (!retval->subkey) {
	    krb5_free_ap_rep_enc_part(retval);
	    return 0;
	}
    }
    if (val->optionals & opt_KRB5_EncAPRepPart_seq__number)
	retval->seq_number = val->seq__number;

    return(retval);
}
