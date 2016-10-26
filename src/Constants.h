//============================================================================
// Name        : Constants.h
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class for storing constants being used throughout the game
//============================================================================


#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

//Light Position
const Ogre::Vector3 LIGHT_POSITION = Ogre::Vector3(200.0f, 800.0f, 500.0f);

//Main Camera Position
const Ogre::Vector3 CAMERA_POSITION = Ogre::Vector3(0, 0,570);

//-------------------------------------------------------------------------------------

//Walls Variables

//Floor
const int FLOOR_WIDTH = 300;
const int FLOOR_HEIGHT = 600;
const int FLOOR_NORMAL_DISTANCE = -100;

//Ceiling
const int CEILING_WIDTH = FLOOR_WIDTH;
const int CEILING_HEIGHT = FLOOR_HEIGHT;
const int CEILING_NORMAL_DISTANCE = FLOOR_NORMAL_DISTANCE;


//Left Wall
const int LEFT_WALL_WIDTH = -2*FLOOR_NORMAL_DISTANCE;
const int LEFT_WALL_HEIGHT = FLOOR_HEIGHT;
const int LEFT_WALL_NORMAL_DISTANCE = -(FLOOR_WIDTH/2);

//Right Wall
const int RIGHT_WALL_WIDTH = LEFT_WALL_WIDTH;
const int RIGHT_WALL_HEIGHT = LEFT_WALL_HEIGHT;
const int RIGHT_WALL_NORMAL_DISTANCE = LEFT_WALL_NORMAL_DISTANCE;

//Rear Wall
const int REAR_WALL_WIDTH = -2*FLOOR_NORMAL_DISTANCE;
const int REAR_WALL_HEIGHT = FLOOR_WIDTH;
const int REAR_WALL_NORMAL_DISTANCE = -(LEFT_WALL_HEIGHT/2);

//Front Wall
const int FRONT_WALL_WIDTH = REAR_WALL_WIDTH;
const int FRONT_WALL_HEIGHT = REAR_WALL_HEIGHT;
const int FRONT_WALL_NORMAL_DISTANCE = REAR_WALL_NORMAL_DISTANCE;

//-------------------------------------------------------------------------------------

//Paddle vairables

// Paddle bounds
const int PADDLE_MIN_X = LEFT_WALL_NORMAL_DISTANCE + 20;
const int PADDLE_MAX_X = - PADDLE_MIN_X;
const int PADDLE_MIN_Y = FLOOR_NORMAL_DISTANCE + 20;
const int PADDLE_MAX_Y = - PADDLE_MIN_Y;
const int PADDLE_Z = -FRONT_WALL_NORMAL_DISTANCE -25;
const Ogre::Vector3 PADDLE_INITIAL_POSITION = Ogre::Vector3(0,0,PADDLE_Z);

//Paddle Scale
const Ogre::Vector3 PADDLE_SCALE = Ogre::Vector3(0.35,0.35,0.1);
//Paddle Speed
const int PADDLE_SPEED = 500;


//-------------------------------------------------------------------------------------


// Ball Constants
const Ogre::Vector3 BALL_INITIAL_POSITION = Ogre::Vector3(0,0,PADDLE_Z - 25);
const int INITIAL_NUM_BALLS = 5;
const int BALL_SPEED[] = {0, 200, 220, 240 , 260, 280, 300, 310, 320, 330, 340};
const int INITIAL_BALL_SPEED = BALL_SPEED[1];
const int MISS_PENALTY[] = {0,20,20,21,21,22,22,23,23,24,24};
const Ogre::Vector3 BALL_SCALE = Ogre::Vector3(0.05,0.05,0.050);

const int BALL_MIN_X = LEFT_WALL_NORMAL_DISTANCE + 6;
const int BALL_MAX_X = -BALL_MIN_X;
const int BALL_MIN_Y = FLOOR_NORMAL_DISTANCE + 6;
const int BALL_MAX_Y = -BALL_MIN_Y;
const int BALL_MIN_Z = REAR_WALL_NORMAL_DISTANCE + 6;
const int BALL_MAX_Z = -BALL_MIN_Z;

//-------------------------------------------------------------------------------------

//Block Random position bounds
const int BLOCK_POS_MIN_X = LEFT_WALL_NORMAL_DISTANCE + 50;
const int BLOCK_POS_MAX_X = -BLOCK_POS_MIN_X;

const int BLOCK_POS_MIN_Y = FLOOR_NORMAL_DISTANCE + 50;
const int BLOCK_POS_MAX_Y = -BLOCK_POS_MIN_Y;

const int BLOCK_POS_MIN_Z = FRONT_WALL_NORMAL_DISTANCE + 50;
const int BLOCK_POS_MAX_Z = -BLOCK_POS_MIN_Z -  50;

//-------------------------------------------------------------------------------------

// Materials
const Ogre::String MATERIAL_STRINGS[] = {
		"Picture/Block1",
		"Picture/Block2",
		"Picture/Block3",
		"Picture/Block4",
		"Picture/Block5",
		"Picture/Block6",
		"Picture/Block7",
		"Picture/Block8",
		"Picture/Block9",
		"Picture/Block10",
		"Picture/Block11",
		"Picture/Block12",
		"Picture/Block13",
		"Picture/Block14"
};
const int NUM_MATERIALS = 14;
//-------------------------------------------------------------------------------------

//Block variables

const int NUM_BLOCKS[] = {0,25,25,30,30,35,35,40,40,45,45};
const int SCORE_PER_BLOCK[] = {0,20,20,18,18,16,16,15,15,10,10};
const int LEVEL_UP_BLOCKS[] = {0,10,10,8,8,6,6,4,4,2,2};
const int MAX_LEVEL = 10;

const Ogre::Vector3 B1 = Ogre::Vector3(0.25,0.25,0.25);
const Ogre::Vector3 B2 = Ogre::Vector3(0.2,0.2,0.2);
const Ogre::Vector3 B3 = Ogre::Vector3(0.15,0.15,0.15);

const Ogre::Vector3 BLOCK_SCALE[] = {B1, B1, B1,B1, B2, B2,
									 B2, B3, B3,B3, B3};

//-------------------------------------------------------------------------------------

//HighScore
const std::string HIGHSCORE_FILE = ".highScores";

//-------------------------------------------------------------------------------------
#endif
