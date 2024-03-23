#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };


    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0, 5), color),
        VertexFormat(corner + glm::vec3(length, 0, 5), color),
        VertexFormat(corner + glm::vec3(length, height, 5), color),
        VertexFormat(corner + glm::vec3(0, height, 5), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };


    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateRhombus(
    const std::string& name,
    glm::vec3 color
    )
{

    // Calculate half-diagonals

    std::vector<VertexFormat> vertices =
    {
        //romb
        VertexFormat(glm::vec3(-1, 0, 20), color),
        VertexFormat(glm::vec3(0, 2, 20), color),
        VertexFormat(glm::vec3(1, 0, 20), color),
        VertexFormat(glm::vec3(0, -2, 20), color),

        //dreptunghi
        VertexFormat(glm::vec3(0, 0.5f, 20), color),
        VertexFormat(glm::vec3(2, 0.5f, 20), color),
        VertexFormat(glm::vec3(2, -0.5f, 20), color),
        VertexFormat(glm::vec3(0, -0.5f, 20), color),
    };

    Mesh* rhombus = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        4, 7, 5,
        7, 6, 5
    };

    // Draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);

    rhombus->InitFromData(vertices, indices);
    return rhombus;
}


Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 color
   )
{
    std::vector<VertexFormat> vertices =
    {
        //pentagon
        VertexFormat(glm::vec3(-2.4f, -0.8f, 30), color),
        VertexFormat(glm::vec3(-1.5f, 2.1f, 30), color),
        VertexFormat(glm::vec3(1.6f, 2.0f, 30), color),
        VertexFormat(glm::vec3(2.3f, -0.9f, 30), color),
        VertexFormat(glm::vec3(-0.1f, -2.6f, 30), color),

        //triangles
        VertexFormat(glm::vec3(-4.5f, 1.5f, 30), color),
        VertexFormat(glm::vec3(0.1f, 4.7f, 30), color),
        VertexFormat(glm::vec3(4.5f, 1.4f, 30), color),
        VertexFormat(glm::vec3(2.8f, -3.9f, 30), color),
        VertexFormat(glm::vec3(-2.8f, -3.8f, 30), color),
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = { 
        0, 2, 1, 
        0, 3, 2, 
        0, 4, 3,
        //triangles
        5, 0, 1,
        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 0, 9
    };
    
    // Draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);
    

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateEnemy(
    const std::string& name,
    glm::vec3 color
)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-2, 0, 1), color),
        VertexFormat(glm::vec3(-1, 1.7f, 1), color),
        VertexFormat(glm::vec3(1, 1.7f, 1), color),
        VertexFormat(glm::vec3(2, 0, 1), color),
        VertexFormat(glm::vec3(1, -1.7f, 1), color),
        VertexFormat(glm::vec3(-1, -1.7f, 1), color),

        VertexFormat(glm::vec3(-1 * 1.2, 0, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
        VertexFormat(glm::vec3(-0.5f * 1.2, 0.9f * 1.2, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
        VertexFormat(glm::vec3(0.5f * 1.2, 0.9f * 1.2, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
        VertexFormat(glm::vec3(1 * 1.2, 0, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
        VertexFormat(glm::vec3(0.5f * 1.2, -0.9f * 1.2, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
        VertexFormat(glm::vec3(-0.5f * 1.2, -0.9f * 1.2, 2), glm::vec3(0.5f, 0.5f, 0.5f)),
    };

    Mesh* hexagon = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 2, 1,
        0, 3, 2,
        0, 5, 3,
        5, 4, 3,
        6, 8, 7,
        6, 9, 8,
        6, 11, 9,
        11, 10, 9
    };


    // Draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);


    hexagon->InitFromData(vertices, indices);
    return hexagon;
}