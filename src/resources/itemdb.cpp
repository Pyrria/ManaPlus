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

#include "resources/itemdb.h"

#include "client.h"
#include "configuration.h"
#include "logger.h"

#include "resources/iteminfo.h"
#include "resources/resourcemanager.h"

#include "utils/dtor.h"
#include "utils/gettext.h"
#include "utils/stringutils.h"
#include "utils/xml.h"

#include "debug.h"

namespace
{
    ItemDB::ItemInfos mItemInfos;
    ItemDB::NamedItemInfos mNamedItemInfos;
    ItemInfo *mUnknown;
    bool mLoaded = false;
    StringVect mTagNames;
    std::map<std::string, int> mTags;
}

// Forward declarations
static void loadSpriteRef(ItemInfo *itemInfo, XmlNodePtr node);
static void loadSoundRef(ItemInfo *itemInfo, XmlNodePtr node);
static void loadFloorSprite(SpriteDisplay *display, XmlNodePtr node);
static void loadReplaceSprite(ItemInfo *itemInfo, XmlNodePtr replaceNode);
static void loadOrderSprite(ItemInfo *itemInfo, XmlNodePtr node,
                            bool drawAfter);
static int parseSpriteName(std::string name);
static int parseDirectionName(std::string name);

static char const *const fields[][2] =
{
    { "attack",    N_("Attack %+d")    },
    { "defense",   N_("Defense %+d")   },
    { "hp",        N_("HP %+d")        },
    { "mp",        N_("MP %+d")        }
};

static std::vector<ItemDB::Stat> extraStats;

void ItemDB::setStatsList(const std::vector<ItemDB::Stat> &stats)
{
    extraStats = stats;
}

static ItemType itemTypeFromString(const std::string &name)
{
    if (name == "generic" || name == "other")
    {
        return ITEM_UNUSABLE;
    }
    else if (name == "usable")
    {
        return ITEM_USABLE;
    }
    else if (name == "equip-1hand")
    {
        return ITEM_EQUIPMENT_ONE_HAND_WEAPON;
    }
    else if (name == "equip-2hand")
    {
        return ITEM_EQUIPMENT_TWO_HANDS_WEAPON;
    }
    else if (name == "equip-torso")
    {
        return ITEM_EQUIPMENT_TORSO;
    }
    else if (name == "equip-arms")
    {
        return ITEM_EQUIPMENT_ARMS;
    }
    else if (name == "equip-head")
    {
        return ITEM_EQUIPMENT_HEAD;
    }
    else if (name == "equip-legs")
    {
        return ITEM_EQUIPMENT_LEGS;
    }
    else if (name == "equip-shield")
    {
        return ITEM_EQUIPMENT_SHIELD;
    }
    else if (name == "equip-ring")
    {
        return ITEM_EQUIPMENT_RING;
    }
    else if (name == "equip-charm")
    {
        return ITEM_EQUIPMENT_CHARM;
    }
    else if (name == "equip-necklace" || name == "equip-neck")
    {
        return ITEM_EQUIPMENT_NECKLACE;
    }
    else if (name == "equip-feet")
    {
        return ITEM_EQUIPMENT_FEET;
    }
    else if (name == "equip-ammo")
    {
        return ITEM_EQUIPMENT_AMMO;
    }
    else if (name == "racesprite")
    {
        return ITEM_SPRITE_RACE;
    }
    else if (name == "hairsprite")
    {
        return ITEM_SPRITE_HAIR;
    }
    else
    {
        logger->log("Unknown item type: " + name);
        return ITEM_UNUSABLE;
    }
}

