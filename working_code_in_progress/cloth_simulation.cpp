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

// used for printing
#include <string>

// macros
#define NUMBER_NODES_WIDTH 21
#define NUMBER_NODES_HEIGHT 23

// Global variables
bool drawWireFrameEnabled = false;
bool drawNodesEnabled = true;
bool drawStructuralConstraintsEnabled = false;
bool drawShearConstraintsEnabled = false;
bool drawStructuralBendConstraintsEnabled = false;
bool drawShearBendConstraintsEnabled = false;

float pitchAngle = 0.0;
float yawAngle = 0.0;
float rollAngle = 0.0;

// increments in degrees
float pitchAngleIncrement = 5.0;
float yawAngleIncrement = 5.0;
float rollAngleIncrement = 5.0;

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
    // push matrix so we can come back to the "origin" (on element (0.0, 0.0, 0.0))
    // for each node to draw.
    glPushMatrix();
        glTranslatef(position.x, position.y, position.z);

        // we always want the nodes to be drawn as solid spheres, even if
        // drawWireFrameEnabled is true.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glutSolidSphere(0.15, 20, 20);
    glPopMatrix();
    // back at "origin" (on element (0.0, 0.0, 0.0)) again.

    // put back wireframe if necessary, so other objects will be rendered correctly.
    if(drawWireFrameEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
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
    Node nodes[NUMBER_NODES_WIDTH][NUMBER_NODES_HEIGHT];
    std::vector<Constraint> structuralConstraints;
    std::vector<Constraint> shearConstraints;
    std::vector<Constraint> structuralBendConstraints;
    std::vector<Constraint> shearBendConstraints;

    // node creation method
    void addNodes();

    // -------------------------------------------------------------------------
    // constraint creation methods
    void createConstraints();
    void createStructuralConstraints();
    void createShearConstraints();
    void createBendConstraints();
    void createStructuralBendConstraints();
    void createShearBendConstraints();

    // -------------------------------------------------------------------------
    // drawing methods
    void drawNodes();
    void drawStructuralConstraints();
    void drawShearConstraints();
    void drawStructuralBendConstraints();
    void drawShearBendConstraints();

public:
    Cloth();
    void draw();
};

Cloth::Cloth()
{
    addNodes();
    createConstraints();
}

void Cloth::addNodes()
{
    for(int x = 0; x < NUMBER_NODES_WIDTH; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT; y += 1)
        {
            // put elements in rectangular grid with 0.0 depth
            nodes[x][y] = Vector3(x, y, 0.0);
        }
    }
}

void Cloth::createConstraints()
{
    createStructuralConstraints();
    createShearConstraints();
    createBendConstraints();
}

void Cloth::createStructuralConstraints()
{
    for(int x = 0; x < NUMBER_NODES_WIDTH; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT; y += 1)
        {
            if(x < NUMBER_NODES_WIDTH - 1)
            {
                Node* leftNode = &nodes[x][y];
                Node* rightNode = &nodes[x + 1][y];
                Constraint rightConstraint(leftNode, rightNode);
                structuralConstraints.push_back(rightConstraint);
            }

            if(y < NUMBER_NODES_HEIGHT - 1)
            {
                Node* bottomNode = &nodes[x][y];
                Node* topNode = &nodes[x][y + 1];
                Constraint topConstraint(bottomNode, topNode);
                structuralConstraints.push_back(topConstraint);
            }
        }
    }
}

void Cloth::createShearConstraints()
{
    for(int x = 0; x < NUMBER_NODES_WIDTH - 1; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT - 1; y += 1)
        {
            Node* lowerLeftNode = &nodes[x][y];
            Node* lowerRightNode = &nodes[x + 1][y];
            Node* upperLeftNode = &nodes[x][y + 1];
            Node* upperRightNode = &nodes[x + 1][y + 1];

            Constraint diagonalConstraint1(lowerLeftNode, upperRightNode);
            Constraint diagonalConstraint2(lowerRightNode, upperLeftNode);

            shearConstraints.push_back(diagonalConstraint1);
            shearConstraints.push_back(diagonalConstraint2);
        }
    }
}

void Cloth::createBendConstraints()
{
    createStructuralBendConstraints();
    createShearBendConstraints();
}

void Cloth::createStructuralBendConstraints()
{
    for(int x = 0; x < NUMBER_NODES_WIDTH; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT; y += 1)
        {
            if(x < NUMBER_NODES_WIDTH - 2)
            {
                Node* leftNode = &nodes[x][y];
                Node* rightNode = &nodes[x + 2][y];
                Constraint rightConstraint(leftNode, rightNode);
                structuralBendConstraints.push_back(rightConstraint);
            }

            if(y < NUMBER_NODES_HEIGHT - 2)
            {
                Node* bottomNode = &nodes[x][y];
                Node* topNode = &nodes[x][y + 2];
                Constraint topConstraint(bottomNode, topNode);
                structuralBendConstraints.push_back(topConstraint);
            }
        }
    }
}

