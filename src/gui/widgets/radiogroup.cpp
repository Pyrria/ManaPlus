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

#include "gui/widgets/radiogroup.h"

#include "gui/widgets/radiobutton.h"

#include "logger.h"

#include "debug.h"

RadioGroup::RadioGroup(std::string group, int height, int spacing) :
    WidgetGroup(group, height, spacing)
{
}

gcn::Widget *RadioGroup::createWidget(std::string text)
{
    RadioButton *widget = new RadioButton(text, mGroup, mCount == 0);
    widget->adjustSize();
    return widget;
}
