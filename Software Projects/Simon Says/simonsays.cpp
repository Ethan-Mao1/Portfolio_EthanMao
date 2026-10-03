#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

using namespace std;

enum GameState { MENU, DISPLAY_PATTERN, PLAY, ITERATE, GAME_OVER };

int pattern[100];
int score = 0;
int count = 0;
int selection = -1;
int gameState = MENU;
double timer = 0.0;

// functions
void game_logic();

int main() {
  InitWindow(800, 600, "Simon Says");
  SetTargetFPS(60);
  ShowCursor();

  while (!WindowShouldClose()) {
    if (IsWindowFullscreen() && !IsWindowFocused()) MinimizeWindow();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Welcome to Simon Says!", 200, 200, 5, BLACK);
    game_logic();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void game_logic() {
  // show menu
  if (gameState == MENU) {
    // reset variables
    score = 0;
    count = 0;

    // allow for start
    gameState = ITERATE;
  }

  // add another pattern to the list
  else if (gameState == ITERATE) {
    srand((unsigned int)time(NULL));
    pattern[score] = rand() % 4;

    // change gamestate and reset associate variables
    timer = 1.0;
    count = 0;
    gameState = DISPLAY_PATTERN;
  }

  // show current pattern to player
  else if (gameState == DISPLAY_PATTERN) {
    // loop to go through entire pattern
    if (count <= score) {
      // display 1 a second
      if (timer >= 0) {
        timer -= GetFrameTime();
      } else {
        // display colour
        printf("%d\n", pattern[count]);
        count++;

        // reset timer
        timer = 1.0;
      }
    }

    else {
      count = 0;
      gameState = PLAY;
    }
  }

  // allow player to select buttons in order
  else if (gameState == PLAY) {
    // logic to determine selection
    cin >> selection;

    // user has selected
    if (selection != -1) {
      // correct
      if (selection == pattern[count]) {
        // correct feedback
        count++;

        // see if end of array
        if (count > score) {
          score++;
          gameState = ITERATE;
          count = 0;
        }
      } else {
        // wrong feedback
        printf("wrong, die nerd!\n");

        // change gameState
        gameState = GAME_OVER;
      }

      // reset selection
      selection = -1;
    }
  }
}