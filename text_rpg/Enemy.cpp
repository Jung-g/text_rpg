#include "Enemy.h"
#include <iostream>
using namespace std;

// 스테이지별로 점점 강해지는 적 생성
Enemy::Enemy(int stage)
    : Character(0, 0), isBoss(false) // 초기값 0으로 설정 후 내부에서 재설정
{
    // 10번째 스테이지는 보스
    if (stage == 10) {
        isBoss = true;
        
        // 보스 스탯
        hp = 285;
        maxHp = 285;
        attackPower = 23;

        cout << "\n========================================" << endl;
        cout << "     최종 보스 등장!" << endl;
        cout << "========================================" << endl;
        cout << "보스 - HP: " << hp << ", ATK: " << attackPower << endl;
        cout << "========================================\n" << endl;
    }
    else {
        // 기본 스탯
        int baseHP = 60;
        int baseATK = 10;

        // 스테이지가 올라갈수록 점진적으로 증가 (템플릿 함수 사용)
        int scaledHP = scaleStat(stage, baseHP, 0.18);   // HP 증가
        int scaledATK = scaleStat(stage, baseATK, 0.10); // ATK 증가

        // 밸런스 조정
        if (scaledHP >= 140) {
            scaledATK -= 3;
        }
        if (scaledATK < 8) scaledATK = 8; // 최소 공격력 보장

        hp = scaledHP;
        maxHp = scaledHP;
        attackPower = scaledATK;

        cout << "적 등장! (스테이지 " << stage << ") HP: " << hp
            << ", ATK: " << attackPower << endl;
    }
}

// 적의 공격: 단순히 공격력 만큼 데미지
int Enemy::attack() const {
    if (isBoss) {
        cout << "[보스의 공격] 보스가 " << attackPower << " 만큼 강력한 피해를 줍니다!" << endl;
    }
    else {
        cout << "[적의 공격] 적이 " << attackPower << " 만큼 피해를 줍니다!" << endl;
    }
    return attackPower;
}
