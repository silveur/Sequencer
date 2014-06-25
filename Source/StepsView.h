/*
  ==============================================================================

    StepsView.h
    Created: 24 Apr 2014 10:50:15am
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef STEPSVIEW_H_INCLUDED
#define STEPSVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

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
		if (theState) g.setColour (Colour::fromRGB(30, 31, 84));
		else g.setColour (Colour::fromRGB(236, 235, 226));
        g.fillEllipse (getWidth()*0.4f, getHeight()*0.1f, getHeight()*0.8f, getHeight()*0.8f);
	}

private:
	bool theState;
};

#endif  // STEPSVIEW_H_INCLUDED
