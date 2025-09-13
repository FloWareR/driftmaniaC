#ifndef AUDIO_H
#define AUDIO_H

#include <raylib.h>
#include "game.h"

#define MAX_SOUND_EFFECTS 16
#define MAX_MUSIC_TRACKS 4

//====================================================================================
// Structs and Typedefs
//====================================================================================
typedef enum SoundEffect
{
    SFX_CRASH,
    SFX_COLLECT,
    SFX_DRIFT,
    SFX_ACCEL,
    SFX_BRAKE
} SoundEffect;

typedef enum MusicTracks
{
    MUSIC_SPLASH,
    MUSIC_MENU,
    MUSIC_GAMEPLAY,
} MusicTracks;

typedef struct AudioManager
{
    Sound sfx[MAX_SOUND_EFFECTS];
    Music music[MAX_MUSIC_TRACKS];
    Music currentMusic;
} AudioManager;

//====================================================================================
// Function Prototypes
//====================================================================================
void InitAudioManager(AudioManager *am);
void UnloadAudioManager(AudioManager *am);
void PlayGameMusic(AudioManager *am, GameScreen screen);
void PlayGameSFX(AudioManager *am, SoundEffect sfx);
void UpdateGameMusic(AudioManager *am);

#endif