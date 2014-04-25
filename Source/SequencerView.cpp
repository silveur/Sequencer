/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#include "SequencerView.h"
#include "ControllerView.h"

extern UndoManager* theUndoManager;
extern File thePresetFolder;

SequencerView::SequencerView(ValueTree& sequencerTree, ControllerView* controllerView): theControllerView(controllerView)
{
	theUndoManager->getNumActionsInCurrentTransaction();

	theSequencerTree = sequencerTree;
	thePosition = theSequencerTree.getProperty("Position");
	for(int i=0;i<16;i++)
	{
		addAndMakeVisible(theStepSliders.add(new Slider("Pitch" + String(i))));
		theStepSliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theStepSliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theStepSliders[i]->setTextBoxIsEditable(false);
		theStepSliders[i]->setDoubleClickReturnValue(true, 0);
		theStepSliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theStepSliders[i]->setRange(-12, 12, 1);
		theStepSliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Pitch"));
		theStepSliders[i]->addListener (this);
		addAndMakeVisible(theVelocitySliders.add(new Slider("Velocity" + String(i))));
		theVelocitySliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theVelocitySliders[i]->setTextBoxIsEditable(false);
		theVelocitySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theVelocitySliders[i]->setRange(0, 127, 1);
		theVelocitySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theVelocitySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Velocity"));
		theVelocitySliders[i]->addListener(this);
		addAndMakeVisible(theStateButtons.add(new ToggleButton("State" + String(i))));
		theStateButtons[i]->setToggleState((bool)theSequencerTree.getChild(i).getProperty("State"), dontSendNotification);
		theStateButtons[i]->addListener(this);
		addAndMakeVisible(theDecaySliders.add(new Slider("Decay" + String(i))));
		theDecaySliders[i]->setSliderStyle(Slider::RotaryVerticalDrag);
		theDecaySliders[i]->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
		theDecaySliders[i]->setTextBoxIsEditable(false);
		theDecaySliders[i]->setDoubleClickReturnValue(true, 0);
		theDecaySliders[i]->setPopupDisplayEnabled(true, theControllerView);
		theDecaySliders[i]->setRange(1, 200, 1);
		theDecaySliders[i]->setValue((int)theSequencerTree.getChild(i).getProperty("Decay"));
		theDecaySliders[i]->addListener (this);
	}
	addAndMakeVisible(theSequencerLength = new Slider("Length"));
	theSequencerLength->setSliderStyle(Slider::LinearHorizontal);
	theSequencerLength->setRange(1, 16,1);
	theSequencerLength->setValue(theSequencerTree.getProperty("Length"));
	theSequencerLength->addListener(this);
	
	addAndMakeVisible(theRandomAllButton = new TextButton("Random all"));
	theRandomAllButton->addListener(this);
	
	addAndMakeVisible(theCopyButton = new TextButton("Copy settings"));
	theCopyButton->addListener(this);
	
	addAndMakeVisible(thePasteButton = new TextButton("Paste settings"));
	thePasteButton->addListener(this);
	
	addAndMakeVisible(theSaveButton = new TextButton("Save"));
	theSaveButton->addListener(this);
	
	addAndMakeVisible(theDeleteButton = new TextButton("Delete"));
	theDeleteButton->addListener(this);
	
	addAndMakeVisible(theShuffleSlider = new Slider("Shuffle"));
	theShuffleSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theShuffleSlider->setRange(0, 5, 1);
	theShuffleSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theShuffleSlider->setValue(theSequencerTree.getProperty("Shuffle"));
	theShuffleSlider->setPopupDisplayEnabled(true, theControllerView);
	theShuffleSlider->addListener(this);
	
	addAndMakeVisible(&theStepView);
	
	addAndMakeVisible(theOnOffButton = new ToggleButton("On/Off"));
	theOnOffButton->setToggleState(theSequencerTree.getProperty("Status"), dontSendNotification);
	theOnOffButton->addListener(this);
	
	addAndMakeVisible(theRangeSlider = new Slider("Range"));
	theRangeSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 50);
	theRangeSlider->setRange(1, 5, 1);
	theRangeSlider->setSliderStyle(Slider::RotaryVerticalDrag);
	theRangeSlider->setPopupDisplayEnabled(true, theControllerView);
	theRangeSlider->setValue(theSequencerTree.getProperty("Range"));
	theRangeSlider->addListener(this);

	addAndMakeVisible(theMidiOutputList = new ComboBox("Midi Output list"));
	theMidiOutputList->setTextWhenNothingSelected("Select an midi output");
	theMidiOutputList->setTextWhenNoChoicesAvailable("No midi output available");
	
	addAndMakeVisible(theChannelList = new ComboBox("Channel"));
	for (int i=1;i<=16;i++)
		theChannelList->addItem(String(i), i);
	theChannelList->setSelectedId(theSequencerTree.getProperty("Channel"));
	theChannelList->addListener(this);
	
	refreshMidiList();
	String str = theSequencerTree.getProperty("MidiOutput");
	updateSelectedMidiOut(str);
	addAndMakeVisible(theRootNoteList = new ComboBox("RootNoteList"));
	addAndMakeVisible(theRootOctaveList = new ComboBox("RootOctaveList"));
	addAndMakeVisible(thePresetBox = new ComboBox("Preset List"));
	thePresetBox->setEditableText(true);
	thePresetBox->addListener(this);
	
	updateNotesAndOctaves();
	theRootNoteList->setSelectedItemIndex(theSequencerTree.getProperty("RootNote"));
	theRootOctaveList->setSelectedItemIndex(theSequencerTree.getProperty("RootOctave"));
	theRootNoteList->addListener(this);
	theRootOctaveList->addListener(this);
	theSequencerTree.addListener(this);
	theMidiOutputList->addListener(this);
	setRepaintsOnMouseActivity(false);
	
	updatePresetList();
	setSize(getWidth(), getHeight());
}

