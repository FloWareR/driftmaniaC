#include "audio.h"

void InitAudioManager(AudioManager *am)
{

    InitAudioDevice();

    // --- LOAD MUSIC ---
    am->music[MUSIC_MENU] = LoadMusicStream("src/assets/audio/main_menu_track.mp3");
    am->music[MUSIC_GAMEPLAY] = LoadMusicStream("src/assets/audio/ingame_track.mp3");

    // Set initial volumes
    SetMusicVolume(am->music[MUSIC_MENU], 0.5f);
    SetMusicVolume(am->music[MUSIC_GAMEPLAY], 0.5f);
}

void UnloadAudioManager(AudioManager *am)
{
    for (int i = 0; i < MAX_MUSIC_TRACKS; i++)
        UnloadMusicStream(am->music[i]);
    for (int i = 0; i < MAX_SOUND_EFFECTS; i++)
        UnloadSound(am->sfx[i]);
    CloseAudioDevice();
}

void PlayGameMusic(AudioManager *am, GameScreen screen)
{
    // Stop any music that is currently playing
    if (IsMusicStreamPlaying(am->currentMusic))
    {
        StopMusicStream(am->currentMusic);
    }

    // Select and play the new track
    switch (screen)
    {
    case MAIN_MENU:
        am->currentMusic = am->music[MUSIC_MENU];
        break;
    case GAMEPLAY:
        am->currentMusic = am->music[MUSIC_GAMEPLAY];
        break;
    default:
        return;
    }
    PlayMusicStream(am->currentMusic);
}

// This function MUST be called every frame to keep playing music
void UpdateGameMusic(AudioManager *am)
{
    if (IsMusicStreamPlaying(am->currentMusic))
    {
        UpdateMusicStream(am->currentMusic);
    }
}