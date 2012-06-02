#include "Instagram.h"

Instagram::Instagram(void):
	parseComplete(false) {
}


Instagram::~Instagram(void) {
}

void Instagram::update() {

}

// the thread function
void Instagram::threadedFunction() {
 
    while(isThreadRunning()) {
		
		if (jsonsToDownload.size()) {
			parseComplete = false;
			string file = "response-"+ofGetTimestampString()+".json"; 
			string cmd  = "curl.exe -k \""+jsonsToDownload.front()+
				"&max_timestamp="+ofToString(max_timestamp)+
				"&min_timestamp="+ofToString(max_timestamp-60)+
				"\" -o \""+"data\\instagram-cache\\"+file+"\"";
			// jsonsToDownload.pop();

			system(cmd.c_str());
			
			ofxJSONElement json;
			
			if ( json.openLocal("instagram-cache\\"+file)) {
				min_timestamp = max_timestamp;
				for (int i=0; i<json["data"].size(); i++) {
					unsigned int timestamp = ofToInt(json["data"][i]["created_time"].asString());
					if(timestamp< min_timestamp) min_timestamp = timestamp;
					images.push_back(ofxJSONElement(json["data"][i]));
				}
				parseComplete = true;
				Sleep(1000);
				max_timestamp = min_timestamp;
			}
		} 
		Sleep(500);
    }
 
}