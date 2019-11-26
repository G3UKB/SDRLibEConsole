/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Components/vfo.h"

//==============================================================================
MainComponent::MainComponent()
{
	vfo_component = new VFOComponent(0, 0, 0, 0, getWidth(), getHeight());
	addAndMakeVisible(vfo_component);
	setSize(600, 400);
	//printf("Main constructor\n");
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	//printf("Main resized\n");
	//setSize(600, 400);
	vfo_component->setSize(getWidth(), getHeight());
}
