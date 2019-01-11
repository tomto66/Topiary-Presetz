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

#pragma once
#include "TopiaryPresetzModel.h"

// following has std model code that can be included (cannot be in TopiaryModel because of variable definitions)
#define TOPIARYMODEL TopiaryPresetzModel
#include"../../Topiary/Source/TopiaryModelIncludes.cpp"

void TopiaryPresetzModel::saveStateToMemoryBlock(MemoryBlock& destData)
{
	addParametersToModel();  // this adds and XML element "Parameters" to the model
	AudioProcessor::copyXmlToBinary(*model, destData);
	model->deleteAllChildElementsWithTagName("Parameters");
}

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::restoreStateFromMemoryBlock(const void* data, int sizeInBytes)
{
	//return; // XXXXXXXXX
	model.reset(AudioProcessor::getXmlFromBinary(data, sizeInBytes));
	restoreParametersToModel();

} // restoreStateFromMemoryBlock

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::addParametersToModel()
{
	auto parameters = new XmlElement("Parameters");
	model->addChildElement(parameters);

	addStringToModel(parameters, name, "name");
	addIntToModel(parameters, BPM, "BPM");
	addIntToModel(parameters, numerator, "numerator");
	addIntToModel(parameters, denominator, "denominator");
	addIntToModel(parameters, variationSelected, "variationSelected");
	addIntToModel(parameters, switchVariation, "switchVariation");
	addIntToModel(parameters, runStopQ, "runStopQ");
	addIntToModel(parameters, variationStartQ, "variationStartQ");
	addBoolToModel(parameters, WFFN, "WFFN");
	addBoolToModel(parameters, overrideHostTransport, "overrideHostTransport");
	addBoolToModel(parameters, notePassThrough, "notePassThrough");

	addBoolToModel(parameters, logMidiIn, "logMidiIn");
	addBoolToModel(parameters, logMidiOut, "logMidiOut");
	addBoolToModel(parameters, logDebug, "logDebug");
	addBoolToModel(parameters, logTransport, "logTransport");
	addBoolToModel(parameters, logVariations, "logVariations");
	addBoolToModel(parameters, logInfo, "logInfo");
	addStringToModel(parameters, filePath, "filePath");
	addIntToModel(parameters, numPatterns, "numPatterns");
	
	addIntToModel(parameters, variationSwitchChannel, "variationSwitchChannel");
	addBoolToModel(parameters, ccVariationSwitching, "ccVariationSwitching");

	for (int i = 0; i < 8; i++) 
	{
		// addIntToModel(parameters, variation[i].lenInTicks, "lenInTicks", i); not needed; len is always measure!
		addStringToModel(parameters, variation[i].name, "variationName", i);
		addBoolToModel(parameters, variation[i].enabled, "variationEnabled", i);
		addBoolToModel(parameters, variation[i].ending, "variationEnding", i);
		addIntToModel(parameters, variation[i].timing, "variationTiming", i);
		for (int j = 0; j < PRESETELEMENTS; j++)
		{
			addIntToModel(parameters, variation[i].presetValue[j], "presetValue", i,j);
		}

		// automation
		addIntToModel(parameters, variationSwitch[i], "variationSwitch", i);
	}

	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		// preset definitions
		addStringToModel(parameters, presetDefinition[i].name, "presetName", i);
		addIntToModel(parameters, presetDefinition[i].inCC, "presetInCC", i);
		addIntToModel(parameters, presetDefinition[i].inChannel, "presetInChannel", i);
		addIntToModel(parameters, presetDefinition[i].fromValue, "presetFrom", i);
		addIntToModel(parameters, presetDefinition[i].outChannel, "presetOutChannel", i);
		addIntToModel(parameters, presetDefinition[i].outCC, "presetOutCC", i);
		addIntToModel(parameters, presetDefinition[i].fromValue, "presetFrom", i);
		addIntToModel(parameters, presetDefinition[i].toValue, "presetTo", i);
		addBoolToModel(parameters, presetDefinition[i].enabled, "presetEnabled", i);
	}
} // addParametersToModel

/////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::savePreset(File f)
{
	addParametersToModel();  // this adds and XML element "Parameters" to the model
	String myXmlDoc = model->createDocument(String());
	f.replaceWithText(myXmlDoc);
	//Logger::writeToLog(myXmlDoc);

	// now delete the no-longer-needed "Parameters" child
	model->deleteAllChildElementsWithTagName("Parameters");

} // savePreset

