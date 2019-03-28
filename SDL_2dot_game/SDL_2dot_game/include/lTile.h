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
const int TOTAL_TILES = 192;
extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;
//Need a ke for the tile types, order is set by the map file we got from lazy foo

const int RED_TILE = 0;
const int GREEN_TILE = 1;
const int BLUE_TILE = 2;
const int CENTER = 3;
const int TOP = 4;
const int TOP_RIGHT = 5;
const int RIGHT = 6;
const int BOTTOM_RIGHT = 7;
const int BOTTOM = 8;
const int BOTTOM_LEFT = 9;
const int LEFT = 10;
const int TOP_LEFT = 11;
const int TOTAL_TILES_TYPES =12;


//need a sprite sheet
//extern lTexture gTileSpriteTexture;

SDL_Rect gTileSprite[TOTAL_TILES_TYPES];

class lTile{
public:
    //need to construct a tile with a position and type
    lTile(int x, int y, int type);
    //method to render the tile if it is in the camera view
    void render(lTexture* tileSpriteSheet, SDL_Rect camera);
    //method to get the collion box of the tile
    SDL_Rect& getBox(){return mBox;};
    //method to get the type of tile
    int getType(){return tileType;};
    
private:
    int xPos, yPos;
    //collison box
    SDL_Rect mBox;
    //type of tile
    int tileType;
};

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

//tile specific methods

//method to setup the tiles subject to the map we will read in
bool setTiles(lTile* tiles[]){
    bool tilesSet = true;
    //also set x and y offsets
    int x = 0;
    int y = 0;
    //first need to read in the map file
    ifstream map("assets/lazy_custom.map");
    //first check if the file was openned
    if(!map.is_open()){
        printf("Could not open map file!\n");
        tilesSet = false;
        
    }else{
        //if the map is openned then we can begin looping to set the values
        for(int i = 0; i < TOTAL_TILES; i++){
            // the values from the file give the type of the tile
            int tileType=-1;
            map >> tileType;
            //check if the read in type is valid
            if(map.fail()){
                printf("Could not read type!\n");
                tilesSet = false;
                break;
            }
            else{
                //check if type is valid
                if((tileType >= 0) && (tileType < TOTAL_TILES_TYPES)){
                    //after the type has been set we can create the tile object
                    tiles[i] = new lTile(x, y, tileType);
                }
                else{
                    printf("Tile type not valid for entry: %d\n", i);
                    tilesSet = false;
                    break;
                }
                //after we create the tile we move the offsets; the map file assumes we move from left to right
                //and then down
                x+=TILE_WIDTH;
                if(x >= LEVEL_WIDTH){
                    //if we have moved acroos the level start moving down
                    x = 0;
                    y += TILE_HEIGHT;
                }
            }
        }
    }
    //we might as well setup the sprite sheet here as well
    if(tilesSet){
        //set all of the widths and heights in for loop
        for(int i = 0; i < TOTAL_TILES_TYPES; i++){
            gTileSprite[i].w = TILE_WIDTH;
            gTileSprite[i].h = TILE_HEIGHT;
        }
        //now manually set the clip x and y's for the clips
        gTileSprite[RED_TILE].x = 0;
        gTileSprite[RED_TILE].y = 0;
        
        gTileSprite[GREEN_TILE].x = 0;
        gTileSprite[GREEN_TILE].y = 80;
        
        gTileSprite[BLUE_TILE].x = 0;
        gTileSprite[BLUE_TILE].y = 160;
        
        gTileSprite[TOP_LEFT].x = 80;
        gTileSprite[TOP_LEFT].y = 0;
        
        gTileSprite[TOP].x = 160;
        gTileSprite[TOP].y = 0;
        
        gTileSprite[TOP_RIGHT].x = 240;
        gTileSprite[TOP_RIGHT].y = 0;
        
        gTileSprite[LEFT].x = 80;
        gTileSprite[LEFT].y = 80;
        
        gTileSprite[CENTER].x = 160;
        gTileSprite[CENTER].y =80;
        
        gTileSprite[RIGHT].x = 240;
        gTileSprite[RIGHT].y = 80;
        
        gTileSprite[BOTTOM_LEFT].x = 80;
        gTileSprite[BOTTOM_LEFT].y = 160;
        
        gTileSprite[BOTTOM].x = 160;
        gTileSprite[BOTTOM].y = 160;
        
        gTileSprite[BOTTOM_RIGHT].x = 240;
        gTileSprite[BOTTOM_RIGHT].y = 160;
    }
    
    map.close();
    return tilesSet;
}



#endif /* lTile_h */
