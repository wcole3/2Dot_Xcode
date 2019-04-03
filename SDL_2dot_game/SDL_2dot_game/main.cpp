//
//  main.cpp
//  SDL_2dot_game
//
//  Created by William Cole on 2/19/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include "include/lUtilityFunc.h"
#include "include/lTexture.h"
#include "include/lTimer.h"
#include "include/lRigidDot.h"
#include "include/lWindow.h"
#include "include/lTile.h"
#include "include/globals.h"
#include "include/lInitMethods.h"
#include "include/lSettingsMethods.h"
#include "include/lLeaderboardMethods.h"
#include "include/lGameMethods.h"
#include "include/lMenuMethods.h"

//implementing settings and leaderboard



int main(int argc, const char * argv[]) {
    // lets get down to business
    if(!init()){
        printf("Could not initialize!\n");
    }else{
        if(!loadMedia()){
            printf("Could not load media!\n");
        }else{
            //start main loop
            SDL_Event e;
            bool quit = false;
            //need an int to move the highlighter box
            int offset = 0;
            while(!quit){
                while(SDL_PollEvent(&e) != 0){
                    if(e.type == SDL_QUIT){
                        quit = true;
                    }
                    //handle window events and resizing
                    gWindow.handleEvent(e);
                    resizeUI(&e);
                    //handle menu interaction
                    handleMenuButtonEvents(e, &offset, &quit);
                    handleMenuMouseEvents(e, &offset, &quit);
                }
                //here we render the menu
                gWindow.render();
                //render the menu background
                SDL_Rect screen = {0,0, gWindow.getWidth(), gWindow.getHeight()};
                gMenu.render(0, 0, NULL, &screen);
                //want a box to highlight the current choice and to help scale the button text
                SDL_Rect buttonBubble ={(3*gWindow.getWidth())/4,(gWindow.getHeight()/3)+(offset*(gWindow.getHeight()/6)),gWindow.getWidth()/5, gWindow.getHeight()/6};
                //render the menu buttons
                for(int i = 0; i < (sizeof(gMenuPrompts)/sizeof(gMenuPrompts[0])); ++i){
                    gMenuPrompts[i].render((3*gWindow.getWidth())/4, (gWindow.getHeight()/3)+(i*buttonBubble.h), NULL, &buttonBubble);
                }
                //render the highlight box
                SDL_SetRenderDrawColor(gWindow.getRenderer(), 100, 100, 200, 100);
                SDL_RenderFillRect(gWindow.getRenderer(), &buttonBubble);
                SDL_RenderPresent(gWindow.getRenderer());
            }
        }
    }
    //free all resources and close libs
    close();
    return 0;
}