//////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::loadPreset(File f)
{
	model.reset(XmlDocument::parse(f));
	restoreParametersToModel();
} // loadPreset

//////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::restoreParametersToModel()
{ 
	
	// model now has a child "Parameters"; set all non-XML parameters to their new values

	auto child = model->getFirstChildElement();
	
	while (child != nullptr)
	{
		String tagName = child->getTagName();

		if (tagName.compare("Parameters") == 0)
		{
			auto parameter = child->getFirstChildElement();
			while (parameter != nullptr)
			{
				String parameterName = parameter->getStringAttribute("Name");
				if (parameterName.compare("name") == 0)	name = parameter->getStringAttribute("Value");
				if (parameterName.compare("BPM") == 0) BPM = parameter->getIntAttribute("Value");
				if (parameterName.compare("numerator") == 0) numerator = parameter->getIntAttribute("Value");
				if (parameterName.compare("overrideHostTransport") == 0) overrideHostTransport = (bool)parameter->getIntAttribute("Value");
				if (parameterName.compare("denominator") == 0) denominator = parameter->getIntAttribute("Value");
				if (parameterName.compare("variationSelected") == 0)
				{
					variationSelected = parameter->getIntAttribute("Value");
					variationRunning = variationSelected;
				}

				if (parameterName.compare("switchVariation") == 0) switchVariation = parameter->getIntAttribute("Value");
				if (parameterName.compare("runStopQ") == 0) runStopQ = parameter->getIntAttribute("Value");
				if (parameterName.compare("variationStartQ") == 0) variationStartQ = parameter->getIntAttribute("Value");
				if (parameterName.compare("name") == 0) name = parameter->getStringAttribute("Value");

				if (parameterName.compare("WFFN") == 0)	WFFN = parameter->getBoolAttribute("Value");
				if (parameterName.compare("notePassThrough") == 0) 	notePassThrough = (bool)parameter->getIntAttribute("Value");

				if (parameterName.compare("logMidiIn") == 0) logMidiIn = parameter->getBoolAttribute("Value");
				if (parameterName.compare("logMidiOut") == 0)	logMidiOut = parameter->getBoolAttribute("Value");
				if (parameterName.compare("logDebug") == 0)	logDebug = parameter->getBoolAttribute("Value");
				if (parameterName.compare("logTransport") == 0)	logTransport = parameter->getBoolAttribute("Value");
				if (parameterName.compare("logVariations") == 0) logVariations = parameter->getBoolAttribute("Value");
				if (parameterName.compare("logInfo") == 0)	logInfo = parameter->getBoolAttribute("Value");

				if (parameterName.compare("filePath") == 0)	filePath = parameter->getStringAttribute("Value");
				if (parameterName.compare("numPatterns") == 0) numPatterns = parameter->getIntAttribute("Value");
				
				// variations 
				if (parameterName.compare("variationName") == 0) variation[parameter->getIntAttribute("Index")].name = parameter->getStringAttribute("Value");
				
				if (parameterName.compare("variationEnabled") == 0) variation[parameter->getIntAttribute("Index")].enabled = parameter->getBoolAttribute("Value");
				if (parameterName.compare("variationTiming") == 0) variation[parameter->getIntAttribute("Index")].timing = parameter->getIntAttribute("Value");

				if (parameterName.compare("presetValue") == 0) variation[parameter->getIntAttribute("Index1")].presetValue[parameter->getIntAttribute("Index2")] = parameter->getIntAttribute("Value");
						
				// presets
				if (parameterName.compare("presetName") == 0) presetDefinition[parameter->getIntAttribute("Index")].name = parameter->getStringAttribute("Value");
				if (parameterName.compare("presetInCC") == 0) presetDefinition[parameter->getIntAttribute("Index")].inCC = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetInChannel") == 0) presetDefinition[parameter->getIntAttribute("Index")].inChannel = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetOutCC") == 0) presetDefinition[parameter->getIntAttribute("Index")].outCC = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetOutChannel") == 0) presetDefinition[parameter->getIntAttribute("Index")].outChannel = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetFrom") == 0) presetDefinition[parameter->getIntAttribute("Index")].fromValue = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetTo") == 0) presetDefinition[parameter->getIntAttribute("Index")].toValue = parameter->getIntAttribute("Value");
				if (parameterName.compare("presetEnabled") == 0) presetDefinition[parameter->getIntAttribute("Index")].enabled = parameter->getBoolAttribute("Value");

				// automation
				if (parameterName.compare("variationSwitch") == 0)  variationSwitch[parameter->getIntAttribute("Index")] = parameter->getIntAttribute("Value");
				if (parameterName.compare("ccVariationSwitching") == 0)  ccVariationSwitching = (bool)parameter->getIntAttribute("Value");
				if (parameterName.compare("variationSwitchChannel") == 0)  variationSwitchChannel = parameter->getIntAttribute("Value");

				parameter = parameter->getNextElement();
			}
			break;
		}
		

		child = child->getNextElement();

	} // foreach parameters

	
	// now delete the no-longer-needed "Parameters" child
	model->deleteAllChildElementsWithTagName("Parameters");


	// inform editor
	broadcaster.sendActionMessage(MsgTransport);
	broadcaster.sendActionMessage(MsgLog);
	broadcaster.sendActionMessage(MsgMaster);
	broadcaster.sendActionMessage(MsgVariationEnables);		// so that if needed variationbuttons are disabled/enabled
	broadcaster.sendActionMessage(MsgVariationAutomation);	// inform editor of variation automation settings;

	setRunState(Topiary::Stopped);

	// make sure that any variation-specific stuff is sent out if needed (needed for presetz e.g.
	outputVariationEvents(); 
	

} // restoreParametersToModel

