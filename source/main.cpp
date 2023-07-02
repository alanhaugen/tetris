#include <core/application.h>
#include <core/x-platform/scene.h>
#include <core/components/camera.h>
#include <core/components/text.h>
#include <core/components/cube.h>
#include <glm/gtc/constants.hpp>
#include <sys/time.h>

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
        L,
        RANDOM
    };

public:
    Block(int type = RANDOM);
    bool canRotate;
    glm::vec3 direction;
};

Block::Block(int type)
{
    const int NUMBER_OF_TETROMINOS = 7;

    if (type == RANDOM)
    {
        type = random.RandomRange(0, NUMBER_OF_TETROMINOS);
    }

    canRotate = true;
    tag = "block";

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
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(1.0f, 0.5f, 0.5f, 1.0f)));

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
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(0,2*2,0));
        Add(new Cube(-1*2,2*2,0));
    }
    else if (type == L)
    {
        Uniform("colour", static_cast<glm::vec4>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));

        Add(new Cube(0,0,0));
        Add(new Cube(0,1*2,0));
        Add(new Cube(0,2*2,0));
        Add(new Cube(1*2,2*2,0));
    }

    matrix.Translate(glm::vec3(0, 15, -45));
}

class Tetris : public IScene
{
private:
    int score;
    int speed;
    bool paused;
    bool isRotated;
    ITime *timer;
    Block *activePiece;
    float gameTickTime;

    void CheckScore();

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
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

    random = Random(ms);

    score = 0;
    speed = 1;
    paused = false;
    timer = Application::GetTime();
    gameTickTime = 500.0f;

    activePiece = new Block();

    components.Add(new Camera());
    components.Add(activePiece);

    // Grid around board v
    int i;

    // bottom (10 block)
    for (i = 0; i < 10; i++)
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
        components.Add(new Cube(10,-35 + 10 + i*2,-45));
    }
}

void Tetris::Update()
{
    activePiece->direction = glm::vec3();
    isRotated = false;

    if (input.Pressed(input.Key.LEFT))
    {
        activePiece->direction.x = -2.0f;
    }
    else if (input.Pressed(input.Key.RIGHT))
    {
        activePiece->direction.x = 2.0f;
    }
    else if (input.Pressed(input.Key.UP) && activePiece->canRotate)
    {
        activePiece->matrix.Rotate(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
        isRotated = true;
    }
    else if (timer->TimeSinceStarted() > gameTickTime / speed || input.Pressed(input.Key.DOWN))
    {
        timer->Reset();
        activePiece->direction.x = 0.0f;
        activePiece->direction.y = -2.0f;
    }

    activePiece->matrix.Translate(activePiece->direction);
}

void Tetris::UpdateAfterPhysics()
{
    if (physics->Collide())
    {
        // GameOver
        if (activePiece->matrix.position.y - activePiece->direction.y == 15.0f)
        {
            Application::NextScene();
            return;
        }

        // Rotate back
        if (isRotated)
        {
            activePiece->matrix.Rotate(-3.14159/2, glm::vec3(0.0f, 0.0f, 1.0f));
            activePiece->Update();
        }
        // Make new brick
        else if (activePiece->direction.y < 0.0f)
        {
            activePiece->matrix.Translate(-activePiece->direction);
            activePiece->Update();

            CheckScore();

            activePiece = new Block();
            components.Add(activePiece);
        }
        // Move back out of collision
        else
        {
            activePiece->matrix.Translate(-activePiece->direction);
            activePiece->Update();
        }
    }
}

void Tetris::CheckScore()
{
    const int LINE_LENGTH = 10;
    const int GRID_HEIGHT = 22;
    const float START_Y = -23.0f;
    const float CUBE_HEIGHT = 2.0f;

    int multiplier = 1;

    // Check if cubes are making a solid line in the grid
    for (unsigned int k = 0; k < GRID_HEIGHT; k++)
    {
        bool deleteRow = false;
        int line = 0;

        for (unsigned int i = 0; i < components.Size(); i++)
        {
            Component *component = (*components[i]);

            if (component->tag == "block")
            {
                Block *block = static_cast<Block*>(component);

                for (unsigned int j = 0; j < block->components.Size(); j++)
                {
                    Component *cube = *block->components[j];

                    if (cube->matrix.position.y == START_Y + (k * CUBE_HEIGHT))
                    {
                        if (deleteRow)
                        {
                            block->components.RemoveAt(j);
                            j = -1; // Revert search back to start...
                            score = score + (1 * multiplier); // Update score

                            if (score > highscore)
                            {
                                highscore = score;
                            }
                        }
                        else
                        {
                            line++;
                        }
                    }

                    if (line == LINE_LENGTH)
                    {
                        Log("DELETE ROW: " + String(line));

                        deleteRow = true;
                        line = 0; // To prevent an infinite loop...
                        i = -1; // Rewind and start deleting the row
                    }
                }
            }
        }

        if (deleteRow == true)
        {
            multiplier++;
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

