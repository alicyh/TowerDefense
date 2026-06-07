/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
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

#include "TestScene.h"
#include "AppMacros.h"
#include "PhysicsScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* TestScene::scene()
{
    return TestScene::create();
}

bool TestScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = LayerColor::create(Color4B(30, 30, 40, 255));
    this->addChild(background, -1);
    
    initPath();
    initUI();
    placeTestTowers();
    spawnTestEnemies();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TestScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->scheduleUpdate();
    
    return true;
}

void TestScene::initPath()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    _pathPoints.clear();
    _pathPoints.push_back(Vec2(origin.x - 100, origin.y + visibleSize.height / 2));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.15f, origin.y + visibleSize.height / 2));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.15f, origin.y + visibleSize.height * 0.8f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.45f, origin.y + visibleSize.height * 0.8f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.45f, origin.y + visibleSize.height * 0.2f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.75f, origin.y + visibleSize.height * 0.2f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width * 0.75f, origin.y + visibleSize.height * 0.5f));
    _pathPoints.push_back(Vec2(origin.x + visibleSize.width + 100, origin.y + visibleSize.height * 0.5f));
    
    auto pathDrawNode = DrawNode::create();
    pathDrawNode->drawPolyline(_pathPoints.data(), _pathPoints.size(), false, Color4F(100, 60, 20, 255));
    pathDrawNode->drawPolygon(_pathPoints.data(), _pathPoints.size(), Color4F(120, 80, 40, 200), 1, Color4F(100, 60, 20, 255));
    this->addChild(pathDrawNode, 1);
    
    auto gridDrawNode = DrawNode::create();
    float gridSize = 60.0f;
    for (float x = origin.x; x < origin.x + visibleSize.width; x += gridSize)
    {
        gridDrawNode->drawLine(Vec2(x, origin.y), Vec2(x, origin.y + visibleSize.height), Color4F(50, 50, 50, 100));
    }
    for (float y = origin.y; y < origin.y + visibleSize.height; y += gridSize)
    {
        gridDrawNode->drawLine(Vec2(origin.x, y), Vec2(origin.x + visibleSize.width, y), Color4F(50, 50, 50, 100));
    }
    this->addChild(gridDrawNode, 0);
}

void TestScene::spawnTestEnemies()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    float delay = 0.0f;
    for (int i = 0; i < 5; i++)
    {
        this->scheduleOnce([this](float dt) {
            int type = i % 3;
            auto enemy = Enemy::create(type);
            enemy->moveAlongPath(_pathPoints, enemy->getSpeed());
            this->addChild(enemy, 5);
            _enemies.push_back(enemy);
        }, delay, "spawnEnemy" + std::to_string(i));
        delay += 1.0f;
    }
}

void TestScene::placeTestTowers()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto tower1 = Tower::create(0);
    tower1->setPosition(Vec2(origin.x + 150, origin.y + visibleSize.height * 0.6f));
    this->addChild(tower1, 3);
    _towers.push_back(tower1);
    
    auto tower2 = Tower::create(1);
    tower2->setPosition(Vec2(origin.x + 350, origin.y + visibleSize.height * 0.4f));
    this->addChild(tower2, 3);
    _towers.push_back(tower2);
    
    auto tower3 = Tower::create(2);
    tower3->setPosition(Vec2(origin.x + 550, origin.y + visibleSize.height * 0.35f));
    this->addChild(tower3, 3);
    _towers.push_back(tower3);
}

void TestScene::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    auto titleLabel = Label::createWithTTF("Tower Defense - Main Menu", "fonts/arial.ttf", 32);
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    titleLabel->setColor(Color3B::YELLOW);
    this->addChild(titleLabel, 10);
    
    auto physicsBtn = MenuItemFont::create("Physics World Simulation", CC_CALLBACK_1(TestScene::goToPhysicsScene, this));
    physicsBtn->setFontSizeObj(28);
    physicsBtn->setColor(Color3B::GREEN);
    
    auto gameBtn = MenuItemFont::create("Tower Defense Game", CC_CALLBACK_1(TestScene::goToGameScene, this));
    gameBtn->setFontSizeObj(28);
    gameBtn->setColor(Color3B::ORANGE);
    
    auto menu = Menu::create(physicsBtn, gameBtn, nullptr);
    menu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    this->addChild(menu, 10);
    
    auto hintLabel = Label::createWithTTF("Select a mode to begin!", "fonts/arial.ttf", 20);
    hintLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 80));
    hintLabel->setColor(Color3B::WHITE);
    this->addChild(hintLabel, 10);
}

bool TestScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    auto touch = touches[0];
    Vec2 location = touch->getLocation();
    
    static int towerType = 0;
    auto tower = Tower::create(towerType);
    tower->setPosition(location);
    this->addChild(tower, 3);
    _towers.push_back(tower);
    
    towerType = (towerType + 1) % 3;
    _statusLabel->setString("Tower placed! Next: " + std::string(towerType == 0 ? "Green" : towerType == 1 ? "Orange" : "Purple"));
    
    return true;
}

void TestScene::update(float dt)
{
    for (auto tower : _towers)
    {
        auto target = tower->findTarget(_enemies);
        if (target)
        {
            tower->shoot(target);
        }
    }
    
    for (auto it = _enemies.begin(); it != _enemies.end(); )
    {
        auto enemy = *it;
        
        if (enemy->reachedEnd())
        {
            enemy->removeFromParent();
            it = _enemies.erase(it);
        }
        else if (enemy->isDead())
        {
            enemy->removeFromParent();
            it = _enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
    static float spawnTimer = 0.0f;
    spawnTimer += dt;
    if (spawnTimer > 3.0f)
    {
        spawnTimer = 0.0f;
        int type = rand() % 3;
        auto enemy = Enemy::create(type);
        enemy->moveAlongPath(_pathPoints, enemy->getSpeed());
        this->addChild(enemy, 5);
        _enemies.push_back(enemy);
    }
}

void TestScene::goToPhysicsScene(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, PhysicsScene::scene()));
}

void TestScene::goToGameScene(Ref* sender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameScene::scene()));
}
