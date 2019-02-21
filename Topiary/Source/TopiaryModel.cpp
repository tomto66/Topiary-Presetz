/////////////////////////////////////////////////////////////////////////////
/*
This file is part of Topiary, Copyright Tom Tollenaere 2018-2019.

Topiary is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Topiary is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Topiary. If not, see <https://www.gnu.org/licenses/>.
*/
/////////////////////////////////////////////////////////////////////////////

#include "../JuceLibraryCode/JuceHeader.h"
#include "Topiary.h"
#include "TopiaryModel.h"

void TopiaryModel::saveStateToMemoryBlock(MemoryBlock& destData)
{
	// virtual
	UNUSED(destData)

} // saveStateToMemoryBlock

/////////////////////////////////////////////////////////////////////////

void TopiaryModel::restoreStateFromMemoryBlock(const void* data, int sizeInBytes)
{
	// virtual
	UNUSED(data)
	UNUSED(sizeInBytes)
} // restoreStateFromMemoryBlock


void TopiaryModel::addParametersToModel() {} // virtual
void TopiaryModel::restoreParametersToModel() {} // virtual

/////////////////////////////////////////////////////////////////////////

void TopiaryModel::savePreset(String msg, String extension)
{
	File f;
	auto directory = File::getSpecialLocation(File::userHomeDirectory);

	if (filePath.compare("") == 0)
		directory = File::File(filePath);
	FileChooser myChooser(msg, directory, extension);

	if (myChooser.browseForFileToSave(true))
	{
		filePath = f.getParentDirectory().getFullPathName();
		f = myChooser.getResult();
		addParametersToModel();  // this adds and XML element "Parameters" to the model
		String myXmlDoc = model->createDocument(String());
		f.replaceWithText(myXmlDoc);
		//Logger::writeToLog(myXmlDoc);

		// now delete the no-longer-needed "Parameters" child
		model->deleteAllChildElementsWithTagName("Parameters");
	}
} // savePreset

/////////////////////////////////////////////////////////////////////////

void TopiaryModel::loadPreset(String msg, String extension)
{
	File f;
	auto directory = File::getSpecialLocation(File::userHomeDirectory);

	if (filePath.compare("") == 0)
		directory = File::File(filePath);

	FileChooser myChooser(msg, directory, extension);

	if (myChooser.browseForFileToOpen())
	{
		filePath = f.getParentDirectory().getFullPathName();
		f = myChooser.getResult();
		model.reset(XmlDocument::parse(f));
		restoreParametersToModel();
	}

} //loadPreset

/////////////////////////////////////////////////////////////////////////

#define xstr(s) str(s)
#define str(s) #s

TopiaryModel::TopiaryModel()
{
	filePath = "";
	logString = "";
	
	overrideHostTransport = true;
	runState = Topiary::Stopped;
	BPM = 120;
	numerator = 4; denominator = 4;
	numPatterns = 0;
	topiaryThread.setModel(this);
	topiaryThread.startThread(8); // thread should start with a wait - and inherited model should call notify when init is done

} // TopiaryModel

//////////////////////////////////////////////////////////////////////////////////////////////////////

TopiaryModel::~TopiaryModel()
{
	topiaryThread.stopThread(-1);
	runState = Topiary::Stopped;

} //~TopiaryModel

///////////////////////////////////////////////////////////////////////
// VARIATIONS
///////////////////////////////////////////////////////////////////////


int TopiaryModel::getVariationLenInTicks(int v)
{
	// virtual
	UNUSED(v)
	return(0);
}

void TopiaryModel::setVariation(int v)
{
	// virtual
	UNUSED(v)
}

bool TopiaryModel::getVariationEnabled(int v)
{
	// virtual
	UNUSED(v)
	return(false);
}

///////////////////////////////////////////////////////////////////////
// LOGGER
///////////////////////////////////////////////////////////////////////

String* TopiaryModel::getLog()
{
	return &logString;
} // getLog