//////////////////////////////////////////////////////////////////////////////////////////////////

TopiaryPresetzModel::TopiaryPresetzModel()
{
	model.reset(new XmlElement("presetzModel"));
	filePath = "";
	variationStartQ = Topiary::Quantization::Immediate;
	switchVariation = Topiary::VariationSwitch::SwitchFromStart;

	numPatterns = 1; // needed otherwise transport won't run - later on there will be a generated pattern to store the transition

	logString = "";
	Log(String("Topiary Presetz V ") + String(xstr(JucePlugin_Version)) + String(" (c) Tom Tollenaere 2018-2019."), Topiary::LogType::License);
	Log(String(""), Topiary::LogType::License);
	Log(String("Topiary Presetz is free software : you can redistribute it and/or modify"), Topiary::LogType::License);
	Log(String("it under the terms of the GNU General Public License as published by"), Topiary::LogType::License);
	Log(String("the Free Software Foundation, either version 3 of the License, or"), Topiary::LogType::License);
	Log(String("(at your option) any later version."), Topiary::LogType::License);
	Log(String(""), Topiary::LogType::License);
	Log(String("Topiary Presetz is distributed in the hope that it will be useful,"), Topiary::LogType::License);
	Log(String("but WITHOUT ANY WARRANTY; without even the implied warranty of"), Topiary::LogType::License);
	Log(String("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the"), Topiary::LogType::License);
	Log(String("GNU General Public License for more details."), Topiary::LogType::License);
	Log(String(""), Topiary::LogType::License);

	overrideHostTransport = false;
	runState = Topiary::Stopped;
	BPM = 120;
	numerator = 4; denominator = 4;
	runStopQ = Topiary::Quantization::Immediate;			// not in editor, make sure these are set correctly	
	

	// initialization of definitions
	int ccStart = 102;
	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		presetDefinition[i].name = "Parameter " + String(i+1);
		presetDefinition[i].fromValue = 0;
		presetDefinition[i].toValue = 127;
		presetDefinition[i].enabled = false;
		presetDefinition[i].outCC = ccStart;
		presetDefinition[i].outChannel = 1;
		presetDefinition[i].inCC = ccStart;
		presetDefinition[i].inChannel = 1;
		presetRTValue[i] = 63;   // these are the values in realTime; they will be set to variation[v].presetvalue[i] when a variation is selected
		                         // or when a cc value is received from the host
		ccStart++;
	}

	ccStart = 22;  // for automation
	for (int v = 0; v < 8; v++)
	{
		variation[v].name = "---";
		variation[v].enabled = false;
		variation[v].lenInTicks = 1; // unused for now
		variation[v].lenInTicks = Topiary::TICKS_PER_QUARTER * numerator; // always 1 measure long 
		variation[v].ending = true;  // indicates this pattern  only runs once; needed in generateMidi (otherwise it will loop)
		variation[v].timing = Topiary::Quantization::Immediate;
		variationSwitch[v] = ccStart;
		variation[v].currentPatternChild = nullptr;
		variation[v].pattern = new XmlElement("Transition");
		ccStart++;

		for (int i = 0; i < PRESETELEMENTS; i++)
		{
			variation[v].presetValue[i] = 63;
		}	
	}

	variationSelected = 0;
	variationRunning = 0;
	topiaryThread.notify();
	
} // TopiaryPresetzModel

