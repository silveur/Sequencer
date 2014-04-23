/*
  ==============================================================================

	Sequencer.cpp
	Created: 22 Dec 2013 11:52:14pm
	Author:  silvere letellier

  ==============================================================================
*/

#include "Sequencer.h"
#include "PluginProcessor.h"

Sequencer::Sequencer(ValueTree& sequencerTree)
{
	thePosition = 0;
	theRootNote = 48;
	thePpqCount = 0;
	theMidiCore = new MidiCore();
    theSequencerTree = sequencerTree;
	for (int i=0; i<16; i++)
	{
        ValueTree stepTree = sequencerTree.getChild(i);
        if (!stepTree.isValid()) stepTree = ValueTree("Step" + String(i));
		theStepArray.add(new Step(stepTree));
		sequencerTree.addChild(stepTree, -1, nullptr);
	}

    String str = theSequencerTree.getProperty("MidiOutput");
    theMidiCore->openMidiOutput(str);
	theLength = theSequencerTree.getProperty("Length");
	theRootNote = theSequencerTree.getProperty("RootNote");
	theRootOctave = theSequencerTree.getProperty("RootOctave");
	theShuffle = theSequencerTree.getProperty("Shuffle");
	theRange = theSequencerTree.getProperty("Range");
	theSequencerTree.addListener(this);
}

Sequencer::~Sequencer()
{
}

void Sequencer::start()
{
	thePpqCount = -1;
	thePosition = -1;
	isIdle = false;
	waitForShuffle = false;
}

void Sequencer::stop()
{
	thePpqCount = -1;
	thePosition = -1;
	theMidiCore->killNotes();
	isIdle = true;
	waitForShuffle = false;
}

void Sequencer::carryOn()
{
	thePpqCount = -1;
	thePosition = -1;
	isIdle = false;
	waitForShuffle = false;
}

void Sequencer::triggerStep()
{
	thePosition = (thePosition+1) % theLength;
	if (theStepArray[thePosition]->theState)
	{
		Step* step = theStepArray[thePosition];
		MidiMessage onMsg = MidiMessage::noteOn(1, (24 + (theRange*step->thePitch) + theRootNote) + (12*theRootOctave), (uint8)step->theVelocity);
		MidiMessage offMsg = MidiMessage::noteOff(1, (24 + (theRange*step->thePitch) + theRootNote) + (12*theRootOctave), (uint8)step->theVelocity);
		theMidiCore->outputMidi(onMsg);
		theMidiCore->outputMidi(offMsg, 40);
	}
	theSequencerTree.setProperty("Position", thePosition, nullptr);
}

void Sequencer::handleIncomingMidiMessage (const MidiMessage& message)
{
	if (message.isMidiClock() && !isIdle)
	{
		thePpqCount = (thePpqCount+1) % 6;
		if( waitForShuffle && (thePpqCount == theShuffle))
		{
			triggerStep();
			waitForShuffle = false;
		}

		else if (!thePpqCount)
		{
			if (!((thePosition +1) %2) || !theShuffle)
			{
				triggerStep();
			}
			else if((thePosition + 1) %2)
			{
				waitForShuffle = true;
			}
		}
	}
	
	else if(message.isSongPositionPointer())
	{
		
	}
	else if (message.isMidiStart())
	{
		start();
	}
	else if (message.isMidiStop())
	{
		stop();
	}
	else if (message.isMidiContinue())
	{
		carryOn();
	}
}

void Sequencer::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "MidiOutput")
	{
		String midiOut = tree.getProperty(property);
		theMidiCore->openMidiOutput(midiOut);
	}
	else if(String(property) == "Length")
	{
		theLength = tree.getProperty(property);
	}
	else if(String(property) == "RootOctave")
	{
		theRootOctave = tree.getProperty(property);
	}
	else if(String(property) == "RootNote")
	{
		theRootNote = tree.getProperty(property);
	}
	else if(String(property) == "Shuffle")
	{
		theShuffle = tree.getProperty(property);
	}
	else if(String(property) == "Range")
	{
		theRange = tree.getProperty(property);
	}
}
