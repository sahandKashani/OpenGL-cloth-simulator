#include "DrawingSettings.h"

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
    drawWireFrameEnabled(false),
    drawNodesEnabled(false),
    drawWorldAxisEnabled(true),
    drawStructuralConstraintsEnabled(true),
    drawShearConstraintsEnabled(true),
    drawStructuralBendConstraintsEnabled(true),
    drawShearBendConstraintsEnabled(true),
    drawSpheresEnabled(true)
{}

bool isDrawWireFrameEnabled()
{
    return drawWireFrameEnabled;
}

bool isDrawNodesEnabled()
{
    return drawNodesEnabled;
}

bool isDrawWorldAxisEnabled()
{
    return drawWorldAxisEnabled;
}

bool isDrawStructuralConstraintsEnabled()
{
    return drawStructuralConstraintsEnabled;
}

bool isDrawShearConstraintsEnabled()
{
    return drawShearConstraintsEnabled;
}

bool isDrawStructuralBendConstraintsEnabled()
{
    return drawStructuralBendConstraintsEnabled;
}

bool isDrawShearBendConstraintsEnabled()
{
    return drawShearBendConstraintsEnabled;
}

bool isDrawSpheresEnabled()
{
    return drawSpheresEnabled;
}

bool toggleDrawWireFrameEnabled()
{
    drawWireFrameEnabled = !drawWireFrameEnabled;
}

bool toggleDrawNodesEnabled()
{
    drawNodesEnabled = !drawNodesEnabled;
}

bool toggleDrawWorldAxisEnabled()
{
    drawWorldAxisEnabled = !drawWorldAxisEnabled;
}

bool toggleDrawStructuralConstraintsEnabled()
{
    drawStructuralConstraintsEnabled = !drawStructuralConstraintsEnabled;
}

bool toggleDrawShearConstraintsEnabled()
{
    drawShearConstraintsEnabled = !drawShearConstraintsEnabled;
}

bool toggleDrawStructuralBendConstraintsEnabled()
{
    drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
}

bool toggleDrawShearBendConstraintsEnabled()
{
    drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
}

bool toggleDrawSpheresEnabled()
{
    drawSpheresEnabled = !drawSpheresEnabled;
}