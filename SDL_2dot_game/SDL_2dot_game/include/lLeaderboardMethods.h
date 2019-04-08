//
//  lLeaderboardMethods.h
//  SDL_2dot_game
//
//  Methods to navigate Leaderboard screen
//
//  Created by William Cole on 3/22/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lLeaderboardMethods_h
#define lLeaderboardMethods_h

//display the leaderboard menu screen
/**
    Display the leaderboard screen
 
    @param globalQuit true if user wants to quit the program
 */
void displayLeaderboardScreen(bool* globalQuit);
//check the game finish time and return true if new leader
/**
    Check if a passed time belongs on the leaderboard
 
    @param finishTime the finishing time for the player
    @param newScore the index for the new top score
 
    @return true if score goes on the leaderboard
 */
bool newLeader(float finishTime, int* newScore);
//method to get new name for the board
/**
    Get a name for a new leaderboard entry
 
    @param e the SDL_Event to handle input
    @param letterBox a SDL_Rect highlighting current letter
    @param globalQuit true if user wants to quit the program
    @param index the index for the new high score
    @param nameEntered true if a new name has been entered
    @param name the name list for the leaderboard
 */
void getNewName(SDL_Event e, SDL_Rect* letterBox , bool* globalQuit, int* index, bool* nameEntered, string name[]);
//method to save the leaderboard to file
/**
    Saves the new leaderboard to the leaderboard file
 
    @param fileName the name of the leaderboard file
 */
void saveLeaderboard(string fileName);

//TODO add the ability to reset the leaderboard in game

//method to display the leaderboard screen
void displayLeaderboardScreen(bool* globalQuit){
    //load a back button
    lTexture lBackButton = lTexture(gWindow.getRenderer());
    lBackButton. setFont(gFont);
    if(!lBackButton.loadFromRenderedText("Back", black)){
        printf("Could not load back button texture!\n");
    }
    bool done = false;
    SDL_Event e;
    bool renderBox = false;
    while(!done){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                //user wants to quit
                *globalQuit = true;
                done = true;
            }
            //otherwise handle window events
            gWindow.handleEvent(e);
            resizeUI(&e);
            if(e.type == SDL_KEYUP || e.type == SDL_KEYDOWN){
                //just want to flicker the back highlight
                if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN){
                    renderBox = true;
                }
                if(e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_RETURN && e.key.repeat == 0){
                    done = true;
                    Mix_PlayChannel(-1, gSelectSound, 0);
                }
            }
            if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN){
                //if the mouse is over the back button and pressed
                int xM, yM;
                SDL_GetMouseState(&xM, &yM);
                SDL_Rect textLine = {3 * (gWindow.getWidth() / 4),5 * (gWindow.getHeight() / 6), gWindow.getWidth() / 8, gWindow.getHeight() / 10};
                if(checkMouseBoxCollision(xM, yM, textLine)){
                    //we are over the back button
                    renderBox = true;
                    if(e.type == SDL_MOUSEBUTTONDOWN){
                        done = true;
                        Mix_PlayChannel(-1, gSelectSound, 0);
                    }
                }else{
                    renderBox = false;
                }
            }
        }
        //render the screen
        gWindow.render();
        SDL_Rect screen = { 0, 0, gWindow.getWidth(), gWindow.getHeight() };
        SDL_Rect textLine = {3 * (gWindow.getWidth() / 4),5 * (gWindow.getHeight() / 6), gWindow.getWidth() / 2, gWindow.getHeight() / 10};
        gLeaderboardScreen.render(0, 0, NULL, &screen);
        //render the leaderboard
        
        for(int i = 0; i < (sizeof(gLeaderboardEntry)/sizeof(gLeaderboardEntry[0])); ++i){
            gLeaderboardEntry[i].render((gWindow.getWidth() - textLine.w)/2, (gWindow.getHeight()/4) + (i * textLine.h), NULL, &textLine);
        }
        textLine.w = textLine.w / 4;
        lBackButton.render(textLine.x, textLine.y, NULL, &textLine);
        //render the highlight box
        if(renderBox){
            SDL_SetRenderDrawColor(gWindow.getRenderer(), 150, 150, 150, 100);
            SDL_RenderFillRect(gWindow.getRenderer(), &textLine);
        }
        SDL_RenderPresent(gWindow.getRenderer());
    }
    lBackButton.free();
}

