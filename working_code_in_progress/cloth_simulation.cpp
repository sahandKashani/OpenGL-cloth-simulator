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
#include "Matrix4.h"

// used for stocking Constraints
#include <vector>

// used for printing
#include <string>

const int numberNodesWidth = 6;
const int numberNodesHeight = 6;
float nearPlane = 1.0;
float farPlane = 1000.0;

// Global variables
bool drawWireFrameEnabled                 = false;
bool drawNodesEnabled                     = true;
bool drawWorldAxisEnabled                 = true;
bool drawStructuralConstraintsEnabled     = false;
bool drawShearConstraintsEnabled          = false;
bool drawStructuralBendConstraintsEnabled = false;
bool drawShearBendConstraintsEnabled      = false;

// camera angle increments in radians
// increment must be power of 2 for precision reasons
float angleIncrement = 0.015625; // 2^(-6)

// camera translation increments
// increment must be power of 2 for precision reasons
float translationIncrement = 0.03125; // 2^(-5)

// -----------------------------------------------------------------------------
// Camera class
class Camera
{
private:
    Vector3 position;
    Vector3 viewDirection;
    Vector3 upDirection;

    // used for getters only, not for computation
    float yaw;
    float pitch;
    float roll;

    Matrix4 getRotationMatrixAroundArbitraryAxisThroughOrigin(float angleInRadians, Vector3 rotationAxisDirection);

public:
    Camera();
    Vector3 getPosition();
    Vector3 getViewDirection();
    Vector3 getUpDirection();
    float getYaw();
    float getPitch();
    float getRoll();
    void setViewDirection(Vector3 direction);
    void setUpDirection(Vector3 direction);

    // translation
    void translate(Vector3 direction);

    // rotation around world axis
    void rotateAroundXAxisWorld(float angleInRadians);
    void rotateAroundYAxisWorld(float angleInRadians);
    void rotateAroundZAxisWorld(float angleInRadians);

    // rotation around camera axis
    void rotateAroundXAxisObject(float angleInRadians);
    void rotateAroundYAxisObject(float angleInRadians);
    void rotateAroundZAxisObject(float angleInRadians);
};

// initialize camera with the following properties:
// position = (0.0, 0.0, 1.0)
// viewDirection = (0.0, 0.0, 0.0)
// upDirection = (0.0, 1.0, 0.0)
// yaw = 0.0
// pitch = 0.0
// roll = 0.0
Camera::Camera() :
    position(Vector3(0.0, 0.0, 1.0)),
    viewDirection(Vector3(0.0, 0.0, 0.0)),
    upDirection(Vector3(0.0, 1.0, 0.0)),
    yaw(0.0),
    pitch(0.0),
    roll(0.0)
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

float Camera::getYaw()
{
    return yaw;
}

float Camera::getPitch()
{
    return pitch;
}

float Camera::getRoll()
{
    return roll;
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
    viewDirection += direction;
}

Matrix4 Camera::getRotationMatrixAroundArbitraryAxisThroughOrigin(float angleInRadians, Vector3 rotationAxisDirection)
{
    // application of Rodriguez' rotation formula (taken from Wikipedia not from
    // the course, since the course slides were using a left-handed coordinate system,
    // but we want to use a right-handed one.)

    // normalize direction
    Vector3 unitRotationAxisDirection = rotationAxisDirection.normalize();

    float u_x = unitRotationAxisDirection.x;
    float u_y = unitRotationAxisDirection.y;
    float u_z = unitRotationAxisDirection.z;

    float cosa = cos(angleInRadians);
    float sina = sin(angleInRadians);

    // first column of final rotation matrix
    Vector3 col1 = Vector3(cosa + u_x * u_x * (1.0 - cosa)      ,
                           u_y * u_x * (1.0 - cosa) + u_z * sina,
                           u_z * u_x * (1.0 - cosa) - u_y * sina);

    // second column of final rotation matrix
    Vector3 col2 = Vector3(u_x * u_y * (1.0 - cosa) - u_z * sina,
                           cosa + u_y * u_y * (1.0 - cosa)      ,
                           u_z * u_y * (1.0 - cosa) + u_x * sina);

    // third column of final rotation matrix
    Vector3 col3 = Vector3(u_x * u_z * (1.0 - cosa) + u_y * sina,
                           u_y * u_z * (1.0 - cosa) - u_x * sina,
                           cosa + u_z * u_z * (1.0 - cosa)      );

    Matrix4 rotationMatrix = Matrix4(col1, col2, col3);
    return rotationMatrix;
}