void Cloth::createShearBendConstraints()
{
    for(int x = 0; x < NUMBER_NODES_WIDTH - 2; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT - 2; y += 1)
        {
            Node* lowerLeftNode = &nodes[x][y];
            Node* lowerRightNode = &nodes[x + 2][y];
            Node* upperLeftNode = &nodes[x][y + 2];
            Node* upperRightNode = &nodes[x + 2][y + 2];

            Constraint diagonalConstraint1(lowerLeftNode, upperRightNode);
            Constraint diagonalConstraint2(lowerRightNode, upperLeftNode);

            shearBendConstraints.push_back(diagonalConstraint1);
            shearBendConstraints.push_back(diagonalConstraint2);
        }
    }
}

void Cloth::draw()
{
    if(drawNodesEnabled)
    {
        drawNodes();
    }

    if(drawStructuralConstraintsEnabled)
    {
        drawStructuralConstraints();
    }

    if(drawShearConstraintsEnabled)
    {
        drawShearConstraints();
    }

    if(drawStructuralBendConstraintsEnabled)
    {
        drawStructuralBendConstraints();
    }

    if(drawShearBendConstraintsEnabled)
    {
        drawShearBendConstraints();
    }
}

void Cloth::drawNodes()
{
    // loop over vertices (not edges)
    for(int x = 0; x < NUMBER_NODES_WIDTH; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT; y += 1)
        {
            nodes[x][y].draw();
        }
    }
}

