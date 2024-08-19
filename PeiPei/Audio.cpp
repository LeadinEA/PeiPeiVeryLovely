#include "const.h"
#include "Audio.h"

AudioDrives::AudioDrives() {
	BASS_Init(-1, 44100, 0, NULL, NULL);
}

bool AudioDrives::SetStream(const char File[]) {
	Sound = BASS_StreamCreateFile(false, File, 0, 0, BASS_SAMPLE_LOOP);
	return true;
}

void AudioDrives::PlayAudio() {
	BASS_ChannelPlay(Sound, true);
}