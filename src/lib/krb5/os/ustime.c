/*
 * $Source$
 * $Author$
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_mstimeofday for BSD 4.3
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_mstime_c[] =
"$Id$";
#endif	/* lint || SABER */

#include <krb5/copyright.h>

#include <krb5/config.h>
#include <krb5/krb5.h>
#include <krb5/libos-proto.h>
#include <krb5/sysincl.h>

extern int errno;

krb5_error_code
krb5_us_timeofday(seconds, microseconds)
register krb5_int32 *seconds, *microseconds;
{
    struct timeval tv;

    if (gettimeofday(&tv, (struct timezone *)0) == -1) {
	/* failed, return errno */
	return (krb5_error_code) errno;
    }
    *seconds = tv.tv_sec;
    *microseconds = tv.tv_usec;
    return 0;
}
