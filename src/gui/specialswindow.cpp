/*
 *  The ManaPlus Client
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

#include "gui/specialswindow.h"

#include "logger.h"

#include "gui/setup.h"
#include "gui/theme.h"

#include "gui/widgets/button.h"
#include "gui/widgets/container.h"
#include "gui/widgets/icon.h"
#include "gui/widgets/label.h"
#include "gui/widgets/layouthelper.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/progressbar.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/tab.h"
#include "gui/widgets/tabbedarea.h"
#include "gui/widgets/flowcontainer.h"
#include "gui/widgets/windowcontainer.h"

#include "net/net.h"
#include "net/specialhandler.h"

#include "resources/specialdb.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include <string>

#include "debug.h"

#define SPECIALS_WIDTH 200
#define SPECIALS_HEIGHT 32

class SpecialEntry : public Container
{
    public:
        SpecialEntry(SpecialInfo *info);

        void update(int current, int needed);

    protected:
        friend class SpecialsWindow;
        SpecialInfo *mInfo;

    private:
        Icon *mIcon;        // icon to display
        Label *mNameLabel;  // name to display
        Label *mLevelLabel; // level number label (only shown when applicable)
        Button *mUse; // use button (only shown when applicable)
        ProgressBar *mRechargeBar; // recharge bar (only shown when applicable)
};

SpecialsWindow::SpecialsWindow():
    Window(_("Specials"), false, nullptr, "specials.xml")
{
    setWindowName("Specials");
    setCloseButton(true);
    setResizable(true);
    setSaveVisible(true);
    setStickyButtonLock(true);
    setDefaultSize(windowContainer->getWidth() - 280, 30, 275, 425);
    setupWindow->registerWindowForReset(this);

    mTabs = new TabbedArea();

    place(0, 0, mTabs, 5, 5);

    setLocationRelativeTo(getParent());
    loadWindowState();
}

SpecialsWindow::~SpecialsWindow()
{
    // Clear gui
}

void SpecialsWindow::action(const gcn::ActionEvent &event)
{
    if (event.getId() == "use")
    {
        if (!event.getSource())
            return;

        SpecialEntry *disp = dynamic_cast<SpecialEntry*>(
            event.getSource()->getParent());

        if (disp)
        {
            /*Being *target = player_node->getTarget();

            if (target)
                Net::getSpecialHandler()->use(disp->mInfo->id,
                                              1, target->getId());
            else*/
                Net::getSpecialHandler()->use(disp->mInfo->id);
        }
    }
    else if (event.getId() == "close")
    {
        setVisible(false);
    }
}

void SpecialsWindow::draw(gcn::Graphics *graphics)
{
    // update the progress bars
    std::map<int, Special> specialData = PlayerInfo::getSpecialStatus();
    bool foundNew = false;
    unsigned int found = 0; // number of entries in specialData
                            // which match mEntries

    for (std::map<int, Special>::const_iterator i = specialData.begin(),
         i_end = specialData.end(); i != i_end; ++i)
    {
        std::map<int, SpecialEntry *>::const_iterator
            e = mEntries.find(i->first);
        if (e == mEntries.end())
        {
            // found a new special - abort update and rebuild from scratch
            foundNew = true;
            break;
        }
        else
        {
            // update progress bar of special
            if (e->second)
                e->second->update(i->second.currentMana, i->second.neededMana);
            found++;
        }
    }
    // a rebuild is needed when a) the number of specials changed or b)
    // an existing entry isn't found anymore
    if (foundNew || found != mEntries.size())
        rebuild(specialData);

    Window::draw(graphics);
}

void SpecialsWindow::rebuild(const std::map<int, Special> &specialData)
{
    make_dtor(mEntries);
    mEntries.clear();
    int vPos = 0; //vertical position of next placed element

    for (std::map<int, Special>::const_iterator i = specialData.begin(),
         i_end = specialData.end(); i != i_end; ++i)
    {
        logger->log("Updating special GUI for %d", i->first);

        SpecialInfo* info = SpecialDB::get(i->first);
        if (info)
        {
            info->rechargeCurrent = i->second.currentMana;
            info->rechargeNeeded = i->second.neededMana;
            SpecialEntry* entry = new SpecialEntry(info);
            entry->setPosition(0, vPos);
            vPos += entry->getHeight();
            add(entry);
            mEntries[i->first] = entry;
        }
        else
        {
            logger->log("Warning: No info available of special %d", i->first);
        }
    }
}


SpecialEntry::SpecialEntry(SpecialInfo *info) :
    mInfo(info),
    mIcon(nullptr),
    mLevelLabel(nullptr),
    mUse(nullptr),
    mRechargeBar(nullptr)
{
    setFrameSize(1);
    setOpaque(false);
    setSize(SPECIALS_WIDTH, SPECIALS_HEIGHT);

    if (info && !info->icon.empty())
        mIcon = new Icon(info->icon);
    else
        mIcon = new Icon(Theme::resolveThemePath("unknown-item.png"));

    mIcon->setPosition(1, 0);
    add(mIcon);

    if (info)
        mNameLabel = new Label(info->name);
    else
        mNameLabel = new Label("");

    mNameLabel->setPosition(35, 0);
    add(mNameLabel);

    if (info && info->hasLevel)
    {
        mLevelLabel = new Label(toString(info->level));
        mLevelLabel->setPosition(getWidth() - mLevelLabel->getWidth(), 0);
        add(mLevelLabel);
    }

    if (info && info->isActive)
    {
        mUse = new Button("Use", "use", specialsWindow);
        mUse->setPosition(getWidth() - mUse->getWidth(), 13);
        add(mUse);
    }

    if (info->hasRechargeBar)
    {
        float progress = 0;
        if (info->rechargeNeeded)
        {
            progress = static_cast<float>(info->rechargeCurrent)
                / static_cast<float>(info->rechargeNeeded);
        }
        mRechargeBar = new ProgressBar(progress, 100, 10, Theme::PROG_MP);
        mRechargeBar->setSmoothProgress(false);
        mRechargeBar->setPosition(0, 13);
        add(mRechargeBar);
    }

}

void SpecialEntry::update(int current, int needed)
{
    if (mRechargeBar && needed)
    {
        float progress = static_cast<float>(current)
            / static_cast<float>(needed);
        mRechargeBar->setProgress(progress);
    }
}
