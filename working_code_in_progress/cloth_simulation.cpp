// compile with the following command:
//     clear; g++ -o cloth_simulation cloth_simulation.cpp Node.cpp Camera.cpp Constraint.cpp Arrow.cpp Sphere.cpp Triangle.cpp Cloth.cpp -lglut -lGLU -lGL; ./cloth_simulation

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

// used for animation duration
#include <sys/time.h>

#include "Node.h"
#include "Camera.h"
#include "Constraint.h"
#include "Arrow.h"
#include "Sphere.h"

float nearPlane = 1.0;
float farPlane = 1000.0;

// Global variables
bool drawWireFrameEnabled                 = false;
bool drawNodesEnabled                     = false;
bool drawWorldAxisEnabled                 = true;
bool drawStructuralConstraintsEnabled     = true;
bool drawShearConstraintsEnabled          = false;
bool drawStructuralBendConstraintsEnabled = false;
bool drawShearBendConstraintsEnabled      = false;

// camera angle increments in radians
// increment must be power of 2 for precision reasons
float angleIncrement = 0.03125; // 2^(-5)

// camera translation increments
// increment must be power of 2 for precision reasons
float translationIncrement = 1.0;

class Cloth
{
private:
    // number of nodes in each dimension
    int numberNodesWidth;
    int numberNodesHeight;

    // Nodes
    std::vector< std::vector<Node> > nodes;

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

    // constraint satisfaction methods
    void satisfyStructuralConstraints();
    void satisfyShearConstraints();
    void satisfyStructuralBendConstraints();
    void satisfyShearBendConstraints();

public:
    Cloth();
    Cloth(int width, int height);

    void draw();
    void satisfyConstraints();
    void addForce(Vector3 force);
    void applyForces(float duration);
    int getNumberNodesWidth();
    int getNumberNodesHeight();
    Node* getNode(int x, int y);
    void setNodeMass(int x, int y, float mass);
    void setNodePosition(int x, int y, Vector3 pos);
    void setNodeMoveable(int x, int y, bool moveable);
};

Cloth::Cloth() :
    numberNodesWidth(10),
    numberNodesHeight(10)
{
    createNodes();
    createConstraints();
}

Cloth::Cloth(int width, int height) :
    numberNodesWidth(width),
    numberNodesHeight(height)
{
    createNodes();
    createConstraints();
}

void Cloth::setNodeMoveable(int x, int y, bool moveable)
{
    getNode(x, y)->setMoveable(moveable);
}

void Cloth::setNodePosition(int x, int y, Vector3 pos)
{
    getNode(x, y)->setPosition(pos);
}

void Cloth::setNodeMass(int x, int y, float mass)
{
    getNode(x, y)->setMass(mass);
}

Node* Cloth::getNode(int x, int y)
{
    return &nodes[x][y];
}

int Cloth::getNumberNodesWidth()
{
    return numberNodesWidth;
}

int Cloth::getNumberNodesHeight()
{
    return numberNodesHeight;
}

void Cloth::createNodes()
{
    float xPosCentered = -(numberNodesWidth - 1) / 2.0;
    float yPosCenteredInit = -(numberNodesHeight - 1) / 2.0;

    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        // start yPosCentered at beginning again
        float yPosCentered = yPosCenteredInit;
        std::vector<Node> nodeColumn;

        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            // put elements in rectangular grid with 0.0 depth
            // note that the nodes are centered around (0.0, 0.0, 0.0)
            // nodes[x][y] = Vector3(xPosCentered, yPosCentered, 0.0);
            nodeColumn.push_back(Node(Vector3(xPosCentered, yPosCentered, 0.0)));
            yPosCentered += 1.0;
        }

        nodes.push_back(nodeColumn);

        xPosCentered += 1.0;
    }
}

// moves the nodes depending on the forces that are being applied to them
void Cloth::applyForces(float duration)
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            nodes[x][y].applyForces(duration);
        }
    }
}

