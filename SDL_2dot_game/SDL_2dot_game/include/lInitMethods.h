//
//  initMethods.h
//  SDL_2dot_game
//
//
//  Collection of all initialization methods for main
//
//
//  Created by William Cole on 3/18/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef initMethods_h
#define initMethods_h


//forward declarations
//method to setup SDL, IMG, and TTF in additon to other class objects
/**
    Sets up SDL, IMG, TTF,and all window and texture objects
 
    @return true if everything is setup properly
 */
bool init();
//Load the media, textures, and any sound files
/**
    Loads all texture objects and setup files
 
    @return true if everything is loaded correctly
 */
bool loadMedia();
//method to load settings from a txt file for the player object
/**
    Load the settings from the settings.txt file
 
    @param player1 the first dot texture
    @param player2 the second dot texture
    @param fileName the name of the settings file
    @param controlPrompts the control prompt to be set
 
    @return true if the controls are loaded successfully
 */
bool loadSettings(lRigidDot* player1,lRigidDot* player2, string fileName, string controlPrompts[]);
//method to load the leaderboard
/**
    Load the leaderboard from the leaderboard.txt file
 
    @param fileName the filename of the leaderboard file
    @param names the names loaded from the leaderboard file
    @param scores the socres loaded from the leaderboard file
 
    @return true if the leaderboard was loaded successfully
 */
bool loadLeaderboard(string fileName, string names[], float scores[]);
//method to load all music and sound effects
/**
    Load the music files and the sounds effects
 
    @return true returns true on success
 */
bool loadMusicAndEffects();
//close down and free resources
/**
    Deallocate all resources and close libs
 */
void close();
//method to resize all UI elements to fit current window dimensions
/**
    Resize the global UI elements on a window event
 
    @param e the SDL_Event to handle
 */
void resizeUI(SDL_Event* e);
//method to setup the tiles subject to the map we will read in
/**
 Reads the map file and sets the associated tile map
 
 @param levelNumber the index of the level to load
 
 @return true if the tiles are set successfully
 */
bool setTiles(int levelNumber, string levelFileName);

bool init(){
    bool successFlag = true;
    //start SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) < 0){
        printf("Could not start SDL! SDL error: %s\n", SDL_GetError());
        successFlag = false;
    }else{
        //create window
        if(!gWindow.init(SCREEN_WIDTH, SCREEN_HEIGHT, "Welcome to 2Dot")){
            printf("Could not create window! SDL error: %s\n", SDL_GetError());
            successFlag = false;
        }else{
            //setup the player object
            player1 = lRigidDot(gWindow.getRenderer());
            player2 = lRigidDot(gWindow.getRenderer());
            //setup player prompts
            for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0])); ++i){
                gPlayerPrompt[i] = lTexture(gWindow.getRenderer());
            }
            //setup menu buttons
            for(int i = 0; i < (sizeof(gMenuPrompts)/sizeof(gMenuPrompts[0])); ++i){
                gMenuPrompts[i] = lTexture(gWindow.getRenderer());
            }
            //setup leaderboard entries
            for(int i = 0; i < (sizeof(gLeaderboardEntry)/sizeof(gLeaderboardEntry[0])); ++i){
                gLeaderboardEntry[i] = lTexture(gWindow.getRenderer());
            }
            for(int i = 0; i < (sizeof(gLetters)/sizeof(gLetters[0])); ++i){
                gLetters[i] = lTexture(gWindow.getRenderer());
            }
            //setup textures
            for(int i = 0; i < TOTAL_IMAGE_TEXTURES; ++i){
                gImageTextures[i] = new lTexture(gWindow.getRenderer());
            }
            //set render clear color
            SDL_SetRenderDrawColor(gWindow.getRenderer(), 255, 255, 255, 255);
        }
    }
    //start sdl image
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        printf("Could not start IMG! IMG error: %s\n", IMG_GetError());
        successFlag = false;
    }
    //start SDL_TTF
    if(TTF_Init() < 0){
        printf("Could not start TTF! TTF error: %s\n", TTF_GetError());
        successFlag = false;
    }
    //Start SDL_mix with 1 kb chunks
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) < 0){
        printf("Could not start Mix! Mix error: %s", Mix_GetError());
        successFlag = false;
    }
    return successFlag;
}


