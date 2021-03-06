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

#ifndef POPUP_MENU_H
#define POPUP_MENU_H

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/popup.h"

#include "actorsprite.h"

#include <guichan/actionlistener.hpp>

#include "localconsts.h"

class Being;
class BrowserBox;
class Button;
class ChatTab;
class FloorItem;
class Item;
class ItemShortcut;
class MapItem;
class TextCommand;
class TextDialog;
class ProgressBar;
class Window;

class RenameListener : public gcn::ActionListener
{
    public:
        RenameListener();

        void action(const gcn::ActionEvent &event);

        void setMapItem(MapItem* mapItem)
        { mMapItem = mapItem; }

        void setDialog(TextDialog *dialog)
        { mDialog = dialog; }

    private:
        MapItem *mMapItem;
        TextDialog *mDialog;
};

class PlayerListener : public gcn::ActionListener
{
    public:
        PlayerListener();

        void action(const gcn::ActionEvent &event);

        void setNick(std::string name)
        { mNick = name; }

        void setDialog(TextDialog *dialog)
        { mDialog = dialog; }

        void setType(int type)
        { mType = type; }

    private:
        std::string mNick;
        TextDialog *mDialog;
        int mType;
};

/**
 * Window showing popup menu.
 */
class PopupMenu : public Popup, public LinkHandler
{
    public:
        /**
         * Constructor.
         */
        PopupMenu();

        /**
         * Shows the being related popup menu at the specified mouse coords.
         */
        void showPopup(int x, int y, Being *being);

        /**
         * Shows the beings related popup menu at the specified mouse coords.
         */
        void showPopup(int x, int y, std::vector<ActorSprite*> &beings);

        void showPlayerPopup(int x, int y, std::string nick);

        /**
         * Shows the floor item related popup menu at the specified
         * mouse coords.
         */
        void showPopup(int x, int y, FloorItem *floorItem);

        /**
         * Shows the related popup menu when right click on the inventory
         * at the specified mouse coordinates.
         */
        void showPopup(Window *parent, int x, int y, Item *item,
                       bool isInventory);

        void showPopup(int x, int y, Button *button);

        void showPopup(int x, int y, ProgressBar *bar);

        void showPopup(int x, int y, MapItem *mapItem);

        void showItemPopup(int x, int y, Item *item);

        void showItemPopup(int x, int y, int itemId, unsigned char color);

        void showDropPopup(int x, int y, Item *item);

        void showOutfitsPopup(int x, int y);

        void showSpellPopup(int x, int y, TextCommand *cmd);

        void showAttackMonsterPopup(int x, int y, std::string name, int type);

        void showPickupItemPopup(int x, int y, std::string name);

        void showUndressPopup(int x, int y, Being *being, Item *item);

        void showMapPopup(int x, int y, int x2, int y2);

        /**
         * Shows the related popup menu when right click on the chat
         * at the specified mouse coordinates.
         */
        void showChatPopup(int x, int y, ChatTab *tab);

        void showChangePos(int x, int y);

        /**
         * Handles link action.
         */
        void handleLink(const std::string &link,
                        gcn::MouseEvent *event A_UNUSED);

    private:
        void addPlayerRelation(std::string name);

        void addFollow();

        void addBuySell(const Being *being);

        void addParty(const std::string &partyName);

        void addBuySellDefault();

        void addPlayerMisc();

        void addPickupFilter(const std::string &name);

        BrowserBox* mBrowserBox;

        int mBeingId;
        int mFloorItemId;
        Item *mItem;
        int mItemId;
        unsigned char mItemColor;
        MapItem *mMapItem;
        ChatTab *mTab;
        TextCommand *mSpell;
        Window *mWindow;
        RenameListener mRenameListener;
        PlayerListener mPlayerListener;
        TextDialog *mDialog;
        Button *mButton;
        std::string mNick;
        int mType;
        int mX;
        int mY;

        /**
         * Shared code for the various showPopup functions.
         */
        void showPopup(int x, int y);
};

#endif
