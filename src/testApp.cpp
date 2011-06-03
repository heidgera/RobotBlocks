#include "testApp.h"

string curDir="beginner";
string ROOT_DIR;

void config(string file){
	ifstream config(ofToDataPath(file).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=");
		if(token.size()){
			if(!token[0].compare("ROOT_DIR")){
				ROOT_DIR = token[1];
			}
		}
		else {
			ROOT_DIR=".";
		}
		
	}
	config.close();
}

//--------------------------------------------------------------
void testApp::setup(){
	config("config.cfg");
	
	//ofSetDataPathRoot("../Resources/");
	
	//--------- Set up the (now unused) dropdown menu for selecting the block catagory
	whichRobot.setup(ofGetWidth()-150, 30, 50, 0);
	whichRobot.setValue(curDir);
	whichRobot.setValue("intermediate");
	whichRobot.setValue("advanced");
	
	//--------- Initialize the valid working space for the blocks
	blocks.setup(250, 0, ofGetWidth(), ofGetHeight());
	
	//--------- Parse the xml file for the blocks, and load them into objects
	parse.loadFile((ROOT_DIR+"/"+curDir+"/blocks.xml"));
	
	objects = new sbGroup(parse,&blocks);
	
	//--------- Load the background images (wood panel and aluminum
	background.loadImage("images/background.jpg");
	sideBarBack.loadImage("images/woodPanel.jpg");
	
	//--------- Load the images for the buttons
	clearBut.setup(64,64,"images/deleteBlocks2.png");
	upBut.setup( 128,128,"images/upload2.png");
	redoBut.setup(64, 64, "images/redo.png");
	undoBut.setup(64, 64, "images/undo.png");
	diyBut.setup(64, 64, "images/hand.png");
	homeBut.setup(64, 64, "images/home.png");
	
	beginner.setup(240, 240, "images/Beginner.png");
	beginner.setTitle("beginner");
	intermediate.setup(240, 240, "images/Intermediate.png");
	intermediate.setTitle("intermediate");
	advanced.setup(240, 240, "images/Advanced.png");
	advanced.setTitle("advanced");
	
	//--------- Load font for drawing on screen
	arial.loadFont("DinC.ttf");
	arial.setSize(30);
	arial.setMode(OF_FONT_CENTER);
	
	
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	
	//********** for some reason, vc++ hangs on loading rasapi32.dll when trying to load videos
	//******* hence this being only for OSX and linux
	
	crabMovie.loadMovie("movies/crab.mov");
	crabMovie.setVolume(0);
	crabMovie.play();
	
#endif
	
	//screenCast.loadMovie("movies/screenCast.mp4");
	//screenCast.setLoopState(false);
	anim.setup();
	ofHideCursor();
	pointer.loadImage("images/pointer.png");
}

//--------------------------------------------------------------
void testApp::update(){
	
	//---------- if one of the sidebars was pressed, unfold that bar
	//objects->unfold();
	
	//---------- blocks.update confirms that all the blocks are in the proper positions
	blocks.update();
	objects->update();
	
	//---------- Set which blocks are available to be pressed
	redoBut.setAvailable(blocks.redoAvailable());
	undoBut.setAvailable(blocks.undoAvailable());
	upBut.setAvailable(!systemCall.isRunning());
	
	//---------- for some reason, we run the upload function from the update loop, not the clicks (deprecated)
	/*if(upBut.pressed()){
		upload();
	}*/
	
	//--------- if the exhibit just timed out, clear the blocks
	if(timeOut.justExpired()){
		blocks.clear();
	}
	
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	
	//--------- Again, only working for osx, TODO: figure out why it doesn't work on the windows build
	if(timeOut.expired()){
		crabMovie.idleMovie();
		if(!crabMovie.isPlaying())
			crabMovie.play();
	}
	
#endif
	//if(screenCast.isPlaying())
	//	screenCast.idleMovie();
	//if(screenCast.getIsMovieDone())
	//	screenCast.setPaused(true);
	
	//--------- if we are running the upload script, spin the spinner
	if(systemCall.isRunning()){
		spinner.spin();
	}
	
	anim.update();
}

