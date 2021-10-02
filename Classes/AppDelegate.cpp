/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainScene.hpp"

#include "TestScene.hpp"

#include "MainSounds.hpp"

#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

#include "MainMultiPlayer.hpp"

USING_NS_CC;

static int drSx = 704;//672;
static cocos2d::Size designResolutionSize = cocos2d::Size(drSx, 1024);

static cocos2d::Size mediumResolutionSize = cocos2d::Size(drSx*2, 2048);
static cocos2d::Size largeResolutionSize = cocos2d::Size(drSx*3, 3072);

AppDelegate::AppDelegate()
{
    
}

AppDelegate::~AppDelegate()
{
    MainMultiPlayer::disconnect();
    
    AudioEngine::end();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
    
    //set file resources files path
    auto filesPath = FileUtils::getInstance();
    filesPath->addSearchPath("/Users/axxwel/Documents/gamoap/kingfield/kingfield/proj.ios_mac/ios/Images.xcassets/");
    filesPath->addSearchPath("/Users/axxwel/Documents/gamoap/kingfield/kingfield/Resources/res/");
    
    // preload all game sound before start
    MainSounds::preLoad();
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("kingfield", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("kingfield");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    auto frameSize = glview->getFrameSize();
    float gameRatio = frameSize.height/frameSize.width;
    const float screenRatio = 1.5;
    
    // chose resolution in accordance to screen size
    if (gameRatio >= screenRatio)
    {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    }
    else
    {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    }
    
    //set sprites resolution scale factor.
    if(frameSize.width < 1000)
        director->setContentScaleFactor(2);
    else
        director->setContentScaleFactor(3);
    
    register_all_packages();
    
    // preload all game sound before start
    MainSounds::preLoad();

    // create the main scene.
    cocos2d::Scene* scene = nullptr;
    if(!TEST_SCENE)
    {
        scene = MainScene::createScene();
    }
    else
    {
        scene = TestScene::createScene();
    }
    
    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    AudioEngine::pauseAll();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    AudioEngine::resumeAll();
}