///////////////////////////////////////////////////////////////////////

void TopiaryModel::Log(String s, int logType)
{
	if (logString.length() > 8000) 
		logString = logString.substring(4000);

	switch (logType)
	{
	case Topiary::LogType::MidiIn:
		if (!logMidiIn) return;
		break;
	case Topiary::LogType::MidiOut:
		if (!logMidiOut) return;
		break;
	case Topiary::LogType::Debug:
		if (!logDebug) return;
		break;
	case Topiary::LogType::Warning:
		if (!logWarning) return;
		break;
	case Topiary::LogType::Info:
		if (!logInfo) return;
		break;
	case Topiary::LogType::Transport:
		if (!logTransport) return;
		break;
	case Topiary::LogType::Variations:
		if (!logVariations) return;
		break;
	case Topiary::LogType::License:
		break;
	default: return;
		break;
	}

	Time t = Time::getCurrentTime();
	char pre[20];
	if (logType == Topiary::LogType::License)
		strcpy(pre, "");
	else
		sprintf(pre, "%02d:%02d:%02d:%03d : ", t.getHours(), t.getMinutes(), t.getSeconds(), t.getMilliseconds());

	logString.append(String(pre) + s + newLine, 250);

	broadcaster.sendActionMessage(MsgLog);
	if (logType == Topiary::LogType::Warning) {
		broadcaster.sendActionMessage(MsgWarning);  // warn the user in the header
		lastWarning = s;
	}
}  // Log

///////////////////////////////////////////////////////////////////////

String TopiaryModel::getLastWarning()
{
	return lastWarning;

} // getLastWarning

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setLogSettings(bool warning, bool midiIn, bool midiOut, bool debug, bool transport, bool variations, bool info)
{
	bool updateNeeded = false;

	if (logWarning != warning)
	{
		logWarning = warning;
		updateNeeded = true;
	}

	else if (logMidiIn != midiIn)
	{
		logMidiIn = midiIn;
		updateNeeded = true;
	}

	else if (logMidiOut != midiOut)
	{
		logMidiOut = midiOut;
		updateNeeded = true;
	}

	else if (logDebug != debug)
	{
		logDebug = debug;
		updateNeeded = true;
	}

	else if (logTransport != transport)
	{
		logTransport = transport;
		updateNeeded = true;
	}

	else if (logVariations != variations)
	{
		logVariations = variations;
		updateNeeded = true;
	}

	else if (logInfo != info)
	{
		logInfo = info;
		updateNeeded = true;
	}

	else if (updateNeeded)
		broadcaster.sendActionMessage(MsgLog);

} // setLogSettings

///////////////////////////////////////////////////////////////////////

void TopiaryModel::clearLog()
{
	logString = "";
	broadcaster.sendActionMessage(MsgLog);

} // clearLog

///////////////////////////////////////////////////////////////////////

void TopiaryModel::getLogSettings(bool& warning, bool& midiIn, bool& midiOut, bool& debug, bool &transport, bool &variations, bool &info)
{
	warning = logWarning;
	midiIn = logMidiIn;
	midiOut = logMidiOut;
	debug = logDebug;
	transport = logTransport;
	variations = logVariations;
	info = logInfo;
	
} // getLogSettings

///////////////////////////////////////////////////////////////////////

void TopiaryModel::getMidiLogSettings(bool& in, bool& out)
{
	// only used by processor to avoid unneeded calls to Log()
	in = logMidiIn;
	out = logMidiOut;

} // getMidiLogSettings

///////////////////////////////////////////////////////////////////////

