//
//  lGameMethods.h
//  SDL_2dot_game
//
//
//  Methods to control game loop logic
//
//
//  Created by William Cole on 3/18/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lGameMethods_h
#define lGameMethods_h
//forward declarations here

//method to main game loop
void playingGame(bool* globalQuit);
//method to check if user wants to play again
bool playAgain(SDL_Renderer* renderer, lTexture* splash, bool* globalQuit, float time);
//method to display the pregame instructions and ask for user to start the game
void pregameSetup(bool* globalQuit);



//the actual game loop
void playingGame(bool* globalQuit){
    if(*globalQuit != true){
        //the loop condition for playing the game
        bool played = true;
        //event to get user inputs
        SDL_Event e;
        //reference to endgame splash screen
        lTexture* endgameSplash = NULL;
        //setup timer for physics
        lTimer frameTicker = lTimer();
        //need a seperate timer for countdown
        lTimer countdownTicker = lTimer();
        //load the starting text
        int timeLimit = 20;
        string startString = "Time Remaining: ";
        SDL_Color countdownColor = {0,0,0};
        gCountdownText.loadFromRenderedText(startString + std::to_string(timeLimit), countdownColor);
        countdownTicker.start();
        while(played){
            while(SDL_PollEvent(&e) != 0){
                //test for quit
                if(e.type == SDL_QUIT){
                    *globalQuit = true;
                    played = false;
                }
                //otherwise handle dot events
                //only handle player events if the player hasnt finished
                if(!player1.isFinished()){
                    player1.handleEvent(e);
                }else{
                    //TODO write out time it took for player 1
                }
                if(!player2.isFinished()){
                    player2.handleEvent(e);
                }else{
                    //TODO write out the timer it took for player 2
                }
                gWindow.handleEvent(e);
                //update the UI
                resizeUI(&e);
            }
            //here we render stuff
            //first move dot
            float time = (frameTicker.getTime() / 1000.f);
            player1.move(time, player2.getCollider(), gTiles);
            player1.setCamera(camera1);
            player2.move(time, player1.getCollider(), gTiles);
            player2.setCamera(camera2);
            //restart time
            frameTicker.start();
            //now render
            gWindow.render();
            SDL_RenderSetViewport(gWindow.getRenderer(), &player1Screen);
            //render background
            for(int i = 0; i < TOTAL_TILES; ++i){
                //render each tile
                gTiles[i]->render(&gTileSpriteSheet, camera1);
            }
            //check if controls need to be rendered
            if(!player1.hasMoved()){
                //render the controls
                for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0]))/2; ++i){
                    gPlayerPrompt[i]. render(80, 70 + (i * gPlayerPrompt[i].getHeight()));
                }
            }
            player1.render(camera1);
            player2.render(camera1);
            //render the second dots screen
            SDL_RenderSetViewport(gWindow.getRenderer(), &player2Screen);
            for(int i = 0; i < TOTAL_TILES; ++i){
                gTiles[i]->render(&gTileSpriteSheet, camera2);
            }
            //render player 2 controls
            if(!player2.hasMoved()){
                //render the controls
                for(int i = 0; i < (sizeof(gPlayerPrompt)/sizeof(gPlayerPrompt[0]))/2; ++i){
                    gPlayerPrompt[i+4]. render(camera2.w - 160, (camera2.h - 160) + (i * gPlayerPrompt[i+4].getHeight()));
                }
            }
            player1.render(camera2);
            player2.render(camera2);
            SDL_RenderSetViewport(gWindow.getRenderer(), NULL);
            SDL_RenderFillRect(gWindow.getRenderer(), &split);
            //now render the time remaining
            float timeLeft = timeLimit - (countdownTicker.getTime() / 1000.f);
            char timeBuffer [10];
            sprintf(timeBuffer, "%.3f", timeLeft);
            //now rerender the countdown text
            gCountdownText.loadFromRenderedText(startString + timeBuffer, countdownColor);
            gCountdownText.render((gWindow.getWidth() - gCountdownText.getWidth())/2, 0);
            SDL_RenderPresent(gWindow.getRenderer());
            //or if the limit has passed or both players are done
            if(timeLeft < 0 || (player1.isFinished() && player2.isFinished())){
                float finishTime;
                countdownTicker.pause();
                //check if game was one or lost
                if(timeLeft > 0){
                    //game was won, set splash screen to winner
                    endgameSplash = &gWinSplash;
                    finishTime = (countdownTicker.getTime() / 1000.f);
                }else{
                    //game was lost splash screen is loser
                    endgameSplash = &gLoseSplash;
                    finishTime = 0;
                }
                //regardless of end condition we reset the game
                player1.reset();
                player1.setCamera(camera1);
                player2.reset();
                player2.setCamera(camera2);
                countdownTicker.stop();
                if(playAgain(gWindow.getRenderer(), endgameSplash, globalQuit, finishTime)){
                    countdownTicker.start();
                    //clear out splash screen
                    SDL_RenderClear(gWindow.getRenderer());
                    endgameSplash = NULL;
                }else{
                    SDL_RenderClear(gWindow.getRenderer());
                    endgameSplash = NULL;
                    played = false;
                }
            }
        }
    }
}

