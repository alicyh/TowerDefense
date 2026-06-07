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

#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Tower.h"

USING_NS_CC;

class TestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* scene();
    virtual bool init();
    
    void update(float dt) override;
    
    CREATE_FUNC(TestScene);
    
private:
    std::vector<Vec2> _pathPoints;
    std::vector<Enemy*> _enemies;
    std::vector<Tower*> _towers;
    Label* _statusLabel;
    
    void initPath();
    void spawnTestEnemies();
    void placeTestTowers();
    void initUI();
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void goToPhysicsScene(Ref* sender);
    void goToGameScene(Ref* sender);
};

#endif // __TEST_SCENE_H__
