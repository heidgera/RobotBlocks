/*
 *  sideBar.cpp
 *  robotBlocksVectors
 *
 *  Created by Exhibits on 3/10/2554.
 *  Copyright 2011 Science Museum of Minnesota. All rights reserved.
 *
 */

#include "blocks.h"

extern string defaultFont;

/****************************** Sidebar *******************************
 **********************************************************************/

sideBar::sideBar(int _x,int _y,int w,int h,string file,ofColor col):ofInterObj(_x,_y,w,h){
	arialHeader.loadFont("DinC.ttf");
	arialHeader.setSize(20);
	bOver=bPressed=false;
	Open = false;
	xo=_x;
	yo=_y;
	color=col;
	filename=file;
}

sideBar::sideBar():ofInterObj(){
	arialHeader.loadFont(defaultFont);
	arialHeader.setSize(20);
	bOver=bPressed=false;
	Open = false;
}

sideBar::~sideBar() {
}
void sideBar::operator=(const sideBar t) {
	x=t.x;
	y=t.y;
	w=t.w;
	h=t.h;
	xo=t.xo;
	yo=t.yo;
}
void sideBar::draw(){
	if(Open){
		roundedShadow(x+20,y,w,h,h/2,.5);
		roundedBox(x,y,w,h,h/2,.5,.5,.52);
		roundedBox(x+(yo-y),y,w,h,h/2,r,g,b);
		ofSetColor(255, 255, 255);
		arialHeader.drawString(filename,x+w/4,y+3*h/4);
	}
	else {
		roundedBox(x,y,w,h,h/4,.5,.5,.52);
		roundedBox(x-200,y,w,h,h/4,r,g,b);
		ofSetColor(255, 255, 255);
		arialHeader.drawString(filename,x+w/4,y+3*h/4);
	}
}
/*void sideBar::draw(double k,double space){
	//drawTexture(textNum,x,y,w,h);
	//roundedShadow(x+5,y,w,h,h/4,.5);
	//roundedBox(x,y,w,h,h/4,.5,.51,.5);
	ofRoundShadow(x+5,y,w,h,h/4, .5);
	ofSetColor(0x555555);
	ofRoundBox(x,y,w,h,h/4, .2);
	ofSetColor(color);
	if(!Open) ofRoundBox(x-200,y,w,h,h/4, .2);
	else ofRoundBox(x,y,w,h,h/4, .2);
	//roundedBox(x-(200-((k-(y+h))/space)*(w-60)),y,w,h,h/4,r,g,b);  //--- re-add this if we go with scrolling bars
	glColor3f(1,1,1);
	arialHeader.drawString(filename,x+w/4,y+3*h/4);
}*/

void sideBar::draw(int _x, int _y){
	x=_x, y=_y;
	ofRoundShadow(x+5,y,w,h,h/4, .5);
	ofSetColor(0x555555);
	ofRoundBox(x,y,w,h,h/4, .2);
	ofSetColor(color);
	if(!Open) ofRoundBox(x-(w-60),y,w,h,h/4, .2);
	else ofRoundBox(x,y,w,h,h/4, .2);
	glColor3f(1,1,1);
	arialHeader.drawString(filename,x+w/4,y+3*h/4);
	if(Open){
		int temp=y+h+20;
		for (unsigned int j=0; j<blocks.size(); j++) {
			blocks[j].draw(20,temp);
			temp+=blocks[j].h+20;
		}
	}
}

double sideBar::updateSize()
{
	for (unsigned int i=0; i<blocks.size(); i++) {
		if (blocks[i].w>3*w/4) {
			w=max(w,4*blocks[i].w/3);
		}
	}
	return w;
}

/****************************** Sidebar Group *************************
 **********************************************************************/