void Cloth::drawStructuralConstraints()
{
    for(
        std::vector<Constraint>::iterator structuralConstraintIterator = structuralConstraints.begin();
        structuralConstraintIterator != structuralConstraints.end();
        ++structuralConstraintIterator
       )
    {
        Node* firstNode = structuralConstraintIterator->getFirstNode();
        Node* secondNode = structuralConstraintIterator->getSecondNode();

        Vector3 firstNodePosition = firstNode->getPosition();
        Vector3 secondNodePosition = secondNode->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

void Cloth::drawShearConstraints()
{
    for(
        std::vector<Constraint>::iterator shearConstraintIterator = shearConstraints.begin();
        shearConstraintIterator != shearConstraints.end();
        ++shearConstraintIterator
       )
    {
        Node* firstNode = shearConstraintIterator->getFirstNode();
        Node* secondNode = shearConstraintIterator->getSecondNode();

        Vector3 firstNodePosition = firstNode->getPosition();
        Vector3 secondNodePosition = secondNode->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

void Cloth::drawStructuralBendConstraints()
{
    for(
        std::vector<Constraint>::iterator structuralBendConstraintIterator = structuralBendConstraints.begin();
        structuralBendConstraintIterator != structuralBendConstraints.end();
        ++structuralBendConstraintIterator
       )
    {
        Node* firstNode = structuralBendConstraintIterator->getFirstNode();
        Node* secondNode = structuralBendConstraintIterator->getSecondNode();

        Vector3 firstNodePosition = firstNode->getPosition();
        Vector3 secondNodePosition = secondNode->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

void Cloth::drawShearBendConstraints()
{
    for(
        std::vector<Constraint>::iterator shearBendConstraintIterator = shearBendConstraints.begin();
        shearBendConstraintIterator != shearBendConstraints.end();
        ++shearBendConstraintIterator
       )
    {
        Node* firstNode = shearBendConstraintIterator->getFirstNode();
        Node* secondNode = shearBendConstraintIterator->getSecondNode();

        Vector3 firstNodePosition = firstNode->getPosition();
        Vector3 secondNodePosition = secondNode->getPosition();

        glBegin(GL_LINES);
            glVertex3f(firstNodePosition.x, firstNodePosition.y, firstNodePosition.z);
            glVertex3f(secondNodePosition.x, secondNodePosition.y, secondNodePosition.z);
        glEnd();
    }
}

// -----------------------------------------------------------------------------
// GLUT setup

// Cloth declaration
Cloth cloth;

// How far the camera should be so that all nodes on the height are visible
float cameraDistanceZ = NUMBER_NODES_HEIGHT;

void showHelp()
{
    // help controls
    std::cout << "help controls:" << std::endl;
    std::cout << "  h: show help" << std::endl;

    std::cout << std::endl;

    // drawing controls
    std::cout << "drawing controls:" << std::endl;
    std::cout << "  x: toggle draw nodes" << std::endl;
    std::cout << "  q: toggle draw structural      constraints" << std::endl;
    std::cout << "  w: toggle draw shear           constraints" << std::endl;
    std::cout << "  e: toggle draw structural bend constraints" << std::endl;
    std::cout << "  r: toggle draw shear bend      constraints" << std::endl;
    std::cout << "  y: toggle draw wireframe" << std::endl;

    std::cout << std::endl;

    // yaw, pitch and roll controls
    std::cout << "yaw, pitch and roll controls:" << std::endl;
    std::cout << "  l: increment yaw   by " << yawAngleIncrement << "°" << std::endl;
    std::cout << "  j: decrement yaw   by " << yawAngleIncrement << "°" << std::endl;
    std::cout << "  i: increment pitch by " << pitchAngleIncrement << "°" << std::endl;
    std::cout << "  k: decrement pitch by " << pitchAngleIncrement << "°" << std::endl;
    std::cout << "  o: increment roll  by " << rollAngleIncrement << "°" << std::endl;
    std::cout << "  u: decrement roll  by " << rollAngleIncrement << "°" << std::endl;

    std::cout << std::endl;

    // camera position controls
    // TODO
    // std::cout << "camera position controls:" << std::endl;

    // std::cout << std::endl;
}

// prints "true" if controlVariableEnabled is true, and "false" otherwise
std::string isEnabled(bool controlVariableEnabled)
{
    if(controlVariableEnabled)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

void showDrawStatus()
{
    std::cout << "draw status:" << std::endl;
    std::cout << "  draw nodes                       = " << isEnabled(drawNodesEnabled) << std::endl;
    std::cout << "  draw structural constraints      = " << isEnabled(drawStructuralConstraintsEnabled) << std::endl;
    std::cout << "  draw shear constraints           = " << isEnabled(drawShearConstraintsEnabled) << std::endl;
    std::cout << "  draw structural bend constraints = " << isEnabled(drawStructuralBendConstraintsEnabled) << std::endl;
    std::cout << "  draw shear bend constraints      = " << isEnabled(drawShearBendConstraintsEnabled) << std::endl;
    std::cout << "  draw wireframe                   = " << isEnabled(drawWireFrameEnabled) << std::endl;

    std::cout << std::endl;
}

void showCameraStatus()
{
    std::cout << "camera status:" << std::endl;
    std::cout << "  yaw:   " << yawAngle << "°" << std::endl;
    std::cout << "  pitch: " << pitchAngle << "°" << std::endl;
    std::cout << "  roll:  " << rollAngle << "°" << std::endl;

    std::cout << std::endl;
}

void init(void)
{
    // show help at the very beginning
    showHelp();
    showDrawStatus();
    showCameraStatus();

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
    float x_offset = -(NUMBER_NODES_WIDTH - 1) / 2.0;
    float y_offset = -(NUMBER_NODES_HEIGHT - 1) / 2.0;
    glTranslatef(x_offset, y_offset, 0.0);

    // draw cloth
    cloth.draw();

    // force redraw
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        // help controls
        case 'h':
            showHelp();
            break;

        // drawing controls
        case 'q':
            drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
            showDrawStatus();
            break;
        case 'w':
            drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
            showDrawStatus();
            break;
        case 'e':
            drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
            showDrawStatus();
            break;
        case 'r':
            drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
            showDrawStatus();
            break;
        case 'x':
            drawNodesEnabled = !drawNodesEnabled;
            showDrawStatus();
            break;
        case 'y':
            drawWireFrameEnabled = !drawWireFrameEnabled;
            showDrawStatus();
            break;

        // yaw, pitch and roll controls
        case 'k':
            // turn camera "down" one notch
            pitchAngle -= pitchAngleIncrement;
            showCameraStatus();
            break;
        case 'i':
            // turn camera "up" one notch
            pitchAngle += pitchAngleIncrement;
            showCameraStatus();
            break;
        case 'j':
            // turn camera "left" one notch
            yawAngle -= yawAngleIncrement;
            showCameraStatus();
            break;
        case 'l':
            // turn camera "right" one notch
            yawAngle += yawAngleIncrement;
            showCameraStatus();
            break;
        case 'u':
            // tilt camera "left" one notch
            rollAngle -= rollAngleIncrement;
            showCameraStatus();
            break;
        case 'o':
            // tilt camera "right" one notch
            rollAngle += rollAngleIncrement;
            showCameraStatus();
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

void keyboardArrows(int key, int x, int y)
{
    // TODO : Move the camera around scene horizontally or vertically
    switch(key)
    {
        case GLUT_KEY_UP:

            break;
        case GLUT_KEY_DOWN:

            break;
        case GLUT_KEY_LEFT:

            break;
        case GLUT_KEY_RIGHT:

            break;
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
    glutSpecialFunc(keyboardArrows);
    glutMainLoop();
    return 0;
}