/*
  ==============================================================================

	RootView.cpp
	Created: 23 Apr 2014 12:15:42pm
	Author:  Silvere Letellier

  ==============================================================================
*/

#include "ControllerView.h"
#include "HeaderView.h"
#include "AboutView.h"

ColourTheme theColourTheme = DARK;
Colour textButtonTextColour;

ControllerView::ControllerView(ValueTree& masterTree, ValueTree& preferenceTree): theMasterTree(masterTree), thePreferenceTree(preferenceTree)
{
	textButtonTextColour = SeqLookAndFeel::getColour(COLOUR_1);
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		ValueTree sequenceTree = theMasterTree.getChild(i);
		theSequencerViews.add(new SequencerView(sequenceTree, this));
		addAndMakeVisible(theSequencerViews[i]);
	}
	addAndMakeVisible(theHeaderView = new HeaderView(this, preferenceTree));
	theMasterTree.addListener(this);
	setInterceptsMouseClicks(false, true);
	refreshView();
}

ControllerView::~ControllerView()
{
	delete theHeaderView;
}

void ControllerView::resized()
{
	refreshView();
}

void ControllerView::refreshView()
{
	float headerHeight = 4.0f; float totalDiv = 0.0f; float pixelsPerDiv = 9.0f;
	theHeaderView->setBounds(0, 0, getWidth(), headerHeight * pixelsPerDiv);
	theHeaderView->repaint();
	totalDiv += theHeaderView->getHeight();
	
	for (int i=0; i<theMasterTree.getNumChildren(); i++)
	{
		float sequencerHeigth;
		if ((int)theMasterTree.getChild(i).getProperty("Length") <= 16) sequencerHeigth = 19.0f * pixelsPerDiv;
		else sequencerHeigth = 34.0f * pixelsPerDiv;
		
		theSequencerViews[i]->setBounds(0, totalDiv, getWidth(), sequencerHeigth);
		theSequencerViews[i]->repaint();
		totalDiv += theSequencerViews[i]->getHeight();
	}
	setSize(1200, totalDiv);
}

void ControllerView::exportAll()
{
	FileChooser fileChooser ("Save as...",
							 thePresetFolder,
							 "*.masterseq");
	if (fileChooser.browseForFileToSave(false))
	{
		File preset = File(fileChooser.getResult().getFullPathName());
		FileOutputStream outputStream(preset);
		ValueTree masterTree = getMasterTree();
		masterTree.writeToStream(outputStream);
	}
}

void ControllerView::importAll()
{
	FileChooser fileChooser ("Load preset file...",
							 thePresetFolder,
							 "*.masterseq");
	if (fileChooser.browseForFileToOpen())
	{
		ValueTree masterTree = getMasterTree();
		while (masterTree.getNumChildren())
		{
			removeSequencer(-1);
		}
		File presetToLoad = fileChooser.getResult();
		FileInputStream inputStream(presetToLoad);
		ValueTree treeToLoad = ValueTree::readFromStream(inputStream);
		for (int i=0;i<treeToLoad.getNumChildren();i++)
		{
			ValueTree treeToAdd = treeToLoad.getChild(i);
			addSequencer(treeToAdd);
		}
	}
}

const int ControllerView::getNumOfSequencer() const
{
	return theMasterTree.getNumChildren();
}

void ControllerView::addSequencer(ValueTree& sequencerTreeToAdd)
{
	if(sequencerTreeToAdd.isValid())
	{
		ValueTree copiedTree = sequencerTreeToAdd.createCopy();
		theMasterTree.addChild(copiedTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(copiedTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
	}
	else
	{
		ValueTree sequencerTree("Sequencer" + String(theMasterTree.getNumChildren()));
		theMasterTree.addChild(sequencerTree, -1, nullptr);
		theSequencerViews.add(new SequencerView(sequencerTree, this));
		addAndMakeVisible(theSequencerViews.getLast());
	}
	refreshView();
}

void ControllerView::removeSequencer(int i)
{
	if (i == -1)
	{
		int index = theMasterTree.getNumChildren()-1;
		theMasterTree.removeChild(index, nullptr);
		theSequencerViews.remove(index);
	}
	else
	{
		theMasterTree.removeChild(i, nullptr);
		theSequencerViews.remove(i);
	}
	refreshView();
}
