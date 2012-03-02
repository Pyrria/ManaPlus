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

#ifndef NET_MANASERV_TRADEHANDLER_H
#define NET_MANASERV_TRADEHANDLER_H

#include "net/tradehandler.h"

#include "net/manaserv/messagehandler.h"

namespace ManaServ
{

class TradeHandler : public MessageHandler, public Net::TradeHandler
{
    public:
        TradeHandler();

        void handleMessage(Net::MessageIn &msg);

        /**
         * Returns whether trade requests are accepted.
         *
         * @see setAcceptTradeRequests
         */
        bool acceptTradeRequests() const
        { return mAcceptTradeRequests; }

        /**
         * Sets whether trade requests are accepted. When set to false, trade
         * requests are automatically denied. When true, a popup will ask the
         * player whether he wants to trade.
         */
        void setAcceptTradeRequests(bool acceptRequests);

        void request(Being *being);

        void respond(bool accept);

        void addItem(Item *item, int amount);

        void removeItem(int slotNum, int amount);

        void setMoney(int amount);

        void confirm();

        void finish();

        void cancel();

    private:
        bool mAcceptTradeRequests;
};

} // namespace ManaServ

#endif // NET_MANASERV_TRADEHANDLER_H