/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary Presetz, Copyright Tom Tollenaere 2018-2019.

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

#include "PluginProcessor.h"
#include "PluginEditor.h"

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzAudioProcessor::TopiaryPresetzAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynthvv
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif  
	)
#endif
{

}

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzAudioProcessor::~TopiaryPresetzAudioProcessor()
{
} // audioProcessor

//==============================================================================
const String TopiaryPresetzAudioProcessor::getName() const
{
	return JucePlugin_Name;
} // TopiaryPresetzAudioProcessor

/////////////////////////////////////////////////////////////////////////

bool TopiaryPresetzAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

/////////////////////////////////////////////////////////////////////////

bool TopiaryPresetzAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

/////////////////////////////////////////////////////////////////////////

bool TopiaryPresetzAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return false;
#else
	return false;
#endif
}

/////////////////////////////////////////////////////////////////////////

double TopiaryPresetzAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

/////////////////////////////////////////////////////////////////////////

int TopiaryPresetzAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

/////////////////////////////////////////////////////////////////////////

int TopiaryPresetzAudioProcessor::getCurrentProgram()
{
	return 0;
} // getCurrentProgram

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::setCurrentProgram(int index)
{
	UNUSED(index)
} // setCurrentProgram

/////////////////////////////////////////////////////////////////////////

const String TopiaryPresetzAudioProcessor::getProgramName(int index)
{
	UNUSED(index)
		return {};
} // getProgramName

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::changeProgramName(int index, const String& newName)
{
	UNUSED(newName)
		UNUSED(index)
} // changeProgramName

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{ 
	ignoreUnused(samplesPerBlock);
	presetzModel.setSampleRate(sampleRate);
	presetzModel.setBlockSize(samplesPerBlock);
	//setLatencySamples(samplesPerBlock / 10); debug for testing - works equally fine
}  // prepareToPlay

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
} // releaseResources

/////////////////////////////////////////////////////////////////////////

