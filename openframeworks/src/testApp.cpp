#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	// string url = "https://api.instagram.com/v1/locations/1399341/media/recent?client_id=91d827904e3c452aab4936d9658b88fd";
	// 55.037826 36.748619
	//string url = "\"https://api.instagram.com/v1/media/search?distance=1000&lat=55.727686&lng=37.607007&client_id=91d827904e3c452aab4936d9658b88fd\"";
	string url = "\"https://api.instagram.com/v1/media/search?distance=1000&lat=55.037826&lng=36.748619&client_id=91d827904e3c452aab4936d9658b88fd\"";
	instagram.jsonsToDownload.push(url);
	instagram.startThread(true, true);
}


//--------------------------------------------------------------
void testApp::update() {
		for (int i = 0; i<instagram.downloadedImages.size(); i++) {
			images.push_back(ofPtr<ofImage>(new ofImage(instagram.downloadedImages[i])));
		}
		instagram.downloadedImages.clear();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for (int i = 0; i<images.size(); i++) {
		images[i]->draw(i*150%900, i/6*150);
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
