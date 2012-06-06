#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowTitle("Ailove-Instagram-Stat");
	ofBackground(0);

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
	
	
	splitRect(ofRectangle(0,0,4095,4095));
	
	/*	
	json["type"]="FeatureCollection";

	for(int i=0; i<rects.size(); i++) {
		// { "type"      : "Feature", 
		//   "id"        : 0, 
		//   "properties": { "longitude"  : 37.486007, 
		//                   "latitude"   : 55.677358 }, 
		//   "geometry"  : { "type"       : "Polygon", 
		//                   "coordinates": [ [ [ 37.486007, 55.677358 ], 
		//                                      [ 37.484710, 55.676092 ], 
		//                                      [ 37.482117, 55.676092 ], 
		//                                      [ 37.480821, 55.677358 ], 
		//                                      [ 37.482117, 55.678624 ], 
		//                                      [ 37.484710, 55.678624 ], 
		//                                      [ 37.486007, 55.677358 ] ] ] } }
		json["features"][i]["type"] = "Feature";
		json["features"][i]["id"  ] = i;

		json["features"][i]["properties"]["longitude"] = min_lng+(rects[i].y+rects[i].height/2)/lng_s;
		json["features"][i]["properties"]["latitude" ] = min_lat+(rects[i].x+rects[i].width /2)/lat_s;
		json["features"][i]["properties"]["count"    ] = integrals[i];


		json["features"][i]["geometry"]["type"] = "Polygon";
		json["features"][i]["geometry"]["coordinates"] = ofxJSONElement("[ [ [ "+
			ofToString(min_lng+ rects[i].y                 /lng_s)+ ","+
			ofToString(min_lat+ rects[i].x                 /lat_s)+"],["+
			ofToString(min_lng+ rects[i].y                 /lng_s)+ ","+
			ofToString(min_lat+(rects[i].x+rects[i].width )/lat_s)+"],["+
			ofToString(min_lng+(rects[i].y+rects[i].height)/lng_s)+ ","+
			ofToString(min_lat+(rects[i].x+rects[i].width )/lat_s)+"],["+
			ofToString(min_lng+(rects[i].y+rects[i].height)/lng_s)+ ","+
			ofToString(min_lat+ rects[i].x                 /lat_s)+
			"] ] ]");
	}
	json.save("stat.geojson");
	*/

}

void testApp::splitRect(ofRectangle r) {
	static char min = 4;
	int size = r.width/2;
	static unsigned int* p = (unsigned int*)img.getPixels();
	if( r.width < min) return;
	int x1, x2, x3, y1, y2, y3;
	x1 = r.x; x2= r.x+size, x3 = r.x+size*2;
	y1 = r.y; y2= r.y+size, y3 = r.y+size*2;

	int i0 = p[x1+y1*4096]+p[x2+y2*4096]-p[x1+y2*4096]-p[x2+y1*4096];
	int i1 = p[x2+y1*4096]+p[x3+y2*4096]-p[x2+y2*4096]-p[x3+y1*4096];
	int i2 = p[x1+y2*4096]+p[x2+y3*4096]-p[x1+y3*4096]-p[x2+y2*4096];
	int i3 = p[x2+y2*4096]+p[x3+y3*4096]-p[x2+y3*4096]-p[x3+y2*4096];
	ofRectangle r0(x1, y1, size, size);
	ofRectangle r1(x2, y1, size, size);
	ofRectangle r2(x1, y2, size, size);
	ofRectangle r3(x2, y2, size, size);
	if(i0) { if (i0 >= 8) splitRect(r0); else { rects.push_back(r0); integrals.push_back(i0); }}
	if(i1) { if (i1 >= 8) splitRect(r1); else { rects.push_back(r1); integrals.push_back(i1); }}
	if(i2) { if (i2 >= 8) splitRect(r2); else { rects.push_back(r2); integrals.push_back(i2); }}
	if(i3) { if (i3 >= 8) splitRect(r3); else { rects.push_back(r3); integrals.push_back(i3); }}
}

void testApp::exit() {
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {
	ofSetHexColor(0xFFFFFF);
	ofScale(0.25,0.25);
	//img.draw(0,0);

	for(int i=0; i<rects.size(); i++) {
		ofSetHexColor(integrals[i]*10000/rects[i].width/rects[i].width);
		ofRect(rects[i]);
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
