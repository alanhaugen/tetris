#include <core/application.h>
#include <core/x-platform/scene.h>
#include <core/components/camera.h>
#include <core/components/text.h>
#include <core/components/cube.h>

int highscore = 0;
const int NUMBER_OF_TETROMINOS = 7;

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

class Block : public Actor
{
private:
    // In tetris the shapes (tetrominos) consist of four cubes
    enum TETROMINOS
    {
        I,
        O,
        T,
        S,
        Z,
        J,
        L
    };

public:
    Block(int type = I);
    bool canRotate;
    glm::vec3 direction;
    glm::vec3 startPos;
};

Block::Block(int type)
{
    canRotate = true;

    if (type == I)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(0,2*2,0));
        Add(new Cube(0,3*2,0));
    }
    else if (type == O)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(1*2,1*2,0));
        Add(new Cube(1*2,0,0));

        canRotate = false;
    }
    else if (type == T)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(1*2,0,0));
        Add(new Cube(-1*2,0,0));
        Add(new Cube(0,1*2,0));
    }
    else if (type == S)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(1*2,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(-1*2,1*2,0));
    }
    else if (type == Z)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(-1*2,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(1*2,1*2,0));
    }
    else if (type == J)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(.0f, 1.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(0,2*2,0));
        Add(new Cube(-1*2,2*2,0));
    }
    else if (type == L)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(.0f, 1.0f, .0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(0,2*2,0));
        Add(new Cube(1*2,2*2,0));
    }

    matrix.Translate(glm::vec3(0, 15, -45));
    startPos = matrix.position;
}

class Tetris : public IScene
{
private:
    int score;
    int speed;
    bool paused;
    ITime *timer;
    Block *activePiece;
    float gameTickTime;

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
    gameTickTime = 500.0f;

    activePiece = new Block();

    components.Add(new Camera());
    components.Add(activePiece);

    //// Grid around board
    int i;

    // bottom (10 block)
    for (i = 0; i <= 10; i++)
    {
        components.Add(new Cube(i*2  - 10,-35 + 10,-45));
    }

    // side left (22 blocks)
    for (i = 0; i < 22; i++)
    {
        components.Add(new Cube( -12,-35 + 10 + i*2,-45)); 
    }

    // side right (22 blocks)
    for (i = 0; i < 22; i++)
    {
        components.Add(new Cube(10*2-8,-35 + 10 + i*2,-45)); 
    }
}

void Tetris::Update()
{
    activePiece->direction = glm::vec3();

    if (input.Pressed(input.Key.LEFT))
    {
        activePiece->direction.x = -2.0f;
    }

    if (input.Pressed(input.Key.RIGHT))
    {
        activePiece->direction.x = 2.0f;
    }

    if (input.Pressed(input.Key.UP) && activePiece->canRotate)
    {
        activePiece->matrix.Rotate(3.14159/2, glm::vec3(0.0f, 0.0f, 1.0f));
    }

    if (timer->TimeSinceStarted() > gameTickTime / speed || input.Pressed(input.Key.DOWN))
    {
        timer->Reset();
        activePiece->direction.x = 0.0f;
        activePiece->direction.y = -2.0f;
    }

    activePiece->matrix.Translate(activePiece->direction);
}

void Tetris::UpdateAfterPhysics()
{
    if (physics->Collide(NULL, "cube"))
    {
        if (activePiece->matrix.position.y == activePiece->startPos.y)
        {
            Application::NextScene();
        }

        if (activePiece->direction.y < 0.0f)
        {
            activePiece->matrix.Translate(glm::vec3(0.0f, -activePiece->direction.y, 0.0));
            activePiece->Update();

            activePiece = new Block(random.RandomRange(0, NUMBER_OF_TETROMINOS));
            components.Add(activePiece);
        }
        else
        {
            activePiece->matrix.Translate(glm::vec3(-activePiece->direction.x, 0.0f, 0.0));
            activePiece->Update();
        }
    }
}

int main(int argc, char **argv)
{
    Application application(argc, argv);
    application.AddScene(new MainMenu());
    application.AddScene(new Tetris());
    application.AddScene(new GameOver());

    return application.Exec();
}

