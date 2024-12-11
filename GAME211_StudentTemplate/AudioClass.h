#pragma once

#include <irrKlang.h>

#include <memory>
#include <string>
#include <iostream>

using namespace std;

class AudioClass {
private:
//	unique_ptr<irrklang::ISoundEngine> engine;
	irrklang::ISoundEngine* engine;
	irrklang::ISoundSource* trumpetSound;
public:
	AudioClass() {
//		engine = unique_ptr<irrklang::ISoundEngine>(irrklang::createIrrKlangDevice());
		engine = irrklang::createIrrKlangDevice();
		if (!engine) {
			cout << "Could not create sound engine." << endl;
		}
		// Pre-load the sound once
		trumpetSound = engine->addSoundSourceFromFile("sound/SKULLTRUMPET.ogg");
	}
	~AudioClass() {
		engine->drop();
	}

	void PlaySound(string fileDir_) {
		// make a sound object from the file
		// save that object locally
		auto ISoundSource = engine->addSoundSourceFromFile(fileDir_.data());

		// use the sound object in the engine->play2D() method
		if (engine) {
			engine->play2D(fileDir_.c_str(), false);
		}
		// profit

	}


};

