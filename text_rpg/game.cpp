#include "Game.h"
#include <iostream>
#include <limits>
#include <random>
#include <algorithm> 
#include <vector>
using namespace std;

Game::Game()
    : player(nullptr) {
}

Game::~Game() {
    delete player;
}

// 잘못된 입력 버퍼 비우기용 헬퍼 함수
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 게임 시작: 캐릭터 선택
void Game::start() {
    cout << "===== 간단한 턴제 RPG =====\n";
    cout << "1. 근거리 전사 (HP 높음, 항상 명중, 데미지 중간)" << endl;
    cout << "2. 원거리 궁수 (HP 낮음, 강한 데미지, 빗나갈 수 있음)" << endl;

    int choice = 0;
    while (true) {
        cout << "캐릭터를 선택하세요 (1 또는 2): ";
        if (cin >> choice && (choice == 1 || choice == 2)) {
            break;
        }
        else {
            cout << "잘못된 입력입니다. 다시 입력하세요." << endl;
            clearInput();
        }
    }

    if (choice == 1) {
        player = new MeleePlayer();
        cout << "근거리 전사를 선택했습니다!" << endl;
    }
    else {
        player = new RangedPlayer();
        cout << "원거리 궁수를 선택했습니다!" << endl;
    }

    cout << "\n게임을 시작합니다!\n" << endl;
    stageLoop();
}

// 전체 스테이지 루프 (총 10스테이지)
void Game::stageLoop() {
    const int maxStage = 10;

    for (int stage = 1; stage <= maxStage; ++stage) {
        if (stage == maxStage) {
            cout << "\n========================================" << endl;
            cout << "     최종 보스전에 도달했습니다!" << endl;
            cout << "========================================" << endl;
            cout << "마지막 전투를 준비하세요..." << endl;
            cout << "========================================\n" << endl;
        }
        else {
            cout << "\n===== 스테이지 " << stage << " 시작 =====\n";
        }
        //  황금 고블린 등장 로직 (20% 확률, 한번만)
        if (!goldenGoblinAppeared) {
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, 99);

            if (dist(gen) < 20) { // 20% 확률
                GoldenGoblin goldenGoblin;
                goldenGoblinAppeared = true; // 플래그 설정 (다시 나타나지 않음)
                cout << "\n 황금 고블린이 나타났다! (HP: 1, ATK: 1) " << endl;

                fightGoldenGoblin(goldenGoblin);

                if (player->isDead()) return; // 플레이어가 쓰러지면 게임 종료

                // 황금 고블린을 처치한 경우에만 보상 상자 드로우
                if (goldenGoblin.isDead()) {
                    cout << "\n 황금 고블린을 처치했습니다!" << endl;
                    drawRewardBoxes(); // 보상 상자 로직
                }
            }
        }

        Enemy enemy(stage);
        battle(enemy, stage);

        if (player->isDead()) {
            cout << "\n당신은 쓰러졌습니다... 게임 오버.\n";
            return;
        }

        // 보스전 클리어 시 승리 처리
        if (stage == maxStage && enemy.getIsBoss()) {
            cout << "\n========================================" << endl;
            cout << "     최종 보스를 처치했습니다!" << endl;
            cout << "========================================" << endl;
            cout << "\n축하합니다! 게임을 승리했습니다!!\n" << endl;
            cout << "모든 스테이지를 클리어하신 것을 축하드립니다!" << endl;
            return;
        }

        cout << "\n스테이지 " << stage << " 클리어!\n";

        // 마지막 스테이지를 제외한 스테이지에서 포션 지급
        if (stage < maxStage) {
            cout << "회복 포션을 얻었습니다!" << endl;
            player->addPotion();

            // 추가 랜덤 보상: 최대 체력 증가 / 체력 회복 / 공격력 증가 / 공격력 포션 중 하나
            static random_device rd;
            static mt19937 gen(rd());
            uniform_int_distribution<int> dist(1, 4);

            int reward = dist(gen);

            cout << "추가 보상을 획득합니다... ";

            switch (reward) {
            case 1: { // 최대 체력 증가
                int inc = 5 + stage * 2; // 스테이지가 올라갈수록 더 많이 증가
                player->increaseMaxHP(inc);
                cout << "최대 체력이 " << inc << " 만큼 영구적으로 증가했습니다!" << endl;
                cout << "(현재 최대 HP: " << player->getMaxHP() << ")" << endl;
                break;
            }
            case 2: { // 체력 일부 회복
                int heal = static_cast<int>(player->getMaxHP() * 0.3); // 최대 체력의 30%
                *player + heal;  // 연산자 오버로딩으로 회복
                cout << "추가로 체력이 " << heal << " 만큼 회복되었습니다!" << endl;
                cout << "(현재 HP: " << player->getHP() << "/" << player->getMaxHP() << ")" << endl;
                break;
            }
            case 3: { // 공격력 증가
                int inc = 3 + stage / 2; // 스테이지가 올라갈수록 더 많이 증가
                player->increaseAttack(inc);
                cout << "공격력이 " << inc << " 만큼 영구적으로 증가했습니다!" << endl;
                cout << "(현재 ATK: " << player->getATK() << ")" << endl;
                break;
            }
            case 4: { // 공격력 포션
                player->addAttackPotion();
                cout << "공격력 포션을 획득했습니다!" << endl;
                cout << "(보유 개수: " << player->getAttackPotionCount() << ")" << endl;
                break;
            }
            }
        }
    }

    // 이 코드는 실행되지 않아야 함 (보스전에서 return됨)
    cout << "\n축하합니다! 모든 스테이지를 클리어했습니다!!\n" << endl;
}

