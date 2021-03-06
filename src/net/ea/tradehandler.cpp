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

#include "net/ea/tradehandler.h"

#include "depricatedevent.h"
#include "inventory.h"
#include "item.h"
#include "logger.h"
#include "playerinfo.h"
#include "playerrelations.h"

#include "gui/confirmdialog.h"
#include "gui/tradewindow.h"

#include "net/inventoryhandler.h"
#include "net/messagein.h"

#include "net/ea/eaprotocol.h"

#include "utils/gettext.h"

#include "debug.h"

extern std::string tradePartnerName;
ConfirmDialog *confirmDlg;

/**
 * Listener for request trade dialogs
 */
namespace
{
    struct RequestTradeListener : public gcn::ActionListener
    {
        void action(const gcn::ActionEvent &event)
        {
            confirmDlg = nullptr;
            if (event.getId() == "ignore")
                player_relations.ignoreTrade(tradePartnerName);
            Net::getTradeHandler()->respond(event.getId() == "yes");
        }
    } listener;
}

namespace Ea
{

TradeHandler::TradeHandler()
{
    confirmDlg = nullptr;
}

void TradeHandler::removeItem(int slotNum A_UNUSED, int amount A_UNUSED)
{
    // TODO
}

void TradeHandler::processTradeRequest(Net::MessageIn &msg)
{
    // If a trade window or request window is already open, send a
    // trade cancel to any other trade request.
    //
    // Note that it would be nice if the server would prevent this
    // situation, and that the requesting player would get a
    // special message about the player being occupied.
    std::string tradePartnerNameTemp = msg.readString(24);

    if (player_relations.hasPermission(tradePartnerNameTemp,
        PlayerRelation::TRADE))
    {
        if (PlayerInfo::isTrading() || confirmDlg)
        {
            Net::getTradeHandler()->respond(false);
            return;
        }

        tradePartnerName = tradePartnerNameTemp;
        PlayerInfo::setTrading(true);
        if (tradeWindow)
        {
            if (tradePartnerName.empty() || tradeWindow->getAutoTradeNick()
                != tradePartnerName)
            {
                tradeWindow->clear();
                confirmDlg = new ConfirmDialog(_("Request for Trade"),
                    strprintf(_("%s wants to trade with you, do"
                    " you accept?"), tradePartnerName.c_str()), true);
                confirmDlg->addActionListener(&listener);
            }
            else
            {
                Net::getTradeHandler()->respond(true);
            }
        }
    }
    else
    {
        Net::getTradeHandler()->respond(false);
        return;
    }
}

void TradeHandler::processTradeResponse(Net::MessageIn &msg)
{
    if (confirmDlg || tradePartnerName.empty()
        || !player_relations.hasPermission(tradePartnerName,
        PlayerRelation::TRADE))
    {
        Net::getTradeHandler()->respond(false);
        return;
    }

    switch (msg.readInt8())
    {
        case 0: // Too far away
            SERVER_NOTICE(_("Trading isn't possible. Trade "
                "partner is too far away."))
            break;
        case 1: // Character doesn't exist
            SERVER_NOTICE(_("Trading isn't possible. Character "
                "doesn't exist."))
            break;
        case 2: // Invite request check failed...
            SERVER_NOTICE(_("Trade cancelled due to an unknown reason."))
            break;
        case 3: // Trade accepted
            if (tradeWindow)
            {
                tradeWindow->reset();
                tradeWindow->setCaption(strprintf(_("Trade: You and %s"),
                    tradePartnerName.c_str()));
                tradeWindow->initTrade(tradePartnerName);
                tradeWindow->setVisible(true);
            }
            break;
        case 4: // Trade cancelled
            if (player_relations.hasPermission(tradePartnerName,
                PlayerRelation::SPEECH_LOG))
            {
                SERVER_NOTICE(strprintf(_("Trade with %s cancelled."),
                              tradePartnerName.c_str()))
            }
            // otherwise ignore silently

            if (tradeWindow)
            {
                tradeWindow->setVisible(false);
//                        tradeWindow->clear();
            }
            PlayerInfo::setTrading(false);
            break;
        default: // Shouldn't happen as well, but to be sure
            SERVER_NOTICE(_("Unhandled trade cancel packet."))
            if (tradeWindow)
                tradeWindow->clear();
            break;
    }
}

void TradeHandler::processTradeItemAdd(Net::MessageIn &msg)
{
    int amount = msg.readInt32();
    int type = msg.readInt16();
    int identify = msg.readInt8();  // identified flag
    msg.readInt8();  // attribute
    int refine = msg.readInt8();  // refine
    msg.skip(8);     // card (4 shorts)

    // TODO: handle also identified, etc
    if (tradeWindow)
    {
        if (type == 0)
        {
            tradeWindow->setMoney(amount);
        }
        else
        {
            tradeWindow->addItem2(type, false, amount, refine,
                static_cast<unsigned char>(identify), false);
        }
    }
}

void TradeHandler::processTradeItemAddResponse(Net::MessageIn &msg)
{
    // Trade: New Item add response (was 0x00ea, now 01b1)
    const int index = msg.readInt16() - INVENTORY_OFFSET;
    Item *item = nullptr;
    if (PlayerInfo::getInventory())
        item = PlayerInfo::getInventory()->getItem(index);

    if (!item)
    {
        if (tradeWindow)
            tradeWindow->receivedOk(true);
        return;
    }
    int quantity = msg.readInt16();

    int res = msg.readInt8();
    switch (res)
    {
        case 0:
            // Successfully added item
            if (item->isEquipment() && item->isEquipped())
                Net::getInventoryHandler()->unequipItem(item);

            if (tradeWindow)
            {
                tradeWindow->addItem2(item->getId(), true,
                    quantity, item->getRefine(), item->getColor(),
                    item->isEquipment());
            }
            item->increaseQuantity(-quantity);
            break;
        case 1:
            // Add item failed - player overweighted
            SERVER_NOTICE(_("Failed adding item. Trade "
                    "partner is over weighted."))
            break;
        case 2:
              // Add item failed - player has no free slot
              SERVER_NOTICE(_("Failed adding item. Trade "
                      "partner has no free slot."))
              break;
        case 3:
              // Add item failed - non tradable item
              SERVER_NOTICE(_("Failed adding item. You "
                      "can't trade this item."))
              break;
        default:
            SERVER_NOTICE(_("Failed adding item for "
                    "unknown reason."))
            logger->log("QQQ SMSG_TRADE_ITEM_ADD_RESPONSE: "
                        + toString(res));
            break;
    }
}

void TradeHandler::processTradeOk(Net::MessageIn &msg)
{
    // 0 means ok from myself, 1 means ok from other;
    if (tradeWindow)
        tradeWindow->receivedOk(msg.readInt8() == 0);
    else
        msg.readInt8();
}

void TradeHandler::processTradeCancel(Net::MessageIn &msg A_UNUSED)
{
    SERVER_NOTICE(_("Trade canceled."))
    if (tradeWindow)
    {
        tradeWindow->setVisible(false);
        tradeWindow->reset();
    }
    PlayerInfo::setTrading(false);
}

void TradeHandler::processTradeComplete(Net::MessageIn &msg A_UNUSED)
{
    SERVER_NOTICE(_("Trade completed."))
    if (tradeWindow)
    {
        tradeWindow->setVisible(false);
        tradeWindow->reset();
    }
    PlayerInfo::setTrading(false);
}

} // namespace Ea
