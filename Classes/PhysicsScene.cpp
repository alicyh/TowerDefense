#include "PhysicsScene.h"
#include "AppMacros.h"
#include "TestScene.h"

USING_NS_CC;

Scene* PhysicsScene::scene()
{
    return PhysicsScene::create();
}

bool PhysicsScene::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }
    
    _world = getPhysicsWorld();
    _world->setGravity(Vec2(0, -980));
    _world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = LayerColor::create(Color4B(20, 30, 40, 255));
    this->addChild(background, -1);
    
    initUI();
    initPhysicsWorld();
    createBoundaries();
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(PhysicsScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(PhysicsScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    
    this->scheduleUpdate();
    this->schedule(schedule_selector(PhysicsScene::spawnRandomObjects), 2.0f, CC_REPEAT_FOREVER, 1.0f);
    
    return true;
}

void PhysicsScene::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto title = Label::createWithTTF("Physics World Simulation", "fonts/arial.ttf", 32);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 40));
    title->setColor(Color3B::YELLOW);
    this->addChild(title, 10);
    
    _infoLabel = Label::createWithTTF("", "fonts/arial.ttf", 18);
    _infoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 80));
    _infoLabel->setColor(Color3B::WHITE);
    this->addChild(_infoLabel, 10);
    
    auto hint1 = Label::createWithTTF("Tap to spawn circles", "fonts/arial.ttf", 16);
    hint1->setPosition(Vec2(origin.x + 120, origin.y + 60));
    hint1->setColor(Color3B::GREEN);
    this->addChild(hint1, 10);
    
    auto hint2 = Label::createWithTTF("Press B for boxes, T for triangles", "fonts/arial.ttf", 16);
    hint2->setPosition(Vec2(origin.x + visibleSize.width - 200, origin.y + 60));
    hint2->setColor(Color3B::GREEN);
    this->addChild(hint2, 10);
    
    auto hint3 = Label::createWithTTF("Press SPACE for explosion!", "fonts/arial.ttf", 16);
    hint3->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 30));
    hint3->setColor(Color3B::RED);
    this->addChild(hint3, 10);
    
    auto backBtn = MenuItemFont::create("Back to Menu", CC_CALLBACK_1(PhysicsScene::goToMainMenu, this));
    backBtn->setFontSizeObj(20);
    backBtn->setColor(Color3B::WHITE);
    
    auto menu = Menu::create(backBtn, nullptr);
    menu->setPosition(Vec2(origin.x + 80, origin.y + visibleSize.height - 40));
    this->addChild(menu, 10);
    
    updateInfo();
}

void PhysicsScene::initPhysicsWorld()
{
    _drawNode = DrawNode::create();
    this->addChild(_drawNode, 1);
}

void PhysicsScene::createBoundaries()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    edgeNode->setPhysicsBody(edgeBody);
    this->addChild(edgeNode);
    
    auto platformHeight = visibleSize.height * 0.2f;
    
    auto platform1 = Node::create();
    platform1->setPosition(Vec2(origin.x + visibleSize.width * 0.25f, origin.y + platformHeight));
    auto platform1Body = PhysicsBody::createBox(Size(150, 20), PhysicsMaterial(1.0f, 0.5f, 0.3f));
    platform1Body->setDynamic(false);
    platform1->setPhysicsBody(platform1Body);
    this->addChild(platform1);
    
    auto platform1Sprite = DrawNode::create();
    platform1Sprite->drawSolidRect(Vec2(-75, -10), Vec2(75, 10), Color4F(0.3f, 0.6f, 0.3f, 1.0f));
    platform1->addChild(platform1Sprite);
    
    auto platform2 = Node::create();
    platform2->setPosition(Vec2(origin.x + visibleSize.width * 0.75f, origin.y + platformHeight));
    auto platform2Body = PhysicsBody::createBox(Size(150, 20), PhysicsMaterial(1.0f, 0.5f, 0.3f));
    platform2Body->setDynamic(false);
    platform2->setPhysicsBody(platform2Body);
    this->addChild(platform2);
    
    auto platform2Sprite = DrawNode::create();
    platform2Sprite->drawSolidRect(Vec2(-75, -10), Vec2(75, 10), Color4F(0.3f, 0.6f, 0.3f, 1.0f));
    platform2->addChild(platform2Sprite);
    
    auto platform3 = Node::create();
    platform3->setPosition(Vec2(origin.x + visibleSize.width * 0.5f, origin.y + platformHeight * 2));
    auto platform3Body = PhysicsBody::createBox(Size(200, 20), PhysicsMaterial(1.0f, 0.5f, 0.3f));
    platform3Body->setDynamic(false);
    platform3->setPhysicsBody(platform3Body);
    this->addChild(platform3);
    
    auto platform3Sprite = DrawNode::create();
    platform3Sprite->drawSolidRect(Vec2(-100, -10), Vec2(100, 10), Color4F(0.3f, 0.6f, 0.3f, 1.0f));
    platform3->addChild(platform3Sprite);
}

