#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	
	instagram.jsonsToLoad.push(string("https://api.instagram.com/v1/locations/1399341/media/recent?client_id=91d827904e3c452aab4936d9658b88fd"));
	instagram.startThread(true, true);
}


//--------------------------------------------------------------
void testApp::update(){
	for (int i = 0; i<instagram.images.size(); i++)
		instagram.images[i]->update();
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for (int i = 0; i<instagram.images.size(); i++) {
		unsigned char* pixels = instagram.images[i]->getPixels();
		instagram.images[i]->draw(i*150%600, i/4*150);
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
