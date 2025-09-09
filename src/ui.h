#ifndef UI_H
#define UI_H

#include "game.h"

//====================================================================================
// Function Prototypes
//====================================================================================
void UpdateSplashScreen(GameState *state);
void UpdateMainMenu(GameState *state);
void UpdatePauseMenu(GameState *state);
void RenderplashScreen(const GameState *state);
void RenderMainMenu(const GameState *state);
void RenderPauseMenu(const GameState *state);

Vector2 GetTextCenterPositionInRect(Rectangle container, const char *text, Font font, float fontSize, float spacing);
#endif