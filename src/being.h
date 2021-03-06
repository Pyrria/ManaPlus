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

#ifndef BEING_H
#define BEING_H

#include "actorsprite.h"
#include "configlistener.h"
#include "equipment.h"
#include "map.h"
#include "position.h"
#include "vector.h"

#include <guichan/color.hpp>

#include <SDL_types.h>

#include <map>
#include <set>

#define FIRST_IGNORE_EMOTE 14
#define STATUS_EFFECTS 32

#define SPEECH_TIME 500
#define SPEECH_MIN_TIME 200
#define SPEECH_MAX_TIME 800

class AnimatedSprite;
class BeingCacheEntry;
class Being;
class BeingInfo;
class FlashText;
class Guild;
class Inventory;
class ItemInfo;
class Item;
class Particle;
class Party;
class SpeechBubble;
class Text;

struct Position;

extern volatile int cur_time;

enum Gender
{
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_UNSPECIFIED = 2,
    GENDER_OTHER = 3
};


class BeingEquipBackend : public Equipment::Backend
{
    public:
        BeingEquipBackend(Being *being);

        virtual ~BeingEquipBackend();

        Item *getEquipment(int index) const;

        void clear();

        void setEquipment(int index, Item *item);

    private:
        Item *mEquipment[EQUIPMENT_SIZE];
        Being *mBeing;
};

class Being : public ActorSprite, public ConfigListener
{
    public:
        friend class BeingEquipBackend;
        friend class LocalPlayer;

        enum FLAGS
        {
            FLAG_SHOP = 1,
            FLAG_AWAY = 2,
            FLAG_INACTIVE = 4,
            FLAG_GENDER_OTHER = 32,
            FLAG_GM = 64,
            FLAG_GENDER_MALE = 128,
            FLAG_SPECIAL = 128 + 64
        };

        /**
         * Action the being is currently performing
         * WARNING: Has to be in sync with the same enum in the Being class
         * of the server!
         */
        enum Action
        {
            STAND = 0,
            MOVE,
            ATTACK,
            SIT,
            DEAD,
            HURT,
            SPAWN
        };

        enum Speech
        {
            NO_SPEECH = 0,
            TEXT_OVERHEAD,
            NO_NAME_IN_BUBBLE,
            NAME_IN_BUBBLE
        };

        enum AttackType
        {
            HIT = 0x00,
            CRITICAL = 0x0a,
            MULTI = 0x08,
            REFLECT = 0x04,
            FLEE = 0x0b,
            SKILL = 0xff
        };

        enum Reachable
        {
            REACH_UNKNOWN = 0,
            REACH_YES = 1,
            REACH_NO = 2
        };

        /**
         * Directions, to be used as bitmask values
         */
        enum BeingDirection
        {
            DOWN = 1,
            LEFT = 2,
            UP = 4,
            RIGHT = 8
        };

        /**
         * Constructor.
         *
         * @param id      a unique being id
         * @param subtype partly determines the type of the being
         * @param map     the map the being is on
         */
        Being(int id, Type type, uint16_t subtype, Map *map);

        virtual ~Being();

        Type getType() const
        { return mType; }

        /**
         * Removes all path nodes from this being.
         */
        void clearPath();

        /**
         * Returns the time spent in the current action.
         */
        int getActionTime() const
        { return mActionTime; }

        /**
         * Set the current action time.
         * @see Ea::BeingHandler that set it to tick time.
         */
        void setActionTime(int actionTime)
        { mActionTime = actionTime; }

        /**
         * Makes this being take the next tile of its path.
         * TODO: Used by eAthena only?
         */
        virtual void nextTile();

        /**
         * Get the current X pixel offset.
         * TODO: Used by eAthena only?
         */
        int getXOffset() const
        { return getOffset(LEFT, RIGHT); }

        /**
         * Get the current Y pixel offset.
         * TODO: Used by eAthena only?
         */
        int getYOffset() const
        { return getOffset(UP, DOWN); }

        /**
         * Creates a path for the being from current position to ex and ey
         */
        void setDestination(int ex, int ey);

