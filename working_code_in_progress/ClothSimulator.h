#ifndef CLOTH_SIMULATOR_H
#define CLOTH_SIMULATOR_H

#include "Cloth.h"
#include "Camera.h"
#include <string>
#include "Triangle.h"

class ClothSimulator;

class ClothSimulator
{
private:
    static ClothSimulator* instance;

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

    // the "timeval" structure contains 2 fields
    //     long int tv_sec = number of whole seconds of elapsed time
    //     long int tv_usec = number of microseconds of elapsed time (always lower than 1 million)
    struct timeval oldTime;

    // cloth declaration
    Cloth* cloth;

    // camera declaration
    Camera* camera;

    // Sphere declaration
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;

    // methods
    ~ClothSimulator();
    std::string isEnabled(bool controlVariableEnabled);
    void showHelp();
    void showDrawStatus();
    void showCameraStatus();
    void chooseRenderingMethod();
    void initializeKeyboardStatus();
    void applyContinuousKeyboardCommands();
    void resetCameraPosition();
    float getTimeStep();

    void drawWorldAxis();
    void drawSpheres();
    void drawTriangles();
    void drawCloth();

    void createScene();
    void createBatmanScene();
};

#endif