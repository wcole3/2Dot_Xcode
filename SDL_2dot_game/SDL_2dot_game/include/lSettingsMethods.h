//
//  lSettingsMethods.h
//  SDL_2dot_game
//
//  Methods to navigate and alter settings in the "Settings" menu
//
//  Created by William Cole on 3/22/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lSettingsMethods_h
#define lSettingsMethods_h
//top level decs

//method to display the settings screeen and handle events on the screen
/**
    Display the settings screen
 
    @param globalQuit a bool to handle user quit events
 */
void displaySettingsScreen(bool* globalQuit);
//method to set where the highlight should be rendered
/**
    Set the location of the highlight box showing current selection
 
    @param box the highlighter box
    @param x the x offset of the selection
    @param y the y offset of the selection
 */
void setHighlightBox(SDL_Rect* box, int x, int y);
//method to handle key down events in settings
/**
    Handle key press events on screen
 
    @param e the SDL_Event to test
    @param xOffset the int handling the x position of the selection
    @param yOffset the int handling the y position of the selection
    @param goBack true if the user wants to go back to the menu
    @param globalQuit true if user wants to quit the program
 */
void handleKeyPress(SDL_Event e, int* xOffset, int* yOffset, bool* goBack, bool* globalQuit);
//method to handle mouse motion events
/**
    Handle mouse events
 
     @param e the SDL_Event to test
     @param xOffset the int handling the x position of the selection
     @param yOffset the int handling the y position of the selection
     @param goBack true if the user wants to go back to the menu
     @param globalQuit true if user wants to quit the program
 */
void handleMouseEvent(SDL_Event e, int* xOffset, int* yOffset, bool* goBack, bool* globalQuit);
//method to change the settings of the selected keybind
/**
    Change the selected setting
 
    @param e the SDL_Event to use to get the new control
    @param xOffset the int handling the x position of the selection
    @param yOffset the int handling the y position of the selection
    @param globalQuit true if user wants to quit the program
 */
void changeSettingHotkey(SDL_Event e, int xOffset, int yOffset, bool* globalQuit);
//method to update the control scheme
/**
    Update the current control scheme and rerender the control prompts
 
    @param playerControls the list of controls for the dots
 */
void updateControls(string playerControls[]);
//method to save the control scheme into the settings file
/**
    Save the new setting to file
 
    @param file the settings file
    @param playerControls the player controls to write to file
 
    @return true if save was successful
 */
bool saveSettings(string file, string playerControls[]);


