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

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "gui/widgets/linkhandler.h"
#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>

#include "localconsts.h"

#include <map>
#include <set>

class BrowserBox;
class LinkHandler;

namespace gcn
{
    class ScrollArea;
}

typedef std::set<std::string> HelpNames;
typedef HelpNames::const_iterator HelpNamesCIter;
typedef std::map<std::string, HelpNames> HelpTagsMap;

/**
 * The help dialog.
 */
class HelpWindow : public Window, public LinkHandler,
                   public gcn::ActionListener
{
    public:
        /**
         * Constructor.
         */
        HelpWindow();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Handles link action.
         */
        void handleLink(const std::string &link,
                        gcn::MouseEvent *event A_UNUSED);

        /**
         * Loads help in the dialog.
         */
        void loadHelp(const std::string &helpFile);

        /**
         * Seach for given text in tags.
         */
        void search(const std::string &text);

    private:
        void loadTags();

        void loadFile(const std::string &file);

        BrowserBox *mBrowserBox;
        gcn::ScrollArea *mScrollArea;
        HelpTagsMap mTagFileMap;
};

extern HelpWindow *helpWindow;

#endif
