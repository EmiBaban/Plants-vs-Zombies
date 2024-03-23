#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    nrResurse = 5;
    timeSinceLastEnemySpawn = 0;
    timeLastStarSpawn = 0;
    collisionCount = 0;
    health = 3;
    releasedOverCell = false;
    resurseRomb = 0;
   
    glm::vec3 corner = glm::vec3(0, 0, 0);

    //centre square
    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize angularStep
    angularStep = 0;

    rd = rand() % 10;

    hexagonRadius = distance(glm::vec3(2, 0, 1), glm::vec3(0, 0, 0)) * 20;
    starRadius = distance(glm::vec3(-4.5f, 1.5f, 1), glm::vec3(0, 0, 0)) * 6;
    rhombusRadius = distance(glm::vec3(0, 0.5f, 1), glm::vec3(0, 0, 0)) * 20;
    starSquareSide = distance(glm::vec3(-0.1f, -2.6f, 1), glm::vec3(1.6f, 2.0f, 1)) * 6;

    //scene + GUI
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(0, 0.5f, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide / 1.5f, glm::vec3(1, 0, 0), true);
    AddMeshToList(square3);

    Mesh* rectangle = object2D::CreateRectangle("rectangle", corner, rectSide, rectHeight, glm::vec3(0, 0, 1), true);
    AddMeshToList(rectangle);

    Mesh* endLine = object2D::CreateRectangle("endLine", corner, 50, 360, glm::vec3(1, 0, 0), true);
    AddMeshToList(endLine);

    //rhombes
    Mesh* orangeRhombus = object2D::CreateRhombus("orangeRhombus", glm::vec3(1, 0.5f, 0));
    AddMeshToList(orangeRhombus);

    Mesh* blueRhombus = object2D::CreateRhombus("blueRhombus", glm::vec3(0, 0, 1));
    AddMeshToList(blueRhombus);

    Mesh* yellowRhombus = object2D::CreateRhombus("yellowRhombus", glm::vec3(1, 1, 0));
    AddMeshToList(yellowRhombus);

    Mesh* purpleRhombus = object2D::CreateRhombus("purpleRhombus", glm::vec3(0.5f, 0, 0.5f));
    AddMeshToList(purpleRhombus);

    //enemies
    Mesh* blueEnemy = object2D::CreateEnemy("blueEnemy", glm::vec3(0, 0, 1));
    AddMeshToList(blueEnemy);

    Mesh* orangeEnemy = object2D::CreateEnemy("orangeEnemy", glm::vec3(1, 0.5f, 0));
    AddMeshToList(orangeEnemy);

    Mesh* yellowEnemy = object2D::CreateEnemy("yellowEnemy", glm::vec3(1, 1, 0));
    AddMeshToList(yellowEnemy);

    Mesh* purpleEnemy = object2D::CreateEnemy("purpleEnemy", glm::vec3(0.5f, 0, 0.5f));
    AddMeshToList(purpleEnemy);

    //stars
    Mesh* blueStar = object2D::CreateStar("blueStar", glm::vec3(0, 0, 1));
    AddMeshToList(blueStar);

    Mesh* orangeStar = object2D::CreateStar("orangeStar", glm::vec3(1, 0.5f, 0));
    AddMeshToList(orangeStar);

    Mesh* yellowStar = object2D::CreateStar("yellowStar", glm::vec3(1, 1, 0));
    AddMeshToList(yellowStar);

    Mesh* purpleStar = object2D::CreateStar("purpleStar", glm::vec3(0.5f, 0, 0.5f));
    AddMeshToList(purpleStar);

    Mesh* star = object2D::CreateStar("star", glm::vec3(1, 0, 1));
    AddMeshToList(star);

    Mesh* star1 = object2D::CreateStar("star1", glm::vec3(0.5f, 0.5f, 0.5f));
    AddMeshToList(star1);

}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    DrawScene(deltaTimeSeconds);
    DrawGUI(deltaTimeSeconds);

    angularStep += 3 * deltaTimeSeconds;

    drawRhomb(deltaTimeSeconds);
    rhombAttack(deltaTimeSeconds);
    drawEnemy(deltaTimeSeconds);

    if (health == 0) {
        cout << "GAME OVER" << endl;
        window->Close();
    }

    for (auto& enemy : enemies) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(enemy.translateX, enemy.translateY);
        modelMatrix *= transform2D::Rotate(0.3);
        modelMatrix *= transform2D::Scale(enemy.scaleX, enemy.scaleX);
        modelMatrix *= transform2D::Scale(20, 20);
        RenderMesh2D(meshes[enemy.name], shaders["VertexColor"], modelMatrix);
    }

    for (auto& star : stars) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(star.x, star.y);
        modelMatrix *= transform2D::Scale(6, 6);
        RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    }

    for (auto& romb : romburi) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(romb.translateX, romb.translateY);
        modelMatrix *= transform2D::Scale(romb.scaleX, romb.scaleY);
        modelMatrix *= transform2D::Scale(20, 20);
        RenderMesh2D(meshes[romb.name], shaders["VertexColor"], modelMatrix);
    }

    for (auto& proiectil : proiectile) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(proiectil.starX, proiectil.starY);
        modelMatrix *= transform2D::Rotate(angularStep);
        modelMatrix *= transform2D::Scale(6, 6);
        RenderMesh2D(meshes[proiectil.name], shaders["VertexColor"], modelMatrix);
    }
}

