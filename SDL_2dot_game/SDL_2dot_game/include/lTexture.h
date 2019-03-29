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
    /**
        Loads a image from file and converts it to an SDL_texture
     
        @param path the file path of the image
        @param colorKey true if colorkeying is needed
        @param keyColor the rgb color to set to transparent in keying
     
        @return true if the image was loaded and converted to an SDL_texture
    */
    bool loadFromFile(string path, SDL_bool colorKey, SDL_Color keyColor = {0,0,0});
#ifdef _SDL_TTF_H
    /**
        Loads a text string into a SDL_texture using a TTF_font
     
        @param text the string to convert to a texture
        @param color the rgb color of the text string
     
        @return true if the text was successfully loaded into a texture
    */
    bool loadFromRenderedText(string text, SDL_Color color);
    /**
        Sets the TTF_font to use
     
        @param font the TTF_Font to use
    */
    void setFont(TTF_Font* font);
#endif
    /**
        Free all allocated textures and resets object variables
     */
    void free();
    //method to create a blank texture with controllable access
    /**
        Create a black SDL_texture of defined size with access permissions
     
        @param width the width of the texture
        @param height the height of the texture
        @param access the SDL_TextureAccess mode
     
        @return true if texture was successfully created
     */
    bool createTexture(int width, int height, SDL_TextureAccess access);
    /**
        Sets the SDL_Renderer for the given object
     
        @param renderer the SDL_Renderer object to use for the object
     */
    void setRenderer(SDL_Renderer* renderer);
    /**
        Sets the blend mode of the given object
     
        @param blend the SDL_BlendMode to use
     */
    void setBlend(SDL_BlendMode blend);
    /**
        Set the color modulation of this
     
        @param red the red mod. value
        @param green the green mod. value
        @param blue the blue mod. value
     */
    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    /**
        Set the alpha of this object
     
        @param alpha the value to set the alpha of the texture to
     */
    void setAlpha(Uint8 alpha);
    /**
        Render the texture to the target of the object's SDL_Renderer
     
        @param x the x coor. on screen to render
        @param y the y coor. on screen to render
        @param clip the portion of the texture to render
        @param screen the size of the screen to render to
        @param angle the angle to rotate the texture
        @param center the point about which to rotate
        @param flip reflection operation to apply to the texture
     */
    void render(int x, int y, SDL_Rect* clip=NULL,SDL_Rect* screen = NULL,double angle=0, SDL_Point* center=NULL,SDL_RendererFlip flip=SDL_FLIP_NONE );
    /**
        Get the width of the texture
     
        @return the width of the texture
     */
    int getWidth(){return textW;};
    /**
        Get the height of the texture
     
        @return the height of the texture
     */
    int getHeight(){return textH;};
    //need to be able to lock and unlock the texture
    /**
        Locks the texture to pixel manipulation
     
        @return true if the texture is locked
     */
    bool lockTexture();
    /**
        Unlocks the texture for pixel manipulation
     
        @return true if the texture is unlocked
     */
    bool unlockTexture();
    /**
        Get the pixels associate with the texture
     
        @return a type-less pointer to the pixels
     */
    void* getPixels(){return mPixels;};
    /**
        Get the pitch of a texture
     
        @return the pitch of the texture
     */
    int getPitch(){return mPitch;};
    //need a method to get a specific pixel in the image
    /**
        Get a specific pixel, (x,y), of a texture
     
        @param x the x index of the pixel
        @param y the y index of the pixel
     
        @return the pixel at (x,y)
     */
    Uint32 getPixel(unsigned int x, unsigned int y);
    //method to copy pixels into a texture
    /**
        Copy the pixels of a texture
     
        @param pixels the pixels of the texture to memory
     */
    void copyPixels(void* pixels);
    //method to set the texture as the renderer target
    /**
        Sets the render target of the renderer to the objects texture
     */
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
/**
    Sets the TTF_fonts for a array of lTexture objects
 
    @param textTextures array of the lTexture to have their font set
    @param font the TTF_Font to use
    @param sizeOfArray the size of the textTextures[] array
 */
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
