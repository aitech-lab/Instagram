#include "Instagram.h"

Instagram::Instagram(void):
parseComplete(false) {
	file.open(ofToDataPath(ofGetTimestampString()+".csv"), ofFile::WriteOnly, false);
	file << "created_time, latitude, longitude, user_id, id" << "\n";
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
			string jsonFile = "response-"+ofGetTimestampString()+".json"; 
			string cmd  = "curl.exe -k \""+jsonsToDownload.front()+
				"&max_timestamp="+ofToString(max_timestamp)+
				"&min_timestamp="+ofToString(max_timestamp-60)+
				"\" -o \""+"data\\instagram-cache\\"+jsonFile+"\"";
			// jsonsToDownload.pop();

			system(cmd.c_str());
			
			ofxJSONElement json;
			
			if ( json.openLocal("instagram-cache\\"+jsonFile)) {
				min_timestamp = max_timestamp;
				for (int i=0; i<json["data"].size(); i++) {
					unsigned int timestamp = ofToInt(json["data"][i]["created_time"].asString());
					if(timestamp< min_timestamp) min_timestamp = timestamp;
					
					string lat = json["data"][i]["location"]["latitude" ].toStyledString();
						string lng = json["data"][i]["location"]["longitude"].toStyledString();
					lat = lat.substr(0, lat.size()-2);
					lng = lng.substr(0, lng.size()-2);

					file << 
						//json["data"][i]["images"]["low_resolution"]["url"].asString() << ", " <<
						json["data"][i]["created_time"].asString() << ", " <<
						lat                                        << ", " << 
						lng                                        << ", " <<
						json["data"][i]["user"]["id"].asString()   << ", " <<
						json["data"][i]["id"].asString()           << "\n";
					//images.push_back(ofxJSONElement(json["data"][i]));
				}
				file.flush();
				parseComplete = true;
				Sleep(1000);
				if (min_timestamp!=max_timestamp) max_timestamp = min_timestamp;
				else max_timestamp -=60;
			}
		} 
		Sleep(500);
    }
 
}