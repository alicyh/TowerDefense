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

#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

class Tower : public cocos2d::Sprite
{
public:
    static Tower* create(int type);
    virtual bool init(int type);
    
    void shoot(Enemy* target);
    void update(float dt) override;
    
    int getRange() const { return _range; }
    int getDamage() const { return _damage; }
    float getAttackSpeed() const { return _attackSpeed; }
    int getCost() const { return _cost; }
    
    Enemy* findTarget(const std::vector<Enemy*>& enemies);
    
private:
    int _range;
    int _damage;
    float _attackSpeed;
    int _cost;
    float _attackTimer;
    DrawNode* _rangeIndicator;
    
    void initTower(int type);
    void showRange();
};

#endif // __TOWER_H__
