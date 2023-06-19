#include <core/application.h>
#include <core/x-platform/scene.h>
#include <core/components/camera.h>
#include <core/components/text.h>
#include <core/components/cube.h>

int highscore = 0;

class MainMenu : public IScene
{
private:
    Text *name;
    Text *startGame;
    Text *highScore;
    Text *quit;
    Text *selector;

    bool isStartSelected;

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
    highScore = new Text("HIGHSCORE: " + String(highscore), 0, 0);
    name = new Text("TETRIS", 475, 100);

    selector = new Text(">", 475, 225);
    startGame = new Text("START", 500, 225);
    quit = new Text("QUIT", 500, 275);

    components.Add(new Camera());

    components.Add(name);
    components.Add(startGame);
    components.Add(highScore);
    components.Add(quit);
    components.Add(selector);

    isStartSelected = true;
}

void MainMenu::Update()
{
    if (input.Pressed(input.Key.DOWN))
    {
        isStartSelected = false;
        selector->y = quit->y;
    }
    if (input.Pressed(input.Key.UP))
    {
        isStartSelected = true;
        selector->y = startGame->y;
    }
    if (input.Pressed(input.Key.ENTER))
    {
        if (isStartSelected == true)
        {
            Application::NextScene();
        }
        else if (isStartSelected == false)
        {
            Application::Quit();
        }
    }
}

void MainMenu::UpdateAfterPhysics()
{
}

class GameOver : public IScene
{
public:
    GameOver();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

GameOver::GameOver()
{
}

void GameOver::Init()
{
}

void GameOver::Update()
{
}

void GameOver::UpdateAfterPhysics()
{
}

class Tetris : public IScene
{
private:
    int score;
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

    components.Add(new Camera());
    components.Add(new Cube(0, 0, -5));
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
    application.AddScene(new MainMenu());
    application.AddScene(new Tetris());
    application.AddScene(new GameOver());

    return application.Exec();
}

