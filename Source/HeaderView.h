/*
  ==============================================================================

    HeaderView.h
    Created: 23 Apr 2014 3:41:16pm
    Author:  Silvere Letellier

  ==============================================================================
*/

#ifndef HEADERVIEW_H_INCLUDED
#define HEADERVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ControllerView.h"

extern UndoManager* theUndoManager;

class HeaderView: public Component, ButtonListener, public ComboBoxListener, public ValueTree::Listener, SliderListener
{
public:
	HeaderView(ControllerView* controllerView, ValueTree& preferenceTree): theControllerView(controllerView)
	{
		theMainLabel = "Sequencer";
		thePreferenceTree = preferenceTree;
		addAndMakeVisible(theAddSequencerButton = new TextButton("Add Sequencer"));
		theAddSequencerButton->addListener(this);
		addAndMakeVisible(theUndoButton = new TextButton("Undo"));
		theUndoButton->addListener(this);
		addAndMakeVisible(theRedoButton = new TextButton("Redo"));
		theRedoButton->addListener(this);
		addAndMakeVisible(theExportAllButton = new TextButton("Export all"));
		theExportAllButton->addListener(this);
		addAndMakeVisible(theImportAllButton = new TextButton("Import all"));
		theImportAllButton->addListener(this);
		addAndMakeVisible(theKickBackButton = new TextButton("Rewind"));
		theKickBackButton->addListener(this);
		addAndMakeVisible(theClockSourceList = new ComboBox("Clock source"));
		theClockSourceList->addSectionHeading("Clock source");
		theClockSourceList->addItem("External", 1);
		theClockSourceList->addItem("Internal", 2);
		bool clockMode = thePreferenceTree.getProperty("ClockMode", 0);
		theClockSourceList->setSelectedItemIndex(clockMode);
		theClockSourceList->addListener(this);
		
		addAndMakeVisible(theMasterClockList = new ComboBox("MasterClock"));
		theMasterClockList->addSectionHeading("Clock output");
		theMasterClockList->setTextWhenNothingSelected("Clock output");
		theMasterClockList->setTextWhenNoChoicesAvailable("No midi output available");
		StringArray midiList = MidiOutput::getDevices();
		theMasterClockList->addItem("--", 1);
		for(int i=0;i<midiList.size();i++)
		{
			if (midiList[i] != "Sequencer") theMasterClockList->addItem(midiList[i], i+2);
		}
		theMasterClockList->setSelectedItemIndex(thePreferenceTree.getProperty("MasterClock"));
		theMasterClockList->addListener(this);

		addAndMakeVisible(theBPMSlider = new Slider("BPM"));
		theBPMSlider->setTextBoxStyle(Slider::TextBoxLeft, false, 60, 50);
		theBPMSlider->setRange(60, 180, 0.1);
		theBPMSlider->setScrollWheelEnabled(false);
		theBPMSlider->setSliderStyle(Slider::RotaryVerticalDrag);
		theBPMSlider->setValue(thePreferenceTree.getProperty("BPM", 120));
		theBPMSlider->addListener(this);
		theBPMSlider->setVisible(clockMode);
		setInterceptsMouseClicks(false, true);
		thePreferenceTree.addListener(this);
	}
	~HeaderView()
	{
		theControllerView = nullptr;
	}
	