SequencerView::~SequencerView()
{
}

void SequencerView::handleAsyncUpdate()
{
	theStepView.update(theStepSliders[thePosition]->getX());
}

void SequencerView::refreshMidiList()
{
	addAndMakeVisible(theMidiOutputList);
	StringArray midiList = MidiOutput::getDevices();
	theMidiOutputList->clear();
	for(int i=0;i<midiList.size();i++)
	{
		if (midiList[i] != "Sequencer")
			theMidiOutputList->addItem(midiList[i], i+1);
	}
}

void SequencerView::paint(Graphics& g)
{
	g.setColour(Colours::blue);
	g.drawRect(0,0,getWidth(),getHeight());
}

void SequencerView::resized()
{
	for(int i=0;i<16;i++)
	{
		theStepSliders[i]->setBounds((getWidth()/16)*i, 40, 50, 50);
		theVelocitySliders[i]->setBounds(theStepSliders[i]->getX(), theStepSliders[i]->getBottom(), 25, 20);
		theDecaySliders[i]->setBounds(theVelocitySliders[i]->getRight(), theStepSliders[i]->getBottom(), 25, 20);
		theStateButtons[i]->setBounds(theStepSliders[i]->getX(), theVelocitySliders[i]->getBottom(), 50, 30);
	}
	theMidiOutputList->setBounds(10, 0, 150, 20);
	theChannelList->setBounds(theMidiOutputList->getRight(), theMidiOutputList->getY(), 50, 20);
	theSequencerLength->setBounds(theChannelList->getRight(), 0, 150, 20);
	theRootNoteList->setBounds(10, 20, 40, 20);
	theRootOctaveList->setBounds(theRootNoteList->getRight(), theRootNoteList->getY(), theRootNoteList->getWidth(), theRootNoteList->getHeight());
	theRandomAllButton->setBounds(theSequencerLength->getRight(), 0, 90, 20);
	theShuffleSlider->setBounds(200, 20, 30, 20);
	theRangeSlider->setBounds(250, 20, 30, 20);
	theCopyButton->setBounds(theRandomAllButton->getRight(), 0, 60, 20);
	thePasteButton->setBounds(theCopyButton->getRight(), 0, 60, 20);
	theStepView.setBounds(0, getHeight()-20, getWidth(), 20);
	thePresetBox->setBounds(getWidth()-150, 0, 150, 20);
	theSaveButton->setBounds(thePasteButton->getRight(), 0, 60, 20);
	theDeleteButton->setBounds(theSaveButton->getRight(), 0, 60, 20);
	theOnOffButton->setBounds(theDeleteButton->getRight() + 20, theDeleteButton->getY(), 60, 20);
}

