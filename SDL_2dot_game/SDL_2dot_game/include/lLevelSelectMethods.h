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
        //render level imgs
        gWindow.render();
        for(int i = 0; i < TOTAL_LEVELS; ++i){
            if(i < 2){
                gImageTextures[gTest]->render(selectionBox.x + ((i % 2) * 2 * selectionBox.w), selectionBox.y, NULL, &selectionBox);
            }
            else{
                gImageTextures[gTest]->render(selectionBox.x + ((i % 2) * 2 * selectionBox.w), selectionBox.y + (2 * selectionBox.h), NULL, &selectionBox);
            }
        }
        SDL_SetRenderDrawColor(gWindow.getRenderer(), highlighter.r, highlighter.g, highlighter.b, 100);
        SDL_RenderFillRect(gWindow.getRenderer(), &highlightBox);
        SDL_RenderPresent(gWindow.getRenderer());
    }
    //the level has been select, play the level
    currentLevel = xOffset + (2 * yOffset);
    pregameSetup(globalQuit);
    playGame(globalQuit, false);
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
