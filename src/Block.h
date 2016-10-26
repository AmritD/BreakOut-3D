//============================================================================
// Name        : Block.h
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
// Description : Class defining the blocks
//============================================================================

#ifndef BLOCK_H_
#define BLOCK_H_

#include "BaseApplication.h"

class Block {

public:

	/**
	 * Default constructor for he Block
	 */
	Block(void);

	/**
	 * Destructor for the block
	 */
	virtual ~Block(void);

	/**
	 * init: Initializes the blocks
	 * Parameter:
	 * 		mSceneMgr: Scene Manager controling the scene
	 * 		pos: Position of the block
	 */
	void init(Ogre::SceneManager*  & mSceneMgr, const Ogre::Vector3 pos);

	/**
	 * getPosition: returns the position of the block in the scene
	 *
	 * return: the position of the block
	 */
	Ogre::Vector3 getPosition(void);

	/**
	 * removeFromScene: remove the block from the scene
	 */
	void removeFromScene();

	/**
	 * getSceneNode: returns the scenenode associated with the block
	 *
	 * return: the sceneNode associated with the block
	 */
	Ogre::SceneNode* getSceneNode(void){
		return mSceneNode;
	}

	/**
	 * getEntity: returns the entity associated with the block
	 *
	 * return: the entity associated with the block
	 */
	Ogre::Entity* getEntity(){
		return mEntity;
	}

	/**
	 * scale: Scales the block to the given scale vector
	 *
	 * Parameter:
	 * 		scale: the scale vector to scale the block by
	 */
	void scale(Ogre::Vector3 scale){
		mSceneNode->scale(scale);
	}


	static const Ogre::uint32 INTERSECTABLE;

protected:
	/**
	 * Entity assocaited with the block
	 */
	Ogre::Entity* mEntity;

	/**
	 * Scenemanager managing the sceene
	 */
	Ogre::SceneManager* mSceneMgr;

	/**
	 * SceneNode assocaited with the block
	 */
	Ogre::SceneNode* mSceneNode;

	/**
	 * current Position of the block in the scene.
	 */
	Ogre::Vector3 mPosition;
};



#endif /* BLOCK_H_ */