void TopiaryModel::logMidi(bool in, MidiMessage &msg)
{
	// only used by processor to avoid unneeded calls to beatsLog()
	if (in)
	{
		if (!logMidiIn) return;
		if (msg.isNoteOff() || msg.isNoteOn())
		{
			int chan = msg.getChannel();
			int velo = msg.getVelocity();
			int  note = msg.getNoteNumber();

			if (msg.isNoteOff())
				Log(String("MIDI in ") + "Note off: "+ MidiMessage::getMidiNoteName(note, true, true, 5) + " velocity: " + String(velo)+"  channel: " + String(chan) + ".", Topiary::LogType::MidiIn);
			else
				Log(String("MIDI in ") + "Note on: " + MidiMessage::getMidiNoteName(note, true, true, 5) + " velocity: " + String(velo) + " channel: " + String(chan) + ".", Topiary::LogType::MidiIn);
		}
		else
		{
			if (msg.isController())
				Log("CC in Controller: " + String(msg.getControllerNumber()) + ": "+ String(msg.getControllerValue()) + " channel: " +  String(msg.getChannel()) + ".", Topiary::LogType::MidiIn);
		}
	}
	else
	{
		if (!logMidiOut) 
			return;
		if (msg.isNoteOff() || msg.isNoteOn())
		{
			int chan = msg.getChannel();
			int velo = msg.getVelocity();
			int  note = msg.getNoteNumber();

			if (msg.isNoteOff())
				Log(String("MIDI out ") + "Note off: " + MidiMessage::getMidiNoteName(note, true, true, 5) + " velocity: " + String(velo) + " channel: " + String(chan)+".", Topiary::LogType::MidiOut);
			else
				Log(String("MIDI out ") + "Note on: " + MidiMessage::getMidiNoteName(note, true, true, 5) + " velocity: " + String(velo) + " channel: " + String(chan) + ".", Topiary::LogType::MidiOut);
		}
		else
		{
			if (msg.isController())
				Log("CC out Controller: " + String(msg.getControllerNumber()) + ": " + String(msg.getControllerValue()) + " channel: " + String(msg.getChannel()) + ".", Topiary::LogType::MidiOut);
		}
	}

} // logMidi

///////////////////////////////////////////////////////////////////////
// TimeSignatures & Transport
///////////////////////////////////////////////////////////////////////

void TopiaryModel::setOverrideHostTransport(bool o)
{
	// virtual
	UNUSED(o)
}


///////////////////////////////////////////////////////////////////////