        /**
         * Returns the destination for this being.
         */
        const Vector &getDestination() const
        { return mDest; }

        /**
         * Returns the tile x coord
         */
        int getTileX() const
        { return mX; }

        /**
         * Returns the tile y coord
         */
        int getTileY() const
        { return mY; }

        /**
         * Sets the tile x and y coord
         */
        void setTileCoords(int x, int y)
        { mX = x; mY = y; }

        /**
         * Puts a "speech balloon" above this being for the specified amount
         * of time.
         *
         * @param text The text that should appear.
         * @param time The amount of time the text should stay in milliseconds.
         */
        void setSpeech(const std::string &text, int time = 0);

        /**
         * Puts a damage bubble above this being.
         *
         * @param attacker the attacking being
         * @param damage the amount of damage recieved (0 means miss)
         * @param type the attack type
         * @param id skill id
         */
        void takeDamage(Being *attacker, int damage,
                        AttackType type, int id = 0);

        /**
         * Handles an attack of another being by this being.
         *
         * @param victim the victim being
         * @param damage the amount of damage dealt (0 means miss)
         * @param type the attack type
         */
        virtual void handleAttack(Being *victim, int damage, AttackType type);

        virtual void handleSkill(Being *victim, int damage, int skillId);

        const ItemInfo *getEquippedWeapon() const
        { return mEquippedWeapon; }

        /**
         * Returns the name of the being.
         */
        const std::string &getName() const
        { return mName; }

        /**
         * Sets the name for the being.
         *
         * @param name The name that should appear.
         */
        void setName(const std::string &name);

        bool getShowName() const
        { return mShowName; }

        void setShowName(bool doShowName);

        /**
         * Sets the name of the party the being is in. Shown in BeingPopup.
         */
        void setPartyName(const std::string &name)
        { mPartyName = name; }

        const std::string &getPartyName() const
        { return mPartyName; }

        const std::string &getGuildName() const
        { return mGuildName; }

        /**
         * Sets the name of the primary guild the being is in. Shown in
         * BeingPopup (eventually).
         */
        void setGuildName(const std::string &name);

        void setGuildPos(const std::string &pos);

        /**
         * Adds a guild to the being.
         */
        void addGuild(Guild *guild);

        /**
         * Removers a guild from the being.
         */
        void removeGuild(int id);

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        Guild *getGuild(const std::string &guildName) const;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        Guild *getGuild(int id) const;

        /**
         * Returns a pointer to the specified guild that the being is in.
         */
        Guild *getGuild() const;

        /**
         * Returns all guilds the being is in.
         */
        const std::map<int, Guild*> &getGuilds() const
        { return mGuilds; }

        /**
         * Removes all guilds the being is in.
         */
        void clearGuilds();

        /**
         * Get number of guilds the being belongs to.
         */
        short getNumberOfGuilds() const
        { return static_cast<short>(mGuilds.size()); }

        bool isInParty() const
        { return mParty; }

        void setParty(Party *party);

        void setGuild(Guild *guild);

        void updateGuild();

        Party *getParty() const
        { return mParty; }

        int getSpritesCount()
        { return static_cast<int>(size()); }

        /**
         * Sets visible equipments for this being.
         */
        void setSprite(unsigned int slot, int id,
                       std::string color = "",
                       unsigned char colorId = 1, bool isWeapon = false,
                       bool isTempSprite = false);

        void setSpriteID(unsigned int slot, int id);

        void setSpriteColor(unsigned int slot, const std::string &color = "");

        /**
         * Get the number of hairstyles implemented
         */
        static int getNumOfHairstyles()
        { return mNumberOfHairstyles; }

        /**
         * Get the number of layers used to draw the being
         */
        int getNumberOfLayers() const;

        /**
         * Performs being logic.
         */
        virtual void logic();

        /**
         * Draws the speech text above the being.
         */
        void drawSpeech(int offsetX, int offsetY);

        /**
         * Draws the emotion picture above the being.
         */
        void drawEmotion(Graphics *graphics, int offsetX, int offsetY);

        uint16_t getSubType() const
        { return mSubType; }