//check the finish time
bool newLeader(float time, int* newScore){
    //tag to check if the time is better than the exist ones on the board
    bool newLeader = false;
    //if the time was 0 then the game was lost and there cannot be a new winner
    if(time <= 0){
        return newLeader;
    }
    //index of the winner
    int index = -1;
    //loop from the slowest to fastest times
    for(int i = (LEADERBOARD_LINES - 1); i > -1; --i){
        if(time < leaderboardScores[i]){
            newLeader = true;
            index = i;
            *newScore = i;
        }
    }
    //now we have the index of where the new winner is, we need add it into the list
    if(newLeader){
        //copy the score and name array
        float oldScores[LEADERBOARD_LINES];
        string oldNames[LEADERBOARD_LINES];
        for(int i = 0; i < LEADERBOARD_LINES; ++i){
            oldScores[i] = leaderboardScores[i];
            oldNames[i] = leaderboardNames[i];
        }
        for(int i = index; i < (LEADERBOARD_LINES - 1); ++i){
            //move all the elements below the new entry down
            leaderboardScores[i+1] = oldScores[i];
            leaderboardNames[i+1] = oldNames[i];
        }
        //now set the new entry
        leaderboardNames[index] = "NEW";
        leaderboardScores[index] = time;
        //now we need to rerender all of the leaderboard entries
        for(int i = 0; i < LEADERBOARD_LINES; ++i){
            char header [5];
            char line [10];
            int rank = i + 1;
            sprintf(header, "%d. ", rank);
            sprintf(line, "%.3f", leaderboardScores[i]);
            string entry = header + leaderboardNames[i] + " " + line;
            if(!gLeaderboardEntry[i].loadFromRenderedText(entry.c_str(), black)){
                printf("Could not load leaderboard entry #: %d!\n", i);
            }
        }
    }
    return newLeader;
}

//method to get a new name from user for board
void getNewName(SDL_Event e, SDL_Rect* letterBox ,bool* globalQuit, int* currentIndex, bool* nameEntered, string name[]){
    //use text input
    if(e.type == SDL_QUIT){
        *globalQuit = true;
        *nameEntered = true;
    }
    gWindow.handleEvent(e);
    resizeUI(&e);
    if(e.type == SDL_KEYDOWN){
        //use left and right to move current index
        if(e.key.keysym.sym == SDLK_LEFT && e.key.repeat == 0){
            --*currentIndex;
            if(*currentIndex < 0){
                *currentIndex = (MAX_LETTERS - 1);
            }
            //set the letter box
            letterBox->x = ((3*gWindow.getWidth()) / 4) + (*currentIndex * (gWindow.getHeight() / 12));
        }
        else if(e.key.keysym.sym == SDLK_RIGHT && e.key.repeat == 0){
            ++*currentIndex;
            if(*currentIndex > (MAX_LETTERS - 1)){
                *currentIndex = 0;
            }
            letterBox->x = ((3*gWindow.getWidth()) / 4) + (*currentIndex * (gWindow.getHeight() / 12));
        }
        //use tab to move as well
        else if(e.key.keysym.sym == SDLK_TAB && e.key.repeat == 0){
            ++*currentIndex;
            if(*currentIndex > (MAX_LETTERS - 1)){
                *currentIndex = 0;
            }
            letterBox->x = ((3*gWindow.getWidth()) / 4) + (*currentIndex * (gWindow.getHeight() / 12));
        }
        else if(e.key.keysym.sym == SDLK_RETURN){
            //name has been entered
            *nameEntered = true;
        }
    }
    else if(e.type == SDL_TEXTINPUT){
        char letter = e.text.text[0];
        name[*currentIndex] = letter;
        gLetters[*currentIndex].loadFromRenderedText(name[*currentIndex].c_str(), blue);
    }
}

//method to save the current leaderboard to file
void saveLeaderboard(string fileName){
    //open the current file
    fstream file(fileName.c_str(), ifstream::out);
    if(file.is_open()){
        //write out to file
        for(int i = 0; i < LEADERBOARD_LINES; ++i){
            file << leaderboardNames[i] << '\t' << leaderboardScores[i] << endl;
        }
    }else{
        printf("Could not save leaderboard to file!\n");
    }
    file.close();
}

#endif /* lLeaderboardMethods_h */
