#include "AppDelegate.h"
#include "ScriptingCore.h"
#include "jsb_crossapp_auto.hpp"
#include "crossapp_specifics.hpp"
#include "js_crossapp_delegates_manual.hpp"

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CAApplication* applicationn = CAApplication::getApplication();
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    applicationn->setOpenGLView(pEGLView);
    
    ScriptingCore* sc = ScriptingCore::getInstance();
    
    sc->addRegisterCallback(register_all_crossapp);
    sc->addRegisterCallback(register_crossapp_js_core);
    sc->addRegisterCallback(register_all_crossapp_delegates_manual);
    sc->addRegisterCallback(register_jsb_websocket);
    sc->start();
    sc->enableDebugger();
    
    sc->runScript("script/jsb_boot.js");
    sc->runScript("script/jsb_crossapp.js");
    
    CAScriptEngineManager::sharedManager()->setScriptEngine(sc);
    
    sc->runScript("js/main.js");

    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CAApplication::getApplication()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CAApplication::getApplication()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