         /**
          * Set Being's subtype (mostly for view for monsters and NPCs)
          */
        void setSubtype(uint16_t subtype);

        const BeingInfo *getInfo() const
        { return mInfo; }

        TargetCursorSize getTargetCursorSize() const;

        int getTargetOffsetX() const;

        int getTargetOffsetY() const;

        /**
         * Gets the way the object is blocked by other objects.
         */
        virtual unsigned char getWalkMask() const;

        /**
         * Gets the way the monster blocks pathfinding for other objects
         */
        Map::BlockType getBlockType() const;

        /**
         * Sets the walk speed.
         * in pixels per second for eAthena,
         * in tiles per second for Manaserv.
         */
        void setWalkSpeed(Vector speed)
        { mWalkSpeed = speed; }

        /**
         * Gets the walk speed.
         * in pixels per second for eAthena,
         * in tiles per second for Manaserv (0.1 precision).
         */
        Vector getWalkSpeed() const
        { return mWalkSpeed; }

        /**
         * Sets the attack speed.
         * @todo In what unit?
         */
        void setAttackSpeed(int speed)
        { mAttackSpeed = speed; }

        /**
         * Gets the attack speed.
         * @todo In what unit?
         */
        int getAttackSpeed() const
        { return mAttackSpeed; }

        /**
         * Sets the current action.
         */
        virtual void setAction(Action action, int attackType = 0);

        /**
         * Get the being's action currently performed.
         */
        Action getCurrentAction() const
        { return mAction; }

        /**
         * Returns whether this being is still alive.
         */
        bool isAlive() const
        { return mAction != DEAD; }

        /**
         * Returns the current direction.
         */
        uint8_t getDirection() const
        { return mDirection; }

        /**
         * Sets the current direction.
         */
        virtual void setDirection(uint8_t direction);

        virtual void setDirectionDelayed(uint8_t direction)
        { mDirectionDelayed = direction; }

        uint8_t getDirectionDelayed() const
        { return mDirectionDelayed; }

        /**
         * Returns the direction the being is facing.
         */
        SpriteDirection getSpriteDirection() const
        { return SpriteDirection(mSpriteDirection); }

        void setPosition(const Vector &pos);

        /**
         * Overloaded method provided for convenience.
         *
         * @see setPosition(const Vector &pos)
         */
        inline void setPosition(float x, float y, float z = 0.0f)
        { setPosition(Vector(x, y, z)); }

        /**
         * Returns the horizontal size of the current base sprite of the being.
         */
        virtual int getWidth() const;

        /**
         * Returns the vertical size of the current base sprite of the being.
         */
        virtual int getHeight() const;

        /**
         * Returns the being's pixel radius used to detect collisions.
         */
        virtual int getCollisionRadius() const;

        /**
         * Shoots a missile particle from this being, to target being
         */
        void fireMissile(Being *target, const std::string &particle);

        /**
         * Returns the path this being is following. An empty path is returned
         * when this being isn't following any path currently.
         */
        const Path &getPath() const
        { return mPath; }

        int getDistance() const
        { return mDistance; }

        void setDistance(int n)
        { mDistance = n; }

        /**
         * Set the Emoticon type and time displayed above
         * the being.
         */
        void setEmote(uint8_t emotion, int emote_time);

        void setState(uint8_t state);

        /**
         * Get the current Emoticon type displayed above
         * the being.
         */
        int getEmotion() const
        { return mEmotion; }

        virtual void drawSprites(Graphics* graphics, int posX, int posY) const;

        virtual void drawSpritesSDL(Graphics* graphics,
                                    int posX, int posY) const;

        void drawHpBar(Graphics *graphics, int x, int y,
                       int maxHP, int hp, int damage, int color1, int color2,
                       int width, int height) const;

        static void load();

        virtual void optionChanged(const std::string &value);

        void flashName(int time);

        int getDamageTaken() const
        { return mDamageTaken; }

        void setDamageTaken(int damage)
        { mDamageTaken = damage; }

        void updateName();

        void setLevel(int n)
        { mLevel = n; }

        virtual int getLevel() const
        { return mLevel; }

