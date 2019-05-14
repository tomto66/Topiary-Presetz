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
#include"../../Topiary/Source/TopiaryModel.cpp.h"

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
	
	addIntToModel(parameters, variationSwitchChannel, "variationSwitchChannel");
	addBoolToModel(parameters, ccVariationSwitching, "ccVariationSwitching");

	for (int i = 0; i < 8; i++) 
	{
		// addIntToModel(parameters, variation[i].lenInTicks, "lenInTicks", i); not needed; len is always measure!
		addStringToModel(parameters, variation[i].name, "variationName", i);
		addBoolToModel(parameters, variation[i].enabled, "variationEnabled", i);
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
	Log(String("VST PlugIn Technology by Steinberg Media Technologies."), Topiary::LogType::License);
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
		variation[v].lenInTicks = Topiary::TicksPerQuarter * numerator; // always 1 measure long 
		variation[v].type = Topiary::VariationTypeEnd;  // indicates this pattern  only runs once; needed in generateMidi (otherwise it will loop)
		variation[v].timing = Topiary::Quantization::Immediate;
		variationSwitch[v] = ccStart;
		variation[v].currentPatternChild = 0;
		
		ccStart++;

		for (int i = 0; i < PRESETELEMENTS; i++)
		{
			variation[v].presetValue[i] = 63;
		}	

		variation[v].pattern.patLenInTicks = Topiary::TicksPerQuarter * 8; //just to be safe :)
		variation[v].pattern.numItems = 0;
	}

	variationSelected = 0;
	variationRunning = 0;
	
} // TopiaryPresetzModel

//////////////////////////////////////////////////////////////////////////////////////////////////////

TopiaryPresetzModel::~TopiaryPresetzModel()
{
	
} //~TopiaryPresetzModel

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::setRTValue(int p, int value, int channel, int CC)
{
	jassert((p >= 0) && (p < PRESETELEMENTS));
	presetRTValue[p] = value;

	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);
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
	
	for(int i=0; i<PRESETELEMENTS; i++)
	{
		if (presetDefinition[i].enabled)
			if ((presetDefinition[i].inCC == msg.getControllerNumber()) && ((presetDefinition[i].inChannel == 0) || (presetDefinition[i].inChannel == msg.getChannel())))
			{
				int value = msg.getControllerValue();
				// recalculate value based on from/to values (interpolate)

				value = (int)(presetDefinition[i].fromValue + (value * (presetDefinition[i].toValue - presetDefinition[i].fromValue + 1) / 128));
				
				presetRTValue[i] = value;
				broadcaster.sendActionMessage(MsgRealTimeParameter);
				
				
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
		// generateTransition();
	}
	

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
		variation[v].lenInTicks = Topiary::TicksPerQuarter * numerator;
		broadcaster.sendActionMessage(MsgVariationEnables);  // may need to update the enable buttons
		Log("Variation " + String(v+1) + " saved.", Topiary::LogType::Info);
	};
} // setVariation

