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

#ifndef NET_EATHENA_SKILLHANDLER_H
#define NET_EATHENA_SKILLHANDLER_H

#include "net/net.h"
#include "net/specialhandler.h"

#include "net/ea/specialhandler.h"

#include "net/eathena/messagehandler.h"

namespace EAthena
{

class SpecialHandler : public MessageHandler, public Ea::SpecialHandler
{
    public:
        SpecialHandler();

        void handleMessage(Net::MessageIn &msg);

        void useBeing(int id, int level, int beingId);

        void usePos(int id, int level, int x, int y);

        void useMap(int id, const std::string &map);
};

} // namespace EAthena

#endif // NET_EATHENA_SKILLHANDLER_H
