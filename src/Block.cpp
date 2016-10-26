//============================================================================
// Name        : Block.cpp
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class defining the blocks
//============================================================================

#include "Block.h"
#include "BaseApplication.h"
#include <iostream>
#include "Constants.h"

#include <stdlib.h>

using namespace Ogre;



const Ogre::uint32 Block::INTERSECTABLE = 2;
Block::Block(void)
{
	mSceneMgr = 0;
	mEntity =  0;
	mSceneNode = 0;
	mPosition = 0;

}

Block::~Block(void){

}

void Block::init(Ogre::SceneManager*  & sceneMgr, const Ogre::Vector3 pos){
	mSceneMgr = sceneMgr;
	mEntity =  mSceneMgr->createEntity("cube.mesh");
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	int i = Ogre::Math::RangeRandom(0, NUM_MATERIALS);
	mEntity->setMaterialName(MATERIAL_STRINGS[i]);
	mSceneNode->attachObject(mEntity);
	mPosition = pos;
	mSceneNode->setPosition(mPosition);
	mEntity->setQueryFlags(INTERSECTABLE);
}

Vector3 Block::getPosition(){
	return mSceneNode->getPosition();
}

void Block::removeFromScene(){
	mSceneMgr->destroySceneNode(mSceneNode);
}




