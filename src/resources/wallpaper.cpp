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

#include "resources/wallpaper.h"

#include "resources/resourcemanager.h"
#include "logger.h"

#include "utils/stringutils.h"
#include "configuration.h"

#include <physfs.h>

#include <algorithm>
#include <cstring>
#include <time.h>

#include "debug.h"

struct WallpaperData
{
    std::string filename;
    int width;
    int height;
};

bool wallpaperCompare(WallpaperData a, WallpaperData b);

static std::vector<WallpaperData> wallpaperData;
static bool haveBackup; // Is the backup (no size given) version available?

static std::string wallpaperPath;
static std::string wallpaperFile;

// Search for the wallpaper path values sequentially..
static void initDefaultWallpaperPaths()
{
    ResourceManager *resman = ResourceManager::getInstance();

    // Init the path
    wallpaperPath = branding.getStringValue("wallpapersPath");

    if (wallpaperPath.empty() || !resman->isDirectory(wallpaperPath))
        wallpaperPath = paths.getValue("wallpapers", "");

    if (wallpaperPath.empty() || !resman->isDirectory(wallpaperPath))
        wallpaperPath = "graphics/images/";

    // Init the default file
    wallpaperFile = branding.getStringValue("wallpaperFile");

    if (!wallpaperFile.empty() && !resman->isDirectory(wallpaperFile))
        return;
    else
        wallpaperFile = paths.getValue("wallpaperFile", "");

    if (wallpaperFile.empty() || resman->isDirectory(wallpaperFile))
        wallpaperFile = "login_wallpaper.png";
}

bool wallpaperCompare(WallpaperData a, WallpaperData b)
{
    int aa = a.width * a.height;
    int ab = b.width * b.height;

    return (aa > ab || (aa == ab && a.width > b.width));
}
#include <iostream>
void Wallpaper::loadWallpapers()
{
    wallpaperData.clear();

    initDefaultWallpaperPaths();

    char **imgs = PHYSFS_enumerateFiles(wallpaperPath.c_str());

    for (char **i = imgs; *i; i++)
    {
        int width;
        int height;

        // If the backup file is found, we tell it.
        if (strncmp (*i, wallpaperFile.c_str(), strlen(*i)) == 0)
            haveBackup = true;

        // If the image format is terminated by: "_<width>x<height>.png"
        // It is taken as a potential wallpaper.

        // First, get the base filename of the image:
        std::string filename = *i;
        unsigned long separator = filename.rfind("_");
        filename = filename.substr(0, separator);

        // Check that the base filename doesn't have any '%' markers.
        separator = filename.find("%");
        if (separator == std::string::npos)
        {
            // Then, append the width and height search mask.
            filename.append("_%dx%d.png");

            if (sscanf(*i, filename.c_str(), &width, &height) == 2)
            {
                WallpaperData wp;
                wp.filename = wallpaperPath;
                wp.filename.append(*i);
                wp.width = width;
                wp.height = height;
                wallpaperData.push_back(wp);
            }
        }
    }

    PHYSFS_freeList(imgs);

    std::sort(wallpaperData.begin(), wallpaperData.end(), wallpaperCompare);
}

std::string Wallpaper::getWallpaper(int width, int height)
{
    WallpaperData wp;

    // Wallpaper filename container
    StringVect wallPaperVector;

    for (std::vector<WallpaperData>::const_iterator
         iter = wallpaperData.begin(), iter_end = wallpaperData.end();
         iter != iter_end; ++iter)
    {
        wp = *iter;
        if (wp.width <= width && wp.height <= height)
            wallPaperVector.push_back(wp.filename);
    }

    // If we've got more than one occurence of a valid wallpaper...
    if (!wallPaperVector.empty())
    {
        // Return randomly a wallpaper between vector[0] and
        // vector[vector.size() - 1]
        srand(static_cast<unsigned>(time(nullptr)));
        return wallPaperVector[int(static_cast<double>(
            wallPaperVector.size()) * rand() / (RAND_MAX + 1.0))];
    }

    // Return the backup file if everything else failed...
    if (haveBackup)
        return std::string(wallpaperPath + wallpaperFile);

    // Return an empty string if everything else failed
    return std::string();
}
