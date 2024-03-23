#pragma once

#include <vector>

#include "components/simple_scene.h"
#define NrSlices 10 


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        typedef struct enemyStruct {
            float translateX, translateY;
            float scaleX, scaleY;
            float speed;
            float row;
            int nrStarHits;
            int stop;
            std::string color;
            std::string name;
            bool available;
        }enemyStruct;

        typedef struct proiectilStruct {
            float starX, starY;
            float scaleX, scaleY;
            bool starAvailable;
            int starStop;
            std::string color;
            std::string name;
        }proiectilStruct;

        typedef struct starStruct {
            float x, y;
        }starStruct;

        typedef struct rhombusStruct {
            float translateX, translateY;
            float scaleX, scaleY;
            float starX, starY;
            float scale_starX, scale_starY;
            std::string color;
            std::string name;
            bool available, starAvailable;
            int starStop;
            float time;
            bool dragging;
            bool isInCell;
            bool toBeDeleted;
        }rhombusStruct;

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void DrawScene(float deltaTimeSeconds);
        void DrawGUI(float deltaTimeSeconds);
        void drawEnemy(float deltaTimeSeconds);
        void Tema1::drawRhomb(float deltaTimeSeconds);
        void rhombAttack(float deltaTimeSeconds);
        bool detectCollision(float radius1, float radius2, proiectilStruct proiectil);
        bool detectCollisionRhombus(float radius1, float radius2, rhombusStruct romb);
        bool isSameLine(rhombusStruct romb);
        enemyStruct CreateEnemy();
        rhombusStruct CreateRhombus(float translateX, float translateY, bool available,
            std::string color);
        proiectilStruct CreateProiectil(float starX, float starY, std::string color);
        starStruct CreateStar(float x, float y);
        bool isCellEmpty(float x, float y);
        void drawRandomStars(float deltaTimeSeconds);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float angularStep;
   
        float squareSide = 100;
        float rectSide = 50;
        float rectHeight = 25;
        float rhombusSide = 50;
        float cxHexagon;
        float cyHexagon;
        int pos;
        int rd, rd2;
        float starRadius, hexagonRadius, rhombusRadius;
        int collisionCount;
        float timeSinceLastEnemySpawn, timeSinceLastProiectilSpawn;
        int nrResurse, health;
        bool releasedOverCell;
        float timeLastStarSpawn;
        int resurseRomb;
        float starSquareSide;

        std::vector<enemyStruct> enemies;
        std::vector<rhombusStruct> romburi;
        std::vector<proiectilStruct> proiectile;
        std::vector<starStruct> stars;
    };
}   // namespace m1
