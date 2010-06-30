/*
 * Copyright (c) 2009 Celestin, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains a BService SAY and ACT which can make
 * botserv bot talk in channel.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"botserv/bottalk", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Rizon Development Group <http://dev.rizon.net>"
);

static void bs_cmd_say(sourceinfo_t *si, int parc, char *parv[]);
static void bs_cmd_act(sourceinfo_t *si, int parc, char *parv[]);

command_t bs_say = { "SAY", N_("Makes the bot say the given text on the given channel."), AC_NONE, 2, bs_cmd_say };
command_t bs_act = { "ACT", N_("Makes the bot do the equivalent of a \"/me\" command."), AC_NONE, 2, bs_cmd_act };


list_t *bs_cmdtree;
list_t *bs_helptree;

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(bs_cmdtree, "botserv/main", "bs_cmdtree");
	MODULE_USE_SYMBOL(bs_helptree, "botserv/main", "bs_helptree");

	command_add(&bs_say, bs_cmdtree);
	command_add(&bs_act, bs_cmdtree);

	help_addentry(bs_helptree, "SAY", "help/botserv/say", NULL);
	help_addentry(bs_helptree, "ACT", "help/botserv/act", NULL);
}

void _moddeinit()
{
	command_delete(&bs_say, bs_cmdtree);
	command_delete(&bs_act, bs_cmdtree);

	help_delentry(bs_helptree, "SAY");
	help_delentry(bs_helptree, "ACT");
}

static void bs_cmd_say(sourceinfo_t *si, int parc, char *parv[])
{
	char *channel = parv[0];
	char *message = parv[1];
	channel_t *c = channel_find(channel);
	mychan_t *mc = mychan_find(channel);
	metadata_t *bs;
	user_t *bot;

	if (!channel || !message)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SAY");
		command_fail(si, fault_needmoreparams, _("Syntax: SAY <#channel> <msg>"));
		return;
	}

	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), channel);
		return;
	}

	if (!c)
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is currently empty."), channel);
		return;
	}

	if (!si->smu)
	{
		command_fail(si, fault_noprivs, _("You are not logged in."));
		return;
	}

	if (!(chanacs_source_flags(mc, si) & (CA_OP | CA_AUTOOP)))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if ((bs = metadata_find(mc, "private:botserv:bot-assigned")) != NULL)
		bot = user_find_named(bs->value);
	else
		bot = NULL;
	if (bot == NULL)
	{
		command_fail(si, fault_nosuch_key, _("\2%s\2 does not have a bot assigned."), mc->name);
		return;
	}

	msg(bot->nick, channel, "%s", message);
	logcommand(si, CMDLOG_DO, "SAY: \2%s\2", message);
}

static void bs_cmd_act(sourceinfo_t *si, int parc, char *parv[])
{
	char *channel = parv[0];
	char *message = parv[1];
	channel_t *c = channel_find(channel);
	mychan_t *mc = mychan_find(channel);
	metadata_t *bs;
	user_t *bot;

	if (!channel)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "SAY");
		command_fail(si, fault_needmoreparams, _("Syntax: ACT <#channel> <msg>"));
		return;
	}

	if (!mc)
	{
		command_fail(si, fault_nosuch_target, _("Channel \2%s\2 is not registered."), channel);
		return;
	}

	if (!c)
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is currently empty."), channel);
		return;
	}

	if (!si->smu)
	{
		command_fail(si, fault_noprivs, _("You are not logged in."));
		return;
	}

	if (!(chanacs_source_flags(mc, si) & (CA_OP | CA_AUTOOP)))
	{
		command_fail(si, fault_noprivs, _("You are not authorized to perform this operation."));
		return;
	}

	if ((bs = metadata_find(mc, "private:botserv:bot-assigned")) != NULL)
		bot = user_find_named(bs->value);
	else
		bot = NULL;
	if (bot == NULL)
	{
		command_fail(si, fault_nosuch_key, _("\2%s\2 does not have a bot assigned."), mc->name);
		return;
	}

	msg(bot->nick, channel, "\001ACTION %s\001", message);
	logcommand(si, CMDLOG_DO, "ACT: \2%s\2", message);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */


