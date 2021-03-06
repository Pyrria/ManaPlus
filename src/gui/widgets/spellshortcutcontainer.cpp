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

#include "gui/widgets/spellshortcutcontainer.h"

#include "gui/inventorywindow.h"
#include "gui/okdialog.h"
#include "gui/palette.h"
#include "gui/shortcutwindow.h"
#include "gui/spellpopup.h"
#include "gui/viewport.h"
#include "gui/textcommandeditor.h"
#include "gui/theme.h"

#include "configuration.h"
#include "graphics.h"
#include "inventory.h"
#include "spellshortcut.h"
#include "itemshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "spellmanager.h"
#include "logger.h"

#include "resources/image.h"
#include "textcommand.h"
#include "resources/resourcemanager.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include "debug.h"

SpellShortcutContainer::SpellShortcutContainer(unsigned number):
    ShortcutContainer(),
    mSpellClicked(false),
    mSpellMoved(nullptr),
    mNumber(number)
{
    mBoxWidth = mBoxWidth;

    addMouseListener(this);
    addWidgetListener(this);

    mSpellPopup = new SpellPopup;

    mBackgroundImg = Theme::getImageFromTheme("item_shortcut_bgr.png");
    if (spellShortcut)
        mMaxItems = spellShortcut->getSpellsCount();
    else
        mMaxItems = 0;

    if (mBackgroundImg)
    {
        mBackgroundImg->setAlpha(Client::getGuiAlpha());
        mBoxHeight = mBackgroundImg->getHeight();
        mBoxWidth = mBackgroundImg->getWidth();
    }
    else
    {
        mBoxHeight = 1;
        mBoxWidth = 1;
    }
    setForegroundColor(Theme::getThemeColor(Theme::TEXT));
}

SpellShortcutContainer::~SpellShortcutContainer()
{
    if (mBackgroundImg)
        mBackgroundImg->decRef();
    mBackgroundImg = nullptr;
    delete mSpellPopup;
    mSpellPopup = nullptr;
}

void SpellShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!spellShortcut)
        return;

    if (Client::getGuiAlpha() != mAlpha)
    {
        mAlpha = Client::getGuiAlpha();
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *g = static_cast<Graphics*>(graphics);

    graphics->setFont(getFont());

    int selectedId = spellShortcut->getSelectedItem();
    g->setColor(getForegroundColor());

    if (mBackgroundImg)
    {
        for (unsigned i = 0; i < mMaxItems; i++)
        {
            g->drawImage(mBackgroundImg, (i % mGridWidth) * mBoxWidth,
                (i / mGridWidth) * mBoxHeight);
        }
    }

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        int itemId = spellShortcut->getItem(
            (mNumber * SPELL_SHORTCUT_ITEMS) + i);
        if (selectedId >= 0 && itemId == selectedId)
        {
            g->drawRectangle(gcn::Rectangle(itemX + 1, itemY + 1,
                mBoxWidth - 1, mBoxHeight - 1));
        }

        if (!spellManager)
            continue;

        TextCommand *spell = spellManager->getSpell(itemId);
        if (spell)
        {
            if (!spell->isEmpty())
            {
                Image* image = spell->getImage();

                if (image)
                {
                    image->setAlpha(1.0f);
                    g->drawImage(image, itemX, itemY);
                }
            }

            g->drawText(spell->getSymbol(), itemX + 2,
                        itemY + mBoxHeight / 2, gcn::Graphics::LEFT);
        }
    }

    if (mSpellMoved)
    {
        // Draw the item image being dragged by the cursor.
    }

}

void SpellShortcutContainer::mouseDragged(gcn::MouseEvent &event)
{
    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mSpellMoved && mSpellClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = spellShortcut->getItem(
                (mNumber * SPELL_SHORTCUT_ITEMS) + index);

            if (itemId < 0)
                return;
        }
        if (mSpellMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
}

void SpellShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
    }
    else if (event.getButton() == gcn::MouseEvent::MIDDLE)
    {
        if (!spellShortcut || !spellManager)
            return;

        const int itemId = spellShortcut->getItem(
            (mNumber * SPELL_SHORTCUT_ITEMS) + index);
        spellManager->invoke(itemId);
    }
}

void SpellShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (!spellShortcut || !spellManager)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = spellShortcut->getItem(
        (mNumber * SPELL_SHORTCUT_ITEMS) + index);

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (itemId < 0)
            return;

        const int selectedId = spellShortcut->getSelectedItem();

        if (selectedId != itemId)
        {
            TextCommand *spell = spellManager->getSpell(itemId);
            if (spell && !spell->isEmpty())
            {
                int num = itemShortcutWindow->getTabIndex();
                if (num >= 0 && num < SHORTCUT_TABS && itemShortcut[num])
                {
                    itemShortcut[num]->setItemSelected(
                        spell->getId() + SPELL_MIN_ID);
                }
                spellShortcut->setItemSelected(spell->getId());
            }
        }
        else
        {
            int num = itemShortcutWindow->getTabIndex();
            if (num >= 0 && num < SHORTCUT_TABS && itemShortcut[num])
                itemShortcut[num]->setItemSelected(-1);
            spellShortcut->setItemSelected(-1);
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        TextCommand *spell = nullptr;
        if (itemId >= 0)
            spell = spellManager->getSpell(itemId);

        if (spell && viewport)
            viewport->showSpellPopup(spell);
    }
}

// Show ItemTooltip
void SpellShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    if (!mSpellPopup || !spellShortcut || !spellManager)
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = spellShortcut->getItem(
        (mNumber * SPELL_SHORTCUT_ITEMS) + index);

    mSpellPopup->setVisible(false);
    TextCommand *spell = spellManager->getSpell(itemId);
    if (spell && !spell->isEmpty())
    {
        mSpellPopup->setItem(spell);
        mSpellPopup->view(viewport->getMouseX(), viewport->getMouseY());
    }
    else
    {
        mSpellPopup->setVisible(false);
    }
}

// Hide SpellTooltip
void SpellShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}

void SpellShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}