void Cloth::addForce(Vector3 force)
{
    for(int x = 0; x < numberNodesWidth; x += 1)
    {
        for(int y = 0; y < numberNodesHeight; y += 1)
        {
            nodes[x][y].addForce(force);
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

void Cloth::satisfyStructuralConstraints()
{
    for(std::vector<Constraint>::iterator structuralConstraintIterator = structuralConstraints.begin();
        structuralConstraintIterator != structuralConstraints.end();
        ++structuralConstraintIterator)
    {
        structuralConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyShearConstraints()
{
    for(std::vector<Constraint>::iterator shearConstraintIterator = shearConstraints.begin();
        shearConstraintIterator != shearConstraints.end();
        ++shearConstraintIterator)
    {
        shearConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyStructuralBendConstraints()
{
    for(std::vector<Constraint>::iterator structuralBendConstraintIterator = structuralBendConstraints.begin();
        structuralBendConstraintIterator != structuralBendConstraints.end();
        ++structuralBendConstraintIterator)
    {
        structuralBendConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyShearBendConstraints()
{
    for(std::vector<Constraint>::iterator shearBendConstraintIterator = shearBendConstraints.begin();
        shearBendConstraintIterator != shearBendConstraints.end();
        ++shearBendConstraintIterator)
    {
        shearBendConstraintIterator->satisfyConstraint();
    }
}

void Cloth::satisfyConstraints()
{
    satisfyStructuralConstraints();
    satisfyShearConstraints();
    satisfyStructuralBendConstraints();
    satisfyShearBendConstraints();
}

void Cloth::drawStructuralConstraints()
{
    for(std::vector<Constraint>::iterator structuralConstraintIterator = structuralConstraints.begin();
        structuralConstraintIterator != structuralConstraints.end();
        ++structuralConstraintIterator)
    {
        structuralConstraintIterator->draw();
    }
}

void Cloth::drawShearConstraints()
{
    for(std::vector<Constraint>::iterator shearConstraintIterator = shearConstraints.begin();
        shearConstraintIterator != shearConstraints.end();
        ++shearConstraintIterator)
    {
        shearConstraintIterator->draw();
    }
}

void Cloth::drawStructuralBendConstraints()
{
    for(std::vector<Constraint>::iterator structuralBendConstraintIterator = structuralBendConstraints.begin();
        structuralBendConstraintIterator != structuralBendConstraints.end();
        ++structuralBendConstraintIterator)
    {
        structuralBendConstraintIterator->draw();
    }
}

void Cloth::drawShearBendConstraints()
{
    for(std::vector<Constraint>::iterator shearBendConstraintIterator = shearBendConstraints.begin();
        shearBendConstraintIterator != shearBendConstraints.end();
        ++shearBendConstraintIterator)
    {
        shearBendConstraintIterator->draw();
    }
}

// -----------------------------------------------------------------------------
// GLUT setup

// Cloth declaration
Cloth* cloth = 0;

// Declare a camera at origin
Camera* camera = 0;

// TODO : remove later, for testing only
// Triangle* triangle = 0;

// array which holds pressed status values of all keyboard keys other than the
// special ones (arrows + F1..F12 keys + home + end ...)
char keyboardStatus[256];

// the "timeval" structure contains 2 fields
//     long int tv_sec = number of whole seconds of elapsed time
//     long int tv_usec = number of microseconds of elapsed time (always lower than 1 million)
struct timeval oldTime;

// prototypes
std::string isEnabled(bool controlVariableEnabled);
void showHelp();
void showDrawStatus();
void showCameraStatus();
void createScene();
void chooseRenderingMethod();
void drawWorldAxis();
void display();
void initializeKeyboardStatus();
void applyContinuousKeyboardCommands();
void normalKeyboard(unsigned char key, int x, int y);
void normalKeyboardRelease(unsigned char key, int x, int y);
void reshape(int w, int h);
void applyChanges();
void resetCameraPosition();
void specialKeyboard(int key, int x, int y);
float getTimeDifference();

void createScene()
{
    cloth = new Cloth(20, 20);
    camera = new Camera();

    // TODO : remove later, for testing only
    // Node* n1 = new Node(Vector3(-2.0, 2.0, 1.0));
    // Node* n2 = new Node(Vector3(0.0, 0.0, 2.0));
    // Node* n3 = new Node(Vector3(2.0, 2.0, 1.0));
    // triangle = new Triangle(n1, n2, n3);

    resetCameraPosition();

    // TODO : enable later
    // show help at program launch
    // showHelp();

    // initialize the time (needed for future animations)
    // the value of oldTime will be changed through it's pointer
    gettimeofday(&oldTime, NULL);

    // TODO : fixing cloth at certain points
    cloth->setNodeMoveable(0, cloth->getNumberNodesHeight() - 1, false);
    cloth->setNodeMoveable(cloth->getNumberNodesWidth() - 1, 0, false);
    cloth->setNodeMoveable(0, 0, false);
    cloth->setNodeMoveable(cloth->getNumberNodesWidth() - 1, cloth->getNumberNodesHeight() - 1, false);
    // cloth->setNodeMoveable(cloth->getNumberNodesWidth() / 2, cloth->getNumberNodesHeight() - 1, false);

    // TODO : find suitable values
    // gravity
    Vector3 gravity(0.0, -1.0, 0.0);

    // TODO : find suitable values
    // wind
    Vector3 wind(0.0, 0.0, 4.0);

    cloth->addForce(gravity);
    cloth->addForce(wind);

    // clear keyboard press status
    initializeKeyboardStatus();
}

float getTimeDifference()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    struct timeval timeDifference;
    timeDifference.tv_sec = currentTime.tv_sec - oldTime.tv_sec;
    timeDifference.tv_usec = currentTime.tv_usec - oldTime.tv_usec;

    float duration = timeDifference.tv_sec + timeDifference.tv_usec / 1000000.0;

    // update oldTime for future time checks
    oldTime = currentTime;

    return duration;
}

// used for idle callback.
// Will process all changes that occur, such as keyboard commands, new forces, ...
void applyChanges()
{
    // find out what commands the keyboard is sending and apply them
    applyContinuousKeyboardCommands();

    // calculate time difference from last call to applyChanges()
    float duration = getTimeDifference();

    // apply all forces to the cloth
    cloth->applyForces(duration);

    // TODO : remove later, for testing only
    // for(int x = 0; x < cloth->getNumberNodesWidth(); x += 1)
    // {
    //     for(int y = 0; y < cloth->getNumberNodesHeight(); y += 1)
    //     {
    //         triangle->testIntersection(cloth->getNode(x, y));
    //     }
    // }

    // satisfy all constraints of the cloth after forces are applied
    cloth->satisfyConstraints();

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
    std::cout << "******************************************" << std::endl;
    std::cout << "********           help           ********" << std::endl;
    std::cout << "******************************************" << std::endl;

    std::cout << std::endl;

    std::cout << "ESC: exit cloth simulator" << std::endl;

    std::cout << std::endl;

    // help controls
    std::cout << "help controls:" << std::endl;
    std::cout << "  F1: show help" << std::endl;

    std::cout << std::endl;

    std::cout << "camera position reset:" << std::endl;
    std::cout << "  F2: reset camera position" << std::endl;

    std::cout << std::endl;

    std::cout << "status controls:" << std::endl;
    std::cout << "  F3: show camera status" << std::endl;
    std::cout << "  F4: show draw   status" << std::endl;

    std::cout << std::endl;

    // drawing controls
    std::cout << "drawing controls:" << std::endl;
    std::cout << "  F5 : toggle draw structural      constraints" << std::endl;
    std::cout << "  F6 : toggle draw shear           constraints" << std::endl;
    std::cout << "  F7 : toggle draw structural bend constraints" << std::endl;
    std::cout << "  F8 : toggle draw shear      bend constraints" << std::endl;
    std::cout << "  F9 : toggle draw nodes" << std::endl;
    std::cout << "  F10: toggle draw wireframe" << std::endl;
    std::cout << "  F11: toggle draw world axis" << std::endl;

    std::cout << std::endl;

    // yaw, pitch and roll controls
    std::cout << "camera object rotation controls:" << std::endl;
    std::cout << "  j: yaw   left " << std::endl;
    std::cout << "  l: yaw   right" << std::endl;
    std::cout << "  k: pitch down " << std::endl;
    std::cout << "  i: pitch up   " << std::endl;
    std::cout << "  u: roll  left " << std::endl;
    std::cout << "  o: roll  right" << std::endl;

    std::cout << std::endl;

    // world camera controls
    std::cout << "camera world rotation controls:" << std::endl;
    std::cout << "  s: rotate camera left  around world X axis" << std::endl;
    std::cout << "  w: rotate camera right around world X axis" << std::endl;
    std::cout << "  a: rotate camera left  around world Y axis" << std::endl;
    std::cout << "  d: rotate camera right around world Y axis" << std::endl;
    std::cout << "  e: rotate camera left  around world Z axis" << std::endl;
    std::cout << "  q: rotate camera right around world Z axis" << std::endl;

    std::cout << std::endl;

    // camera position controls
    std::cout << "camera translation controls:" << std::endl;
    std::cout << "  f: translate camera left" << std::endl;
    std::cout << "  h: translate camera right" << std::endl;
    std::cout << "  t: translate camera up" << std::endl;
    std::cout << "  g: translate camera down" << std::endl;
    std::cout << "  r: translate camera back" << std::endl;
    std::cout << "  z: translate camera front" << std::endl;

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
    std::cout << "  draw world axis                 : " << isEnabled(drawWorldAxisEnabled) << std::endl;

    std::cout << std::endl;
}

void showCameraStatus()
{
    std::cout << "camera status:" << std::endl;
    std::cout << "  camera position      : " << camera->getPosition().toString() << std::endl;
    std::cout << "  camera view direction: " << camera->getViewDirection().toString() << std::endl;
    std::cout << "  camera up direction  : " << camera->getUpDirection().toString() << std::endl;
    std::cout << "  yaw                  : " << camera->getYaw() << " rad" << std::endl;
    std::cout << "  pitch                : " << camera->getPitch() << " rad" << std::endl;
    std::cout << "  roll                 : " << camera->getRoll() << " rad" << std::endl;

    std::cout << std::endl;
}

void resetCameraPosition()
{
    // reset the camera
    delete camera;
    camera = new Camera();

    // move camera back
    float cameraZ = 1.75 * std::max(cloth->getNumberNodesWidth(), cloth->getNumberNodesHeight());
    camera->translate(Vector3(0.0, 0.0, cameraZ));
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

// The display function only takes care of drawing.
// No modifications to any forces, keyboard commands, ... are processed here.
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

    Vector3 cameraPosition = camera->getPosition();
    Vector3 cameraViewDirection = camera->getViewDirection();
    Vector3 cameraUpDirection = camera->getUpDirection();

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

    // choose wireframe or solid rendering
    chooseRenderingMethod();

    // draw the world axis
    drawWorldAxis();

    // TODO : remove later, for testing only
    // triangle->draw();

    // draw cloth
    cloth->draw();

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
                    camera->rotateAroundYAxisWorld(-angleIncrement);
                    break;
                case 'd':
                    camera->rotateAroundYAxisWorld(angleIncrement);
                    break;
                case 's':
                    camera->rotateAroundXAxisWorld(-angleIncrement);
                    break;
                case 'w':
                    camera->rotateAroundXAxisWorld(angleIncrement);
                    break;
                case 'q':
                    camera->rotateAroundZAxisWorld(-angleIncrement);
                    break;
                case 'e':
                    camera->rotateAroundZAxisWorld(angleIncrement);
                    break;

                // yaw, pitch and roll camera controls
                case 'k':
                    // turn camera "down" one notch
                    camera->rotateAroundXAxisObject(angleIncrement);
                    break;
                case 'i':
                    // turn camera "up" one notch
                    camera->rotateAroundXAxisObject(-angleIncrement);
                    break;
                case 'j':
                    // turn camera "left" one notch
                    camera->rotateAroundYAxisObject(angleIncrement);
                    break;
                case 'l':
                    // turn camera "right" one notch
                    camera->rotateAroundYAxisObject(-angleIncrement);
                    break;
                case 'u':
                    // tilt camera "left" one notch
                    camera->rotateAroundZAxisObject(-angleIncrement);
                    break;
                case 'o':
                    // tilt camera "right" one notch
                    camera->rotateAroundZAxisObject(angleIncrement);
                    break;

                // camera translation controls
                case 'f':
                    // translate camera left
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    // FIXME : translates in wrong direction
                    camera->translate(camera->getUpDirection().cross(camera->getViewDirection()).normalize() * translationIncrement);
                    break;
                case 'h':
                    // translate camera right
                    // calculate "x" axis of camera with a cross product between
                    // it's upDirection and viewDirection
                    // FIXME : translates in wrong direction
                    camera->translate(camera->getViewDirection().cross(camera->getUpDirection()).normalize() * translationIncrement);
                    break;
                case 't':
                    // translate camera up
                    camera->translate(camera->getUpDirection().normalize() * translationIncrement);
                    break;
                case 'g':
                    // translate camera down
                    camera->translate(-camera->getUpDirection().normalize() * translationIncrement);
                    break;
                case 'r':
                    // translate camera back
                    // FIXME : gets stuck if advance too much, since the vector's length becomes 0.0, and normalize throws assertion error
                    camera->translate(camera->getViewDirection().normalize() * translationIncrement);
                    break;
                case 'z':
                    // translate camera front
                    // FIXME : gets stuck if advance too much, since the vector's length becomes 0.0, and normalize throws assertion error
                    camera->translate(-camera->getViewDirection().normalize() * translationIncrement);
                    break;

                default:
                    break;
            } // end switch(key)
        } // end if(keyboardStatus[key])
    } // end loop
}

void normalKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        // ESC control
        case 27:
            // clean up the cloth and camera before exiting
            delete camera;
            delete cloth;
            exit(0);
            break;

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

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            showHelp();
            break;
        case GLUT_KEY_F2:
            resetCameraPosition();
            break;
        case GLUT_KEY_F3:
            showCameraStatus();
            break;
        case GLUT_KEY_F4:
            showDrawStatus();
            break;
        case GLUT_KEY_F5:
            drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
            break;
        case GLUT_KEY_F6:
            drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
            break;
        case GLUT_KEY_F7:
            drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
            break;
        case GLUT_KEY_F8:
            drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
            break;
        case GLUT_KEY_F9:
            drawNodesEnabled = !drawNodesEnabled;
            break;
        case GLUT_KEY_F10:
            drawWireFrameEnabled = !drawWireFrameEnabled;
            break;
        case GLUT_KEY_F11:
            drawWorldAxisEnabled = !drawWorldAxisEnabled;
            break;

        default:
            break;
    }
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

    glutInitWindowSize(400, 400);

    // INF3 room
    glutInitWindowPosition(1200, 800);

    // normal laptop screen
    // glutInitWindowPosition(100, 100);

    glutCreateWindow("Cloth Simulation");

    createScene();

    glutDisplayFunc(display);
    glutIdleFunc(applyChanges);
    glutReshapeFunc(reshape);

    // disable keyboard repeat, because we will use variables for continuous animation
    glutIgnoreKeyRepeat(1);

    glutKeyboardFunc(normalKeyboard);
    glutKeyboardUpFunc(normalKeyboardRelease);
    glutSpecialFunc(specialKeyboard);

    glutMainLoop();
    return 0;
}