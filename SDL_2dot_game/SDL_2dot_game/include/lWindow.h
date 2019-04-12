//
//  lWindow.h
//
//  wrapper class for window to accomodate window events
//
//  Created by William Cole on 1/5/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lWindow_h
#define lWindow_h
#include <sstream>
using namespace std;

//wrapper class for SDL windows
class lWindow{
public:
    lWindow();
    ~lWindow();
    //method to destroy window
    /**
        Destroy window and renderer
     */
    void free();
    //method to set up window and renderer
    /**
        Setup the window and renderer
     
        @param screenWidth the width of the window
        @param screenHeight the height of the window
        @param title the title of the window
     
        @return true if setup has finished successfully
     */
    bool init(int screenWidth, int screenHeight, string title);
    //method to handle window events
    /**
        Handle window events
     
        @param e SDL_Event input
     */
    void handleEvent(SDL_Event& e);
    //method to render the window
    /**
        Set the renderer draw color and clear the renderer
     */
    void render();
    //method to focus on window
    /**
        Focus on the window
     */
    void focus();
    //method to get reference to window renderer
    /**
        @return the renderer associated with the window
     */
    SDL_Renderer* getRenderer(){return mRenderer;};
    //some method to get private variable values
    /**
        @return the width of the window
     */
    int getWidth(){return mWidth;};
    /**
        @return the height of the window
     */
    int getHeight(){return mHeight;};
    /**
        @return the window ID
     */
    int getWindowID(){return mWindowID;};
    /**
        @return true if mouse is over window
     */
    bool hasMouseFocus(){return mMouseFocus;};
    /**
        @return true if window has keyboard focus
     */
    bool hasKeyboardFocus(){return mKeyboardFocus;};
    /**
        @return true if the window is minimized
     */
    bool isMinimized(){return mMinimized;};
    /**
        @return true if the window is full screen
     */
    bool isFullScreen(){return mFullScreen;};
    /**
        @return true if the window is shown
     */
    bool isShown(){return mShown;};
    
private:
    //the window object
    SDL_Window* mWindow;
    //renderer object
    SDL_Renderer* mRenderer;
    //the window's width and height
    int mWidth;
    int mHeight;
    int mWindowID = 0;
    int mWindowDisplayID = 0;
    //an outside definition of the max number fo connected displays
    int TOTAL_DISPLAYS = 0;
    //the bounds of each display
    SDL_Rect* mDisplayBounds = NULL;
    //bools to determine the state of the window
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mMinimized;
    bool mFullScreen;
    bool mShown;
    
};

lWindow::lWindow(){
    //initialize all varibales
    mWindow = NULL;
    mRenderer = NULL;
    mWidth = 0;
    mHeight = 0;
    mMinimized = false;
    mFullScreen = false;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mShown = true;
    
}

lWindow::~lWindow(){
    free();
}

void lWindow::free(){
    //destroy the window
    if(mRenderer!=NULL){
        SDL_DestroyRenderer(mRenderer);
        mRenderer = NULL;
        if(mWindow!=NULL){
            SDL_DestroyWindow(mWindow);
            mWindow = NULL;
            mWidth = 0;
            mHeight = 0;
        }
    }
}
//setup window and renderer
bool lWindow::init(int width, int height, string title){
    bool successFlag = true;
    //need to setup the window
    mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(mWindow == NULL){
        printf("Could not create window #%s! SDL error: %s\n", to_string(getWindowID()).c_str(),SDL_GetError());
        successFlag = false;
        
    }else{
        //get window id
        mWindowID = SDL_GetWindowID(mWindow);
        //get display id
        mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
        //get the total number of displays
        TOTAL_DISPLAYS = SDL_GetNumVideoDisplays();
        if(TOTAL_DISPLAYS < 2){
            printf("Only one display connected!\n");
        }
        //create renderer
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
        if(mRenderer == NULL){
            printf("Could not create renderer for window %s! SDL error: %s\n", to_string(getWindowID()).c_str(), SDL_GetError());
            successFlag = false;
            //need to destory window if this fails
            SDL_DestroyWindow(mWindow);
            mWindow = NULL;
        }else{
            SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
            mMouseFocus = true;
            mKeyboardFocus = true;
            mShown = true;
            mWidth = width;
            mHeight = height;
            //setup the display bounds
            mDisplayBounds = new SDL_Rect[TOTAL_DISPLAYS];
            for(int i = 0; i < TOTAL_DISPLAYS; i++){
                SDL_GetDisplayBounds(i, &mDisplayBounds[i]);
            }
            //setup renderer blending for alpha blending
            SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        }
        
    }
    return successFlag;
}

//need to hande window events
void lWindow::handleEvent(SDL_Event& e){
    //need to check if the event is window event
    if(e.type == SDL_WINDOWEVENT && e.window.windowID == getWindowID()){
        //check the type of window event
        switch (e.window.event) {
            //reset the display index if the window moved
            case SDL_WINDOWEVENT_MOVED:
                mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
                break;
            case SDL_WINDOWEVENT_SHOWN:
                mShown = true;
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                mShown = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                //the window has been resized
                //change the width and height
                mWidth=e.window.data1;
                mHeight=e.window.data2;
                //rerender the window
                SDL_RenderPresent(mRenderer);
                break;
            
            case SDL_WINDOWEVENT_EXPOSED:
                //the window has been exposed
                SDL_RenderPresent(mRenderer);
                break;
                
            //now need to check all the conditionals
            case SDL_WINDOWEVENT_ENTER:
                mMouseFocus = true;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                mMouseFocus = false;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                mKeyboardFocus = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                mKeyboardFocus = false;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                mMinimized = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                mMinimized = false;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                mMinimized = false;
                break;
            //need to hide window if the close event is shown
            case SDL_WINDOWEVENT_CLOSE:
                mShown = false;
                SDL_HideWindow(mWindow);
                break;
            default:
                break;
        }
    }
}

void lWindow::render(){
    //only render if shown
    if(!isMinimized()){
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);
        //should change this so that the render only setsup the window
        //SDL_RenderPresent(mRenderer);
    }
}

void lWindow::focus(){
    //lets check to make sure they arent minimized
    if(!isMinimized()){
        //first show window if not shown
        if(!isShown()){
            SDL_ShowWindow(mWindow);
        }
        //move window forward
        SDL_RaiseWindow(mWindow);
    }
}
#endif /* lWindow_h */
