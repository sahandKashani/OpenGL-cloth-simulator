// compile with the following command:
//     g++ -o cloth_simulation cloth_simulation.cpp -lglut -lGLU -lGL;

// Conventions:
// - When you finish drawing something, always come back to (0.0, 0.0, 0.0)

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// math iemports
#include "Vector3.h"
#include "Matrix4.h"

// used for stocking Constraints
#include <vector>

// used for printing
#include <string>

// macros
#define NUMBER_NODES_WIDTH 31
#define NUMBER_NODES_HEIGHT 31

// Global variables
bool drawWireFrameEnabled = false;
bool drawNodesEnabled = true;
bool drawStructuralConstraintsEnabled = false;
bool drawShearConstraintsEnabled = false;
bool drawStructuralBendConstraintsEnabled = false;
bool drawShearBendConstraintsEnabled = false;

// camera angles
float pitchAngleInRadians = 0.0;
float yawAngleInRadians = 0.0;
float rollAngleInRadians = 0.0;

// camera angle increments in radians
// increment must be power of 2 for precision reasons
float pitchAngleIncrementInRadians = 0.125;
float yawAngleIncrementInRadians = 0.125;
float rollAngleIncrementInRadians = 0.125;

// camera world angles
float worldAngleHorizontalInRadians = 0.0;
float worldAngleVerticalInRadians = 0.0;

// camera world angle increments in radians
// increment must be power of 2 for precision reasons
float worldAngleHorizontalIncrementInRadians = 0.125;
float worldAngleVerticalIncrementInRadians = 0.125;

// -----------------------------------------------------------------------------
// Camera class
class Camera
{
private:
    Vector3 position;
    Vector3 viewDirection;
    Vector3 upDirection;

public:
    Camera();
    Vector3 getPosition();
    Vector3 getViewDirection();
    Vector3 getUpDirection();
    void setPosition(Vector3 pos);
    void setViewDirection(Vector3 direction);
    void setUpDirection(Vector3 direction);
    void translate(Vector3 direction);
    void rotateObjectX(float angleInRadians);
    void rotateObjectY(float angleInRadians);
    void rotateObjectZ(float angleInRadians);
    void rotateWorldHorizontal(float angleInRadians);
    void rotateWorldVertical(float angleInRadians);
};

// initialize camera with the following properties:
// position = (0.0, 0.0, 0.0)
// viewDirection = (0.0, 0.0, 1.0)
// upDirection = (0.0, 1.0, 0.0)
Camera::Camera() :
    position(Vector3(0.0, 0.0, 0.0)),
    viewDirection(Vector3(0.0, 0.0, 1.0)),
    upDirection(Vector3(0.0, 1.0, 0.0))
{}

Vector3 Camera::getPosition()
{
    return position;
}

Vector3 Camera::getViewDirection()
{
    return viewDirection;
}

Vector3 Camera::getUpDirection()
{
    return upDirection;
}

void Camera::setPosition(Vector3 pos)
{
    position = pos;
}

void Camera::setViewDirection(Vector3 direction)
{
    viewDirection = direction;
}

void Camera::setUpDirection(Vector3 direction)
{
    upDirection = direction;
}

void Camera::translate(Vector3 direction)
{
    position += direction;
    viewDirection = Vector3(direction.x, direction.y, viewDirection.z + direction.z);
}

void Camera::rotateObjectX(float angleInRadians)
{
    Matrix4 rotationMatrix = Matrix4(1.0, 0.0                , 0.0                 , 0.0,
                                     0.0, cos(angleInRadians), -sin(angleInRadians), 0.0,
                                     0.0, sin(angleInRadians),  cos(angleInRadians), 0.0,
                                     0.0, 0.0                , 0.0                 , 1.0);

    // position doesn't change
    upDirection = rotationMatrix * upDirection;
    viewDirection = rotationMatrix * viewDirection;
}

void Camera::rotateObjectY(float angleInRadians)
{
    Matrix4 rotationMatrix = Matrix4(cos(angleInRadians) , 0.0, sin(angleInRadians), 0.0,
                                     0.0                 , 1.0, 0.0                , 0.0,
                                     -sin(angleInRadians), 0.0, cos(angleInRadians), 0.0,
                                     0.0                 , 0.0, 0.0                , 1.0);

    // position doesn't change
    // upDirection doesn't change
    viewDirection = rotationMatrix * viewDirection;
}