//method to check if the user wants to play again, this got a lot bigger than I intended (TODO chop this up)
bool playAgain(SDL_Renderer* renderer, lTexture* splashScreen, bool* globalQuit, float time){
    //reset the letter textures
    string defaultName[MAX_LETTERS] = {"N", "E", "W"};
    for(int i = 0; i < (sizeof(gLetters)/sizeof(gLetters[0])); ++i){
        gLetters[i].loadFromRenderedText(defaultName[i].c_str(), blue);
    }
    //box to highlight the current letter
    SDL_Rect currentLetter = {(3*gWindow.getWidth()) / 4, (gWindow.getHeight() / 7) + ((LEADERBOARD_LINES + 1) * (gWindow.getHeight() / 10)), (gWindow.getHeight() / 12), (gWindow.getHeight() / 10)};
    //index for the current letter
    int letterIndex = 0;
    //local text textures for the leaderboard
    lTexture lLeaderboardHeader = lTexture(gWindow.getRenderer());
    lTexture lUserTime = lTexture(gWindow.getRenderer());
    lTexture lPlayAgainPrompt = lTexture(gWindow.getRenderer());
    lTexture lEnterNewNamePrompt = lTexture(gWindow.getRenderer());
    lEnterNewNamePrompt.setFont(gFont);
    lPlayAgainPrompt.setFont(gFont);
    lUserTime.setFont(gFont);
    lLeaderboardHeader.setFont(gFont);
    string header = "Current Leaders";
    if(!lLeaderboardHeader.loadFromRenderedText(header.c_str(), black)){
        printf("Could not load header texture!\n");
    }
    //load play again prompt
    string playAgainPrompt = "Play again? (y/n)";
    if(!lPlayAgainPrompt.loadFromRenderedText(playAgainPrompt.c_str(), black)){
        printf("Could not load play again prompt!\n");
    }
    //load the enter new name prompt
    string newNamePrompt = "Enter new name: ";
    if(!lEnterNewNamePrompt.loadFromRenderedText(newNamePrompt.c_str(), blue)){
        printf("Could not load enter new name prompt!\n");
    }
    if(time > 0){
        //load a prompt of the users time if they won
        char userTime[10];
        sprintf(userTime, "%.3f", time);
        string newTime = "Your time: ";
        newTime += userTime;
        if(!lUserTime.loadFromRenderedText(newTime.c_str(), blue)){
            printf("Could not load user time prompt!\n");
        }
        
    }
    //need to chek for global closes
    bool playAgain = false;
    //variables for control loop
    bool done = false;
    SDL_Event e;
    //highlight box for new winner
    SDL_Rect highlightBox = {gWindow.getWidth()/4, gWindow.getHeight() / 7 , gWindow.getWidth() / 2 ,gWindow.getHeight() / 10};
    //int index for a possible new high score
    int index = -1;
    //check for new entries
    bool highlight = newLeader(time, &index);
    if(highlight){
        //change the highlight box y value to the correct one
        highlightBox.y += (index + 1) * highlightBox.h;
    }
    //check for new name entered
    bool newNameEntered = false;
    SDL_StartTextInput();
    while(!done){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                //if the user want to quit
                *globalQuit = true;
                done = true;
            }
            //handle window resizing of the ui
            gWindow.handleEvent(e);
            resizeUI(&e);
            if(e.type == SDL_KEYDOWN && (newNameEntered && highlight)){
                if(e.key.keysym.sym == SDLK_y){
                    playAgain = true;
                    done = true;
                    SDL_Delay(100);
                    if(highlight){
                        //TODO save leaderboard
                        saveLeaderboard(leaderboardFile);
                    }
                }
                if(e.key.keysym.sym == SDLK_n){
                    done = true;
                    SDL_Delay(100);
                    if(highlight){
                        //TODO save leaderboard
                        saveLeaderboard(leaderboardFile);
                    }
                }
            }
            //handle event if user is entering a name
            if(highlight && !newNameEntered){
                getNewName(e, &currentLetter, globalQuit, &letterIndex, &newNameEntered, defaultName);
            }
        }
        //display the chosen splash screen
        SDL_RenderClear(renderer);
        SDL_Rect currentScreen = {0,0,gWindow.getWidth(), gWindow.getHeight()};
        SDL_Rect textLine = {gWindow.getWidth()/4, gWindow.getHeight() / 7 , gWindow.getWidth() / 2 ,gWindow.getHeight() / 10};
        splashScreen->render(0, 0, NULL, &currentScreen);
        //render the leaderboard
        
        lLeaderboardHeader.render(textLine.x, textLine.y, NULL, &textLine);
        for(int i = 0; i < (sizeof(gLeaderboardEntry)/sizeof(gLeaderboardEntry[0])); ++i){
            gLeaderboardEntry[i].render(textLine.x, textLine.y + ((i + 1) * textLine.h), NULL, &textLine);
        }
        //render play again prompt
        lPlayAgainPrompt.render(textLine.x, (9 * gWindow.getHeight()) / 10, NULL, &textLine);
        //render the usertimes
        if(time > 0){
            if(highlight){
                //the users time is on the board
                SDL_SetRenderDrawColor(gWindow.getRenderer(), 100, 50, 50, 100);
                SDL_RenderFillRect(gWindow.getRenderer(), &highlightBox);
                
                //TODO enter a new name into the board
                
                if(!newNameEntered){
                    for(int i = 0; i < MAX_LETTERS; ++i){
                        gLetters[i].render(((3*gWindow.getWidth()) / 4) + (i * currentLetter.w), (gWindow.getHeight() / 7) + ((LEADERBOARD_LINES + 1) * (gWindow.getHeight() / 10)), NULL, &currentLetter);
                    }
                    lEnterNewNamePrompt.render(textLine.x, textLine.y + ((LEADERBOARD_LINES + 1) * textLine.h), NULL, &textLine);
                    SDL_RenderFillRect(gWindow.getRenderer(), &currentLetter);
                }else{
                    //name has been entered, we need to rerender the appropriate leaderboard entry
                    //build new name
                    leaderboardNames[index] = defaultName[0] + defaultName[1] + defaultName[2];
                    char header [5];
                    char line [10];
                    int rank = index + 1;
                    sprintf(header, "%d. ", rank);
                    sprintf(line, "%.3f", leaderboardScores[index]);
                    string entry = header + leaderboardNames[index] + " " + line;
                    gLeaderboardEntry[index].loadFromRenderedText(entry.c_str(), black);
                }
            }else{
                //users time didnt make the board but we want to show them still
                lUserTime.render(textLine.x, textLine.y + ((LEADERBOARD_LINES + 1) * textLine.h), NULL, &textLine);
            }
        }
        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput();
    lEnterNewNamePrompt.free();
    lLeaderboardHeader.free();
    lPlayAgainPrompt.free();
    lUserTime.free();
    return playAgain;
}

