#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

using namespace std;

enum GameState { MENU, DISPLAY_PATTERN, PLAY, ITERATE, GAME_OVER };

// global variables
int pattern[100];
int score = 0;
int count = 0;
int selection = -1;
int gameState = MENU;
double timer = 0.0;
int width;
int height;
Color highlight = {255, 255, 255, 100};

// shape declarations
Rectangle start_btn = {};
Rectangle quit_btn = {};

// functions
int game_logic();
void graphics_handler(int state);
void center_text(const char* text, int x, int y, int fontSize, Color color);

int main() {
  InitWindow(800, 600, "Simon Says");
  SetTargetFPS(60);
  ShowCursor();

  // initialize screen variables
  width = GetScreenWidth();
  height = GetScreenHeight();
  start_btn = {(float)(width / 2 - 200), (float)(height / 5), 400, 50};
  quit_btn = {(float)(width / 2 - 200), (float)(height / 5 + 75), 400, 50};

  while (!WindowShouldClose()) {
    if (IsWindowFullscreen() && !IsWindowFocused()) MinimizeWindow();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    graphics_handler(game_logic());

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

// handle graphics for the game every frame
void graphics_handler(int state) {
  // menu screen graphics
  if (gameState == MENU) {
    // Draw welcome text
    center_text("Welcome to Simon Says!", width / 2, height / 10, 30, BLACK);

    // draw buttons, outlines, and text
    DrawRectangleRec(start_btn, LIGHTGRAY);
    DrawRectangleRec(quit_btn, LIGHTGRAY);
    DrawRectangleLinesEx(start_btn, 2, BLACK);
    DrawRectangleLinesEx(quit_btn, 2, BLACK);
    center_text("Start", (int)(start_btn.x + start_btn.width / 2),
                (int)(start_btn.y + start_btn.height / 2), 20, BLACK);
    center_text("Quit", (int)(quit_btn.x + quit_btn.width / 2),
                (int)(quit_btn.y + quit_btn.height / 2), 20, BLACK);

    // highlight buttons if hovered
    if (CheckCollisionPointRec(GetMousePosition(), start_btn)) {
      DrawRectangleLinesEx(start_btn, 2, GRAY);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        gameState = ITERATE;
      }
    } else if (CheckCollisionPointRec(GetMousePosition(), quit_btn)) {
      DrawRectangleLinesEx(quit_btn, 2, GRAY);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CloseWindow();
      }
    }
  }

  // display pattern graphics to player
  else if (gameState == DISPLAY_PATTERN) {
    // display circles
    DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 0, 90, 10,
                     RED);
    DrawCircleSectorLines({(float)width / 2, (float)height / 2}, 100, 0, 90, 10,
                          BLACK);
    DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 90, 180, 10,
                     BLUE);
    DrawCircleSectorLines({(float)width / 2, (float)height / 2}, 100, 90, 180,
                          10, BLACK);
    DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 180, 270, 10,
                     YELLOW);
    DrawCircleSectorLines({(float)width / 2, (float)height / 2}, 100, 180, 270,
                          10, BLACK);
    DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 270, 360, 10,
                     GREEN);
    DrawCircleSectorLines({(float)width / 2, (float)height / 2}, 100, 270, 360,
                          10, BLACK);

    // highlight current colour
    if (state == 1) {
      DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 0, 90, 10,
                       highlight);
    } else if (state == 2) {
      DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 90, 180, 10,
                       highlight);
    } else if (state == 3) {
      DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 180, 270, 10,
                       highlight);
    } else if (state == 4) {
      DrawCircleSector({(float)width / 2, (float)height / 2}, 100, 270, 360, 10,
                       highlight);
    }

  }

  else if (gameState == PLAY) {
    center_text("Repeat the pattern!", width / 2, height / 10, 30, BLACK);
  }

  else if (gameState == GAME_OVER) {
    center_text("Game Over!", width / 2, height / 10, 30, BLACK);
    center_text("Press any key to return to menu", width / 2, height / 5, 20,
                BLACK);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      gameState = MENU;
    }
  }
}

// contain all game logic in a loop that is called every frame
int game_logic() {
  // show menu
  if (gameState == MENU) {
    // reset variables
    score = 0;
    count = 0;
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
        // reset timer
        timer = 1.0;
        // display colour
        printf("%d\n", pattern[count]);
        count++;
        return pattern[count - 1] + 1;
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

  return 0;
}

// draw text in the center of a given location
void center_text(const char* text, int x, int y, int fontSize, Color color) {
  // get text width
  int text_width = MeasureText(text, fontSize);

  // draw text in the center of the given location
  DrawText(text, x - text_width / 2, y - fontSize / 2, fontSize, color);

  return;
}