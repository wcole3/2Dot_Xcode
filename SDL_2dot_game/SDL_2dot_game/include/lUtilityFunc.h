//
//  lUtilityFunc.h
//
//  Some utility functions used in SDL programs
//
//  Created by William Cole on 12/10/18.
//  Copyright © 2018 William Cole. All rights reserved.
//

#ifndef lUtilityFunc_h
#define lUtilityFunc_h
using namespace std;

//a circle structure
struct circle{
    //center coordinates, they are floats for physics based movement
    float x, y;
    //radius
    int r;
};

//method to calculate the distance squared between two points
/**
    Finded the distanced squared between two points (x1, y1) and (x2, y2)
 
    @param x1 the x coor. of the first point
    @param x2 the x coor. of the second point
    @param y1 the y coor. of the first point
    @param y2 the y coor. of the second point
 
    @return the distance sq. betw (x1,y1) and (x2,y2)
 
*/
double distanceSquared(double x1, double x2, double y1, double y2){
    double distanceSq = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
    return distanceSq;
}
//simple method to get sign of a number
/**
    Finds the sign of a given float
 
    @param number The number to be checked.
    @return 1 for positive, -1 for negative, 0 for zero
*/
int getSign(float number){
    if(number < 0){
        return -1;
    }
    if(number > 0){
        return 1;
    }
    else{
        return 0;
    }
}

//method to check collision between two SDL_Rect objects
/**
    Method to check if two SDL_Rect objects are touching
 
    @param boxA The first SDL_Rect object
    @param boxB The second SDL_Rect object
 
    @return true if the boxs are touching
 
*/
bool checkBoxCollison(SDL_Rect boxA, SDL_Rect boxB){
    //start by checking x direction collion
    if(boxA.x + boxA.w < boxB.x || boxA.x > boxB.x + boxB.w){
        return false;
    }
    //check y direction collision
    if(boxA.y + boxA.h < boxB.y || boxA.y > boxB.y + boxB.h){
        return false;
    }
    //default is collision
    return true;
}

//method to check whether the mouse is over a rect
/**
    Checks if the mouse is inside a SDL_Rect border
 
    @param x the mouse x coor.
    @param y the mouse y coor.
    @param box the SDL_Rect to check collision with
 
    @return true if the mouse is inside the SDL_Rect
 
*/
bool checkMouseBoxCollision(int x, int y, SDL_Rect box){
    //check x coor
    if(x < box.x || x > box.x + box.w){
        return false;
    }
    //check y coor
    if(y < box.y || y > box.y + box.h){
        return false;
    }
    return true;
}

#endif /* lUtilityFunc_h */
