//
//  lRigidDot.h
//
//  Classes that inherits from lTexture and adds rigid body behavior starting with
//  collision detection
//
//  Created by William Cole on 12/6/18.
//  Copyright © 2018 William Cole. All rights reserved.
//

#ifndef lRigidDot_h
#define lRigidDot_h
#include "lTexture.h"
#include "lTile.h"
using namespace std;

//wrapper class for a movable dot with circular collision 'boxes'
class lRigidDot: public lTexture{
public:
    //total number of particles per dot
    static const int TOTAL_PARTICLES = 20;
    //constructor to setup screen size and renderer to use
    /**
        Initialize a lRigidDot object
     
        @param renderer the SDL_Renderer for the object
        @param startX the starting x coor on the screen
        @param startY the starting y coor on the screen
     
     */
    lRigidDot(SDL_Renderer* renderer = NULL, int startX = 0, int startY = 0);
    //need a deconstructor to destroy particles
    ~lRigidDot();
    /**
         Set the starting position of the dot
         
         @param x the starting x coor. of the dot
         @param y the starting y coor. of the dot
     */
    void setStartingPos(int x, int y);
    //method to load the control scheme for the dot
    /**
        Loads the control scheme for the dot
     
        @param control the controls to use for the controls
     */
    void loadControls(const char* control[]);
    //handle events to set the velocity through key presses
    /**
        Handle SDL_Events for the dot
     
        @param e the event to check
     */
    void handleEvent(SDL_Event& e);
    //method to update the speed of the dot
    /**
        Update the velocity of the dot subject to the user inputs and its location
     
        @param time the time passed since last call to updateVelocity
        @param tiles the tiles defining the map
     */
    void updateVelocity(float time, lTile* tiles[]);
    //the first move method will just move the dot subject to the screen boundaries
    /**
        Move the dot subject to the time since last move
     
        @param timeStep the time since last move call
     */
    void move(float timeStep);
    //move the dot's posiiton synced to time subject to and one circle
    /**
        Move the dot, check for collision with another dot object on map
     
        @param time the time since last call to move
        @param circle the collidable dot on the map
     */
    void move(float time, circle& circle);
    //method to move the dot relative to a bunch of tiles
    /**
        Move the dot, check for collisions with another dot and the map
     
        @param time the time since the last call to move
        @param circle the other collidable dot on the map
        @param tiles the tiles defining the map
     */
    void move(float time,circle& circle, lTile* tiles[]);
    //method to detect if the dot is touching walls defined by tiles
    /**
        Check for collision between a circle and the map
     
        @param circle the dot object
        @param tiles the tiles defining the map
     
        @return true if any collision is occuring
    */
    bool touchingTileWall(circle& circle, lTile* tiles[]);
    //detect if two circles are touching
    /**
        Check collision between two circles
     
        @param circleA the first circle
        @param circleB the second circle
     
        @return true if the circles are touching
     */
    bool detectCollision(circle& circleA, circle& circleB);
    //method to see if a circle and a box are touching
    /**
        Check for collision between a circle and an SDL_Rect
     
        @param circle the circle
        @param box the SDL_Rect
     
        @return true if the objects are touching
     */
    bool detectCollision(circle& circle, SDL_Rect& box);
    //going to overload the loadFromFile method of lTexture to setup collision box here
    /**
        Get the dot texture from file
     
        @param path the file path for the image
        @param colorKey true if color keying is needed
        @param keyColor the rgb color to use in color keying
     
        @return true if image was converted to texture successfully
     */
    bool loadFromFile(string path, SDL_bool colorKey, SDL_Color keyColor = {0,0,0});
    //need a function to render the circle taking into account the offset and the camera
    /**
        Render to screen defined by the camera
     
        @param camera the SDL_Rect representing the camera for the dot
     */
    void render(SDL_Rect& camera);
    //need a method to set the camera relative to dot's position
    /**
        Set the camera location relative to the dot location
     
        @param camera the camera object
     */
    void setCamera(SDL_Rect& camera);
    /**
        Set the level size of the dot
     
        @param width the width of the level
        @param height the height of the level
     */
    void setLevelSize(int width, int height);
    //functions to return the dot's position
    /**
        @return the x position of the dot
     */
    float getXPos(){return xCenterPos;};
    /**
        @return the y position of the dot
     */
    float getYPos(){return yCenterPos;};
    /**
        @return the circle defining the boundary of the dot
     */
    circle& getCollider(){return mCollisionCircle;};
    //constant static for the maximum velocity
    static const int DOT_MAX_VEL = 300; //PIXELS PER SECOND
    //constant for dot acceleration in pixels/s*s
    static const int DOT_ACCEL = 600;
    //method to return true if the dot has moved from its starting location
    /**
        @return true if the dot has moved out of the starting area
     */
    bool hasMoved();
    //method to return true is the dot has reached the end point (which is the middle of the screen)
    /**
        @return true if the dot is in the endzone
     */
    bool isFinished();
    //method to reset the player
    /**
        Resets the dot's position and velocity and acceleration
     */
    void reset();
    //method to get the control scheme for given player
    /**
        Get the control for motion i = { 0 -> UP, 1 -> DOWN, 2-> LEFT, 3 -> RIGHT}
     
        @param i the control to return
     
        @return the SDL_Scancode for the desired control
     */
    SDL_Scancode getControlButton(int i);
private:
    //screen positions of the CENTER of the circle
    float xCenterPos, yCenterPos;
    //floats of the velocity
    float xVelocity, yVelocity;
    //Acceleration components
    float xAccel, yAccel;
    //two ints specify the startting screen position
    int xStart, yStart;
    //SDL_rect which defines the collision zone for the texture
    circle mCollisionCircle;
    //screen/level size
    int screenW, screenH;
    //method to shift the collision circle with dot movement
    void shiftCollider();
    