void slantBar(int x, int y, int w, int h, int H){
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x+w, y-H);
	glVertex2f(x+w, y-H+h);
	glVertex2f(x, y+h);
	glEnd();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//--------- If we have selected one of the groups of blocks, draw the composition screen
	if(timeOut.running()){
		
		//--------- Init the size variables for the side and bottom bar
		int sideWidth=objects->w-15;
		int bottomBarH=75;
		
		//--------- Slightly dim the background image, and draw it on the bottom layer
		//ofSetColor(178, 178, 178);
		//background.draw(0, 0,ofGetWidth(),ofGetHeight());
		ofBackground(0x33, 0x33, 0x33);
		
		/*ofSetColor(0x222222);
		for (int i=0; i*100<ofGetHeight()+200; i++) {
			slantBar(sideWidth, i*100, 200, 50, 150);
		}*/
		
		ofSetColor(0x444400);
		for (int i=0; i*10<ofGetHeight(); i++) {
			ofRect(sideWidth, i*10, ofGetWidth()-(sideWidth), 1);
		}
		for (int i=0; i*10<ofGetWidth(); i++) {
			ofRect(sideWidth+i*10, 0, 1, ofGetHeight());
		}
		
		//--------- Draw any blocks that are in the composition area and are not clicked.
		blocks.draw();
		
		
		//--------- if we are running the upload script, dim the background
		if(systemCall.isRunning()){
			ofSetColor(0, 0, 0,128);
			ofRect(0, 0, ofGetWidth(), ofGetHeight());
		}
		
		
		
		//********************* Bottom Bar *********************
		
		//--------- Draw a light gray bar on the bottom of the screen, and shade it gently
		ofSetColor(0xCFCFCF);
		ofShadeBox(0, ofGetHeight()-bottomBarH, ofGetWidth(), bottomBarH, OF_DOWN,(0xCF-0xA8)/255.);
		ofShade(0, ofGetHeight()-bottomBarH, 10, ofGetWidth(), OF_UP,.25);

		//--------- Draw the relevent buttons onto the bottom bar, in appropriate positions
		clearBut.draw(ofGetWidth()/2-100,ofGetHeight()-(bottomBarH+clearBut.h)/2);
		redoBut.draw(ofGetWidth()/2+200,ofGetHeight()-(bottomBarH+redoBut.h)/2);
		undoBut.draw(ofGetWidth()/2+100,ofGetHeight()-(bottomBarH+undoBut.h)/2);
	
		//--------- Draw these buttons on the bottom bar, to the right of the screen
		diyBut.draw(ofGetWidth()-100,ofGetHeight()-(bottomBarH+undoBut.h)/2);
		homeBut.draw(ofGetWidth()-200,ofGetHeight()-(bottomBarH+undoBut.h)/2);
		
		//********************* Sidebar *************************
		
		//--------- Dim the wood panel image slightly, and draw it on the left of the screen
		ofSetColor(190, 190, 190);
		double sideBarImgAdj=ofGetHeight()/double(background.width);
		background.draw(sideWidth-background.width*sideBarImgAdj, 0,background.width*sideBarImgAdj,ofGetHeight());
		
		/******** wood background
		ofSetColor(220, 220, 220);
		double sideBarImgAdj=ofGetHeight()/double(sideBarBack.width);
		sideBarBack.draw(sideWidth-sideBarBack.width*sideBarImgAdj, 0,sideBarBack.width*sideBarImgAdj,ofGetHeight());
		 */
		
		//--------- Shade the bar, and put drop shadows to the right of the bar
		ofShade(sideWidth, 0, 15, ofGetHeight(), OF_LEFT,.35);
		ofShade(sideWidth, 0, 10, ofGetHeight()-bottomBarH, OF_RIGHT,.5);
		ofShade(sideWidth, ofGetHeight()-bottomBarH, 5, bottomBarH, OF_RIGHT,.5);
		
		//--------- Draw the bars into the sidebar, and then draw the blocks for the open bar
		objects->draw(0,blocks.base.y+50);
		
		//******************** Menu Bar **************************
		//--------- Draw the programming base block at the top of the screen
		int menuHeight=100;
		
		blocks.drawBase((ofGetWidth()-300)/2, menuHeight);
		upBut.draw(blocks.base.x+blocks.base.w+10,blocks.base.y+30);
		
		//--------- Draw the bar on the top of the screen
		ofSetColor(0x555555);
		ofShadeBox(0, 0, ofGetWidth(), menuHeight, OF_DOWN, .3);
		ofShade(0, menuHeight, 10, ofGetWidth(), OF_DOWN);
		
		arial.setSize(70);
		arial.setMode(OF_FONT_TOP);
		string title="Drag blocks to program the crab!";
		ofSetColor(0xDFF500);
		arial.drawString(title, ofGetWidth()/2, 0);
		arial.setMode(OF_FONT_BOT);
		arial.setSize(30);
		
		
		/*//--------- Draw the block select menu at the top of the screen
		ofSetColor(0,0,0);
		ofRoundBox(beginner.x-25, -(beginner.h+25), beginner.w*3+50, beginner.h*2+50, beginner.h/2+5, 0);
		ofSetColor(178, 178, 178);
		ofRoundBox(beginner.x-20, -(beginner.h+20), beginner.w*3+40, beginner.h*2+40, beginner.h/2, .1);
		
		//--------- Draw the block select buttons
		ofSetColor(255, 255, 255,128);
		if(!beginner.getAvailable()) ofRoundShape(beginner.x, beginner.y, beginner.w, beginner.h, beginner.h/4,true);
		else if(!intermediate.getAvailable()) ofRoundShape(intermediate.x, beginner.y, beginner.w, beginner.h, beginner.h/4,true);
		if(!advanced.getAvailable()) ofRoundShape(advanced.x, beginner.y, beginner.w, beginner.h, beginner.h/4,true);
		beginner.draw(ofGetWidth()/2-3*beginner.w/2, 0,128,128);
		intermediate.draw(ofGetWidth()/2-beginner.w/2, 0,128,128);
		advanced.draw(ofGetWidth()/2+beginner.w/2, 0,128,128);
		 */
		
		//--------- Draw the dropdown, if it is being used
		//whichRobot.move(ofGetWidth()-(whichRobot.w+whichRobot.h), 4);
		//whichRobot.draw();
		
		//*********************** Upload on the side *********************
		
		//ofSetColor(178, 178, 178);
		//ofRoundBox(upBut.x-20, upBut.y-20, upBut.w*3, upBut.h+40, upBut.h/2+20, .2);
		//upBut.draw(ofGetWidth()-(upBut.w+20),(ofGetHeight()-upBut.h)/2);
		
		//********************** Draw the blocks which are being held by the mouse ********
		
		blocks.drawForeground();
		
		/*ofSetColor(255,0,0);
		ofRegularPolygon(500, 500, 100, 8);
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofSetLineWidth(3);
		ofRegularPolygon(500, 500, 100, 8);
		ofFill();*/
		//if(screenCast.isPlaying()){
			//screenCast.draw(0, 0);
		//}
	}
	//************************** if the exhibit has timed out, draw the welcome screen ************** 
	else {
		
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
		//--------- Again, dumb movie crap
		ofSetColor(255, 255, 255);
		crabMovie.draw(0,0,ofGetWidth(),ofGetHeight());
#endif
		//--------- Get the size of the text so we can draw a nice box on screen
		string printOut="Select a difficulty to begin";
		ofRectangle k=arial.getBoundingBox(printOut, ofGetWidth()/2, ofGetHeight()/2);
		
		//--------- Draw a nice box on screen
		ofSetColor(0,0,0,128);
		ofRoundShape(ofGetWidth()/4-beginner.w/2-20, k.y-20, ofGetWidth()/2+beginner.w+40, k.height+beginner.h+40, 10, 1);
		
		//--------- Draw the text in the box
		ofSetColor(255, 255, 255);
		arial.drawString(printOut, ofGetWidth()/2, ofGetHeight()/2);
		
		//--------- Draw teh block select buttons
		beginner.draw(ofGetWidth()/4-beginner.w/2, k.y+k.height,240,240);
		intermediate.draw(2*ofGetWidth()/4-intermediate.w/2, k.y+k.height,240,240);
		advanced.draw(3*ofGetWidth()/4-advanced.w/2, k.y+k.height,240,240);
	}
	
	//************************ if we're uploading, draw a fancy "uploading..." on screen

	if (systemCall.isRunning()) {
		ofSetColor(255, 255, 255);
		spinner.draw((ofGetWidth())/2,ofGetHeight()/2, 300);
		arial.setMode(OF_FONT_LEFT);
		arial.setSize(70);
		string printOut="uploading";
		for (int i=0; i<(spinner.count()/3)%4; i++) {
			printOut.append(".");
		}
		ofSetColor(255, 255, 255);
		arial.drawString(printOut, ofGetWidth()/4, ofGetHeight()/4);
		arial.setMode(OF_FONT_CENTER);
		arial.setSize(30);
	}
	
	if(anim.isClicked()) ofSetColor(128, 128, 128)
		;	else ofSetColor(255, 255, 255);
	//ofCircle(anim.x, anim.y, 20);
	if(anim.isPlaying()) pointer.draw(anim.x-10, anim.y, pointer.width*2,pointer.height*2);
	else pointer.draw(mouseX-10, mouseY, pointer.width*2,pointer.height*2);
}

