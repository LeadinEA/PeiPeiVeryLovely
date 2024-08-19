#pragma once
#include "bass.h"

class AudioDrives
{
public:
	AudioDrives();

	bool SetStream(const char File[]);
	void PlayAudio();
private:
	HSTREAM Sound = NULL;
};