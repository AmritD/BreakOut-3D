//============================================================================
// Name        : Paddle.h
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class for defining the paddle and controling its position
//============================================================================

#ifndef PADDLE_H_
#define PADDLE_H_

#include "BaseApplication.h"

class Paddle {
public:
	/**
	 * Costructor for Paddle
	 * Parameter:
	 * 		mSceneMgr: Scene manager for the scene
	 */
	Paddle(Ogre::SceneManager*  & mSceneMgr);

	/**
	 * Destructor for Paddle
	 */
	virtual ~Paddle(void);

	/**
	 * move: Moves the paddle toward the given direction
	 * Parameter:
	 * 		direction: direction towards which the paddle is to be moved
	 * 		evt: FrameEvent  (required to get time since last event)
	 */
	void move(const Ogre::Vector3 direction, const Ogre::FrameEvent& evt);

	/**
	 * setPosition: sets the position of the paddle to the given vector
	 *
	 */
	void setPosition(const Ogre::Vector3 pos);

	/**
	 * getPosition: returns the current position of the paddle in the scene
	 *
	 * return: the current position of the paddle in the scene
	 */
	Ogre::Vector3 getPosition(void);

	/**
	 * getSceneNode: returns the sceneNode associated with the paddle
	 */
	Ogre::SceneNode* getSceneNode(void){
		return mSceneNode;
	}

protected:

	/**
	 * Entity associated with the paddle
	 */
	Ogre::Entity* mEntity;

	/**
	 * Ogre Scenemanager for controling the scene
	 */
	Ogre::SceneManager* mSceneMgr;

	/**
	 * SceneNode associated with the paddle
	 */
	Ogre::SceneNode* mSceneNode;

	/**
	 * Direction of the paddle
	 */
	Ogre::Vector3 mDirection;

	/**
	 * Current position of the paddle
	 */
	Ogre::Vector3 mPosition;

	/**
	 * Speed of the paddle (only applies to the keyboard movement)
	 */
	int mSpeed;
};


#endif /* PADDLE_H_ */
