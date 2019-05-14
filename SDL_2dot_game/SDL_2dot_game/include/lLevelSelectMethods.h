//
//  lLevelSelectMethods.h
//  SDL_2dot_game
//
//  Handle selecting a level to play
//
//  Created by William Cole on 5/9/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lLevelSelectMethods_h
#define lLevelSelectMethods_h

//render options and select a level
void selectLevel(bool* globalQuit);
//handle key presses
void handleSelectKeyEvents(SDL_Event e, bool* levelSelected, int* xOffset, int* yOffset);
//handle mouse events
void handleSelectMouseEvents(SDL_Event e, bool* levelSelected, int* xOffset, int* yOffset);

void selectLevel(bool* globalQuit){
    //need labels for each level and the level size
    lTexture lLevelNameLabels[TOTAL_LEVELS];
    lTexture lLevelSizeLabels[TOTAL_LEVELS];
    for(int i = 0; i < TOTAL_LEVELS; ++i){
        lLevelNameLabels[i] = lTexture(gWindow.getRenderer());
        lLevelSizeLabels[i] = lTexture(gWindow.getRenderer());
        lLevelSizeLabels[i].setFont(gFont);
        lLevelNameLabels[i].setFont(gFont);
        if(!lLevelNameLabels[i].loadFromRenderedText("Level " + to_string(i), black) || !lLevelSizeLabels[i].loadFromRenderedText(to_string(LEVEL_WIDTH[i] / TILE_WIDTH) + " x " + to_string(LEVEL_HEIGHT[i] / TILE_HEIGHT), black)){
            printf("Could not load level label textures!\n");
        }
    }
    bool levelSelected = false;
    SDL_Event e;
    int xOffset = 0;
    int yOffset = 0;
    while(!levelSelected){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                *globalQuit = true;
                levelSelected = true;
            }
            gWindow.handleEvent(e);
            resizeUI(&e);
            //handle key and mouse events
            handleSelectKeyEvents(e, &levelSelected, &xOffset, &yOffset);
            handleSelectMouseEvents(e, &levelSelected, &xOffset, &yOffset);
        }
        SDL_Rect selectionBox = {gWindow.getWidth() / 8, gWindow.getHeight() / 8, gWindow.getWidth() / 4, gWindow.getHeight() / 4};
        SDL_Rect highlightBox = {
                                    (selectionBox.x + (xOffset * 2 * selectionBox.w)) - 10,
                                    (selectionBox.y + (yOffset * 2 * selectionBox.h)) - 10,
                                    selectionBox.w + 20,
                                    selectionBox.h + 20
                                };
        SDL_Rect textLine = {selectionBox.x, selectionBox.y - 10, selectionBox.w, gWindow.getHeight() / 12};
        //render level imgs
        gWindow.render();
        for(int i = 0; i < TOTAL_LEVELS; ++i){
            int index = gLevel0Img + i;
            if(i < 2){
                gImageTextures[index]->render(selectionBox.x + ((i % 2) * 2 * selectionBox.w), selectionBox.y, NULL, &selectionBox);
                lLevelNameLabels[i].render(textLine.x + ((i % 2) * 2 * textLine.w), textLine.y - textLine.h, NULL, &textLine);
                lLevelSizeLabels[i].render(textLine.x + ((i % 2) * 2 * textLine.w), textLine.y + 20 + selectionBox.h, NULL, &textLine);
            }
            else{
                gImageTextures[index]->render(selectionBox.x + ((i % 2) * 2 * selectionBox.w), selectionBox.y + (2 * selectionBox.h), NULL, &selectionBox);
                lLevelNameLabels[i].render(textLine.x + ((i % 2) * 2 * textLine.w), (textLine.y - textLine.h) + (2* selectionBox.h), NULL, &textLine);
                lLevelSizeLabels[i].render(textLine.x + ((i % 2) * 2 * textLine.w), (textLine.y + 20 + selectionBox.h) + (2 * selectionBox.h), NULL, &textLine);
            }
        }
        SDL_SetRenderDrawColor(gWindow.getRenderer(), highlighter.r, highlighter.g, highlighter.b, 100);
        SDL_RenderFillRect(gWindow.getRenderer(), &highlightBox);
        SDL_RenderPresent(gWindow.getRenderer());
    }
    //the level has been select, play the level
    if(!*globalQuit){
        currentLevel = xOffset + (2 * yOffset);
        pregameSetup(globalQuit);
        playGame(globalQuit, false);
    }
    //free allocated label textures
    for(int i = 0; i < TOTAL_LEVELS; ++i){
        lLevelNameLabels[i].free();
        lLevelSizeLabels[i].free();
    }
}

void handleSelectKeyEvents(SDL_Event e, bool* levelSelected, int* xOffset, int* yOffset){
    if(e.type == SDL_KEYUP && e.key.repeat == 0){
        switch (e.key.keysym.sym) {
            case SDLK_RIGHT:
                ++*xOffset;
                if(*xOffset > 1){
                    *xOffset = 0;
                }
                break;
            case SDLK_LEFT:
                --*xOffset;
                if(*xOffset < 0){
                    *xOffset = 1;
                }
                break;
            case SDLK_UP:
                ++*yOffset;
                if(*yOffset > 1){
                    *yOffset = 0;
                }
                break;
            case SDLK_DOWN:
                --*yOffset;
                if(*yOffset < 0){
                    *yOffset = 1;
                }
                break;
            case SDLK_RETURN:
                *levelSelected = true;
                break;
            default:
                break;
        }
    }
}

void handleSelectMouseEvents(SDL_Event e, bool* levelSelected, int* xOffset, int* yOffset){
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN){
        //make the box
        SDL_Rect levelBox = {gWindow.getWidth() / 8, gWindow.getHeight() / 8, gWindow.getWidth() / 4, gWindow.getHeight() / 4};
        //get mouse position
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);
        for(int x = 0; x < 2; ++x){
            for(int y = 0; y < 2; ++y){
                levelBox.x = (gWindow.getWidth() / 8) + (x * 2 * levelBox.w);
                levelBox.y = (gWindow.getWidth() / 8) + (y * 2 * levelBox.h);
                if(checkMouseBoxCollision(mX, mY, levelBox)){
                    *xOffset = x;
                    *yOffset = y;
                    if(e.type == SDL_MOUSEBUTTONDOWN){
                        *levelSelected = true;
                    }
                }
            }
        }
    }
}

#endif /* lLevelSelectMethods_h */