void Camera::rotateObjectZ(float angleInRadians)
{
    Matrix4 rotationMatrix = Matrix4(cos(angleInRadians), -sin(angleInRadians), 0.0, 0.0,
                                     sin(angleInRadians), cos(angleInRadians) , 0.0, 0.0,
                                     0.0               , 0.0                  , 1.0, 0.0,
                                     0.0               , 0.0                  , 0.0, 1.0);

    // position doesn't change
    // viewDirection doesn't change
    upDirection = rotationMatrix * upDirection;
}

void Camera::rotateWorldHorizontal(float angleInRadians)
{
    std::cout << "-------" << std::endl;
    std::cout << "before rotateWorldHorizontal pos = " << position.toString() << std::endl;

    Vector3 oldPosition = position;
    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));
    std::cout << "after translation to origin pos = " << position.toString() << std::endl;
    rotateObjectY(angleInRadians);
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));

    std::cout << "after rotateWorldHorizontal pos = " << position.toString() << std::endl;
    std::cout << "-------" << std::endl;
}

void Camera::rotateWorldVertical(float angleInRadians)
{
    Vector3 oldPosition = position;
    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));
    rotateObjectX(angleInRadians);
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

// -----------------------------------------------------------------------------
// Node class
class Node
{
private:
    Vector3 position;
    bool moveable;

public:
    Node();
    Node(Vector3 pos);
    Vector3 getPosition();
    void draw();
    bool isMoveable();
    void setMoveable(bool isMovePossible);
    void translate(Vector3 direction);
};

Node::Node() :
    position(Vector3(0.0, 0.0, 0.0)),
    moveable(true)
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
        glutSolidSphere(0.15, 20, 20);
    glPopMatrix();
    // back at "origin" (on element (0.0, 0.0, 0.0)) again.
}

bool Node::isMoveable()
{
    return moveable;
}

void Node::setMoveable(bool isMovePossible)
{
    moveable = isMovePossible;
}

