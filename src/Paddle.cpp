//============================================================================
// Name        : Paddle.cpp
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class for defining the paddle and controling its position
//============================================================================
#include "Paddle.h"
#include "BaseApplication.h"
#include <iostream>
#include "Constants.h"
using namespace Ogre;


Paddle::Paddle(Ogre::SceneManager*  & sceneMgr)
{
	mSpeed = PADDLE_SPEED;
	mDirection = Vector3::ZERO;
	mSceneMgr = sceneMgr;
	mEntity =  mSceneMgr->createEntity("Paddle", "cube.mesh");
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaddleNode");
	mEntity->setMaterialName("Picture/Paddle");
	mSceneNode->attachObject(mEntity);
	mSceneNode->scale(PADDLE_SCALE);
	mPosition = PADDLE_INITIAL_POSITION;
	mSceneNode->setPosition(mPosition);
	mEntity->setQueryFlags(Ogre::int32(2));
	mSceneNode->showBoundingBox(true);
}

Paddle::~Paddle(void){

}

void Paddle::move(const Ogre::Vector3 direction, const Ogre::FrameEvent& evt) {
	// Calculate the new position of the paddle
	Ogre::Vector3 nPos = mSceneNode->getPosition() + (mSpeed * direction) * evt.timeSinceLastFrame;
	//check to see if the new position is within the bounds and move if so.
	if(direction == Ogre::Vector3::UNIT_X || direction == Ogre::Vector3::NEGATIVE_UNIT_X) {
		if(nPos.x > PADDLE_MIN_X && nPos.x < PADDLE_MAX_X) {
			mPosition = mSceneNode->getPosition() + (mSpeed * direction) * evt.timeSinceLastFrame;
			mSceneNode->setPosition(mPosition);
		}
	}
	else if(direction == Ogre::Vector3::UNIT_Y || direction == Ogre::Vector3::NEGATIVE_UNIT_Y){
		if(nPos.y > PADDLE_MIN_Y && nPos.y < PADDLE_MAX_Y) {
			mPosition = mSceneNode->getPosition() + (mSpeed * direction) * evt.timeSinceLastFrame;
			mSceneNode->setPosition(mPosition);
		}
	}
}


void Paddle::setPosition(const Ogre::Vector3 pos){
	mPosition = pos;
	mSceneNode->setPosition(mPosition);
}

Vector3 Paddle::getPosition(){
	return mSceneNode->getPosition();
}