Tema1::enemyStruct Tema1::CreateEnemy() {
    glm::ivec2 resolution = window->GetResolution();
    enemyStruct enemy;

    pos = 20;
    rd = rand() % 3;

    if (rd == 1) {
        pos += 130;
    }
    else if (rd == 2) {
        pos += 260;
    }

    rd = rand() % 4;

    switch (rd) {
        case 0:
            enemy.color = "orange";
            break;
        case 1:
            enemy.color = "blue";
            break;
        case 2:
            enemy.color = "yellow";
            break;
        default:
            enemy.color = "purple";
            break;
    }

    enemy.translateX = resolution.x;
    enemy.translateY = cy + pos;
    enemy.scaleX = 1;
    enemy.scaleY = 1;
    enemy.row = pos;
    enemy.row = pos;
    enemy.nrStarHits = 0;
    enemy.stop = 1;
    enemy.available = true;

    if (enemy.color == "blue") {
        enemy.name = "blueEnemy";
    }
    else if (enemy.color == "orange") {
        enemy.name = "orangeEnemy";
    }
    else if (enemy.color == "yellow") {
        enemy.name = "yellowEnemy";
    }
    else {
        enemy.name = "purpleEnemy";
    }

    return enemy;
}

Tema1::rhombusStruct Tema1::CreateRhombus(float translateX, float translateY,
    bool available, std::string color) {
    rhombusStruct rhombus;
    rhombus.translateX = translateX;
    rhombus.translateY = translateY;
    rhombus.scaleX = 1;
    rhombus.scaleY = 1;
    rhombus.starX = 0;
    rhombus.starY = 0;
    rhombus.scale_starX = 0;
    rhombus.scale_starY = 0;
    rhombus.available = available;
    rhombus.starAvailable = true;
    rhombus.starStop = 1;
    rhombus.time = 0;
    rhombus.dragging = false;
    rhombus.color = color;
    rhombus.isInCell = false;
    rhombus.toBeDeleted = false;

    if (color == "blue") {
        rhombus.name = "blueRhombus";
    }
    else if (color == "orange") {
        rhombus.name = "orangeRhombus";
    }
    else if (color == "yellow") {
        rhombus.name = "yellowRhombus";
    }
    else {
        rhombus.name = "purpleRhombus";
    }

    return rhombus;
}

Tema1::proiectilStruct Tema1::CreateProiectil(float starX, float starY, std::string color) {
    proiectilStruct proiectil;
    proiectil.starX = starX;
    proiectil.starY = starY;
    proiectil.scaleX = 1;
    proiectil.scaleY = 1;
    proiectil.starAvailable = true;
    proiectil.color = color;

    if (color == "blue") {
        proiectil.name = "blueStar";
    }
    else if (color == "orange") {
        proiectil.name = "orangeStar";
    }
    else if (color == "yellow") {
        proiectil.name = "yellowStar";
    }
    else {
        proiectil.name = "purpleStar";
    }

    return proiectil;
}

Tema1::starStruct Tema1::CreateStar(float x, float y) {
    starStruct star;

    star.x = x;
    star.y = y;

    return star;
}

void Tema1::DrawScene(float deltaTimeSeconds) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 20);
    RenderMesh2D(meshes["endLine"], shaders["VertexColor"], modelMatrix);

    //celule
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float x = i * 130.0f;
            float y = j * 130.0f;
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(x, y);
            modelMatrix *= transform2D::Translate(100, 20);
            RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::drawRandomStars(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    
    timeLastStarSpawn += deltaTimeSeconds;

    if (timeLastStarSpawn >= 10) {
        for (int i = 0; i < 3; i++) {
            float x = rand() % resolution.x;
            float y = rand() % resolution.y;

            stars.push_back(CreateStar(x, y));
            timeLastStarSpawn = 0.0f;
        }
    }
}