	void buttonClicked(Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == theAddSequencerButton)
		{
			ValueTree tree;
			theControllerView->addSequencer(tree);
		}
		else if (buttonThatWasClicked == theKickBackButton)
		{
			theControllerView->kickBack();
		}
		else if (buttonThatWasClicked == theImportAllButton)
		{
			importAll();
		}
		else if (buttonThatWasClicked == theExportAllButton)
		{
			exportAll();
		}
		repaint();
	}
	
	void comboBoxChanged(ComboBox* box)
	{
		if (box == theClockSourceList)
		{
			thePreferenceTree.setProperty("ClockMode", box->getSelectedItemIndex(), nullptr);
		}
		else if (box == theMasterClockList)
		{
			thePreferenceTree.setProperty("MasterClock", box->getSelectedItemIndex(), nullptr);
		}
	}
	
	void sliderValueChanged(Slider* slider)
	{
		if (slider == theBPMSlider)
		{
			thePreferenceTree.setProperty("BPM", slider->getValue(), nullptr);
		}
	}
	
	void exportAll()
	{
		FileChooser fileChooser ("Save as...",
								 thePresetFolder,
								 "*.masterseq");
		if (fileChooser.browseForFileToSave(false))
		{
			File preset = File(fileChooser.getResult().getFullPathName());
			FileOutputStream outputStream(preset);
			ValueTree masterTree = theControllerView->getMasterTree();
			masterTree.writeToStream(outputStream);
		}
	}
	
	void importAll()
	{
		FileChooser fileChooser ("Load preset file...",
								 thePresetFolder,
								 "*.masterseq");
		if (fileChooser.browseForFileToOpen())
		{
			ValueTree masterTree = theControllerView->getMasterTree();
			while (masterTree.getNumChildren())
			{
				theControllerView->removeSequencer(-1);
			}
			File presetToLoad = fileChooser.getResult();
			FileInputStream inputStream(presetToLoad);
			ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
			for (int i=0;i<treeToLoad.getNumChildren();i++)
			{
				ValueTree treeToAdd = treeToLoad.getChild(i);
				theControllerView->addSequencer(treeToAdd);
			}
		}
	}

	void paint(Graphics& g) {}
	
	void resized()
	{
		theAddSequencerButton->setBounds(10, getHeight()/4, getWidth()/12, getHeight()/2);
		theExportAllButton->setBounds(theAddSequencerButton->getRight(), theAddSequencerButton->getY(), getWidth()/16, getHeight()/2);
		theImportAllButton->setBounds(theExportAllButton->getRight(), theExportAllButton->getY(), getWidth()/16, getHeight()/2);
		theBPMSlider->setBounds(getWidth()/1.6, 0, getHeight()*3, getHeight());
		theMasterClockList->setBounds(theBPMSlider->getRight(), theMasterClockList->getY(), getWidth()/8, getHeight());
		theClockSourceList->setBounds(theMasterClockList->getRight(), 0, 110, getHeight());
	}
	
	void valueTreePropertyChanged (ValueTree& tree, const Identifier& property)
	{
		if (String(property) == "ClockMode")
		{
			ClockMode theClockMode = (ClockMode)(bool)tree.getProperty(property);
			if (theClockMode == INTERNAL)
			{
				theBPMSlider->setVisible(true);
			}
			else if (theClockMode == EXTERNAL)
			{
				theBPMSlider->setVisible(false);
			}
		}
		else if (String(property) == "BPM")
		{
			theBPMSlider->setValue(tree.getProperty(property));
		}
		else if (String(property) == "MasterClock")
		{
			theMasterClockList->setSelectedItemIndex(tree.getProperty(property));
		}
	}
	void valueTreeChildAdded (ValueTree& parentTree, ValueTree& childWhichHasBeenAdded){}
	void valueTreeChildRemoved (ValueTree& parentTree, ValueTree& childWhichHasBeenRemoved){}
	void valueTreeChildOrderChanged (ValueTree& parentTreeWhoseChildrenHaveMoved){}
	void valueTreeParentChanged (ValueTree& treeWhoseParentHasChanged){}
	
private:
	ScopedPointer<TextButton> theAddSequencerButton;
	ScopedPointer<TextButton> theUndoButton;
	ScopedPointer<TextButton> theRedoButton;
	ScopedPointer<TextButton> theExportAllButton;
	ScopedPointer<TextButton> theImportAllButton;
	ScopedPointer<TextButton> theKickBackButton;
	ScopedPointer<Slider> theBPMSlider;
	ScopedPointer<ComboBox> theClockSourceList;
	ScopedPointer<ComboBox> theMasterClockList;
	ControllerView* theControllerView;
	ValueTree thePreferenceTree;
	String theMainLabel;
};



#endif  // HEADERVIEW_H_INCLUDED
