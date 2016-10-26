//============================================================================
// Name        : Ball.h
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class for defining the ball and controling its position
//============================================================================

#ifndef BALL_H_
#define BALL_H_

#include "BaseApplication.h"
#include "BaseApplication.h"

class Ball {
public:

	/**
	 * Constructor for Ball
	 * Parameters:
	 * 		mSceneMgr: Ogre Scene Manager for controlling the scene
	 */
	Ball(Ogre::SceneManager*  & mSceneMgr);

	/**
	 * Destructor for the ball
	 */
	virtual ~Ball(void);

	/**
	 * move: moves the ball based on the given event
	 * Parameters:
	 * 		evt: FrameEvent object (required for getting time since last frame
	 */
	void move(const Ogre::FrameEvent& evt);

	/**
	 * setPosition: sets the position of the ball to the given position
	 * Parameters:
	 * 		pos: the new position of the ball
	 */
	void setPosition(const Ogre::Vector3 pos);

	/**
	 * getPosition: returns the position of ball
	 *
	 * return: the current position of the ball
	 */
	Ogre::Vector3 getPosition(void);

	/**
	 * getDirection: returns the current direction of the ball
	 *
	 * return: the current direction  of the ball
	 */
	Ogre::Vector3 getDirection(void);

	/**
	 * setDirection: sets the direction of the ball to the given direction
	 * Parameters:
	 * 		direction: new direction of the ball
	 */
	void setDirection(const Ogre::Vector3 direction);

	/**
	 * getSceneNode: returns the scenenode associated with the ball
	 *
	 * return: the sceneNode associated with the ball
	 */
	Ogre::SceneNode* getSceneNode(void);

	/**
	 * isMissed: returns whether the ball has been missed or not
	 *
	 * return:
	 * 		true: if the ball has been missed
	 * 		false: if the ball has not been missed
	 */
	bool isMissed(){
		return mMissed;
	}

	/**
	 * setMissed: sets whether the ball  has been missed or not
	 * (used to reuse the same ball after it's been missed)
	 *
	 * Parameters:
	 * 		missed: whether the ball has been missed or not
	 */
	void setMissed(bool missed){
		mMissed = missed;
	}

	/**
	 * setSpeed: sets the speed of the ball
	 * Parameter:
	 * 		speed: new speed of the ball
	 */
	void setSpeed(int speed){
		mSpeed = speed;
	}

	/**
	 * Query flag for Intersectable objects
	 */
	static const Ogre::uint32 INTERSECTABLE;

	/**
	 * Resets the ball to it's initial position and sets the direction to Zero vector
	 */
	void reset(){
		mDirection = Ogre::Vector3::ZERO;
		mPosition = mInitialPosition;
	}

protected:
	/**
	 * Entity associated with the ball
	 */
	Ogre::Entity* mEntity;

	/**
	 * Scene Manager used to control the scene
	 */
	Ogre::SceneManager* mSceneMgr;

	/**
	 * Scene Node associated with the ball
	 */
	Ogre::SceneNode* mSceneNode;

	/***
	 * Current direction of the ball
	 */
	Ogre::Vector3 mDirection;

	/**
	 * Current position of the ball
	 */
	Ogre::Vector3 mPosition;

	/**
	 * Initial position of the ball
	 */
	Ogre::Vector3 mInitialPosition;

	/**
	 * Current speed of the ball
	 */
	int mSpeed;

	/**
	 * Whether the ball has been missed or not.
	 */
	bool mMissed = false;

};




#endif /* BALL_H_ */
