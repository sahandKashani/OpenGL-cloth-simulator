#ifndef DRAWING_SETTINGS_H
#define DRAWING_SETTINGS_H

#include <string>
#include "Vector3.h"

class DrawingSettings
{
private:
    static DrawingSettings* instance;

    bool drawWireFrameEnabled;
    bool drawNodesEnabled;
    bool drawWorldAxisEnabled;
    bool drawStructuralConstraintsEnabled;
    bool drawShearConstraintsEnabled;
    bool drawSpheresEnabled;
    bool drawTrianglesEnabled;
    bool drawFloorEnabled;
    bool drawArrowsEnabled;

    Vector3 nodesColor;
    Vector3 structuralConstraintColor;
    Vector3 shearConstraintColor;
    Vector3 sphereColor;
    Vector3 triangleColor;
    Vector3 floorColor;
    Vector3 arrowColor;

    float timeStep;
    float originalTimeStep;

protected:
    DrawingSettings();

public:
    static DrawingSettings* getInstance();

    bool isDrawWireFrameEnabled();
    bool isDrawNodesEnabled();
    bool isDrawWorldAxisEnabled();
    bool isDrawStructuralConstraintsEnabled();
    bool isDrawShearConstraintsEnabled();
    bool isDrawSpheresEnabled();
    bool isDrawTrianglesEnabled();
    bool isDrawFloorEnabled();
    bool isDrawArrowsEnabled();

    void toggleDrawWireFrameEnabled();
    void toggleDrawNodesEnabled();
    void toggleDrawWorldAxisEnabled();
    void toggleDrawStructuralConstraintsEnabled();
    void toggleDrawShearConstraintsEnabled();
    void toggleDrawSpheresEnabled();
    void toggleDrawTrianglesEnabled();
    void toggleDrawFloorEnabled();
    void toggleDrawArrowsEnabled();

    void showDrawStatus();
    std::string isEnabled(bool controlVariableEnabled);

    void chooseRenderingMethod();

    Vector3 getNodesColor();
    Vector3 getStructuralConstraintColor();
    Vector3 getShearConstraintColor();
    Vector3 getSphereColor();
    Vector3 getTriangleColor();
    Vector3 getFloorColor();
    Vector3 getArrowColor();

    float getTimeStep();
    void setTimeStep(float time);
    float getOriginalTimeStep();
    void setOriginalTimeStep(float time);
};

#endif