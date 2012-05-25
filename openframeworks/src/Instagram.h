#pragma once

#include "ofMain.h"
#include <queue>
#include "ofxJSONElement.h"


class Instagram :
	public ofThread
{
public:
	Instagram(void);
	~Instagram(void);

	void threadedFunction();

	string status;
	queue<string> imagesToLoad;
	queue<string> jsonsToLoad;

	vector< ofPtr<ofImage> > images;

	
};