bool loadMedia(){
    bool successFlag = true;
    //load the font
    gFont = TTF_OpenFont(textFontFile.c_str(), 15);
    if(gFont == NULL){
        printf("Could not open font! TTF error: %s\n", TTF_GetError());
        successFlag = false;
    }else{
        //setup the prompt textures
        setFonts(gPlayerPrompt, gFont, (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0])));
        //do the same for menus
        setFonts(gMenuPrompts, gFont, (sizeof(gMenuPrompts)/sizeof(gMenuPrompts[0])));
        //same for leaderboard entries
        setFonts(gLeaderboardEntry, gFont, (sizeof(gLeaderboardEntry)/sizeof(gLeaderboardEntry[0])));
        //set letter fonts
        setFonts(gLetters, gFont, (sizeof(gLetters)/sizeof(gLetters[0])));
        //setup text prompt
        gImageTextures[gCountdownText]->setFont(gFont);
        gImageTextures[gChangeSettingsPrompt]->setFont(gFont);
    }
    if(!loadSettings(&player1,&player2, settingsFile, playerControls)){
        printf("Could not load settings!\n");
        successFlag = false;
    }else{
        //set the text of the prompt texture now that the control scheme is known
        string controlPrompt = "";
        //now load the rendered text
        for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0])); ++i){
            controlPrompt = controlButton[i%4] + playerControls[i];
            if(!gPlayerPrompt[i].loadFromRenderedText(controlPrompt.c_str(), white)){
                printf("Could not load text string from prompt!\n");
                successFlag = false;
            }
        }
    }
    //load leaderboard
    if(!loadLeaderboard(leaderboardFile, leaderboardNames, leaderboardScores)){
        printf("Could not load leaderboard!\n");
        successFlag = false;
    }else{
        //load the leaderboard entries
        for(int i = 0; i < LEADERBOARD_LINES; ++i){
            char header [5];
            char line [10];
            int rank = i + 1;
            sprintf(header, "%d. ", rank);
            sprintf(line, "%.3f", leaderboardScores[i]);
            string entry = header + leaderboardNames[i] + " " + line;
            if(!gLeaderboardEntry[i].loadFromRenderedText(entry.c_str(), black)){
                printf("Could not load leaderboard entry #: %d!\n", i);
                successFlag = false;
            }
        }
    }
    //load all image textures
    for(int i = 0; i < TOTAL_IMAGE_TEXTURES; ++i){
        if(imageTextureFiles[i] != ""){//check if the file exists
            if(!gImageTextures[i]->loadFromFile(imageTextureFiles[i], SDL_FALSE)){
                printf("Could not load texture at path: %s\n", imageTextureFiles[i].c_str());
                successFlag = false;
            }
        }
    }
    //load menu button textures
    for(int i = 0; i < (sizeof(gMenuPrompts)/sizeof(gMenuPrompts[0])); ++i){
        if(!gMenuPrompts[i].loadFromRenderedText(menuButtons[i].c_str(), blue)){
            printf("Could not load menu button texture!\n");
            successFlag = false;
        }
    }
    //load the change setting prompt
    if(!gImageTextures[gChangeSettingsPrompt]->loadFromRenderedText(changeSetting.c_str(), red)){
        printf("Could not load change settings prompt texture!\n");
        successFlag = false;
    }
    //now we need to set the tiles
    for(int i = 0; i < TOTAL_LEVELS; ++i){
        string levelFileName = DEFAULT_ASSET_LOC + "level_" + to_string(i) + ".map";
        if(!setTiles(i, levelFileName)){
            printf("Could not load level: %d!\n", i);
            successFlag = false;
        }
    }
    //if all the levels were loaded successfully we can init the player dots
    if(successFlag){
        //need to setup the player's level area and starting position, always load level 0 to start
        player1.setLevelSize(LEVEL_WIDTH[currentLevel], LEVEL_HEIGHT[currentLevel]);
        player1.setStartingPos(0, 0);
        player1.setLevel(currentLevel);
        player2.setLevelSize(LEVEL_WIDTH[currentLevel], LEVEL_HEIGHT[currentLevel]);
        player2.setStartingPos(LEVEL_WIDTH[currentLevel], LEVEL_HEIGHT[currentLevel]);
        player2.setLevel(currentLevel);
    }
    //load the player texture
    if(!player1.loadFromFile(dot1File, SDL_TRUE, white)){
        printf("Could not load player 1 texture!\n");
        successFlag = false;
    }
    if(!player2.loadFromFile(dot2File, SDL_TRUE, white)){
        printf("Could not load player 2 texture!\n");
        successFlag = false;
    }
    //load sounds
    if(!loadMusicAndEffects()){
        printf("Could not load sound!\n");
        successFlag = false;
    }else{
        //set dot sound effects
        player1.setSoundEffect(gWallBounceSound, gGrowthSound);
        player2.setSoundEffect(gWallBounceSound, gGrowthSound);
    }
    return successFlag;
}