        void setIsReachable(int n)
        { mIsReachable = n; }

        int isReachable() const
        { return mIsReachable; }

        static void reReadConfig();

        static BeingCacheEntry* getCacheEntry(int id);

        void addToCache();

        bool updateFromCache();

        /**
         * Sets the gender of this being.
         */
        virtual void setGender(Gender gender);

        Gender getGender() const
        { return mGender; }

        /**
         * Return sprite sit action for current environment.
         */
        std::string getSitAction() const;

        /**
         * Whether or not this player is a GM.
         */
        bool isGM() const
        { return mIsGM; }

        /**
         * Triggers whether or not to show the name as a GM name.
         */
        void setGM(bool gm);

        bool canTalk();

        void talkTo();

        static bool isTalking();

        bool draw(Graphics *graphics, int offsetX, int offsetY) const;

        bool drawSpriteAt(Graphics *graphics, int x, int y) const;

        void setMoveTime()
        { mMoveTime = cur_time; }

        void setAttackTime()
        { mAttackTime = cur_time; }

        void setTalkTime()
        { mTalkTime = cur_time; }

        void setTestTime()
        { mTestTime = cur_time; }

        void setOtherTime()
        { mOtherTime = cur_time; }

        unsigned int getMoveTime() const
        { return mMoveTime; }

        unsigned int getAttackTime() const
        { return mAttackTime; }

        unsigned int getTalkTime() const
        { return mTalkTime; }

        unsigned int getTestTime() const
        { return mTestTime; }

        unsigned int getOtherTime() const
        { return mOtherTime; }

        void resetCounters();

        virtual void updateColors();

        void setEnemy(bool n)
        { mEnemy = n; }

        const std::string &getIp() const
        { return mIp; }

        void setIp(std::string ip)
        { mIp = ip; }

        unsigned int getPvpRank() const
        { return mPvpRank; }

        void setPvpRank(unsigned int rank)
        { mPvpRank = rank; }

        void setHP(int n);

        void setMaxHP(int hp);

        int getHP() const
        { return mHP; }

        uint8_t calcDirection(int dstX, int dstY) const;

        uint8_t calcDirection() const;

        void setAttackDelay(int n)
        { mAttackDelay = n; }

        int getAttackDelay() const
        { return mAttackDelay; }

        int getMinHit() const
        { return mMinHit; }

        void setMinHit(int n)
        { mMinHit = n; }

        int getMaxHit() const
        { return mMaxHit; }

        void setMaxHit(int n)
        { mMaxHit = n; }

        int getCriticalHit() const
        { return mCriticalHit; }

        void setCriticalHit(int n)
        { mCriticalHit = n; }

        void updateHit(int amount);

        Equipment *getEquipment();

        void undressItemById(int id);

        int getGoodStatus() const
        { return mGoodStatus; }

        void setGoodStatus(int n)
        { mGoodStatus = n; }

        std::string getGenderSign() const;

        std::string getGenderSignWithSpace() const;

        void updateComment();

        const std::string getComment() const
        { return mComment; }

        void setComment(std::string n)
        { mComment = n; }

        static void clearCache();

        static std::string loadComment(const std::string &name, int type);

        static void saveComment(const std::string &name,
                                const std::string &comment, int type);

        bool isAdvanced() const
        { return mAdvanced; }

        void setAdvanced(bool n)
        { mAdvanced = n; addToCache(); }

        bool isShopEnabled() const
        { return mShop; }

        void enableShop(bool b)
        { mShop = b; }

        /**
         * Sets the attack range.
         */
        void setAttackRange(int range)
        { mAttackRange = range; }

        void attack(Being *target = nullptr, bool keep = false,
                    bool dontChangeEquipment = false);

        void attack2(Being *target = nullptr, bool keep = false,
                     bool dontChangeEquipment = false);

        void updatePercentHP();

        void setRaceName(std::string name)
        { mRaceName = name; }

        std::string getRaceName()
        { return mRaceName; }

        int getSpriteID(int slot);

        static int genderToInt(Gender sex);

        static Gender intToGender(int sex);

