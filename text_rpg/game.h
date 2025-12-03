#pragma once

#pragma once
#include "Player.h"
#include "Enemy.h"

// 게임 전체 로직을 관리하는 클래스
class Game {
private:
    Player* player; // 동적으로 할당된 플레이어 객체 포인터
    bool goldenGoblinAppeared = false; // 황금 고블린 등장 여부 (한번만)
public:
    Game();
    ~Game();

    void start();      // 시작 화면 및 캐릭터 선택
    void stageLoop();  // 스테이지 반복 루프 (총 10스테이지)
    void battle(Enemy& enemy, int stage); // 전투 로직 처리
    bool playMinigame(); // 미니게임 플레이 (성공 시 true, 실패 시 false)
    void fightGoldenGoblin(GoldenGoblin& goblin); // 황금 고블린과의 전투
    void drawRewardBoxes(); // 상자 보상 로직
};