void Camera::rotateAroundXAxisObject(float angleInRadians)
{
    pitch += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = upDirection.cross(viewDirection);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundYAxisObject(float angleInRadians)
{
    yaw += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = upDirection;
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundZAxisObject(float angleInRadians)
{
    roll += angleInRadians;

    Vector3 oldPosition = position;

    // go to (0.0, 0.0, 0.0)
    translate(Vector3(-oldPosition.x, -oldPosition.y, -oldPosition.z));

    Vector3 rotationAxis = viewDirection;
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    // position does not change
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;

    // go back to the position the camera was previously in
    translate(Vector3(oldPosition.x, oldPosition.y, oldPosition.z));
}

void Camera::rotateAroundXAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(1.0, 0.0, 0.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    upDirection = rotationMatrix * upDirection;
    viewDirection = rotationMatrix * viewDirection;
}

void Camera::rotateAroundYAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(0.0, 1.0, 0.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
}

void Camera::rotateAroundZAxisWorld(float angleInRadians)
{
    Vector3 rotationAxis = Vector3(0.0, 0.0, 1.0);
    Matrix4 rotationMatrix = getRotationMatrixAroundArbitraryAxisThroughOrigin(angleInRadians, rotationAxis);

    position = rotationMatrix * position;
    viewDirection = rotationMatrix * viewDirection;
    upDirection = rotationMatrix * upDirection;
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

        glutSolidSphere(0.15, 10, 10);
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
    Node nodes[numberNodesWidth][numberNodesHeight];
    std::vector<Constraint> structuralConstraints;
    std::vector<Constraint> shearConstraints;
    std::vector<Constraint> structuralBendConstraints;
    std::vector<Constraint> shearBendConstraints;

    // node creation method
    void createNodes();

    // constraint creation methods
    void createConstraints();
    void createStructuralConstraints();
    void createShearConstraints();
    void createBendConstraints();
    void createStructuralBendConstraints();
    void createShearBendConstraints();

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
    float xPosCentered = -(numberNodesWidth - 1) / 2.0;
    float yPosCenteredInit = -(numberNodesHeight - 1) / 2.0;

    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        // start yPosCentered at beginning again
        float yPosCentered = yPosCenteredInit;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            // put elements in rectangular grid with 0.0 depth
            // note that the nodes are centered around (0.0, 0.0, 0.0)
            nodes[x][y] = Vector3(xPosCentered, yPosCentered, 0.0);
            yPosCentered += 1.0;
        }

        xPosCentered += 1.0;
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
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 1)
            {
                Node* leftNode = &nodes[x][y];
                Node* rightNode = &nodes[x + 1][y];
                Constraint rightConstraint(leftNode, rightNode);
                structuralConstraints.push_back(rightConstraint);
            }

            if(y < numberNodesHeight - 1)
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
    for(int x = 0; x < numberNodesWidth - 1; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 1; y += 1)
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
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            if(x < numberNodesWidth - 2)
            {
                Node* leftNode = &nodes[x][y];
                Node* rightNode = &nodes[x + 2][y];
                Constraint rightConstraint(leftNode, rightNode);
                structuralBendConstraints.push_back(rightConstraint);
            }

            if(y < numberNodesHeight - 2)
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
    for(int x = 0; x < numberNodesWidth - 2; x += 1)
    {
        for(int y = 0; y < numberNodesHeight - 2; y += 1)
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
    glColor3f(1.0, 1.0, 1.0);

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
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
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

// Declare a camera at origin
Camera camera;

// array which holds pressed status values of all keyboard keys other than the
// special ones (arrows + F1..F12 keys + home + end ...)
char keyboardStatus[256];

// prototypes
std::string isEnabled(bool controlVariableEnabled);
void showHelp();
void showDrawStatus();
void showCameraStatus();
void init();
void chooseRenderingMethod();
void drawWorldAxis();
void display();
void initializeKeyboardStatus();
void applyContinuousKeyboardCommands();
void normalKeyboard(unsigned char key, int x, int y);
void normalKeyboardRelease(unsigned char key, int x, int y);
void reshape(int w, int h);
void applyChanges();

// TODO
void drawBoundingBox();

void drawBoundingBox()
{

}

// used for idle callback
void applyChanges()
{
    applyContinuousKeyboardCommands();

    // redraw the screen
    glutPostRedisplay();
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

void showHelp()
{
    // help controls
    std::cout << "help controls:" << std::endl;
    std::cout << "  0: show help" << std::endl;

    std::cout << std::endl;

    // drawing controls
    std::cout << "drawing controls:" << std::endl;
    std::cout << "  1: toggle draw structural      constraints" << std::endl;
    std::cout << "  2: toggle draw shear           constraints" << std::endl;
    std::cout << "  3: toggle draw structural bend constraints" << std::endl;
    std::cout << "  4: toggle draw shear      bend constraints" << std::endl;
    std::cout << "  5: toggle draw nodes" << std::endl;
    std::cout << "  6: toggle draw wireframe" << std::endl;

    std::cout << std::endl;

    // yaw, pitch and roll controls
    std::cout << "camera object rotation controls:" << std::endl;
    std::cout << "  j: yaw   left  by " << angleIncrement << " rad" << std::endl;
    std::cout << "  l: yaw   right by " << angleIncrement << " rad" << std::endl;
    std::cout << "  k: pitch down  by " << angleIncrement << " rad" << std::endl;
    std::cout << "  i: pitch up    by " << angleIncrement << " rad" << std::endl;
    std::cout << "  u: roll  left  by " << angleIncrement << " rad" << std::endl;
    std::cout << "  o: roll  right by " << angleIncrement << " rad" << std::endl;

    std::cout << std::endl;

    // world camera controls
    std::cout << "camera world rotation controls:" << std::endl;
    std::cout << "  s: rotate camera left  around world X axis by " << angleIncrement << " rad" << std::endl;
    std::cout << "  w: rotate camera right around world X axis by " << angleIncrement << " rad" << std::endl;
    std::cout << "  a: rotate camera left  around world Y axis by " << angleIncrement << " rad" << std::endl;
    std::cout << "  d: rotate camera right around world Y axis by " << angleIncrement << " rad" << std::endl;
    std::cout << "  e: rotate camera left  around world Z axis by " << angleIncrement << " rad" << std::endl;
    std::cout << "  q: rotate camera right around world Z axis by " << angleIncrement << " rad" << std::endl;

    std::cout << std::endl;

    // camera position controls
    std::cout << "camera translation controls:" << std::endl;
    // note, these are unicode characters for representing arrows.
    // - Leftwards  Arrow (\u2190)
    // - Rightwards Arrow (\u2192)
    // - Upwards    Arrow (\u2191)
    // - Downwards  Arrow (\u2193)
    std::cout << "  \u2190: translate camera left  by " << translationIncrement << std::endl;
    std::cout << "  \u2192: translate camera right by " << translationIncrement << std::endl;
    std::cout << "  \u2191: translate camera up    by " << translationIncrement << std::endl;
    std::cout << "  \u2193: translate camera down  by " << translationIncrement << std::endl;

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
    std::cout << "  yaw                  : " << camera.getYaw() << " rad" << std::endl;
    std::cout << "  pitch                : " << camera.getPitch() << " rad" << std::endl;
    std::cout << "  roll                 : " << camera.getRoll() << " rad" << std::endl;

    std::cout << std::endl;
}

void init()
{
    // move camera back
    float cameraZ = numberNodesHeight;

    camera.translate(Vector3(0.0, 0.0, cameraZ));

    // show help, draw status, and camera status at program launch
    showHelp();
    showDrawStatus();
    showCameraStatus();

    // clear keyboard press status
    initializeKeyboardStatus();
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

void drawWorldAxis()
{
    if(drawWorldAxisEnabled)
    {
        glColor3f(1.0, 0.0, 0.0);
        // x-axis in red
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(1.0, 0.0, 0.0);
        glEnd();
        glPushMatrix();
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();

        // y-axis in green
        glColor3f(0.0, 1.0, 0.0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 1.0, 0.0);
        glEnd();
        glPushMatrix();
            glRotatef(-90, 1.0, 0.0, 0.0);
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();

        // z-axis in blue
        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 0.0, 0.0);
            glVertex3f(0.0, 0.0, 1.0);
        glEnd();
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidCone(0.15, 0.30, 10, 10);
        glPopMatrix();
    }
}

void display()
{
    // clear color buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // clear depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    chooseRenderingMethod();

    applyChanges();

    Vector3 cameraPosition = camera.getPosition();
    Vector3 cameraViewDirection = camera.getViewDirection();
    Vector3 cameraUpDirection = camera.getUpDirection();

    // all perspectives are already calculated and stored, so just position
    // the camera where it is supposed to be
    gluLookAt(cameraPosition.x,
              cameraPosition.y,
              cameraPosition.z,
              cameraViewDirection.x,
              cameraViewDirection.y,
              cameraViewDirection.z,
              cameraUpDirection.x,
              cameraUpDirection.y,
              cameraUpDirection.z);

    drawWorldAxis();

    // draw cloth
    cloth.draw();

    // swap buffers needed for double buffering
    glutSwapBuffers();
}

void initializeKeyboardStatus()
{
    for(int i = 0; i < 256; i += 1)
    {
        keyboardStatus[i] = false;
    }
}

void applyContinuousKeyboardCommands()
{
    for(int key = 0; key < 256; key += 1)
    {
        // if the key is being continuously held down
        if(keyboardStatus[key])
        {
            switch(key)
            {
                // world axis camera controls
                case 'a':
                    camera.rotateAroundYAxisWorld(-angleIncrement);
                    break;
                case 'd':
                    camera.rotateAroundYAxisWorld(angleIncrement);
                    break;
                case 's':
                    camera.rotateAroundXAxisWorld(-angleIncrement);
                    break;
                case 'w':
                    camera.rotateAroundXAxisWorld(angleIncrement);
                    break;
                case 'q':
                    camera.rotateAroundZAxisWorld(-angleIncrement);
                    break;
                case 'e':
                    camera.rotateAroundZAxisWorld(angleIncrement);
                    break;

                // yaw, pitch and roll camera controls
                case 'k':
                    // turn camera "down" one notch
                    camera.rotateAroundXAxisObject(angleIncrement);
                    break;
                case 'i':
                    // turn camera "up" one notch
                    camera.rotateAroundXAxisObject(-angleIncrement);
                    break;
                case 'j':
                    // turn camera "left" one notch
                    camera.rotateAroundYAxisObject(angleIncrement);
                    break;
                case 'l':
                    // turn camera "right" one notch
                    camera.rotateAroundYAxisObject(-angleIncrement);
                    break;
                case 'u':
                    // tilt camera "left" one notch
                    camera.rotateAroundZAxisObject(-angleIncrement);
                    break;
                case 'o':
                    // tilt camera "right" one notch
                    camera.rotateAroundZAxisObject(angleIncrement);
                    break;

                // camera translation controls
                case 'f':
                    // translate camera left
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    camera.translate(camera.getUpDirection().cross(camera.getViewDirection()).normalize() * translationIncrement);
                    break;
                case 'h':
                    // translate camera right
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    camera.translate(camera.getViewDirection().cross(camera.getUpDirection()).normalize() * translationIncrement);
                    break;
                case 't':
                    // translate camera up
                    camera.translate(camera.getUpDirection().normalize() * translationIncrement);
                    break;
                case 'g':
                    // translate camera down
                    camera.translate(-camera.getUpDirection().normalize() * translationIncrement);
                    break;
                case 'r':
                    // translate camera back
                    // TODO : gets stuck if advance too much
                    camera.translate(camera.getViewDirection().normalize() * translationIncrement);
                    break;
                case 'z':
                    // translate camera front
                    // TODO : gets stuck if advance too much
                    camera.translate(-camera.getViewDirection().normalize() * translationIncrement);
                    break;

                default:
                    break;
            } // end switch(key)

            // TODO : called to often
            // showCameraStatus();

        } // end if(keyboardStatus[key])
    } // end loop
}

void normalKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        // ESC control
        case 27:
            exit(0);
            break;

        // help controls
        case '0':
            showHelp();
            break;

        // drawing controls
        case '1':
            drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
            showDrawStatus();
            break;
        case '2':
            drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
            showDrawStatus();
            break;
        case '3':
            drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
            showDrawStatus();
            break;
        case '4':
            drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
            showDrawStatus();
            break;
        case '5':
            drawNodesEnabled = !drawNodesEnabled;
            showDrawStatus();
            break;
        case '6':
            drawWireFrameEnabled = !drawWireFrameEnabled;
            showDrawStatus();
            break;
        case '7':
            drawWorldAxisEnabled = !drawWorldAxisEnabled;
            showDrawStatus();

        // toggle enable state for other keyboard buttons which are to be
        // continuously applied (like rotations, translations, ...)
        default:
            keyboardStatus[key] = true;
    }
}

void normalKeyboardRelease(unsigned char key, int x, int y)
{
    keyboardStatus[key] = false;
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
    gluPerspective(60.0, aspectRatio, nearPlane, farPlane);

    // go back to modelview matrix (for other functions)
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(500, 500);

    // INF3 room
    glutInitWindowPosition(1070, 655);

    // normal laptop screen
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("Cloth Simulation");

    init();
    glutDisplayFunc(display);

    glutIdleFunc(applyChanges);

    glutReshapeFunc(reshape);

    // disable keyboard repeat, because we will use variables for continuous animation
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(normalKeyboard);
    glutKeyboardUpFunc(normalKeyboardRelease);

    glutMainLoop();
    return 0;
}