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

#include "gui/textpopup.h"

#include "gui/gui.h"
#include "gui/palette.h"
#include "gui/theme.h"

#include "gui/widgets/label.h"

#include "graphics.h"
#include "units.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <guichan/font.hpp>

#include "debug.h"

TextPopup::TextPopup():
    Popup("TextPopup", "textpopup.xml")
{
    const int fontHeight = getFont()->getHeight();

    mText1 = new Label;
    mText1->setPosition(getPadding(), getPadding());

    mText2 = new Label;
    mText2->setPosition(getPadding(), fontHeight + 2 * getPadding());

    mText3 = new Label;
    mText3->setPosition(getPadding(), (2 * fontHeight) + 2 * getPadding());

    add(mText1);
    add(mText2);
    add(mText3);
    addMouseListener(this);
}

TextPopup::~TextPopup()
{
}

void TextPopup::show(int x, int y, const std::string &str1,
                     const std::string &str2, const std::string &str3)
{
    mText1->setCaption(str1);
    mText1->adjustSize();
    mText2->setCaption(str2);
    mText2->adjustSize();
    mText3->setCaption(str3);
    mText3->adjustSize();

    int minWidth = mText1->getWidth();
    if (mText2->getWidth() > minWidth)
        minWidth = mText2->getWidth();
    if (mText3->getWidth() > minWidth)
        minWidth = mText3->getWidth();

    minWidth += 4 * getPadding();
    setWidth(minWidth);

    int cnt = 1;
    if (!str2.empty())
        cnt ++;
    if (!str3.empty())
        cnt ++;

    setHeight((2 * getPadding() + mText1->getFont()->getHeight()) * cnt);

    const int distance = 20;

    int posX = std::max(0, x - getWidth() / 2);
    int posY = y + distance;

    if (posX + getWidth() > mainGraphics->mWidth)
        posX = mainGraphics->mWidth - getWidth();
    if (posY + getHeight() > mainGraphics->mHeight)
        posY = y - getHeight() - distance;

    setPosition(posX, posY);
    setVisible(true);
    requestMoveToTop();
}

void TextPopup::mouseMoved(gcn::MouseEvent &event)
{
    Popup::mouseMoved(event);

    // When the mouse moved on top of the popup, hide it
    setVisible(false);
}
