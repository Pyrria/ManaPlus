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

#ifndef BUYDIALOG_H
#define BUYDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/selectionlistener.hpp>

class ShopItems;
class ShopListBox;
class IntTextField;
class ListBox;

namespace gcn
{
    class Button;
    class Label;
    class ScrollArea;
    class Slider;
}

/**
 * The buy dialog.
 *
 * \ingroup Interface
 */
class BuyDialog : public Window, public gcn::ActionListener,
                  public gcn::SelectionListener
{
    public:
        /**
         * Constructor.
         *
         * @see Window::Window
         */
        BuyDialog(int npcId);

        /**
         * Constructor.
         */
        BuyDialog(std::string nick);

        /**
         * Destructor
         */
        ~BuyDialog();

        void init();

        /**
         * Resets the dialog, clearing shop inventory.
         */
        void reset();

        /**
         * Sets the amount of available money.
         */
        void setMoney(int amount);

        /**
         * Adds an item to the shop inventory.
         */
        void addItem(int id, unsigned char color, int amount, int price);

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Returns the number of items in the shop inventory.
         */
        int getNumberOfElements();

        /**
         * Updates the labels according to the selected item.
         */
        void valueChanged(const gcn::SelectionEvent &event);

        /**
         * Returns the name of item number i in the shop inventory.
         */
        std::string getElementAt(int i);

        /**
         * Updates the state of buttons and labels.
         */
        void updateButtonsAndLabels();

        /**
         * Sets the visibility of this window.
         */
        void setVisible(bool visible);

        /**
         * Returns true if any instances exist.
         */
        static bool isActive()
        { return !instances.empty(); }

        /**
         * Closes all instances.
         */
        static void closeAll();

        void scheduleDelete();

    private:
        typedef std::list<BuyDialog*> DialogList;
        static DialogList instances;

        int mNpcId;

        gcn::Button *mBuyButton;
        gcn::Button *mQuitButton;
        gcn::Button *mAddMaxButton;
        gcn::Button *mIncreaseButton;
        gcn::Button *mDecreaseButton;
        ShopListBox *mShopItemList;
        gcn::ScrollArea *mScrollArea;
        gcn::Label *mMoneyLabel;
        gcn::Label *mQuantityLabel;
        gcn::Slider *mSlider;
        gcn::Label *mAmountLabel;
        IntTextField *mAmountField;

        ShopItems *mShopItems;

        int mMoney;
        int mAmountItems;
        int mMaxItems;
        std::string mNick;
        bool mEnabledKeyboard;
};

#endif
