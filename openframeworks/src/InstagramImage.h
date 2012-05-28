#pragma once

#include "ofMain.h"

class InstagramImage : public ofThread {
public:
	InstagramImage(void);
	~InstagramImage(void);
	void threadedFunction();
};

