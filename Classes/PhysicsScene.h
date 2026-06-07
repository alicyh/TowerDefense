#ifndef __PHYSICS_SCENE_H__
#define __PHYSICS_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class PhysicsScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* scene();
    virtual bool init();
    
    void update(float dt) override;
    
    CREATE_FUNC(PhysicsScene);
    
private:
    PhysicsWorld* _world;
    DrawNode* _drawNode;
    Label* _infoLabel;
    
    std::vector<Sprite*> _physicsObjects;
    
    void initUI();
    void initPhysicsWorld();
    void createBoundaries();
    void spawnCircle(const Vec2& position);
    void spawnBox(const Vec2& position);
    void spawnTriangle(const Vec2& position);
    void spawnRandomObjects();
    void applyForceToAll(float forceX, float forceY);
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void updateInfo();
    void goToMainMenu(Ref* sender);
};

#endif // __PHYSICS_SCENE_H__