//load the setting screen and handle events on screen
void displaySettingsScreen(bool* globalQuit){
    //setup the constant string textures
    lTexture lSettingsStrings[3];
    //the textures will be 0 = "Player 1", 1 = "Player 2", 2 = "Back"
    string settingsStrings[3];
    settingsStrings[0] = "Left Dot";
    settingsStrings[1] = "Right Dot";
    settingsStrings[2] = "Back";
    for(int i = 0; i < (sizeof(lSettingsStrings)/sizeof(lSettingsStrings[0])); ++i){
        lSettingsStrings[i] = lTexture(gWindow.getRenderer());
        lSettingsStrings[i].setFont(gFont);
        if(!lSettingsStrings[i].loadFromRenderedText(settingsStrings[i].c_str(), white)){
            printf("Could not load setting screen string!\n");
        }
    }
    //need to rerender the boost prompt
    string boostPrompt = "Boost: " + playerControls[8];
    if(!gPlayerPrompt[8].loadFromRenderedText(boostPrompt.c_str(), white)){
        printf("Could not rerender boost prompt!\n");
    }
    //get event and done flag
    bool done = false;
    SDL_Event e;
    //need an offset to handle settings entrys
    int xOffset = 0;
    int yOffset = 0;
    while(!done){
        if(!*globalQuit){
            while(SDL_PollEvent(&e) != 0){
                //if quit is hit do a global quit
                if(e.type == SDL_QUIT){
                    *globalQuit = true;
                    done = true;
                }
                gWindow.handleEvent(e);
                resizeUI(&e);
                //TODO handle settings menu events
                handleKeyPress(e, &xOffset, &yOffset, &done, globalQuit);
                handleMouseEvent(e, &xOffset, &yOffset, &done, globalQuit);
            }
            //render the setting screen
            gWindow.render();
            SDL_Rect screen = {0,0,gWindow.getWidth(),gWindow.getHeight()};
            gSettingsScreen.render(0, 0, NULL, &screen);
            //TODO render settings and quit button
            //render the settings string constants
            SDL_Rect textLine = {0,0, gWindow.getWidth() / 7, gWindow.getHeight() / 10};
            lSettingsStrings[0].render((gWindow.getWidth() / 4) - (textLine.w / 2), (gWindow.getHeight() / 5), NULL, &textLine);
            lSettingsStrings[1].render(( 3 * (gWindow.getWidth() / 4)) - (textLine.w / 2), (gWindow.getHeight() / 5), NULL, &textLine);
            lSettingsStrings[2].render(3 * (gWindow.getWidth() / 4), 5 * (gWindow.getHeight() / 6), NULL, &textLine);
            //now render the players control schemes
            for(int i = 0; i < (sizeof(controlButton) / sizeof(controlButton[0])); ++i){
                //render the player 1 controls on the left and player 2 on the right
                gPlayerPrompt[i].render((gWindow.getWidth() / 4) - (textLine.w / 2), (gWindow.getHeight() / 5) + (i * textLine.h) + textLine.h, NULL, &textLine);
                gPlayerPrompt[i+4].render((3*(gWindow.getWidth() / 4))-(textLine.w/2), (gWindow.getHeight() / 5) + (i * textLine.h) + textLine.h, NULL, &textLine);
            }
            //render boost prompt
            gPlayerPrompt[8].render((gWindow.getWidth() / 2) - (textLine.w / 2), 2 * (gWindow.getHeight() / 3), NULL, &textLine);
            //set the highlighter box
            setHighlightBox(&textLine, xOffset, yOffset);
            SDL_SetRenderDrawColor(gWindow.getRenderer(), 100, 200, 100, 100);
            SDL_RenderFillRect(gWindow.getRenderer(), &textLine);
            SDL_RenderPresent(gWindow.getRenderer());
        }else{
            done = true;
        }
    }
    //free the constant textures
    for(int i = 0; i < (sizeof(lSettingsStrings)/sizeof(lSettingsStrings[0])); ++i){
        lSettingsStrings[i].free();
    }
}

//method to set where the highlight box should be rendered
void setHighlightBox(SDL_Rect* box, int x, int y){
    if(x == 0){
        //do left side first
        box->x = (gWindow.getWidth() / 4) - (box->w / 2);
        box->y = (gWindow.getHeight() / 5) + (y * box->h) + box->h;
    }else{
        //do right side
        box->x = (3*(gWindow.getWidth() / 4))-(box->w/2);
        box->y = (gWindow.getHeight() / 5) + (y * box->h) + box->h;
    }
    //TODO the boost button
    if(y == 4){
        box->x = (gWindow.getWidth() / 2) - (box->w / 2);
        box->y = 2 * (gWindow.getHeight() / 3);
    }
    //the back button which lives at x,y = any, 5
    if(y == 5){
        //handle the special case
        box->x = 3 * (gWindow.getWidth() / 4);
        box->y = 5 * (gWindow.getHeight() / 6);
    }
}

