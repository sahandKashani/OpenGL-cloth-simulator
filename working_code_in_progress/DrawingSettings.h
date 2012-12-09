#ifndef DRAWING_SETTINGS_H
#define DRAWING_SETTINGS_H

class DrawingSettings
{
private:
    static DrawingSettings* instance;

    bool drawWireFrameEnabled;
    bool drawNodesEnabled;
    bool drawWorldAxisEnabled;
    bool drawStructuralConstraintsEnabled;
    bool drawShearConstraintsEnabled;
    bool drawStructuralBendConstraintsEnabled;
    bool drawShearBendConstraintsEnabled;
    bool drawSpheresEnabled;

protected:
    DrawingSettings();

public:
    static DrawingSettings* getInstance();

    bool isDrawWireFrameEnabled();
    bool isDrawNodesEnabled();
    bool isDrawWorldAxisEnabled();
    bool isDrawStructuralConstraintsEnabled();
    bool isDrawShearConstraintsEnabled();
    bool isDrawStructuralBendConstraintsEnabled();
    bool isDrawShearBendConstraintsEnabled();
    bool isDrawSpheresEnabled();

    bool toggleDrawWireFrameEnabled();
    bool toggleDrawNodesEnabled();
    bool toggleDrawWorldAxisEnabled();
    bool toggleDrawStructuralConstraintsEnabled();
    bool toggleDrawShearConstraintsEnabled();
    bool toggleDrawStructuralBendConstraintsEnabled();
    bool toggleDrawShearBendConstraintsEnabled();
    bool toggleDrawSpheresEnabled();
};

#endif