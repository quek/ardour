#include <stdio.h>

#include "aaf/debug.h"

#include "aaf/AAFCore.h"
#include "aaf/AAFIParser.h"
#include "aaf/AAFIface.h"
#include "aaf/LibCFB.h"

#include "aaf/utils.h"

struct dbg*
laaf_new_debug (void)
{
	struct dbg* dbg = calloc (sizeof (struct dbg), sizeof (char));

	dbg->debug_callback = &laaf_debug_callback;
	dbg->fp             = stdout;

	return dbg;
}

void
laaf_free_debug (struct dbg* dbg)
{
	if (dbg->_dbg_msg) {
		free (dbg->_dbg_msg);
	}

	free (dbg);
}

void
laaf_debug_callback (struct dbg* dbg, void* ctxdata, int libid, int type, const char* srcfile, const char* srcfunc, int lineno, const char* msg, void* user)
{
	AAF_Iface* aafi = NULL;
	AAF_Data*  aafd = NULL;
	CFB_Data*  cfbd = NULL;

	const char* lib     = "";
	const char* typestr = "";
	const char* color   = "";

	if (dbg->fp == NULL) {
		return;
	}

	switch (libid) {
		case DEBUG_SRC_ID_LIB_CFB:
			lib  = "libCFB";
			aafi = (AAF_Iface*)ctxdata;
			break;
		case DEBUG_SRC_ID_AAF_CORE:
			lib  = "AAFCore";
			aafd = (AAF_Data*)ctxdata;
			break;
		case DEBUG_SRC_ID_AAF_IFACE:
			lib  = "AAFIface";
			cfbd = (CFB_Data*)ctxdata;
			break;
		case DEBUG_SRC_ID_TRACE:
			lib  = "trace";
			aafi = (AAF_Iface*)ctxdata;
			break;
		case DEBUG_SRC_ID_DUMP:
			lib = "dump";
			break;
	}

	switch (type) {
		case VERB_ERROR:
			typestr = " error ";
			color   = ANSI_COLOR_RED;
			break;
		case VERB_WARNING:
			typestr = "warning";
			color   = ANSI_COLOR_YELLOW;
			break;
		case VERB_DEBUG:
			typestr = " debug ";
			color   = ANSI_COLOR_DARKGREY;
			break;
	}

	if (libid != DEBUG_SRC_ID_TRACE && libid != DEBUG_SRC_ID_DUMP) {
		fprintf (dbg->fp, "[%s%s%s] ", color, typestr, ANSI_COLOR_RESET);
		fprintf (dbg->fp, "%s%s:%i in %s()%s : ", ANSI_COLOR_DARKGREY, srcfile, lineno, srcfunc, ANSI_COLOR_RESET);
	}

	fprintf (dbg->fp, "%s\n", msg);

	/* avoids -Wunused-parameter -Wunused-but-set-variable */
	(void)aafi;
	(void)aafd;
	(void)cfbd;
	(void)lib;
	(void)user;
}
