dnl
dnl
dnl How do we find other scripts needed for configuration?
dnl Scripts like Cygnus configure, config.sub, config.guess are stored
dnl together in one directory.  For now, have the configure.in file
dnl specify it explicitly with AC_CONFIG_AUX.  We'll provide a half-way
dnl acceptable default of ${srcdir}.
dnl
define(AC_CONFIG_AUX,[
  if test -f $1/config.sub ; then
    config_aux=$1
  else
    config_aux=${srcdir}/$1
  fi
  config_sub=${config_aux}/config.sub
  config_guess=${config_aux}/config.guess
])dnl
dnl
define(AC__CONFIG_AUX,[
  if test "z${config_sub}" = "z" ; then
    config_sub=${srcdir}/config.sub
  fi
  if test "z${config_guess}" = "z" ; then
    config_guess=${srcdir}/config.guess
  fi
AC_PROVIDE([$0])dnl
])dnl
dnl
dnl Does configure need to be run in immediate subdirectories of this
dnl directory?
dnl
define(CONFIG_DIRS,[
AC_REQUIRE([AC__CONFIG_AUX])
changequote(<<<,>>>)dnl
SUBDIRS="$1"
if [ -z "${norecursion}" ] ; then
	for configdir in $1 ; do

		if [ -d ${srcdir}/${configdir} ] ; then
			eval echo Configuring ${configdir}... ${redirect}
			case "${srcdir}" in
			".") ;;
			*)
				if [ ! -d ./${configdir} ] ; then
					if mkdir ./${configdir} ; then
						true
					else
						echo '***' "${progname}: could not make `pwd`/${configdir}" 1>&2
						exit 1
					fi
				fi
				;;
			esac

			POPDIR=`pwd`
			cd ${configdir}

