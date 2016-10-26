//============================================================================
// Name        : BreakOut3D.h
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

#ifndef __BREAKOUT3D_H_
#define __BREAKOUT3D_H_

#include "BaseApplication.h"
#include "Paddle.h"
#include "HighScores.h"
#include "Ball.h"
#include "Block.h"
#include "Constants.h"
#include <vector>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

//---------------------------------------------------------------------------

class BreakOut3D : public BaseApplication, public Ogre::IntersectionSceneQueryListener
{
public:
	static const Ogre::uint32 INTERSECTABLE;

	BreakOut3D(void);
	virtual ~BreakOut3D(void);


protected:
	CEGUI::OgreRenderer* mRenderer;
	virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createFrameListener(void);
	virtual void createViewports(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool queryResult(Ogre::MovableObject* first, Ogre::MovableObject* second);
	virtual bool queryResult(Ogre::MovableObject* movable, Ogre::SceneQuery::WorldFragment* fragment){
		return true;
	}
	/**
	 * initialize: Initialized blocks for each level
	 * Paramenter:
	 * 		level: level that is to be initialized
	 */
	void initialize(int level);

	/**
	 * clearBLocks: clears all the blocks from the scene
	 */
	void clearBlocks();

	/**
	 * setupGui: sets up the Graphical User Interface of the game
	 */
	void setupGUI();

	/**
	 * quit: Closes the game (called when Quit button is clicked)
	 */
	bool quit(const CEGUI::EventArgs &e);

	/**
	 * restart: Restarts the game (called when Restart button is clicked)
	 */
	bool restart(const CEGUI::EventArgs &e);

	/**
	 * Resume a paused game (called when Resume button is clicked)
	 */
	bool resume(const CEGUI::EventArgs &e);

	/**
	 * Set's the name of the current player
	 * (called when OK button of name prompt dialog box is clicked)
	 */
	bool nameEntered(const CEGUI::EventArgs &e);

	/**
	 * makes the high score list window visible and sets the high scores
	 */
	bool showHighScores(const CEGUI::EventArgs &e);

	/**
	 * Hides the high score list window
	 */
	bool hideHighScores(const CEGUI::EventArgs &e);

	/**
	 * Clears the high scores stored so far
	 */
	bool resetHighScores(const CEGUI::EventArgs &e);

	/**
	 * Changes the visibility of all the buttons in the scene
	 * Parameter:
	 * 		val: true if buttons are to be shown, false otherwise
	 */
	void showButtons(bool val);

	/**
	 * Updates the Highscores if the current score is eligible
	 * to be in the high score list (this method is called every time high score changes)
	 */
	void updateHighScores();

	/**
	 * Window manager for CEGUI
	 */
	CEGUI::Window *mGuiRoot;

private:
	/**
	 * High Scores for the game
	 */
	HighScores* mHighScore;

	/**
	 * Multi column List for Dispalying high scores
	 */
	CEGUI::MultiColumnList* mScoreBox;

	/**
	 * Position of current score in the high score table
	 * -1 if the score is not yet in th high score list
	 */
	int mScorePosition = -1;

	/**
	 * Direction of the paddle
	 */
	Ogre::Vector3 mPaddleDirection;

	/**
	 * Paddle for the game
	 */
	Paddle* mPaddle;

	/**
	 * Ball for the game
	 */
	Ball* mBall;

	/**
	 * List of all the blocks that are currently in the scene
	 */
	std::vector<Block*> mBlocks;

	/**
	 * Current level of the game
	 */
	int mLevel;

	/**
	 * Current score of the player
	 */
	int mScore;

	/**
	 * Initial number of balls remaining
	 */
	int mNumBalls = INITIAL_NUM_BALLS;

	/**
	 * Name of current player
	 */
	CEGUI::String mPlayerName;

	/**
	 * Intersection query for handling collision between blocks and ball
	 */
	Ogre::IntersectionSceneQuery *mIntersectionQuery;

	/**
	 * Old position of the paddle.
	 */
	Ogre::Vector3 mOldPosition;

	/**
	 * Is the game being paused?
	 */
	bool mPause = true;
	/**
	 * Is the game being stopped
	 */
	bool mStop = true;

	/**
	 * Viewport for the main game window.
	 */
	Ogre::Viewport* mViewport;


};

//---------------------------------------------------------------------------

#endif // #ifndef __BREAKOUT3D_H_

//---------------------------------------------------------------------------