void Tema1::drawEnemy(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    timeSinceLastEnemySpawn += deltaTimeSeconds;

    if (timeSinceLastEnemySpawn >= rd2) {
        enemies.push_back(CreateEnemy());
        timeSinceLastEnemySpawn = 0.0f;
        rd2 = 3 + (rand() % 7);
    }

    for (auto it = enemies.begin(); it != enemies.end();) {
        auto& enemy = *it;

        enemy.translateX -= enemy.stop * deltaTimeSeconds * 60;

        //ajunge la endLine
        if (enemy.translateX < 20 + rectSide / 2) {
            enemy.scaleX -= deltaTimeSeconds;
            enemy.scaleY -= deltaTimeSeconds;
            enemy.stop = 0;
            it = enemies.erase(it);
            health--;
        } 
        else if (enemy.nrStarHits == 3) {
            enemy.available = false;
            enemy.scaleX -= deltaTimeSeconds;
            enemy.scaleY -= deltaTimeSeconds;
            enemy.stop = 0;
        }
        if (enemy.scaleX < 0 && !enemy.available) {
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Tema1::DrawGUI(float deltaTimeSeconds) {
    for (int i = 0; i < 4; i++) {
        float x = i * 180.0;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(20, 0);
        modelMatrix *= transform2D::Translate(x, 600);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

        if (i == 0) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(40, -25);
            modelMatrix *= transform2D::Translate(x, 600);
            modelMatrix *= transform2D::Scale(3, 3);
            RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(cx, cy);
            modelMatrix *= transform2D::Translate(20, 0);
            modelMatrix *= transform2D::Translate(x, 600);
            modelMatrix *= transform2D::Scale(20, 20);
            RenderMesh2D(meshes["orangeRhombus"], shaders["VertexColor"], modelMatrix);
        }

        if (i == 1 || i == 2) {
            for (int j = 0; j < 2; j++) {
                float y = j * 30;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(y, 0);
                modelMatrix *= transform2D::Translate(40, -25);
                modelMatrix *= transform2D::Translate(x, 600);
                modelMatrix *= transform2D::Scale(3, 3);
                RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

                if (i == 1) {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(cx, cy);
                    modelMatrix *= transform2D::Translate(20, 0);
                    modelMatrix *= transform2D::Translate(x, 600);
                    modelMatrix *= transform2D::Scale(20, 20);
                    RenderMesh2D(meshes["blueRhombus"], shaders["VertexColor"], modelMatrix);
                }

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Translate(20, 0);
                modelMatrix *= transform2D::Translate(x, 600);
                modelMatrix *= transform2D::Scale(20, 20);
                RenderMesh2D(meshes["yellowRhombus"], shaders["VertexColor"], modelMatrix);
            }
        }

        if (i == 3) {
            for (int j = 0; j < 3; j++) {
                float y = j * 30;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(y, 0);
                modelMatrix *= transform2D::Translate(40, -25);
                modelMatrix *= transform2D::Translate(x, 600);
                modelMatrix *= transform2D::Scale(3, 3);
                RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(cx, cy);
                modelMatrix *= transform2D::Translate(20, 0);
                modelMatrix *= transform2D::Translate(x, 600);
                modelMatrix *= transform2D::Scale(20, 20);
                RenderMesh2D(meshes["purpleRhombus"], shaders["VertexColor"], modelMatrix);
            }
        }
    }

    //vieti
    for (int i = 4; i < 4 + health; i++) {
        float x = i * 120;
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(300, 25);
        modelMatrix *= transform2D::Translate(x, 600);
        RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);     
    }

    int a = 0;
    for (int i = 0; i < nrResurse; i++) {
        float y = (i % 15) * 30;
        modelMatrix = glm::mat3(1);

        if (i % 15 == 0) {
            a -= 30;
        }
        modelMatrix *= transform2D::Translate(y, a);
        modelMatrix *= transform2D::Translate(50, 20);
        modelMatrix *= transform2D::Translate(120 * 6, 600);
        modelMatrix *= transform2D::Scale(3, 3);
        RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::drawRhomb(float deltaTimeSeconds) {
    for (auto it = romburi.begin(); it != romburi.end();) {
        auto& romb = *it;

        if ((detectCollisionRhombus(rhombusRadius, hexagonRadius, romb) && romb.isInCell) || romb.toBeDeleted) {
            romb.available = false;
        }

        if (isSameLine(romb) && romb.available) {
            // creeaza proiectile
            romb.time += deltaTimeSeconds;
            if (romb.time >= 2) {
                proiectile.push_back(CreateProiectil(30 + romb.translateX, romb.translateY, romb.color));
                romb.time = 0.0f;
            }
        }

        if (!romb.available) {
            romb.scaleX -= deltaTimeSeconds;
            romb.scaleY -= deltaTimeSeconds;
            if (romb.scaleX < 0) {
                it = romburi.erase(it);
            }
            else {
                ++it;
            }
        }
        else {
            ++it;
        }
    }
}

void Tema1::rhombAttack(float deltaTimeSeconds) {
    for (auto it = proiectile.begin(); it != proiectile.end();) {
        auto& proiectil = *it;

        if (proiectil.starAvailable) {
            proiectil.starX += deltaTimeSeconds * 300;

            if (detectCollision(hexagonRadius, starRadius, proiectil)) {
                proiectil.starAvailable = false;
            }
        }

        if (!proiectil.starAvailable) {
            it = proiectile.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool Tema1::isSameLine(rhombusStruct romb) {
    for (auto& enemy : enemies) {
        if (romb.translateY == enemy.translateY 
            && romb.color == enemy.color && enemy.available) {
           return true;
        }
    }
    return false;
}

//hexagon - proiectil
bool Tema1::detectCollision(float radius1, float radius2, proiectilStruct proiectil) {
    for (auto& enemy : enemies) {
        if (distance(glm::vec3(proiectil.starX, proiectil.starY, 0),
            glm::vec3(enemy.translateX, enemy.translateY, 0)) < radius1 + radius2
            && proiectil.color == enemy.color && enemy.available) {
            enemy.nrStarHits++;
            return true;
        }
    }
    return false;
}

bool Tema1::detectCollisionRhombus(float radius1, float radius2, rhombusStruct romb) {
    for (auto& enemy : enemies) {
        if (distance(glm::vec3(romb.translateX, romb.translateY, 0),
            glm::vec3(enemy.translateX, enemy.translateY, 0)) < radius1 + radius2
            && enemy.available) {
            return true;
        }
    }

    return false;
}

bool Tema1::isCellEmpty(float x, float y) {
    // Check if the cell is empty
    for (const auto& romb : romburi) {
        if (romb.translateX == x && romb.translateY == y) {
            return false;
        }
    }
    return true;
}

void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    drawRandomStars(deltaTime);
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    for (auto& romb : romburi) {
        float x = romb.translateX;
        float y = romb.translateY;
        if (romb.dragging && window->MouseHold(GLFW_MOUSE_BUTTON_1)) {
            romb.translateX = mouseX;
            romb.translateY = 720 - mouseY;
        }
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1) {
        for (int i = 0; i < 4; i++) {
            float x = i * 180;
            std::string culoare;

            switch (i) {
                case 0:
                    resurseRomb = 1;
                    culoare = "orange";
                    break;
                case 1:
                    resurseRomb = 2;
                    culoare = "blue";
                    break;
                case 2:
                    resurseRomb = 2;
                    culoare = "yellow";
                    break;
                default:
                    resurseRomb = 3;
                    culoare = "purple";
                    break;
            }

            if (mouseX > 20 + x && mouseX < 20 + x + squareSide 
                && 720 - mouseY > 20 + 600 && 720 - mouseY < 20 + 600 + squareSide
                && nrResurse >= resurseRomb) {
                    rhombusStruct romb = CreateRhombus(x + cx + 20, 600 + cy + 20,
                        true, culoare);
                    romb.dragging = true;
                    romburi.push_back(romb);
                    break;
            }
        }

        //detectie si stergere stea
        for (auto it = stars.begin(); it != stars.end(); ++it) {
            auto& star = *it;
            if (mouseX > star.x - starSquareSide && mouseX < star.x + starSquareSide &&
                720 - mouseY > star.y - starSquareSide && 720 - mouseY < star.y + starSquareSide) {
                nrResurse++;
                it = stars.erase(it);
                break;
            }
        }
    }

    if (button == 2) {
        for (auto it = romburi.begin(); it != romburi.end(); ++it) {
            auto& romb = *it;
            if (mouseX > romb.translateX - squareSide / 2 && mouseX < romb.translateX + squareSide / 2 &&
                720 - mouseY > romb.translateY - squareSide / 2 && 720 - mouseY < romb.translateY + squareSide / 2) {
                romb.available = false;
                romb.toBeDeleted = true;
                break;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    auto it = romburi.begin();
    while (it != romburi.end()) {
        auto& romb = *it;
        if (romb.dragging) {
            romb.dragging = false;

            bool releasedOverCell = false;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    float x = i * 130;
                    float y = j * 130;
                    if (mouseX > 100 + x && mouseX < 100 + x + squareSide &&
                        720 - mouseY > 20 + y && 720 - mouseY < 20 + y + squareSide &&
                        isCellEmpty(x + cx + 100, y + cy + 20)) {
                        romb.translateX = x + cx + 100;
                        romb.translateY = y + cy + 20;
                        releasedOverCell = true;
                        romb.isInCell = true;
                        nrResurse -= resurseRomb;
                        break;
                    }
                }
                if (releasedOverCell) {
                    break;
                }
            }

            if (!releasedOverCell) {
                it = romburi.erase(it);
                continue;
            }
        }

        ++it;
    }
}



void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}