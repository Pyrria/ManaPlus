/*
 *  The ManaPlus Client
 *  Copyright (C) 2009  The Mana World Development Team
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

#include "gui/widgets/layouthelper.h"

#include "debug.h"

LayoutHelper::LayoutHelper(gcn::Container *container):
    mContainer(container)
{
    mContainer->addWidgetListener(this);
}

LayoutHelper::~LayoutHelper()
{
    mContainer->removeWidgetListener(this);
}

const Layout &LayoutHelper::getLayout() const
{
    return mLayout;
}

LayoutCell &LayoutHelper::place(int x, int y, gcn::Widget *wg, int w, int h)
{
    mContainer->add(wg);
    return mLayout.place(wg, x, y, w, h);
}

ContainerPlacer LayoutHelper::getPlacer(int x, int y)
{
    return ContainerPlacer(mContainer, &mLayout.at(x, y));
}

void LayoutHelper::reflowLayout(int w, int h)
{
    mLayout.reflow(w, h);
    mContainer->setSize(w, h);
}

void LayoutHelper::widgetResized(const gcn::Event &event A_UNUSED)
{
    const gcn::Rectangle area = mContainer->getChildrenArea();
    int w = area.width;
    int h = area.height;
    mLayout.reflow(w, h);
}