void SequencerView::buttonClicked(Button* button)
{
	if (button == theRandomAllButton)
	{
		for (int i=0;i<16;i++)
		{
			ValueTree child = theSequencerTree.getChild(i);
			child.setProperty("Pitch", ((int)rand() % 24) - 12, theUndoManager);
			child.setProperty("State", rand() % 2, theUndoManager);
			child.setProperty("Velocity", ((int)rand() % 127), theUndoManager);
			child.setProperty("Decay", ((int)rand() % 200), theUndoManager);
		}
	}
	else if (button == theCopyButton)
	{
		getCopyTree() = theSequencerTree.createCopy();
	}
	else if (button == thePasteButton)
	{
		getCopyTree().removeProperty("MidiOutput", nullptr);
		theSequencerTree.copyPropertiesFrom(getCopyTree(), theUndoManager);
		for (int i=0; i<16; i++)
		{
			ValueTree sourceChild = getCopyTree().getChild(i);
			ValueTree destinationChild = theSequencerTree.getChild(i);
			destinationChild.copyPropertiesFrom(sourceChild, theUndoManager);
		}
		thePresetBox->setSelectedItemIndex(0); 
	}
	else if (button == theSaveButton)
	{
		if (thePresetBox->getText() != "* New Preset *")
		{
			File presetToSave(thePresetFolder.getFullPathName() + "/" + thePresetBox->getText() + ".seq");
			if (presetToSave.exists()) presetToSave.replaceWithData(nullptr, 0);
			FileOutputStream outputStream(presetToSave);
			theSequencerTree.writeToStream(outputStream);
		}
	}
	else if (button == theDeleteButton)
	{
		File presetToSave(thePresetFolder.getFullPathName() + "/" + thePresetBox->getText() + ".seq");
		if (presetToSave.exists())
		{
			presetToSave.deleteFile();
			theControllerView->updatePresetList();
		}
	}
	else if (button == theOnOffButton)
	{
		theSequencerTree.setProperty("Status", theOnOffButton->getToggleState(), nullptr);
	}
	else
	{
		int index = button->getName().getTrailingIntValue();
		theSequencerTree.getChild(index).setProperty("State", (bool)button->getToggleState(), theUndoManager);
	}
}

void SequencerView::sliderValueChanged(Slider* slider)
{
	int index = slider->getName().getTrailingIntValue();
	if(slider->getName().contains("Pitch"))
	{
		theSequencerTree.getChild(index).setProperty("Pitch", (int)slider->getValue(), theUndoManager);
	}
	else if(slider->getName().contains("Velocity"))
	{
		theSequencerTree.getChild(index).setProperty("Velocity", (int)slider->getValue(), theUndoManager);
	}
	else if(slider->getName().contains("Decay"))
	{
		theSequencerTree.getChild(index).setProperty("Decay", (int)slider->getValue(), theUndoManager);
	}
	else if(slider == theSequencerLength)
	{
		theSequencerTree.setProperty("Length", slider->getValue(), theUndoManager);
	}
	else if(slider == theShuffleSlider)
	{
		theSequencerTree.setProperty("Shuffle", slider->getValue(), theUndoManager);
	}
	else if(slider == theRangeSlider)
	{
		theSequencerTree.setProperty("Range", slider->getValue(), theUndoManager);
	}
}

