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

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "listener.h"

#include "configlistener.h"

#include "gui/widgets/window.h"

#include "utils/stringvector.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/widget.hpp>
#include <guichan/widgetlistener.hpp>

#include <list>
#include <map>
#include <set>

class BrowserBox;
class ChatTab;
class Channel;
class ChatInput;
class ColorListModel;
class ScrollArea;
class TabbedArea;
class ItemLinkHandler;
class Tab;
class WhisperTab;

namespace gcn
{
    class DropDown;
}

#define DEFAULT_CHAT_WINDOW_SCROLL 7 // 1 means `1/8th of the window size'.

enum Own
{
    BY_GM = 0,
    BY_PLAYER,
    BY_OTHER,
    BY_SERVER,
    BY_CHANNEL,
    ACT_WHISPER,      // getting whispered at
    ACT_IS,           // equivalent to "/me" on IRC
    BY_LOGGER,
    BY_UNKNOWN = -1
};

/** One item in the chat log */
struct CHATLOG
{
    std::string nick;
    std::string text;
    Own own;
};

/**
 * The chat window.
 *
 * \ingroup Interface
 */
class ChatWindow : public Window,
                   public gcn::ActionListener,
                   public gcn::KeyListener,
                   public Listener,
                   public ConfigListener
{
    public:
        /**
         * Constructor.
         */
        ChatWindow();

        /**
         * Destructor: used to write back values to the config file
         */
        ~ChatWindow();

        /**
         * Reset the chat window to default positions.
         */
        void resetToDefaultSize();

        /**
         * Gets the focused tab.
         */
        ChatTab *getFocused() const;

        /**
         * Clear the given tab.
         */
        void clearTab(ChatTab *tab);

        /**
         * Clear the current tab.
         */
        void clearTab();

        /**
         * Switch to the previous tab in order
         */
        void prevTab();

        /**
         * Switch to the next tab in order
         */
        void nextTab();

        /**
         * Close current chat tab
         */
        void closeTab();

        /**
         * Switch to the default tab
         */
        void defaultTab();

        /**
         * Performs action.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Request focus for typing chat message.
         *
         * \returns true if the input was shown
         *          false otherwise
         */
        bool requestChatFocus();

        /**
         * Checks whether ChatWindow is Focused or not.
         */
        bool isInputFocused() const;

        /**
         * Passes the text to the current tab as input
         *
         * @param msg  The message text which is to be sent.
         */
        void chatInput(const std::string &msg);

        /**
         * Passes the text to the local chat tab as input
         *
         * @param msg  The message text which is to be sent.
         */
        void localChatInput(const std::string &msg);

        /** Called when key is pressed */
        void keyPressed(gcn::KeyEvent &event);

        /** Set the chat input as the given text. */
        void setInputText(const std::string &text);

        /** Add the given text to the chat input. */
        void addInputText(const std::string &text, bool space = true);

        /** Called to add item to chat */
        void addItemText(const std::string &item);

        /** Override to reset mTmpVisible */
        void setVisible(bool visible);

        /**
         * Handles mouse when dragged.
         */
        void mouseDragged(gcn::MouseEvent &event);

        /**
         * Handles mouse when pressed.
         */
        void mousePressed(gcn::MouseEvent &event);

        void processEvent(Channels channel, const DepricatedEvent &event);

        /**
         * Scrolls the chat window
         *
         * @param amount direction and amount to scroll.  Negative numbers scroll
         * up, positive numbers scroll down.  The absolute amount indicates the
         * amount of 1/8ths of chat window real estate that should be scrolled.
         */
        void scroll(int amount);

        /**
         * Sets the file being recorded to
         *
         * @param msg The file to write out to. If null, then stop recording.
         */
        void setRecordingFile(const std::string &msg);

        bool getReturnTogglesChat() const
        { return mReturnToggles; }

        void setReturnTogglesChat(bool toggles)
        { mReturnToggles = toggles; }

        void doPresent();

        void addWhisper(const std::string &nick, const std::string &mes,
                        Own own = BY_OTHER);

        ChatTab *addWhisperTab(const std::string &nick, bool switchTo = false);

        ChatTab *getWhisperTab(const std::string &nick) const;

        void removeAllWhispers();

        void ignoreAllWhispers();

        void resortChatLog(std::string line, Own own = BY_UNKNOWN,
                           bool ignoreRecord = false,
                           bool tryRemoveColors = true);

        void battleChatLog(std::string line, Own own = BY_UNKNOWN,
                           bool ignoreRecord = false,
                           bool tryRemoveColors = true);

        void updateOnline(std::set<std::string> &onlinePlayers);

        void loadState();

        void saveState();

        void loadCustomList();

        void loadGMCommands();

        std::string doReplace(const std::string &msg);

        void adjustTabSize();

        void addToAwayLog(std::string line);

        void displayAwayLog();

        void clearAwayLog()
        { mAwayLog.clear(); }

        void parseHighlights();

        bool findHighlight(std::string &str);

        void copyToClipboard(int x, int y);

        void optionChanged(const std::string &name);

        void mouseEntered(gcn::MouseEvent& mouseEvent);

        void mouseMoved(gcn::MouseEvent &event);

        void mouseExited(gcn::MouseEvent& mouseEvent A_UNUSED);

        void draw(gcn::Graphics* graphics);

        void updateVisibility();

        void unHideWindow();

    protected:
        friend class ChatTab;
        friend class WhisperTab;
        friend class PopupMenu;

        typedef std::list<std::string> History;

        /** Remove the given tab from the window */
        void removeTab(ChatTab *tab);

        /** Add the tab to the window */
        void addTab(ChatTab *tab);

        void removeWhisper(const std::string &nick);

        void autoComplete();

        std::string addColors(std::string &msg);

        std::string autoCompleteHistory(std::string partName);

        std::string autoComplete(std::string partName, History *words);

        std::string autoComplete(StringVect &names,
                                 std::string partName) const;

        /** Used for showing item popup on clicking links **/
        ItemLinkHandler *mItemLinkHandler;

        /** Input box for typing chat messages. */
        ChatInput *mChatInput;

        void widgetResized(const gcn::Event &event);

        void initTradeFilter();

        int mRainbowColor;

    private:
        void fillCommands();

        bool mTmpVisible;

        /** Tabbed area for holding each channel. */
        TabbedArea *mChatTabs;

        typedef std::map<const std::string, ChatTab*> TabMap;
        /** Manage whisper tabs */
        TabMap mWhispers;

        typedef History::iterator HistoryIterator;
        History mHistory;           /**< Command history. */
        HistoryIterator mCurHist;   /**< History iterator. */

        typedef std::list<std::string> Commands;
        typedef Commands::iterator CommandsIterator;
        History mCommands;           /**< Command list. */
        History mCustomWords;

        bool mReturnToggles; /**< Marks whether <Return> toggles the chat log
                                or not */

        StringVect mTradeFilter;

        gcn::DropDown *mColorPicker;
        ColorListModel *mColorListModel;
        int mChatColor;
        unsigned int mChatHistoryIndex;
        std::list<std::string> mAwayLog;
        StringVect mHighlights;
        bool mGMLoaded;
        bool mHaveMouse;
        bool mAutoHide;
        bool mShowBattleEvents;
};

extern ChatWindow *chatWindow;

#endif