void Node::translate(Vector3 direction)
{
    position += direction;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constraint class
class Constraint
{
private:
    Node* node1;
    Node* node2;
    float distanceAtRest;

public:
    Constraint(Node* n1, Node* n2);
    Node* getFirstNode();
    Node* getSecondNode();
    float getDistanceAtRest();
    void satisfyConstraint();
};

Constraint::Constraint(Node* n1, Node* n2) :
    node1(n1),
    node2(n2)
{
    Vector3 vectorBetween2Nodes = n1->getPosition() - n2->getPosition();
    distanceAtRest = vectorBetween2Nodes.length();
}

Node* Constraint::getFirstNode()
{
    return node1;
}

Node* Constraint::getSecondNode()
{
    return node2;
}

float Constraint::getDistanceAtRest()
{
    return distanceAtRest;
}

void Constraint::satisfyConstraint()
{
    Vector3 vectorFromNode1ToNode2 = node2->getPosition() - node1->getPosition();
    float currentDistance = vectorFromNode1ToNode2.length();
    float restToCurrentDistanceRatio = distanceAtRest / currentDistance;
    Vector3 correctionVectorFromNode1ToNode2 = vectorFromNode1ToNode2 * (1 - restToCurrentDistanceRatio);

    bool node1Moveable = node1->isMoveable();
    bool node2Moveable = node2->isMoveable();

    if(node1Moveable && node2Moveable)
    {
        // move both nodes towards each other by 0.5 * correctionVectorFromNode1ToNode2
        // positive direction for node1 (correction vector goes from node1 to node 2)
        // therefore, negative direction for node2
        node1->translate(0.5 * correctionVectorFromNode1ToNode2);
        node2->translate(-0.5 * correctionVectorFromNode1ToNode2);
    }
    else if(node1Moveable && !node2Moveable)
    {
        // move node1 towards node2 by +1.0 * correctionVectorFromNode1ToNode2
        // (positive sign, because the correction vector is pointing towards node2)
        node1->translate(correctionVectorFromNode1ToNode2);
    }
    else if(!node1Moveable && node2Moveable)
    {
        // move node2 towards node1 by -1.0 * correctionVectorFromNode1ToNode2
        // (negative sign, because the correction vector is pointing towards node2)
        node2->translate(-correctionVectorFromNode1ToNode2);
    }
    else
    {
        // nothing to do, since none of them can move
    }
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
    void createNodes();

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
    createNodes();
    createConstraints();
}

void Cloth::createNodes()
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
    // we always want the nodes to be drawn as solid spheres, even if
    // drawWireFrameEnabled is true.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // loop over vertices (not edges)
    for(int x = 0; x < NUMBER_NODES_WIDTH; x += 1)
    {
        for(int y = 0; y < NUMBER_NODES_HEIGHT; y += 1)
        {
            nodes[x][y].draw();
        }
    }

    if(drawWireFrameEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

// Declare a camera at origin
Camera camera;

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
    std::cout << "  j: decrement yaw   by " << yawAngleIncrementInRadians << " rad" << std::endl;
    std::cout << "  l: increment yaw   by " << yawAngleIncrementInRadians << " rad" << std::endl;
    std::cout << "  k: decrement pitch by " << pitchAngleIncrementInRadians << " rad" << std::endl;
    std::cout << "  i: increment pitch by " << pitchAngleIncrementInRadians << " rad" << std::endl;
    std::cout << "  u: decrement roll  by " << rollAngleIncrementInRadians << " rad" << std::endl;
    std::cout << "  o: increment roll  by " << rollAngleIncrementInRadians << " rad" << std::endl;

    std::cout << std::endl;

    // camera position controls
    std::cout << "camera position controls:" << std::endl;
    // note, these are unicode characters for representing arrows.
    // - Leftwards  Arrow (\u2190)
    // - Rightwards Arrow (\u2192)
    // - Upwards    Arrow (\u2191)
    // - Downwards  Arrow (\u2193)
    std::cout << "  \u2190: rotate camera horizontally left      around (0.0, 0.0, 0.0) by " << worldAngleHorizontalIncrementInRadians << " rad" << std::endl;
    std::cout << "  \u2192: rotate camera horizontally right     around (0.0, 0.0, 0.0) by " << worldAngleHorizontalIncrementInRadians << " rad" << std::endl;
    std::cout << "  \u2191: rotate camera vertically   upwards   around (0.0, 0.0, 0.0) by " << worldAngleVerticalIncrementInRadians << " rad" << std::endl;
    std::cout << "  \u2193: rotate camera vertically   downwards around (0.0, 0.0, 0.0) by " << worldAngleVerticalIncrementInRadians << " rad" << std::endl;

    std::cout << std::endl;
}

void showDrawStatus()
{
    std::cout << "draw status:" << std::endl;
    std::cout << "  draw nodes                      : " << isEnabled(drawNodesEnabled) << std::endl;
    std::cout << "  draw structural constraints     : " << isEnabled(drawStructuralConstraintsEnabled) << std::endl;
    std::cout << "  draw shear constraints          : " << isEnabled(drawShearConstraintsEnabled) << std::endl;
    std::cout << "  draw structural bend constraints: " << isEnabled(drawStructuralBendConstraintsEnabled) << std::endl;
    std::cout << "  draw shear bend constraints     : " << isEnabled(drawShearBendConstraintsEnabled) << std::endl;
    std::cout << "  draw wireframe                  : " << isEnabled(drawWireFrameEnabled) << std::endl;

    std::cout << std::endl;
}

void showCameraStatus()
{
    std::cout << "camera status:" << std::endl;
    std::cout << "  camera position      : " << camera.getPosition().toString() << std::endl;
    std::cout << "  camera view direction: " << camera.getViewDirection().toString() << std::endl;
    std::cout << "  camera up direction  : " << camera.getUpDirection().toString() << std::endl;
    std::cout << "  yaw                  : " << yawAngleInRadians << " rad" << std::endl;
    std::cout << "  pitch                : " << pitchAngleInRadians << " rad" << std::endl;
    std::cout << "  roll                 : " << rollAngleInRadians << " rad" << std::endl;

    std::cout << std::endl;
}

void init()
{
    float cameraX = (NUMBER_NODES_WIDTH - 1) / 2.0;
    float cameraY = (NUMBER_NODES_HEIGHT - 1) / 2.0;
    // move camera back
    float cameraZ = -NUMBER_NODES_HEIGHT;

    camera.setPosition(Vector3(0.0, 0.0, 0.0));
    camera.translate(Vector3(cameraX, cameraY, cameraZ));

    // show help at the very beginning
    // TODO : enable later
    // showHelp();
    // showDrawStatus();
    showCameraStatus();

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

// determines if a wireframe is to be drawn, or a textured version
void chooseRenderingMethod()
{
    if(drawWireFrameEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void display()
{
    // clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    chooseRenderingMethod();

    Vector3 cameraPosition = camera.getPosition();
    Vector3 cameraViewDirection = camera.getViewDirection();
    Vector3 cameraUpDirection = camera.getUpDirection();

    gluLookAt(cameraPosition.x,
              cameraPosition.y,
              cameraPosition.z,
              cameraViewDirection.x,
              cameraViewDirection.y,
              cameraViewDirection.z,
              cameraUpDirection.x,
              cameraUpDirection.y,
              cameraUpDirection.z);

    // TODO : call rotations in function of state.
    // camera.rotateWorldHorizontal(worldAngleHorizontalInRadians);
    // camera.rotateWorldVertical(worldAngleVerticalInRadians);
    // camera.rotateObjectX(pitchAngleInRadians);
    // camera.rotateObjectY(yawAngleInRadians);
    // camera.rotateObjectZ(rollAngleInRadians);

    // draw cloth
    cloth.draw();

    // swap buffers needed for double buffering
    glutSwapBuffers();
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
            pitchAngleInRadians -= pitchAngleIncrementInRadians;
            showCameraStatus();
            break;
        case 'i':
            // turn camera "up" one notch
            pitchAngleInRadians += pitchAngleIncrementInRadians;
            showCameraStatus();
            break;
        case 'j':
            // turn camera "left" one notch
            yawAngleInRadians -= yawAngleIncrementInRadians;
            showCameraStatus();
            break;
        case 'l':
            // turn camera "right" one notch
            yawAngleInRadians += yawAngleIncrementInRadians;
            showCameraStatus();
            break;
        case 'u':
            // tilt camera "left" one notch
            rollAngleInRadians -= rollAngleIncrementInRadians;
            showCameraStatus();
            break;
        case 'o':
            // tilt camera "right" one notch
            rollAngleInRadians += rollAngleIncrementInRadians;
            showCameraStatus();
            break;

        default:
            break;
    }

    // redraw screen after a change in preferences
    glutPostRedisplay();
}

void keyboardArrows(int key, int x, int y)
{
    // TODO : Move the camera around scene horizontally or vertically
    switch(key)
    {
        case GLUT_KEY_UP:
            worldAngleVerticalInRadians += worldAngleVerticalIncrementInRadians;
            showCameraStatus();
            break;
        case GLUT_KEY_DOWN:
            worldAngleVerticalIncrementInRadians -= worldAngleVerticalIncrementInRadians;
            showCameraStatus();
            break;
        case GLUT_KEY_LEFT:
            worldAngleHorizontalInRadians -= worldAngleHorizontalIncrementInRadians;
            showCameraStatus();
            break;
        case GLUT_KEY_RIGHT:
            worldAngleHorizontalInRadians += worldAngleHorizontalIncrementInRadians;
            showCameraStatus();
            break;
        default:
            break;
    }

    // redraw screen after a change in preferences
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);


    // reset projection matrix
    glLoadIdentity();

    // set window size to new values
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // set a big clipping plane for now (no display errors)
    float aspectRatio = (1.0 * w) / h;
    gluPerspective(60.0, aspectRatio, 1.0, 1000.0);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    // TODO : depth
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(500, 500);

    // INF3 room
    glutInitWindowPosition(1070, 655);

    // normal laptop screen
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("Cloth Simulation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardArrows);

    glutMainLoop();
    return 0;
}