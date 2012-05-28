#include "Instagram.h"

Instagram::Instagram(void) {
}


Instagram::~Instagram(void) {
	for (int i=0; i<images.size(); i++) delete images[i];
	images.clear();
}

void Instagram::update() {
	InstagramImage::update();
}

// the thread function
void Instagram::threadedFunction() {
 
    while(isThreadRunning()) {
		
		if (jsonsToDownload.size()) {
			string file = "response-"+ofGetTimestampString()+".json"; 
			string cmd  = "curl.exe -k "+jsonsToDownload.front()+" -o \""+"data\\instagram-cache\\"+file+"\"";
			system(cmd.c_str());
			jsonsToDownload.pop();

			ofxJSONElement json;
			bool parsingSuccessful = json.openLocal("instagram-cache\\"+file);
			if ( parsingSuccessful ) {
				//cout << result.getRawString() << endl;
				for (int i=0; i<json["data"].size(); i++){
					images.push_back(new InstagramImage(json["data"][i]));
					
					//Sleep(100);
				}
				cout << json["pagination"]["next_url"].asString() << "\n";
				if(InstagramImage::id <100) jsonsToDownload.push("\""+json["pagination"]["next_url"].asString()+"\"");
			}
		} 
		Sleep(500);
    }
 
}