//////////////////////////////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::initializeVariationsForRunning()
{
	// careful; this code is different in various plugins - don't think it's generic!!!
	for (int v = 0; v < 8; v++)
	{
		variation[v].ended = true;
		variation[v].currentPatternChild = 0;
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
		
	variation[variationSelected].pattern.numItems = 0;

	int transitionTiming = variation[variationSelected].timing;

	int length = 0;

	if (transitionTiming == Topiary::Quantization::Measure)
		length = Topiary::TicksPerQuarter * numerator;
	if (transitionTiming == Topiary::Quantization::Half)
		length = Topiary::TicksPerQuarter * 2;
	if (transitionTiming == Topiary::Quantization::Quarter)
		length = Topiary::TicksPerQuarter;
	if (transitionTiming == Topiary::Quantization::Immediate)
		length = (int) Topiary::TicksPerQuarter/16;

	jassert((transitionTiming == Topiary::Quantization::Measure) || (transitionTiming == Topiary::Quantization::Quarter) || (transitionTiming == Topiary::Quantization::Half)  || (transitionTiming == Topiary::Quantization::Immediate));
	// others not implemented (yet)
	
	for (int p = 0; p < PRESETELEMENTS; p++)
	{
		generateTransition(p, variationSelected, length);
	}

	// sort the children under transition
	variation[variationSelected].pattern.renumber(); 
	variation[variationSelected].ended = false;
	
} // generateTransition

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::outputVariationEvents()
{
	// potentially generate events when variation button is pressed (outside of running) - certainly needed for presetz
	// called when a variation is clicked on outside of running - pre-dest the preset values in modelEventBuffer so processor will pick it up

	jassert(runState != Topiary::Running);
	// the caller is setVariation whic has the model lock, so this is safe
	
	MidiMessage msg;
	modelEventBuffer.clear();
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

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::copyVariation(int from, int to)
{
	jassert((from < 8) && (from >= 0));
	jassert((to < 8) && (to >= 0));
	jassert(false);
	
	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);

	variation[to].enabled = variation[from].enabled;
	variation[to].name = variation[from].name + String(" copy");
	variation[to].lenInTicks = variation[from].lenInTicks;
	variation[to].currentPatternChild = 0;

	variation[to].timing = variation[from].timing;
	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		variation[to].presetValue[i] = variation[from].presetValue[i];
	}
	
	broadcaster.sendActionMessage(MsgMaster);
	broadcaster.sendActionMessage(MsgVariationEnables);
	Log("Variation " + String(from+1) + " copied to " + String(to+1) + ".", Topiary::LogType::Info);
	
} // copyVariation

///////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::swapVariation(int from, int to)
{
	jassert((from < 8) && (from >= 0));
	jassert((to < 8) && (to >= 0));
	jassert(false);
	
	bool rEnabled;
	String rName;
	int rLenInTicks;
	int rCurrentPatternChild;
	int rTiming;
	int rPresetValue[PRESETELEMENTS];

	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);

	rEnabled = variation[from].enabled;
	rName = variation[from].name;
	rLenInTicks = variation[from].lenInTicks;
	rCurrentPatternChild = variation[from].currentPatternChild;

	rTiming = variation[from].timing;
	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		rPresetValue[i] = variation[from].presetValue[i];
	}

	variation[from].enabled = variation[to].enabled;
	variation[from].name = variation[to].name;
	variation[from].lenInTicks = variation[to].lenInTicks;
	variation[from].currentPatternChild = variation[to].currentPatternChild;

	variation[from].timing = variation[to].timing;
	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		variation[from].presetValue[i] = variation[to].presetValue[i];
	}

	variation[to].enabled = rEnabled;
	variation[to].name = rName;
	variation[to].lenInTicks = rLenInTicks;
	variation[to].currentPatternChild = rCurrentPatternChild;

	variation[to].timing = rTiming;
	for (int i = 0; i < PRESETELEMENTS; i++)
	{
		variation[to].presetValue[i] = rPresetValue[i];
	}

	broadcaster.sendActionMessage(MsgMaster);
	broadcaster.sendActionMessage(MsgVariationEnables);
	Log("Variation " + String(from+1) + " swapped with " + String(to+1) + ".", Topiary::LogType::Info);
	
} // swapVariation

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::copyPreset(int from, int to)
{
	jassert((from < PRESETELEMENTS) && (from >= 0));
	jassert((to < PRESETELEMENTS) && (to >= 0));
	jassert(false);
	
	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);

	presetDefinition[to].name = presetDefinition[from].name + String(" copy");
	presetDefinition[to].fromValue = presetDefinition[from].fromValue;
	presetDefinition[to].toValue = presetDefinition[from].toValue;
	presetDefinition[to].inCC = presetDefinition[from].inCC;
	presetDefinition[to].inChannel = presetDefinition[from].inChannel;
	presetDefinition[to].outCC = presetDefinition[from].outCC;
	presetDefinition[to].outChannel = presetDefinition[from].outChannel;
	presetDefinition[to].enabled = presetDefinition[from].enabled;

	// also copy the preset values in the variations !!!
	for (int v = 0; v < 8; v++)
	{
		variation[v].presetValue[to] = variation[v].presetValue[from];
	}

	broadcaster.sendActionMessage(MsgMaster);
	Log("Preset " + String(from+1) + " copied to " + String(to+1) + ".", Topiary::LogType::Info);
	
} // copyPreset

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::swapPreset(int from, int to)
{
	jassert((from < 8) && (from >= 0));
	jassert((to < 8) && (to >= 0));

	String rName;
	int rFromValue = 0;
	int rToValue = 127;
	int rInCC = 0;
	int rInChannel = 0;
	int rOutCC = 0;
	int rOutChannel;
	bool rEnabled;

	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);

	rName = presetDefinition[to].name;
	rFromValue = presetDefinition[to].fromValue;
	rToValue = presetDefinition[to].toValue;
	rInCC = presetDefinition[to].inCC;
	rInChannel = presetDefinition[to].inChannel;
	rOutCC = presetDefinition[to].outCC;
	rOutChannel = presetDefinition[to].outChannel;
	rEnabled = presetDefinition[to].enabled;

	presetDefinition[to].name = presetDefinition[from].name;
	presetDefinition[to].fromValue = presetDefinition[from].fromValue;
	presetDefinition[to].toValue = presetDefinition[from].toValue;
	presetDefinition[to].inCC = presetDefinition[from].inCC;
	presetDefinition[to].inChannel = presetDefinition[from].inChannel;
	presetDefinition[to].outCC = presetDefinition[from].outCC;
	presetDefinition[to].outChannel = presetDefinition[from].outChannel;
	presetDefinition[to].enabled = presetDefinition[from].enabled;

	presetDefinition[from].name = rName;
	presetDefinition[from].fromValue = rFromValue;
	presetDefinition[from].toValue = rToValue;
	presetDefinition[from].inCC = rInCC;
	presetDefinition[from].inChannel = rInChannel;
	presetDefinition[from].outCC = rOutCC;
	presetDefinition[from].outChannel = rOutChannel;
	presetDefinition[from].enabled = rEnabled;

	// also swap the preset values in the variations !!!
	for (int v = 0; v < 8; v++)
	{
		int r;
		r = variation[v].presetValue[to];
		variation[v].presetValue[to] = variation[v].presetValue[from];
		variation[v].presetValue[from] = r;
	}

	broadcaster.sendActionMessage(MsgMaster);
	Log("Preset " + String(from+1) + " swapped with " + String(to+1) + ".", Topiary::LogType::Info);

} // swapPreset

