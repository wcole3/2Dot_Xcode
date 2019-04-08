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
//total control buttons
const int TOTAL_CONTROLS = 9;
//player control prompt textures
lTexture gPlayerPrompt[TOTAL_CONTROLS];
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

//Mix Chunks and music
Mix_Chunk* gLoseSound = NULL;
Mix_Chunk* gWinSound = NULL;
Mix_Chunk* gClickSound = NULL;
Mix_Chunk* gSelectSound = NULL;
Mix_Chunk* gWallBounceSound = NULL;
Mix_Music* gMenuMusic = NULL;
Mix_Music* gGameMusic = NULL;




//FILE_LOCATIONS
string DEFAULT_ASSET_LOC = "assets/";
//need the name of the settings file
string settingsFile = DEFAULT_ASSET_LOC + "settings.txt";
//name of the leaderboard file
string leaderboardFile = DEFAULT_ASSET_LOC + "leaderboard.txt";
string textFontFile = DEFAULT_ASSET_LOC + "OpenSans-Regular.ttf";
string winScreenFile = DEFAULT_ASSET_LOC + "winScreen.png";
string loseScreenFile = DEFAULT_ASSET_LOC + "loseScreen.png";
string pregameScreen = DEFAULT_ASSET_LOC + "preGameInst.png";
string menuScreenFile = DEFAULT_ASSET_LOC + "menuScreen.png";
string settingsScreenFile = DEFAULT_ASSET_LOC + "settingsScreen.png";
string leaderboardScreenFile = DEFAULT_ASSET_LOC + "leaderboardScreen.png";
string tileSpriteFile = DEFAULT_ASSET_LOC + "tile_sprites.png";
string dot1File = DEFAULT_ASSET_LOC + "dot1.png";
string dot2File = DEFAULT_ASSET_LOC + "dot2.png";
string loseSoundFile = DEFAULT_ASSET_LOC + "aww.wav";
string clickSoundFile = DEFAULT_ASSET_LOC + "click.wav";
string wallBounceSoundFile = DEFAULT_ASSET_LOC + "jump.wav";
string selectSoundFile = DEFAULT_ASSET_LOC + "pop.wav";
string winSoundFile = DEFAULT_ASSET_LOC + "short_fanfare.wav";
string menuMusicFile = DEFAULT_ASSET_LOC + "Sky_puzzle.mp3";
string gameMusicFile = DEFAULT_ASSET_LOC + "Techno_Caper.mp3";
//all of the player controls
string playerControls[TOTAL_CONTROLS];
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
