#include "TestScene.hpp"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* TestScene::createScene()
{
    return TestScene::create();
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
    
    
    
    GLProgram* glprogram = GLProgram::createWithFilenames("res/shaders/KFShader.vsh", "res/shaders/KFShader.fsh");
    GLProgramState* glProgramState = GLProgramState::getOrCreateWithGLProgram(glprogram);
    
   /* glProgramState->setUniformVec3("u_color", Vec3(1.0, 0.0, 0.0));
    glProgramState->setUniformFloat("u_line_size", 0.01);
    glProgramState->applyUniforms();*/
    
    //introLogo->setGLProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    
    
    Sprite* introLogo = Sprite::createWithSpriteFrameName("screen_logo.png");
    
    Layer* introLayer = Layer::create();
    
    this->addChild(introLayer);
    
    introLayer->addChild(introLogo);
    
    introLayer->setGLProgram(glprogram);
    
    
    moveSprite(introLogo);
    
    
    return true;
}
void TestScene::moveSprite(cocos2d::Sprite* sprite)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto move = MoveTo::create(10, Vec2(visibleSize.width, visibleSize.height));
    auto callFunc = CallFunc::create([=]()
    {
        sprite->removeFromParent();
    });
    auto seq = Sequence::create(move, callFunc, NULL);
    sprite->runAction(seq);
}
