/*
 *  Setup action data configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
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

#ifndef GUI_SETUPACTIONDATA_H
#define GUI_SETUPACTIONDATA_H

#include "utils/gettext.h"

#include "debug.h"

int selectedData = 0;

struct SetupActionData
{
    const std::string name;
    const int actionId;
    std::string text;
};

static SetupActionData setupActionData0[] =
{
    {
        N_("Move Keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        N_("Move Up"),
        Input::KEY_MOVE_UP,
        "",
    },
    {
        N_("Move Down"),
        Input::KEY_MOVE_DOWN,
        "",
    },
    {
        N_("Move Left"),
        Input::KEY_MOVE_LEFT,
        "",
    },
    {
        N_("Move Right"),
        Input::KEY_MOVE_RIGHT,
        "",
    },
    {
        N_("Target and attack keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        N_("Attack"),
        Input::KEY_ATTACK,
        "",
    },
    {
        N_("Target & Attack"),
        Input::KEY_TARGET_ATTACK,
        "",
    },
    {
        N_("Move to Target"),
        Input::KEY_MOVE_TO_TARGET,
        "",
    },
    {
        N_("Change Move to Target type"),
        Input::KEY_CHANGE_MOVE_TO_TARGET,
        "",
    },
    {
        N_("Move to Home location"),
        Input::KEY_MOVE_TO_HOME,
        "",
    },
    {
        N_("Set home location"),
        Input::KEY_SET_HOME,
        "",
    },
    {
        N_("Move to navigation point"),
        Input::KEY_MOVE_TO_POINT,
        "",
    },
    {
        N_("Talk"),
        Input::KEY_TALK,
        "",
    },
    {
        N_("Stop Attack"),
        Input::KEY_STOP_ATTACK,
        "",
    },
    {
        N_("Untarget"),
        Input::KEY_UNTARGET,
        "",
    },
    {
        N_("Target monster"),
        Input::KEY_TARGET_MONSTER,
        "",
    },
    {
        N_("Target NPC"),
        Input::KEY_TARGET_NPC,
        "",
    },
    {
        N_("Target Player"),
        Input::KEY_TARGET_PLAYER,
        "",
    },
    {
        N_("Other Keys"),
        Input::KEY_NO_VALUE,
        "",
    },
    {
        N_("Pickup"),
        Input::KEY_PICKUP,
        "",
    },
    {
        N_("Change Pickup Type"),
        Input::KEY_CHANGE_PICKUP_TYPE,
        "",
    },
    {
        N_("Sit"),
        Input::KEY_SIT,
        "",
    },
    {
        N_("Screenshot"),
        Input::KEY_SCREENSHOT,
        "",
    },
    {
        N_("Enable/Disable Trading"),
        Input::KEY_TRADE,
        "",
    },
    {
        N_("Change Map View Mode"),
        Input::KEY_PATHFIND,
        "",
    },
    {
        N_("Select OK"),
        Input::KEY_OK,
        "",
    },
    {
        N_("Quit"),
        Input::KEY_QUIT,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData1[] =
{
    {
        N_("Shortcuts modifiers keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        N_("Item Shortcuts Key"),
        Input::KEY_SHORTCUTS_KEY,
        "",
    },
    {
        N_("Shortcuts keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        strprintf(N_("Item Shortcut %d"), 1),
        Input::KEY_SHORTCUT_1,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 2),
        Input::KEY_SHORTCUT_2,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 3),
        Input::KEY_SHORTCUT_3,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 4),
        Input::KEY_SHORTCUT_4,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 5),
        Input::KEY_SHORTCUT_5,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 6),
        Input::KEY_SHORTCUT_6,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 7),
        Input::KEY_SHORTCUT_7,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 8),
        Input::KEY_SHORTCUT_8,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 9),
        Input::KEY_SHORTCUT_9,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 10),
        Input::KEY_SHORTCUT_10,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 11),
        Input::KEY_SHORTCUT_11,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 12),
        Input::KEY_SHORTCUT_12,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 13),
        Input::KEY_SHORTCUT_13,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 14),
        Input::KEY_SHORTCUT_14,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 15),
        Input::KEY_SHORTCUT_15,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 16),
        Input::KEY_SHORTCUT_16,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 17),
        Input::KEY_SHORTCUT_17,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 18),
        Input::KEY_SHORTCUT_18,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 19),
        Input::KEY_SHORTCUT_19,
        "",
    },
    {
        strprintf(N_("Item Shortcut %d"), 20),
        Input::KEY_SHORTCUT_20,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData2[] =
{
    {
        N_("Hide Windows"),
        Input::KEY_HIDE_WINDOWS,
        "",
    },
    {
        N_("Help Window"),
        Input::KEY_WINDOW_HELP,
        "",
    },
    {
        N_("Status Window"),
        Input::KEY_WINDOW_STATUS,
        "",
    },
    {
        N_("Inventory Window"),
        Input::KEY_WINDOW_INVENTORY,
        "",
    },
    {
        N_("Equipment Window"),
        Input::KEY_WINDOW_EQUIPMENT,
        "",
    },
    {
        N_("Skill Window"),
        Input::KEY_WINDOW_SKILL,
        "",
    },
    {
        N_("Minimap Window"),
        Input::KEY_WINDOW_MINIMAP,
        "",
    },
    {
        N_("Chat Window"),
        Input::KEY_WINDOW_CHAT,
        "",
    },
    {
        N_("Item Shortcut Window"),
        Input::KEY_WINDOW_SHORTCUT,
        "",
    },
    {
        N_("Setup Window"),
        Input::KEY_WINDOW_SETUP,
        "",
    },
    {
        N_("Debug Window"),
        Input::KEY_WINDOW_DEBUG,
        "",
    },
    {
        N_("Social Window"),
        Input::KEY_WINDOW_SOCIAL,
        "",
    },
    {
        N_("Emote Shortcut Window"),
        Input::KEY_WINDOW_EMOTE_SHORTCUT,
        "",
    },
    {
        N_("Outfits Window"),
        Input::KEY_WINDOW_OUTFIT,
        "",
    },
    {
        N_("Shop Window"),
        Input::KEY_WINDOW_SHOP,
        "",
    },
    {
        N_("Quick drop Window"),
        Input::KEY_WINDOW_DROP,
        "",
    },
    {
        N_("Kill Stats Window"),
        Input::KEY_WINDOW_KILLS,
        "",
    },
    {
        N_("Commands Window"),
        Input::KEY_WINDOW_SPELLS,
        "",
    },
    {
        N_("Bot Checker Window"),
        Input::KEY_WINDOW_BOT_CHECKER,
        "",
    },
    {
        N_("Who Is Online Window"),
        Input::KEY_WINDOW_ONLINE,
        "",
    },
    {
        N_("Did you know Window"),
        Input::KEY_WINDOW_DIDYOUKNOW,
        "",
    },
    {
        N_("Previous Social Tab"),
        Input::KEY_PREV_SOCIAL_TAB,
        "",
    },
    {
        N_("Next Social Tab"),
        Input::KEY_NEXT_SOCIAL_TAB,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData3[] =
{
    {
        N_("Emote modifiers keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        N_("Emote modifier key"),
        Input::KEY_EMOTE,
        "",
    },
    {
        N_("Emote shortcuts"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        strprintf(N_("Emote Shortcut %d"), 1),
        Input::KEY_EMOTE_1,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 2),
        Input::KEY_EMOTE_2,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 3),
        Input::KEY_EMOTE_3,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 4),
        Input::KEY_EMOTE_4,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 5),
        Input::KEY_EMOTE_5,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 6),
        Input::KEY_EMOTE_6,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 7),
        Input::KEY_EMOTE_7,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 8),
        Input::KEY_EMOTE_8,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 9),
        Input::KEY_EMOTE_9,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 10),
        Input::KEY_EMOTE_10,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 11),
        Input::KEY_EMOTE_11,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 12),
        Input::KEY_EMOTE_12,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 13),
        Input::KEY_EMOTE_13,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 14),
        Input::KEY_EMOTE_14,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 15),
        Input::KEY_EMOTE_15,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 16),
        Input::KEY_EMOTE_16,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 17),
        Input::KEY_EMOTE_17,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 18),
        Input::KEY_EMOTE_18,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 19),
        Input::KEY_EMOTE_19,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 20),
        Input::KEY_EMOTE_20,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 21),
        Input::KEY_EMOTE_21,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 22),
        Input::KEY_EMOTE_22,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 23),
        Input::KEY_EMOTE_23,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 24),
        Input::KEY_EMOTE_24,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 25),
        Input::KEY_EMOTE_25,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 26),
        Input::KEY_EMOTE_26,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 27),
        Input::KEY_EMOTE_27,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 28),
        Input::KEY_EMOTE_28,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 29),
        Input::KEY_EMOTE_29,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 30),
        Input::KEY_EMOTE_30,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 31),
        Input::KEY_EMOTE_31,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 32),
        Input::KEY_EMOTE_32,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 33),
        Input::KEY_EMOTE_33,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 34),
        Input::KEY_EMOTE_34,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 35),
        Input::KEY_EMOTE_35,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 36),
        Input::KEY_EMOTE_36,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 37),
        Input::KEY_EMOTE_37,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 38),
        Input::KEY_EMOTE_38,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 39),
        Input::KEY_EMOTE_39,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 40),
        Input::KEY_EMOTE_40,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 41),
        Input::KEY_EMOTE_41,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 42),
        Input::KEY_EMOTE_42,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 43),
        Input::KEY_EMOTE_43,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 44),
        Input::KEY_EMOTE_44,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 45),
        Input::KEY_EMOTE_45,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 46),
        Input::KEY_EMOTE_46,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 47),
        Input::KEY_EMOTE_47,
        "",
    },
    {
        strprintf(N_("Emote Shortcut %d"), 48),
        Input::KEY_EMOTE_48,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData4[] =
{
    {
        N_("Outfits keys"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        N_("Wear Outfit"),
        Input::KEY_WEAR_OUTFIT,
        "",
    },
    {
        N_("Copy Outfit"),
        Input::KEY_COPY_OUTFIT,
        "",
    },
    {
        N_("Copy equipped to Outfit"),
        Input::KEY_COPY_EQUIPED_OUTFIT,
        "",
    },
    {
        N_("Outfits shortcuts"),
        Input::KEY_NO_VALUE,
        ""
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 1),
        Input::KEY_OUTFIT_1,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 2),
        Input::KEY_OUTFIT_2,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 3),
        Input::KEY_OUTFIT_3,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 4),
        Input::KEY_OUTFIT_4,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 5),
        Input::KEY_OUTFIT_5,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 6),
        Input::KEY_OUTFIT_6,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 7),
        Input::KEY_OUTFIT_7,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 8),
        Input::KEY_OUTFIT_8,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 9),
        Input::KEY_OUTFIT_9,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 10),
        Input::KEY_OUTFIT_10,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 11),
        Input::KEY_OUTFIT_11,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 12),
        Input::KEY_OUTFIT_12,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 13),
        Input::KEY_OUTFIT_13,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 14),
        Input::KEY_OUTFIT_14,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 15),
        Input::KEY_OUTFIT_15,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 16),
        Input::KEY_OUTFIT_16,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 17),
        Input::KEY_OUTFIT_17,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 18),
        Input::KEY_OUTFIT_18,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 19),
        Input::KEY_OUTFIT_19,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 20),
        Input::KEY_OUTFIT_20,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 21),
        Input::KEY_OUTFIT_21,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 22),
        Input::KEY_OUTFIT_22,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 23),
        Input::KEY_OUTFIT_23,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 24),
        Input::KEY_OUTFIT_24,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 25),
        Input::KEY_OUTFIT_25,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 26),
        Input::KEY_OUTFIT_26,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 27),
        Input::KEY_OUTFIT_27,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 28),
        Input::KEY_OUTFIT_28,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 29),
        Input::KEY_OUTFIT_29,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 30),
        Input::KEY_OUTFIT_30,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 31),
        Input::KEY_OUTFIT_31,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 32),
        Input::KEY_OUTFIT_32,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 33),
        Input::KEY_OUTFIT_33,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 34),
        Input::KEY_OUTFIT_34,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 35),
        Input::KEY_OUTFIT_35,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 36),
        Input::KEY_OUTFIT_36,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 37),
        Input::KEY_OUTFIT_37,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 38),
        Input::KEY_OUTFIT_38,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 39),
        Input::KEY_OUTFIT_39,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 40),
        Input::KEY_OUTFIT_40,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 41),
        Input::KEY_OUTFIT_41,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 42),
        Input::KEY_OUTFIT_42,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 43),
        Input::KEY_OUTFIT_43,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 44),
        Input::KEY_OUTFIT_44,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 45),
        Input::KEY_OUTFIT_45,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 46),
        Input::KEY_OUTFIT_46,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 47),
        Input::KEY_OUTFIT_47,
        "",
    },
    {
        strprintf(N_("Outfit Shortcut %d"), 48),
        Input::KEY_OUTFIT_48,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData5[] =
{
    {
        N_("Toggle Chat"),
        Input::KEY_TOGGLE_CHAT,
        "",
    },
    {
        N_("Scroll Chat Up"),
        Input::KEY_SCROLL_CHAT_UP,
        "",
    },
    {
        N_("Scroll Chat Down"),
        Input::KEY_SCROLL_CHAT_DOWN,
        "",
    },
    {
        N_("Previous Chat Tab"),
        Input::KEY_PREV_CHAT_TAB,
        "",
    },
    {
        N_("Next Chat Tab"),
        Input::KEY_NEXT_CHAT_TAB,
        "",
    },
    {
        N_("Close current Chat Tab"),
        Input::KEY_CLOSE_CHAT_TAB,
        "",
    },
    {
        N_("Previous chat line"),
        Input::KEY_CHAT_PREV_HISTORY,
        "",
    },
    {
        N_("Next chat line"),
        Input::KEY_CHAT_NEXT_HISTORY,
        "",
    },
    {
        N_("Chat Auto Complete"),
        Input::KEY_AUTOCOMPLETE_CHAT,
        "",
    },
    {
        N_("Deactivate Chat Input"),
        Input::KEY_DEACTIVATE_CHAT,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData setupActionData6[] =
{
    {
        N_("Ignore input 1"),
        Input::KEY_IGNORE_INPUT_1,
        "",
    },
    {
        N_("Ignore input 2"),
        Input::KEY_IGNORE_INPUT_2,
        "",
    },
    {
        N_("Direct Up"),
        Input::KEY_DIRECT_UP,
        "",
    },
    {
        N_("Direct Down"),
        Input::KEY_DIRECT_DOWN,
        "",
    },
    {
        N_("Direct Left"),
        Input::KEY_DIRECT_LEFT,
        "",
    },
    {
        N_("Direct Right"),
        Input::KEY_DIRECT_RIGHT,
        "",
    },
    {
        N_("Crazy moves"),
        Input::KEY_CRAZY_MOVES,
        "",
    },
    {
        N_("Change Crazy Move mode"),
        Input::KEY_CHANGE_CRAZY_MOVES_TYPE,
        "",
    },
    {
        N_("Quick Drop N Items from 0 slot"),
        Input::KEY_QUICK_DROP,
        "",
    },
    {
        N_("Quick Drop N Items"),
        Input::KEY_QUICK_DROPN,
        "",
    },
    {
        N_("Switch Quick Drop Counter"),
        Input::KEY_SWITCH_QUICK_DROP,
        "",
    },
    {
        N_("Quick heal target or self"),
        Input::KEY_MAGIC_INMA1,
        "",
    },
    {
        N_("Use #itenplz spell"),
        Input::KEY_MAGIC_ITENPLZ,
        "",
    },
    {
        N_("Use magic attack"),
        Input::KEY_MAGIC_ATTACK,
        "",
    },
    {
        N_("Switch magic attack"),
        Input::KEY_SWITCH_MAGIC_ATTACK,
        "",
    },
    {
        N_("Switch pvp attack"),
        Input::KEY_SWITCH_PVP_ATTACK,
        "",
    },
    {
        N_("Change move type"),
        Input::KEY_INVERT_DIRECTION,
        "",
    },
    {
        N_("Change Attack Weapon Type"),
        Input::KEY_CHANGE_ATTACK_WEAPON_TYPE,
        "",
    },
    {
        N_("Change Attack Type"),
        Input::KEY_CHANGE_ATTACK_TYPE,
        "",
    },
    {
        N_("Change Follow mode"),
        Input::KEY_CHANGE_FOLLOW_MODE,
        "",
    },
    {
        N_("Change Imitation mode"),
        Input::KEY_CHANGE_IMITATION_MODE,
        "",
    },
    {
        N_("Disable / Enable Game modifier keys"),
        Input::KEY_DISABLE_GAME_MODIFIERS,
        "",
    },
    {
        N_("On / Off audio"),
        Input::KEY_CHANGE_AUDIO,
        "",
    },
    {
        N_("Enable / Disable away mode"),
        Input::KEY_AWAY,
        "",
    },
    {
        N_("Emulate right click from keyboard"),
        Input::KEY_RIGHT_CLICK,
        "",
    },
    {
        N_("Toggle camera mode"),
        Input::KEY_CAMERA,
        "",
    },
    {
        N_("Modifier key"),
        Input::KEY_MOD,
        "",
    },
    {
        "",
        Input::KEY_NO_VALUE,
        ""
    }
};

static SetupActionData *setupActionData[] =
{
    setupActionData0,
    setupActionData1,
    setupActionData2,
    setupActionData3,
    setupActionData4,
    setupActionData5,
    setupActionData6
};

static const char *pages[] =
{
    N_("Basic"),
    N_("Shortcuts"),
    N_("Windows"),
    N_("Emotes"),
    N_("Outfits"),
    N_("Chat"),
    N_("Other"),
    nullptr
};

#endif