#ifndef DRAWING_SETTINGS_H
#define DRAWING_SETTINGS_H

#include <string>

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

    void toggleDrawWireFrameEnabled();
    void toggleDrawNodesEnabled();
    void toggleDrawWorldAxisEnabled();
    void toggleDrawStructuralConstraintsEnabled();
    void toggleDrawShearConstraintsEnabled();
    void toggleDrawStructuralBendConstraintsEnabled();
    void toggleDrawShearBendConstraintsEnabled();
    void toggleDrawSpheresEnabled();

    void showDrawStatus();
    std::string isEnabled(bool controlVariableEnabled);

    void chooseRenderingMethod();
};

#endif