//============================================================================
// Name        : BreakOut3D.cpp
// Author      : Amrit Dhakal
// Version     : 1.0
// Copyright   : © Amrit Dhakal 2014. All right reserved. :D
//============================================================================

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
 */

#include "BreakOut3D.h"
#include "Paddle.h"
#include "Ball.h"
#include <iostream>
#include "Constants.h"
#include "HighScores.h"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>

using namespace Ogre;

//flag values should be powers of two
const Ogre::uint32 BreakOut3D::INTERSECTABLE = 2;

/**
 * Converts OIS::MouseButtonID to CEGUI::MouseButton
 */
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;

	case OIS::MB_Right:
		return CEGUI::RightButton;

	case OIS::MB_Middle:
		return CEGUI::MiddleButton;

	default:
		return CEGUI::LeftButton;
	}
}


//-------------------------------------------------------------------------------------
//---------------------------------------------------------------------------
BreakOut3D::BreakOut3D(void):
																																																																																																				mBlocks()
{
	mPaddleDirection = Ogre::Vector3::ZERO;
	mPaddle = 0;
	mBall = 0;
	mIntersectionQuery = 0;
	mViewport = 0;
	mGuiRoot = 0;
	mLevel = 1;
	mHighScore = 0;


}
//---------------------------------------------------------------------------
BreakOut3D::~BreakOut3D(void)
{
	if (mPaddle){
		delete mPaddle;
		mPaddle = 0;
	}
	if(mBall){
		delete mBall;
		mBall = 0;
	}
	if(mHighScore){
		delete mHighScore;
		mHighScore = 0;
	}
	if(!mBlocks.empty()){
		for(int i = 0; i < mBlocks.size();i++){
			delete mBlocks[i];
		}
		mBlocks.clear();

	}
	if(mIntersectionQuery){
		mSceneMgr->destroyQuery(mIntersectionQuery);
	}
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	bool result = BaseApplication::frameRenderingQueued(evt);
	if(!mPause && !mStop){
		//Check to see if the ball has been missed and update
		// GUI components and score accordingly
		if(mBall->isMissed()){
			--mNumBalls;
			mStop = true;
			mScore = mScore - MISS_PENALTY[mLevel];
			updateHighScores();
			if(mNumBalls < 0){
				mGuiRoot->getChild("GameOverWindow")->getChild("GameOverPrompt")->setText(
						"Game Over !!! You have missed all of your balls."  );
				mGuiRoot->getChild("MessageLabel")->setText(
						"Game Over !!! You have missed all of your balls.");
				mGuiRoot->getChild("GameOverWindow")->setVisible(true);
			}
			else{
				mGuiRoot->getChild("BallsValue")->setText(std::to_string(mNumBalls));
				mGuiRoot->getChild("ScoreValue")->setText(std::to_string(mScore));
				showButtons(true);
				mGuiRoot->getChild("ResumeButton")->setVisible(false);
				mGuiRoot->getChild("MessageLabel")->setText(
						"You missed the ball. Press Space to continue the game.");
				mBall->setPosition(BALL_INITIAL_POSITION);
				mPaddle->setPosition(PADDLE_INITIAL_POSITION);
			}
			mBall->setMissed(false);
		}
		mPaddle->move(mPaddleDirection, evt);
		mIntersectionQuery->execute(this);
		mOldPosition = mBall->getPosition();
		mBall->move(evt);
	}

	return result;
}

