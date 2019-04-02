//
//  globals.h
//  SDL_2dot_game
//
//  All global variables and textures for main
//
//  Created by William Cole on 3/18/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef globals_h
#define globals_h
using namespace std;

//screen constants
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
int LEVEL_WIDTH;
int LEVEL_HEIGHT;

//Game window
lWindow gWindow;
//Players or dots
lRigidDot player1;
lRigidDot player2;



//ALL TEXTURES
//sprite sheet for background tiles
lTexture gTileSpriteSheet;
//the actual tile objects
lTile* gTiles[TOTAL_TILES];
//winning game splash screen
lTexture gWinSplash;
//losing game splash screen
lTexture gLoseSplash;
//pregame instruction splash
lTexture gPregameSplash;
//menu screen
lTexture gMenu;
//settings screen
lTexture gSettingsScreen;
//leaderboard screen
lTexture gLeaderboardScreen;

//Text to use
TTF_Font* gFont = NULL;

//ALL TEXT TEXTURES
//headings for control prompts
const string controlButton[4] = {"Up   : ", "Down : ", "Left : ", "Right: "};
//player control prompt textures
lTexture gPlayerPrompt[8];
//texture for the countdown texture
lTexture gCountdownText;
//the menu commands
lTexture gMenuPrompts[4];
//the actual menu button names
string menuButtons[4] = {"Play", "Settings", "Leaderboard", "Quit"};
//the prompt for when user wants to change a setting
string changeSetting = "Press new key";
//the texture for the change setting prompt
lTexture gChangeSettingPrompt;
//the textures of the leaderboard names and scores
lTexture gLeaderboardEntry[5];
//max number of character in leaderbaord name
const int MAX_LETTERS = 3;
//entered name letter textures
lTexture gLetters[MAX_LETTERS];



//FILE INPUTS
//need the name of the settings file
string settingsFile = "assets/settings.txt";
//all of the player controls
string playerControls[8];
//name of the leaderboard file
string leaderboardFile = "assets/leaderboard.txt";
//number of leaderboard entries
const int LEADERBOARD_LINES = 5;
//names of the leaderboard
string leaderboardNames[LEADERBOARD_LINES];
//leaderboard scores
float leaderboardScores[LEADERBOARD_LINES];


//UI ELEMENTS
//now we define the UI frames for startup
SDL_Rect player1Screen = {0,20,(SCREEN_WIDTH/2)-10,SCREEN_HEIGHT-20};
SDL_Rect player2Screen = {(SCREEN_WIDTH/2)+10, 20, (SCREEN_WIDTH/2)-10, SCREEN_HEIGHT-20};
//cameras for the screens
SDL_Rect camera1 = {0,20,(SCREEN_WIDTH/2)-10,SCREEN_HEIGHT-20};
SDL_Rect camera2 = {(SCREEN_WIDTH/2)+10, 20, (SCREEN_WIDTH/2)-10, SCREEN_HEIGHT-20};
//dividing box for the two screens
SDL_Rect split = {(SCREEN_WIDTH/2 - 10), 0, 20, SCREEN_HEIGHT};

//other globals
int TOTAL_MENU_BUTTONS = 4;
//colors
SDL_Color black = {0, 0, 0};
SDL_Color red = {255, 0, 0};
SDL_Color blue = {0, 0, 255};
SDL_Color green = {0, 255, 0};
SDL_Color white = {255, 255, 255};
SDL_Color cyan = {0, 255, 255};
#endif /* globals_h */
