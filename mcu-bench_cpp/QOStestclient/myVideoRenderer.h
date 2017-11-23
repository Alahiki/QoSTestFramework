#ifndef MY_VIDEO_RENDERER_H_
#define MY_VIDEO_RENDERER_H_

#include "woogeen/base/videorendererinterface.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

using namespace woogeen::base;
//namespace woogeen {
//namespace base {

class MyVideoRenderer : public VideoRendererARGBInterface
{
public:
	void RenderFrame(std::unique_ptr<ARGBBuffer> buffer);
	MyVideoRenderer();
	~MyVideoRenderer();

	int width;
	int height;
	struct timeval tv;
	uint8_t* argb;
	int mynum;
};
//}
//}

#endif