void TopiaryModel::getTransportState(int& b, int& n, int& d, int& bs, bool& override, bool &waitFFN)
// all transport variables, inc signature, BPM etc
{
	b = BPM;
	n = numerator;
	d = denominator;
	bs = runState;
	waitFFN = WFFN;
	override = overrideHostTransport;
	
} // getTransportState

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setNumeratorDenominator(int nu, int de)
{
	UNUSED(nu);
	UNUSED(de);
	jassert(false); // virtual

} // setNumeratorDenominator

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setBPM(int n)
{
	const GenericScopedLock<SpinLock> myScopedLock(lockModel);
	if (n == 0) n = 120;  // do not allow 0 BPM
	if (BPM != n)
	{
		BPM = n;
		recalcRealTime(); // housekeeping!
		Log(String("BPM set to ") + String(n), Topiary::LogType::Transport);
		
		broadcaster.sendActionMessage(MsgTransport);
	}
} // setBPM

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setRunState(int n)
{
	
	int remember;
	remember = runState;  // needed because in 1 case setting to Armed should fail!!!
	bool varEnabled = false;
	int enabledVariation = -1;
	bool changeSetVariation = false;

	if (runState != n)
	{
		const GenericScopedLock<SpinLock> myScopedLock(lockModel);  // because we may need to call setVariation and that also relies on the lock!
		runState = n;
		switch (n)
		{
		case  Topiary::Running:
			Log("Start running.", Topiary::LogType::Transport);
			//Logger::outputDebugString("RTcur to 0");
			// initialize the patterncursor
			patternCursor = 0;
			blockCursor = 0;
			cursorToStop = (int64)-1;
			
			threadRunnerState = Topiary::ThreadRunnerState::NothingToDo;
			broadcaster.sendActionMessage(MsgTransport);
			break;

		case Topiary::Stopped:

			// reset stuff
			patternCursor = 0;
			blockCursor = 0;
			cursorToStop = (int64)-1;
			Log("Stopped.", Topiary::LogType::Transport);
			broadcaster.sendActionMessage(MsgTransport);

			// if there is a variation waiting, then we need to make sure it becomes boue again - do that outside this scoped lock otherwise we'll lock -- see below

			break;
		case Topiary::Ending:
			Log("Ending, cleaning up.", Topiary::LogType::Transport);
			broadcaster.sendActionMessage(MsgTransport);
			break;
		case Topiary::Ended:
			break;
		case Topiary::Armed:
			if (numPatterns == 0)
			{
				Log("Cannot run because there is no pattern data loaded.", Topiary::LogType::Warning);
				runState = remember;
			}
			else
			{
				// make sure there are variations enbabled
				// and that we selected an enabled variation
				
				for (int i = 0; i < 8; i++)
				{
					if (getVariationEnabled(i))
					{
						varEnabled = true;
						if (enabledVariation == -1)
						{
							enabledVariation = i;
						}
					}
				}
				if (varEnabled)
				{
					if (!getVariationEnabled(variationSelected))
					{
						changeSetVariation = true;
						// we need to call setVariation but that one also needs the lock - do that when lock has been released
					}

					Log("Armed, waiting for first note.", Topiary::LogType::Transport);
					broadcaster.sendActionMessage(MsgTransport);
				}
				else
				{
					Log("Cannot run because there is no variation enabled.", Topiary::LogType::Warning);
					runState = Topiary::Stopped;
				}
			}
			break;
		default:
			break;
		}
		
		broadcaster.sendActionMessage(MsgTransport);
	}

	if (changeSetVariation)
	{
		// if the currently selected variation is disabled switch to one that has been selected!
		// we do that here because setVariation needs the model lock!
		setVariation(enabledVariation);
	}
	else
	{
		setVariation(variationSelected);	// so that if the button was orange, it becomes blue again
	}

	// now the first waiting variation might stil be orange; fix that below
	if (remember == Topiary::Armed)
		setVariation(variationSelected);

} // setRunState

///////////////////////////////////////////////////////////////////////

int TopiaryModel::getRunState()
{
	return runState;
} // getRunState

///////////////////////////////////////////////////////////////////////

void TopiaryModel::processTransportControls(int buttonEnabled)  // buttonEnabled; 0=Stop; 1=start; 2=rec
{
	// careful - transport has 3 buttons but runState has more states!!! need logic to convert here!
	
	// can only be called when overiden
	jassert(overrideHostTransport);

	if (buttonEnabled == 1)
	{
		//  start button pressed
		if ((runState != Topiary::Running) && (runState != Topiary::Armed))
		{
			setRunState(Topiary::Armed);  // if override then either processblock will switch it to Running asap, or processblock will set it to Running at first note 
			// else do nothing otherwise it would restart!	
			broadcaster.sendActionMessage(MsgTransport);
		}
	}
	else
	{   // stop button pressed
		if (buttonEnabled == 0)
		{
			if (runState == Topiary::Armed)
			{
				setRunState(Topiary::Stopped);  // because then it never got started in the first place
				broadcaster.sendActionMessage(MsgTransport);
				return;
			}
			else
			{
				if (runState == Topiary::Running)
				{
					setRunState(Topiary::Ending);  // it will go to Stopped in processblock, when the time has come (depending on runStopQ)
					broadcaster.sendActionMessage(MsgTransport);
				}
				else
				{
					setRunState(Topiary::Stopped);
					broadcaster.sendActionMessage(MsgTransport);
				}
				return;
			}

		}
		else
		{
			jassert(false); // REC not written yet
		}
	}

} // processTransportControls

///////////////////////////////////////////////////////////////////////