void sbGroup::updateBlocks(int i){
	int divs=bars[i].blocks.size();
	for (unsigned int j=0; j<divs; j++) {
		int w=bars[i].blocks[j].w;
		bars[i].blocks[j].x=(bars[i].w-30-w)/2;
		bars[i].blocks[j].y=bars[i].y+5*bars[i].h/4+sideBarSpace*j*\
		((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace)/divs;
		bars[i].blocks[j].h=bars[i].blocks[j].oH*((bars[i+1].y-(bars[i].y+bars[i].h))/sideBarSpace);
	}
}
sbGroup::sbGroup(ofXML & xml,bGroup * destin):ofInterObj(){
	dest=destin;
	string font=xml.getCurrentTag().getAttribute("font");
	xml.setCurrentTag(";blocks");
	ofTag & tag=xml.getCurrentTag();
	for (unsigned int i=0; i<tag.size(); i++) {
		if (tag[i].getLabel()=="bar") {
			string col=tag[i].getAttribute("color");
			unsigned long colTrip=strtol(col.c_str(),NULL,0);
			ofColor color(colTrip);
			cout << tag[i].getAttribute("name") + " " + tag[i].getLabel() <<endl;
			unsigned int curBar=bars.size();
			bars.push_back( sideBar(-20,50+40*i,260,40,tag[i].getAttribute("name"),color));
			
			for (unsigned int j=0; j<tag[i].size(); j++) {
				if (tag[i][j].getLabel()=="block") {
					bars[curBar].blocks.push_back(block(tag[i][j],color,j*45));
				}
			}
		}
	}
	bars.push_back( sideBar(-20,50+40*bars.size(),260,40,"Filler",ofColor(0,0,0)));
	if (bars.size()) {
		y=bars[0].y;
		x=0;
		w=bars[0].w;
	}
	updateHeight();
	if(bars.size()>=2) bars[bars.size()-2].Open=true;
	for (unsigned int i=0; i<bars.size(); i++) {
		//updateBlocks(i);
	}
}
void sbGroup::setDest(bGroup * destin){
	dest=destin;
}

void sbGroup::updateHeight(){
	//double hgt=ofGetHeight();
	//if(hgt!=h+70){
	double maxWid=0;
	double maxHeight=0;
	for (unsigned int i=0; i<bars.size(); i++) {
		double hgt=20;
		for (unsigned int j=0; j<bars[i].size(); j++) {
			hgt+=bars[i][j].h+20;
		}
		maxHeight=max(maxHeight,hgt);
		maxWid=max(maxWid,bars[i].updateSize());
		//updateBlocks(i);
	}
	sideBarSpace=maxHeight;
	h=sideBarSpace-y+40*(bars.size());
	//sideBarSpace=h-40*(bars.size()-1);
	w=max(w,maxWid);
	for (unsigned int i=0; i<bars.size(); i++) {
		bars[i].w=maxWid;
	}
	//}
}

void sbGroup::update()
{
	for (unsigned int i=0; i<bars.size(); i++) {
		for (unsigned int j=0; j<bars[i].size(); j++) {
			if(bars[i][j].updateSize())
				updateHeight();
			bars[i][j].updatePositions();
		}
	}
}

void sbGroup::unfold(){
	for (unsigned int i=0; i<bars.size()-1; i++) {
		int dir;
		if (bars[i].Open&&bars[i+1].y<bars[i+1].yo+sideBarSpace) {
			dir = 1;
		}
		else if(!bars[i].Open&&bars[i+1].y>bars[i].y+bars[i].h){
			dir = -1;}
		else {
			dir=0;
		}
		if(dir){
			while((bars[i].Open&&bars[i+1].y<bars[i+1].yo+sideBarSpace)||\
				  (!bars[i].Open&&bars[i+1].y>bars[i].y+bars[i].h))
				for (unsigned int j=0; j<bars.size(); j++) {
					if (j>i) {
						bars[j].y+=dir;
						//updateBlocks(j);
					}
				}
		}
		//updateBlocks(i);
	}
}

void sbGroup::draw(int _x, int _y)
{
	x=_x, y=_y;
	//h=ofGetHeight()-y-20;
	//sideBarSpace=h-40*(bars.size()-1);
	draw();
}

void sbGroup::draw(){
	
	//--------- Draw a brown box onto the sidebar to hold the blocks
	//ofSetColor(0x80633B);
	ofSetColor(0x777777);
	ofShadeBox(0,y,w-15-25,h,OF_DOWN,.1);
	
	int pos=0;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		ofSetColor(0, 0, 0,60);
		ofRect(bars[i].x+bars[i].h/2, bars[i].y+bars[i].h, bars[i].w-bars[i].h, bars[i+1].y-(bars[i].y+bars[i].h));
		shadeHor(0,bars[i].y+bars[i].h,5,bars[i+1].y-(bars[i].y+bars[i].h),.4);
		shadeHor(bars[i].w-bars[i].h,bars[i].y+bars[i].h,-5,bars[i+1].y-(bars[i].y+bars[i].h),.4);
		if(bars[i].Open) shadeVer(0,bars[i].y+bars[i].h,bars[i].w-bars[i].h,5,.4);
		if(bars[i+1].y>bars[i].y+bars[i].h) shadeVer(0,bars[i+1].y,bars[i].w-bars[i].h,-5,.4);
		bars[i].draw(-20,pos+y);
		pos+=bars[i].h;
		if(bars[i].Open) pos+=sideBarSpace;
	}
	bars[bars.size()-1].y=y+h;
}

bool sbGroup::clickDown(double _x, double _y){
	bool ret=false;
	for (unsigned int i=0; i<bars.size()-1; i++) {
		if(bars[i].over(_x,_y)){ 
			for (unsigned int j=0; j<bars.size()-1; j++) {
				if (j!=i) {
					bars[j].Open=false;
				}
			}
			ret=bars[i].Open=true;
		}
		for (unsigned int j=0; j<bars[i].blocks.size(); j++) {
			if(dest&&bars[i].Open) dest->addFromSB(bars[i].blocks[j],_x,_y);
		}
	}
	return ret;
}
/*int sbGroup::size(){
	int ret=0;
	for (unsigned int i=0; i<bars.size(); i++) {
		ret+=bars[i].blocks.size();
	}
	return ret;
}*/
