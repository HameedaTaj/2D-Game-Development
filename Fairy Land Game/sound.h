#ifndef SOUND__H
#define SOUND__H

#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

class SDLSound {
public:
  SDLSound();
  ~SDLSound();

  void startMusic();
  void stopMusic();
  void play();
  void pause();
  void playSoundEffect(const std::string&);
  void toggleSoundEffects();
  void toggleBackgroundMusic();
  bool isBackgroundMusicOn() { return backgroundMusic; }
  bool areSoundEffectsOn() { return soundEffects; }

private:
  bool soundEffects;
  bool backgroundMusic;
  int volume;
  int currentSound;
  Mix_Music *music;

  int audioRate;
  int audioChannels;
  int audioBuffers;
  std::vector<Mix_Chunk*> sounds;
  std::vector<int> channels;
  SDLSound(const SDLSound&);
  SDLSound& operator=(const SDLSound&);
};

#endif