void ItemDB::load()
{
    if (mLoaded)
        unload();

    int tagNum = 0;
    logger->log1("Initializing item database...");

    mTags.clear();
    mTagNames.clear();
    mTagNames.push_back("All");
    mTagNames.push_back("Usable");
    mTagNames.push_back("Unusable");
    mTagNames.push_back("Equipment");
    mTags["All"] = tagNum ++;
    mTags["Usable"] = tagNum ++;
    mTags["Unusable"] = tagNum ++;
    mTags["Equipment"] = tagNum ++;

    mUnknown = new ItemInfo;
    mUnknown->setName(_("Unknown item"));
    mUnknown->setDisplay(SpriteDisplay());
    std::string errFile = paths.getStringValue("spriteErrorFile");
    mUnknown->setSprite(errFile, GENDER_MALE, 0);
    mUnknown->setSprite(errFile, GENDER_FEMALE, 0);
    mUnknown->setSprite(errFile, GENDER_OTHER, 0);
    mUnknown->addTag(mTags["All"]);

    XML::Document doc("items.xml");
    XmlNodePtr rootNode = doc.rootNode();

    if (!rootNode || !xmlNameEqual(rootNode, "items"))
    {
        logger->log("ItemDB: Error while loading items.xml!");
        mLoaded = true;
        return;
    }

    for_each_xml_child_node(node, rootNode)
    {
        if (!xmlNameEqual(node, "item"))
            continue;

        int id = XML::getProperty(node, "id", 0);

        if (id == 0)
        {
            logger->log1("ItemDB: Invalid or missing item ID in items.xml!");
            continue;
        }
        else if (mItemInfos.find(id) != mItemInfos.end())
        {
            logger->log("ItemDB: Redefinition of item ID %d", id);
        }

        std::string typeStr = XML::getProperty(node, "type", "other");
        int weight = XML::getProperty(node, "weight", 0);
        int view = XML::getProperty(node, "view", 0);

        std::string name = XML::langProperty(node, "name", "");
        std::string image = XML::getProperty(node, "image", "");
        std::string floor = XML::getProperty(node, "floor", "");
        std::string description = XML::langProperty(node, "description", "");
        std::string attackAction = XML::getProperty(node, "attack-action", "");
        std::string drawBefore = XML::getProperty(node, "drawBefore", "");
        std::string drawAfter = XML::getProperty(node, "drawAfter", "");
//        std::string removeSprite = XML::getProperty(node, "removeSprite", "");
        std::string colors;
        if (serverVersion >= 1)
        {
            colors = XML::getProperty(node, "colors", "");

            // check for empty hair palete
            if (colors.empty() && id <= -1 && id > -100)
                colors = "hair";
        }
        else
        {
            if (id <= -1 && id > -100)
                colors = "hair";
            else
                colors = "";
        }

        std::string tags[3];
        tags[0] = XML::getProperty(node, "tag",
            XML::getProperty(node, "tag1", ""));
        tags[1] = XML::getProperty(node, "tag2", "");
        tags[2] = XML::getProperty(node, "tag3", "");

        int drawPriority = XML::getProperty(node, "drawPriority", 0);

        int attackRange = XML::getProperty(node, "attack-range", 0);
        std::string missileParticle = XML::getProperty(
            node, "missile-particle", "");
        int hitEffectId = XML::getProperty(node, "hit-effect-id",
            paths.getIntValue("hitEffectId"));
        int criticalEffectId = XML::getProperty(node, "critical-hit-effect-id",
            paths.getIntValue("criticalHitEffectId"));

        SpriteDisplay display;
        display.image = image;
        if (floor != "")
            display.floor = floor;
        else
            display.floor = image;

        ItemInfo *itemInfo = new ItemInfo;
        itemInfo->setId(id);
        itemInfo->setName(name.empty() ? _("unnamed") : name);
        itemInfo->setDescription(description);
        itemInfo->setType(itemTypeFromString(typeStr));
        itemInfo->addTag(mTags["All"]);
        switch (itemInfo->getType())
        {
            case ITEM_USABLE:
                itemInfo->addTag(mTags["Usable"]);
                break;
            case ITEM_UNUSABLE:
                itemInfo->addTag(mTags["Unusable"]);
                break;
            default:
            case ITEM_EQUIPMENT_ONE_HAND_WEAPON:
            case ITEM_EQUIPMENT_TWO_HANDS_WEAPON:
            case ITEM_EQUIPMENT_TORSO:
            case ITEM_EQUIPMENT_ARMS:
            case ITEM_EQUIPMENT_HEAD:
            case ITEM_EQUIPMENT_LEGS:
            case ITEM_EQUIPMENT_SHIELD:
            case ITEM_EQUIPMENT_RING:
            case ITEM_EQUIPMENT_NECKLACE:
            case ITEM_EQUIPMENT_FEET:
            case ITEM_EQUIPMENT_AMMO:
            case ITEM_EQUIPMENT_CHARM:
            case ITEM_SPRITE_RACE:
            case ITEM_SPRITE_HAIR:
                itemInfo->addTag(mTags["Equipment"]);
                break;
        }
        for (int f = 0; f < 3; f++)
        {
            if (tags[f] != "")
            {
                if (mTags.find(tags[f]) == mTags.end())
                {
                    mTagNames.push_back(tags[f]);
                    mTags[tags[f]] = tagNum ++;
                }
                itemInfo->addTag(mTags[tags[f]]);
            }
        }

        itemInfo->setView(view);
        itemInfo->setWeight(weight);
        itemInfo->setAttackAction(attackAction);
        itemInfo->setAttackRange(attackRange);
        itemInfo->setMissileParticleFile(missileParticle);
        itemInfo->setHitEffectId(hitEffectId);
        itemInfo->setCriticalHitEffectId(criticalEffectId);
        itemInfo->setDrawBefore(-1, parseSpriteName(drawBefore));
        itemInfo->setDrawAfter(-1, parseSpriteName(drawAfter));
        itemInfo->setDrawPriority(-1, drawPriority);
        itemInfo->setColorsList(colors);

        std::string effect;
        for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++ i)
        {
            int value = XML::getProperty(node, fields[i][0], 0);
            if (!value)
                continue;
            if (!effect.empty())
                effect += " / ";
            effect += strprintf(gettext(fields[i][1]), value);
        }
        for (std::vector<Stat>::const_iterator it = extraStats.begin(),
             it_end = extraStats.end(); it != it_end; ++it)
        {
            int value = XML::getProperty(node, it->tag.c_str(), 0);
            if (!value)
                continue;
            if (!effect.empty())
                effect += " / ";
            effect += strprintf(it->format.c_str(), value);
        }
        std::string temp = XML::langProperty(node, "effect", "");
        if (!effect.empty() && !temp.empty())
            effect += " / ";
        effect += temp;
        itemInfo->setEffect(effect);

        for_each_xml_child_node(itemChild, node)
        {
            if (xmlNameEqual(itemChild, "sprite"))
            {
                std::string attackParticle = XML::getProperty(
                    itemChild, "particle-effect", "");
                itemInfo->setParticleEffect(attackParticle);

                loadSpriteRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "sound"))
            {
                loadSoundRef(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "floor"))
            {
                loadFloorSprite(&display, itemChild);
            }
            else if (xmlNameEqual(itemChild, "replace"))
            {
                loadReplaceSprite(itemInfo, itemChild);
            }
            else if (xmlNameEqual(itemChild, "drawAfter"))
            {
                loadOrderSprite(itemInfo, itemChild, true);
            }
            else if (xmlNameEqual(itemChild, "drawBefore"))
            {
                loadOrderSprite(itemInfo, itemChild, false);
            }
        }

        itemInfo->setDisplay(display);

        mItemInfos[id] = itemInfo;
        if (!name.empty())
        {
            temp = normalize(name);

            NamedItemInfos::const_iterator itr = mNamedItemInfos.find(temp);
            if (itr == mNamedItemInfos.end())
            {
                mNamedItemInfos[temp] = itemInfo;
            }
            else
            {
                logger->log("ItemDB: Duplicate name of item found item %d",
                    id);
            }
        }

        if (!attackAction.empty())
        {
            if (attackRange == 0)
            {
                logger->log("ItemDB: Missing attack range from weapon %i!",
                    id);
            }
        }

