/*
 *  Support for non-overlapping floating text
 *  Copyright (C) 2008  Douglas Boffey <DougABoffey@netscape.net>
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

#include "textmanager.h"

#include "text.h"

#include <cstring>

#include "debug.h"

TextManager *textManager = nullptr;

TextManager::TextManager()
{
}

void TextManager::addText(Text *text)
{
    place(text, nullptr, text->mX, text->mY, text->mHeight);
    mTextList.push_back(text);
}

void TextManager::moveText(Text *text, int x, int y)
{
    text->mX = x;
    text->mY = y;
    place(text, text, text->mX, text->mY, text->mHeight);
}

void TextManager::removeText(const Text *text)
{
    for (TextList::iterator ptr = mTextList.begin(),
         pEnd = mTextList.end(); ptr != pEnd; ++ptr)
    {
        if (*ptr == text)
        {
            mTextList.erase(ptr);
            return;
        }
    }
}

TextManager::~TextManager()
{
}

void TextManager::draw(gcn::Graphics *graphics, int xOff, int yOff)
{
    for (TextList::const_iterator bPtr = mTextList.begin(),
         ePtr = mTextList.end();
         bPtr != ePtr; ++ bPtr)
    {
        (*bPtr)->draw(graphics, xOff, yOff);
    }
}

void TextManager::place(const Text *textObj, const Text *omit,
                        int &x A_UNUSED, int &y, int h)
{
    int xLeft = textObj->mX;
    int xRight1 = xLeft + textObj->mWidth;
    const int TEST = 50; // Number of lines to test for text
    const int nBeings = 50;
    bool occupied[TEST]; // is some other text obscuring this line?
    std::memset(&occupied, 0, sizeof(occupied)); // set all to false
    int wantedTop = (TEST - h) / 2; // Entry in occupied at top of text
    int occupiedTop = y - wantedTop; // Line in map representing to of occupied
    int cnt = 0;

    for (TextList::const_iterator ptr = mTextList.begin(),
         pEnd = mTextList.end(); ptr != pEnd && cnt < nBeings; ++ptr, cnt ++)
    {
        if (*ptr != omit &&
            (*ptr)->mX + 1 <= xRight1 &&
            (*ptr)->mX + (*ptr)->mWidth > xLeft)
        {
            int from = (*ptr)->mY - occupiedTop;
            int to = from + (*ptr)->mHeight - 1;
            if (to < 0 || from >= TEST) // out of range considered
                continue;
            if (from < 0)
                from = 0;
            if (to >= TEST)
                to = TEST - 1;
            for (int i = from; i <= to; ++i)
                occupied[i] = true;
        }
    }
    bool ok = true;
    for (int i = wantedTop; i < wantedTop + h; ++i)
    {
        ok = ok && !occupied[i];
    }

    if (ok)
        return;

    // Have to move it up or down, so find nearest spaces either side
    int consec = 0;
    int upSlot = -1; // means not found
    for (int seek = wantedTop + h - 2; seek >= 0; --seek)
    {
        if (occupied[seek])
        {
            consec = 0;
        }
        else
        {
            if (++consec == h)
            {
                upSlot = seek;
                break;
            }
        }
    }
    int downSlot = -1;
    consec = 0;
    for (int seek = wantedTop + 1; seek < TEST; ++seek)
    {
        if (occupied[seek])
        {
            consec = 0;
        }
        else
        {
            if (++consec == h)
            {
                downSlot = seek - h + 1;
                break;
            }
        }
    }
    if (upSlot == -1 && downSlot == -1) // no good solution, so leave as is
    {
        return;
    }
    if (upSlot == -1) // must go down
    {
        y += downSlot - wantedTop;
        return;
    }
    if (downSlot == -1) // must go up
    {
        y -= wantedTop - upSlot;
        return;
    }
    if (wantedTop - upSlot > downSlot - wantedTop) // down is better
    {
        y += downSlot - wantedTop;
    }
    else
    {
        y -= wantedTop - upSlot;
    }
}
