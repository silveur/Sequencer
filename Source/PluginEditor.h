/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef __PLUGINEDITOR_H_14E432C7__
#define __PLUGINEDITOR_H_14E432C7__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


#define SEQUENCER_SIZE_X   810
#define SEQUENCER_SIZE_Y   250


//==============================================================================
/**
 */
class PluginEditor  : public AudioProcessorEditor, Timer, SliderListener, ButtonListener, Random,ComboBoxListener,KeyListener
{
public:
    PluginEditor (SequencerAuAudioProcessor* ownerFilter);
    ~PluginEditor();
    
    void timerCallback();
    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider*);
    AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;
    
    Label infoLabel, gainLabel, delayLabel;
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    
    SequencerAuAudioProcessor* getProcessor() const
    {
        return static_cast <SequencerAuAudioProcessor*> (getAudioProcessor());
    }
    bool currentPlaying;
    bool previousPlaying;
    int sampleCountEditor;
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent);
    void buttonClicked(Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged);
    void fillRootKeyBox(ComboBox* theBox);
    void fillTheOctaveBox(ComboBox* theBox);
    
    double getBPMSliderValue();
    int getVelocity(int index);
    float getShuffle();
    int getRange();
    int getStepValue(int index);
    int getTransposeValue();
    int getSequencerLength();
    int getKey();
    void setStepValue(int index, int value);
    void setKey(int value);
    void setTranspose(int value);
    void setShuffle(double value);
    void setSequencerLength(int value);
    void setRange(double value);
    
    int sequencerIndex;
    
    int midiOutputIndex;
    
    
private:
    KeyListener* theKeyListener;
    
    int XPosition,YPosition,range;
    
    OwnedArray<Slider> p_theSliders;
    
    ComboBox* theRootKeyBox;
    TextButton* theRandomButton;
    Random* theRandomNumber;    
    ScopedPointer<ComboBox> theOctaveBox;
    
    ScopedPointer<TextButton> theResetButton;
    
    Random* theRandomBool;
    
    ScopedPointer<TextButton> theVelocityRandomButton;
    
    ScopedPointer<TextButton> theVelocityRandomReset;
    
    ScopedPointer<ComboBox> theMidiOutputBox;
    
    ScopedPointer<Slider> theShuffleSlider;
    
    OwnedArray<Slider> theVelocitySliders;
    
    ScopedPointer<Slider> theSequenceLengthSlider;
    
    ScopedPointer<ComboBox> theRangeBox;
    
    ScopedPointer<TextButton> theSaveSequencerButton;
    
    ScopedPointer<TextButton> thePasteSequencerButton;
    OwnedArray<ToggleButton> theOnOffButtons;
    
};



#endif  // __PLUGINEDITOR_H_14E432C7__