### figure out what to do with srcdir
			case "${srcdir}" in
			".") newsrcdir=${srcdir} ;; # no -srcdir option.  We're building in place.
			/*) # absolute path
				newsrcdir=${srcdir}/${configdir}
				srcdiroption="--srcdir=${newsrcdir}"
				;;
			*) # otherwise relative
				newsrcdir=../${srcdir}/${configdir}
				srcdiroption="--srcdir=${newsrcdir}"
				;;
			esac

### check for guested configure, otherwise get Cygnus style configure
### script from ${config_aux}
			if [ -f ${newsrcdir}/configure ] ; then
				recprog=${newsrcdir}/configure
			elif [ -f ${newsrcdir}/configure.in ] ; then
				recprog=${config_aux}/configure
			else
				eval echo No configuration information in ${configdir} ${redirect}
				recprog=
			fi

### The recursion line is here.
			if [ ! -z "${recprog}" ] ; then
				if eval ${config_shell} ${recprog} ${verbose} ${buildopt} --host=${host_alias} --target=${target_alias} \
					${prefixoption} ${tmpdiroption} ${exec_prefixoption} \
					${srcdiroption} ${program_prefixoption} ${program_suffixoption} ${program_transform_nameoption} ${site_option} ${withoptions} ${withoutoptions} ${removing} ${redirect} ; then
					true
				else
					echo Configure in `pwd` failed, exiting. 1>&2
					exit 1
				fi
			fi

			cd ${POPDIR}
		fi
	done
fi
changequote([,])dnl
AC_SUBST(SUBDIRS)dnl
])dnl
dnl
dnl append subdir rule -- MAKE_SUBDIRS("making",all)
dnl
define(MAKE_SUBDIRS,[
changequote(<<<,>>>)dnl
divert(9)dnl

$2::<<<
	@case '${MFLAGS}' in *[ik]*) set +e ;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo>>> $1 <<<"in $(CURRENT_DIR)/$$i..."; \
			$(MAKE) $(MFLAGS) CURRENT_DIR=$(CURRENT_DIR)/$$i >>>$2<<<); \
	done>>>
divert(0)dnl
changequote([,])dnl
])dnl
dnl
dnl check for yylineno -- HAVE_YYLINENO
dnl
define(HAVE_YYLINENO,[
AC_COMPILE_CHECK([yylineno],
[extern int yylineno;], [t(&yylineno);], , AC_DEFINE(NO_YYLINENO))])dnl
dnl
dnl take saved makefile stuff and put it in the Makeile
dnl
define(EXTRA_RULES,[
cat >> Makefile <<"SUBDIREOF"
undivert(9)
SUBDIREOF
])dnl
dnl
dnl drop in standard configure rebuild rules -- CONFIG_RULES
dnl
define(CONFIG_RULES,[
divert(9)dnl
[
Makefile: Makefile.in config.status
	$(SHELL) config.status
config.status: configure
	$(SHELL) config.status --recheck
configure: configure.in
	cd $(srcdir); autoconf
]
divert(0)dnl
])dnl
dnl
dnl check for sys_errlist -- DECLARE_SYS_ERRLIST
dnl
define(DECLARE_SYS_ERRLIST,[
AC_COMPILE_CHECK([sys_errlist declaration],
[#include <errno.h>], [sys_nerr;], , AC_DEFINE(NEED_SYS_ERRLIST))])dnl
dnl
dnl check for <stdarg.h> -- CHECK_STDARG
dnl (name used for consistency with krb5/config.h)
dnl
define(CHECK_STDARG,[
AC_HEADER_CHECK(stdarg.h,AC_DEFINE(STDARG_PROTOTYPES))])dnl
dnl
define(AC_PROG_ARCHIVE, [AC_PROGRAM_CHECK(ARCHIVE, ar, ar qv, false)])dnl
define(AC_PROG_ARCHIVE_ADD, [AC_PROGRAM_CHECK(ARADD, ar, ar cruv, false)])dnl
dnl
dnl drop in rules for building error tables -- ET_RULES
dnl
define(ET_RULES,[
AC_PROG_AWK dnl
divert(9)dnl
[

SRCTOP=$(srcdir)/$(BUILDTOP)
### /* these are invoked as $(...) foo.et, which works, but could be better */
COMPILE_ET_H= $(AWK) -f $(SRCTOP)/util/et/et_h.awk outfile=$@
COMPILE_ET_C= $(AWK) -f $(SRCTOP)/util/et/et_c.awk outfile=$@
.SUFFIXES:  .h .c .et .ct

