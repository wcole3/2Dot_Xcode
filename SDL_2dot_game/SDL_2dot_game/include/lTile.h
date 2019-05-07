//
//  lTile.h
//
//  wrapper class and some utility functions for tiles
//
//  Created by William Cole on 1/12/19.
//  Copyright © 2019 William Cole. All rights reserved.
//

#ifndef lTile_h
#define lTile_h
using namespace std;

//some tile constants which are the same for each tile
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

//Need a key for the tile types, order is set by the map file
const int RED_TILE = 0;
const int BLUE_TILE = 1;
const int RED_CHECK = 2;
const int GREEN_TILE = 3;
const int ENDZONE = 4;
const int TOP_CAP = 5;
const int CENTER = 6;
const int TOP_LEFT = 7;
const int TOP = 8;
const int TOP_RIGHT = 9;
const int LEFT = 10;
const int RIGHT = 11;
const int BOTTOM_LEFT = 12;
const int BOTTOM = 13;
const int BOTTOM_RIGHT = 14;
const int ALL_BORDER = 15;
const int VERT_BORDER = 16;
const int HORZ_BORDER = 17;
const int RIGHT_CAP = 18;
const int LEFT_CAP = 19;
const int BOT_CAP = 20;
const int TOTAL_TILES_TYPES = 21;


SDL_Rect gTileSprite[TOTAL_TILES_TYPES];

class lTile{
public:
    lTile();
    //need to construct a tile with a position and type
    /**
        Initialize the tile object
     
        @param x the x position of the tile
        @param y the y position of the tile
        @param type the type of the tile
     */
    lTile(int x, int y, int type);
    //method to render the tile if it is in the camera view
    /**
        Renders the tile to screen if it is in the view of the camera
     
        @param tileSpriteSheet the sprite sheet for the tiles
        @param camera the camera object
     */
    void render(lTexture* tileSpriteSheet, SDL_Rect camera);
    //method to get the collion box of the tile
    /**
        @return the box defining the tiles position
     */
    SDL_Rect& getBox(){return mBox;};
    //method to get the type of tile
    /**
        @return the type of the tile
     */
    int getType(){return tileType;};
    
private:
    int xPos, yPos;
    //collison box
    SDL_Rect mBox;
    //type of tile
    int tileType;
};
lTile::lTile(){
    xPos = 0;
    yPos = 0;
    mBox = {0,0,TILE_WIDTH,TILE_HEIGHT};
    tileType = -1;
}

//setup tile object
lTile::lTile(int x, int y, int type){
    xPos = x;
    yPos = y;
    //me could keep track of the position with the box but that could be a problem if the collision box changes shape
    mBox = {x,y,TILE_WIDTH,TILE_HEIGHT};
    tileType = type;
}

//method to render tile if it is in the camera view
void lTile::render(lTexture* tileSpriteSheet, SDL_Rect camera){
    //we check if the tile is in the camera view by check the collion boxs, if they collion then we need to render
    if(checkBoxCollison(getBox(), camera)){
        //if there is collision then we need to render it taking into account the camera offset
        tileSpriteSheet->render(getBox().x - camera.x, getBox().y - camera.y, &gTileSprite[getType()]);
    }
}


#endif /* lTile_h */