//load the settings from file; create a new file if default one doesnt exist
bool loadSettings(lRigidDot* player1,lRigidDot* player2, string fileName, string controlPrompts[]){
    bool successFlag = true;
    //start by setting up a default setting object
    const char* defaultSettings[TOTAL_CONTROLS - 1];//up button, down button, left, right for each player
    for(int i = 0; i < sizeof(defaultSettings)/sizeof(defaultSettings[0]); ++i){
        defaultSettings[i] = NULL;
    }
    const char* player1Controls[4];
    const char* player2Controls[4];
    for(int i = 0; i < sizeof(player1Controls)/sizeof(player1Controls[0]); ++i){
        player1Controls[i] = NULL;
        player2Controls[i] = NULL;
    }
    //get the file object
    fstream file(fileName.c_str(), ifstream::in);
    //we just need to read from the file
    //check if the file exists
    if(!file.is_open()){
        printf("The settings file does not exist...\n");//if the file doesn't exist we need to create it
        file.open(settingsFile.c_str(), ifstream::out);
        if(file.is_open()){
            printf("Creating new settings file...\n");
            //here we need to fill in the contents of the file
            //also fill the string prompts so we can render the control scheme
            defaultSettings[0] = "W";//player 1
            controlPrompts[0] = "W";
            defaultSettings[1] = "S";
            controlPrompts[1] = "S";
            defaultSettings[2] = "A";
            controlPrompts[2] = "A";
            defaultSettings[3] = "D";
            controlPrompts[3] = "D";
            defaultSettings[4] = "I";//start player 2
            controlPrompts[4] = "I";
            defaultSettings[5] = "K";
            controlPrompts[5] = "K";
            defaultSettings[6] = "J";
            controlPrompts[6] = "J";
            defaultSettings[7] = "L";
            controlPrompts[7] = "L";
            string boost = "Space";
            controlPrompts[8] = "Space";
            //now write buttons
            for(int i = 0; i < sizeof(defaultSettings)/sizeof(defaultSettings[0]); ++i){
                file << defaultSettings[i];
                file << endl;
            }
            file << boost.c_str();
            file.close();
            //need to partition the settings into arrays for loading to the players
            for(int i = 0; i < sizeof(player1Controls)/sizeof(player1Controls[0]); ++i){
                player1Controls[i] = defaultSettings[i];
                player2Controls[i] = defaultSettings[i+4];
            }
            player1->loadControls(player1Controls, boost.c_str());
            player2->loadControls(player2Controls, boost.c_str());
        }else{
            printf("Could not create new settings file!\n");
            successFlag = false;
        }
    }else{
        //here we read in the data from the settings file
        printf("Loading settings...\n");
        string line[TOTAL_CONTROLS];
        for(int i = 0; i < sizeof(defaultSettings)/sizeof(defaultSettings[0]); ++i){
            char key;
            file >> key;
            //change the char to upper case if need; theres got to be a better way to do this
            key = toupper(key);
            string keyOut(1, key);
            line[i] = keyOut;
            //check if keybind missing
            if(line[i] == "#"){
                line[i] = " ";
            }
            //since we are assigning pointers we need to make sure that each object is different
            defaultSettings[i] = line[i].c_str();
            controlPrompts[i] = line[i];//also assign the control prompt
        }
        string boost;
        file >> boost;
        controlPrompts[8] = boost;
        file.close();
        //need to partition the settings into arrays for loading to the players
        for(int i = 0; i < sizeof(player1Controls)/sizeof(player1Controls[0]); ++i){
            player1Controls[i] = defaultSettings[i];
            player2Controls[i] = defaultSettings[i+4];
        }
        player1->loadControls(player1Controls, boost.c_str());
        player2->loadControls(player2Controls, boost.c_str());
    }
    return successFlag;
}

