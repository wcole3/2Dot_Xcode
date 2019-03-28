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
double distanceSquared(float x1, float x2, float y1, float y2){
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
