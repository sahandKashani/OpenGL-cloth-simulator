#include "StructuralConstraint.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

StructuralConstraint::StructuralConstraint(Node* n1, Node* n2) :
    Constraint(n1, n2)
{}

void StructuralConstraint::draw()
{
    glPushAttrib(GL_POLYGON_BIT); // save mesh settings
        glPushAttrib(GL_CURRENT_BIT); // save color

            DrawingSettings* drawingSettings = DrawingSettings::getInstance();
            if(drawingSettings->isDrawStructuralConstraintsEnabled())
            {
                Vector3 color = drawingSettings->getStructuralConstraintColor();
                glColor3f(color.x, color.y, color.z);
                Constraint::draw();
            }

        glPopAttrib(); // GL_CURRENT_BIT
    glPopAttrib(); // GL_POLYGON_BIT
}