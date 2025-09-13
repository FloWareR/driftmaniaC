#include "ui.h"
#include "config.h"
#include "audio.h"

static int splashScreenTimer = 0;
static int mainMenuSelection = 0;
static int pauseMenuSelection = 0;

/**
 * @brief Frame Counter for the splash screen
 */
void UpdateSplashScreen(GameState *state)
{
    splashScreenTimer++;

    if (splashScreenTimer > 180)
    {
        state->currentScreen = MAIN_MENU;
        PlayGameMusic(state->audioManager, MAIN_MENU);
    }
}

/**
 * @brief Updates main menu and handles button clicking.
 */
void UpdateMainMenu(GameState *state)
{
    // Keyboard Input
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP))
    {
        mainMenuSelection = (mainMenuSelection + 1) % 2;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        switch (mainMenuSelection)
        {
        case 0: // PLAY
            ResetGameplayState(state);
            state->currentScreen = GAMEPLAY;
            PlayGameMusic(state->audioManager, GAMEPLAY);
            break;
        case 1: // EXIT
            state->currentScreen = QUIT;
            break;
        }
    }

    // Mouse Input
    Vector2 mousePoint = GetMousePosition();
    Rectangle playButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f - 25, 200, 50};
    Rectangle exitButton = {SCREEN_WIDTH / 2.0f - 100, SCREEN_HEIGHT / 2.0f + 35, 200, 50};

    if (CheckCollisionPointRec(mousePoint, playButton))
    {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            ResetGameplayState(state);
            PlayGameMusic(state->audioManager, GAMEPLAY);
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
    // --- KEYBOARD INPUT ---
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_UP))
    {
        pauseMenuSelection = (pauseMenuSelection + 1) % 2; // Toggles between 0 and 1
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        switch (pauseMenuSelection)
        {
        case 0: // RESUME
            state->currentScreen = GAMEPLAY;
            break;
        case 1: // MAIN MENU
            state->currentScreen = MAIN_MENU;
            PlayGameMusic(state->audioManager, MAIN_MENU);
            break;
        }
    }
    // Allow unpausing by pressing key again
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
            PlayGameMusic(state->audioManager, MAIN_MENU);
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
        SCREEN_HEIGHT / 2.0f - (totalContentHeight / 2.0f)};

    DrawTexturePro(state->logoTexture,
                   (Rectangle){0, 0, (float)state->logoTexture.width, (float)state->logoTexture.height},
                   (Rectangle){logoDrawPosition.x, logoDrawPosition.y, targetLogoWidth, targetLogoHeight},
                   (Vector2){0, 0},
                   0.0f,
                   Fade(WHITE, alpha));

    // --- DRAW TEXT ---
    const char *logoText = "FLOWAREGAMES";
    float fontSize = 50;
    float spacing = 2;

    Vector2 textSize = MeasureTextEx(state->mainFont, logoText, fontSize, spacing);

    Vector2 textPosition = {
        SCREEN_WIDTH / 2.0f - textSize.x / 2.0f,
        logoDrawPosition.y + targetLogoHeight + 50.0f};

    DrawTextEx(state->mainFont, logoText, textPosition, 50, 2, Fade(WHITE, alpha));
}

/**
 * @brief Renders the main menu
 */
void RenderMainMenu(const GameState *state)
{
    ClearBackground(BLACK);

    // Get screen and texture dimensions
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();
    float texWidth = (float)state->mainMenuBackground.width;
    float texHeight = (float)state->mainMenuBackground.height;

    // Calculate aspect ratios
    float screenAspectRatio = screenWidth / screenHeight;
    float textureAspectRatio = texWidth / texHeight;

    Rectangle sourceRec = {0, 0, texWidth, texHeight};    
    Rectangle destRec = {0, 0, screenWidth, screenHeight}; 

    // Adjust source or destination to maintain aspect ratio and fill screen
    if (textureAspectRatio > screenAspectRatio)
    {
        // Texture is wider than screen, need to crop horizontally
        float scale = screenHeight / texHeight;
        float scaledWidth = texWidth * scale;
        float cropAmount = (scaledWidth - screenWidth) / scale;
        sourceRec.x = cropAmount / 2; // Crop equally from both sides
        sourceRec.width = texWidth - cropAmount;
    }
    else
    {
        // Texture is taller than screen, need to crop vertically
        float scale = screenWidth / texWidth;
        float scaledHeight = texHeight * scale;
        float cropAmount = (scaledHeight - screenHeight) / scale;
        sourceRec.y = cropAmount / 2; // Crop equally from top and bottom
        sourceRec.height = texHeight - cropAmount;
    }

    // Draw the background texture, filling the screen while maintaining aspect ratio
    DrawTexturePro(state->mainMenuBackground,
                   sourceRec,
                   destRec,
                   (Vector2){0, 0},
                   0.0f,
                   Fade(WHITE, 0.65f));

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

    if (CheckCollisionPointRec(GetMousePosition(), playButton))
    {
        mainMenuSelection = 0;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), exitButton))
    {
        mainMenuSelection = 1;
    }
    bool isPlaySelected = mainMenuSelection == 0;
    bool isExitSelected = mainMenuSelection == 1;

    DrawRectangleRec(playButton, LIME);
    if (isPlaySelected)
    {
        DrawRectangleLinesEx(playButton, 3, WHITE); // Draw a 3px white outline if selected
    }
    Vector2 playTextPos = GetTextCenterPositionInRect(playButton, "PLAY", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "PLAY", playTextPos, 30, 2, BLACK);

    DrawRectangleRec(exitButton, RED);
    if (isExitSelected)
    {
        DrawRectangleLinesEx(exitButton, 3, WHITE); // Draw a 3px white outline if selected
    }
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

    if (CheckCollisionPointRec(GetMousePosition(), resumeButton))
    {
        pauseMenuSelection = 0;
    }
    else if (CheckCollisionPointRec(GetMousePosition(), menuButton))
    {
        pauseMenuSelection = 1;
    }
    bool isResumeSelected = pauseMenuSelection == 0;
    bool isMenuSelected = pauseMenuSelection == 1;

    DrawRectangleRec(resumeButton, LIME);
    if (isResumeSelected)
    {
        DrawRectangleLinesEx(resumeButton, 3, WHITE); // Draw a 3px white outline if selected
    }
    Vector2 resumeTextPos = GetTextCenterPositionInRect(resumeButton, "RESUME", state->mainFont, 30, 2);
    DrawTextEx(state->mainFont, "RESUME", resumeTextPos, 30, 2, BLACK);

    DrawRectangleRec(menuButton, ORANGE);
    if (isMenuSelected)
    {
        DrawRectangleLinesEx(menuButton, 3, WHITE); // Draw a 3px white outline if selected
    }
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