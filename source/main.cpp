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
    components.Add(new Camera());
    components.Add(new Text("GAME OVER", 200, 300));
    components.Add(new Text("HIGHSCORE: " + String(highscore), 200, 400));
    components.Add(new Text("PRESS SPACE TO CONTINUE", 200, 500));
}

void GameOver::Update()
{
    if (input.Pressed(input.Key.SPACE))
    {
        Application::LoadScene(0);
    }
}

void GameOver::UpdateAfterPhysics()
{
}

// In tetris the shapes consist of four cubes
enum TETREMINOS
{
    I,
    O,
    T,
    S,
    Z,
    J,
    L
};

const int NUMBER_OF_TETREMINOS = 7;

class Block : public Actor
{
private:

public:
    Block(int type = I);
};

Block::Block(int type)
{
    if (type == I)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(0,2*2,0));
        components.Add(new Cube(0,3*2,0));
    }
    else if (type == O)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(1*2,1*2,0));
        components.Add(new Cube(1*2,0,0));
    }
    else if (type == T)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(1*2,0,0));
        components.Add(new Cube(-1*2,0,0));
        components.Add(new Cube(0,1*2,0));
    }
    else if (type == S)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(1*2,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(-1*2,1*2,0));
    }
    else if (type == Z)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(-1*2,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(1*2,1*2,0));
    }
    else if (type == J)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(0,2*2,0));
        components.Add(new Cube(-1*2,2*2,0));
    }
    else if (type == L)
    {
        components.Add(new Cube(0,0,0));
        components.Add(new Cube(0,1*2,0));
        components.Add(new Cube(0,2*2,0));
        components.Add(new Cube(1*2,2*2,0));
    }

    matrix.Translate(glm::vec3(0, 10, -25));
}

const float TICK_TIME = 500.0f;

class Tetris : public IScene
{
private:
    int score;
    int speed;
    bool paused;
    ITime *timer;
    Block *activePiece;

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
    speed = 1;
    paused = false;
    timer = Application::GetTime();

    activePiece = new Block();

    components.Add(new Camera());
    components.Add(activePiece);
}

void Tetris::Update()
{

    if (timer->TimeSinceStarted() > TICK_TIME / speed || input.Pressed(input.Key.DOWN))
    {
        timer->Reset();
        activePiece->matrix.Translate(glm::vec3(0.0f, -2.0f, 0.0f));
    }

    if (input.Pressed(input.Key.LEFT))
    {
        activePiece->matrix.Translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    }
    if (input.Pressed(input.Key.RIGHT))
    {
        activePiece->matrix.Translate(glm::vec3(2.0f, 0.0f, 0.0f));
    }

    if (activePiece->matrix.position.y < -10.0f)
    {
        activePiece = new Block(random.RandomRange(0, NUMBER_OF_TETREMINOS));
        components.Add(activePiece);
        //Application::NextScene();
    }
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

