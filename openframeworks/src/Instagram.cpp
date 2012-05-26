#include "Instagram.h"

Instagram::Instagram(void) {
}


Instagram::~Instagram(void) {
}

// the thread function
void Instagram::threadedFunction() {
 
    while(isThreadRunning()) {
		
		if (jsonsToDownload.size()) {
			string file = "response-"+ofGetTimestampString()+".json"; 
			string cmd  = "curl.exe -k "+jsonsToDownload.front()+" -o \""+"data\\"+file+"\"";
			system(cmd.c_str());
			jsonsToDownload.pop();
			status = INSTAGRAM_JSON_DOWNLOAD_COMPLET;
			ofxJSONElement json;
			bool parsingSuccessful = json.openLocal(file);
			if ( parsingSuccessful ) {
				//cout << result.getRawString() << endl;
				for (int i=0; i<json["data"].size(); i++){
					string url = json["data"][i]["images"]["thumbnail"]["url"].asString();
					cout << url << "\n";
					imagesToDownload.push(url);
				}
			}

		} else if(imagesToDownload.size()) {
			string url  = imagesToDownload.front();
			string file = url.substr(url.find_last_of("/")+1);
			FILE *fp = fopen(("data\\"+file).c_str(),"r");
			if(!fp) {
				string cmd  = "curl.exe -k "+url+" -o "+"data\\"+file;
				cout << cmd <<"\n";
				system(cmd.c_str());
			} else fclose(fp);
			imagesToDownload.pop();
			downloadedImages.push_back(file);
		} else {
			status = INSTAGRAM_IMAGES_DOWNLOAD_COMPLETE;
			stopThread(true);
		}

		Sleep(500);
    }
 
}