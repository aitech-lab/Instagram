#include "Instagram.h"

Instagram::Instagram(void) {
}


Instagram::~Instagram(void) {
}

// the thread function
void Instagram::threadedFunction() {
 
    while(isThreadRunning()) {
		
		if (jsonsToLoad.size()) {
			status = "download json";
			string file = "response-"+ofGetTimestampString()+".json"; 
			string cmd  = "curl.exe -k "+jsonsToLoad.front()+" -o "+"data\\"+file;
			system(cmd.c_str());
			jsonsToLoad.pop();
			status = "json download complete, parse";
			ofxJSONElement json;
			bool parsingSuccessful = json.openLocal(file);
			if ( parsingSuccessful ) {
				//cout << result.getRawString() << endl;
				for (int i=0; i<json["data"].size(); i++){
					string url = json["data"][i]["images"]["thumbnail"]["url"].asString();
					cout << url << "\n";
					imagesToLoad.push(url);
				}
			}

		}
		
		if(imagesToLoad.size()) {
			status = "download image";
			string url  = imagesToLoad.front();
			string file = url.substr(url.find_last_of("/")+1);
			FILE *fp = fopen(("data\\"+file).c_str(),"r");
			if(!fp) {
				string cmd  = "curl.exe -k "+url+" -o "+"data\\"+file;
				cout << cmd <<"\n";
				system(cmd.c_str());
			} else fclose(fp);
			imagesToLoad.pop();
			images.push_back(ofPtr<ofImage>(new ofImage(file)));
		} 

		Sleep(500);
    }
 
}