#define CHECK_PARAM(param, error_value) \
        if (param == error_value) \
            logger->log("ItemDB: Missing " #param " attribute for item %i!", \
                        id)

        if (id >= 0 && typeStr != "other")
        {
            CHECK_PARAM(name, "");
            CHECK_PARAM(description, "");
            CHECK_PARAM(image, "");
        }
        // CHECK_PARAM(effect, "");
        // CHECK_PARAM(type, 0);
        // CHECK_PARAM(weight, 0);
        // CHECK_PARAM(slot, 0);

#undef CHECK_PARAM
    }

    mLoaded = true;
}

const StringVect &ItemDB::getTags()
{
    return mTagNames;
}

int ItemDB::getTagId(std::string tagName)
{
    return mTags[tagName];
}

void ItemDB::unload()
{
    logger->log1("Unloading item database...");

    delete mUnknown;
    mUnknown = nullptr;

    delete_all(mItemInfos);
    mItemInfos.clear();
    mNamedItemInfos.clear();
    mTags.clear();
    mTagNames.clear();
    mLoaded = false;
}

bool ItemDB::exists(int id)
{
    if (!mLoaded)
        return false;

    ItemInfos::const_iterator i = mItemInfos.find(id);

    return i != mItemInfos.end();
}

const ItemInfo &ItemDB::get(int id)
{
    if (!mLoaded)
        load();

    ItemInfos::const_iterator i = mItemInfos.find(id);

    if (i == mItemInfos.end())
    {
        logger->log("ItemDB: Warning, unknown item ID# %d", id);
        return *mUnknown;
    }

    return *(i->second);
}