//handle button events for settings page
void handleKeyPress(SDL_Event e, int* xOffset, int* yOffset, bool* goBack, bool* globalQuit){
    if(e.type == SDL_KEYUP){
        //check for up and down motion
        if(e.key.keysym.sym == SDLK_UP){
            --*yOffset;
            //check boundaries, be careful of column
            if(*yOffset < 0){
                *yOffset = 5;
            }
            Mix_PlayChannel(-1, gClickSound, 0);
        }
        if(e.key.keysym.sym == SDLK_DOWN){
            ++*yOffset;
            //check boundaries
            if(*yOffset > 5){
                *yOffset = 0;
            }
            Mix_PlayChannel(-1, gClickSound, 0);
        }
        if(e.key.keysym.sym == SDLK_LEFT){
            //only do this if we are not on the back button
            if(*yOffset < 4){
                --*xOffset;
                if(*xOffset < 0){
                    *xOffset = 1;
                }
                Mix_PlayChannel(-1, gClickSound, 0);
            }
        }
        if(e.key.keysym.sym == SDLK_RIGHT){
            //only move if not on done button
            if(*yOffset < 4){
                ++*xOffset;
                if(*xOffset > 1){
                    *xOffset = 0;
                }
                Mix_PlayChannel(-1, gClickSound, 0);
            }
        }
        if(e.key.keysym.sym == SDLK_RETURN){
            //if the user highlights a setting we want to have an indicator pop up to have them
            //enter a new setting
            if(*yOffset == 5){
                //user wants to go back
                *goBack = true;
                Mix_PlayChannel(-1, gSelectSound, 0);
                //save any changes
                if(!saveSettings(settingsFile, playerControls)){
                    printf("Could not save settings!\n");
                }
            }else{
                //then we need to change the settings file and the player prompts
                Mix_PlayChannel(-1, gSelectSound, 0);
                changeSettingHotkey(e, *xOffset, *yOffset, globalQuit);
                Mix_PlayChannel(-1, gClickSound, 0);
            }
        }
    }
}

//method to handle mouse motion events
void handleMouseEvent(SDL_Event e, int* xOffset, int *yOffset, bool* goBack, bool* globalQuit){
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN){
        //get the mouse position
        int xM, yM;
        SDL_GetMouseState(&xM, &yM);
        //need a rect to represent the different settings, use some constants to help
        int width = gWindow.getWidth() / 8;
        int height = gWindow.getHeight() / 10;
        int xStart = (gWindow.getWidth() / 4) - (width / 2);
        int yStart = (gWindow.getHeight() / 5) + height;
        //setup the rect
        SDL_Rect textBox = {xStart, yStart, width, height};
        //need to test each possible textBox location excluding the back button and boost
        for(int y = 0; y < 4; ++y){
            for(int x = 0; x < 2; ++x){
                textBox.x = xStart + (x * (gWindow.getWidth()/2));
                textBox.y = yStart + (y * height);
                //text mouse position
                if(checkMouseBoxCollision(xM, yM, textBox)){
                    //the mouse is over the corresponding textbox
                    if(*yOffset != y || *xOffset != x){
                        Mix_PlayChannel(-1, gClickSound, 0);//just got here, play sound
                    }
                    *xOffset = x;
                    *yOffset = y;
                    //check mouse button down
                    if(e.type == SDL_MOUSEBUTTONDOWN){
                        Mix_PlayChannel(-1, gSelectSound, 0);
                        changeSettingHotkey(e, *xOffset, *yOffset, globalQuit);
                        Mix_PlayChannel(-1, gClickSound, 0);
                    }
                }
            }
        }
        //set the textBox to the boost button
        textBox.x = (gWindow.getWidth() / 2) - (width / 2);
        textBox.y = 2 * (gWindow.getHeight() / 3);
        if(checkMouseBoxCollision(xM, yM, textBox)){
            if(*yOffset != 4){
                Mix_PlayChannel(-1, gClickSound, 0);//just got here, play sound
            }
            *yOffset = 4;
            if(e.type == SDL_MOUSEBUTTONDOWN){
                Mix_PlayChannel(-1, gSelectSound, 0);
                changeSettingHotkey(e, *xOffset, 4, globalQuit);
                Mix_PlayChannel(-1, gClickSound, 0);
            }
        }
        //set the textBox to the back button location
        textBox.x = 3 * (gWindow.getWidth() / 4);
        textBox.y = 5 * (gWindow.getHeight() / 6);
        if(checkMouseBoxCollision(xM, yM, textBox)){
            if(*yOffset != 5){
                Mix_PlayChannel(-1, gClickSound, 0);//just got here, play sound
            }
            *yOffset = 5;
            if(e.type == SDL_MOUSEBUTTONDOWN){
                *goBack = true;
                Mix_PlayChannel(-1, gSelectSound, 0);
                //save any changes
                if(!saveSettings(settingsFile, playerControls)){
                    printf("Could not save settings!\n");
                }
            }
        }
    }
}

