//
//  lMenuMethods.h
//  SDL_2dot_game
//
//  Methods to navigate the menu handle menu input events
//
//  Created by William Cole on 3/22/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lMenuMethods_h
#define lMenuMethods_h
//top level decs
//method to handle menu key events
/**
    Handle key event on the menu screen
 
    @param e the event to handle
    @param offset the int representing the current selection
    @param globalQuit true if the user wants to quit
 
 */
void handleMenuButtonEvents(SDL_Event e, int* offset, bool* globalQuit);
//method to handle menu mouse events
/**
    Handle mouse events on the menu screen
 
     @param e the event to handle
     @param offset the int representing the current selection
     @param globalQuit true if the user wants to quit
 */
void handleMenuMouseEvents(SDL_Event e, int* offset, bool* globalQuit);
//method to load the settings page


//handle menu button events
void handleMenuButtonEvents(SDL_Event e, int* offset, bool* globalQuit){
    //navigate the menu and select option
    if(e.type == SDL_KEYUP){
        if(e.key.keysym.sym == SDLK_UP){
            //move selector up
            --*offset;
            //check boundaries
            if(*offset < 0){
                *offset = TOTAL_MENU_BUTTONS - 1;
            }
        }
        if(e.key.keysym.sym == SDLK_DOWN){
            ++*offset;
            //check boundaries
            if(*offset > TOTAL_MENU_BUTTONS - 1){
                *offset = 0;
            }
        }
        //once option is selected use offset to do things
        if(e.key.keysym.sym == SDLK_RETURN){
            switch (*offset) {
                case 0://play game
                    pregameSetup(globalQuit);
                    playingGame(globalQuit);
                    break;
                case 1://display settings screen
                    displaySettingsScreen(globalQuit);
                    break;
                case 2:
                    displayLeaderboardScreen(globalQuit);
                    break;
                case 3://user wants to quit
                    *globalQuit = true;
                    break;
                    
            }
        }
    }
}


//method to handle menu mouse events
void handleMenuMouseEvents(SDL_Event e, int* offset, bool* globalQuit){
    //check if mouse if over button
    if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN){
        //get the mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);
        //clone of buttonBubble
        SDL_Rect button ={(3*gWindow.getWidth())/4,(gWindow.getHeight()/3),gWindow.getWidth()/5, gWindow.getHeight()/6};
        //now check if the mouse if over any of the boxs
        for(int i = 0; i < TOTAL_MENU_BUTTONS; ++i){
            //adjust the buttons position
            button.y = (gWindow.getHeight()/3) + (i * (gWindow.getHeight()/6));
            //check mouse box collision
            if(checkMouseBoxCollision(x, y, button)){
                //the mouse is over the button
                *offset = i;
                //after offset is set check for button down event
                if(e.type == SDL_MOUSEBUTTONDOWN){
                    //again check the offset and do the appropriate thing
                    switch (*offset) {
                        case 0://play game
                            pregameSetup(globalQuit);
                            playingGame(globalQuit);
                            break;
                        case 1://display settings screen
                            displaySettingsScreen(globalQuit);
                            break;
                        case 2:
                            displayLeaderboardScreen(globalQuit);
                            break;
                        case 3://user wants to quit
                            *globalQuit = true;
                            break;
                    }
                }
            }
        }
    }
}

#endif /* lMenuMethods_h */