//method to load the current leaderboard from file
bool loadLeaderboard(string fileName, string names[], float scores[]){
    bool successFlag = true;
    //open the file
    fstream file(fileName.c_str(), ifstream::in);
    //check if file is open
    if(!file.is_open()){
        //file doesnt exist, create default
        printf("Leaderboard file doesn't exist!\n");
        file.open(fileName.c_str(), ifstream::out);
        if(file.is_open()){
            printf("Creating default leaderboard file...\n");
            //write default leaderboard
            for(int i = 0; i < LEADERBOARD_LINES; ++i){
                scores[i] = defaultStartingScore + (i * defaultStartingScore);//set default scores to something arbitrairly high
                names[i] = defaultNames[i];
                //now write out to file
                file << defaultNames[i] << '\t' << scores[i] << endl;
            }
            file.close();
        }else{
            printf("Error: Could not create default leaderboard file!\n");
            successFlag = false;
        }
    }else{
        //file exists, read in current leaderboard
        printf("Loading leaderboard....\n");
        for(int i = 0; i < LEADERBOARD_LINES; ++i){
            file >> names[i];
            file >> scores[i];
        }
        //close file
        file.close();
    }
    return successFlag;
}
//load music and sound effects
bool loadMusicAndEffects(){
    bool successFlag = true;
    //load all sound effects and set volume, for reference the music is at volume 30
    gWinSound = Mix_LoadWAV(winSoundFile.c_str());
    if(gWinSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", winSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gWinSound, 25);//set volume
    }
    gLoseSound = Mix_LoadWAV(loseSoundFile.c_str());
    if(gLoseSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", loseSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gLoseSound, 25);
    }
    gClickSound = Mix_LoadWAV(clickSoundFile.c_str());
    if(gClickSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", clickSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gClickSound, 40);
    }
    gSelectSound = Mix_LoadWAV(selectSoundFile.c_str());
    if(gSelectSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", selectSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gSelectSound, 40);
    }
    gWallBounceSound = Mix_LoadWAV(wallBounceSoundFile.c_str());
    if(gWallBounceSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", wallBounceSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gWallBounceSound, 80);
    }
    gGrowthSound = Mix_LoadWAV(growthSoundFile.c_str());
    if(gGrowthSound == NULL){
        printf("Could not load sound effect at : %s! Mix Error: %s\n", growthSoundFile.c_str(), Mix_GetError());
        successFlag = false;
    }else{
        Mix_VolumeChunk(gGrowthSound, 80);
    }
    //load music
    gMenuMusic = Mix_LoadMUS(menuMusicFile.c_str());
    if(gMenuMusic == NULL){
        printf("Could not load music file at : %s! Mix Error: %s\n", menuMusicFile.c_str(), Mix_GetError());
        successFlag = false;
    }
    gGameMusic = Mix_LoadMUS(gameMusicFile.c_str());
    if(gGameMusic == NULL){
        printf("Could not load music file at : %s! Mix Error: %s\n", gameMusicFile.c_str(), Mix_GetError());
        successFlag = false;
    }
    return successFlag;
}