    //bool to tell is the dot should be deccerating to stop
    bool xdecel, ydecel;
    //a float defining the friction of the surface the dot is currently on
    float surfaceFriction = 200;//TEMP, later we want to define this depending on the surface
    //the following is the control scheme and the prompts to render to screen at the start
    SDL_Scancode upButton;
    SDL_Scancode downButton;
    SDL_Scancode leftButton;
    SDL_Scancode rightButton;
    
#ifdef lParticle_h
    //list of particles for dot
    lParticle* mParticles[TOTAL_PARTICLES];
    //method to spawn particle effects
    void renderParticles();
#endif
};
//constructor to set screen and renderer
lRigidDot::lRigidDot(SDL_Renderer* renderer, int startX, int startY){
    //initialize all variables
    xCenterPos = 0;
    yCenterPos = 0;
    xAccel = 0;
    yAccel = 0;
    xVelocity = 0;
    yVelocity = 0;
    //set screen size
    screenW = 0;
    screenH = 0;
    //set starting position
    setStartingPos(startX, startY);
    //setup rest
    lTexture();
    //set renderer
    this->setRenderer(renderer);
#ifdef lParticle_h
    //also need to create then particles for the dot
    for( int i = 0; i < TOTAL_PARTICLES; ++i){
        mParticles[i] = new lParticle(startX, startY, xVelocity, yVelocity);
        //set starting point as dot starting point since center pos isnt set until a call to loadFromFile
    }
#endif
}
//deconstrcutor to destroy particles
lRigidDot::~lRigidDot(){
#ifdef lParticle_h
    //delete each particle on close
    for(int i = 0; i < TOTAL_PARTICLES; ++i){
        mParticles[i]->free();
    }
#endif
}

//method to set the starting position and make sure it is on screen
void lRigidDot::setStartingPos(int x, int y){
    //check to make sure the desired started position is on screen
    if(x < 0 || x > screenW){
        //invalid set at x=0
        xStart=0;
    }else{
        xStart=x;
    }
    if(y < 0 || y > screenH){
        yStart=0;
    }else{
        yStart=y;
    }
}//the only real reason to do it this way instead of directly setting the xCenterPos and yCenterPos is we can check if the coordinates are on screen