//////////////////////////////////////////////////////////////////////////////////////////////////////

TopiaryPresetzModel::~TopiaryPresetzModel()
{
	XmlElement toDelete("toDelete");

	for (int i = 0; i < 8; ++i)
	{
		// ensure these will get destroyed
		toDelete.addChildElement(variation[i].pattern);

	}
	
} //~TopiaryPresetzModel

//////////////////////////////////////////////////////////////////////////////////////////////////////
/* moved to modelIncludes
void TopiaryPresetzModel::getVariationDetailForGenerateMidi(XmlElement** parent, XmlElement** noteChild, int& parentLength, bool& ending, bool& ended)
{
	parentLength = variation[variationRunning].lenInTicks;
	ending = variation[variationRunning].ending;
	ended = variation[variationRunning].ended;
	*parent = variation[variationRunning].pattern;
	*noteChild = variation[variationRunning].currentPatternChild;

} // getVariationDetailForGenerateMidi
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::setRTValue(int p, int value, int channel, int CC)
{
	jassert((p >= 0) && (p < PRESETELEMENTS));
	presetRTValue[p] = value;

	const GenericScopedLock<SpinLock> myScopedLock(lockModel);
	// output a CC value!!
	MidiMessage msg;
	msg = MidiMessage::controllerEvent(channel, CC, value);
	modelEventBuffer.addEvent(msg, 0);
	logMidi(false,msg);

}  // setRTValue

//////////////////////////////////////////////////////////////////////////////////////////////////////

int TopiaryPresetzModel::getRTValue(int p)
{
	jassert((p >= 0) && (p < PRESETELEMENTS));
	return presetRTValue[p];

} // getRTValue

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::processCC(MidiMessage& msg, MidiBuffer* midiBuffer) // different version below - this one is called by pluginProcessor
{
	// do the realtime CC processing done in the pluginProcessor
	// check if the CC number is one of the enabled RT controllers; if so check from/to, update the RTvalue and send msg to the preset to update the slider
	// then send out the midimessage according to the preset definition
	
	//int outCCs[PRESETELEMENTS]; // keep track of which CCs we processed

	for(int i=0; i<PRESETELEMENTS; i++)
	{
		if (presetDefinition[i].enabled)
			if ((presetDefinition[i].inCC == msg.getControllerNumber()) && ((presetDefinition[i].inChannel == 0) || (presetDefinition[i].inChannel == msg.getChannel())))
			{
				int value = msg.getControllerValue();
				if ((value >= presetDefinition[i].fromValue) && (value <= presetDefinition[i].toValue))
				{
					presetRTValue[i] = value;
					broadcaster.sendActionMessage(MsgRealTimeParameter);
				}
				
				auto outmsg = MidiMessage::controllerEvent(presetDefinition[i].outChannel, presetDefinition[i].outCC, presetRTValue[i]);
				midiBuffer->addEvent(outmsg, (int) msg.getTimeStamp());
				logMidi(false, outmsg);
		}
	}

} // processCC

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::processCC(MidiMessage& msg)  // this version to be called by generateMidi (no midibuffer in the header)
{
	// do the realtime CC processing, called by generateMidi
	// check if the CC number is one of the enabled RT controllers; if so check from/to, update the RTvalue and send msg to the preset to update the slider

	//int outCCs[PRESETELEMENTS]; // keep track of which CCs we processed

	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		if (presetDefinition[i].enabled)
			if ((presetDefinition[i].outCC == msg.getControllerNumber()) && (presetDefinition[i].outChannel == msg.getChannel()))
			{
				int value = msg.getControllerValue();
				if ((value >= presetDefinition[i].fromValue) && (value <= presetDefinition[i].toValue))
				{
					presetRTValue[i] = value;
					broadcaster.sendActionMessage(MsgRealTimeParameter);
				}

			}
	}

} // processCC

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::getPresetDefinition(int p, String &pname, int &fromValue, int &toValue, int &inCC, int &inChannel, int& outCC, int &outChannel, bool &enabled)
{
	pname = presetDefinition[p].name;
	fromValue = presetDefinition[p].fromValue;
	toValue = presetDefinition[p].toValue;
	inCC = presetDefinition[p].inCC;
	inChannel = presetDefinition[p].inChannel;
	outCC = presetDefinition[p].outCC;
	outChannel = presetDefinition[p].outChannel;
	enabled = presetDefinition[p].enabled;

} // getPresetDefinition

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::setPresetDefinition(int p, String pname, int fromValue, int toValue, int inCC, int inChannel, int outCC, int outChannel, bool enabled)
{
	if ((presetDefinition[p].name != pname) || (presetDefinition[p].fromValue != fromValue) 
		|| (presetDefinition[p].toValue != toValue)  || (presetDefinition[p].inCC != inCC) || (presetDefinition[p].inChannel != inChannel) || 
												(presetDefinition[p].outCC != outCC) || (presetDefinition[p].outChannel != outChannel) ||	(presetDefinition[p].enabled != enabled))
	{
		presetDefinition[p].name = pname;
		presetDefinition[p].fromValue = fromValue;
		presetDefinition[p].toValue = toValue;
		presetDefinition[p].inCC = inCC;
		presetDefinition[p].inChannel = inChannel;
		presetDefinition[p].outCC = outCC;
		presetDefinition[p].outChannel = outChannel;
		presetDefinition[p].enabled = enabled;
		Log("Preset " + String(p+1) + " saved.", Topiary::LogType::Info);
	}
	generateTransition(); 

} // setPresetDefinition

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::getVariationDefinition(int v, bool &enabled, String &vname,  int presetVal[PRESETELEMENTS], int& timing)
{
	enabled = variation[v].enabled;
	vname = variation[v].name;
	timing = variation[v].timing;

	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		presetVal[i] = variation[v].presetValue[i];
	}
} // getVariation

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::setVariationDefinition(int v, bool enabled, String vname,  int presetVal[PRESETELEMENTS], int timing)
{
	bool saved = false;

	if (variation[v].timing != timing)
	{
		variation[v].timing = timing;
		saved = true;
	};

	if (variation[v].enabled != enabled)
	{
		variation[v].enabled = enabled;
		saved = true;
	};

	if (vname.compare(variation[v].name) != 0)
	{
		variation[v].name = vname;
		saved = true;
	};

	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		if (variation[v].presetValue[i] != presetVal[i])
		{
			variation[v].presetValue[i] = presetVal[i];
			saved = true;
		}
	};
	
	if (saved)
	{
		variation[v].lenInTicks = Topiary::TICKS_PER_QUARTER * numerator;
		broadcaster.sendActionMessage(MsgVariationEnables);  // may need to update the enable buttons
		Log("Variation " + String(v+1) + " saved.", Topiary::LogType::Info);
	};
} // setVariation

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::getVariation(int& running, int& selected)
{
	running = variationRunning;
	selected = variationSelected;
	return;

} // getVariation

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::setVariation(int n)
{
	jassert(n < 8);
	jassert(n >= 0);
	const GenericScopedLock<SpinLock> myScopedLock(lockModel);

	if (n != variationSelected)
		if (variation[n].enabled)
		{
			variationSelected = n;
			if (runState == Topiary::Stopped)  // otherwise the switch will be done when running depending on the variation switch Q
				variationRunning = n;
			Log(String("Variation ") + String(n+1) + String(" selected."), Topiary::LogType::Variations);
			broadcaster.sendActionMessage(MsgVariationSelected);
		}

	if ((runState != Topiary::Running) && variation[variationSelected].enabled) 
		// we ALWAYS do this - even if the variation does not change (because we may wanna hit the variation button to reset it (presetz)
		// so even if WFFN is on - as long as we are waiting wo do an immediate output of the variation settings!
		outputVariationEvents(); // output the variation preset values!

} // setVariation

///////////////////////////////////////////////////////////////////////
/* move to modelIncludes
void TopiaryPresetzModel::getVariationEnables(bool enables[8])
{
	for (int i = 0; i < 8; i++)
		enables[i] = variation[i].enabled;

} // getVariationEnables

///////////////////////////////////////////////////////////////////////

int TopiaryPresetzModel::getVariationLenInTicks(int v)
{
	return variation[v].lenInTicks;

} // getVariationLenInTicks

///////////////////////////////////////////////////////////////////////

bool TopiaryPresetzModel::getVariationEnabled(int v)
{
	return variation[v].enabled;
	
} // getVariationEnabled
*/
///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::initializeVariationsForRunning()
{
	// careful; this code is different in various plugins - don't think it's generic!!!
	for (int v = 0; v < 8; v++)
	{
		variation[v].ended = true;
		variation[v].currentPatternChild = nullptr;
	}
} // initializeVariationsForRunning



