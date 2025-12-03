#pragma once
#include "Character.h"
#include <iostream>
using namespace std;

// 스테이지에 따라 체력/공격력이 달라지는 적 캐릭터
// 템플릿 함수로 스탯 스케일링 구현
template <typename T>
T scaleStat(int stage, T base, double factor) {
    // stage가 커질수록 base에 factor 비율만큼 증가
    return static_cast<T>(base * (1.0 + factor * (stage - 1)));
}



class Enemy : public Character {
private:
    bool isBoss;  // 보스 여부

public:
    Enemy(int stage);  // 스테이지 번호를 받아 난이도 조절
    int attack() const override;
    bool getIsBoss() const { return isBoss; }
};



class GoldenGoblin : public Enemy {
public:
    // HP 1, ATK 1로 고정 설정
    GoldenGoblin() : Enemy(1) { // Enemy 생성자를 사용하여 기본 멤버 초기화
        hp = 1;
        maxHp = 1;
        attackPower = 1;
    }
    // 황금 고블린의 공격 (데미지 1 고정)
    int attack() const override {
        cout << "[황금 고블린 공격] 당신에게 1의 피해를 입혔습니다!" << endl;
        return attackPower;
    }
};
