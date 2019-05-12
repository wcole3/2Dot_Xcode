//
//  lModeSelectionMethods.h
//  SDL_2dot_game
//
//  Methods for selecting whcih game mode to play
//
//  Created by William Cole on 5/9/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lModeSelectionMethods_h
#define lModeSelectionMethods_h

//method to display the mode selection screen; two possible choices full run or play a specific level
void showModeSelection(bool* globalQuit);
//handle key events on screen
void handleMSKeyEvents(SDL_Event e, bool* optionSelected, int* mode);
//handle mouse events
void handleMSMouseEvents(SDL_Event e, bool* optionSelected, int* mode);
//select the approprate game mode
void showModeSelection(bool* globalQuit){
    //need texture for the labels
    lTexture lFullRunText = lTexture(gWindow.getRenderer());
    lTexture lSingleLevelText = lTexture(gWindow.getRenderer());
    lFullRunText.setFont(gFont);
    lSingleLevelText.setFont(gFont);
    if(!lFullRunText.loadFromRenderedText("Full Run", black) || !lSingleLevelText.loadFromRenderedText("Level Select", black)){
        printf("Could not load label textures!\n");
    }
    bool modeSelected = false;
    SDL_Event e;
    int mode = 0;//which mode does the user want to play
    while(!modeSelected){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                *globalQuit = true;
                modeSelected = true;
            }
            //handle window events and ui resizing
            gWindow.handleEvent(e);
            resizeUI(&e);
            //handle key and mouse events
            handleMSKeyEvents(e, &modeSelected, &mode);
            handleMSMouseEvents(e, &modeSelected, &mode);
        }
        //selection box
        SDL_Rect selectionBox = {gWindow.getWidth() / 8, gWindow.getHeight() / 4, gWindow.getWidth() / 4, gWindow.getHeight() / 4};
        SDL_Rect textBox = {(gWindow.getWidth() - (8 * (gWindow.getWidth()/ 10))) / 2, selectionBox.y + selectionBox.h, (8 * (gWindow.getWidth()/ 10)), gWindow.getHeight() / (3 - mode)};
        SDL_Rect highlightBox = {(selectionBox.x + (mode * 2 * selectionBox.w)) - 10, selectionBox.y - 10, selectionBox.w + 20, selectionBox.h + 20};
        SDL_Rect textLine = {selectionBox.x, highlightBox.y - (gWindow.getHeight() / 10), selectionBox.w, gWindow.getHeight() / 10};
        //render screen
        gWindow.render();
        //render images
        gImageTextures[gLevel3Img]->render(selectionBox.x, selectionBox.y, NULL, &selectionBox);
        gImageTextures[gLevel0Img]->render(gWindow.getWidth() - (selectionBox.x + selectionBox.w), selectionBox.y, NULL, &selectionBox);
        //render labels
        lFullRunText.render(textLine.x, textLine.y, NULL, &textLine);
        lSingleLevelText.render(textLine.x + (2 * selectionBox.w), textLine.y, NULL, &textLine);
        SDL_SetRenderDrawColor(gWindow.getRenderer(), highlighter.r, highlighter.g, highlighter.b, 100);
        //draw accompanying text for current mode
        switch (mode) {
            case 0:
                gImageTextures[gFullRunTextImg]->render(textBox.x, textBox.y, NULL, &textBox);
                break;
            case 1:
                gImageTextures[gSingleLevelTextImg]->render(textBox.x, textBox.y, NULL, &textBox);
                break;
            default:
                break;
        }
        SDL_RenderFillRect(gWindow.getRenderer(), &highlightBox);
        SDL_RenderPresent(gWindow.getRenderer());
    }
    //selection has been made, do selection specific things
    //only if user isnt quitting
    if(!*globalQuit){
        switch (mode) {
            case 0:
                //play full run mode
                pregameSetup(globalQuit);
                playGame(globalQuit, true);
                break;
            case 1:
                //play a specific level
                selectLevel(globalQuit);
                break;
            default:
                break;
        }
    }
    //deallocate labels
    lFullRunText.free();
    lSingleLevelText.free();
}


void handleMSKeyEvents(SDL_Event e, bool* optionSelected, int* mode){
    if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                --*mode;
                if(*mode < 0){
                    *mode = 1;
                }
                break;
            case SDLK_RIGHT:
                ++*mode;
                if(*mode > 1){
                    *mode = 0;
                }
                break;
            case SDLK_RETURN:
                *optionSelected = true;
                break;
            default:
                break;
        }
    }
}

void handleMSMouseEvents(SDL_Event e, bool* optionSelected, int* mode){
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN){
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);//find mouse positions
        //define the selection locations
        SDL_Rect modeBoxes = {0, gWindow.getHeight() / 4, gWindow.getWidth() / 4, gWindow.getHeight() / 4};
        //test both locations
        for(int i = 0; i < 2; ++i){
            modeBoxes.x = (gWindow.getWidth() / 8) + (i * 2 * modeBoxes.w);
            if(checkMouseBoxCollision(mX, mY, modeBoxes)){
                *mode = i;
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    *optionSelected = true;
                }
            }
        }
    }
}
#endif /* lModeSelectionMethods_h */