//method to change hotkey
void changeSettingHotkey(SDL_Event e, int x, int y, bool* globalQuit){
    //need to loop while in method
    bool keyEntered = false;
    //need to start text entry
    SDL_StartTextInput();
    string newKey;
    //want to knwo if the key was actually changed
    bool keyChanged = false;
    while(!keyEntered){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                *globalQuit = true;
                keyEntered = true;
            }
            //handle resizing events
            gWindow.handleEvent(e);
            resizeUI(&e);
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_BACKSPACE || e.key.keysym.sym == SDLK_ESCAPE){
                    keyEntered = true;
                    Mix_PlayChannel(-1, gClickSound, 0);
                    break;
                }
                //check for special space bar entry
                if(e.key.keysym.sym == SDLK_SPACE && y == 4){
                    newKey = "Space";
                    keyEntered = true;
                    keyChanged = true;
                    Mix_PlayChannel(-1, gClickSound, 0);
                }
            }
            //make sure the key wasnt already entered
            else if(e.type == SDL_TEXTINPUT && !keyEntered){
                //for consistency always uses caps
                char key;
                key = toupper(e.text.text[0]);
                newKey += key;
                keyEntered = true;
                keyChanged = true;
                Mix_PlayChannel(-1, gClickSound, 0);
            }
            
            
        }
        //render to correct place
        SDL_Rect textLine = {0,0, gWindow.getWidth() / 4, gWindow.getHeight() / 7};
        gChangeSettingPrompt.render((gWindow.getWidth() - textLine.w)/2, (6 * gWindow.getHeight()) / 7, NULL, &textLine);
        SDL_RenderPresent(gWindow.getRenderer());
    }
    SDL_StopTextInput();
    if(keyChanged){
        if(y != 4){
            //once the new key has been entered we need to load it into prompts
            gPlayerPrompt[y + (x * 4)].free();
            //should check the rest of the controls to see if control already exists and if it does reassign it
            for(int i = 0; i < (sizeof(playerControls)/sizeof(playerControls[0])); ++i){
                if(playerControls[i] == newKey){
                    playerControls[i] = " ";
                    //rerender as blank
                    string blank = controlButton[i % 4] + playerControls[i];
                    if(!gPlayerPrompt[i].loadFromRenderedText(blank.c_str(), white)){
                        printf("Could not render player control prompt!\n");
                    }
                }
            }
            //load new setting
            playerControls[y + (x * 4)] = newKey;
            newKey = controlButton[y] + newKey;
            if(!gPlayerPrompt[y + (x * 4)].loadFromRenderedText(newKey.c_str(), white)){
                printf("Could not load new player prompt!\n");
            }
        }else{
            //case for boost button
            playerControls[8] = newKey;
            newKey = "Boost: " + newKey;
            if(!gPlayerPrompt[8].loadFromRenderedText(newKey.c_str(), white)){
                printf("Could not rerender new player prompt!\n");
            }
        }
        //once the new settings are set we need to load them into the controls and save them in settings
        updateControls(playerControls);
    }
}

//method to update the dot control scheme
void updateControls(string playerControls[]){
    //create a pointer array for controls
    const char* player1Controls[4];
    const char* player2Controls[4];
    for(int i = 0; i < (sizeof(player1Controls)/sizeof(player1Controls[0])); ++i){
        player1Controls[i] = NULL;
        player2Controls[i] = NULL;
        //after dec we can assign them
        player1Controls[i] = playerControls[i].c_str();
        player2Controls[i] = playerControls[i + 4].c_str();
    }
    //now set the dot control scheme
    player1.loadControls(player1Controls, playerControls[8].c_str());
    player2.loadControls(player2Controls, playerControls[8].c_str());
}

//method to save new settings to the settings file
bool saveSettings(string fileName, string playerControls[]){
    bool settingsSaved = true;
    //open the file
    fstream file(fileName.c_str(), ifstream::out);
    if(!file.is_open()){
        //the settings file doesnt exist, break
        printf("Could not find settings file at: %s\n", fileName.c_str());
        settingsSaved = false;
    }else{
        //write the new settings
        //use player prompts instead of string
        for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0])); ++i){
            if(playerControls[i] != " "){
                file << playerControls[i];
                file << endl;
            }else{
                file << "#";
                file << endl;
            }
        }
    }
    file.close();
    return settingsSaved;
}
#endif /* lSettingsMethods_h */
