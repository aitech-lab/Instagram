#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxJSONElement.h"

class testApp : public ofBaseApp {
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void exit();
		
		double min_lat;
		double min_lng;
		double max_lat;
		double max_lng;

		vector<ofPoint> points;
		ofImage img;

		vector<ofRectangle> rects;
		vector<int>         integrals;
		void splitRect(ofRectangle rect);
		
		ofxJSONElement json;
	
};

#endif	

