#include "DrawingSettings.h"
#include <iostream>

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

DrawingSettings* DrawingSettings::instance = 0;

DrawingSettings* DrawingSettings::getInstance()
{
    if(instance == 0)
    {
        instance = new DrawingSettings();
    }

    return instance;
}

DrawingSettings::DrawingSettings() :
    drawWireFrameEnabled                 (false),
    drawNodesEnabled                     (false),
    drawWorldAxisEnabled                 (true ),
    drawStructuralConstraintsEnabled     (false),
    drawShearConstraintsEnabled          (false),
    drawSpheresEnabled                   (true ),
    drawTrianglesEnabled                 (true ),
    drawFloorEnabled                     (false),
    drawArrowsEnabled                    (false),
    nodesColor                   (1.0, 0.0, 0.0),
    structuralConstraintColor    (1.0, 1.0, 1.0),
    shearConstraintColor         (1.0, 1.0, 1.0),
    sphereColor                  (0.0, 0.0, 1.0),
    triangleColor                (1.0, 1.0, 1.0),
    floorColor                   (1.0, 1.0, 1.0),
    arrowColor                   (0.0, 1.0, 0.0)
{}

bool DrawingSettings::isDrawWireFrameEnabled()
{
    return drawWireFrameEnabled;
}

bool DrawingSettings::isDrawNodesEnabled()
{
    return drawNodesEnabled;
}

bool DrawingSettings::isDrawWorldAxisEnabled()
{
    return drawWorldAxisEnabled;
}

bool DrawingSettings::isDrawStructuralConstraintsEnabled()
{
    return drawStructuralConstraintsEnabled;
}

bool DrawingSettings::isDrawShearConstraintsEnabled()
{
    return drawShearConstraintsEnabled;
}

bool DrawingSettings::isDrawSpheresEnabled()
{
    return drawSpheresEnabled;
}

bool DrawingSettings::isDrawTrianglesEnabled()
{
    return drawTrianglesEnabled;
}

bool DrawingSettings::isDrawFloorEnabled()
{
    return drawFloorEnabled;
}

bool DrawingSettings::isDrawArrowsEnabled()
{
    return drawArrowsEnabled;
}

void DrawingSettings::toggleDrawWireFrameEnabled()
{
    drawWireFrameEnabled = !drawWireFrameEnabled;
}

void DrawingSettings::toggleDrawNodesEnabled()
{
    drawNodesEnabled = !drawNodesEnabled;
}

void DrawingSettings::toggleDrawWorldAxisEnabled()
{
    drawWorldAxisEnabled = !drawWorldAxisEnabled;
}

void DrawingSettings::toggleDrawStructuralConstraintsEnabled()
{
    drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
}

void DrawingSettings::toggleDrawShearConstraintsEnabled()
{
    drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
}

void DrawingSettings::toggleDrawSpheresEnabled()
{
    drawSpheresEnabled = !drawSpheresEnabled;
}

void DrawingSettings::toggleDrawTrianglesEnabled()
{
    drawTrianglesEnabled = !drawTrianglesEnabled;
}

void DrawingSettings::toggleDrawFloorEnabled()
{
    drawFloorEnabled = !drawFloorEnabled;
}

void DrawingSettings::toggleDrawArrowsEnabled()
{
    drawArrowsEnabled = !drawArrowsEnabled;
}

void DrawingSettings::showDrawStatus()
{
    std::cout << "draw status:" << std::endl;
    std::cout << "  draw nodes                      : " << isEnabled(drawNodesEnabled) << std::endl;
    std::cout << "  draw structural constraints     : " << isEnabled(drawStructuralConstraintsEnabled) << std::endl;
    std::cout << "  draw shear constraints          : " << isEnabled(drawShearConstraintsEnabled) << std::endl;
    std::cout << "  draw wireframe                  : " << isEnabled(drawWireFrameEnabled) << std::endl;
    std::cout << "  draw world axis                 : " << isEnabled(drawWorldAxisEnabled) << std::endl;
    std::cout << "  draw spheres                    : " << isEnabled(drawSpheresEnabled) << std::endl;
    std::cout << "  draw arrows                     : " << isEnabled(drawArrowsEnabled) << std::endl;
    std::cout << "  draw floor                      : " << isEnabled(drawFloorEnabled) << std::endl;
    std::cout << "  draw triangles                  : " << isEnabled(drawTrianglesEnabled) << std::endl;

    std::cout << std::endl;
}

// prints "true" if controlVariableEnabled is true, and "false" otherwise
std::string DrawingSettings::isEnabled(bool controlVariableEnabled)
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

// determines if a wireframe is to be drawn, or a textured version
void DrawingSettings::chooseRenderingMethod()
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

Vector3 DrawingSettings::getNodesColor()
{
    return nodesColor;
}

Vector3 DrawingSettings::getStructuralConstraintColor()
{
    return structuralConstraintColor;
}

Vector3 DrawingSettings::getShearConstraintColor()
{
    return shearConstraintColor;
}

Vector3 DrawingSettings::getSphereColor()
{
    return sphereColor;
}

Vector3 DrawingSettings::getTriangleColor()
{
    return triangleColor;
}

Vector3 DrawingSettings::getFloorColor()
{
    return floorColor;
}

Vector3 DrawingSettings::getArrowColor()
{
    return arrowColor;
}