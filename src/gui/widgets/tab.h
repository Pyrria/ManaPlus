/*
 *  The ManaPlus Client
 *  Copyright (C) 2008-2009  The Mana World Development Team
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

#ifndef TAB_H
#define TAB_H

#include <guichan/widgets/label.hpp>
#include <guichan/widgets/tab.hpp>
#include <guichan/widgetlistener.hpp>

class GraphicsVertexes;
class ImageRect;
class TabbedArea;

/**
 * A tab, the same as the Guichan tab in 0.8, but extended to allow
 * transparency.
 */
class Tab : public gcn::Tab, public gcn::WidgetListener
{
    public:
        Tab();
        ~Tab();

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draw the tabbed area.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Set the normal color fo the tab's text.
         */
        void setTabColor(const gcn::Color *color);

        /**
         * Set tab flashing state
         */
        void setFlash(int flash);

        int getFlash() const
        { return mFlash; }

        void widgetResized(const gcn::Event &event);

        void widgetMoved(const gcn::Event &event);

        void setLabelFont(gcn::Font *font);

        gcn::Label *getLabel() const
        { return mLabel; }

    protected:
        friend class TabbedArea;
        virtual void setCurrent()
        { }

    private:
        /** Load images if no other instances exist yet */
        void init();

        static ImageRect tabImg[4];    /**< Tab state graphics */
        static int mInstances;         /**< Number of tab instances */
        static float mAlpha;

        const gcn::Color *mTabColor;
        int mFlash;
        GraphicsVertexes *mVertexes;
        bool mRedraw;
        int mMode;
        gcn::Color mFlashColor;
        gcn::Color mPlayerFlashColor;
};

#endif