bool lRigidDot::loadFromFile(string path, SDL_bool colorKey, SDL_Color keyColor){
    bool successFlag=true;
    //load the file as lTexture would normally
    successFlag=lTexture::loadFromFile(path, colorKey, keyColor);
    //then setup the extras, I'm doing this since the texture's width and height are not set before this call
    //we can then setup the circle
    mCollisionCircle.r=getWidth()/2;
    xCenterPos= xStart + mCollisionCircle.r;
    yCenterPos= yStart + mCollisionCircle.r;
    //move the collision circle's x and y positions to the xCenterPos and yCenterPos taking into account offsets
    shiftCollider();
    return successFlag;
}
//method to load control scheme for the dot
void lRigidDot::loadControls(const char* control[]){
    upButton = SDL_GetScancodeFromName(control[0]);
    downButton = SDL_GetScancodeFromName(control[1]);
    leftButton = SDL_GetScancodeFromName(control[2]);
    rightButton = SDL_GetScancodeFromName(control[3]);
}
//need to handle keypresses and set the velocity appropriately
void lRigidDot::handleEvent(SDL_Event& e){
    //make sure the key is down and we discard repeats
    const Uint8* keyStates = NULL;
    keyStates = SDL_GetKeyboardState(NULL);
    //set motion depending on the key presses
    if(keyStates[upButton]){
        if(keyStates[downButton]){
            ydecel = true;
        }else{
            yAccel = (-1*DOT_ACCEL);
            ydecel = false;
        }
    }
    if(keyStates[downButton]){
        if(keyStates[upButton]){
            ydecel = true;
        }else{
            yAccel = DOT_ACCEL;
            ydecel = false;
        }
    }
    if(!keyStates[upButton] && !keyStates[downButton]){
        ydecel = true;
    }
    if(keyStates[leftButton]){
        if(keyStates[rightButton]){
            xdecel = true;
        }else{
            xAccel = (-1*DOT_ACCEL);
            xdecel = false;
        }
    }
    if(keyStates[rightButton]){
        if(keyStates[leftButton]){
            xdecel = true;
        }else{
            xAccel = DOT_ACCEL;
            xdecel = false;
        }
    }
    if(!keyStates[leftButton] && !keyStates[rightButton]){
        xdecel = true;
    }
}
//need to be able to detect collision between two circles
bool lRigidDot::detectCollision(circle& circleA, circle& circleB){
    //need to test if the distance squared between the two circles is less than the total radius squared if so then the objects are colliding
    //circle A parameter
    float xA=circleA.x;
    float yA=circleA.y;
    int rA=circleA.r;
    //circle B parameter
    float xB=circleB.x;
    float yB=circleB.y;
    int rB=circleB.r;
    int totalRadiusSQ= (rA+rB)*(rA+rB);
    if(distanceSquared(xA, xB, yA, yB) < totalRadiusSQ){
        //if the distance sq is smaller than the radius squared they are touching
        return true;
    }
    //if test gets here there is not collision
    return false;
}

//method to check collision between circle and rect
bool lRigidDot::detectCollision(circle& circle, SDL_Rect& box){
    //here we test the distance sq between the circle center and the closest point on the box agaisnt the circle's radius sq
    //we first need to find the closest point on the box (remember the coordinates are defined at the top left
    //test x and y seperately, x first
    float closestX, closestY;
    //if the circle center is to the left of the box the left most coordinate is box.x
    if((circle.x) < box.x){
        closestX=box.x;
    }
    //if the circle center is to right then right side is closest
    else if((circle.x) > (box.x + box.w)){
        closestX = box.x + box.w;
    }
    //the only other option is the x coordinate of the circle is somewhere between box.x and box.x+box.w
    //in that case the x coordinate of the box that is closest is the x coor of the circle
    else{
        closestX=circle.x;
    }
    //now do y axis
    //if circle center is above the top of the box is closest
    if(circle.y < box.y){
        closestY=box.y;
    }
    //if circle center is below then the bottom is closest
    else if( circle.y > (box.y + box.h)){
        closestY=box.y + box.h;
    }
    //otherwise the y coordinate of the circle is closest
    else{
        closestY=circle.y;
    }
    //now we know the closest point on the box we can check collision against radius sq
    if(distanceSquared(circle.x, closestX, circle.y, closestY) < (circle.r*circle.r)){
        //if the distance is smaller then there is collision
        return true;
    }
    //otherwise there is no collison
    return false;
}
//method to check collision between circle and tile object
bool lRigidDot::touchingTileWall(circle& circleCollider, lTile* tiles[]){
    //need to loop through each tile
    for(int i = 0; i < TOTAL_TILES; i++){
        //need to check what type of tile an entry is, we only care about the tiles with walls
        if(tiles[i]->getType() >= CENTER && tiles[i]->getType() < TOTAL_TILES_TYPES){
            //so if we know the tile is the correct type we want to check the collision
            if(detectCollision(getCollider(), tiles[i]->getBox())){
                return true;//returns true if touching wall
            }
        }//could have an else statment here to change movement of the dot depending on the tile type
    }
    return false;
}

//method to move the collision circle in sync with the xCenterPos and yCenterPos variables
void lRigidDot::shiftCollider(){
    mCollisionCircle.x=xCenterPos;
    mCollisionCircle.y=yCenterPos;
}

