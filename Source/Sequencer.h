/*
  ==============================================================================

    Sequencer.h
    Created: 22 Dec 2013 11:52:14pm
    Author:  silvere letellier

  ==============================================================================
*/

#ifndef SEQUENCER_H_INCLUDED
#define SEQUENCER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Step.h"
class SequencerAudioProcessor;
class MidiCore;
class NoteOnClient;
class NoteOffClient;
class NoteManager;
class Sequencer: public Thread
{
public:
	Sequencer(SequencerAudioProcessor* processor, MidiCore* midiCore);
	~Sequencer();
	
	void setPosition(AudioPlayHead::CurrentPositionInfo& info);
	void stop();
	void start();
	void newStep();
	void repositionSequencer();
	MidiCore* theMidiCore;
	SequencerAudioProcessor* theProcessor;
	NoteOnClient* theNoteOnClient;
	NoteOffClient* theNoteOffClient;
	double theTempo;
	double thePPQPosition;
	int thePosition;
	static int theStepTime;
	int theSyncTime;
	int theRootNote;
//	bool wait;
	void run();
	NoteManager* theNoteManager;
};

class NoteManager: public TimeSliceThread
{
public:
	NoteManager(): TimeSliceThread("NoteManager")
	{startThread();}
	~NoteManager()
	{stopThread(200);}
};

class NoteOnClient: public TimeSliceClient
{
public:
	NoteOnClient(){}
	~NoteOnClient(){}
	int useTimeSlice();
	Sequencer* theSequencer;
};

class NoteOffClient: public TimeSliceClient
{
public:
	NoteOffClient(){}
	~NoteOffClient(){}
	int useTimeSlice();
	Sequencer* theSequencer;
	int nextNoteOff;
};

#endif  // SEQUENCER_H_INCLUDED