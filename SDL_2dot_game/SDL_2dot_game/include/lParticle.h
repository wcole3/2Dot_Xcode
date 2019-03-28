//
//  lParticle.h
//  SDL_particleEffects
//  wrapper class for particle effects
//  Created by William Cole on 1/10/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lParticle_h
#define lParticle_h


//the textures for all the particles and the shimmer particle
lTexture gRedParticle;
lTexture gBlueParticle;
lTexture gGreenParticle;
lTexture gShimmerParticle;



//a seperate initialization method
bool loadParticles(SDL_Renderer* lRenderer);
//and to free the textures on close
void freeParticleTextures();

//wrapper class for the particles
class lParticle{
public:
    //total frames to animate
    static const int TOTAL_FRAMES = 10;
    //create a particle
    lParticle(int x=0, int y=0, float xVel=0, float yVel=0);
    
    //method to free resources
    void free();
    //method to render a particle
    void render();
    //method to check if a particle is dead
    bool isDead();
    
private:
    //the position the particle spawns at
    int xPos, yPos;
    //the current frame of the animation
    int currentFrame;
    //the texture for the particle, dont actuall want a texture object just a pointer to the global texture
    lTexture* mParticleTexture;
};

//first the method to load in all of the particle textures
bool loadParticles(SDL_Renderer* lRenderer){
    bool successFlag = true;
    //first set the renderer of each of the textures
    gRedParticle = lTexture(lRenderer);
    gBlueParticle = lTexture(lRenderer);
    gGreenParticle = lTexture(lRenderer);
    gShimmerParticle = lTexture(lRenderer);
    //color key
    SDL_Color cyanKey = {0,255,255};
    //load each particle with color keying
    if(!gRedParticle.loadFromFile("red.bmp", SDL_TRUE, cyanKey)){
        printf("Could not load red particle texture!");
        successFlag = false;
    }
    if(!gBlueParticle.loadFromFile("blue.bmp", SDL_TRUE, cyanKey)){
        printf("Could not load blue particle texture!");
        successFlag = false;
    }
    if(!gGreenParticle.loadFromFile("green.bmp", SDL_TRUE, cyanKey)){
        printf("Could not load green particle texture!");
        successFlag = false;
    }
    if(!gShimmerParticle.loadFromFile("shimmer.bmp", SDL_TRUE, cyanKey)){
        printf("Could not load red particle texture!");
        successFlag = false;
    }
    //once all particles are loaded then set the alpha to 192 so they are slightly transparent
    gRedParticle.setAlpha(192);
    gBlueParticle.setAlpha(192);
    gGreenParticle.setAlpha(192);
    gShimmerParticle.setAlpha(192);
    return successFlag;
}

//need to free the textures on close
void freeParticleTextures(){
    gRedParticle.free();
    gBlueParticle.free();
    gGreenParticle.free();
    gShimmerParticle.free();
}

//now define methods for the class lParticle
lParticle::lParticle(int x, int y, float xVel, float yVel){
    //initialize the variables, the x and y inputs will be the coordinates of the CENTER of the dot texture
    xPos = x - getSign(xVel)*((rand() % 40)) + ((rand() % 20) - 10);
    yPos = y - getSign(yVel)*((rand() % 40)) + ((rand() % 20) - 10);
    //add some randomness to the starting frame
    currentFrame = rand() % 5;
    //randomly choose which type of particle
    switch (rand() % 3) {
        case 0:
            mParticleTexture = &gRedParticle;
            //printf("red");
            break;
        case 1:
            mParticleTexture = &gGreenParticle;
            //printf("green");
            break;
        case 2:
            mParticleTexture = &gBlueParticle;
            //printf("blue");
            break;
    }
}


void lParticle::free(){
    xPos = 0;
    yPos = 0;
    currentFrame = 0;
    mParticleTexture->free();
}
//method to check if particle is dead
bool lParticle::isDead(){
    return currentFrame > TOTAL_FRAMES;
}

//method to render the particles
void lParticle::render(){
    mParticleTexture->render(xPos, yPos);
    //want to shimmer every other frame
    if(currentFrame % 10 == 0){
        gShimmerParticle.render(xPos, yPos);
    }
    //animate by moving the frame forward
    currentFrame++;
}

#endif /* lParticle_h */