//cleanup everything
void close(){
    //free players
    player1.free();
    player2.free();
    //free all textures
    for(int i = 0; i < TOTAL_IMAGE_TEXTURES; i++){
        gImageTextures[i]->free();
        gImageTextures[i] = NULL;
    }
    //free all global text textures
    for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0])); ++i){
        gPlayerPrompt[i].free();
    }
    for(int i = 0; i < (sizeof(gMenuPrompts)/sizeof(gMenuPrompts[0])); ++i){
        gMenuPrompts[i].free();
    }
    for(int i = 0; i < (sizeof(gLeaderboardEntry)/sizeof(gLeaderboardEntry[0])); ++i){
        gLeaderboardEntry[i].free();
    }
    for(int i = 0; i < (sizeof(gLetters)/sizeof(gLetters[0])); ++i){
        gLetters[i].free();
    }
    for(int i = 0; i < TOTAL_LEVELS; ++i){
        for(int x = 0; x < TOTAL_TILES[i]; ++x){
            delete gTiles[i][x];
        }
    }
    //free chunks and music
    Mix_FreeChunk(gWinSound);
    Mix_FreeChunk(gLoseSound);
    Mix_FreeChunk(gClickSound);
    Mix_FreeChunk(gSelectSound);
    Mix_FreeChunk(gWallBounceSound);
    Mix_FreeChunk(gGrowthSound);
    Mix_FreeMusic(gMenuMusic);
    Mix_FreeMusic(gGameMusic);
    gWindow.free();
    TTF_CloseFont(gFont);
    gFont = NULL;
    //close libs
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
}

//resize all UI element to fit current window size
void resizeUI(SDL_Event* e){
    if(e->window.event == SDL_WINDOWEVENT_RESIZED){
        //if the window resizes we need to adjust the UI
        //get the new window dimesions and set the playerscreens dimensions to match
        player1Screen.w = (gWindow.getWidth()/2)-10;
        player1Screen.h = gWindow.getHeight()-20;
        player2Screen.w = (gWindow.getWidth()/2)-10;
        player2Screen.h = gWindow.getHeight()-20;
        player2Screen.x = (gWindow.getWidth()/2)+10;
        split.x = (gWindow.getWidth()/2)-10;
        split.h = gWindow.getHeight();
        //change the player cameras
        camera1.w = player1Screen.w;
        camera1.h = player1Screen.h;
        camera2.w = player2Screen.w;
        camera2.h = player2Screen.h;
    }
}

//method to setup the tiles subject to the map we will read in
/**
 Reads the map file and sets the associated tile map
 
 @param tiles the to be set from the map file
 
 @return true if the tiles are set successfully
 */
