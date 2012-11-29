// compile with the following command:
//     g++ -o cloth_simulation cloth_simulation.cpp -lglut -lGLU -lGL;

// Conventions:
// - When you finish drawing something, always come back to (0.0, 0.0, 0.0)

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// math imports
#include "Vector3.h"

// used for stocking Constraints
#include <vector>

// macros
#define NUMBER_NODES_WIDTH 31
#define NUMBER_NODES_HEIGHT 31

// Global variables
bool drawNodesEnabled = true;
bool drawConstraintsEnabled = true;
bool drawWireFrameEnabled = false;

// -----------------------------------------------------------------------------
// Node class
class Node
{
private:
    Vector3 position;

public:
    Node();
    Node(Vector3 pos);
    Vector3 getPosition();
    void draw();
};

Node::Node() : position(Vector3(0.0, 0.0, 0.0))
{}

Node::Node(Vector3 pos) : position(pos)
{}

Vector3 Node::getPosition()
{
    return position;
}

void Node::draw()
{
    // push matrix so we can come back to the "origin" (center of screen)
    // for each node to draw.
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
    // back at "origin" (center of screen) again.
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constraint class
class Constraint
{
private:
    Node* node1;
    Node* node2;

public:
    Constraint(Node* n1, Node* n2);
    Node* getFirstNode();
    Node* getSecondNode();
};

Constraint::Constraint(Node* n1, Node* n2) : node1(n1), node2(n2)
{}

Node* Constraint::getFirstNode()
{
    return node1;
}

Node* Constraint::getSecondNode()
{
    return node2;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Cloth class
class Cloth
{
private:
    Node nodes[NUMBER_NODES_HEIGHT][NUMBER_NODES_WIDTH];
    std::vector<Constraint> constraints;
    void drawNodes();
    void drawConstraints();
    void addNodes();
    void createStructuralConstraints();
    void createShearConstraints();
    void createStructuralBendConstraints();
    void createShearBendConstraints();
    void createConstraints();
    void createBendConstraints();

public:
    Cloth();
    void draw();
};

void Cloth::createBendConstraints()
{
    createStructuralBendConstraints();
    createShearBendConstraints();
}

void Cloth::createConstraints()
{
    createStructuralConstraints();
    createShearConstraints();
    createBendConstraints();
}

void Cloth::createStructuralBendConstraints()
{
    for(int row = 0; row < NUMBER_NODES_HEIGHT - 2; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH - 2; col += 1)
        {
            Node* centerNode = &nodes[row][col];
            Node* rightNode = &nodes[row][col + 2];
            Node* bottomNode = &nodes[row + 2][col];

            Constraint rightConstraint(centerNode, rightNode);
            Constraint bottomConstraint(centerNode, bottomNode);

            constraints.push_back(rightConstraint);
            constraints.push_back(bottomConstraint);
        }
    }
}

void Cloth::createShearBendConstraints()
{
    for(int row = 0; row < NUMBER_NODES_HEIGHT - 2; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH - 2; col += 1)
        {
            Node* upperLeftNode = &nodes[row][col];
            Node* upperRightNode = &nodes[row][col + 2];
            Node* lowerLeftNode = &nodes[row + 2][col];
            Node* lowerRightNode = &nodes[row + 2][col + 2];

            Constraint downwardsRightConstraint(upperLeftNode, lowerRightNode);
            Constraint upwardsRightConstraint(lowerLeftNode, upperRightNode);

            constraints.push_back(downwardsRightConstraint);
            constraints.push_back(upwardsRightConstraint);
        }
    }
}

void Cloth::createShearConstraints()
{
    for(int row = 0; row < NUMBER_NODES_HEIGHT - 1; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH - 1; col += 1)
        {
            Node* upperLeftNode = &nodes[row][col];
            Node* upperRightNode = &nodes[row][col + 1];
            Node* lowerLeftNode = &nodes[row + 1][col];
            Node* lowerRightNode = &nodes[row + 1][col + 1];

            Constraint downwardsRightConstraint(upperLeftNode, lowerRightNode);
            Constraint upwardsRightConstraint(lowerLeftNode, upperRightNode);

            constraints.push_back(downwardsRightConstraint);
            constraints.push_back(upwardsRightConstraint);
        }
    }
}

