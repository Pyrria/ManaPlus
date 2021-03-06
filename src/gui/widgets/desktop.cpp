/*
 *  Desktop widget
 *  Copyright (c) 2009-2010  The Mana World Development Team
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

#include "gui/widgets/desktop.h"

#include "configuration.h"
#include "graphics.h"
#include "logger.h"
#include "main.h"

#include "gui/palette.h"
#include "gui/theme.h"

#include "gui/widgets/label.h"

#include "resources/image.h"
#include "resources/imagehelper.h"
#include "resources/resourcemanager.h"
#include "resources/wallpaper.h"

#include "utils/stringutils.h"

#include "debug.h"

Desktop::Desktop() :
    mWallpaper(nullptr)
{
    addWidgetListener(this);

    Wallpaper::loadWallpapers();

    std::string appName = branding.getValue("appName", std::string(""));

    if (appName.empty())
    {
        mVersionLabel = new Label(FULL_VERSION);
    }
    else
    {
        mVersionLabel = new Label(strprintf("%s (%s)", FULL_VERSION,
            appName.c_str()));
    }

    mVersionLabel->setBackgroundColor(
        Theme::getThemeColor(Theme::BACKGROUND, 128));
    add(mVersionLabel, 25, 2);

    mBackgroundColor = Theme::getThemeColor(Theme::BACKGROUND, 128);
    mBackgroundGrayColor = Theme::getThemeColor(Theme::BACKGROUND_GRAY);
}

Desktop::~Desktop()
{
    if (mWallpaper)
    {
        mWallpaper->decRef();
        mWallpaper = nullptr;
    }
}

void Desktop::reloadWallpaper()
{
    Wallpaper::loadWallpapers();
    setBestFittingWallpaper();
}

void Desktop::widgetResized(const gcn::Event &event A_UNUSED)
{
    setBestFittingWallpaper();
}

void Desktop::draw(gcn::Graphics *graphics)
{
    Graphics *g = static_cast<Graphics *>(graphics);

    if (!mWallpaper || (getWidth() > mWallpaper->getWidth() ||
        getHeight() > mWallpaper->getHeight()))
    {
        g->setColor(mBackgroundGrayColor);
        g->fillRectangle(gcn::Rectangle(0, 0, getWidth(), getHeight()));
    }

    if (mWallpaper)
    {
        if (!imageHelper->useOpenGL())
        {
            g->drawImage(mWallpaper,
                (getWidth() - mWallpaper->getWidth()) / 2,
                (getHeight() - mWallpaper->getHeight()) / 2);
        }
        else
        {
            g->drawRescaledImage(mWallpaper, 0, 0, 0, 0,
                mWallpaper->getWidth(), mWallpaper->getHeight(),
                getWidth(), getHeight(), false);
        }
    }

    // Draw a thin border under the application version...
    g->setColor(mBackgroundColor);
    g->fillRectangle(gcn::Rectangle(mVersionLabel->getDimension()));

    Container::draw(graphics);
}

void Desktop::setBestFittingWallpaper()
{
    if (!config.getBoolValue("showBackground"))
        return;

    const std::string wallpaperName =
        Wallpaper::getWallpaper(getWidth(), getHeight());

    Image *nWallPaper = Theme::getImageFromTheme(wallpaperName);

    if (nWallPaper)
    {
        if (mWallpaper)
            mWallpaper->decRef();

        if (!imageHelper->useOpenGL()
            && (nWallPaper->getWidth() != getWidth()
            || nWallPaper->getHeight() != getHeight()))
        {
            // We rescale to obtain a fullscreen wallpaper...
            Image *newRsclWlPpr = ResourceManager::getInstance()->getRescaled(
                nWallPaper, getWidth(), getHeight());

            // We replace the resource in the resource manager
            nWallPaper->decRef();
            if (newRsclWlPpr)
                mWallpaper = newRsclWlPpr;
            else
                mWallpaper = nWallPaper;
        }
        else
        {
            mWallpaper = nWallPaper;
        }
    }
    else
    {
        logger->log("Couldn't load %s as wallpaper", wallpaperName.c_str());
    }
}