bool setTiles(int levelNumber, string levelFileName){
    bool tilesSet = true;
    //also set x and y offsets
    int x = 0;
    int y = 0;
    //first need to read in the map file
    ifstream map(levelFileName);
    //first check if the file was openned
    if(!map.is_open()){
        printf("Could not open map file!\n");
        tilesSet = false;
        
    }else{
        //if the map is openned then we can begin looping to set the values
        int width, height;
        map >> width;
        map >> height;
        TOTAL_TILES[levelNumber] = width * height;
        //now that total tiles is know allocate gTiles
        gTiles[levelNumber] = new lTile*[TOTAL_TILES[levelNumber]];
        LEVEL_WIDTH[levelNumber] = width * TILE_WIDTH;
        LEVEL_HEIGHT[levelNumber] = height * TILE_HEIGHT;
        for(int i = 0; i < TOTAL_TILES[levelNumber]; i++){
            // the values from the file give the type of the tile
            int tileType=-1;
            map >> tileType;
            //check if the read in type is valid
            if(map.fail()){
                printf("Could not read type!\n");
                tilesSet = false;
                break;
            }
            else{
                //check if type is valid
                if((tileType >= 0) && (tileType < TOTAL_TILES_TYPES)){
                    //after the type has been set we can create the tile object
                    gTiles[levelNumber][i] = new lTile(x, y, tileType);
                }
                else{
                    printf("Tile type not valid for entry: %d\n", i);
                    tilesSet = false;
                    break;
                }
                //after we create the tile we move the offsets; the map file assumes we move from left to right
                //and then down
                x+=TILE_WIDTH;
                if(x >= LEVEL_WIDTH[levelNumber]){
                    //if we have moved acroos the level start moving down
                    x = 0;
                    y += TILE_HEIGHT;
                }
            }
        }
    }
    //we might as well setup the sprite sheet here as well
    if(tilesSet){
        //set all of the widths and heights in for loop
        for(int i = 0; i < TOTAL_TILES_TYPES; i++){
            gTileSprite[i].w = TILE_WIDTH;
            gTileSprite[i].h = TILE_HEIGHT;
        }
        //now manually set the clip x and y's for the clips
        gTileSprite[RED_TILE].x = 0;
        gTileSprite[RED_TILE].y = 0;
        
        gTileSprite[BLUE_TILE].x = 80;
        gTileSprite[BLUE_TILE].y = 0;
        
        gTileSprite[TOP_LEFT].x = 160;
        gTileSprite[TOP_LEFT].y = 0;
        
        gTileSprite[TOP].x = 240;
        gTileSprite[TOP].y = 0;
        
        gTileSprite[TOP_RIGHT].x = 320;
        gTileSprite[TOP_RIGHT].y = 0;
        
        gTileSprite[ALL_BORDER].x = 400;
        gTileSprite[ALL_BORDER].y = 0;
        
        gTileSprite[RIGHT_CAP].x = 480;
        gTileSprite[RIGHT_CAP].y = 0;
        
        gTileSprite[RED_CHECK].x = 0;
        gTileSprite[RED_CHECK].y = 80;
        
        gTileSprite[GREEN_TILE].x = 80;
        gTileSprite[GREEN_TILE].y = 80;
        
        gTileSprite[LEFT].x = 160;
        gTileSprite[LEFT].y = 80;
        
        gTileSprite[CENTER].x = 240;
        gTileSprite[CENTER].y = 80;
        
        gTileSprite[RIGHT].x = 320;
        gTileSprite[RIGHT].y = 80;
        
        gTileSprite[VERT_BORDER].x = 400;
        gTileSprite[VERT_BORDER].y = 80;
        
        gTileSprite[LEFT_CAP].x = 480;
        gTileSprite[LEFT_CAP].y = 80;
        
        gTileSprite[ENDZONE].x = 0;
        gTileSprite[ENDZONE].y = 160;
        
        gTileSprite[TOP_CAP].x = 80;
        gTileSprite[TOP_CAP].y = 160;
        
        gTileSprite[BOTTOM_LEFT].x = 160;
        gTileSprite[BOTTOM_LEFT].y = 160;
        
        gTileSprite[BOTTOM].x = 240;
        gTileSprite[BOTTOM].y = 160;
        
        gTileSprite[BOTTOM_RIGHT].x = 320;
        gTileSprite[BOTTOM_RIGHT].y = 160;
        
        gTileSprite[HORZ_BORDER].x = 400;
        gTileSprite[HORZ_BORDER].y = 160;
        
        gTileSprite[BOT_CAP].x = 480;
        gTileSprite[BOT_CAP].y = 160;
        
    }
    
    map.close();
    return tilesSet;
}



#endif /* initMethods_h */
