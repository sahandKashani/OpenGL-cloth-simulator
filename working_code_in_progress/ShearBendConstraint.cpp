#include "ShearBendConstraint.h"
#include "DrawingSettings.h"

// OpenGL imports
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

ShearBendConstraint::ShearBendConstraint(Node* n1, Node* n2) :
    Constraint(n1, n2)
{}

void ShearBendConstraint::draw()
{
    glPushAttrib(GL_POLYGON_BIT); // save mesh settings
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPushAttrib(GL_CURRENT_BIT); // save color

            DrawingSettings* drawingSettings = DrawingSettings::getInstance();
            if(drawingSettings->isDrawShearBendConstraintsEnabled())
            {
                Vector3 color = DrawingSettings::getInstance()->getShearBendConstraintColor();
                glColor3f(color.x, color.y, color.z);
                Constraint::draw();
            }

        glPopAttrib(); // GL_CURRENT_BIT
    glPopAttrib(); // GL_POLYGON_BIT
}