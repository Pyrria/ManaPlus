/*
 *  The ManaPlus Client
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

#include "gui/widgets/sliderlist.h"

#include "client.h"
#include "logger.h"

#include "gui/gui.h"
#include "gui/sdlfont.h"

#include "gui/widgets/button.h"
#include "gui/widgets/label.h"

#include "utils/dtor.h"

#include <guichan/font.hpp>

#include "debug.h"

static const int buttonWidth = 27;
static const int buttonSpace = 30;
static const int sliderHeight = 30;

SliderList::SliderList(gcn::ListModel *listModel,
                       gcn::ActionListener* listener,
                       std::string eventId) :
    mListModel(listModel),
    mOldWidth(0),
    mSelectedIndex(0)
{
    mPrevEventId = eventId + "_prev";
    mNextEventId = eventId + "_next";

    setHeight(sliderHeight);

    mButtons[0] = new Button("<", mPrevEventId, this);
    mButtons[1] = new Button(">", mNextEventId, this);
    mLabel = new Label("");
    add(mButtons[0]);
    add(mLabel);
    add(mButtons[1]);

    if (!eventId.empty())
        setActionEventId(eventId);

    if (listener)
        addActionListener(listener);

    updateLabel();
    addMouseListener(this);
}

SliderList::~SliderList()
{
}

void SliderList::updateAlpha()
{
    mButtons[0]->updateAlpha();
    mButtons[1]->updateAlpha();
}

void SliderList::mouseWheelMovedUp(gcn::MouseEvent& mouseEvent)
{
    logger->log("SliderList::mouseWheelMovedUp");
    setSelected(mSelectedIndex - 1);
    mouseEvent.consume();
}

void SliderList::mouseWheelMovedDown(gcn::MouseEvent& mouseEvent)
{
    setSelected(mSelectedIndex + 1);
    mouseEvent.consume();
}

void SliderList::resize()
{
    const int width = getWidth();

    mButtons[0]->setWidth(buttonWidth);
//    mLabel->setPosition(buttonSpace, 0);
    mLabel->setWidth(width - buttonSpace * 2);
    mButtons[1]->setPosition(width - buttonSpace + 3, 0);
    mButtons[1]->setWidth(buttonWidth);
    updateLabel();
}

void SliderList::draw(gcn::Graphics *graphics)
{
    if (mOldWidth != getWidth())
    {
        resize();
        mOldWidth = getWidth();
    }
    Container::draw(graphics);
}

void SliderList::updateLabel()
{
    if (!mListModel || mSelectedIndex < 0
        || mSelectedIndex >= mListModel->getNumberOfElements())
    {
        return;
    }

    mLabel->setCaption(mListModel->getElementAt(mSelectedIndex));
    mLabel->adjustSize();
    const int space = getWidth() - buttonSpace * 2;
    const int labelWidth = mLabel->getWidth();
    int labelY = (getHeight() - mLabel->getHeight()) / 2;
    if (labelY < 0)
        labelY = 0;

    if (space < 0 || space < labelWidth)
        mLabel->setPosition(buttonSpace, labelY);
    else
        mLabel->setPosition(buttonSpace + (space - labelWidth) / 2, labelY);
}

void SliderList::action(const gcn::ActionEvent &event)
{
    if (!mListModel)
        return;

    if (event.getId() == mPrevEventId)
    {
        mSelectedIndex --;
        if (mSelectedIndex < 0)
            mSelectedIndex = mListModel->getNumberOfElements() - 1;
    }
    else if (event.getId() == mNextEventId)
    {
        mSelectedIndex ++;
        if (mSelectedIndex >= mListModel->getNumberOfElements())
            mSelectedIndex = 0;
    }
    updateLabel();
    distributeActionEvent();
}

void SliderList::setSelectedString(std::string str)
{
    if (!mListModel)
        return;

    for (int f = 0; f < mListModel->getNumberOfElements(); f ++)
    {
        if (mListModel->getElementAt(f) == str)
        {
            setSelected(f);
            break;
        }
    }
}

std::string SliderList::getSelectedString() const
{
    if (!mListModel)
        return "";

    return mListModel->getElementAt(mSelectedIndex);
}

void SliderList::setSelected(int idx)
{
    if (!mListModel)
        return;

    mSelectedIndex = idx;
    if (mSelectedIndex >= mListModel->getNumberOfElements())
        mSelectedIndex = 0;
    if (mSelectedIndex < 0)
        mSelectedIndex = mListModel->getNumberOfElements() - 1;
    updateLabel();
}

void SliderList::adjustSize()
{
    setWidth(getMaxLabelWidth() + 60);
    updateLabel();
}

int SliderList::getMaxLabelWidth()
{
    if (!mListModel || !gui)
        return 1;

    int maxWidth = 0;
    SDLFont *font = gui->getFont();

    for (int f = 0; f < mListModel->getNumberOfElements(); f ++)
    {
        int w = font->getWidth(mListModel->getElementAt(f));
        if (w > maxWidth)
            maxWidth = w;
    }

    return maxWidth;
}
