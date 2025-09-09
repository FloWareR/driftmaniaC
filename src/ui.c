#include "ui.h"
#include "config.h"

static int splashScreenTimer = 0;

/**
 * @brief Frame Counter for the splash screen
 */
void UpdateSplashScreen(GameState *state)
{
    splashScreenTimer++;

    if (splashScreenTimer > 180)
    {
        state->currentScreen = MAIN_MENU;
    }
}

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
            ResetGameplayState(state);
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
 * @brief Updates pause menu and handles button clicking
 */
void UpdatePauseMenu(GameState *state)
{
    // Allow unpausing by pressing P again
    if (IsKeyPressed(PAUSE_KEY))
    {
        state->currentScreen = GAMEPLAY;
    }

    Vector2 mousePoint = GetMousePosition();
    Rectangle resumeButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 25, 200, 50};
    Rectangle menuButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 35, 200, 50};

    // Check for button clicks
    if (CheckCollisionPointRec(mousePoint, resumeButton))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            state->currentScreen = GAMEPLAY;
        }
    }

    if (CheckCollisionPointRec(mousePoint, menuButton))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            state->currentScreen = MAIN_MENU;
        }
    }
}

/**
 * @brief Draws our logo / Splash screen
 */
void RenderplashScreen(const GameState *state)
{
    ClearBackground(BLACK);

    // --- FADE IN / FADE OUT LOGIC ---
    float alpha = 0.0f;

    // Fade in for the first second
    if (splashScreenTimer < 60)
    {
        alpha = (float)splashScreenTimer / 60.0f;
    }
    // Stay fully visible for the second second
    else if (splashScreenTimer < 120)
    {
        alpha = 1.0f;
    }
    // Fade out for the third second
    else
    {
        alpha = 1.0f - ((float)splashScreenTimer - 120) / 60.0f;
    }

    // --- RESIZE LOGO ---
    float targetLogoWidth = SCREEN_WIDTH * 0.4f;
    float targetLogoHeight = SCREEN_HEIGHT * 0.3f;

    float logoAspectRatio = (float)state->logoTexture.width / state->logoTexture.height;
    if (targetLogoWidth / targetLogoHeight > logoAspectRatio)
    {
        targetLogoWidth = targetLogoHeight * logoAspectRatio;
    }
    else
    {
        targetLogoHeight = targetLogoWidth / logoAspectRatio;
    }

    // --- DRAW LOGO IMAGE ---
    float totalContentHeight = targetLogoHeight + 50.0f + 50;

    Vector2 logoDrawPosition = {
        SCREEN_WIDTH / 2.0f - targetLogoWidth / 2.0f,
        SCREEN_HEIGHT / 2.0f - (totalContentHeight / 2.0f) // Start drawing logo from this Y
    };

    DrawTexturePro(state->logoTexture,
                   (Rectangle){0, 0, (float)state->logoTexture.width, (float)state->logoTexture.height},
                   (Rectangle){logoDrawPosition.x, logoDrawPosition.y, targetLogoWidth, targetLogoHeight},
                   (Vector2){0, 0},
                   0.0f,
                   Fade(WHITE, alpha));

    // --- DRAW TEXT ---
    const char *logoText = "FLOWAREGAMES";
    float fontSize = 50; // Use a variable for font size
    float spacing = 2;   // Use a variable for spacing

    Vector2 textSize = MeasureTextEx(state->mainFont, logoText, fontSize, spacing);

    Vector2 textPosition = {
        SCREEN_WIDTH / 2.0f - textSize.x / 2.0f,
        logoDrawPosition.y + targetLogoHeight + 50.0f
    };

    DrawTextEx(state->mainFont, logoText, textPosition, 50, 2, Fade(WHITE, alpha));
}

/**
 * @brief Renders the main menu
 */
void RenderMainMenu(const GameState *state)
{
    ClearBackground(DARKGRAY);

    // --- DRAW TITLE ---
    // Measure the text dimensions first
    Rectangle titleArea = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f};

    // Calculate the position
    Vector2 titlePos = GetTextCenterPositionInRect(titleArea, GAME_NAME, state->mainFont, 70, 2);

    // Draw the text at the calculated position
    DrawTextEx(state->mainFont, GAME_NAME, titlePos, 70, 2, WHITE);

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
    Vector2 playTextPos = GetTextCenterPositionInRect(playButton, "PLAY", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "PLAY", playTextPos, 30, 2, BLACK);

    DrawRectangleRec(exitButton, exitColor);
    Vector2 exitTextPos = GetTextCenterPositionInRect(exitButton, "EXIT", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "EXIT", exitTextPos, 30, 2, BLACK);
}

/**
 * @brief Renders the pause menu
 */
void RenderPauseMenu(const GameState *state)
{
    // Draw a semi-transparent overlay to dim the background
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5f));

    // --- DRAW TITLE ---
    // Measure the text dimensions first
    Rectangle titleArea = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f};

    // Calculate the position
    Vector2 titlePos = GetTextCenterPositionInRect(titleArea, "PAUSED", state->mainFont, 50, 2);

    // Draw the text at the calculated position
    DrawTextEx(state->mainFont, "PAUSED", titlePos, 50, 2, WHITE);

    // --- Draw Buttons and Handle Hover ---
    Vector2 mousePoint = GetMousePosition();
    Rectangle resumeButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 25, 200, 50};
    Rectangle menuButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 35, 200, 50};

    Color resumeColor = LIME;
    Color menuColor = ORANGE;

    if (CheckCollisionPointRec(mousePoint, resumeButton))
        resumeColor = GREEN;
    if (CheckCollisionPointRec(mousePoint, menuButton))
        menuColor = GOLD;

    // --- Draw RESUME Button ---
    DrawRectangleRec(resumeButton, resumeColor);
    Vector2 resumeTextPos = GetTextCenterPositionInRect(resumeButton, "RESUME", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "RESUME", resumeTextPos, 30, 2, BLACK);

    // --- Draw MAIN MENU Button ---
    DrawRectangleRec(menuButton, menuColor);
    Vector2 menuTextPos = GetTextCenterPositionInRect(menuButton, "MAIN MENU", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "MAIN MENU", menuTextPos, 30, 2, BLACK);
}

/**
 * @brief Calculates the position to draw text so it's centered in a rectangle.
 * @param container The rectangle to center the text within.
 * @param text The text to be centere.
 * @param font The font to use for measuring.
 * @param fontSize The size of the font.
 * @param spacing The spacing between characters.
 * @return A Vector2 representing the (x, y) coordinates to pass to DrawTextEx.
 */
Vector2 GetTextCenterPositionInRect(Rectangle container, const char *text, Font font, float fontSize, float spacing)
{
    // Measure the dimensions of the text
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);

    // Calculate the position to center the text
    Vector2 textPosition = {
        container.x + (container.width / 2.0f) - (textSize.x / 2.0f),
        container.y + (container.height / 2.0f) - (textSize.y / 2.0f)};

    return textPosition;
}