/////////////////////////////////////////////////////////////////////////////

bool TopiaryPresetzModel::midiLearn(MidiMessage m)
{
	// called by processor; if midiLearn then learn based on what came in
	const GenericScopedLock<CriticalSection> myScopedLock(lockModel);
	bool remember = learningMidi;
	if (learningMidi)
	{
		bool note = m.isNoteOn();
		bool cc = m.isController();

		if (note || cc)
		{
			// check the Id to learn; tells us what to set
			if ((midiLearnID >= Topiary::LearnMidiId::variationSwitch) && (midiLearnID < (Topiary::LearnMidiId::variationSwitch+8)))
			{
				// learning variation switch
				if (note)
				{
					ccVariationSwitching = false;
					variationSwitch[midiLearnID] = m.getNoteNumber();
				}
				else
				{
					ccVariationSwitching = true;
					variationSwitch[midiLearnID] = m.getControllerNumber();
				}
				learningMidi = false;
				Log("Midi learned", Topiary::LogType::Warning);
				broadcaster.sendActionMessage(MsgVariationAutomation);	// update utility tab
			}
			else if ((midiLearnID >= Topiary::LearnMidiId::presetMidiCin) && (midiLearnID < (Topiary::LearnMidiId::presetMidiCin + 8)))
			{

				if (cc)
				{
					presetDefinition[midiLearnID- Topiary::LearnMidiId::presetMidiCin].inCC = m.getControllerNumber();
					learningMidi = false;
					Log("Midi learned", Topiary::LogType::Warning);
					broadcaster.sendActionMessage(MsgMaster);	// update master
				}
			}
			
		}
	}

	return remember;
} // midiLearn

/////////////////////////////////////////////////////////////////////////////

void TopiaryPresetzModel::getVariationNames(String vNames[8])
{
	for (int v = 0; v < 8; v++)
	{
		vNames[v] = variation[v].name;
	}
} // getVariationNames


/////////////////////////////////////////////////////////////////////////////
#include "../../Topiary/Source/TopiaryMidiLearnEditor.cpp.h"