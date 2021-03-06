/*
 *  Custom keyboard shortcuts configuration
 *  Copyright (C) 2007  Joshua Langley <joshlangley@optusnet.com.au>
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

#include "keyboardconfig.h"

#include "configuration.h"
#include "inputmanager.h"
#include "logger.h"

#include "gui/sdlinput.h"

#include "utils/gettext.h"
#include "utils/stringutils.h"

#include <SDL_events.h>

#include "debug.h"

extern volatile int tick_time;

KeyboardConfig::KeyboardConfig() :
    mEnabled(true),
    mActiveKeys(nullptr),
    mActiveKeys2(nullptr)
{
}

void KeyboardConfig::init()
{
    mEnabled = true;
    if (mActiveKeys2)
        delete mActiveKeys2;
    mActiveKeys2 = new uint8_t[500];
}

void KeyboardConfig::deinit()
{
    delete [] mActiveKeys2;
    mActiveKeys2 = nullptr;
}

int KeyboardConfig::getKeyValueFromEvent(const SDL_Event &event) const
{
    if (event.key.keysym.sym)
        return event.key.keysym.sym;
    else if (event.key.keysym.scancode > 1)
        return -event.key.keysym.scancode;
    return 0;
}

int KeyboardConfig::getKeyIndex(const SDL_Event &event, int grp) const
{
    const int keyValue = getKeyValueFromEvent(event);
    return inputManager.getKeyIndex(keyValue, grp, INPUT_KEYBOARD);
}

void KeyboardConfig::refreshActiveKeys()
{
    mActiveKeys = SDL_GetKeyState(nullptr);
}

std::string KeyboardConfig::getKeyName(int key)
{
    if (key == Input::KEY_NO_VALUE)
        return "";
    if (key >= 0)
        return SDL_GetKeyName(static_cast<SDLKey>(key));

    return strprintf(_("key_%d"), key);
}

std::string KeyboardConfig::getKeyShortString(const std::string &key) const
{
    if (key == "backspace")
    {
        return "bksp";
    }
    else if (key == "unknown key")
    {
        // TRANSLATORS: Unknown key short string. This string must be maximum 5 chars
        return _("u key");
    }
    return key;
}

SDLKey KeyboardConfig::getKeyFromEvent(const SDL_Event &event) const
{
    return event.key.keysym.sym;
}

KeysVector *KeyboardConfig::getActionVector(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("key triggerAction: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

KeysVector *KeyboardConfig::getActionVectorByKey(int i)
{
//    logger->log("key triggerAction: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToAction.find(i) != mKeyToAction.end())
        return &mKeyToAction[i];
    return nullptr;
}

int KeyboardConfig::getActionId(const SDL_Event &event)
{
    const int i = getKeyValueFromEvent(event);
//    logger->log("getActionId: %d", i);
    if (i != 0 && i < SDLK_LAST && mKeyToId.find(i) != mKeyToId.end())
        return mKeyToId[i];
    return -1;
}

bool KeyboardConfig::isActionActive(int index) const
{
    if (!mActiveKeys)
        return false;

    const KeyFunction &key = inputManager.getKey(index);
    for (size_t i = 0; i < KeyFunctionSize; i ++)
    {
        if (key.values[i].type != INPUT_KEYBOARD)
            continue;

        const int value = key.values[i].value;
        if (value >= 0)
        {
            if (mActiveKeys[value])
                return true;
        }
        else if (value < -1 && value > -500)
        {
            if (mActiveKeys2[-value])
                return true;
        }
    }
    return false;
}

void KeyboardConfig::update()
{
    inputManager.updateKeyActionMap(mKeyToAction, mKeyToId,
        mKeyTimeMap, INPUT_KEYBOARD);
}

void KeyboardConfig::handleActicateKey(const SDL_Event &event)
{
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 1;
    resetRepeat(key);
}

void KeyboardConfig::handleDeActicateKey(const SDL_Event &event)
{
    const int key = getKeyValueFromEvent(event);
    if (key < -1 && key > -500)
        mActiveKeys2[-key] = 0;
    resetRepeat(key);
}

void KeyboardConfig::handleRepeat(int time)
{
    for (KeyTimeMapIter it = mKeyTimeMap.begin(), it_end = mKeyTimeMap.end();
         it != it_end; ++ it)
    {
        bool repeat(false);
        const int key = (*it).first;
        int &keyTime = (*it).second;
        if (key >= 0)
        {
            if (mActiveKeys[key])
                repeat = true;
        }
        else if (key < -1 && key > -500)
        {
            if (mActiveKeys2[-key])
                repeat = true;
        }
        if (repeat && abs(keyTime - time) > 10)
        {
            keyTime = time;
            inputManager.triggerAction(getActionVectorByKey(key));
        }
    }
}

void KeyboardConfig::resetRepeat(int key)
{
    KeyTimeMapIter it = mKeyTimeMap.find(key);
    if (it != mKeyTimeMap.end())
        (*it).second = tick_time;
}
