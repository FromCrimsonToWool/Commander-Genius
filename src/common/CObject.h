/*
 * CObject.h
 *
 *  Created on: 17.05.2009
 *      Author: gerstrong
 *
 *  This class handles the objects like sprites that are draw on the screen
 *  It also handles the AI if any.
 */

#ifndef COBJECT_H_
#define COBJECT_H_

#define MAX_PLAYERS            4

// structures for each AI module's data
#include "../vorticon/ai/enemydata.h"
#include "../common/CMap.h"
#include "options.h"
#include <vector>

// Enumerations are here
#include "objenums.h"

class CObject {
public:
	CObject(CMap *pmap, int num_players=1, int index=0);
	
	unsigned int m_type;        // yorp, vorticon, etc.
	unsigned int m_index;        // Like an ID for some objects that need this implementation
	bool exists;
	bool onscreen;    			// true=(scrx,scry) position is visible onscreen
	bool hasbeenonscreen;
	unsigned int sprite;      			// which sprite should this object be drawn with
	int scrx, scry;           // x,y pixel position on screen
	
	// Bouncing Boxes
	Uint32 bboxX1, bboxX2;
	Uint32 bboxY1, bboxY2;

	// if type is OBJ_PLAYER, this contains the player number that this object
	// is associated with
	int AssociatedWithPlayer;
	
	// if zero, priority tiles will not be honored and object will always
	// appear in front of the background
	bool honorPriority;
	bool solid;
	
	bool canbezapped;         // if 0 ray will not stop on hitdetect
	int zapped;              // number of times got hit by keen's raygun
	int zapx, zapy, zapd;	   // x,y, and direction of last shot at time of impact
	char zappedbyenemy;	   // if 1, it was an ENEMYRAY and not keen that shot it
	
	char inhibitfall;         // if true common_enemy_ai will not do falling
	std::vector<bool> cansupportplayer;
	
	unsigned int blockedl, blockedr, blockedu, blockedd;
	signed int xinertia, yinertia;
	unsigned char xinertiatimer;
	
	unsigned char touchPlayer;      // 1=hit detection with player
	unsigned char touchedBy;        // which player was hit
	// Y position on this object the hit was detected
	// this is used for the yorps' bonk-on-the-head thing.
	// objects are scanned bottom to top, and first pixel
	// touching player is what goes in here.
	unsigned char hity;
	
	bool needinit;    // 1=new object--requires initilization
	bool dead;
	
	// data for ai and such, used differently depending on
	// what kind of object it is
	union ai
	{
		// ep1
		stYorpData yorp;
		stGargData garg;
		stVortData vort;
		stButlerData butler;
		stTankData tank;
		stRayData ray;
		stDoorData door;
		stIceChunk icechunk;
		stTeleportData teleport;
		stRopeData rope;
		
		// ep2
		stScrubData scrub;
		stPlatformData platform;
		stVortEliteData vortelite;
		stSEData se;
		stBabyData baby;
		
		// ep3
		stFoobData foob;
		stNinjaData ninja;
		stMeepData meep;
		stMotherData mother;
		stBallJackData bj;
		stNessieData nessie;
	} ai;
	
	void setupObjectType(int Episode);
	void setScrPos( int px, int py );
	bool calcVisibility( int player_x, int player_y );
	bool spawn(int x0, int y0, int otype, int Episode);
	void setIndex(int index);
	
	/*void moveTo(int x, int y);
	void moveLeft(int amount);
	void moveRight(int amount);
	void moveUp(int amount);
	void moveDown(int amount);*/

	virtual void process() { }
	
	bool hitdetect(CObject &hitobject);
	void kill();

	// Collision parts
	void performCollision(CMap *p_map);
	bool checkSolidR( int x2, int y1, int y2);
	bool checkSolidL( int x1, int y1, int y2);
	bool checkSolidU( int x1, int x2, int y1);
	bool checkSolidD( int x1, int x2, int y2);

	void processFalling(CMap *p_map);

	unsigned int x, y;        			// x,y location in map coords, CSFed

	virtual ~CObject();

protected:
	CMap *mp_Map;
};

#endif /* COBJECT_H_ */
