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

	queue<string> imagesToDownload;
	queue<string>  jsonsToDownload;

	vector<InstagramImage*> images;
	
};

#endif