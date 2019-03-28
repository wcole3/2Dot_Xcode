//
//  lTexture.h
//
//  wrapper class for SDL_textures
//
//  Created by William Cole on 11/11/18.
//  Copyright © 2018 William Cole. All rights reserved.
//


#ifndef lTexture_h
#define lTexture_h
using namespace std;

class lTexture{
public:
    lTexture(SDL_Renderer* renderer=NULL);
    ~lTexture();
    bool loadFromFile(string path, SDL_bool colorKey, SDL_Color keyColor = {0,0,0});
    //want a seperate load function to the image but without color keying and manualing setting the pixels
    //TEMP REMOVAL
    //bool loadFromFileWithPixels(string path, bool colorKeying = false, SDL_Color colorKey = {0,0,0});
#ifdef _SDL_TTF_H
    bool loadFromRenderedText(string text, SDL_Color color);
    void setFont(TTF_Font* font);
#endif
    void free();
    //method to create a blank texture with controllable access
    bool createTexture(int width, int height, SDL_TextureAccess access);
    void setRenderer(SDL_Renderer* renderer);
    void setBlend(SDL_BlendMode blend);
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setAlpha(Uint8 alpha);
    void render(int x, int y, SDL_Rect* clip=NULL,SDL_Rect* screen = NULL,double angle=0, SDL_Point* center=NULL,SDL_RendererFlip flip=SDL_FLIP_NONE );
    int getWidth(){return textW;};
    int getHeight(){return textH;};
    //need to be able to lock and unlock the texture
    bool lockTexture();
    bool unlockTexture();
    void* getPixels(){return mPixels;};
    int getPitch(){return mPitch;};
    //need a method to get a specific pixel in the image
    Uint32 getPixel(unsigned int x, unsigned int y);
    //method to copy pixels into a texture
    void copyPixels(void* pixels);
    //method to set the texture as the renderer target
    void setRenderTarget();
private:
    int textW;
    int textH;
    //need a variable defining the ixels of the texture
    void* mPixels;
    //need a varibale that describes the pitch of an image.  The pitch is the width of the texture in memory, each pixel is 4 bits
    int mPitch;
    SDL_Texture* mTexture;
    SDL_Renderer* lRenderer;
#ifdef _SDL_TTF_H
    TTF_Font* lFont;
#endif
};
//top level declaration of method to batch assign fonts
void setFonts(lTexture textTextures[], TTF_Font* font, int sizeOfArray);

//define methods
//constructor
lTexture::lTexture(SDL_Renderer* render){
    textH=0;
    textW=0;
    mTexture=NULL;
    lRenderer=render;
    mPixels = NULL;
    mPitch = 0;
}
//destrcutor
lTexture::~lTexture(){
    free();
}
//deallocation of variables
void lTexture::free(){
    if(mTexture!=NULL){
        SDL_DestroyTexture(mTexture);
        mTexture=NULL;
        textW=0;
        textH=0;
        //Since lRenderer is a passed variable it will be destroyed in main
    }
}
//method to create a blank texture
bool lTexture::createTexture(int width, int height, SDL_TextureAccess access){
    bool successFlag = true;
    mTexture = SDL_CreateTexture(lRenderer, SDL_PIXELFORMAT_RGB888, access, width, height);
    if(mTexture == NULL){
        printf("Could not create blank texture! SDL error: %s\n", SDL_GetError());
        successFlag = false;
    }else{
        //setup the width and height
        textW = width;
        textH = height;
    }
    return successFlag;
}

