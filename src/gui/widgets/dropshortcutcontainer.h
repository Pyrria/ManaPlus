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

#ifndef DROPSHORTCUTCONTAINER_H
#define DROPSHORTCUTCONTAINER_H

#include <guichan/mouselistener.hpp>

#include "gui/widgets/shortcutcontainer.h"

class Image;
class Item;
class ItemPopup;

/**
 * An item shortcut container. Used to quickly use items.
 *
 * \ingroup GUI
 */
class DropShortcutContainer : public ShortcutContainer
{
    public:
        /**
         * Constructor. Initializes the graphic.
         */
        DropShortcutContainer();

        /**
         * Destructor.
         */
        virtual ~DropShortcutContainer();

        /**
         * Draws the items.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(gcn::MouseEvent &event);

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(gcn::MouseEvent &event);

        /**
         * Handles mouse release.
         */
        void mouseReleased(gcn::MouseEvent &event);

        void widgetHidden(const gcn::Event &event);

    private:
        void mouseExited(gcn::MouseEvent &event);
        void mouseMoved(gcn::MouseEvent &event);

        bool mItemClicked;
        Item *mItemMoved;

        ItemPopup *mItemPopup;
        gcn::Color mEquipedColor;
        gcn::Color mUnEquipedColor;
};

#endif