// 황금 고블린과의 전투 (변경 없음)

void Game::fightGoldenGoblin(GoldenGoblin& goblin) {
    cout << ">> 황금 고블린은 일반 몬스터와 달리 공격만 가능합니다." << endl;
    cout << ">> 황금 고블린을 처치하면 특별한 보상을 얻을 수 있습니다!" << endl;

    while (!goblin.isDead() && !player->isDead()) {
        player->printStatus("플레이어");
        goblin.printStatus("황금 고블린");

        cout << "\n행동을 선택하세요: 1. 공격 >> ";
        int choice = 0;
        while (!(cin >> choice) || choice != 1) {
            cout << "잘못된 입력입니다. 1을 입력하세요: ";
            clearInput();
        }

        // 1. 플레이어 공격
        int playerDamage = player->getATK();
        goblin - playerDamage;

        if (goblin.isDead()) {
            cout << "\n황금 고블린이 쓰러졌습니다!" << endl;
            break;
        }

        // 2. 황금 고블린 공격 (데미지 1)
        int enemyDamage = goblin.attack();
        // 플레이어 회피 시도 (원거리 궁수만 해당)
        if (player->tryEvade(enemyDamage)) {
            cout << "플레이어가 적의 공격을 회피했습니다!" << endl;
        }
        else {
            *player - enemyDamage;
            cout << "플레이어는 " << enemyDamage << " 만큼 피해를 입었습니다." << endl;
        }


        if (player->isDead()) {
            cout << "\n플레이어가 쓰러졌습니다..." << endl;
            break;
        }
    }
}

// 상자 보상 로직 (변경 없음)

void Game::drawRewardBoxes() {
    cout << "\n 특별 보상: 상자 뽑기! (3개 중 1개 당첨) " << endl;
    cout << "당첨 시 치명타 스킬을 영구적으로 획득합니다!" << endl;

    // 1(당첨), 0(꽝), 0(꽝)을 담은 벡터를 만들어 섞음
    vector<int> boxes = { 1, 0, 0 };
    static random_device rd;
    static mt19937 gen(rd());
    shuffle(boxes.begin(), boxes.end(), gen); // 상자 순서 섞기

    cout << "상자를 선택하세요 (1, 2, 3): ";
    int choice = 0;
    while (!(cin >> choice) || choice < 1 || choice > 3) {
        cout << "잘못된 입력입니다. 1, 2, 3 중 하나를 입력하세요: ";
        clearInput();
    }

    // 인덱스는 0부터 시작하므로 choice - 1
    if (boxes[choice - 1] == 1) {
        cout << "\n========================================================" << endl;
        cout << "       축하합니다! 치명타 스킬을 획득했습니다! " << endl;
        cout << "========================================================" << endl;
        cout << ">> 효과: 최종 보스전(10 스테이지) 진입 시 자동으로 적용됩니다." << endl;
        cout << ">>      공격 시 50% 확률로 10의 고정 추가 데미지를 입힙니다." << endl;
        cout << "========================================================" << endl;
        player->setCriticalSkill(true);
    }
    else {
        cout << "\n========================================================" << endl;
        cout << "       아쉽습니다! 꽝입니다." << endl;
        cout << "========================================================" << endl;
    }
}

