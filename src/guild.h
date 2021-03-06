/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
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

#ifndef GUILD_H
#define GUILD_H

#include "avatar.h"

#include "gui/widgets/avatarlistbox.h"

#include "utils/dtor.h"
#include "utils/stringvector.h"

#include <map>

class Guild;

typedef std::map<unsigned, std::string> PositionsMap;

class GuildMember : public Avatar
{
public:
    Guild *getGuild() const
    { return mGuild; }

    int getPos() const
    { return mPos; }

    void setPos(int pos)
    { mPos = pos; }

    std::string getAdditionString() const;

protected:
    friend class Guild;

    GuildMember(Guild *guild, int id, int accountId, const std::string &name);

    GuildMember(Guild *guild, const std::string &name);

    Guild *mGuild;
    int mPos;
};

class Guild : public AvatarListModel
{
public:
    virtual ~Guild();

    /**
     * Set the guild's name.
     */
    void setName(const std::string &name)
    { mName = name; }

    /**
     * Adds member to the list.
     */
    GuildMember *addMember(int accountId, int charId, const std::string &name);

    /**
     * Adds member to the list.
     */
    GuildMember *addMember(const std::string &name);

    /**
     * Find a member by ID.
     *
     * @return the member with the given ID, or NULL if they don't exist.
     */
    GuildMember *getMember(int id) const;

    /**
     * Find a member by account ID and char ID.
     *
     * @return the member with the given ID, or NULL if they don't exist.
     */
    GuildMember *getMember(int accountId, int charId) const;

    /**
     * Find a member by name.
     *
     * @return the member with the given name, or NULL if they don't exist.
     */
    GuildMember *getMember(const std::string &name) const;

    /**
     * Get the name of the guild.
     * @return returns name of the guild
     */
    const std::string &getName() const
    { return mName; }

    /**
     * Get the id of the guild.
     * @return Returns the id of the guild
     */
    short getId() const
    { return mId; }

    /**
     * Removes a member from the guild.
     */
    void removeMember(GuildMember *member);

    /**
     * Removes a member from the guild.
     */
    void removeMember(int id);

    /**
     * Removes a member from the guild.
     */
    void removeMember(const std::string &name);

    void removeFromMembers();

    void clearMembers()
    { delete_all(mMembers); mMembers.clear(); }

    /**
     * Get size of members list.
     * @return Returns the number of members in the guild.
     */
    int getNumberOfElements()
    { return static_cast<int>(mMembers.size()); }

    Avatar *getAvatarAt(int i);

    /**
     * Get whether user can invite users to this guild.
     * @return Returns true if user can invite users
     */
    bool getInviteRights() const
    { return mCanInviteUsers; }

    void setRights(short rights);

    bool isMember(GuildMember *member) const;

    bool isMember(int id) const;

    bool isMember(const std::string &name) const;

    void getNames(StringVect &names) const;

    void addPos(int id, std::string name);

    void sort();

    std::string getPos(int id) const;

    static Guild *getGuild(short id);

    const PositionsMap &getPositions() const
    { return mPositions; }

    void setEmblemId(int id)
    { mEmblemId = id; }

    int getEmblemId() const
    { return mEmblemId; }

    static void clearGuilds();

    void setServerGuild(bool b)
    { mServerGuild = b; }

    bool getServerGuild() const
    { return mServerGuild; }

private:
    typedef std::map<int, Guild*> GuildMap;
    static GuildMap guilds;

    /**
     * Constructor with guild id passed to it.
     */
    Guild(short id);

    typedef std::vector<GuildMember*> MemberList;
    MemberList mMembers;
    std::string mName;
    short mId;
    bool mCanInviteUsers;
    int mEmblemId;
    PositionsMap mPositions;
    bool mServerGuild;
};

#endif // GUILD_H
