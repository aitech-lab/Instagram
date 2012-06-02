#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowTitle("Ailove-Instagram");
	ofBackground(0);
	// string url = "https://api.instagram.com/v1/locations/1399341/media/recent?client_id=91d827904e3c452aab4936d9658b88fd";
	// string url = "\"https://api.instagram.com/v1/locations/1399341/media/recent?max_id=177897742881064387_4234416&client_id=91d827904e3c452aab4936d9658b88fd\"";
	// string url = "\"https://api.instagram.com/v1/locations/1399341/media/recent?max_id=148506543753856562_4234416&client_id=91d827904e3c452aab4936d9658b88fd\"";
	// 55.037826 36.748619
	// string url = "\"https://api.instagram.com/v1/media/search?distance=100&lat=55.727686&lng=37.607007&client_id=91d827904e3c452aab4936d9658b88fd\"";
	// string url = "\"https://api.instagram.com/v1/media/search?distance=1000&lat=55.037826&lng=36.748619&client_id=91d827904e3c452aab4936d9658b88fd\"";
	// string url = "\"https://api.instagram.com/v1/tags/cat/media/recent?client_id=91d827904e3c452aab4936d9658b88fd\"";
	
	// Moscow 
	// lng: 37.296135 38.031335 
	// lat: 55.503198 55.941715

	lat = 55.751943;
	lng = 37.617123;
	string url = "https://api.instagram.com/v1/media/search"
		"?distance=20000"
		"&lat="+ofToString(lat)+
		"&lng="+ofToString(lng)+
		"&client_id=91d827904e3c452aab4936d9658b88fd";

	cout << url << "\n";
	instagram.max_timestamp = ofGetUnixTime();
	instagram.jsonsToDownload.push(url);
	instagram.startThread(true, true);
}


//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {	
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
	ofNoFill();
	ofSetCircleResolution(3);
	if(instagram.parseComplete) {
		for (int i = 0; i<instagram.images.size(); i++) {
			if (instagram.images[i].isMember("location")) {
				//cout << max_timestamp - ofToInt(instagram.images[i]["created_time"].asString()) << "\n";
				double x = (lat - instagram.images[i]["location"]["latitude" ].asDouble())*1000;
				double y = (lng - instagram.images[i]["location"]["longitude"].asDouble())*1000;
				ofCircle(x, y, 3);
			}
		}
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
