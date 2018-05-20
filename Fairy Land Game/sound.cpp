#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  //Mix_FreeMusic(music);
 /* for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }*/
  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() :
  soundEffects(true),
  backgroundMusic(true),
  volume(SDL_MIX_MAXVOLUME/4), 
  currentSound(-1), 
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("sounds/background.mp3");
  if (!music) throw string("Couldn't load background music")+Mix_GetError();

  startMusic();

  sounds.push_back( Mix_LoadWAV("sounds/player-dies.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/enemy-dies.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/enemy-dies.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/player-wins.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/player-lost.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/collect-eggs.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/player-shoot.wav") );
  sounds.push_back( Mix_LoadWAV("sounds/enemy-shoot.wav") );
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
  std::cout << "Music and Sound is loaded" << std::endl;
}

void SDLSound::play() {
  if( backgroundMusic && Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  }
}

void SDLSound::pause() {
  if( !Mix_PausedMusic() ) { 
    Mix_PauseMusic();
  }
}

void SDLSound::toggleBackgroundMusic() {
  backgroundMusic = !backgroundMusic;
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  }
  else Mix_PauseMusic();
}

void SDLSound::toggleSoundEffects() {
  soundEffects = !soundEffects;
}

void SDLSound::playSoundEffect(const std::string& soundName) {
  int index = -1;
  if ( soundName == "PLAYER_EXPLODE" ) index = 0;
  else if ( soundName == "ENEMY_EXPLODE" ) index = 1;
  else if ( soundName == "BOSS_ENEMY_EXPLODE" ) index = 2;
  else if ( soundName == "GAME_WON" ) index = 3;
  else if ( soundName == "GAME_LOST" ) index = 4;
  else if ( soundName == "EGG_COLLECTED" ) index = 5;
  else if ( soundName == "PLAYER_SHOOT" ) index = 6;
  else if ( soundName == "BOSS_ENEMY_SHOOT" ) index = 7;

  if (soundEffects && index >=0 && index <= 7) {
    if (currentSound >= 0) Mix_HaltChannel(currentSound);
    currentSound = index;
    Mix_VolumeChunk(sounds[index], volume);
    channels[index] = Mix_PlayChannel(-1, sounds[index], 0);
  }
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

