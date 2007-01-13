/*
 * Copyright (c) 2005-2006 William Pitcock <nenolod@nenolod.net> et al
 * Rights to this code are documented in doc/LICENSE.
 *
 * Rock Paper Scissors
 *
 * $Id: rps.c 7461 2007-01-13 11:59:40Z nenolod $
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"gameserv/rps", FALSE, _modinit, _moddeinit,
	"$Id: rps.c 7461 2007-01-13 11:59:40Z nenolod $",
	"Atheme Development Group <http://www.atheme.org>"
);

static void command_rps(sourceinfo_t *si, int parc, char *parv[]);

command_t cmd_rps = { "RPS", "Ask the 8-Ball a question.", AC_NONE, 0, command_rps };

list_t *gs_cmdtree;
list_t *cs_cmdtree;

void _modinit(module_t * m)
{
	MODULE_USE_SYMBOL(gs_cmdtree, "gameserv/main", "gs_cmdtree");
	MODULE_USE_SYMBOL(cs_cmdtree, "chanserv/main", "cs_cmdtree");	/* fantasy commands */

	command_add(&cmd_rps, gs_cmdtree);

	command_add(&cmd_rps, cs_cmdtree);
}

void _moddeinit()
{
	command_delete(&cmd_rps, gs_cmdtree);

	command_delete(&cmd_rps, cs_cmdtree);
}

/*
 * Handle reporting for both fantasy commands and normal commands in GameServ
 * quickly and easily. Of course, sourceinfo has a vtable that can be manipulated,
 * but this is quicker and easier...                                  -- nenolod
 */
static void gs_command_report(sourceinfo_t *si, char *fmt, ...)
{
	va_list args;
	char buf[BUFSIZE];

	va_start(args, fmt);
	vsnprintf(buf, BUFSIZE, fmt, args);
	va_end(args);

	if (si->c != NULL)
		msg(chansvs.nick, si->c->name, "%s", buf);
	else
		command_success_nodata(si, "%s", buf);
}

static void command_rps(sourceinfo_t *si, int parc, char *parv[])
{
	static char *rps_responses[3] = {
		"Rock",
		"Paper",
		"Scissors"
	};

	gs_command_report(si, "%s", rps_responses[rand() % 3]);
}
