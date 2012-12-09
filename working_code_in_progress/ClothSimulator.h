#ifndef CLOTH_SIMULATOR_H
#define CLOTH_SIMULATOR_H

#include "Cloth.h"
#include "Camera.h"
#include "Floor.h"
#include "Triangle.h"
#include <string>

class ClothSimulator;

class ClothSimulator
{
private:
    static ClothSimulator* instance;

    void chooseRenderingMethod();
    void drawWorldAxis();
    void drawSpheres();
    void drawTriangles();
    void drawCloth();
    void drawFloor();

protected:
    ClothSimulator();

public:
    static ClothSimulator* getInstance();

    // Global variables
    float nearPlane;
    float farPlane;

    bool drawWireFrameEnabled;
    bool drawNodesEnabled;
    bool drawWorldAxisEnabled;
    bool drawStructuralConstraintsEnabled;
    bool drawShearConstraintsEnabled;
    bool drawStructuralBendConstraintsEnabled;
    bool drawShearBendConstraintsEnabled;
    bool drawSpheresEnabled;

    // increments must be power of 2 for precision reasons
    // camera angle increments in radians
    float angleIncrement;

    // camera translation increment
    float translationIncrement;

    // Time step between each application of the forces
    float timeStep;

    // array which holds pressed status values of all keyboard keys other than the
    // special ones (arrows + F1..F12 keys + home + end ...)
    char keyboardStatus[256];

    // cloth declaration
    Cloth* cloth;

    // camera declaration
    Camera* camera;

    // floor declaration
    Floor* floor;

    // Sphere declaration
    std::vector<Sphere> spheres;

    // Triangle declaration
    std::vector<Triangle> triangles;

    // methods
    ~ClothSimulator();
    std::string isEnabled(bool controlVariableEnabled);
    void showHelp();
    void showDrawStatus();
    void showCameraStatus();
    void initializeKeyboardStatus();
    void applyContinuousKeyboardCommands();
    void resetCameraPosition();
    float getTimeStep();

    void draw();

    void createScene();
    void createBatmanScene();

    bool leftFootGoingUp;
    bool leftFootGoingDown;
    bool rightFootGoingUp;
    bool rightFootGoingDown;
    bool leftFootUp;
    bool rightFootUp;
    void swingLeftFoot();
    void swingRightFoot();

    void simulate();

    void handleNormalKeyboardInput(unsigned char key, int x, int y);
    void handleNormalKeyboardRelease(unsigned char key, int x, int y);
};

#endif