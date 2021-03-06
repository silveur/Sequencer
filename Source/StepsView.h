/* =====================================================================
 
 * Steps - Midi sequencer
 * Copyright (C) 2014  Silvere Letellier for Nummer Music
 * Contact: <silvere.letellier@gmail.com>
 
 -----------------------------------------------------------------------
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 ===================================================================== */

#ifndef STEPSVIEW_H_INCLUDED
#define STEPSVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "LookAndFeel.h"

class StepView: public Component
{
public:
	StepView()
	{
		setInterceptsMouseClicks(false, true);
	}
	
	~StepView() {}
	
	void update(bool state)
	{
		theState = state;
		repaint();
	}

	void paint(Graphics& g)
	{
		if (theState) g.setColour (SeqLookAndFeel::getColour(COLOUR_1));
		else g.setColour (SeqLookAndFeel::getColour(COLOUR_4));
        g.fillEllipse (getWidth()*0.45f, getHeight()*0.2f, getHeight()*0.6f, getHeight()*0.6f);
	}

private:
	bool theState;
};

#endif  // STEPSVIEW_H_INCLUDED
