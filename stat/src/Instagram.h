#ifndef _INSTAGRAM_
#define _INSTAGRAM_

#include "ofMain.h"
#include <queue>
#include "ofxJSONElement.h"
#include "InstagramImage.h"

class Instagram : public ofThread {
public:
	
	Instagram(void);
	~Instagram(void);

	void threadedFunction();
	void update();

	queue<string>  jsonsToDownload;
	bool parseComplete;
	vector<ofxJSONElement> images;

	unsigned int max_timestamp;
	unsigned int min_timestamp;

	ofFile file;
	
	
};

#endif