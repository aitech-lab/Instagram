#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowTitle("Ailove-Instagram-Stat");
	ofBackground(0);

	depth = 8;

	min_lat = 1e8;
	min_lng = 1e8;
	max_lat =-1e8;
	max_lng =-1e8;

	ofBuffer b = ofBufferFromFile("stat.csv");
	b.getNextLine();
	while (!b.isLastLine()) {
		string s = b.getNextLine();
		int p = s.find(", ");
		if (p !=-1) {
			float lat = ofToFloat(s.substr(0, p));
			float lng = ofToFloat(s.substr(p+2));
			if(min_lat > lat) min_lat = lat;
			if(min_lng > lng) min_lng = lng;
			if(max_lat < lat) max_lat = lat;
			if(max_lng < lng) max_lng = lng;

			points.push_back(ofPoint(lat, lng));
		}
	}
	
	img     .allocate(4096, 4096,OF_IMAGE_COLOR_ALPHA);
	unsigned int* p1 = (unsigned int*) img.getPixels();
	memset(p1, 0 , 4096*4096*4);
	
	float lat_s = 4095.0/(max_lat - min_lat);
	float lng_s = 4095.0/(max_lng - min_lng);
	for (int i=0; i<points.size(); i++) {
		ofPoint& p = points[i];
		unsigned int px = (p.x-min_lat)*lat_s;
		unsigned int py = (p.y-min_lng)*lng_s;
		p1[px+4096*py] += 1;
	}
	
	
	for(unsigned int i=1; i<4096; i++) {
		p1[i     ] = p1[ i-1      ] + p1[i     ];
		p1[i*4096] = p1[(i-1)*4096] + p1[i*4096];
	}
	for(unsigned int i=1; i<4096; i++) {
		for(unsigned int j=1; j<4096; j++) {
			p1[i+j*4096] = 
				p1[(i-1) + j   *4096] + 
				p1[ i    +(j-1)*4096] - 
				p1[(i-1) +(j-1)*4096] + 
				p1[ i    + j   *4096] ;
		}
	}

	img.update();

	float cx = img.width /2;
	float cy = img.height/2;
	float r = max(cx,cy)*4;
	float a = 3.141592654/6;
	float x1 = cx+r*sin(a+1*3.1415926*2/3); float y1 = cx+r*cos(a+1*3.1415926*2/3);
	float x2 = cx+r*sin(a+2*3.1415926*2/3); float y2 = cx+r*cos(a+2*3.1415926*2/3);
	float x3 = cx+r*sin(a+3*3.1415926*2/3); float y3 = cx+r*cos(a+3*3.1415926*2/3);
	triangle = new FractalTriangle(x1, y1, x2, y2, x3, y3);
	splitTriangles(*triangle);

}


void testApp::splitTriangles(FractalTriangle& t) {
	
	unsigned static int w = img.width;
	unsigned static int h = img.height;
	unsigned static char* pixels = img.getPixels();

	if(t.l < depth) {
		t.split();
		getIntegralFromTriangle(*t.t1);
		getIntegralFromTriangle(*t.t2);
		getIntegralFromTriangle(*t.t3);
		getIntegralFromTriangle(*t.t4);

		splitTriangles(*t.t1);
		splitTriangles(*t.t2);
		splitTriangles(*t.t3);
		splitTriangles(*t.t4);

	} else {
		if (t.hasIntegral) triangles.push_back(t);
	}
}

bool testApp::getIntegralFromTriangle(FractalTriangle& t) {
	
	unsigned int* pixels = (unsigned int*) img.getPixels();

	int minx = (int)min(t.x1, min(t.x2, t.x3));
	int miny = (int)min(t.y1, min(t.y2, t.y3));
	int maxx = (int)max(t.x1, max(t.x2, t.x3));
	int maxy = (int)max(t.y1, max(t.y2, t.y3));
	
	if (minx>=0 && maxx<4096 && miny>0 && maxy<4096) {
		t.integral = pixels[minx+miny*4096] + pixels[maxx+maxy*4096] - pixels[minx+maxy*4096] - pixels[maxx+miny*4096];
		t.hasIntegral = true;
		return true;
	}
	return false;
}


void testApp::drawTriangles(FractalTriangle& t) {

	if (t.l == depth) {
		
		if (t.hasIntegral) {
			ofFill();
				float c = t.integral / 20.0;
				glBegin(GL_TRIANGLES);
				glColor4f(c, c, c, 1.1); glVertex2d(t.x1, t.y1);
				glColor4f(c, c, c, 1.1); glVertex2d(t.x2, t.y2);
				glColor4f(c, c, c, 1.1); glVertex2d(t.x3, t.y3);
				glEnd();
		}		
	}
	else {
		drawTriangles(*t.t1);
		drawTriangles(*t.t2);
		drawTriangles(*t.t3);
		drawTriangles(*t.t4);
	}
}

void testApp::exit() {
	delete triangle;
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetColor(255);
	ofScale(0.25,0.25);
	drawTriangles(*triangle);
	//img.draw(0,0,ofGetWidth(), ofGetHeight());
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