const ItemInfo &ItemDB::get(const std::string &name)
{
    if (!mLoaded)
        load();

    NamedItemInfos::const_iterator i = mNamedItemInfos.find(normalize(name));

    if (i == mNamedItemInfos.end())
    {
        if (!name.empty())
        {
            logger->log("ItemDB: Warning, unknown item name \"%s\"",
                        name.c_str());
        }
        return *mUnknown;
    }

    return *(i->second);
}

const std::map<int, ItemInfo*> &ItemDB::getItemInfos()
{
    return mItemInfos;
}

int parseSpriteName(std::string name)
{
    int id = -1;
    if (name == "shoes" || name == "boot" || name == "boots")
    {
        id = 1;
    }
    else if (name == "bottomclothes" || name == "bottom" || name == "pants")
    {
        id = 2;
    }
    else if (name == "topclothes" || name == "top"
             || name == "torso" || name == "body")
    {
        id = 3;
    }
    else if (name == "misc1")
    {
        id = 4;
    }
    else if (name == "misc2" || name == "scarf" || name == "scarfs")
    {
        id = 5;
    }
    else if (name == "hair")
    {
        id = 6;
    }
    else if (name == "hat" || name == "hats")
    {
        id = 7;
    }
    else if (name == "wings")
    {
        id = 8;
    }
    else if (name == "glove" || name == "gloves")
    {
        id = 9;
    }
    else if (name == "weapon" || name == "weapons")
    {
        id = 10;
    }
    else if (name == "shield" || name == "shields")
    {
        id = 11;
    }
    else if (name == "amulet" || name == "amulets")
    {
        id = 12;
    }
    else if (name == "ring" || name == "rings")
    {
        id = 13;
    }

    return id;
}

int parseDirectionName(std::string name)
{
    int id = -1;
    if (name == "down")
    {
        if (serverVersion > 0)
            id = DIRECTION_DOWN;
        else
            id = -2;
    }
    else if (name == "downleft" || name == "leftdown")
    {
        id = DIRECTION_DOWNLEFT;
    }
    else if (name == "left")
    {
        id = DIRECTION_LEFT;
    }
    else if (name == "upleft" || name == "leftup")
    {
        id = DIRECTION_UPLEFT;
    }
    else if (name == "up")
    {
        if (serverVersion > 0)
            id = DIRECTION_UP;
        else
            id = -3;
    }
    else if (name == "upright" || name == "rightup")
    {
        id = DIRECTION_UPRIGHT;
    }
    else if (name == "right")
    {
        id = DIRECTION_RIGHT;
    }
    else if (name == "downright" || name == "rightdown")
    {
        id = DIRECTION_DOWNRIGHT;
    }
    else if (name == "downall")
    {
        id = -2;
    }
    else if (name == "upall")
    {
        id = -3;
    }

    return id;
}

void loadSpriteRef(ItemInfo *itemInfo, XmlNodePtr node)
{
    const std::string gender = XML::getProperty(node, "gender", "unisex");
    const std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);

    const int race = XML::getProperty(node, "race", 0);
    if (gender == "male" || gender == "unisex")
        itemInfo->setSprite(filename, GENDER_MALE, race);
    if (gender == "female" || gender == "unisex")
        itemInfo->setSprite(filename, GENDER_FEMALE, race);
    if (gender == "other" || gender == "unisex")
        itemInfo->setSprite(filename, GENDER_OTHER, race);
}

