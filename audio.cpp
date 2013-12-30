#include "audio.h"
#include "types.h"
#include "fileio.h"
#include <irrKlang.h>
#include <unordered_map>
using namespace irrklang;

static std::unordered_map<string, ISoundSource*> tracks;
static ISoundEngine *engine;

bool load_audio()
{
	engine = createIrrKlangDevice();
	if(!engine)
	{
		log_msg("Failed to load audio device");
		return false;
	}

	return true;
}

bool load_sound(const std::string &filename)
{
	ISoundSource *src = engine->addSoundSourceFromFile(filename.c_str());
	if (!src)
	{
		log_msg("Failed to load sound: " + filename);
		return false;
	}
	tracks[filename] = src;
	return true;
}

void free_audio()
{
	// Audio sources are handled internally, so we don't need to drop them
	engine->drop();
}

void play_sound(const string &filename, bool looped)
{
	std::unordered_map<string, ISoundSource*>::iterator i = tracks.find(filename);
	if (i != tracks.end())
	{
		engine->play2D(i->second);
	}
	else
	{
		// Tried to play non-added track
	}
}

void stop_all_sounds()
{
	engine->stopAllSounds();
}