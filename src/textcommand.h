/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  Andrei Karas
 *  Copyright (C) 2011-2012  The ManaPlus developers
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

#ifndef TEXTCOMMAND_H
#define TEXTCOMMAND_H

#include "resources/itemdb.h"

#define MAGIC_START_ID 340

class Image;

enum SpellTarget
{
    NOTARGET = 0,
    ALLOWTARGET = 1,
    NEEDTARGET = 2
};

enum MagicSchool
{
    SKILL_MAGIC = 340,
    SKILL_MAGIC_LIFE = 341,
    SKILL_MAGIC_WAR = 342,
    SKILL_MAGIC_TRANSMUTE = 343,
    SKILL_MAGIC_NATURE = 344,
    SKILL_MAGIC_ASTRAL = 345
};

enum TextCommandType
{
    TEXT_COMMAND_MAGIC = 0,
    TEXT_COMMAND_TEXT = 1
};

/**
 * Represents one or more instances of a certain item type.
 */
class TextCommand
{
    public:
        /**
         * Constructor.
         */
        TextCommand(unsigned int id, std::string symbol, std::string command,
                    std::string comment, SpellTarget type, std::string icon,
                    unsigned int basicLvl, MagicSchool school = SKILL_MAGIC,
                    unsigned int schoolLvl = 0, int mana = 0);

        /**
         * Constructor.
         */
        TextCommand(unsigned int id, std::string symbol, std::string command,
                    std::string comment, SpellTarget type, std::string icon);

        /**
         * Constructor.
         */
        TextCommand(unsigned int id);

        /**
         * Destructor.
         */
        ~TextCommand();

        std::string getName() const
        { return mCommand; }

        std::string getCommand() const
        { return mCommand; }

        std::string getComment() const
        { return mComment; }

        std::string getSymbol() const
        { return mSymbol; }

        unsigned int getId() const
        { return mId; }

        SpellTarget getTargetType() const
        { return mTargetType; }

        std::string getIcon() const
        { return mIcon; }

        int getMana() const
        { return mMana; }

        MagicSchool getSchool() const
        { return mSchool; }

        int getBaseLvl() const
        { return mBaseLvl; }

        int getSchoolLvl() const
        { return mSchoolLvl; }

        TextCommandType getCommandType() const
        { return mCommandType; }

        void setCommand(std::string command)
        { mCommand = command; }

        void setComment(std::string comment)
        { mComment = comment; }

        void setSymbol(std::string symbol)
        { mSymbol = symbol; }

        void setId(unsigned int id)
        { mId = id; }

        void setTargetType(SpellTarget targetType)
        { mTargetType = targetType; }

        void setIcon(std::string icon)
        { mIcon = icon; loadImage(); }

        void setMana(unsigned int mana)
        { mMana = mana; }

        void setSchool(MagicSchool school)
        { mSchool = school; }

        void setBaseLvl(unsigned int baseLvl)
        { mBaseLvl = baseLvl; }

        void setSchoolLvl(unsigned int schoolLvl)
        { mSchoolLvl = schoolLvl; }

        void setCommandType(TextCommandType commandType)
        { mCommandType = commandType; }

        bool isEmpty() const
        { return mCommand == "" && mSymbol == ""; }

        Image *getImage() const
        { return mImage; }

    private:
        void loadImage();

    protected:
        std::string mCommand;
        std::string mComment;
        std::string mSymbol;
        SpellTarget mTargetType;
        std::string mIcon;
        unsigned int mId;
        int mMana;
        MagicSchool mSchool;
        int mBaseLvl;
        int mSchoolLvl;
        TextCommandType mCommandType;
        Image *mImage;
};

#endif
