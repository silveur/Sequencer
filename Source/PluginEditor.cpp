/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define NUM_STEPS   16

//==============================================================================
PluginEditor::PluginEditor (SequencerAuAudioProcessor* ownerFilter)
: AudioProcessorEditor (ownerFilter),theRandomButton(0),theRandomNumber(0),theOctaveBox(0),theResetButton(0),theRandomBool(0)
{
    XPosition=0;
    YPosition=0;
    
    sequencerIndex=0;
    for (int i=0; i<NUM_STEPS; i++)
    {
        p_theSliders.add(new Slider());
        addAndMakeVisible(p_theSliders.getUnchecked(i));
        p_theSliders.getUnchecked(i)->setRange(-24, 24,1);
        p_theSliders.getUnchecked(i)->setSliderStyle(Slider::LinearVertical);
        p_theSliders.getUnchecked(i)->setTextBoxStyle(Slider::NoTextBox, false, 30, 30);
        //p_theSliders.getUnchecked(i)->setPopupDisplayEnabled (true, theSequencer);
        p_theSliders.getUnchecked(i)->setDoubleClickReturnValue(true, 0);
        p_theSliders.getUnchecked(i)->setScrollWheelEnabled(false);
        p_theSliders.getUnchecked(i)->setValue(getProcessor()->noteValues[i]);
        p_theSliders.getUnchecked(i)->addListener(this);
        
        theVelocitySliders.add(new Slider());
        addAndMakeVisible(theVelocitySliders.getUnchecked(i));
        theVelocitySliders.getUnchecked(i)->setRange(0, 127,1);
        theVelocitySliders.getUnchecked(i)->setSliderStyle(Slider::RotaryVerticalDrag);
        theVelocitySliders.getUnchecked(i)->setTextBoxStyle(Slider::NoTextBox, false, 10, 10);
        //theVelocitySliders.getUnchecked(i)->setPopupDisplayEnabled (true, theSequencer);
        theVelocitySliders.getUnchecked(i)->setDoubleClickReturnValue(true, 127);
        theVelocitySliders.getUnchecked(i)->addListener(this);
        theVelocitySliders.getUnchecked(i)->setScrollWheelEnabled(false);
        theVelocitySliders.getUnchecked(i)->setValue(getProcessor()->velocities[i]);
        
        theOnOffButtons.add(new ToggleButton("onOff"));
        addAndMakeVisible(theOnOffButtons.getUnchecked(i));
        theOnOffButtons[i]->setToggleState(getProcessor()->theOnOffArray[i],dontSendNotification);
        theOnOffButtons.getUnchecked(i)->addListener(this);
    }
    
    addAndMakeVisible(theRootKeyBox = new ComboBox());
    theRootKeyBox->setBounds(XPosition, YPosition, 110, 30);
    theRootKeyBox->setJustificationType(Justification::centred);
    this->fillRootKeyBox(theRootKeyBox);
    theRootKeyBox->addListener(this);
    theRootKeyBox->setSelectedId(getProcessor()->rootKey+1);
    
    addAndMakeVisible(theOctaveBox = new ComboBox());
    theOctaveBox->setBounds(theRootKeyBox->getX(), YPosition+theRootKeyBox->getHeight(), 110, 30);
    theOctaveBox->setJustificationType(Justification::centred);
    this->fillTheOctaveBox(theOctaveBox);
    theOctaveBox->addListener(this);
    theOctaveBox->setSelectedId(getProcessor()->transpose+3);
    
    addAndMakeVisible( theRandomButton = new TextButton(L"Pitch Random"));
    theRandomButton->setBounds(theRootKeyBox->getX()+theRootKeyBox->getWidth()+20, YPosition , 70, 30);
    theRandomButton->addListener(this);
    
    
    addAndMakeVisible( theResetButton = new TextButton(L"Reset"));
    theResetButton->setBounds(theRootKeyBox->getX()+theRootKeyBox->getWidth()+20, theRandomButton->getHeight() + YPosition , 70, 30);
    theResetButton->addListener(this);
    
    
    addAndMakeVisible( theVelocityRandomButton = new TextButton(L"Velocity Random"));
    theVelocityRandomButton->setBounds(theRandomButton->getX()+theRandomButton->getWidth()+20, YPosition , 70, 30);
    theVelocityRandomButton->addListener(this);
    
    addAndMakeVisible( theVelocityRandomReset = new TextButton(L"Reset"));
    theVelocityRandomReset->setBounds(theResetButton->getX()+theResetButton->getWidth()+20, YPosition + theVelocityRandomButton->getHeight() , 70, 30);
    theVelocityRandomReset->addListener(this);
    
    StringArray array;
    array = MidiOutput::getDevices();
    
    addAndMakeVisible(theSequenceLengthSlider = new Slider(L"Sequencer steps"));
    theSequenceLengthSlider->setBounds(theVelocityRandomReset->getX() + theVelocityRandomReset->getWidth(), YPosition+theVelocityRandomButton->getY(), 70, 30);
    theSequenceLengthSlider->addListener(this);
    theSequenceLengthSlider->setRange(2, 16,1);
    theSequenceLengthSlider->setValue(getProcessor()->numSteps);
    theSequenceLengthSlider->setSliderStyle(Slider::LinearHorizontal);
    theSequenceLengthSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 20);
    theSequenceLengthSlider->setScrollWheelEnabled(false);
    // theSequenceLengthSlider->setPopupDisplayEnabled (true, theSequencer);
    theSequenceLengthSlider->setDoubleClickReturnValue(true, 120);
    theSequenceLengthSlider->setTextValueSuffix(" Steps");
    
    addAndMakeVisible(theMidiOutputBox = new ComboBox());
    theMidiOutputBox->setBounds(theSequenceLengthSlider->getX()+theSequenceLengthSlider->getWidth()+20, YPosition, 130, 30);
    theMidiOutputBox->setJustificationType(Justification::centred);
    theMidiOutputBox->addItemList(array, 1);
    theMidiOutputBox->addListener(this);
    theMidiOutputBox->setSelectedId(getProcessor()->midiOutputIndex+1);
    StringArray theRangeArray;
    theRangeArray.add("2 semitones");
    theRangeArray.add("6 semitones");
    theRangeArray.add("12 semitones");
    theRangeArray.add("24 semitones");
    addAndMakeVisible(theRangeBox = new ComboBox());
    theRangeBox->setBounds(theMidiOutputBox->getX(), theMidiOutputBox->getY()+theMidiOutputBox->getHeight(), 130, 30);
    theRangeBox->setJustificationType(Justification::centred);
    theRangeBox->addItemList(theRangeArray, 1);
    theRangeBox->addListener(this);
    theRangeBox->setSelectedId(3);
    this->range=12;
    
    
    addAndMakeVisible( theSaveSequencerButton = new TextButton(L"Save"));
    theSaveSequencerButton->setBounds(theMidiOutputBox->getX()+theMidiOutputBox->getWidth()+10, theMidiOutputBox->getY() , 50, theRangeBox->getHeight());
    theSaveSequencerButton->addListener(this);
    
    addAndMakeVisible( thePasteSequencerButton = new TextButton(L"Paste"));
    thePasteSequencerButton->setBounds(theSaveSequencerButton->getX(), theRangeBox->getY(), 50, theRangeBox->getHeight());
    thePasteSequencerButton->addListener(this);

    
    addAndMakeVisible(theShuffleSlider = new Slider(L"ShuffleSlider"));
    theShuffleSlider->setRange(0, 1,0.05);
    theShuffleSlider->setValue(0);
    theShuffleSlider->setScrollWheelEnabled(false);
    theShuffleSlider->setDoubleClickReturnValue(true, 0);
    theShuffleSlider->setSliderStyle(Slider::RotaryVerticalDrag);
    theShuffleSlider->setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    //theShuffleSlider->setPopupDisplayEnabled (true, theSequencer);
    //theShuffleSlider->setTextValueSuffix(" Shuffle");
    theShuffleSlider->setBounds(SEQUENCER_SIZE_X - 75, YPosition, 60, 50);
    
    p_theSliders.getUnchecked(0)->setBounds(0, theOctaveBox->getY()+40, 50, 100);
    theVelocitySliders.getUnchecked(0)->setBounds(p_theSliders.getUnchecked(0)->getX()+15, p_theSliders.getUnchecked(0)->getY()+p_theSliders.getUnchecked(0)->getHeight(), 20, 20);
    theOnOffButtons.getUnchecked(0)->setBounds(theVelocitySliders.getUnchecked(0)->getX(), theVelocitySliders.getUnchecked(0)->getHeight()+theVelocitySliders.getUnchecked(0)->getY(), 20, 20);

    
    for (int i=1; i<NUM_STEPS; i++)
    {
        p_theSliders.getUnchecked(i)->setBounds(p_theSliders.getUnchecked(i-1)->getX()+50, theOctaveBox->getY()+ 40, 50, 100);
        theVelocitySliders.getUnchecked(i)->setBounds(
                                                      p_theSliders.getUnchecked(i-1)->getX()+65,
                                                      p_theSliders.getUnchecked(i-1)->getY()+p_theSliders.getUnchecked(i-1)->getHeight(),
                                                      20,
                                                      20);
             theOnOffButtons.getUnchecked(i)->setBounds(theVelocitySliders.getUnchecked(i)->getX(), theVelocitySliders.getUnchecked(i)->getHeight()+theVelocitySliders.getUnchecked(i)->getY(), 20, 20);
    }
    
    theRandomNumber = new Random(16);
    theRandomBool = new Random(16);
    
    this->setSize (SEQUENCER_SIZE_X,SEQUENCER_SIZE_Y);
    setSize (SEQUENCER_SIZE_X,
             SEQUENCER_SIZE_Y+30);
    startTimer(50);
}