void SequencerView::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	if(comboBoxThatHasChanged == theMidiOutputList)
	{
		String midiOutString = theMidiOutputList->getItemText(theMidiOutputList->getSelectedItemIndex());
		theSequencerTree.setProperty("MidiOutput", midiOutString, nullptr);
	}
	else if(comboBoxThatHasChanged == theRootOctaveList)
	{
		int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootOctave", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == theRootNoteList)
	{
		int id = comboBoxThatHasChanged->getSelectedItemIndex();
		theSequencerTree.setProperty("RootNote", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == theChannelList)
	{
		int id = comboBoxThatHasChanged->getSelectedId();
		theSequencerTree.setProperty("Channel", id, theUndoManager);
	}
	else if(comboBoxThatHasChanged == thePresetBox)
	{
		if (thePresetBox->getSelectedItemIndex() == -1)
		{
			if (thePresetBox->getText() != "* New Preset *" && !thePresetBox->getText().isEmpty())
			{
				File presetToSave(thePresetFolder.getFullPathName() + "/" + comboBoxThatHasChanged->getText() + ".seq");
				FileOutputStream outputStream(presetToSave);
				theSequencerTree.writeToStream(outputStream);
				theControllerView->updatePresetList();
				for (int i=0;i<thePresetBox->getNumItems();i++)
				{
					if (thePresetBox->getItemText(i) == presetToSave.getFileNameWithoutExtension())
					{
						thePresetBox->setSelectedItemIndex(i);
					}
				}
			}
		}
		else
		{
			String preset = thePresetBox->getText();
			File presetToLoad(thePresetFolder.getFullPathName() + "/" + preset + ".seq");
			if (presetToLoad.exists())
			{
				FileInputStream inputStream(presetToLoad);
				ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
				treeToLoad.removeProperty("MidiOutput", nullptr);
				theSequencerTree.copyPropertiesFrom(treeToLoad, nullptr);
				for (int i=0; i<16; i++)
				{
					ValueTree sourceChild = treeToLoad.getChild(i);
					ValueTree destinationChild = theSequencerTree.getChild(i);
					destinationChild.copyPropertiesFrom(sourceChild, nullptr);
				}
			}
		}
	}
}

void SequencerView::valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
{
	if(String(property) == "Position")
	{
		thePosition = tree.getProperty(property);
		triggerAsyncUpdate();
	}
	else if(String(property) == "MidiOutput")
	{
		String midiOutput = tree.getProperty("MidiOutput");
		updateSelectedMidiOut(midiOutput);
	}
	else if(String(property) == "Range")
	{
		theRangeSlider->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Shuffle")
	{
		theShuffleSlider->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Length")
	{
		theSequencerLength->setValue(tree.getProperty(property));
	}
	else if(String(property) == "Channel")
	{
		theChannelList->setSelectedId(tree.getProperty(property));
	}
	else if(String(property) == "Status")
	{
		theOnOffButton->setToggleState(tree.getProperty(property), dontSendNotification);
	}
	else
	{
		for (int i=0; i<16;i++)
		{
			if (tree == (theSequencerTree.getChild(i)))
			{
				if (String(property) == "Pitch")
					theStepSliders[i]->setValue((int)tree.getProperty(property));
				else if (String(property) == "State")
					theStateButtons[i]->setToggleState((bool)tree.getProperty(property), dontSendNotification);
				else if (String(property) == "Velocity")
					theVelocitySliders[i]->setValue((int)tree.getProperty(property));
				else if (String(property) == "Decay")
					theDecaySliders[i]->setValue((int)tree.getProperty(property));
			}
		}
	}
}

void SequencerView::updateSelectedMidiOut(String& midiOut)
{
	for (int i=0;i<theMidiOutputList->getNumItems();i++)
	{
		if (theMidiOutputList->getItemText(i) == midiOut)
		{
			theMidiOutputList->setSelectedItemIndex(i);
			break;
		}
	}
}

void SequencerView::updateNotesAndOctaves()
{
	theRootNoteList->addItem("C",1);
	theRootNoteList->addItem("C#",2);
	theRootNoteList->addItem("D",3);
	theRootNoteList->addItem("D#",4);
	theRootNoteList->addItem("E",5);
	theRootNoteList->addItem("F",6);
	theRootNoteList->addItem("F#",7);
	theRootNoteList->addItem("G",8);
	theRootNoteList->addItem("G#",9);
	theRootNoteList->addItem("A",10);
	theRootNoteList->addItem("A#",11);
	theRootNoteList->addItem("B",12);
	for (int i=0;i<8;i++)
		theRootOctaveList->addItem(String(i), i+1);
}

void SequencerView::updatePresetList()
{
	String currentlySelectedItem = thePresetBox->getText();
	thePresetBox->clear();
	thePresetBox->addItem("* New Preset *", 1);
	thePresetBox->setSelectedItemIndex(0);
	Array<File> presetArray;
	int numPreset = thePresetFolder.findChildFiles(presetArray, File::findFiles, true, "*.seq");
	for (int i=0;i<numPreset;i++)
	{
		File preset = presetArray[i];
		thePresetBox->addItem(preset.getFileNameWithoutExtension(), 1 + thePresetBox->getNumItems());
		if (preset.getFileNameWithoutExtension() == currentlySelectedItem)
		{
			thePresetBox->setSelectedId(i);
		}
	}
}

