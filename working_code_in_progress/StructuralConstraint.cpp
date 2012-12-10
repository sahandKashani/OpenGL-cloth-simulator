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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPushAttrib(GL_CURRENT_BIT); // save color

            Vector3 color = DrawingSettings::getInstance()->getStructuralConstraintColor();
            glColor3f(color.x, color.y, color.z);
            Constraint::draw();

        glPopAttrib(); // GL_CURRENT_BIT
    glPopAttrib(); // GL_POLYGON_BIT
}