int TopiaryModel::getNumPatterns()
{
	// from 1 to number of patterns
	return numPatterns;

} // getNumPatterns


///////////////////////////////////////////////////////////////////////
// GENERATOR STUFF
///////////////////////////////////////////////////////////////////////

void TopiaryModel::threadRunner()
{
	// virtual
} // threadRunner

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setSampleRate(double sr)
{
	if (sr != sampleRate)  // not really needed because also checked in processor !!!
	{
		sampleRate = sr;
		recalcRealTime(); // housekeeping
		Log(String("Samplerate set to ") + String(sampleRate) + String("."), Topiary::LogType::Debug);
	}

} // setSampleRate

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setStartTimes()
{ // time in samples when running really starts + other housekeeping
	//rtCursor = 0;
	blockCursor = 0;
	variationRunning = variationSelected;
	//for (int i = 0; i < 4; i++)
	//	for (int j = 0; j < 8; j++)
	//	{
	//		// initialize variation regeneration
	//		variation[j].poolIdCursor[i] = 1;
	//		variation[j].poolTickCursor[i] = 0;
	//	}
	broadcaster.sendActionMessage(MsgVariationSelected);  // so orange waiting button turn blue;

} // setStartTimes

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setBlockSize(int blocksz)
{
	if (blocksz != blockSize)
	{
		blockSize = blocksz;
		Log(String("Blocksize ") + String(blockSize), Topiary::LogType::Debug);
	}
} // setBlockSize

///////////////////////////////////////////////////////////////////////

void TopiaryModel::getVariationDetailForGenerateMidi(XmlElement** parent, XmlElement** noteChild, int& parentLength, bool& ending, bool& ended)
{                  
	// virtual
	UNUSED(parent);
	UNUSED(noteChild);
	UNUSED(parentLength);
	UNUSED(ending);
	UNUSED(ended);
} //getVariationDetailForGenerateMidi

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setEnded()
{
	// virtual
} // setEnded


///////////////////////////////////////////////////////////////////////

void TopiaryModel::generateMidi(MidiBuffer *buffer)
{
	UNUSED(buffer)
} // generateMidi

///////////////////////////////////////////////////////////////////////

bool TopiaryModel::processVariationSwitch() // called just before generateMidi - to see if a variation is changed, and if so whether to switch now (and make the switch)
{
	// virtual, but standard code that is in TopiaryIncludes.h
	return true;
} // processVariationSwitch

///////////////////////////////////////////////////////////////////////

void TopiaryModel::outputModelEvents(MidiBuffer& buffer)
{	
	// outputs what is in modelEventBuffer
	MidiMessage msg;
	int position;
	const GenericScopedLock<SpinLock> myScopedLock(lockModel);

	auto iterator = MidiBuffer::Iterator(modelEventBuffer);

	while (iterator.getNextEvent(msg, position))
	{
		buffer.addEvent(msg, 0);
	}

	modelEventBuffer.clear();

}  // outputModelEvents

///////////////////////////////////////////////////////////////////////

void TopiaryModel::outputVariationEvents()
{
	// virtual - potentially generate events when variation button is pressed (outside of running) - certainly needed for presetz

} // outputVariationEvents

///////////////////////////////////////////////////////////////////////

bool TopiaryModel::switchingVariations()
{
	// virtual, but standard code that is in TopiaryIncludes.h
	return true;
} // switchingVariations

///////////////////////////////////////////////////////////////////////

void TopiaryModel::initializeVariationsForRunning()
{
	// virtual
}
///////////////////////////////////////////////////////////////////////

void TopiaryModel::endNotesOn(MidiBuffer* midiBuffer)
{
	
	for (int i=1;i<=16;i++)
	{
		MidiMessage msg = MidiMessage::allNotesOff(i);
		midiBuffer->addEvent(msg, 1);
	}

} // endNotesOn

///////////////////////////////////////////////////////////////////////