void testApp::upload()
{
	//--------- Generate the file which we will compile and upload to the arduino
	ofstream out(ofToDataPath("arduino_make/arduino_make.pde").c_str());
	blocks.writeFile(&out);
	out.close();
	
	//--------- once we've generated the file, compile and upload with one of teh following scripts
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	systemCall.run(ofToDataPath("arduino_make/upload.sh"));
#else
	systemCall.run(ofToDataPath("\"data\\arduino_make\\build.bat\""));
#endif
	
	//--------- unpress the upload button 
	upBut.setPressed(false);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//************** none of these are used in any real fashion
	if(key=='w'){
		upload();
	}
	if(key=='p'){
		if(!anim.isPlaying()){
			blocks.clear();
			int hDisp=0;
			int wDisp=0;
			anim.clearEvents();
			anim.addEvent(OF_VMOUSE_MOVE_TO, mouseX,mouseY, 0);
			if(!(*objects)[0].Open){
				anim.addEvent(OF_VMOUSE_CLICK_DOWN, (*objects)[0], .5);
				anim.addEvent(OF_VMOUSE_CLICK_UP, (*objects)[0], 0.5);
			}
			if((*objects)[0][0].ddGroup.size()){
				wDisp=(*objects)[0][0].ddGroup[0].xdis+40;
				hDisp=(*objects)[0][0].h/4+5;
			}
			anim.addEvent(OF_VMOUSE_MOVE_TO, (*objects)[0][0], 1.5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, (*objects)[0][0], .25);
			anim.addEvent(OF_VMOUSE_MOVE_TO, blocks.base,40,blocks.base.h+10, 1);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,40,blocks.base.h+10, .5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, blocks.base,wDisp,blocks.base.h+hDisp, .5);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,wDisp,blocks.base.h+hDisp, .5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, blocks.base,wDisp,blocks.base.h+hDisp+30, .5);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,wDisp,blocks.base.h+hDisp+30, .5);
			//anim.addEvent(OF_VMOUSE_CLICK_DOWN, blocks.base.blocksOn[0].ddGroup[0], .5);
			//anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base.blocksOn[0].ddGroup[0], .5);
			hDisp=(*objects)[0][0].h;
			anim.addEvent(OF_VMOUSE_MOVE_TO, (*objects)[0][1], 1.5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, (*objects)[0][1], .25);
			anim.addEvent(OF_VMOUSE_MOVE_TO, blocks.base,40,blocks.base.h+hDisp+10, 1);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,40,blocks.base.h+hDisp+10, .5);
			if((*objects)[0][0].ddGroup.size()){
				wDisp=(*objects)[0][0].ddGroup[0].xdis+40;
				hDisp+=(*objects)[0][0].h/4+5;
			}
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, blocks.base,wDisp,blocks.base.h+hDisp, .5);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,wDisp,blocks.base.h+hDisp, .5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, blocks.base,wDisp,blocks.base.h+hDisp+45, .5);
			anim.addEvent(OF_VMOUSE_CLICK_UP, blocks.base,wDisp,blocks.base.h+hDisp+45, .5);
			anim.addEvent(OF_VMOUSE_CLICK_DOWN, upBut, .75);
			anim.addEvent(OF_VMOUSE_CLICK_UP, upBut, .5);
			anim.addEvent(OF_VMOUSE_MOVE_TO, mouseX,mouseY, .5);
			anim.reset();
		}
		anim.play();
	}
	if(key=='r'){
		if(!anim.isPlaying()) anim.play();
		else anim.pause();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	//--------- if the mouse is moving and not clicked, inform the blocks
	blocks.motion(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	//--------- if we are dragging the mouse, tell blocks so it can update the positions of any of the held blocks
	blocks.drag(x, y);
}

//****************** Function for changing the selected set of blocks ***********

void testApp::loadBlocks(string dir){
	
	//--------- set current directory for finding the xml file, and set the dropdown, if we want to.
	curDir=dir;
	//whichRobot.setSelected(curDir);
	
	//--------- Delete the old parsing of the xml file and the current selection of blocks
	delete objects;
	
	//--------- reparse the xml file and load the new blocks with that data
	parse.loadFile((ROOT_DIR+"/"+curDir+"/blocks.xml").c_str());
	objects = new sbGroup(parse,&blocks);
	
	//--------- set the timeout timer to three minutes
	timeOut.set(180);
	
#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
	//--------- Pause the movie, if we are using the movie
	crabMovie.setPaused(true);
#endif
	windowResized(0, 0);
}

//***************** Mouse Click down function ***************
void testApp::mousePressed(int x, int y, int button){
	vector<ofButton *> choices;
	choices.push_back(&beginner);
	choices.push_back(&intermediate);
	choices.push_back(&advanced);
	//--------- Check the block selection buttons; if pressed, load a different set of blocks each
	if(!timeOut.running()){
		for (unsigned int i=0; i<choices.size(); i++) {
			if(choices[i]->clickDown(x,y)){
				for (unsigned int j=0; j<choices.size(); j++) 
					if(j!=i) choices[j]->setAvailable(true);
				choices[i]->setAvailable(false);
				loadBlocks(choices[i]->title);
			}
			
		}
	}
	//--------- If we aren't timed out, check all of the click for the composition screen
	else {
		
		//--------- reset the timeOut timer with each click
		timeOut.set(180);
		
		//--------- Check the dropdown, and load some blocks if we're doing that sort of thing (removed for beta version)
		/*if(whichRobot.clickDown(x, y)){}
		if(whichRobot.selected){
			loadBlocks(whichRobot.getString());
			whichRobot.selected=false;
		}*/
		
		//--------- Check the blocks in the sidebar, to see if they have been clicked
		objects->clickDown(x, y);
		
		//--------- Check the blocks in the comp area
		blocks.clickDown(x, y);
		
		//--------- Run upload function if the upload button is pressed
		if(upBut.clickDown(x, y)){
			upload();
		}
		
		//--------- if home button is pressed, expire the timeOut timer and clear the blocks
		if(homeBut.clickDown(x, y)){
			for (unsigned int j=0; j<choices.size(); j++) 
				choices[j]->setAvailable(true);
			timeOut.reset();
			blocks.clear();
		}
		
		//--------- If we press the clear button, clear the blocks in the comp area
		if (clearBut.clickDown(x, y)) {
			blocks.clear();
		}
		
		//--------- if we press the undo button, roll back the state of the blockGroup
		if (undoBut.clickDown(x, y)) {
			blocks.undoState();
		}
		
		//--------- if we press the redo button, push the state forward
		if (redoBut.clickDown(x, y)) {
			blocks.redoState();
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	//--------- do a bunch of clickups
	blocks.clickUp(x, y);
	upBut.clickUp();
	clearBut.clickUp();
	redoBut.clickUp();
	undoBut.clickUp();
	homeBut.clickUp();
	beginner.clickUp();
	advanced.clickUp();
	intermediate.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	//--------- if the window changes size, refresh the size of the objects group
	objects->updateHeight();
	blocks.move(objects->w, 0);
	blocks.changeSize(ofGetWidth()-blocks.x, ofGetHeight());
}

