#define _CRT_SECURE_NO_WARNINGS

#include <bangtal>
#include <bangtal.h>
#include <cstring>
#include <cstdio>
#include <ctime>

using namespace bangtal;

// 배경화면 설정
ScenePtr scene;


// 목표물 구조체
struct Target
{
    int num;
    ObjectPtr tarObj;
};

// 목표물 객체 생성
Target cintamai;
Target puang;


int main(void)
{
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

    // 시작화면 생성
    scene = Scene::create("Treasure Hunt", "images/start.png");
    

    // 게임 시작 소개글
    showMessage("어느 수요일 오후 2시 55분. 당신은 객지프 수업을 듣기 위해 강의실로 가는 길이다. \n\
그때, 중앙대학교 청룡탕에 사는 용이 잃어버린 여의주를 찾아주면 보물을 준다고 한다.\n\
3시까지 강의실에 들어가려면 적어도 1분 안에 보물을 손에 넣어야. . . .“서둘러야 한다.”");

    // 60초 제한을 보여주는 타이머
    auto fullTimer = Timer::create(60);
    showTimer(fullTimer);

    auto game1_hide_timer = Timer::create(1);// 게임1에서 여의주가 안나오는 시간을 재는 타이머
    auto game1_show_timer = Timer::create(1);// 게임1에서 여의주가 나오는 시간을 재는 타이머
    auto game2_hide_timer = Timer::create(1);// 게임2에서 푸앙이가 안나오는 시간을 재는 타이머
    auto game2_show_timer = Timer::create(1);// 게임2에서 푸앙이가 나오는 시간을 재는 타이머

    // 손 이미지 객체 
    auto hand = Object::create("images/hand1.png", scene, 40, 250, false);
    // 시간을 보여주는 박스 이미지 객체 
    auto time_box = Object::create("images/time.png", scene, 500, 670);
    // 여의주 이미지 객체
    auto yb = Object::create("images/yb.png", scene, 565, 470, false);
    // 보물상자 이미지 객체 
    auto ybs = Object::create("images/ybs.png", scene, 480, 200, false);

    // 시작 버튼 객체
    auto startButton = Object::create("images/gs.png", scene, 400, 300);
    startButton->setScale(1.5f);
    // 다음 장면을 이동시키는 버튼 객체
    auto click1 = Object::create("images/click.png", scene, 1010, 620, false);
    auto click2 = Object::create("images/click.png", scene, 1010, 620, false);
    auto click3 = Object::create("images/click.png", scene, 1010, 620, false);
    auto click4 = Object::create("images/click.png", scene, 1010, 620, false);
    // 교실로 돌아가는 버튼 객체 
    auto roomButton = Object::create("images/back.png", scene, 890, 620, false);
    // 종료 버튼 객체
    auto endButton = Object::create("images/end_game.png", scene, 420, 320, false);
    endButton->setScale(1.3f);

    // 게임 오버 이미지 객체
    auto gameover = Object::create("images/gameover.png", scene, 400, 300, false);
    // 재시작 버튼 객체 
    auto restart = Object::create("images/restart.png", scene, 460, 180, false);

    // 여의주 객체 
    cintamai.tarObj = Object::create("images/cintamai.png", scene, 0, 0, false);
    // 푸앙이 객체
    puang.tarObj = Object::create("images/puang.png", scene, 0, 0, false);

    // 60초 제한 시간이 끝나면 종료 이미지가 나오고, 재시작을 할 수 있게 해준다.
    fullTimer->setOnTimerCallback([&](TimerPtr t)->bool {

        scene->setImage("images/fail.png");
        gameover->show();
        restart->show();

        game1_hide_timer->stop();
        game1_show_timer->stop();
        game2_hide_timer->stop();
        game2_show_timer->stop();
        cintamai.tarObj->hide();
        puang.tarObj->hide();
        
        return true;
        });

    // 여의주를 클릭하면 현재 개수를 보여주고, 6개 이상일 시 게임1이 끝난다. 
    cintamai.tarObj->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        cintamai.num += 1;
        showMessage("현재 " + std::to_string(cintamai.num) + " 개의 여의주 조각을 모았다!");

        if (cintamai.num >= 6) {
            fullTimer->stop();
            game1_hide_timer->stop();
            game1_show_timer->stop();
            cintamai.tarObj->hide();

            scene->setImage("images/game1_end.png");
            showMessage("여의주를 다 모았다!!!");
            hand->setScale(0.5f);
            hand->show();
            click1->show();

        };
        return true;
        });

    // 푸앙이를 클릭하면 게임2가 끝난다. 
    puang.tarObj->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        puang.num += 1;
        if (puang.num >= 1) {
            fullTimer->stop();
            game2_hide_timer->stop();
            game2_show_timer->stop();
            puang.tarObj->hide();

            scene->setImage("images/lake.png");
            showMessage("도망치는 푸앙이를 잡아 여의주를 가져왔다!");

            click3->show();

        };
        return true;
        });

    // 클릭 버튼을 누르면 적합한 화면으로 넘어간다. 
    click1->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        scene->setImage("images/run_puang.png");
        showMessage("앗..! 푸앙이가 여의주를 갖고 도망간다! 푸앙이를 잡아라!");
        hand->hide();
        click1->hide();
        click2->show();
        return true;
        });

    click2->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        click2->hide();
        scene->setImage("images/library.png");
        fullTimer->start();
        game2_show_timer->start();
        return true;
        });

    click3->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        click3->hide();
        showMessage("이제 용에게 여의주를 돌려주자!");
        click4->show();
        return true;
        });

    click4->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        click4->hide();
        yb->show();
        ybs->show();
        yb->setScale(0.5f);
        ybs->setScale(0.7f);
        showMessage("시간 안에 모두 성공하여 보물을 얻었다. 이제 기쁜 마음으로 강의실에 들어가자!");
        roomButton->show();
        return true;
        });

    // 방으로 돌아가기 버튼을 누르면 다음 화면으로 넘어간다.
    roomButton->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        scene->setImage("images/room.png");
        yb->hide();
        ybs->hide();
        showMessage("보물을 얻고 3시 전에 강의실에 들어왔다. 이제 게임을 종료하고 수업을 듣자!");
        roomButton->hide();
        endButton->show();
        return true;
        });

    // 게임 종료를 누르면 게임이 끝난다. 
    endButton->setOnMouseCallback([&](auto object, auto x, auto y, auto action)->bool {
        endGame();
        return true;
        });

    // 게임1에서, 2초 이내로 푸앙이가 안나오다가, 나온 후 3초 이내로 사라진다.
    // 나오는 위치는 해당 화면 범위 내 난수로 결정된다.
    game1_hide_timer->setOnTimerCallback([&](TimerPtr t)->bool {

        cintamai.tarObj->hide();
        game1_show_timer->set(rand() % 10 / 10);
        game1_show_timer->start();
        game1_hide_timer->stop();
        return true;
        });

    game1_show_timer->setOnTimerCallback([&](TimerPtr t)->bool {

        game1_hide_timer->set(rand() % 20 / 10);
        game1_hide_timer->start();
        game1_show_timer->stop();
        cintamai.tarObj->show();
        cintamai.tarObj->locate(scene, rand() % (1050 - 20) + 20, rand() % (680 - 20) + 20);

        return true;
        });

    // 게임2에서, 푸앙이는 0.1초 내로 안나오다가, 나온 후 1초 이내로 사라진다.
    // 푸앙이가 나오는 위치는 화면 내에서 난수로 결정된다.
    game2_hide_timer->setOnTimerCallback([&](TimerPtr t)->bool {
        puang.tarObj->hide();
        game2_show_timer->set(rand() % 5 / 10);
        game2_show_timer->start();
        game2_hide_timer->stop();
        return true;
        });

    game2_show_timer->setOnTimerCallback([&](TimerPtr t)->bool {
        game2_hide_timer->set(rand() % 15 / 10);
        game2_hide_timer->start();
        game2_show_timer->stop();
        puang.tarObj->show();
        puang.tarObj->locate(scene, rand() % (1050 - 20) + 20, rand() % (680 - 20) + 20);

        return true;
        });

    // 시작 버튼을 누르면 게임이 시작된다. 
    startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action) -> bool
        {
            startButton->hide();
            scene->setImage("images/game1.png");
            showMessage("청룡탕 용에게 보물을 받으려면 용이 잃어버린 여의주가 필요하다. \n캠퍼스에 흩어져 있는 노란색 여의주 조각 6개를 찾아라!");

            fullTimer->start();
            game1_show_timer->start();
            return true;
        });

    // 재시작 버튼이 누르면 게임이 재시작된다. 
    restart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action) -> bool
        {
            gameover->hide();
            restart->hide();
            scene->setImage("images/game1.png");

            fullTimer->set(60);
            fullTimer->start();
            cintamai.num = 0;
            puang.num = 0;
            game1_show_timer->start();
            return true;
        });

    // 게임 시작
    startGame(scene);

    return 0;
}