bool TopiaryModel::processEnding() // called just before generateMidi - to see if we need to stop, and if so whether to stop now (and stop if needed)
{
	// see if we are past the last note
	// we are going to set global variable cursorToStop, based on current block cursor; and only if it is not equal to -1 

	if (cursorToStop == -1)
	{
		switch (runStopQ) {
		case (Topiary::Immediate):
		{
			cursorToStop = blockCursor;
			break;
		}
		case (Topiary::WholePattern):
		{
			cursorToStop = (int64)((blockCursor - patternCursor*samplesPerTick) /* start of this pattern */ + (getVariationLenInTicks(variationRunning)*samplesPerTick));
			//Logger::outputDebugString(String("blockCur ") + String(blockCursor) + String(" stopCursor ") + String(cursorToStop));

			break;
		}
		case (Topiary::Measure):
		{
			cursorToStop = (int64)(blockCursor + samplesPerTick * ((Topiary::TICKS_PER_QUARTER - tick - 1) + (numerator - beat - 1)* Topiary::TICKS_PER_QUARTER));
			break;
		}
		case (Topiary::Half):
		{
			cursorToStop = (int64)(blockCursor + samplesPerTick * ((Topiary::TICKS_PER_QUARTER - tick - 1) + Topiary::TICKS_PER_QUARTER));
			break;
		}
		case (Topiary::Quarter):
		{
			cursorToStop = (int64)(blockCursor + samplesPerTick * ((Topiary::TICKS_PER_QUARTER - tick - 1)));
			break;
		}

		} // switch
	}

	// now see if we're close enough to cursorToStop
	if (cursorToStop < (blockCursor + blockSize))
	{
		cursorToStop = -1;
		setRunState(Topiary::Stopped);
		return true;
	}
	else return false;

} // processEnding


///////////////////////////////////////////////////////////////////////
// Settings stuff
///////////////////////////////////////////////////////////////////////

void TopiaryModel::setWFFN(bool wffn)
{
	WFFN = wffn;
}

///////////////////////////////////////////////////////////////////////

bool TopiaryModel::getWFFN()
{
	return WFFN;
}

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setNotePassThrough(bool echo)
{
	notePassThrough = echo;
} // setNotePassThrough

///////////////////////////////////////////////////////////////////////

bool TopiaryModel::getNotePassThrough()
{
	return notePassThrough;
} // getNotePassThrough

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setVariationStartQ(int q)
{
	variationStartQ = q;
} // setVariationStartQ

///////////////////////////////////////////////////////////////////////

int TopiaryModel::getVariationStartQ()
{
	return variationStartQ;
} // getVariationStartQ

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setRunStopQ(int q)
{
	runStopQ = q;
} // setRunStopQ

///////////////////////////////////////////////////////////////////////

int TopiaryModel::getRunStopQ()
{
	return runStopQ;
} // setRunStopQ

///////////////////////////////////////////////////////////////////////

void TopiaryModel::getTime(int& m, int& b)
{
	const GenericScopedLock<SpinLock> myScopedLock(lockModel); // wait if it's generating
	m = measure;
	b = beat;

} // setRunStopQ

///////////////////////////////////////////////////////////////////////

String TopiaryModel::getName()
{
	return name;

} // getName

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setName(String n)
{
	name = n;

} // setName

///////////////////////////////////////////////////////////////////////

void TopiaryModel::setSwitchVariation(int a)
{
	switchVariation = a;

} // setSwitchVariation

///////////////////////////////////////////////////////////////////////

int TopiaryModel::getSwitchVariation()
{
	return switchVariation;

} // getSwitchVariation

///////////////////////////////////////////////////////////////////////
// Broadcaster & listeners
///////////////////////////////////////////////////////////////////////

void TopiaryModel::setListener(ActionListener *listener)
{
	broadcaster.addActionListener(listener);

} // setListener


void TopiaryModel::removeListener(ActionListener *listener)
{
	broadcaster.removeActionListener(listener);

} // setListener

