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
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//level information
const int TOTAL_LEVELS = 4;
int LEVEL_WIDTH[TOTAL_LEVELS];
int LEVEL_HEIGHT[TOTAL_LEVELS];
int TOTAL_TILES[TOTAL_LEVELS];
//starting values for leaderboard
constexpr float defaultStartingScore = 50;
//the marker for the current level
int currentLevel = 0;
//running time of a current run
float runTime = 0;

//Game window
lWindow gWindow;
//Players or dots
lRigidDot player1;
lRigidDot player2;


enum imageTextures {
    gTileSpriteSheet = 0,
    gWinSplash = 1,
    gNextLevelSplash = 2,
    gPregameSplash = 3,
    gMenu = 4,
    gSettingsScreen = 5,
    gLeaderboardScreen = 6,
    gCountdownText = 7,
    gChangeSettingsPrompt = 8,
    gLevel0Img = 9,
    gLevel1Img = 10,
    gLevel2Img = 11,
    gLevel3Img = 12,
    gFullRunTextImg = 13,
    gSingleLevelTextImg = 14,
    TOTAL_IMAGE_TEXTURES = 15
};
//ALL TEXTURES
lTexture* gImageTextures[TOTAL_IMAGE_TEXTURES];
//the actual tile objects
lTile** gTiles[TOTAL_LEVELS];


//Text to use
TTF_Font* gFont = NULL;

//ALL TEXT TEXTURES
//headings for control prompts
const string controlButton[4] = {"Up   : ", "Down : ", "Left : ", "Right: "};
//total control buttons
const int TOTAL_CONTROLS = 9;
//player control prompt textures
lTexture gPlayerPrompt[TOTAL_CONTROLS];
//the menu commands
lTexture gMenuPrompts[4];
//the actual menu button names
string menuButtons[4] = {"Play", "Settings", "Leaderboard", "Quit"};
//the prompt for when user wants to change a setting
string changeSetting = "Press new key";
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
Mix_Chunk* gGrowthSound = NULL;
Mix_Music* gMenuMusic = NULL;
Mix_Music* gGameMusic = NULL;





//FILE_LOCATIONS
const string DEFAULT_ASSET_LOC = "assets/";
//using an aray for image texture to make initialization easier; make sure order is same as imageTextures
const string imageTextureFiles[TOTAL_IMAGE_TEXTURES] = {
    DEFAULT_ASSET_LOC + "tile_sprites.png",
    DEFAULT_ASSET_LOC + "winScreen.png",
    DEFAULT_ASSET_LOC + "nextLevelScreen.png",
    DEFAULT_ASSET_LOC + "preGameInst.png",
    DEFAULT_ASSET_LOC + "menuScreen.png",
    DEFAULT_ASSET_LOC + "settingsScreen.png",
    DEFAULT_ASSET_LOC + "leaderboardScreen.png",
    "",//text texture are blanks
    "",
    DEFAULT_ASSET_LOC + "level_0.png",
    DEFAULT_ASSET_LOC + "level_1.png",
    DEFAULT_ASSET_LOC + "level_2.png",
    DEFAULT_ASSET_LOC + "level_3.png",
    DEFAULT_ASSET_LOC + "fullRunText.png",
    DEFAULT_ASSET_LOC + "singleLevelText.png"
};
const string settingsFile = DEFAULT_ASSET_LOC + "settings.txt";//need the name of the settings file
const string leaderboardFile = DEFAULT_ASSET_LOC + "leaderboard.txt";//name of the leaderboard file
const string textFontFile = DEFAULT_ASSET_LOC + "OpenSans-Regular.ttf";
const string dot1File = DEFAULT_ASSET_LOC + "dot1.png";
const string dot2File = DEFAULT_ASSET_LOC + "dot2.png";
const string loseSoundFile = DEFAULT_ASSET_LOC + "aww.wav";
const string clickSoundFile = DEFAULT_ASSET_LOC + "click.wav";
const string wallBounceSoundFile = DEFAULT_ASSET_LOC + "jump.wav";
const string growthSoundFile = DEFAULT_ASSET_LOC + "pling.wav";
const string selectSoundFile = DEFAULT_ASSET_LOC + "pop.wav";
const string winSoundFile = DEFAULT_ASSET_LOC + "short_fanfare.wav";
const string menuMusicFile = DEFAULT_ASSET_LOC + "Sky_puzzle.mp3";
const string gameMusicFile = DEFAULT_ASSET_LOC + "Techno_Caper.mp3";
const string level0MapFile = DEFAULT_ASSET_LOC + "level_0.map";//names of the level map files
const string level1MapFile = DEFAULT_ASSET_LOC + "level_1.map";
const string level2MapFile = DEFAULT_ASSET_LOC + "level_2.map";
const string level3MapFile = DEFAULT_ASSET_LOC + "level_3.map";

//all of the player controls
string playerControls[TOTAL_CONTROLS];
//number of leaderboard entries
const int LEADERBOARD_LINES = 5;
//names of the leaderboard
string leaderboardNames[LEADERBOARD_LINES];
//leaderboard scores
float leaderboardScores[LEADERBOARD_LINES];
//default leaderboard names
const string defaultNames[LEADERBOARD_LINES] = {"IBM", "FDR", "SAS", "JFK", "LOL"};

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
SDL_Color highlighter = {100, 100, 200};
#endif /* globals_h */
