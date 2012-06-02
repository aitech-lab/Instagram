#ifndef _INSTAGRAM_IMAGE_
#define _INSTAGRAM_IMAGE_

#include "ofMain.h"
#include "ofxJSONElement.h"

class InstagramImage : public ofThread {
public:
	InstagramImage(void);
	InstagramImage(const ofxJSONElement& json);
	~InstagramImage(void);

	static int id;
	static queue<InstagramImage*> imagesNeedsUpdate;

	void threadedFunction();
	void setJson(const ofxJSONElement& json);
	ofImage image;

	static void update();

private:
	ofxJSONElement _json;
	
};

#endif
