#pragma once

#include "ofMain.h"
#include <queue>
#include "ofxJSONElement.h"

enum Status{
	INSTAGRAM_JSON_DOWNLOAD_COMPLET = 10,
	INSTAGRAM_IMAGES_DOWNLOAD_COMPLETE,
};

class Instagram :
	public ofThread
{
public:
	
	Instagram(void);
	~Instagram(void);

	void threadedFunction();

	Status status;
	queue<string> imagesToDownload;
	queue<string>  jsonsToDownload;

	vector<string> downloadedImages;

	
};