PluginEditor::~PluginEditor()
{
    deleteAndZero(theRootKeyBox);
    deleteAndZero(theRandomButton);
    deleteAndZero(theRandomNumber);
    deleteAndZero(theRandomBool);
}
void PluginEditor::timerCallback()
{
    SequencerAuAudioProcessor* ourProcessor = getProcessor();
    for(int i=0;i<16;i++)
    {
        p_theSliders.getUnchecked(i)->setValue(ourProcessor->noteValues[i],dontSendNotification);
        theOnOffButtons.getUnchecked(i)->setToggleState(ourProcessor->theOnOffArray[i], dontSendNotification);
    }
    int newSequencerIndex = ourProcessor->getStep();
    if(sequencerIndex!=newSequencerIndex)
    {
        sequencerIndex=newSequencerIndex;
        repaint();
    } 
}
void PluginEditor::paint(Graphics &g)
{
    g.fillAll (Colour (0xffffffff));
    g.setColour (Colours::white);
    
    g.setColour(Colours::red);
    g.fillEllipse(20+(50*(sequencerIndex)), theVelocitySliders.getUnchecked(1)->getY()+43, 10, 10);
}
void PluginEditor::resized()
{
    infoLabel.setBounds (10, 4, 400, 25);
    
    getProcessor()->lastUIWidth = getWidth();
    getProcessor()->lastUIHeight = getHeight();
}
void PluginEditor::fillRootKeyBox(ComboBox* theBox)
{
    theBox->addItem("Key: C  " , 1);
    theBox->addItem("Key: C# " , 2);
    theBox->addItem("Key: D  " , 3);
    theBox->addItem("Key: D# " , 4);
    theBox->addItem("Key: E  " , 5);
    theBox->addItem("Key: F  " , 6);
    theBox->addItem("Key: F# " , 7);
    theBox->addItem("Key: G  " , 8);
    theBox->addItem("Key: G#"  , 9);
    theBox->addItem("Key: A " , 10);
    theBox->addItem("Key: A#" , 11);
    theBox->addItem("Key: B " , 12);
}
int PluginEditor::getStepValue(int index)
{
    return p_theSliders.getUnchecked(index)->getValue();
    
}
void PluginEditor::setStepValue(int index, int value)
{
    p_theSliders.getUnchecked(index)->setValue(value);
}
int PluginEditor::getTransposeValue()
{
    return theOctaveBox->getSelectedItemIndex();
}
int PluginEditor::getKey()
{
    return theRootKeyBox->getSelectedItemIndex();
}
int PluginEditor::getSequencerLength()
{
    return theSequenceLengthSlider->getValue();
}
void PluginEditor::setKey(int value)
{
    theRootKeyBox->setSelectedId(value+1);
}
void PluginEditor::setTranspose(int value)
{
    theOctaveBox->setSelectedId(value+1);
}
void PluginEditor::setSequencerLength(int value)
{
    theSequenceLengthSlider->setValue(value);
}
void PluginEditor::setRange(double value)
{
    theRangeBox->setSelectedId(value+1);
}
void PluginEditor::fillTheOctaveBox(ComboBox* theBox)
{
    theBox->addItem("-2 octave" , 1);
    theBox->addItem("-1 octave" , 2);
    theBox->addItem("No transpose" , 3);
    theBox->addItem("+1 octave" , 4);
    theBox->addItem("+2 octave" , 5);
}
void PluginEditor::setShuffle(double value)
{
    theShuffleSlider->setValue(value);
}
int PluginEditor::getVelocity(int index)
{
    int temp = theVelocitySliders.getUnchecked(index)->getValue();
    return temp;
}
float PluginEditor::getShuffle()
{
    float temp = theShuffleSlider->getValue();
    return temp;
}
void PluginEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if(buttonThatWasClicked == theRandomButton)
    {
        for (int i=0; i<NUM_STEPS; i++)
        {
            int random = rand() % (range*2);
            p_theSliders.getUnchecked(i)->setValue(random-range);
        }
    }
    else if(buttonThatWasClicked == theResetButton)
    {
        for (int i=0; i<NUM_STEPS; i++)
        {
            p_theSliders.getUnchecked(i)->setValue(0);
        }
    }
    else if(buttonThatWasClicked == theVelocityRandomButton)
    {
        for (int i=0; i<NUM_STEPS; i++)
        {
            int random = rand() % 127;
            theVelocitySliders.getUnchecked(i)->setValue(random);
        }
    }
    else if(buttonThatWasClicked == theVelocityRandomReset)
    {
        for (int i=0; i<NUM_STEPS; i++)
        {
            theVelocitySliders.getUnchecked(i)->setValue(127);
        }
    }
    else if(buttonThatWasClicked == theSaveSequencerButton)
    {
        // theSequencer->copyToXml();
    }
    else if(buttonThatWasClicked == thePasteSequencerButton)
    {
        // theSequencer->pasteToXml();
    }
    else
    {
        for(int i=0;i<theOnOffButtons.size();i++)
        {
            if(buttonThatWasClicked==theOnOffButtons.getUnchecked(i))
            {
                getProcessor()->theOnOffArray[i]=theOnOffButtons.getUnchecked(i)->getToggleState();
                return;
            }
        }

    }
}
int PluginEditor::getRange()
{
    return theRangeBox->getSelectedItemIndex();
}
void PluginEditor::comboBoxChanged (ComboBox *comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == theRootKeyBox)
    {
        int newRootKey = theRootKeyBox->getSelectedId()-1;
        
        getProcessor()->rootKey=newRootKey;
        //  theSequencer->rootKeyChanged(newRootKey);
    }
    else if(comboBoxThatHasChanged == theOctaveBox)
    {
        int newTranspose = theOctaveBox->getSelectedId()-3;
        getProcessor()->transpose=newTranspose;
    }
    else if(comboBoxThatHasChanged == theMidiOutputBox)
    {
        getProcessor()->midiOutputIndex=theMidiOutputBox->getSelectedId()-1;
        getProcessor()->setMidiOutputDevice();
    }
    else if(comboBoxThatHasChanged == theRangeBox)
    {
        int newRange = theRangeBox->getSelectedId();
        if(newRange==1)
        {
            for (int i=0; i<16; i++)
            {
                p_theSliders.getUnchecked(i)->setRange(-2,2,1);
                range = 2;
            }
        }
        else if(newRange==2)
        {
            for (int i=0; i<16; i++)
            {
                p_theSliders.getUnchecked(i)->setRange(-6,6,1);
                range = 6;
            }
        }
        else if(newRange==3)
        {
            for (int i=0; i<16; i++)
            {
                p_theSliders.getUnchecked(i)->setRange(-12,12,1);
                range = 12;
            }
        }
        
        else if(newRange==4)
        {
            for (int i=0; i<16; i++)
            {
                p_theSliders.getUnchecked(i)->setRange(-24, 24,1);
                range = 24;
            }
        }
    }
}
void PluginEditor::sliderValueChanged(Slider *sliderThatWasMoved)
{
    SequencerAuAudioProcessor* ourProcessor = getProcessor();
    
    if(sliderThatWasMoved == theSequenceLengthSlider)
    {
        ourProcessor->numSteps=theSequenceLengthSlider->getValue();
        return;
    }
    else
    {
        for(int i=0;i<16;i++)
        {
            if(sliderThatWasMoved==p_theSliders.getUnchecked(i))
            {
                int value = p_theSliders.getUnchecked(i)->getValue();
                ourProcessor->noteValues[i]=value;
                return;
            }
            else if(sliderThatWasMoved==theVelocitySliders.getUnchecked(i))
            {
                ourProcessor->velocities[i]=theVelocitySliders[i]->getValue();
                return;
            }
        }
    }
}
bool PluginEditor::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    return true;
}