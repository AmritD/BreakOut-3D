//============================================================================
// Name        : Ball.cpp
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class for defining the ball and controling its position
//============================================================================

#include "Ball.h"
#include "Constants.h"

using namespace Ogre;

const Ogre::uint32 Ball::INTERSECTABLE = 2;

Ball::Ball(Ogre::SceneManager*  & sceneMgr)
{
	mSpeed = INITIAL_BALL_SPEED;
	mDirection = Ogre::Vector3::ZERO;
	mSceneMgr = sceneMgr;
	mEntity =  mSceneMgr->createEntity("Ball", "sphere.mesh");
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode");
	mEntity->setMaterialName("Picture/Ball");
	mSceneNode->attachObject(mEntity);
	mSceneNode->scale(BALL_SCALE);
	mPosition = BALL_INITIAL_POSITION;
	mSceneNode->setPosition(mPosition);
	mEntity->setQueryFlags(INTERSECTABLE);

}

Ball::~Ball(void){

}

void Ball::move(const Ogre::FrameEvent& evt) {
	Vector3 prevPos;

	Ogre::Vector3 nPos = mSceneNode->getPosition() + (mSpeed * mDirection) * evt.timeSinceLastFrame;
	if(nPos.x < BALL_MIN_X || nPos.x > BALL_MAX_X) {
		mDirection.x = - mDirection.x;
		nPos = mSceneNode->getPosition() + (mSpeed * mDirection) * evt.timeSinceLastFrame;
		mPosition = nPos;
		mSceneNode->setPosition(nPos);
		return;
	}
	if(nPos.y < BALL_MIN_Y || nPos.y > BALL_MAX_Y) {
		mDirection.y = - mDirection.y;
		nPos = mSceneNode->getPosition() + (mSpeed * mDirection) * evt.timeSinceLastFrame;
		mPosition = nPos;
		mSceneNode->setPosition(nPos);
		return;
	}

	if(nPos.z < BALL_MIN_Z) {
		mDirection.z= - mDirection.z;
		nPos = mSceneNode->getPosition() + (mSpeed * mDirection) * evt.timeSinceLastFrame;
		mPosition = nPos;
		mSceneNode->setPosition(nPos);
		return;
	}
	if(nPos.z > BALL_MAX_Z){
		mDirection = Vector3::ZERO;
		mPosition = BALL_INITIAL_POSITION;
		mSceneNode->setPosition(mPosition);
		mMissed = true;
		return;
	}
	mPosition = nPos;
	mSceneNode->setPosition(nPos);

}


void Ball::setPosition(const Ogre::Vector3 pos){
		mPosition = pos;
		mSceneNode->setPosition(pos);
}

Vector3 Ball::getPosition(){
	return mSceneNode->getPosition();
}

Ogre::Vector3 Ball::getDirection(void) {
	return mDirection;
}

void Ball::setDirection(const Ogre::Vector3 direction) {
	mDirection = direction;
}

Ogre::SceneNode* Ball::getSceneNode(void){
	return mSceneNode;
}
