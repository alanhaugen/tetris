#include <core/application.h>
#include <core/x-platform/scene.h>

int score;

class MainMenu : public IScene
{
public:
    MainMenu();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

MainMenu::MainMenu()
{
}

void MainMenu::Init()
{
}

void MainMenu::Update()
{
}

void MainMenu::UpdateAfterPhysics()
{
}

class Tetris : public IScene
{
private:
    MainMenu *mainMenuScene;
    bool paused;

public:
    Tetris();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

Tetris::Tetris()
{
}

void Tetris::Init()
{
    score = 0;
    paused = false;

    mainMenuScene = new MainMenu();
}

void Tetris::Update()
{
}

void Tetris::UpdateAfterPhysics()
{
}

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new Tetris());

    return application.Exec();
}

