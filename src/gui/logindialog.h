/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
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

#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "gui/widgets/window.h"

#include <guichan/actionlistener.hpp>
#include <guichan/keylistener.hpp>
#include <guichan/listmodel.hpp>

#include <string>
#include <vector>

class LoginData;
class UpdateTypeModel;

/**
 * The login dialog.
 *
 * \ingroup Interface
 */
class LoginDialog : public Window, public gcn::ActionListener,
                    public gcn::KeyListener
{
    public:
        /**
         * Constructor
         *
         * @see Window::Window
         */
        LoginDialog(LoginData *loginData, std::string serverName,
                    std::string *updateHost);

        ~LoginDialog();

        /**
         * Called when receiving actions from the widgets.
         */
        void action(const gcn::ActionEvent &event);

        /**
         * Called when a key is pressed in one of the text fields.
         */
        void keyPressed(gcn::KeyEvent &keyEvent);

        static std::string savedPasswordKey;
        static std::string savedPassword;

    private:
        /**
         * Returns whether submit can be enabled. This is true in the login
         * state, when all necessary fields have some text.
         */
        bool canSubmit() const;

        gcn::TextField *mUserField;
        gcn::TextField *mPassField;
        gcn::CheckBox *mKeepCheck;
        gcn::Label *mUpdateTypeLabel;
        gcn::DropDown *mUpdateTypeDropDown;
        gcn::Button *mServerButton;
        gcn::Button *mLoginButton;
        gcn::Button *mRegisterButton;
        gcn::CheckBox *mCustomUpdateHost;
        gcn::TextField *mUpdateHostText;

        LoginData *mLoginData;
        std::string *mUpdateHost;
        UpdateTypeModel *mUpdateTypeModel;
};

#endif