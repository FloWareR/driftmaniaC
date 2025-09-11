@echo off
echo Building the project...

gcc -o game.exe src/main.c src/player.c src/game.c src/particle.c src/ui.c src/entity.c src/audio.c -I src/include -L src/lib -lraylib -lgdi32 -lwinmm

if %errorlevel% equ 0 (
    echo Build successful! Starting game...
    game.exe
) else (
    echo Build failed with error code %errorlevel%
    pause
)