void TopiaryModel::sendActionMessage(String s)
{
	broadcaster.sendActionMessage(s);

} // sendActionMessage




///////////////////////////////////////////////////////////////////////
// Automation Parameters
///////////////////////////////////////////////////////////////////////

void TopiaryModel::setVariationControl(bool ccSwitching, int channel, int switches[8])
{
	ccVariationSwitching = ccSwitching;
	variationSwitchChannel = channel;

	for (int i = 0; i < 8; i++)
	{
		variationSwitch[i] = switches[i];
	}

} // setVariationControl

///////////////////////////////////////////////////////////////////////

void TopiaryModel::getVariationControl(bool& ccSwitching, int& channel, int switches[8])
{
	ccSwitching = ccVariationSwitching;
	channel = variationSwitchChannel;

	for (int i = 0; i < 8; i++)
	{
		switches[i] = variationSwitch[i];
	}

} // getVariationControl

///////////////////////////////////////////////////////////////////////

void TopiaryModel::processCC(MidiMessage& msg, MidiBuffer* buffer)
{
	// virtual
	UNUSED(msg);
	UNUSED(buffer);

} // processCC

void TopiaryModel::processCC(MidiMessage& msg)
{
	// virtual
	UNUSED(msg);

} // processCC

///////////////////////////////////////////////////////////////////////

void TopiaryModel::processAutomation(MidiMessage& msg)
{
		
	// assert that called only calls this consistently: msg is CC if ccVariationSwithing; msg is NoteOn it not
	if (ccVariationSwitching && !msg.isController())
		return;

	if (!ccVariationSwitching && !msg.isNoteOn())
	return;

	int channel = msg.getChannel();
	int note;
	int controller;

	if (ccVariationSwitching)
	{
		// we use midi CC messages for variation switching
		for (int i = 0; i < 8; i++)
		{
			controller = msg.getControllerNumber();
			if (controller == variationSwitch[i])
			{
				if ((variationSwitchChannel == 0) || (variationSwitchChannel == channel))
				{
					setVariation(i); 
					break;
				}
			}
		}
	}
	else
	{
		// we use note on events for variation switching
		for (int i = 0; i < 8; i++)
		{
			note = msg.getNoteNumber();
			if (note == variationSwitch[i])
			{
				if ((variationSwitchChannel == 0) || (variationSwitchChannel == channel))
				{
					setVariation(i); 
					break;
				}
			}
		}
	}
} // processAutomation

///////////////////////////////////////////////////////////////////////


ActionBroadcaster* TopiaryModel::getBroadcaster()
{
	return &broadcaster;

}

///////////////////////////////////////////////////////////////////////

void TopiaryModel::cleanPattern(int p)
{
	// virtual
	// if there were edits done, recalculate stuff
	// check the length; if turns out to be longer than what the length should be; delete unneeded event
	// redo the Ids (might have added or deleted somthing
	// recalculate timestamps based on meabure, beat and tick
	// set the note number as the note label may have changed
	// regenerate any variations that depend on this pattern

	jassert(false); // should have been overridden
	UNUSED(p)

} // cleanPattern

///////////////////////////////////////////////////////////////////////

void TopiaryModel::copyVariation(int from, int to)
{
	// virtual
	UNUSED(from);
	UNUSED(to);

} // copyVariation

///////////////////////////////////////////////////////////////////////

void TopiaryModel::swapVariation(int from, int to)
{
	// virtual
	UNUSED(from);
	UNUSED(to);

} // swapVariation

///////////////////////////////////////////////////////////////////////

void TopiaryModel::swapPreset(int from, int to)
{
	// virtual
	UNUSED(from);
	UNUSED(to);

} // swapPreset

///////////////////////////////////////////////////////////////////////

void TopiaryModel::copyPreset(int from, int to)
{
	// virtual
	UNUSED(from);
	UNUSED(to);

} // copyPreset