    protected:
        /**
         * Sets the new path for this being.
         */
        void setPath(const Path &path);

        /**
         * Updates name's location.
         */
        virtual void updateCoords();

        void recalcSpritesOrder();

        void showName();

        BeingInfo *mInfo;

        int mActionTime;      /**< Time spent in current action */

        int mEmotion;         /**< Currently showing emotion */
        int mEmotionTime;     /**< Time until emotion disappears */
        /** Time until the last speech sentence disappears */
        int mSpeechTime;

        int mAttackType;
        int mAttackSpeed;     /**< Attack speed */

        Action mAction;       /**< Action the being is performing */
        uint16_t mSubType;      /**< Subtype (graphical view, basically) */

        uint8_t mDirection;               /**< Facing direction */
        uint8_t mDirectionDelayed;        /**< Facing direction */

        uint8_t mSpriteDirection;         /**< Facing direction */
        std::string mName;              /**< Name of character */
        std::string mRaceName;
        std::string mPartyName;
        std::string mGuildName;

        /**
         * Holds a text object when the being displays it's name, 0 otherwise
         */
        FlashText *mDispName;
        const gcn::Color *mNameColor;
        bool mShowName;

        /** Engine-related infos about weapon. */
        const ItemInfo *mEquippedWeapon;

        static int mNumberOfHairstyles; /** Number of hair styles in use */

        Path mPath;
        std::string mSpeech;
        Text *mText;
        const gcn::Color *mTextColor;

        int mLevel;
        Vector mDest;  /**< destination coordinates. */

        std::vector<int> mSpriteIDs;
        StringVect mSpriteColors;
        std::vector<int> mSpriteColorsIds;
        Gender mGender;

        // Character guild information
        std::map<int, Guild*> mGuilds;
        Party *mParty;

        bool mIsGM;
        int mAttackRange;

    private:

        /**
         * Calculates the offset in the given directions.
         * If walking in direction 'neg' the value is negated.
         * TODO: Used by eAthena only?
         */
        int getOffset(char pos, char neg) const;

        int searchSlotValue(std::vector<int> &slotRemap, int val);

        void searchSlotValueItr(std::vector<int>::iterator &it, int &idx,
                                std::vector<int> &slotRemap, int val);

        const Type mType;

        /** Speech Bubble components */
        SpeechBubble *mSpeechBubble;

        /**
         * Walk speed for x and y movement values.
         * In pixels per second for eAthena,
         * In pixels per ticks for Manaserv.
         * @see MILLISECONDS_IN_A_TICK
         */
        Vector mWalkSpeed;

        int mX, mY;   /**< Position in tile */

        int mDamageTaken;
        int mHP;
        int mMaxHP;
        int mDistance;
        int mIsReachable; /**< 0 - unknown, 1 - reachable, 2 - not reachable*/
        int mGoodStatus;

        static int mUpdateConfigTime;
        static unsigned int mConfLineLim;
        static int mSpeechType;
        static bool mHighlightMapPortals;
        static bool mHighlightMonsterAttackRange;
        static bool mLowTraffic;
        static bool mDrawHotKeys;
        static bool mShowBattleEvents;
        static bool mShowMobHP;
        static bool mShowOwnHP;
        static bool mShowGender;
        static bool mShowLevel;
        static bool mShowPlayersStatus;
        static bool mEnableReorderSprites;
        static bool mHideErased;

        unsigned int mMoveTime;
        unsigned int mAttackTime;
        unsigned int mTalkTime;
        unsigned int mTestTime;
        unsigned int mOtherTime;
        bool mErased;
        bool mEnemy;
        std::string mIp;
        int mAttackDelay;
        int mMinHit;
        int mMaxHit;
        int mCriticalHit;
        unsigned int mPvpRank;
        int *mSpriteRemap;
        int *mSpriteHide;
        std::string mComment;
        bool mGotComment;
        bool mAdvanced;
        bool mShop;
        bool mAway;
        bool mInactive;
        unsigned mNumber;
};

extern std::list<BeingCacheEntry*> beingInfoCache;

#endif
