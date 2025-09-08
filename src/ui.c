#include "ui.h"
#include "config.h"

/**
 * @brief Updates main menu and handles button clicking.
 */
void UpdateMainMenu(GameState *state)
{
    Vector2 mousePoint = GetMousePosition();

    Rectangle playButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 25, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 35, 200, 50};

    if (CheckCollisionPointRec(mousePoint, playButton))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            state->currentScreen = GAMEPLAY;
        }
    }

    if (CheckCollisionPointRec(mousePoint, exitButton))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            state->currentScreen = QUIT;
        }
    }
}

/**
 * @brief Renders the main menu
 */
void RenderMainMenu(const GameState *state)
{
    ClearBackground(DARKGRAY);

    // --- DRAW TITLE ---
    // Measure the text dimensions first
    Vector2 textSize = MeasureTextEx(state->mainFont, GAME_NAME, 50, 2);

    // Then calculate the centered position
    Vector2 textPosition = {
        SCREEN_WIDTH / 2.0f - textSize.x / 2.0f,
        SCREEN_HEIGHT / 3.0f - textSize.y / 2.0f};

    // Draw the text at the calculated position
    DrawTextEx(state->mainFont, GAME_NAME, textPosition, 50, 2, WHITE);

    // --- Draw Buttons and Handle Hover ---
    Vector2 mousePoint = GetMousePosition();
    Rectangle playButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 25, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 35, 200, 50};

    Color playColor = LIME;
    Color exitColor = RED;

    if (CheckCollisionPointRec(mousePoint, playButton))
        playColor = GREEN;
    if (CheckCollisionPointRec(mousePoint, exitButton))
        exitColor = PINK;

    // Draw buttons as rectangles with text
    DrawRectangleRec(playButton, playColor);
    DrawText("PLAY", playButton.x + 65, playButton.y + 10, 30, BLACK);

    DrawRectangleRec(exitButton, exitColor);
    DrawText("EXIT", exitButton.x + 65, exitButton.y + 10, 30, BLACK);
}