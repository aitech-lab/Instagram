#include "InstagramImage.h"

queue <InstagramImage*> InstagramImage::imagesNeedsUpdate;
int InstagramImage::id = 0;

InstagramImage::InstagramImage(void) {
	id++;
}

InstagramImage::InstagramImage(const ofxJSONElement& json) {
	setJson(json);
}


InstagramImage::~InstagramImage(void)
{
}

void InstagramImage::update() {

	while (InstagramImage::imagesNeedsUpdate.size()) {
		
		InstagramImage& img = *InstagramImage::imagesNeedsUpdate.front();
		ofPixels& pix = img.image.getPixelsRef();
		img.image.getTextureReference().allocate(
			pix.getWidth()
			,pix.getHeight()
			,ofGetGlInternalFormat(pix)
		);
		img.image.setUseTexture(true);
		img.image.update();

		InstagramImage::imagesNeedsUpdate.pop();
	}
}

void InstagramImage::threadedFunction() {

	string url  = _json["images"]["thumbnail"]["url"].asString();
	string file = url.substr(url.find_last_of("/")+1);
	FILE *fp = fopen(("data\\instagram-cache\\"+file).c_str(),"r");
	if(!fp) {
		string cmd  = "curl.exe -k "+url+" -o "+"data\\instagram-cache\\"+file;
		cout << cmd <<"\n";
		system(cmd.c_str());
	} else fclose(fp);
	image.setUseTexture(false);
	image.loadImage("instagram-cache\\"+file);
	
	imagesNeedsUpdate.push(this);
}

void InstagramImage::setJson(const ofxJSONElement& json) {
	_json = json;

	startThread(true, true);
}