//method to set renderer to specific reference (used for inheritance classes)
void lTexture::setRenderer(SDL_Renderer* renderer){
    lRenderer=NULL;
    lRenderer=renderer;
}
//method to set the blend mode
void lTexture::setBlend(SDL_BlendMode blend){
    SDL_SetTextureBlendMode(mTexture, blend);
}
//set the color modulation of the texture
void lTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}
//set alpha modulation
void lTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(mTexture, alpha);
}
//method to load image from file path
bool lTexture::loadFromFile(string path, SDL_bool colorKey, SDL_Color keyColor){
    free();
    SDL_Surface* loadedSurface=NULL;
    SDL_Texture* loadedTexture=NULL;
    loadedSurface=IMG_Load(path.c_str());
    if(loadedSurface==NULL){
        printf("Could not load image at path: %s! IMG error: %s\n",path.c_str(),IMG_GetError());
    }else{
        //color key here if need; should change this to make it possible in the call to specify the color (DO THIS!!)
        SDL_SetColorKey(loadedSurface, colorKey, SDL_MapRGB(loadedSurface->format, keyColor.r, keyColor.g, keyColor.b));
        //if the surface is loaded put it in a texture
        loadedTexture=SDL_CreateTextureFromSurface(lRenderer, loadedSurface);
        if(loadedTexture==NULL){
            printf("Could not create Texture from Surface! SDL error: %s\n",SDL_GetError());
        }else{
            textW=loadedSurface->w;
            textH=loadedSurface->h;
        }
    }
    SDL_FreeSurface(loadedSurface);
    mTexture=loadedTexture;
    //return true if mTexture is not NULL
    return mTexture!=NULL;
}
//I'm leaving this here for now as I might use it in later builds
/*
//method to load image and pixels into mPixel and color key if the user wants to
bool lTexture::loadFromFileWithPixels(string path, bool colorKeying, SDL_Color colorKey){
    //important to note that this will need to run we a global window that has already been created
    extern SDL_Window* gWindow;
    if(gWindow == NULL){
        printf("Window has not yet been created!");
        return false;
    }
    //if the window has created we can do things
    free();
    //now load in the image
    SDL_Surface* newSurface = NULL;
    SDL_Texture* newTexture = NULL;
    newSurface = IMG_Load(path.c_str());
    if(newSurface == NULL){
        printf("Could not load image at path: %s! IMG error: %s\n", path.c_str(), IMG_GetError());
    }else{
        //if the surface has been loaded in then we can get the format
        SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(newSurface, SDL_GetWindowPixelFormat(gWindow), NULL);
        if(formattedSurface == NULL){
            printf("Could not created formatted surface! SDL error: %s\n", SDL_GetError());
        }else{
            //once we have the formatted surface we need to get the pixels of the image whcih will start with creating a texture with teh correct formatting
            newTexture = SDL_CreateTexture(lRenderer, SDL_GetWindowPixelFormat(gWindow), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
            if(newTexture == NULL){
                printf("Could not create new texture! SDL error: %s\n", SDL_GetError());
            }else{
                //set texture blending
                SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
                //here we get the pixels, we have to lock the texture first
                SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);
                //now we need to get the pixels from the surface
                //the last argument is the size which is the memory width * the surface height
                memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
                //now that we have the pixel we can put them into the texture by unlocking the texture and clearing the pointer to the pixels
                //get the width and height
                textW = formattedSurface->w;
                textH = formattedSurface->h;
                //if we are doing colorkeying we need to do it here
                if(colorKeying){
                    //get the pitch and pixel count
                    Uint32* pixels = (Uint32*)getPixels();
                    int pixelCount = (getPitch()/4)*getHeight();
                    //the transparent pixel will be the bad color with zero alhpa
                    Uint32 transparent = SDL_MapRGBA(formattedSurface->format, (Uint32)colorKey.r, (Uint32)colorKey.g, (Uint32)colorKey.b, 0);
                    //get the Uint32 of the colorkey
                    Uint32 badColor = SDL_MapRGB(formattedSurface->format, (Uint32)colorKey.r, (Uint32)colorKey.g, (Uint32)colorKey.b);
                    //loop through each pixel
                    for(int i = 0; i < pixelCount; i++){
                        //check if the pixel is the bad color
                        if(pixels[i] == badColor){
                            pixels[i] = transparent;
                        }
                    }
                }
                SDL_UnlockTexture(newTexture);
                mPixels = NULL;
            }
            //now we can release some variables
            SDL_FreeSurface(formattedSurface);
        }
        SDL_FreeSurface(newSurface);
    }
    mTexture = newTexture;
    //return true if the texture is set
    return mTexture != NULL;
}
*/
#ifdef _SDL_TTF_H
//method to load an image text string, this works pretty similar to above just using TTF
bool lTexture::loadFromRenderedText(string text, SDL_Color color){
    //start with freeing things
    free();
    SDL_Surface* loadedSurface=NULL;
    //using TTF and our loaded font, create an image of the text string
    loadedSurface=TTF_RenderText_Solid(lFont, text.c_str(), color);
    if(loadedSurface==NULL){
        printf("Could not create text image! TTF error: %s\n",TTF_GetError());
    }else{
        //if the surface is loaded we need to create a texture
        mTexture=SDL_CreateTextureFromSurface(lRenderer, loadedSurface);
        if(mTexture==NULL){
            printf("Could not create texture from surface! SDL error: %s\n", SDL_GetError());
        }else{
            //set the dimensions
            textH=loadedSurface->h;
            textW=loadedSurface->w;
        }
    }
    //free surface
    SDL_FreeSurface(loadedSurface);
    //return true if mTexture is not NULL
    return mTexture!=NULL;
}