//method for the pregame setup, this is currently just rendering an instruction screen and getting a start
//signal from the user
void pregameSetup(bool* globalQuit){
    //flag to check if the user is ready
    bool userReady = false;
    SDL_Event e;
    //user key press prompt
    SDL_Color redText = {255,0,0};
    lTexture lUserPrompt = lTexture(gWindow.getRenderer());
    lUserPrompt.setFont(gFont);
    //need to set the text string here
    string promptString = "Press " + playerControls[3] + " and " + playerControls[6] + " to start!";
    if(!lUserPrompt.loadFromRenderedText(promptString.c_str(), redText)){
        printf("Could not load prompt string!\n");
    }
    //while waiting on the user display the instruction screen
    while(!userReady){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                userReady = true;
                *globalQuit = true;
            }
            //handle window event and UI resizing
            gWindow.handleEvent(e);
            resizeUI(&e);
            //check if user is ready, ready signal is the button for player 1 right and player 2 left held down
            if(e.type == SDL_KEYDOWN){
                //get the keystate
                const Uint8* keyState = NULL;
                keyState = SDL_GetKeyboardState(NULL);
                if(keyState[player1.getControlButton(3)] && keyState[player2.getControlButton(2)]){
                    userReady = true;
                }
            }
        }
        //in the meantime display the pregame splash
        SDL_RenderClear(gWindow.getRenderer());
        SDL_Rect screen = {0,0,gWindow.getWidth(), gWindow.getHeight()};
        gPregameSplash.render(0, 0, NULL, &screen);
        //render the prompt
        //scale the text up slightly
        SDL_Rect textBox = {0,0,(4*gWindow.getWidth()/5), (gWindow.getHeight()/8)};
        lUserPrompt.render((gWindow.getWidth() - textBox.w)/2, (3*(gWindow.getHeight()- textBox.h))/4, NULL, &textBox);
        SDL_RenderPresent(gWindow.getRenderer());
    }
    //get rid of user prompt
    lUserPrompt.free();
}

#endif /* lGameMethods_h */
