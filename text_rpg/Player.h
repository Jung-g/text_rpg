#pragma once
#include "Character.h"
#include <random>

// 플레이어의 공통 부모 클래스 (포션, 회복률 보유)
class Player : public Character {
protected:
    int potionCount;   // 포션 개수
    double healRate;   // 최대 체력 대비 회복 비율
    int attackPotionCount;  // 공격력 포션 개수
    bool hasCriticalSkill = false; // 치명타 스킬 획득 여부

public:
    Player(int hp, int atk, double healRate);
    virtual ~Player() {}

    void addPotion() { potionCount++; } // 포션 획득
    bool hasPotion() const { return potionCount > 0; }
    int getPotionCount() const { return potionCount; }

    void addAttackPotion() { attackPotionCount++; } // 공격력 포션 획득
    bool hasAttackPotion() const { return attackPotionCount > 0; }
    int getAttackPotionCount() const { return attackPotionCount; }

    // 포션 사용 (해당 턴에는 공격 대신 사용)
    void usePotion();

    // 공격력 포션 사용 (영구적으로 공격력 증가)
    void useAttackPotion();

    void increaseMaxHP(int amount);   // 최대 체력 증가
    void increaseAttack(int amount);  // 공격력 증가

    // 기본 회피 함수: 기본적으로는 회피 안 함
    // incomingDamage는 필요하면 회피 확률 계산에 활용 가능
    virtual bool tryEvade(int incomingDamage) {
        return false;
    }

    // 각 플레이어 타입별로 공격 구현
    virtual int attack() const = 0;

    //  추가된 메서드: 치명타 스킬 관련
    void setCriticalSkill(bool val) { hasCriticalSkill = val; }
    bool getHasCriticalSkill() const { return hasCriticalSkill; }

    // 최종 보스전에서 치명타 데미지를 계산하여 반환
    int getFinalBossDamage() const;
    
};

// 근거리 캐릭터: 체력이 많고, 공격은 항상 명중하지만 데미지는 약간 낮음
class MeleePlayer : public Player {
public:
    MeleePlayer();
    int attack() const override;  // 100% 명중
};

// 원거리 캐릭터: 체력이 적고, 공격은 강하지만 빗나갈 확률이 있음
class RangedPlayer : public Player {
public:
    RangedPlayer();
    int attack() const override;  // 명중 확률 존재
    bool tryEvade(int incomingDamage) override; // 일정 확률로 적의 공격 회피
};
