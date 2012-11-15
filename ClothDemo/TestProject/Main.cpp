/********************************************************************************************************
*	A Cloth physics simulation program																	*
*	By Nicholas Crook																					*
*																										*
*	based on work by:																					*
*		Mosegaard <http://cg.alexandra.dk/2009/06/02/mosegaards-cloth-simulation-coding-tutorial/>		*
*			Outline of class structure and application of force to cloth								*
*		Hugo.Elias <http://freespace.virgin.net/hugo.elias/models/m_cloth.htm>							*
*			Algorithm used to simulate cloth like behavior												*
*		Ian Millington <http://procyclone.com/>															*
*			Basis for integration based physics rather then timestep as Mosegaard uses					*
*********************************************************************************************************/

#include "Cloth.h"

#include "glut.h"
  
unsigned long lastUpdate;	// When was the last time we updated the cloth position
Cloth cloth(9,9,35,35);		// The cloth fabric were operating on
Vector3f ball_pos(3.5, -3,3.5);	// The location of the Ball the cloth lands on
float ball_radius = 2;		// The size of the ball
bool started = false;		// Wait until space bar has been pressed before dropping the cloth

// Initialize the GL context with the following properties
void init(GLvoid)
{
	glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearDepth(1.0f);
	
	// Display with depth cues to prevent the sphere or cloth from incorrectly covering eachother
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	// Used to light the bottom back side
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = {1.0,-0.5,-0.2,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,(GLfloat *) &lightPos);

	// Lights the top left
	glEnable(GL_LIGHT1);
	GLfloat lightAmbient1[4] = {0.0,0.0,0.0,0.0};
	GLfloat lightPos1[4] = {-1.0,1.0,0,0.0};
	GLfloat lightDiffuse1[4] = {0.5,0.5,0.3,0.0};
	glLightfv(GL_LIGHT1,GL_POSITION,(GLfloat *) &lightPos1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,(GLfloat *) &lightAmbient1);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,(GLfloat *) &lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
}

void idleFunc() {
	// If the cloth has been droppede
	if (started) {
		// Apply a wind force to the cloth and calculate the pull of neighbouring particles
		cloth.calculateForces(Vector3f(0.04, -0.02,0.02),ball_pos, ball_radius);

		// Get the current time so we can calculate how much time since the last update
		// used when integrating the distance a particle has traveled
		unsigned long currentUpdate = glutGet(GLUT_ELAPSED_TIME);

		// Move the cloth particles forward in time by how much time passed since the last update
		cloth.update((currentUpdate - lastUpdate)/100.0f);
		lastUpdate = currentUpdate;
	}
}

void display() {
	// Clear the screen with the blue color set in the init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// Move the scene in front of the camera then rotate it before doing any drawing
	glPushMatrix();
	glTranslatef(-4,3.5,-9.0f);
	glRotatef(25,0,1,0);
	cloth.draw();

	// Move the ball then draw
	glPushMatrix();
	glTranslatef(ball_pos.getX(),ball_pos.getY(),ball_pos.getZ());
	glColor3f(0.8f,0.3f,0.2f);
	glutSolidSphere(ball_radius-0.1,50,50);

	// Clear the transforms and rotations applied earlier
	glPopMatrix();
	glPopMatrix();

	// Display the new frame
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) 
{
	switch ( key ) {

	// On ESC quit
	case 27:    
		exit ( 0 );
		break;  

	// On Space drop the cloth or pause its faul
	case 32:
		started = !started;
		lastUpdate = glutGet(GLUT_ELAPSED_TIME);
		break;
	}
}

void reshape(int w, int h)  
{
	// Adjust the viewport if the user resizes the window
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();  
	if (h==0)  
		gluPerspective(80,(float)w,1.0,5000.0);
	else
		gluPerspective (80,( float )w /( float )h,1.0,5000.0 );
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity(); 
}

   
int main(int argc, char** argv) {
	// Initialize a window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH ); 
	glutInitWindowSize(800, 600 ); 
	glutCreateWindow( "Cloth Test" );

	// Initialize the GL context with predefined values
	init();

	// Link the window to the rest of the code
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idleFunc);

	// Begin the window's event polling
	glutMainLoop();
	return 0;
}