//here we want to update the velocity of the dot making sure that the velocity doesnt exceed the maximum
void lRigidDot::updateVelocity(float timeStep, lTile* tiles[]){
    //figure out what type of tile we are on and modify velocity accordingly
    float xVeloMod = 1;
    float yVeloMod = 1;
    for(int i = 0; i < TOTAL_TILES; ++i){
        if(tiles[i]->getType() < CENTER){
            if(detectCollision(getCollider(), tiles[i]->getBox())){
            //if the tiles is colored
                switch (tiles[i]->getType()) {
                    case 0://red tile, want to boost at the start
                        if(xdecel){xVeloMod = 0.75;}
                        else{xVeloMod = 2;}
                        if(ydecel){yVeloMod = 0.75;}
                        else{yVeloMod = 2;}
                        break;
                    case 1://green tile, normal movement
                        xVeloMod = 1;
                        yVeloMod = 1;
                        break;
                    case 2://blue tile, cut speed
                        if(xdecel){xVeloMod = 2;}
                        else{xVeloMod = 0.5;}
                        if(ydecel){yVeloMod = 2;}
                        else{yVeloMod = 0.5;}
                        break;
                    default:
                        break;
                }
            }
        }
    }
    //start with x component
    float oldXVelocity = xVelocity;
    xVelocity += ((xAccel)*timeStep)*xVeloMod;
    if(xdecel){
        //set the acceleration to the appropriate direction; which is the opposite of the current direction
        xAccel = ((-1)*getSign(oldXVelocity)*surfaceFriction);//the rate of deceleration is controled by the surface the dot is on
        //dot is deccelerating and we need to stop it at 0
        //we want the dot to stop moving once it changes direction
        if(getSign(xVelocity) == getSign(xAccel)){
            xVelocity = 0;
            xAccel = 0;
        }
    }
    //same thing for y velocity
    float oldYVelocity = yVelocity;
    yVelocity += ((yAccel)*timeStep)*yVeloMod;
    if(ydecel){
        yAccel = ((-1)*getSign(oldYVelocity)*surfaceFriction);
        if(getSign(yVelocity) == getSign(yAccel)){
            yVelocity = 0;
            yAccel = 0;
        }
    }
    //make sure the particle isn't speeding
    if(sqrtf((xVelocity*xVelocity)+(yVelocity*yVelocity)) >= DOT_MAX_VEL){
        xVelocity = (xVelocity/sqrtf((xVelocity*xVelocity)+(yVelocity*yVelocity))) * DOT_MAX_VEL;//normalize the components
        yVelocity = (yVelocity/sqrtf((xVelocity*xVelocity)+(yVelocity*yVelocity))) * DOT_MAX_VEL;
    }
}
//move function subject to screen size
void lRigidDot::move(float timeStep){
    //move the dot then check boundaries
    //you can cut out either direction to keep dot rooted in that direction
    //updateVelocity(timeStep);
    xCenterPos+=xVelocity*timeStep;
    shiftCollider();
    //check x
    if(xCenterPos-mCollisionCircle.r < 0){
        xCenterPos=mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    if(xCenterPos + mCollisionCircle.r > screenW){
        xCenterPos=screenW-mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    
    //check y
    yCenterPos+=yVelocity*timeStep;
    shiftCollider();
    if(yCenterPos - mCollisionCircle.r < 0){
        yCenterPos=mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
    if(yCenterPos + mCollisionCircle.r > screenH){
        yCenterPos=screenH-mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
    
}

//now need to move the dot wrt time passed and make sure it hasn't collided with anything
//right now the collision behaves like the box has inifite mass
void lRigidDot::move(float time, circle& circle){
    //here box is some external structure the dot could collide with
    //first update the x position
    float oldxPos=xCenterPos;
    //updateVelocity(time);
    
    xCenterPos+=xVelocity*time;
    //also need to update the collision box
    shiftCollider();
    //check if the dot is off screen
    if(xCenterPos - mCollisionCircle.r < 0){
        //too far left
        xCenterPos = 0 + mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }else if(xCenterPos + mCollisionCircle.r > screenW){
        //too far right
        xCenterPos = screenW - mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }else if(detectCollision(getCollider(), circle)){
        //the dot has collided with the box along the x axis
        xCenterPos=oldxPos;//not sure if there is a cleverer way to do this
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    //now do the same for the y direction
    float oldyPos=yCenterPos;
    yCenterPos+=yVelocity*time;
    shiftCollider();
    
    if(yCenterPos - mCollisionCircle.r < 0){
        //too far up
        yCenterPos = 0 + mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }else if(yCenterPos + mCollisionCircle.r > screenH){
        //too far down
        yCenterPos=screenH - mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }else if(detectCollision(getCollider(), circle)){
        //the dot has collided with the box along the y axis
        yCenterPos=oldyPos;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
}

void lRigidDot::move(float timeStep,circle& circle, lTile* tiles[]){
    //start by getting the current position
    float oldXPos = getXPos();
    float oldYPos = getYPos();
    //start with x direct; move the dot and shift colliders
    updateVelocity(timeStep, tiles);
    xCenterPos += timeStep*xVelocity;
    shiftCollider();
    //now check for level collision and wall collision
    if(xCenterPos - mCollisionCircle.r < 0){
        xCenterPos = mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    else if(xCenterPos + mCollisionCircle.r > screenW){
        xCenterPos = screenW - mCollisionCircle.r;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    //test wall and circle collision
    else if(touchingTileWall(getCollider(), tiles) || detectCollision(getCollider(), circle)){
        xCenterPos = oldXPos;
        shiftCollider();
        xVelocity=(-1)*xVelocity;
    }
    //do the same for y direction
    yCenterPos += timeStep*yVelocity;
    shiftCollider();
    //check collisions
    if(yCenterPos - mCollisionCircle.r < 0){
        yCenterPos = mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
    else if(yCenterPos + mCollisionCircle.r > screenH){
        yCenterPos = screenH - mCollisionCircle.r;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
    else if(touchingTileWall(getCollider(), tiles) || detectCollision(getCollider(), circle)){
        yCenterPos = oldYPos;
        shiftCollider();
        yVelocity = (-1)*yVelocity;
    }
    
}



//need to overload the render function to take into account that we render at top left instead of center
void lRigidDot::render(SDL_Rect& camera){
    //apply the offset
    int topLeftX = (int)(xCenterPos-getCollider().r);
    int topLeftY = (int)(yCenterPos-getCollider().r);
    //render relative to the offsets
    lTexture::render((topLeftX - camera.x), (topLeftY - camera.y));
#ifdef lParticle_h
    //then we need to render the particles
    renderParticles();
#endif
}
#ifdef lParticle_h
//method to render the particle and create them
void lRigidDot::renderParticles(){
    //first we need to check if the particles are dead
    for(int i = 0; i < TOTAL_PARTICLES; ++i){
        if(mParticles[i]->isDead()){
            //if dead delete and make a new one
            delete mParticles[i];
            mParticles[i] = new lParticle((int)xCenterPos, (int)yCenterPos, xVelocity, yVelocity);
        }
    }
    //after that render each particle
    for(int i = 0; i < TOTAL_PARTICLES; ++i){
        mParticles[i]->render();
    }
}
#endif

//method to set the camera's position
void lRigidDot::setCamera(SDL_Rect& camera){
    //center the camera over the dot
    camera.x = xCenterPos - (camera.w/2);
    camera.y = yCenterPos - (camera.h/2);
    //check to make sure the camera doesnt go out of bounds
    if(camera.x < 0){
        camera.x = 0;
    }
    else if(camera.x + camera.w > screenW){
        camera.x = screenW - camera.w;
    }
    if(camera.y < 0){
        camera.y = 0;
    }
    else if(camera.y + camera.h > screenH){
        camera.y = screenH - camera.h;
    }
}

//set the level size, must be called after tiles are set
void lRigidDot::setLevelSize(int width, int height){
    screenW = width;
    screenH = height;
}

//method to check if the dot has moved from start
bool lRigidDot::hasMoved(){
    //we could change this so that there is a small buffer zone where the controls return
    if(distanceSquared(xStart, xCenterPos, yStart, yCenterPos) >= 25000){
        return true;//dot has moved
    }else{
        return false;
    }
}
//check if the dot is within the 2 x 2 tile finishing zone in the middle of the map
bool lRigidDot::isFinished(){
    SDL_Rect endZone = {(screenW/2) - TILE_WIDTH, (screenH/2) - TILE_HEIGHT, 2*TILE_WIDTH, 2* TILE_HEIGHT};
    if(detectCollision(mCollisionCircle, endZone)){
        //we have reached the endzone we need to slow down
        xdecel = true;
        ydecel = true;
        return true;
    }
    //default case is false
    return false;
}
//method to reset the player to starting positions
void lRigidDot::reset(){
    xVelocity = 0;
    xAccel = 0;
    yVelocity = 0;
    yAccel = 0;
    xCenterPos = xStart + mCollisionCircle.r;
    yCenterPos = yStart + mCollisionCircle.r;
    shiftCollider();
}

//method to return the sdl scancode of a particular control
SDL_Scancode lRigidDot::getControlButton(int i){
    //the key is 0 = UP, 1 = DOWN, 2 = LEFT, and 3 = RIGHT
    switch (i) {
        case 0: return upButton; break;
        case 1: return downButton; break;
        case 2: return leftButton; break;
        case 3: return rightButton; break;
        default:
            printf("Error: player control scheme not set!\n");
            return SDL_SCANCODE_0;
            break;//if control scheme is not set
    }
}
#endif /* lRigidDot_h */
