#pragma once
#include <string>
#include <iostream>
using namespace std;

// 공통 캐릭터(플레이어/몬스터)의 기본이 되는 추상 클래스
class Character {
protected:
    int hp;         // 현재 체력
    int maxHp;      // 최대 체력
    int attackPower; // 공격력

public:
    Character(int hp, int atk);
    virtual ~Character() {}

    // 체력 회복 연산자 오버로딩 (hp += heal)
    Character& operator+(int heal);

    // 체력 감소 연산자 오버로딩 (hp -= dmg)
    Character& operator-(int dmg);

    // 상태 출력 함수
    void printStatus(const string& name) const;

    bool isDead() const { return hp <= 0; }

    // 공격을 수행하고 데미지를 반환하는 순수 가상 함수 (추상화)
    virtual int attack() const = 0;

    int getHP() const { return hp; }
    int getMaxHP() const { return maxHp; }
    int getATK() const { return attackPower; }
};
