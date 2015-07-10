//
//  Constants.h
//  Tetriminos
//
//  Created by Tingting Gu on 2015/07/01.
//
//

#ifndef Tetriminos_Constants_h
#define Tetriminos_Constants_h

// define the grid's width and height
static const int GRID_WIDTH = 10;
static const int GRID_HEIGHT = 20;

static const float INITIAL_STEP_INTERVAL = 1.0f;
static const float SPEED_MAX = 0.05f;

static const int SCORE_TO_ACCELERATE = 5;
static const float ACCELERATTE_FACTOR = 0.95f;

static const float DROP_VELOCITY = 8000.0f;

static const int TETROMINO_BLOCK_NUMBER = 4;

static const float TIME_PER_GAME = 120.0f;

// enum class or enum struct means almost the same thing
enum class TetrominoType {
    I = 0,
    O = 1,
    T = 2,
    S = 3,
    Z = 4,
    J = 5,
    L = 6
};

#endif
