#ifndef TILESET_4922_INL
#define TILESET_4922_INL

#define TILESET_RES_W 832
#define TILESET_RES_H 373 
#define TILESET_COUNT_X 49
#define TILESET_COUNT_Y 22

#define TILESET_STEP_X (1.0/TILESET_COUNT_X)
#define TILESET_STEP_Y (1.0/TILESET_COUNT_Y)

#define TILESET_SKULL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*0, TILESET_RES_H*TILESET_STEP_Y*7, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_COW_SKULL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*1, TILESET_RES_H*TILESET_STEP_Y*7, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_PLAYER_TILE (v4(TILESET_RES_W*TILESET_STEP_X*25, TILESET_RES_H*TILESET_STEP_Y*0, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_EMPTY_TILE (v4(TILESET_RES_W*TILESET_STEP_X*0, TILESET_RES_H*TILESET_STEP_Y*22, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))
#define TILESET_WALL_TILE (v4(TILESET_RES_W*TILESET_STEP_X*10, TILESET_RES_H*TILESET_STEP_Y*4, TILESET_RES_W*TILESET_STEP_X, -TILESET_RES_H*TILESET_STEP_Y))

#endif