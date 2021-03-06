/*
 *  The ManaPlus Client
 *  Copyright (C) 2007-2009  The Mana World Development Team
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

#include "gui/widgets/itemshortcutcontainer.h"

#include "configuration.h"
#include "graphics.h"
#include "inventory.h"
#include "inputmanager.h"
#include "item.h"
#include "itemshortcut.h"
#include "spellshortcut.h"
#include "keyboardconfig.h"
#include "localplayer.h"
#include "playerinfo.h"
#include "spellmanager.h"
#include "textcommand.h"

#include "gui/inventorywindow.h"
#include "gui/itempopup.h"
#include "gui/palette.h"
#include "gui/skilldialog.h"
#include "gui/spellpopup.h"
#include "gui/theme.h"
#include "gui/viewport.h"

#include "resources/image.h"
#include "resources/iteminfo.h"

#include "utils/stringutils.h"

#include "debug.h"

ItemShortcutContainer::ItemShortcutContainer(unsigned number):
    ShortcutContainer(),
    mItemClicked(false),
    mItemMoved(nullptr),
    mNumber(number)
{
    addMouseListener(this);
    addWidgetListener(this);

    mItemPopup = new ItemPopup;
    mSpellPopup = new SpellPopup;

    mBackgroundImg = Theme::getImageFromTheme("item_shortcut_bgr.png");
    if (itemShortcut[mNumber])
        mMaxItems = itemShortcut[mNumber]->getItemCount();
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
    mEquipedColor = Theme::getThemeColor(Theme::ITEM_EQUIPPED);
    mUnEquipedColor = Theme::getThemeColor(Theme::ITEM_NOT_EQUIPPED);
}

ItemShortcutContainer::~ItemShortcutContainer()
{
    if (mBackgroundImg)
    {
        mBackgroundImg->decRef();
        mBackgroundImg = nullptr;
    }
    delete mItemPopup;
    mItemPopup = nullptr;
    delete mSpellPopup;
    mSpellPopup = nullptr;
}

void ItemShortcutContainer::draw(gcn::Graphics *graphics)
{
    if (!itemShortcut[mNumber])
        return;

    mAlpha = Client::getGuiAlpha();
    if (Client::getGuiAlpha() != mAlpha)
    {
        if (mBackgroundImg)
            mBackgroundImg->setAlpha(mAlpha);
    }

    Graphics *g = static_cast<Graphics*>(graphics);

    graphics->setFont(getFont());

    if (mBackgroundImg)
    {
        for (unsigned i = 0; i < mMaxItems; i++)
        {
            g->drawImage(mBackgroundImg, (i % mGridWidth) * mBoxWidth,
                (i / mGridWidth) * mBoxHeight);
        }
    }

    Inventory *inv = PlayerInfo::getInventory();
    if (!inv)
        return;

    for (unsigned i = 0; i < mMaxItems; i++)
    {
        const int itemX = (i % mGridWidth) * mBoxWidth;
        const int itemY = (i / mGridWidth) * mBoxHeight;

        // Draw item keyboard shortcut.
        std::string key = inputManager.getKeyValueString(
            Input::KEY_SHORTCUT_1 + i);
        graphics->setColor(getForegroundColor());

        g->drawText(key, itemX + 2, itemY + 2, gcn::Graphics::LEFT);

        const int itemId = itemShortcut[mNumber]->getItem(i);
        const int itemColor = itemShortcut[mNumber]->getItemColor(i);

        if (itemId < 0)
            continue;

        // this is item
        if (itemId < SPELL_MIN_ID)
        {
            Item *item = inv->findItem(itemId, itemColor);

            if (item)
            {
                // Draw item icon.
                Image* image = item->getImage();

                if (image)
                {
                    std::string caption;
                    if (item->getQuantity() > 1)
                        caption = toString(item->getQuantity());
                    else if (item->isEquipped())
                    caption = "Eq.";

                    image->setAlpha(1.0f);
                    g->drawImage(image, itemX, itemY);
                    if (item->isEquipped())
                        g->setColor(mEquipedColor);
                    else
                        g->setColor(mUnEquipedColor);
                    g->drawText(caption, itemX + mBoxWidth / 2,
                        itemY + mBoxHeight - 14, gcn::Graphics::CENTER);
                }
            }
        }
        else if (itemId < SKILL_MIN_ID && spellManager)
        {   // this is magic shortcut
            TextCommand *spell = spellManager->getSpellByItem(itemId);
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
        else if (skillDialog)
        {
            SkillInfo *skill = skillDialog->getSkill(itemId - SKILL_MIN_ID);
            if (skill)
            {
                Image* image = skill->icon;

                if (image)
                {
                    image->setAlpha(1.0f);
                    g->drawImage(image, itemX, itemY);
                }

                g->drawText(skill->shortName, itemX + 2,
                    itemY + mBoxHeight / 2, gcn::Graphics::LEFT);
            }
        }
    }

    if (mItemMoved)
    {
        // Draw the item image being dragged by the cursor.
        Image* image = mItemMoved->getImage();
        if (image)
        {
            const int tPosX = mCursorPosX - (image->mBounds.w / 2);
            const int tPosY = mCursorPosY - (image->mBounds.h / 2);

            g->drawImage(image, tPosX, tPosY);
            g->drawText(toString(mItemMoved->getQuantity()),
                        tPosX + mBoxWidth / 2, tPosY + mBoxHeight - 14,
                        gcn::Graphics::CENTER);
        }
    }
}

void ItemShortcutContainer::mouseDragged(gcn::MouseEvent &event)
{
    if (!itemShortcut[mNumber])
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (!mItemMoved && mItemClicked)
        {
            const int index = getIndexFromGrid(event.getX(), event.getY());

            if (index == -1)
                return;

            const int itemId = itemShortcut[mNumber]->getItem(index);
            const int itemColor = itemShortcut[mNumber]->getItemColor(index);

            if (itemId < 0)
                return;

            if (itemId < SPELL_MIN_ID)
            {
                if (!PlayerInfo::getInventory())
                    return;

                Item *item = PlayerInfo::getInventory()->findItem(
                    itemId, itemColor);

                if (item)
                {
                    mItemMoved = item;
                    itemShortcut[mNumber]->removeItem(index);
                }
            }
            else if (itemId < SKILL_MIN_ID && spellManager)
            {
                TextCommand *spell = spellManager->getSpellByItem(itemId);
                if (spell)
                    itemShortcut[mNumber]->removeItem(index);
            }
            else
            {
                itemShortcut[mNumber]->removeItem(index);
            }
        }
        if (mItemMoved)
        {
            mCursorPosX = event.getX();
            mCursorPosY = event.getY();
        }
    }
}

void ItemShortcutContainer::mousePressed(gcn::MouseEvent &event)
{
    if (!itemShortcut[mNumber])
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        // Stores the selected item if theirs one.
        if (itemShortcut[mNumber]->isItemSelected() &&
            inventoryWindow && (inventoryWindow->isVisible() ||
            itemShortcut[mNumber]->getSelectedItem() >= SPELL_MIN_ID))
        {
            itemShortcut[mNumber]->setItem(index);
            itemShortcut[mNumber]->setItemSelected(-1);
            if (spellShortcut)
                spellShortcut->setItemSelected(-1);
            inventoryWindow->unselectItem();
        }
        else if (itemShortcut[mNumber]->getItem(index))
        {
            mItemClicked = true;
        }
    }
    else if (event.getButton() == gcn::MouseEvent::RIGHT)
    {
        if (viewport && itemShortcut[mNumber])
        {
            viewport->showItemPopup(itemShortcut[mNumber]->getItem(index),
                itemShortcut[mNumber]->getItemColor(index));
        }
    }
}

void ItemShortcutContainer::mouseReleased(gcn::MouseEvent &event)
{
    if (!itemShortcut[mNumber])
        return;

    if (event.getButton() == gcn::MouseEvent::LEFT)
    {
        if (itemShortcut[mNumber]->isItemSelected())
            itemShortcut[mNumber]->setItemSelected(-1);

        const int index = getIndexFromGrid(event.getX(), event.getY());
        if (index == -1)
        {
            mItemMoved = nullptr;
            return;
        }
        if (mItemMoved)
        {
            itemShortcut[mNumber]->setItems(index,
                mItemMoved->getId(), mItemMoved->getColor());
            mItemMoved = nullptr;
        }
        else if (itemShortcut[mNumber]->getItem(index) && mItemClicked)
        {
            itemShortcut[mNumber]->useItem(index);
        }

        if (mItemClicked)
            mItemClicked = false;
    }
}

// Show ItemTooltip
void ItemShortcutContainer::mouseMoved(gcn::MouseEvent &event)
{
    if (!itemShortcut[mNumber])
        return;

    const int index = getIndexFromGrid(event.getX(), event.getY());

    if (index == -1)
        return;

    const int itemId = itemShortcut[mNumber]->getItem(index);
    const int itemColor = itemShortcut[mNumber]->getItemColor(index);

    if (itemId < 0)
        return;

    if (itemId < SPELL_MIN_ID)
    {
        mSpellPopup->setVisible(false);

        if (!PlayerInfo::getInventory())
            return;

        Item *item = PlayerInfo::getInventory()->findItem(itemId, itemColor);

        if (item && viewport)
        {
            mItemPopup->setItem(item);
            mItemPopup->position(viewport->getMouseX(), viewport->getMouseY());
        }
        else
        {
            mItemPopup->setVisible(false);
        }
    }
    else if (itemId < SKILL_MIN_ID && spellManager)
    {
        mItemPopup->setVisible(false);
        TextCommand *spell = spellManager->getSpellByItem(itemId);
        if (spell && viewport)
        {
            mSpellPopup->setItem(spell);
            mSpellPopup->view(viewport->getMouseX(), viewport->getMouseY());
        }
        else
        {
            mSpellPopup->setVisible(false);
        }
    }
    else if (skillDialog)
    {
        mItemPopup->setVisible(false);
    }
}

// Hide ItemTooltip
void ItemShortcutContainer::mouseExited(gcn::MouseEvent &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}

void ItemShortcutContainer::widgetHidden(const gcn::Event &event A_UNUSED)
{
    if (mItemPopup)
        mItemPopup->setVisible(false);
    if (mSpellPopup)
        mSpellPopup->setVisible(false);
}
