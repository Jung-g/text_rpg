#include "Player.h"
#include <iostream>
#include <random>
using namespace std;

// 플레이어 공통 생성자
Player::Player(int hp, int atk, double healRate)
    : Character(hp, atk), potionCount(0), healRate(healRate), attackPotionCount(0) {
}

// 포션 사용: 최대 체력의 일정 비율만큼 회복
void Player::usePotion() {
    if (!hasPotion()) {
        cout << "사용 가능한 포션이 없습니다!" << endl;
        return;
    }

    potionCount--;
    int healAmount = static_cast<int>(maxHp * healRate);
    *this + healAmount; // 연산자 오버로딩 (체력 회복)
    cout << "포션을 사용하여 " << healAmount << " 만큼 회복했습니다!" << endl;
}

// 최대 체력 증가 (현재 체력도 같이 올려줌)
void Player::increaseMaxHP(int amount) {
    maxHp += amount;
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

// 공격력 증가
void Player::increaseAttack(int amount) {
    attackPower += amount;
}

// 공격력 포션 사용 (영구적으로 공격력 증가)
void Player::useAttackPotion() {
    if (!hasAttackPotion()) {
        cout << "공격력 포션이 없습니다!" << endl;
        return;
    }

    attackPotionCount--;
    int inc = 5; // 공격력 포션은 5씩 영구적으로 증가
    attackPower += inc;
    cout << "공격력 포션을 사용하여 공격력이 " << inc << " 만큼 영구적으로 증가했습니다!" << endl;
    cout << "(현재 ATK: " << attackPower << ")" << endl;
}

// 보스전 치명타 확률
int Player::getFinalBossDamage() const {
    int baseDamge = attack();
    int totalDamage = baseDamge;

    // 치명타 스킬 획득 및 발동 조건: 50% 확률
    if (totalDamage > 0 && hasCriticalSkill) {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<int> dist(0, 99);

        if (dist(gen) < 50) { // 50% 확률 발동
            const int bonusDmg = 10; // 10의 고정 추가 데미지
            totalDamage += bonusDmg;
            cout << "!!!  치명타 스킬 발동! 고정 추가 데미지 " << bonusDmg << "!!!" << endl;
        }
    }
    return totalDamage;
}

// 근거리 캐릭터: 체력 높고 공격력은 중간, 항상 명중
MeleePlayer::MeleePlayer()
// hp, atk, healRate(30%)
    : Player(120, 23, 0.3) {
}

// 항상 명중하는 공격
int MeleePlayer::attack() const {
    cout << "[근거리 공격] 적을 공격합니다! (" << attackPower << " 데미지)" << endl;
    return attackPower;
}

// 원거리 캐릭터: 체력 낮고 공격력은 높지만 명중률이 낮음
RangedPlayer::RangedPlayer()
// hp, atk, healRate(30%)
    : Player(80, 30, 0.3) {
}

// 원거리 명중률
int RangedPlayer::attack() const {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);

    int roll = dist(gen);
    if (roll <= 75) { // 75% 확률로 명중
        cout << "[원거리 공격] 화살이 적에게 명중했습니다! ("
            << attackPower << " 데미지)" << endl;
        return attackPower;
    }
    else {
        cout << "[원거리 공격] 화살이 빗나갔습니다..." << endl;
        return 0; // 빗나가면 데미지 0
    }
}

// 적 공격 회피
bool RangedPlayer::tryEvade(int incomingDamage) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 100);

    int roll = dist(gen);

    // 15% 확률로 회피
    if (roll <= 15) {
        cout << "[회피] 원거리 캐릭터가 재빨리 적의 공격을 피했습니다!" << endl;
        return true;
    }
    // 회피 실패
    return false;
}