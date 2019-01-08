//////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary Presetz, Copyright Tom Tollenaere 2018-19.

Topiary Presetz is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Topiary Presetz is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Topiary Presetz. If not, see <https://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TopiaryPresetzModel.h" 

//==============================================================================
/**
*/
class TopiaryPresetzAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    TopiaryPresetzAudioProcessor();
    ~TopiaryPresetzAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	TopiaryPresetzModel* getModel();

private:
	TopiaryPresetzModel presetzModel;	// do this first!!!
	
	int tellModelToRun()
	{ // do all that is needed to tell the model to really start running at (sample) time t
		presetzModel.setRunState(Topiary::Running);
		presetzModel.setSampleRate(getSampleRate());	// see if prepareToPlay is called, if so no need to keep checking this
		presetzModel.setStartTimes();	// and do some housekeeping like set the parents to the correct variation
		presetzModel.initializeVariationsForRunning();
		return presetzModel.getRunState();  // because if there are no variations selected, it shouldn't run!!!
	} // tellModelToRun
	

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopiaryPresetzAudioProcessor)
};