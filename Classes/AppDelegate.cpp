
#include "AppDelegate.h"

#include "Scene/TestScene.h"
#include "Scene/TitleScene.h"

#define TEST_SCENE

USING_NS_CC;
using namespace TapGun;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 16, 8};

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview) {
		glview = GLViewImpl::create("TapGun");
		director->setOpenGLView(glview);
	}

	// turn on display FPS
#if ( _DEBUG || DEBUG)
	director -> setDisplayStats( true);
#else
	director -> setDisplayStats( false);
#endif

	glview -> setDesignResolutionSize( 1280, 800, ResolutionPolicy::SHOW_ALL);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	glview -> setFrameSize( 1280, 800);
#endif
	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
#ifdef TEST_SCENE
	auto scene = Test::createScene();
#else
	auto scene = TitleScene::createScene();
#endif
	// run
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

	// if you use SimpleAudioEngine, it must be pause
	// SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

	// if you use SimpleAudioEngine, it must resume here
	// SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
