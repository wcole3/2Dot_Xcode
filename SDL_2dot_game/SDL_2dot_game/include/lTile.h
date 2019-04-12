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
//level dependent info which changes with the map
extern int TOTAL_TILES;
extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;
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
extern const string levelMapFile;

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

//tile specific methods
extern lTile** gTiles;
//method to setup the tiles subject to the map we will read in
/**
    Reads the map file and sets the associated tile map
 
    @param tiles the to be set from the map file
 
    @return true if the tiles are set successfully
 */
bool setTiles(lTile** tiles){
    bool tilesSet = true;
    //also set x and y offsets
    int x = 0;
    int y = 0;
    //first need to read in the map file
    ifstream map(levelMapFile);
    //first check if the file was openned
    if(!map.is_open()){
        printf("Could not open map file!\n");
        tilesSet = false;
        
    }else{
        //if the map is openned then we can begin looping to set the values
        int width, height;
        map >> width;
        map >> height;
        TOTAL_TILES = width * height;
        //now that total tiles is know allocate gTiles
        gTiles = new lTile*[TOTAL_TILES];
        LEVEL_WIDTH = width * TILE_WIDTH;
        LEVEL_HEIGHT = height * TILE_HEIGHT;
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
                    gTiles[i] = new lTile(x, y, tileType);
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
        
        gTileSprite[BLUE_TILE].x = 80;
        gTileSprite[BLUE_TILE].y = 0;
        
        gTileSprite[TOP_LEFT].x = 160;
        gTileSprite[TOP_LEFT].y = 0;
        
        gTileSprite[TOP].x = 240;
        gTileSprite[TOP].y = 0;
        
        gTileSprite[TOP_RIGHT].x = 320;
        gTileSprite[TOP_RIGHT].y = 0;
        
        gTileSprite[ALL_BORDER].x = 400;
        gTileSprite[ALL_BORDER].y = 0;
        
        gTileSprite[RIGHT_CAP].x = 480;
        gTileSprite[RIGHT_CAP].y = 0;
        
        gTileSprite[RED_CHECK].x = 0;
        gTileSprite[RED_CHECK].y = 80;
        
        gTileSprite[GREEN_TILE].x = 80;
        gTileSprite[GREEN_TILE].y = 80;
        
        gTileSprite[LEFT].x = 160;
        gTileSprite[LEFT].y = 80;
        
        gTileSprite[CENTER].x = 240;
        gTileSprite[CENTER].y = 80;
        
        gTileSprite[RIGHT].x = 320;
        gTileSprite[RIGHT].y = 80;
        
        gTileSprite[VERT_BORDER].x = 400;
        gTileSprite[VERT_BORDER].y = 80;
        
        gTileSprite[LEFT_CAP].x = 480;
        gTileSprite[LEFT_CAP].y = 80;
        
        gTileSprite[ENDZONE].x = 0;
        gTileSprite[ENDZONE].y = 160;
        
        gTileSprite[TOP_CAP].x = 80;
        gTileSprite[TOP_CAP].y = 160;
        
        gTileSprite[BOTTOM_LEFT].x = 160;
        gTileSprite[BOTTOM_LEFT].y = 160;
        
        gTileSprite[BOTTOM].x = 240;
        gTileSprite[BOTTOM].y = 160;
        
        gTileSprite[BOTTOM_RIGHT].x = 320;
        gTileSprite[BOTTOM_RIGHT].y = 160;
        
        gTileSprite[HORZ_BORDER].x = 400;
        gTileSprite[HORZ_BORDER].y = 160;
        
        gTileSprite[BOT_CAP].x = 480;
        gTileSprite[BOT_CAP].y = 160;
        
    }
    
    map.close();
    return tilesSet;
}



#endif /* lTile_h */
