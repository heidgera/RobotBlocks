#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofExtended.h"
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
#include "ofxSystemCall.h"
#endif
#include "ofxShapes.h"
#include "blocks.h"
#include "ofVMouse.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
	void upload();
	void loadBlocks(string dir);
	
	
	ofDropDown	whichRobot;
	
	bGroup blocks;
	sbGroup * objects;
	ofXML parse;

	ofImage background;
	ofImage sideBarBack;
	
	ofButton upBut;
	ofButton clearBut;
	ofButton redoBut;
	ofButton undoBut;
	ofButton diyBut;
	ofButton homeBut;
	
	ofButton beginner;
	ofButton intermediate;
	ofButton advanced;
	
	ofTimer timeOut;
	ofFont	arial;
	ofSystemCall systemCall;
	ofProgressSpinner spinner;
	
	ofVideoPlayer crabMovie;
	ofVideoPlayer screenCast;
	
	ofVMouse anim;
	ofImage pointer;
};

#endif