.et.h:
	$(AWK) -f $(SRCTOP)/util/et/et_h.awk outfile=$][*.h $<

.et.c:
	$(AWK) -f $(SRCTOP)/util/et/et_c.awk outfile=$][*.c $<

]
divert(0)dnl
])dnl
dnl
dnl drop in rules for building command tables -- SS_RULES
dnl
define(SS_RULES,[
divert(9)dnl
changequote({,})dnl
{

MAKE_COMMANDS= $(BUILDTOP)/util/ss/mk_cmds
.SUFFIXES:  .h .c .et .ct

.ct.c:
	if [ $< != $}{*.ct ]; then \
		(cp $< $}{*.ct && $(MAKE_COMMANDS) $}{*.ct && rm $}{*.ct) || exit 1; \
	else \
		$(MAKE_COMMANDS) $}{*.ct || exit 1; \
	fi

}
changequote([,])dnl
divert(0)dnl
])dnl
dnl
dnl check for <dirent.h> -- CHECK_DIRENT
dnl (may need to be more complex later)
dnl
define(CHECK_DIRENT,[
AC_HEADER_CHECK(dirent.h,AC_DEFINE(USE_DIRENT_H))])dnl
dnl
dnl check if sys/fcntl.h is needed for O_* -- CHECK_FCNTL
dnl
define(CHECK_FCNTL,[
AC_COMPILE_CHECK([O_RDONLY from sys/file.h],
[#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>],
[O_RDONLY;], ,AC_COMPILE_CHECK([O_RDONLY from sys/fcntl.h],
[#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/fcntl.h>],
[O_RDONLY;],AC_DEFINE(NEED_SYS_FCNTL_H)))])dnl
dnl
dnl check if union wait is defined, or if WAIT_USES_INT -- CHECK_WAIT_TYPE
dnl
define(CHECK_WAIT_TYPE,[
AC_COMPILE_CHECK([union wait],
[#include <sys/wait.h>], [union wait i;], , AC_DEFINE(WAIT_USES_INT))])dnl
dnl
dnl set $(KRB5ROOT) from --with-krb5-root=value -- WITH_KRB5ROOT
dnl
define(WITH_KRB5ROOT,[
AC_WITH([krb5-root],
echo "krb5-root is $withval"
KRB5ROOT=$withval,
echo "krb5-root defaults to /krb5"
KRB5ROOT=/krb5)dnl
AC_SUBST(KRB5ROOT)])dnl
dnl
dnl Imake LinkFile rule, so they occur in the right place -- LinkFile(dst,src)
dnl
define(LinkFile,[
AC_LN_S
changequote({,})dnl
divert(9)dnl

$1:: $2{
	$(RM) $}{@
	$(LN) $}{? $}{@

}divert(0)dnl
changequote([,])dnl
])dnl
dnl
dnl explicit append text (for non-general things) -- AppendRule(txt)
dnl
define(AppendRule,[
divert(9)dnl

$1

divert(0)dnl
])dnl
dnl
dnl create DONE file for lib/krb5 -- SubdirLibraryRule(list)
define(SubdirLibraryRule,[
divert(9)dnl

all:: DONE

DONE:: $1
	echo $1 > [$]@

clean::
	$(RM) DONE
divert(0)dnl
])dnl
dnl
dnl copy header file into include dir -- CopyHeader(hfile,hdir)
dnl
define(CopyHeader,[
divert(9)dnl

includes:: $1
	-if cmp $1 $2/$1 >/dev/null 2>&1; then \
		echo ; \
	else \
		[$](RM) $2/$1 ; \
		[$](CP) $1 $2/$1; \
	fi

divert(0)dnl
])dnl
dnl
dnl Krb5InstallHeaders(headers,destdir)
define(Krb5InstallHeaders,[
divert(9)dnl
install:: $1
	@set -x; for f in $1 ; \
	do [$](INSTALL) -c [$](INSTDATFLAGS) [$$]f $2/[$$]f ; \
	done
divert(0)dnl
])dnl
dnl
dnl PepsyTarget(basename)
dnl
define(PepsyTarget,[
divert(9)
.SUFFIXES:	.py
$1_defs.h $1_pre_defs.h $1-types.h $1_tables.c: $1-asn.py
	@echo '***Ignore the warning message "Warning: Can'"'"'t find UNIV.ph failed"'
	[$](PEPSY) [$](PSYFLAGS) [$](srcdir)/$1-asn.py

divert(0)dnl
])dnl
dnl
define(UsePepsy,[
echo "using pepsy"
PSYFLAGS="-f -h0 -a -s -C"
PEPSY="$(BUILDTOP)/isode/pepsy/xpepsy"
AC_SUBST(PEPSY)dnl
AC_SUBST(PSYFLAGS)dnl
])dnl
dnl
dnl arbitrary DEFS -- ADD_DEF(value)
dnl
define(ADD_DEF,[
DEFS="[$]DEFS "'$1'
])dnl
dnl
dnl local includes are used -- KRB_INCLUDE
dnl
define(KRB_INCLUDE,[
ADD_DEF([-I${SRCTOP}/include -I${BUILDTOP}/include])dnl
])dnl
dnl
dnl ISODE/pepsy includes are used -- ISODE_INCLUDE
dnl
define(ISODE_INCLUDE,[
ADD_DEF([-I${SRCTOP}/isode/h -I${BUILDTOP}/isode/h])dnl
])dnl
dnl
