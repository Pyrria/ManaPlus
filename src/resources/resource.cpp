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

#include "resources/resource.h"

#include "client.h"
#include "logger.h"

#include "resources/resourcemanager.h"

#include "debug.h"

Resource::~Resource()
{
}

void Resource::incRef()
{
    mRefCount++;
}

void Resource::decRef()
{
    // Reference may not already have reached zero
    if (mRefCount == 0)
    {
        logger->log("Warning: mRefCount already zero for %s", mIdPath.c_str());
        return;
    }

    mRefCount--;

    if (mRefCount == 0)
    {
        // Warn the manager that this resource is no longer used.
        ResourceManager *resman = ResourceManager::getInstance();
        resman->release(this);
    }
}