void PhysicsScene::spawnCircle(const Vec2& position)
{
    auto radius = 20.0f + rand() % 20;
    
    auto circle = DrawNode::create();
    circle->drawSolidCircle(Vec2::ZERO, radius, 0, 36, 1.0f, 1.0f, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1.0f));
    
    auto body = PhysicsBody::createCircle(radius, PhysicsMaterial(0.8f, 0.6f, 0.5f));
    body->setLinearDamping(0.3f);
    body->setAngularDamping(0.5f);
    body->setVelocity(Vec2((rand() % 200 - 100), rand() % 100));
    body->setAngularVelocity(rand() % 10 - 5);
    
    circle->setPhysicsBody(body);
    circle->setPosition(position);
    this->addChild(circle, 2);
    _physicsObjects.push_back(circle);
    
    updateInfo();
}

void PhysicsScene::spawnBox(const Vec2& position)
{
    auto width = 30.0f + rand() % 30;
    auto height = 30.0f + rand() % 30;
    
    auto box = DrawNode::create();
    box->drawSolidRect(Vec2(-width / 2, -height / 2), Vec2(width / 2, height / 2), Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1.0f));
    
    auto body = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0.7f, 0.5f, 0.4f));
    body->setLinearDamping(0.3f);
    body->setAngularDamping(0.5f);
    body->setVelocity(Vec2((rand() % 200 - 100), rand() % 100));
    body->setAngularVelocity(rand() % 10 - 5);
    
    box->setPhysicsBody(body);
    box->setPosition(position);
    this->addChild(box, 2);
    _physicsObjects.push_back(box);
    
    updateInfo();
}

void PhysicsScene::spawnTriangle(const Vec2& position)
{
    auto size = 40.0f + rand() % 30;
    
    Vec2 points[3];
    points[0] = Vec2(0, size / 2);
    points[1] = Vec2(-size / 2, -size / 2);
    points[2] = Vec2(size / 2, -size / 2);
    
    auto triangle = DrawNode::create();
    triangle->drawSolidPoly(points, 3, Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1.0f));
    
    auto body = PhysicsBody::createPolygon(points, 3, PhysicsMaterial(0.6f, 0.4f, 0.3f));
    body->setLinearDamping(0.3f);
    body->setAngularDamping(0.5f);
    body->setVelocity(Vec2((rand() % 200 - 100), rand() % 100));
    body->setAngularVelocity(rand() % 10 - 5);
    
    triangle->setPhysicsBody(body);
    triangle->setPosition(position);
    this->addChild(triangle, 2);
    _physicsObjects.push_back(triangle);
    
    updateInfo();
}

void PhysicsScene::spawnRandomObjects()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    for (int i = 0; i < 2; i++)
    {
        int type = rand() % 3;
        Vec2 pos(origin.x + visibleSize.width * 0.2f + rand() % (int)(visibleSize.width * 0.6f),
                 origin.y + visibleSize.height * 0.6f + rand() % (int)(visibleSize.height * 0.3f));
        
        if (type == 0)
            spawnCircle(pos);
        else if (type == 1)
            spawnBox(pos);
        else
            spawnTriangle(pos);
    }
}

void PhysicsScene::applyForceToAll(float forceX, float forceY)
{
    for (auto obj : _physicsObjects)
    {
        if (obj->getPhysicsBody())
        {
            obj->getPhysicsBody()->applyImpulse(Vec2(forceX, forceY));
        }
    }
}

void PhysicsScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    for (auto touch : touches)
    {
        Vec2 location = touch->getLocation();
        spawnCircle(location);
    }
}

void PhysicsScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    
    if (keyCode == EventKeyboard::KeyCode::KEY_B)
    {
        spawnBox(center);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_T)
    {
        spawnTriangle(center);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
    {
        applyForceToAll(0, 5000);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
    {
        applyForceToAll(-3000, 0);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
    {
        applyForceToAll(3000, 0);
    }
}

void PhysicsScene::updateInfo()
{
    _infoLabel->setString(StringUtils::format("Objects: %zu | Gravity: -980", _physicsObjects.size()));
}

void PhysicsScene::update(float dt)
{
    _drawNode->clear();
}

void PhysicsScene::goToMainMenu(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, TestScene::scene()));
}
