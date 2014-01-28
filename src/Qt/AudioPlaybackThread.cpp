/**
 * @file
 * @brief Source file for AudioPlaybackThread class
 * @author Duzy Chan <code@duzy.info>
 *
 * @section LICENSE
 *
 * Copyright (c) 2008-2013 OpenShot Studios, LLC
 * (http://www.openshotstudios.com). This file is part of
 * OpenShot Library (http://www.openshot.org), an open-source project
 * dedicated to delivering high quality video editing and animation solutions
 * to the world.
 *
 * OpenShot Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenShot Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenShot Library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "../include/ReaderBase.h"
#include "../include/RendererBase.h"
#include "AudioPlaybackThread.h"

namespace openshot
{
    AudioPlaybackThread::AudioPlaybackThread()
	: Thread("audio-playback")
	, audioDeviceManager()
	, player()
	, transport()
	, mixer()
	, source(NULL)
	, sampleRate(0.0)
	, numChannels(0)
    {
	audioDeviceManager.initialise (
	    0, /* number of input channels */
	    2, /* number of output channels */
	    0, /* no XML settings.. */
	    true  /* select default device on failure */);
	audioDeviceManager.addAudioCallback(&player);
	mixer.addInputSource(&transport, false);
	player.setSource(&mixer);
    }

    AudioPlaybackThread::~AudioPlaybackThread()
    {
	player.setSource(0);
	audioDeviceManager.removeAudioCallback(&player);
	audioDeviceManager.closeAudioDevice();
	audioDeviceManager.removeAllChangeListeners();
	audioDeviceManager.dispatchPendingMessages();
    }

    void AudioPlaybackThread::run()
    {
	while (!threadShouldExit()) {
	    play.wait();

	    /*
	    transport.setSource(
		&source,
		5000, // tells it to buffer this many samples ahead
		nullptr,
		sampleRate,
		numChannels);
	    transport.setPosition(0);
	    transport.setGain(1.0);
	    transport.start();
	    while (transport.isPlaying()) {
		sleep(10);
	    }
	    transport.stop();
	    transport.setSource(0);
	    */

	    played.signal();
	}
    }
}