void loadSoundRef(ItemInfo *itemInfo, XmlNodePtr node)
{
    std::string event = XML::getProperty(node, "event", "");
    std::string filename = reinterpret_cast<const char*>(
        node->xmlChildrenNode->content);

    if (event == "hit")
    {
        itemInfo->addSound(EQUIP_EVENT_HIT, filename);
    }
    else if (event == "strike")
    {
        itemInfo->addSound(EQUIP_EVENT_STRIKE, filename);
    }
    else
    {
        logger->log("ItemDB: Ignoring unknown sound event '%s'",
            event.c_str());
    }
}

void loadFloorSprite(SpriteDisplay *display, XmlNodePtr floorNode)
{
    for_each_xml_child_node(spriteNode, floorNode)
    {
        if (xmlNameEqual(spriteNode, "sprite"))
        {
            SpriteReference *currentSprite = new SpriteReference;
            currentSprite->sprite = reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content);
            currentSprite->variant
                = XML::getProperty(spriteNode, "variant", 0);
            display->sprites.push_back(currentSprite);
        }
        else if (xmlNameEqual(spriteNode, "particlefx"))
        {
            std::string particlefx = reinterpret_cast<const char*>(
                spriteNode->xmlChildrenNode->content);
            display->particles.push_back(particlefx);
        }
    }
}

void loadReplaceSprite(ItemInfo *itemInfo, XmlNodePtr replaceNode)
{
    std::string removeSprite = XML::getProperty(replaceNode, "sprite", "");
    int direction = parseDirectionName(XML::getProperty(
        replaceNode, "direction", "all"));

    itemInfo->setRemoveSprites();

    switch (direction)
    {
        case -1:
        {
            for (int f = 0; f < 9; f ++)
            {
                std::map<int, int> *mapList
                    = itemInfo->addReplaceSprite(
                    parseSpriteName(removeSprite), f);
                if (!mapList)
                    continue;
                for_each_xml_child_node(itemNode, replaceNode)
                {
                    if (xmlNameEqual(itemNode, "item"))
                    {
                        int from = XML::getProperty(itemNode, "from", 0);
                        int to = XML::getProperty(itemNode, "to", 1);

                        (*mapList)[from] = to;
                    }
                }
            }
            break;
        }
        case -2:
        {
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_DOWN);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_DOWNLEFT);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_DOWNRIGHT);

            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    int from = XML::getProperty(itemNode, "from", 0);
                    int to = XML::getProperty(itemNode, "to", 1);
                    std::map<int, int> *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), DIRECTION_DOWN);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), DIRECTION_DOWNLEFT);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), DIRECTION_DOWNRIGHT);
                    if (mapList)
                        (*mapList)[from] = to;
                }
            }
            break;
        }
        case -3:
        {
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_UP);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_UPLEFT);
            itemInfo->addReplaceSprite(parseSpriteName(
                removeSprite), DIRECTION_UPRIGHT);

            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    int from = XML::getProperty(itemNode, "from", 0);
                    int to = XML::getProperty(itemNode, "to", 1);
                    std::map<int, int> *mapList = itemInfo->addReplaceSprite(
                        parseSpriteName(removeSprite), DIRECTION_UP);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), DIRECTION_UPLEFT);
                    if (mapList)
                        (*mapList)[from] = to;

                    mapList = itemInfo->addReplaceSprite(parseSpriteName(
                        removeSprite), DIRECTION_UPRIGHT);
                    if (mapList)
                        (*mapList)[from] = to;
                }
            }
            break;
        }
        default:
        {
            std::map<int, int> *mapList = itemInfo->addReplaceSprite(
                parseSpriteName(removeSprite), direction);
            if (!mapList)
                return;
            for_each_xml_child_node(itemNode, replaceNode)
            {
                if (xmlNameEqual(itemNode, "item"))
                {
                    int from = XML::getProperty(itemNode, "from", 0);
                    int to = XML::getProperty(itemNode, "to", 1);
                    (*mapList)[from] = to;
                }
            }
            break;
        }
    }
}

void loadOrderSprite(ItemInfo *itemInfo, XmlNodePtr node, bool drawAfter)
{
    int sprite = parseSpriteName(XML::getProperty(node, "name", ""));
    int priority = XML::getProperty(node, "priority", 0);

    int direction = parseDirectionName(XML::getProperty(
        node, "direction", "all"));
    if (drawAfter)
        itemInfo->setDrawAfter(direction, sprite);
    else
        itemInfo->setDrawBefore(direction, sprite);
    itemInfo->setDrawPriority(direction, priority);
}