void Cloth::createStructuralConstraints()
{
    for(int row = 0; row < NUMBER_NODES_HEIGHT - 1; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH - 1; col += 1)
        {
            Node* centerNode = &nodes[row][col];
            Node* rightNode = &nodes[row][col + 1];
            Node* bottomNode = &nodes[row + 1][col];

            Constraint rightConstraint(centerNode, rightNode);
            Constraint bottomConstraint(centerNode, bottomNode);

            constraints.push_back(rightConstraint);
            constraints.push_back(bottomConstraint);
        }
    }
}

void Cloth::addNodes()
{
    for(int row = 0; row < NUMBER_NODES_HEIGHT; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH; col += 1)
        {
            // put elements in rectangular grid with 0.0 depth
            nodes[row][col] = Vector3(row, col, 0.0);
        }
    }
}

Cloth::Cloth()
{
    addNodes();
    createConstraints();
}

void Cloth::drawNodes()
{
    // loop over vertices (not edges)
    for(int row = 0; row < NUMBER_NODES_HEIGHT; row += 1)
    {
        for(int col = 0; col < NUMBER_NODES_WIDTH; col += 1)
        {
            nodes[row][col].draw();
        }
    }
}

void Cloth::drawConstraints()
{
    for(std::vector<Constraint>::iterator constraintIterator = constraints.begin(); constraintIterator != constraints.end(); ++constraintIterator) {
        Node* firstNode = constraintIterator->getFirstNode();
        Node* secondNode = constraintIterator->getSecondNode();

        Vector3 firstNodePosition = firstNode->getPosition();
        Vector3 secondNodePosition = secondNode->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

// void Cloth::drawTriangles()
// {
//     // loop over edges (not vertices)
//     for(int row = 0; row < NUMBER_NODES_HEIGHT - 1; row += 1)
//     {
//         for(int col = 0; col < NUMBER_NODES_WIDTH - 1; col += 1)
//         {
//             float x1 = (nodes[row][col]).getPosition().x;
//             float y1 = (nodes[row][col]).getPosition().y;
//             float z1 = (nodes[row][col]).getPosition().z;

//             float x2 = (nodes[row + 1][col]).getPosition().x;
//             float y2 = (nodes[row + 1][col]).getPosition().y;
//             float z2 = (nodes[row + 1][col]).getPosition().z;

//             float x3 = (nodes[row + 1][col + 1]).getPosition().x;
//             float y3 = (nodes[row + 1][col + 1]).getPosition().y;
//             float z3 = (nodes[row + 1][col + 1]).getPosition().z;

//             float x4 = (nodes[row][col + 1]).getPosition().x;
//             float y4 = (nodes[row][col + 1]).getPosition().y;
//             float z4 = (nodes[row][col + 1]).getPosition().z;

//             glBegin(GL_TRIANGLE_STRIP);
//                 glVertex3f(x1, y1, z1);
//                 glVertex3f(x2, y2, z2);
//                 glVertex3f(x3, y3, z3);

//                 glVertex3f(x4, y4, z4);
//             glEnd();
//         }
//     }
// }

void Cloth::draw()
{
    if(drawNodesEnabled)
    {
        drawNodes();
    }

    if(drawConstraintsEnabled)
    {
        drawConstraints();
    }
}
// -----------------------------------------------------------------------------
// GLUT setup

// Cloth declaration
Cloth cloth;
// How far the camera should be so that all nodes on the height are visible
float cameraDistanceZ = NUMBER_NODES_HEIGHT;

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw a wireframe or the filled version
    if(drawWireFrameEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // set camera distance big enough for all elements to fit on screen
    gluLookAt(0.0, 0.0, cameraDistanceZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // translate the cloth towards the center of the screen
    // (translate by half the number of edges on each side)
    glTranslatef(-(NUMBER_NODES_WIDTH - 1)/2.0, -(NUMBER_NODES_HEIGHT - 1)/2.0, 0.0);

    // draw cloth
    cloth.draw();

    // force redraw
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'c':
            drawConstraintsEnabled = !drawConstraintsEnabled;
            break;
        case 'n':
            drawNodesEnabled = !drawNodesEnabled;
            break;
        case 'w':
            drawWireFrameEnabled = !drawWireFrameEnabled;
        default:
            break;
    }

    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // set a big clipping plane for now (no display errors)
    gluPerspective(60.0, 1.0, 1.0, 200.0);
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}