///////////////////////////////////////////////////////////////////////
// Generator
///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::generateTransition()
{
	// generates the transition (singular!) based on
	// - TransitionSelected (that's the one we do)
	// - Number of preset elements ( PRESETELEMENTS )
	// - transitionSpeed ( Measure, Beat for now )
	// - RTvalues of the controlled elements ( presetRTValue[] )
	// - transitionTiming as defined in the variation
	// Generates locally, swaps at end of generation and swap protected by model spinlock
	// sets generatingTransition in the model to signal we're busy generating
	
	auto transition = new XmlElement("Transition");
	int vSelected = variationSelected;
	
	int transitionTiming = variation[variationSelected].timing;

	int length = 0;

	if (transitionTiming == Topiary::Quantization::Measure)
		length = Topiary::TICKS_PER_QUARTER * numerator;
	if (transitionTiming == Topiary::Quantization::Half)
		length = Topiary::TICKS_PER_QUARTER * 2;
	if (transitionTiming == Topiary::Quantization::Quarter)
		length = Topiary::TICKS_PER_QUARTER;
	if (transitionTiming == Topiary::Quantization::Immediate)
		length = (int) Topiary::TICKS_PER_QUARTER/16;

	jassert((transitionTiming == Topiary::Quantization::Measure) || (transitionTiming == Topiary::Quantization::Quarter) || (transitionTiming == Topiary::Quantization::Half)  || (transitionTiming == Topiary::Quantization::Immediate));
	// others not implemented (yet)
	
	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		generateTransition(p, variationSelected, transition, length);
	}

	// sort the children under transition
	renumberByTimestamp(transition); 

	//Logger::writeToLog("-------------------- TRANSITION GENERATED ---------------------");
	//String myXmlDoc2 = transition->createDocument(String());
	//Logger::writeToLog(myXmlDoc2);
	//Logger::writeToLog("Transition from "+String(transitioningFrom)+" to "+String(transitioningTo)+" generated.");

	// set spinlock
	const GenericScopedLock<SpinLock> myScopedLock(lockModel);

	swapXmlElementPointers(&(variation[vSelected].pattern), &transition);
	variation[vSelected].currentPatternChild = nullptr;
	delete transition;
	transitioningFrom = -1;
	transitioningTo = -1;
	threadRunnerState = Topiary::ThreadRunnerState::DoneGenerating;
	// free spinLock
	
} // generateTransition

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::threadRunner()
{
	if (variationRunning == variationSelected) return; // nothing to do (should not be called either)

	// to avoid another transition running; set variation[variationRunning].ended = true (that will stop generateMidi)
	variation[variationRunning].ended = true;
	variation[variationSelected].ended = true;
	//Logger::outputDebugString("RUNNER: set variation[" + String(variationRunning) + "].ended TRUE");

	generateTransition();  // this one sets transitioningFrom and transitioningTo

	//Logger::outputDebugString("RUNNER: set variation{" + String(variationSelected) + "].ended FALSE");
	variation[variationSelected].ended = false; // so it can start running
	variation[variationRunning].ended = false;
	Logger::outputDebugString("RUNNER: Variation ready for transition ------------------------>");

} // threadRunner

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::outputVariationEvents()
{
	// potentially generate events when variation button is pressed (outside of running) - certainly needed for presetz
	// called when a variation is clicked on outside of running - pre-dest the preset values in modelEventBuffer so processor will pick it up

	jassert(runState != Topiary::Running);

	MidiMessage msg;
	
	{
		for (int p = 0; p < PRESETELEMENTS; p++)
		{
			if (presetDefinition[p].enabled)
			{
				msg = MidiMessage::controllerEvent(presetDefinition[p].outChannel, presetDefinition[p].outCC, variation[variationSelected].presetValue[p]);
				modelEventBuffer.addEvent(msg, 0);
				logMidi(false, msg);
				// also do in the model and inform the editor
				presetRTValue[p] = variation[variationSelected].presetValue[p];
				broadcaster.sendActionMessage(MsgRealTimeParameter);  // may need to update the enable buttons
			}
		}
	}
	
} // outputVariationEvents