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

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <guichan/widget.hpp>

#include <string>

#include <guichan/widgetlistener.hpp>

class Graphics;
class GraphicsVertexes;
class ImageRect;

/**
 * A progress bar.
 *
 * \ingroup GUI
 */
class ProgressBar : public gcn::Widget, public gcn::WidgetListener
{
    public:
        /**
         * Constructor, initializes the progress with the given value.
         */
        ProgressBar(float progress = 0.0f,
                    int width = 40, int height = 7,
                    int color = -1);

        ~ProgressBar();

        /**
         * Performs progress bar logic (fading colors)
         */
        void logic();

        /**
         * Update the alpha value to the graphic components.
         */
        void updateAlpha();

        /**
         * Draws the progress bar.
         */
        void draw(gcn::Graphics *graphics);

        /**
         * Sets the current progress.
         */
        void setProgress(float progress);

        /**
         * Returns the current progress.
         */
        float getProgress() const
        { return mProgress; }

        /**
         * Change the ProgressPalette for this ProgressBar to follow or -1 to
         * disable this and manage color manually.
         */
        void setProgressPalette(int progressPalette);

        /**
         * Change the color of the progress bar.
         */
        void setColor(const gcn::Color &color);

        /**
         * Returns the color of the progress bar.
         */
        const gcn::Color &getColor() const
        { return mColor; }

        /**
         * Sets the text shown on the progress bar.
         */
        void setText(const std::string &str)
        { mText = str; }

        /**
         * Returns the text shown on the progress bar.
         */
        const std::string &text() const
        { return mText; }

        /**
         * Set whether the progress is moved smoothly.
         */
        void setSmoothProgress(bool smoothProgress)
        { mSmoothProgress = smoothProgress; }

        /**
         * Set whether the color changing is made smoothly.
         */
        void setSmoothColorChange(bool smoothColorChange)
        { mSmoothColorChange = smoothColorChange; }

        /**
         * Renders a progressbar with the given properties.
         */
        static void render(Graphics *graphics, const gcn::Rectangle &area,
                           const gcn::Color &color, float progress,
                           const std::string &text,
                           GraphicsVertexes *vert, bool *redraw);

        /**
         * Renders a progressbar with the given properties.
         */
        static void render(Graphics *graphics, const gcn::Rectangle &area,
                           const gcn::Color &color, float progress,
                           const std::string &text = "");

        void widgetResized(const gcn::Event &event);

        void widgetMoved(const gcn::Event &event);

    private:
        float mProgress, mProgressToGo;
        bool mSmoothProgress;

        int mProgressPalette; /** < Entry in ProgressPalette or -1 for none. */
        gcn::Color mColor;
        gcn::Color mColorToGo;
        bool mSmoothColorChange;

        std::string mText;
        GraphicsVertexes *mVertexes;
        bool mRedraw;

        static ImageRect mBorder;
        static int mInstances;
        static float mAlpha;

        static const gcn::Color TEXT_COLOR;
};

#endif