// 한 스테이지의 전투 처리
void Game::battle(Enemy& enemy, int stage) {
    while (!player->isDead() && !enemy.isDead()) {
        cout << endl;
        cout << "\n--- 현재 상태 ---\n";
        player->printStatus("플레이어");
        enemy.printStatus("적");

        cout << "\n행동을 선택하세요:\n";
        cout << "1. 공격" << endl;
        cout << "2. 포션 사용 (해당 턴에는 공격 불가)" << endl;
        cout << "3. 공격력 포션 사용 (영구적으로 공격력 증가)" << endl;
        cout << "보유 포션: " << player->getPotionCount() << "개" << endl;
        cout << "보유 공격력 포션: " << player->getAttackPotionCount() << "개" << endl;

        int action = 0;
        while (true) {
            cout << "선택: ";
            if (cin >> action && (action == 1 || action == 2 || action == 3)) {
                break;
            }
            else {
                cout << "잘못된 입력입니다. 다시 입력하세요." << endl;
                clearInput();
            }
        }
        cout << endl;

        // 플레이어 턴
        if (action == 1) {
            int damage = enemy.getIsBoss() ? player->getFinalBossDamage() : player->attack();

            int initialDamage = damage; // 미니게임 보너스 계산을 위해 초기 데미지를 저장
            int totalDamage = damage;   // 총 데미지 추적 변수

            if (damage > 0) {
                // 보스전에서 미니게임 옵션 제공
                if (enemy.getIsBoss()) {
                    cout << "\n[미니게임] 보스에게 추가 데미지를 주기 위해 미니게임을 플레이하시겠습니까?" << endl;
                    cout << "1. 미니게임 플레이 (성공 시 추가 데미지)" << endl;
                    cout << "2. 일반 공격만 하기" << endl;
                    cout << "선택: ";

                    int minigameChoice = 0;
                    while (true) {
                        if (cin >> minigameChoice && (minigameChoice == 1 || minigameChoice == 2)) {
                            break;
                        }
                        else {
                            cout << "잘못된 입력입니다. 1 또는 2를 입력하세요: ";
                            clearInput();
                        }
                    }

                    if (minigameChoice == 1) {
                        if (playMinigame()) {
                            // 미니게임 성공 시 추가 데미지
                            int bonusDamage = damage / 2; // 기본 데미지의 50% 추가
                            enemy - bonusDamage;
                            cout << "미니게임 성공! 추가로 " << bonusDamage << " 데미지를 입혔습니다!" << endl;
                        }
                        else {
                            cout << "미니게임 실패... 추가 데미지를 주지 못했습니다." << endl;
                        }
                    }
                }

                enemy - damage; // 연산자 오버로딩으로 체력 감소
                cout << "적에게 " << damage << " 만큼 피해를 입혔습니다." << endl;
            }
            else {
                cout << "공격이 실패하여 피해를 주지 못했습니다." << endl;
            }
        }
        else if (action == 2) {
            // 포션 사용 선택
            if (player->hasPotion()) {
                player->usePotion();
            }
            else {
                cout << "포션이 없어 공격으로 전환합니다." << endl;
                int damage = player->attack();
                if (damage > 0) {
                    enemy - damage;
                    cout << "적에게 " << damage << " 만큼 피해를 입혔습니다." << endl;
                }
                else {
                    cout << "공격이 실패하여 피해를 주지 못했습니다." << endl;
                }
            }
        }
        else { // action == 3
            // 공격력 포션 사용
            if (player->hasAttackPotion()) {
                player->useAttackPotion();
            }
            else {
                cout << "공격력 포션이 없어 공격으로 전환합니다." << endl;
                int damage = player->attack();
                if (damage > 0) {
                    enemy - damage;
                    cout << "적에게 " << damage << " 만큼 피해를 입혔습니다." << endl;
                }
                else {
                    cout << "공격이 실패하여 피해를 주지 못했습니다." << endl;
                }
            }
        }
        cout << endl;

        // 적이 죽었으면 턴 종료
        if (enemy.isDead()) {
            cout << "적을 처치했습니다!" << endl;
            break;
        }

        // 적의 턴
        int enemyDamage = enemy.attack();
        //*player - enemyDamage; // 연산자 오버로딩으로 플레이어 체력 감소
        //cout << "플레이어는 " << enemyDamage << " 만큼 피해를 입었습니다." << endl;

        //if (player->isDead()) {
        //    cout << "플레이어가 쓰러졌습니다..." << endl;
        //    break;
        //}
        if (player->tryEvade(enemyDamage)) {
            cout << "플레이어는 피해를 입지 않았습니다!" << endl;
        }
        else {
            *player - enemyDamage; // 연산자 오버로딩으로 플레이어 체력 감소
            cout << "플레이어는 " << enemyDamage << " 만큼 피해를 입었습니다." << endl;
        }

        if (player->isDead()) {
            cout << "플레이어가 쓰러졌습니다..." << endl;
            break;
        }
    }
}

// 미니게임: 타이밍 게임 (숫자 맞추기)
bool Game::playMinigame() {
    cout << "\n========================================" << endl;
    cout << "        미니게임: 숫자 맞추기" << endl;
    cout << "========================================" << endl;
    cout << "1부터 10까지의 숫자 중 하나를 맞춰보세요!" << endl;
    cout << "기회는 3번입니다." << endl;
    cout << "========================================\n" << endl;

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 10);
    int answer = dist(gen);

    for (int attempt = 1; attempt <= 3; ++attempt) {
        cout << "[" << attempt << "/3] 숫자를 입력하세요 (1-10): ";
        int guess = 0;

        while (true) {
            if (cin >> guess && guess >= 1 && guess <= 10) {
                break;
            }
            else {
                cout << "1부터 10까지의 숫자를 입력하세요: ";
                clearInput();
            }
        }

        if (guess == answer) {
            cout << "\n정답입니다! 미니게임 성공!" << endl;
            return true;
        }
        else if (guess < answer) {
            cout << "더 큰 숫자입니다!" << endl;
        }
        else {
            cout << "더 작은 숫자입니다!" << endl;
        }
    }

    cout << "\n정답은 " << answer << "이었습니다. 미니게임 실패..." << endl;
    return false;
}