#ifndef JucePlugin_PreferredChannelConfigurations
bool TopiaryPresetzAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
} // isBusesLayoutSupported
#endif

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

	// the audio buffer in a midi effect will have zero channels!
	jassert(buffer.getNumChannels() == 0);

	presetzModel.setBlockSize(buffer.getNumSamples());
	buffer.clear();

	int BPM, numerator, denominator, runState;
	bool override, waitFFN;
	presetzModel.getTransportState(BPM, numerator, denominator, runState, override, waitFFN);

	AudioPlayHead::CurrentPositionInfo lastPosInfo;
	auto playHed = getPlayHead();
	if (playHed != nullptr)
	{
		playHed->getCurrentPosition(lastPosInfo);

		if (lastPosInfo.isPlaying && (runState != Topiary::Running)) // Armed as well to take into account??? Don't think so
		{
			if (!override)
			{
				// just arm it - if we get a note it will switch to playing if WFFN; else if will auto-switch before generating MIDI
				presetzModel.setRunState(Topiary::Armed);
				runState = Topiary::Armed;
			}
		} // detect we are playing depending on override
		else if (!override)
		{ // all other stuff we pick up from host transport
			if (!lastPosInfo.isPlaying && (runState == Topiary::Running))
			{
				// we'll set model to Stopping; it will then decide to really stop when needed
				presetzModel.setRunState(Topiary::Ending);
				runState = Topiary::Ending;
			}
			if (lastPosInfo.bpm != BPM)
			{
				presetzModel.setBPM((int)lastPosInfo.bpm);
			}
			if ((lastPosInfo.timeSigNumerator != numerator) || (lastPosInfo.timeSigDenominator != denominator))
			{
				presetzModel.setNumeratorDenominator(lastPosInfo.timeSigNumerator, lastPosInfo.timeSigDenominator);
			}
		}
	}
	else presetzModel.Log("ERROR>>>> No playhead provided!", Topiary::LogType::Warning);

	if (!waitFFN && (runState == Topiary::Armed))
	{
		//presetzmodel.setRunState(TopiaryPresetzModel::Running) is in tellModel
		tellModelToRun();
		runState = Topiary::Running;
	}

	MidiBuffer processedMidi;   	// midibuffer object to hold what we generate
									// at the end of the processorblock we will swap &midiMessages (that came in) by processedMidi (what goes out)
	MidiMessage msg;
	bool logMidiIn, logMidiOut;	
	presetzModel.getMidiLogSettings(logMidiIn, logMidiOut);
	bool notePassThrough = presetzModel.getNotePassThrough();

	// first see if there are (CC) messages to be output by the model
	presetzModel.outputModelEvents(processedMidi);

	int ignore;  // for the samplePosition in getnextEvent - we ignore that c'se we process immeditately
	for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, ignore);)
	{
		if (msg.isNoteOn())
		{
			//Logger::outputDebugString("Note on");
			//Logger::outputDebugString(String("WaitFFN ") + String((int)waitFFN));
			//Logger::outputDebugString(String("RunState ") + String(runState));

			// if we are ready to play and waiting for first note in, start playing
			if (waitFFN && (runState == Topiary::Armed))
			{
				tellModelToRun();
				runState = Topiary::Running;
			}

			presetzModel.processAutomation(msg); // because we may have switching by notes!
		}
		else
		{
			if (msg.isController())
			{
				presetzModel.processAutomation(msg);  // automation by cc messages 
				presetzModel.processCC(msg, &processedMidi);
			}
			else
			{
				if (msg.isMidiMachineControlMessage() && override)
				{   // only respond to this if overridden
					auto command = msg.getMidiMachineControlCommand();
					switch (command)
					{
					case juce::MidiMessage::mmc_stop:
						presetzModel.setRunState(Topiary::Ending);
						runState = Topiary::Ending;
						break;
					case juce::MidiMessage::mmc_play:
						if (waitFFN)
						{
							presetzModel.setRunState(Topiary::Armed);
							runState = Topiary::Armed;
						}
						else
						{
							presetzModel.setRunState(Topiary::Running);
							runState = Topiary::Running;
						}
						break;
					case juce::MidiMessage::mmc_deferredplay:
						break;
					case juce::MidiMessage::mmc_fastforward:
						break;
					case juce::MidiMessage::mmc_rewind:
						break;
					case juce::MidiMessage::mmc_recordStart:
						presetzModel.Log("Host sent RECORD NOTE DONE YET", Topiary::LogType::Warning);
						break;
					case juce::MidiMessage::mmc_recordStop:
						break;
					case juce::MidiMessage::mmc_pause:
						break;
					default:
						break;
					} // switch MMC command				
				} // incoming MCC commands
			}
			
		}

		if (notePassThrough)
		{
			// pass through anything that came in
			processedMidi.addEvent(msg, (int)msg.getTimeStamp());
			if (logMidiOut) 
				presetzModel.logMidi(false, msg);
			
		}

		if (logMidiIn)
			presetzModel.logMidi(true, msg);

	} // for loop over incoming midiBuffer 

	////////////////////////////////////
	// start generating new Midi data
	////////////////////////////////////

	//Logger::outputDebugString(String(">>>>>>>>>>>>>>>>>>>Processor ready to check switchVariations"));

	if (runState == Topiary::Running) 
	{
		bool wasRunning = (runState == Topiary::Running);
		//Logger::outputDebugString(String(">>>>>>>>>>>>>>>>>>>Processor ready to check switchVariations Running ")+String((int)wasRunning));
		if (presetzModel.switchingVariations())
		{
			// new variation selected but old one still running; only do this when really running!
			presetzModel.processVariationSwitch();
			if (!presetzModel.switchingVariations())  // for now this one does the notification
			{
				// meaning we just did the switch
				// do any remaining notes off
				
				if (wasRunning)
					presetzModel.endNotesOn(&processedMidi);
			}
		}
	}

	if (runState == Topiary::Ending)
	{
		if (presetzModel.processEnding())
		{
			presetzModel.endNotesOn(&processedMidi);
			runState = Topiary::Stopped; // already set to Stopped in the model by processEnding if true; so no need to do that again
		}
	}

	//Logger::outputDebugString(String(">>>>>>>>>>>>>>>>>>>Processor ready to run Running ") + String((int)runState));

	if ((runState == Topiary::Running) || (runState == Topiary::Ending))
	{
		presetzModel.generateMidi(&processedMidi);
	}

	midiMessages.swapWith(processedMidi);

} // processBlock

/////////////////////////////////////////////////////////////////////////

bool TopiaryPresetzAudioProcessor::hasEditor() const
{
	return true;
} // hasEditor

AudioProcessorEditor* TopiaryPresetzAudioProcessor::createEditor()
{
	return new TopiaryPresetzAudioProcessorEditor(*this);
}  // createEditor

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	presetzModel.saveStateToMemoryBlock(destData);
} // getStateInformation

void TopiaryPresetzAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
	presetzModel.restoreStateFromMemoryBlock(data, sizeInBytes);
} // setStateInformation

/////////////////////////////////////////////////////////////////////////

TopiaryPresetzModel* TopiaryPresetzAudioProcessor::getModel()
{
	return (&presetzModel);
} // getModel

/////////////////////////////////////////////////////////////////////////
// This creates new instances of the plugin.

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new TopiaryPresetzAudioProcessor();
}  // createPluginFilter