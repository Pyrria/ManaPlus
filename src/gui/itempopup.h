/*
 *  The ManaPlus Client
 *  Copyright (C) 2008  The Legend of Mazzeroth Development Team
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

#ifndef ITEMPOPUP_H
#define ITEMPOPUP_H

#include "gui/widgets/popup.h"

#include "resources/iteminfo.h"

#include <guichan/mouselistener.hpp>

class Icon;
class TextBox;

namespace gcn
{
    class Button;
    class CheckBox;
    class Label;
    class ScrollArea;
}

/**
 * A popup that displays information about an item.
 */
class ItemPopup : public Popup
{
    public:
        /**
         * Constructor. Initializes the item popup.
         */
        ItemPopup();

        /**
         * Destructor. Cleans up the item popup on deletion.
         */
        ~ItemPopup();

        /**
         * Sets the info to be displayed given a particular item.
         */
        void setItem(const ItemInfo &item, unsigned char color,
                     bool showImage = false, int id = -1);

        void setItem(const Item *item, bool showImage = false);

        void mouseMoved(gcn::MouseEvent &mouseEvent);

    private:
        gcn::Label *mItemName;
        TextBox *mItemDesc;
        TextBox *mItemEffect;
        TextBox *mItemWeight;
        ItemType mItemType;
        Icon *mIcon;
        std::string mLastName;
        unsigned char mLastColor;

        static gcn::Color getColor(ItemType type);
};

#endif // ITEMPOPUP_H
