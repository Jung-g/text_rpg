#include "Character.h"
#include <iostream>
using namespace std;

// 생성자: 기본 체력 및 공격력을 설정
Character::Character(int hp, int atk)
    : hp(hp), maxHp(hp), attackPower(atk) {
}

// 체력 회복 (최대 체력을 넘지 않도록 제한)
Character& Character::operator+(int heal) {
    hp += heal;
    if (hp > maxHp) hp = maxHp;
    return *this;
}

// 체력 감소 (0 아래로 내려가지 않도록 제한)
Character& Character::operator-(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
    return *this;
}

// 현재 상태 출력
void Character::printStatus(const string& name) const {
    cout << name << " - HP: " << hp << "/" << maxHp
        << "  ATK: " << attackPower << endl;
}