//---------------------------------------------------------------------------
void BreakOut3D::createScene(void)
{
	//Seed random for future use
	srand( (unsigned)time( NULL ) );
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(LIGHT_POSITION);

	//Floor
	Ogre::Plane floor(Ogre::Vector3::UNIT_Y, FLOOR_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("floor",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			floor, FLOOR_WIDTH, FLOOR_HEIGHT, 4, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entfloor = mSceneMgr->createEntity("FloorEntity", "floor");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entfloor);
	entfloor->setCastShadows(false);
	entfloor->setMaterialName("Picture/Floor");

	//Ceiling
	Ogre::Plane ceiling(Ogre::Vector3::NEGATIVE_UNIT_Y, CEILING_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("ceiling",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			ceiling, CEILING_WIDTH, CEILING_HEIGHT, 5, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entceiling = mSceneMgr->createEntity("CeilingEntity", "ceiling");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entceiling);
	entceiling->setMaterialName("Picture/Ceiling");
	entceiling->setCastShadows(false);

	//Left Wall
	Ogre::Plane leftwall(Ogre::Vector3::UNIT_X, LEFT_WALL_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("leftwall",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			leftwall, LEFT_WALL_WIDTH, LEFT_WALL_HEIGHT,5,20, true, 1,4,12, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entlwall = mSceneMgr->createEntity("LeftWallEntity", "leftwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entlwall);
	entlwall->setMaterialName("Picture/PurpleWall");
	entlwall->setCastShadows(false);

	//Right Wall
	Ogre::Plane rightwall(Ogre::Vector3::NEGATIVE_UNIT_X, RIGHT_WALL_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("rightwall",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			rightwall, RIGHT_WALL_WIDTH,RIGHT_WALL_HEIGHT,5,20, true, 1, 4, 12, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* entrwall = mSceneMgr->createEntity("RightWallEntity", "rightwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entrwall);
	entrwall->setMaterialName("Picture/PurpleWall");
	entrwall->setCastShadows(false);


	//Rear Wall
	Ogre::Plane rearwall(Ogre::Vector3::UNIT_Z, REAR_WALL_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("rearwall",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			rearwall, REAR_WALL_WIDTH,REAR_WALL_HEIGHT,5,20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
	Ogre::Entity* entrearwall = mSceneMgr->createEntity("RearWallEntity", "rearwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entrearwall);
	entrearwall->setMaterialName("Picture/RearWall");
	entrearwall->setCastShadows(false);

	//Front Wall
	Ogre::Plane frontwall(Ogre::Vector3::NEGATIVE_UNIT_Z, FRONT_WALL_NORMAL_DISTANCE);
	Ogre::MeshManager::getSingleton().createPlane("frontwall",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			frontwall, FRONT_WALL_WIDTH,FRONT_WALL_HEIGHT,5,20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);

	Ogre::Entity* entfrontwall = mSceneMgr->createEntity("FrontWallEntity", "frontwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entfrontwall);

	entfrontwall->setMaterialName("Picture/RearWall");
	entfrontwall->setCastShadows(false);

	//Initialize paddle, ball and high scores
	mPaddle = new Paddle(mSceneMgr);
	mBall = new Ball(mSceneMgr);
	mHighScore = new HighScores("High Scores", 20, true);

	if(!mHighScore->readFromFile(HIGHSCORE_FILE)){
		mHighScore->writeToFile(HIGHSCORE_FILE);
	}
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	setupGUI();
	initialize(mLevel);
	mGuiRoot->getChild("NameWindow")->getChild("NameBox")->activate();

}
//-------------------------------------------------------------------------------------
void BreakOut3D::showButtons(bool val){

	mGuiRoot->getChild("ResumeButton")->setVisible(val);
	mGuiRoot->getChild("RestartButton")->setVisible(val);
	mGuiRoot->getChild("HighScoresButton")->setVisible(val);

}
//-------------------------------------------------------------------------------------
void BreakOut3D::initialize(int level){
	mBall->setPosition(BALL_INITIAL_POSITION);
	mPaddle->setPosition(PADDLE_INITIAL_POSITION);
	for(int i = 0; i < NUM_BLOCKS[mLevel]; i++){
		Block* b = new Block();
		int x = Ogre::Math::RangeRandom(BLOCK_POS_MIN_X, BLOCK_POS_MAX_X);
		int y = Ogre::Math::RangeRandom(BLOCK_POS_MIN_Y, BLOCK_POS_MAX_Y);
		int z = Ogre::Math::RangeRandom(BLOCK_POS_MIN_Z, BLOCK_POS_MAX_Z);
		b->init(mSceneMgr, Vector3(x,y,z));
		b->scale(BLOCK_SCALE[mLevel]);
		mBlocks.push_back(b);
	}
	mGuiRoot->getChild("BlocksValue")->setText(std::to_string(NUM_BLOCKS[mLevel]-LEVEL_UP_BLOCKS[mLevel]));

}
//-------------------------------------------------------------------------------------
void BreakOut3D::setupGUI(){

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().createFromFile("GlossySerpentFHD.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("GlossySerpentFHDCursors/MouseArrow");
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//load our file
	mGuiRoot = wmgr.loadLayoutFromFile("BreakOut3D.layout");
	//setup main window
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mGuiRoot);

	mGuiRoot->getChild("QuitButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::quit, //method call
					this));	//object to call it on
	mGuiRoot->getChild("ResumeButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::resume, //method call
					this));	//object to call it on

	mGuiRoot->getChild("RestartButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::restart, //method call
					this));	//object to call it on
	mGuiRoot->getChild("BallsValue")->setText(std::to_string(mNumBalls));
	mGuiRoot->getChild("NameWindow")->getChild("NameButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::nameEntered, //method call
					this));	//object to call it on
	mGuiRoot->getChild("HighScoresButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::showHighScores, //method call
					this));	//object to call it on

	//Setup score window
	mScoreBox = static_cast<CEGUI::MultiColumnList*>(mGuiRoot->getChild("ScoreWindow/ScoresList"));
	//mScoreBox->initialiseComponents();
	mScoreBox->setUserSortControlEnabled(false);
	mScoreBox->insertColumn("[font='HighScoreFont-12.font']Rank", 0, CEGUI::UDim(0.15,0), 0);
	mScoreBox->insertColumn("[font='HighScoreFont-12.font']Player Name", 1, CEGUI::UDim(0.485,0), 1);
	mScoreBox->insertColumn("[font='HighScoreFont-12.font']Score", 2, CEGUI::UDim(0.335,0), 2);
	mScoreBox->setUserColumnSizingEnabled(false);
	mGuiRoot->getChild("ScoreWindow")->getChild("ScoreClose")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::hideHighScores, //method call
					this));	//object to call it on
	mGuiRoot->getChild("ScoreWindow")->getChild("ScoreReset")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::resetHighScores, //method call
					this));	//object to call it on
	mGuiRoot->getChild("GameOverWindow")->getChild("RestartButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::restart, //method call
					this));	//object to call it on
	mGuiRoot->getChild("GameOverWindow")->getChild("QuitButton")->subscribeEvent(
			CEGUI::PushButton::EventClicked,	//which event to call
			CEGUI::Event::Subscriber(&BreakOut3D::quit, //method call
					this));	//object to call it on
	mGuiRoot->getChild("HighScoreValue")->setText(std::to_string(mHighScore->getHighestScore()));
	CEGUI::FrameWindow* scoreWindow = static_cast<CEGUI::FrameWindow*>(mGuiRoot->getChild("ScoreWindow"));
	scoreWindow->setCloseButtonEnabled(true);
	scoreWindow->getCloseButton()->subscribeEvent(CEGUI::PushButton::EventClicked,
			CEGUI::Event::Subscriber(&BreakOut3D::hideHighScores, this));
	//Setup HighScore Fonts
	CEGUI::FontManager::getSingleton().createFreeTypeFont("HighScoreFont-18.font", 18, true, "HighScoreFont.ttf");
	CEGUI::FontManager::getSingleton().createFreeTypeFont("HighScoreFont-12.font", 12, true, "HighScoreFont.ttf");
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::quit(const CEGUI::EventArgs &e)
{
	mShutDown = true;
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::showHighScores(const CEGUI::EventArgs &e)
{
	mScoreBox->resetList();
	for(int i = 0; i < mHighScore->totalScores(); i++) {
		CEGUI::String name;
		CEGUI::String score;
		CEGUI::String rank;
		if(i % 2 == 0){
			rank = "[font='HighScoreFont-18.font'][colour='FFFF0000']" + std::to_string(i+1);
			name = "[font='HighScoreFont-18.font'][colour='FFFF0000']" + mHighScore->getName(i);
			score = "[font='HighScoreFont-18.font'][colour='FFFF0000']" + std::to_string(mHighScore->getScore(i));
		}
		else{
			rank = "[font='HighScoreFont-18.font'][colour='FF00FF00']" + std::to_string(i+1);
			name = "[font='HighScoreFont-18.font'][colour='FF00FF00']" + mHighScore->getName(i);
			score = "[font='HighScoreFont-18.font'][colour='FF00FF00']" + std::to_string(mHighScore->getScore(i));
		}
		int rowID = mScoreBox->addRow();	//Create new row and get row id
		CEGUI::ListboxTextItem* pRank= new CEGUI::ListboxTextItem(rank);
		CEGUI::ListboxTextItem* pName= new CEGUI::ListboxTextItem(name);
		pName->setSelectionBrushImage("GlossySerpentFHD/MultiListSelectionBrush");
		CEGUI::ListboxTextItem* pScore= new CEGUI::ListboxTextItem(score);
		// Add row to all the columns using row id received above
		mScoreBox->setItem(pRank, 0, rowID);
		mScoreBox->setItem(pName, 1, rowID);
		mScoreBox->setItem(pScore, 2, rowID);
	}
	mGuiRoot->getChild("MessageLabel")->setVisible(false);
	mGuiRoot->getChild("ScoreWindow")->setVisible(true);
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::resetHighScores(const CEGUI::EventArgs &e)
{
	mScoreBox->resetList();
	mHighScore->clearScores();
	mHighScore->writeToFile(HIGHSCORE_FILE);
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::hideHighScores(const CEGUI::EventArgs &e)
{
	mGuiRoot->getChild("ScoreWindow")->setVisible(false);
	mGuiRoot->getChild("MessageLabel")->setVisible(true);
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::restart(const CEGUI::EventArgs &e)
{
	if(!mGuiRoot->getChild("ScoreWindow")->isVisible()){
		mGuiRoot->getChild("GameOverWindow")->setVisible(false);
		showButtons(false);
		mStop = true;
		mLevel = 1;
		mNumBalls = INITIAL_NUM_BALLS;
		mBall->setSpeed(INITIAL_BALL_SPEED);
		mBall->setPosition(BALL_INITIAL_POSITION);
		mScore = 0;
		mScorePosition = -1;
		mGuiRoot->getChild("ScoreValue")->setText(std::to_string(mScore));
		mGuiRoot->getChild("LevelValue")->setText(std::to_string(mLevel));
		mGuiRoot->getChild("BallsValue")->setText(std::to_string(mNumBalls));
		clearBlocks();
		initialize(1);
		mGuiRoot->getChild("NameWindow")->setVisible(true);
		mGuiRoot->getChild("MessageLabel")->setText("Please Enter your name.");
		mGuiRoot->getChild("NameWindow")->getChild("NameBox")->activate();
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::resume(const CEGUI::EventArgs& e) {
	if(!mGuiRoot->getChild("ScoreWindow")->isVisible()){
		mPause = false;
		showButtons(false);
		mGuiRoot->getChild("MessageLabel")->setText("Press P to pause.");
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::nameEntered(const CEGUI::EventArgs& e) {
	Ogre::String pName(mGuiRoot->getChild("NameWindow")->getChild("NameBox")->getText().c_str());
	Ogre::StringUtil::trim(pName);	//trim white spaces
	if(pName != ""){	//Reject blank names or names with only whitespaces
		mPlayerName = pName;
		mGuiRoot->getChild("NameValue")->setText(pName);
		mGuiRoot->getChild("NameWindow")->setVisible(false);
		showButtons(true);
		mGuiRoot->getChild("ResumeButton")->setVisible(false);
		mGuiRoot->getChild("MessageLabel")->setText("Press Space to start the Game");
	}
	return true;
}
//-------------------------------------------------------------------------------------

void BreakOut3D::createCamera(void) {
	BaseApplication::createCamera();
	Ogre::Camera *camera2 = mSceneMgr->createCamera("TopCamera");
	camera2->setPosition(0, 950, 0);
	camera2->pitch(Ogre::Degree(-90));
}
//-------------------------------------------------------------------------------------
void BreakOut3D::createFrameListener(void) {
	BaseApplication::createFrameListener();
	mTrayMgr->hideTrays();
	mIntersectionQuery = mSceneMgr->createIntersectionQuery(INTERSECTABLE);
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::queryResult(Ogre::MovableObject* first,
		Ogre::MovableObject* second) {
	//If first object is ball and second paddle or vice versa
	if((first->getParentSceneNode() == mBall->getSceneNode()
			&& second->getParentSceneNode() == mPaddle->getSceneNode())
			|| (first->getParentSceneNode() == mBall->getSceneNode()
					&& second->getParentSceneNode() == mPaddle->getSceneNode())){
		Vector3 newDir = mBall->getDirection();
		if(newDir.dotProduct(Vector3::NEGATIVE_UNIT_Z) < 0){
			Vector3 paddlePos = mPaddle->getPosition();
			Vector3 ballPos = mBall->getPosition();
			newDir = ( ballPos - Vector3(paddlePos.x, paddlePos.y, paddlePos.z + 15)).normalisedCopy();
			mBall->setDirection(newDir);
		}
	}

	// go through each block and check to see if it has been hit by the ball
	for(int i = 0; i < mBlocks.size(); i++){

		// If the first object is ball and second object is the block or vice versa
		if(	(second->getParentSceneNode() == mBall->getSceneNode() && first->getParentSceneNode() == mBlocks[i]->getSceneNode()) ||
				(first->getParentSceneNode() == mBall->getSceneNode() && second->getParentSceneNode() == mBlocks[i]->getSceneNode()))	{
			Block* block = mBlocks[i];
			Vector3 ballPos = mBall->getPosition();
			// Get the Axis aligned bounding box of the block for calculating size
			// and maximum and minimum boundaries of AABB
			Ogre::AxisAlignedBox box = block->getSceneNode()->_getWorldAABB();
			Vector3 boxSize = box.getSize();

			Vector3 boxCenter = box.getCenter();
			int maxX = boxCenter.x + boxSize.x/2;
			int minX = boxCenter.x - boxSize.x/2;

			int maxY = boxCenter.y + boxSize.y/2 ;
			int minY = boxCenter.y - boxSize.y/2;

			int maxZ = boxCenter.z + boxSize.z/2;
			int minZ = boxCenter.z - boxSize.z/2;

			int ballX = ballPos.x;
			int ballY = ballPos.y;
			int ballZ = ballPos.z;

			Vector3 ballDir = mBall->getDirection();
			if(ballX >= maxX || ballX <= minX){
				ballDir.x = - ballDir.x;
			}
			if(ballY >= maxY || ballY <= minY){
				ballDir.y = - ballDir.y;
			}
			if(ballZ >= maxZ || ballZ <= minZ){
				ballDir.z = - ballDir.z;
			}
			mBall->setDirection(ballDir);

			// Remove the block from the scene
			mBlocks[i]->removeFromScene();
			mBlocks.erase(mBlocks.begin() + i);

			// Update GUI components
			mGuiRoot->getChild("BlocksValue")->setText(std::to_string(mBlocks.size()-LEVEL_UP_BLOCKS[mLevel]));
			mScore = mScore + SCORE_PER_BLOCK[mLevel];
			updateHighScores();
			mGuiRoot->getChild("ScoreValue")->setText(std::to_string(mScore));
			mGuiRoot->getChild("HighScoreValue")->setText(std::to_string(mHighScore->getHighestScore()));

			// level up
			if(mBlocks.size() <= LEVEL_UP_BLOCKS[mLevel]){
				clearBlocks();
				if(mLevel < MAX_LEVEL){
					mLevel = mLevel + 1;
					mNumBalls++;
					mGuiRoot->getChild("LevelValue")->setText(std::to_string(mLevel));
					mGuiRoot->getChild("LevelUpWindow")->getChild("LevelUpPrompt")->setText("Congratulations " + mPlayerName + " !!! You have reached Level "
							+ std::to_string(mLevel) + ". Press Space to start the new level.");
					mGuiRoot->getChild("LevelUpWindow")->setVisible(true);
					mGuiRoot->getChild("MessageLabel")->setText("Congratulations " + mPlayerName + " !!! \n You have reached Level "
							+ std::to_string(mLevel) + ". Press Space to start the new level.");
					mGuiRoot->getChild("BallsValue")->setText(std::to_string(mNumBalls));
					initialize(mLevel);
					showButtons(true);
					mGuiRoot->getChild("ResumeButton")->setVisible(false);
					mBall->setSpeed(BALL_SPEED[mLevel]);
					mPause = true;
					mStop = true;
				}
				else {
					mStop = true;
					mGuiRoot->getChild("GameOverWindow")->getChild("GameOverPrompt")->setText("Congratulations " + mPlayerName + " !!! You have completed the Game."  );
					mGuiRoot->getChild("GameOverWindow")->setVisible(true);
				}
			}
		}

	}
	return true;
}
//-------------------------------------------------------------------------------------
void BreakOut3D::updateHighScores(){
	if(mScorePosition == -1){
		int scorePos = mHighScore->getPlace(mScore);
		mHighScore->print();
		if(scorePos != -1){
			mScorePosition = mHighScore->getPlace(mScore);
			mHighScore->addScore(mPlayerName.c_str(), mScore);
		}
	}
	else{
		mScorePosition = mHighScore->updateScore(mScorePosition, mPlayerName.c_str(), mScore);
		mHighScore->writeToFile(HIGHSCORE_FILE);
	}
	mHighScore->print();

}
//-------------------------------------------------------------------------------------
void BreakOut3D::clearBlocks(){

	for(int i = 0; i < mBlocks.size(); i++){
		mBlocks[i]->removeFromScene();
	}
	for(int i = 0; i < mBlocks.size(); i++){
		mBlocks.erase(mBlocks.begin() + i);
	}
	mBlocks.clear();
}
//-------------------------------------------------------------------------------------
void BreakOut3D::createViewports(void) {
	mViewport = mWindow->addViewport(mCamera);
	mViewport->setDimensions(0, 0, .75, 1);
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	mCamera->setPosition(CAMERA_POSITION);
	Ogre::Camera *camera2 = mSceneMgr->getCamera("TopCamera");
	Ogre::Viewport* vp2 = mWindow->addViewport(camera2, 1);
	vp2->setDimensions(.75, 0, .25, 1);
	camera2->setAspectRatio(Ogre::Real(vp2->getActualWidth()) / Ogre::Real(vp2->getActualHeight()));
	vp2->setOverlaysEnabled(false);

}
//-------------------------------------------------------------------------------------
bool BreakOut3D::mouseMoved(const OIS::MouseEvent &arg)
{
	if(!mPause && !mStop) {
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(70,70);
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		Ogre::Real xPct = ((Ogre::Real)arg.state.X.abs)/mViewport->getActualWidth()*0.75;
		Ogre::Real yPct = ((Ogre::Real)arg.state.Y.abs)/mViewport->getActualHeight();
		int newX = 2*PADDLE_MAX_X*xPct -PADDLE_MAX_X;
		int newY = 2*PADDLE_MAX_Y*(1-yPct) - PADDLE_MAX_Y;
		mPaddle->setPosition(Ogre::Vector3(newX, newY, PADDLE_Z));
	}
	else {
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
		if (arg.state.buttonDown(OIS::MB_Right))
		{
			mCameraMan->injectMouseMove(arg);
		}
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
		// Scroll wheel.
		if (arg.state.Z.rel)
			sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	}
	return true;
}
//---------------------------------------------------------------------------
bool BreakOut3D::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
	return true;
}
//---------------------------------------------------------------------------
bool BreakOut3D::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

//-------------------------------------------------------------------------------------
bool BreakOut3D::keyPressed(const OIS::KeyEvent& arg) {
	//Check to see if Name prompt window is visible
	CEGUI::String isVisible = mGuiRoot->getChild("NameWindow")->getProperty("Visible");
	if(isVisible == "true") {	// Allow keyboard input only when name prompt window is visible
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan)arg.key);
		sys.getDefaultGUIContext().injectChar((CEGUI::Key::Scan)arg.text);
		return true;
	}
	else {
		if(arg.key == OIS::KC_SPACE && !mGuiRoot->getChild("ScoreWindow")->isVisible()){
			if(mStop && mNumBalls >= 0){
				mGuiRoot->getChild("LevelUpWindow")->setVisible(false);
				mPause = false;
				mStop = false;
				mGuiRoot->getChild("MessageLabel")->setText("Press P to pause.");
				showButtons(false);
				mBall->setDirection(Ogre::Vector3(Ogre::Math::RangeRandom(1,5),
						Ogre::Math::RangeRandom(1,5),-2).normalisedCopy());
			}
		}
		switch(arg.key){
		case OIS::KC_LEFT:
			mPaddleDirection = Vector3::NEGATIVE_UNIT_X;
			break;
		case OIS::KC_RIGHT:
			mPaddleDirection = Vector3::UNIT_X;
			break;
		case OIS::KC_UP:
			mPaddleDirection = Vector3::UNIT_Y;
			break;
		case OIS::KC_DOWN:
			mPaddleDirection = Vector3::NEGATIVE_UNIT_Y;
			break;
		case OIS::KC_P:
			if(!mStop && !mGuiRoot->getChild("ScoreWindow")->isVisible()){
				if(mPause){
					resume(CEGUI::EventArgs());
				}
				else {
					mPause = true;
					showButtons(true);
					mGuiRoot->getChild("MessageLabel")->setText("Press P to resume.");
				}
			}
			break;

		default:
			break;
		}
	}
	if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool BreakOut3D::keyReleased(const OIS::KeyEvent& arg) {
	switch(arg.key){
	case OIS::KC_UP:
		mPaddleDirection.y = 0;
		break;
	case OIS::KC_DOWN:
		mPaddleDirection.y = 0;
		break;
	case OIS::KC_LEFT:
		mPaddleDirection.x = 0;
		break;
	case OIS::KC_RIGHT:
		mPaddleDirection.x = 0;
		break;
	default:
		break;
	}
	return BaseApplication::keyReleased(arg);
}


//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char *argv[])
#endif
{
	// Create application object
	BreakOut3D app;

	try {
		app.go();
	} catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "An exception has occurred: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
	}

	return 0;
}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------