void lTexture::setFont(TTF_Font* font){
    lFont=font;
}
#endif
//finally the rendering method
void lTexture::render(int x, int y, SDL_Rect* clip,SDL_Rect* screen ,double angle, SDL_Point* center, SDL_RendererFlip flip ){
    //first setup screen loaction argument
    SDL_Rect screenLoc;
    screenLoc.x = x;
    screenLoc.y = y;
    if(screen == NULL){
        screenLoc.w = getWidth();
        screenLoc.h = getHeight();
    }else{
        //chang the screen size
        screenLoc.w = screen->w;
        screenLoc.h = screen->h;
    }
    if(clip!=NULL){
        screenLoc.w=clip->w;
        screenLoc.h=clip->h;
    }
    //render with rendercopyex
    SDL_RenderCopyEx(lRenderer, mTexture, clip, &screenLoc, angle, center, flip);
}

//need a method to lock the texture and allow the pixels to be accessed
bool lTexture::lockTexture(){
    bool successFlag = true;
    //need to make sure the texture is not already locked
    if(mPixels != NULL){
        printf("Texture is already locked");
        successFlag = false;
    }
    else{
        if(SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0){
            printf("Could not lock texture! SDL error: %s\n", SDL_GetError());
            successFlag = false;
        }
    }
    return successFlag;
}

//method to unlock the texture and clear the pixel
bool lTexture::unlockTexture(){
    bool successFlag = true;
    //check if the texture is already unlocked
    if(mPixels == NULL){
        printf("Texture is already unlocked!");
        successFlag = false;
    }else{
        //need to unlock the texture and reset the pixle and pitch variables
        SDL_UnlockTexture(mTexture);
        mPitch = 0;
        mPixels =NULL;
        
    }
    return successFlag;
}

Uint32 lTexture::getPixel(unsigned int x, unsigned int y){
    //need to use the pitch and image height to get the pixel at x,y
    //get the pixels
    Uint32* pixels = (Uint32*)getPixels();
    //the pointer saves the pixels as a long 1D array so use pitch and height to index
    //remember that pitch is in bits and each pixel is 4 bits
    return pixels[(y*(getPitch()/4)) + x ];
}
//method to copy to pixels to the texture
void lTexture::copyPixels(void *pixels){
    //first make sure the texture is unlocked
    if(mPixels != NULL){
        //copy the pixels into the texture
        memcpy(getPixels(), pixels, mPitch*getHeight());
    }
}

//method to set the texture as the rendering target
void lTexture::setRenderTarget(){
    if(mTexture != NULL){
        SDL_SetRenderTarget(lRenderer, mTexture);
    }
}

//method to assign an array of lTextures their font
void setFonts(lTexture textTextures[], TTF_Font* font, int sizeOfArray){
    for(int i = 0; i < sizeOfArray; ++i){
        textTextures[i].setFont(font);
    }
}

#endif /* lTexture_h */
