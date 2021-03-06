/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#ifndef GUILDHANDLER_H
#define GUILDHANDLER_H

#include "guild.h"

#include <iosfwd>

class Being;

namespace Net
{

class GuildHandler
{
    public:
        virtual ~GuildHandler()
        { }

        virtual bool isSupported()
        { return false; }

        virtual void create(const std::string &name) = 0;

        virtual void invite(int guildId, const std::string &name) = 0;

        virtual void invite(int guildId, Being *being) = 0;

        virtual void inviteResponse(int guildId, bool response) = 0;

        virtual void leave(int guildId) = 0;

        virtual void kick(GuildMember *member, std::string reason = "") = 0;

        virtual void chat(int guildId, const std::string &text) = 0;

        virtual void memberList(int guildId) = 0;

        virtual void info(int guildId) = 0;

        virtual void changeMemberPostion(GuildMember *member, int level) = 0;

        virtual void requestAlliance(int guildId, int otherGuildId) = 0;

        virtual void requestAllianceResponse(int guildId, int otherGuildId,
                                             bool response) = 0;

        virtual void endAlliance(int guildId, int otherGuildId) = 0;

        virtual void changeNotice(int guildId, std::string msg1,
                                  std::string msg2) = 0;

        virtual void clear() = 0;
};

}

#endif // GUILDHANDLER_H
