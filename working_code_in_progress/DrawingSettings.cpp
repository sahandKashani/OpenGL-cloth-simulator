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

bool DrawingSettings::isDrawStructuralBendConstraintsEnabled()
{
    return drawStructuralBendConstraintsEnabled;
}

bool DrawingSettings::isDrawShearBendConstraintsEnabled()
{
    return drawShearBendConstraintsEnabled;
}

bool DrawingSettings::isDrawSpheresEnabled()
{
    return drawSpheresEnabled;
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

void DrawingSettings::toggleDrawStructuralBendConstraintsEnabled()
{
    drawStructuralBendConstraintsEnabled = !drawStructuralBendConstraintsEnabled;
}

void DrawingSettings::toggleDrawShearBendConstraintsEnabled()
{
    drawShearBendConstraintsEnabled = !drawShearBendConstraintsEnabled;
}

void DrawingSettings::toggleDrawSpheresEnabled()
{
    drawSpheresEnabled = !drawSpheresEnabled;
}