/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2012  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "playerrelations.h"

#include <string>

class ChatTab;

extern ChatTab *localChatTab;

#define BOOLEAN_OPTIONS _("Options to /%s are \"yes\", \"no\", \"true\", "\
"\"false\", \"1\", \"0\".")

/**
 * A class to parse and handle user commands
 */
class CommandHandler
{
    public:
        /**
         * Constructor
         */
        CommandHandler();

        /**
         * Destructor
         */
        ~CommandHandler()
        { }

        /**
         * Parse and handle the given command.
         */
        void handleCommand(const std::string &command,
                           ChatTab *tab = localChatTab);

        void handleCommands(const std::string &command,
                            ChatTab *tab = localChatTab);

        void replaceVars(std::string &str);

        static char parseBoolean(const std::string &value);

    protected:
        friend class ChatTab;
        friend class WhisperTab;

        /**
         * Handle an announce command.
         */
        void handleAnnounce(const std::string &args, ChatTab *tab);

        /**
         * Handle a help command.
         */
        void handleHelp(const std::string &args, ChatTab *tab);

        /**
         * Handle a where command.
         */
        void handleWhere(const std::string &args, ChatTab *tab);

        /**
         * Handle a who command.
         */
        void handleWho(const std::string &args, ChatTab *tab);

        /**
         * Handle a msg command.
         */
        void handleMsg(const std::string &args, ChatTab *tab);

        /**
         * Handle a msg tab request.
         */
        void handleQuery(const std::string &args, ChatTab *tab);

        /**
         * Handle a join command.
         */
        void handleJoin(const std::string &args, ChatTab *tab);

        /**
         * Handle a listchannels command.
         */
        void handleListChannels(const std::string &args, ChatTab *tab);

        /**
         * Handle a clear command.
         */
        void handleClear(const std::string &args, ChatTab *tab);

        /**
         * Handle a createparty command.
         */
        void handleCreateParty(const std::string &args, ChatTab *tab);

        /**
         * Handle a createguild command.
         */
        void handleCreateGuild(const std::string &args, ChatTab *tab);

        /**
         * Handle a party command.
         */
        void handleParty(const std::string &args, ChatTab *tab);

        /**
         * Handle a me command.
         */
        void handleMe(const std::string &args, ChatTab *tab);

        /**
         * Handle a toggle command.
         */
        void handleToggle(const std::string &args, ChatTab *tab);

        /**
         * Handle a present command.
         */
        void handlePresent(const std::string &args, ChatTab *tab);

        /**
         * Handle an ignore command.
         */
        void handleIgnore(const std::string &args, ChatTab *tab);

        /**
         * Handle an unignore command.
         */
        void handleUnignore(const std::string &args, ChatTab *tab);

        /**
         * Handle an friend command.
         */
        void handleFriend(const std::string &args, ChatTab *tab);

        /**
         * Handle an disregard command.
         */
        void handleDisregard(const std::string &args, ChatTab *tab);

        /**
         * Handle an neutral command.
         */
        void handleNeutral(const std::string &args, ChatTab *tab);

        /**
         * Handle an blacklist command.
         */
        void handleBlackList(const std::string &args, ChatTab *tab);

        /**
         * Handle an enemy command.
         */
        void handleEnemy(const std::string &args, ChatTab *tab);

        /**
         * Handle an erase command.
         */
        void handleErase(const std::string &args, ChatTab *tab);

        /**
         * Change relation.
         */
        void changeRelation(const std::string &args,
                            PlayerRelation::Relation relation,
                            const std::string &relationText, ChatTab *tab);

        /**
         * Handle a quit command.
         */
        void handleQuit(const std::string &args, ChatTab *tab);

        /**
         * Handle show all command.
         */
        void handleShowAll(const std::string &args, ChatTab *tab);

        /**
         * Handle move command.
         */
        void handleMove(const std::string &args, ChatTab *tab);

        /**
         * Handle target command.
         */
        void handleTarget(const std::string &args, ChatTab *tab);

        /**
         * Handle closeall command.
         */
        void handleCloseAll(const std::string &args, ChatTab *tab);

        /**
         * Handle ignoreall command.
         */
        void handleIgnoreAll(const std::string &args, ChatTab *tab);

        /**
         * Handle outfit command.
         */
        void handleOutfit(const std::string &args, ChatTab *tab);

        /**
         * Handle emote command.
         */
        void handleEmote(const std::string &args, ChatTab *tab);

        /**
         * Handle away command.
         */
        void handleAway(const std::string &args, ChatTab *tab);

        /**
         * Handle pseudo away command.
         */
        void handlePseudoAway(const std::string &args, ChatTab *tab);

        /**
         * Handle follow command.
         */
        void handleFollow(const std::string &args, ChatTab *tab);

        /**
         * Handle imitation command.
         */
        void handleImitation(const std::string &args, ChatTab *tab);

        /**
         * Handle heal command.
         */
        void handleHeal(const std::string &args, ChatTab *tab);

        /**
         * Handle navigate command.
         */
        void handleNavigate(const std::string &args, ChatTab *tab);

        void handleMail(const std::string &args, ChatTab *tab);

        void handleHack(const std::string &args, ChatTab *tab);

        void handlePriceLoad(const std::string &args, ChatTab *tab);

        void handlePriceSave(const std::string &args, ChatTab *tab);

        void handleTrade(const std::string &args, ChatTab *tab);

        void handleDisconnect(const std::string &args, ChatTab *tab);

        void handleUndress(const std::string &args, ChatTab *tab);

        void handleAttack(const std::string &args, ChatTab *tab);

        void handleDirs(const std::string &args, ChatTab *tab);

        void handleInfo(const std::string &args, ChatTab *tab);

        void handleWait(const std::string &args, ChatTab *tab);

        void handleUptime(const std::string &args, ChatTab *tab);

        void handleAddAttack(const std::string &args, ChatTab *tab);

        void handleAddPriorityAttack(const std::string &args, ChatTab *tab);

        void handleRemoveAttack(const std::string &args, ChatTab *tab);

        void handleAddIgnoreAttack(const std::string &args, ChatTab *tab);

        void handleServerIgnoreAll(const std::string &args, ChatTab *tab);

        void handleServerUnIgnoreAll(const std::string &args, ChatTab *tab);

        void handleSetDrop(const std::string &args, ChatTab *tab);

        void handleError(const std::string &args, ChatTab *tab);

        void handleDump(const std::string &args, ChatTab *tab);

        void handleDumpGraphics(const std::string &args, ChatTab *tab);

        void handleDumpTests(const std::string &args, ChatTab *tab);

        void handleDumpOGL(const std::string &args, ChatTab *tab);

        void outString(ChatTab *tab, const std::string &str,
                       const std::string &def);

        void outStringNormal(ChatTab *tab, const std::string &str,
                             const std::string &def);

        void handleCacheInfo(const std::string &args, ChatTab *tab);

        bool parse2Int(const std::string &args, int *x, int *y);
};

extern CommandHandler *commandHandler;

#endif // COMMANDHANDLER_H
