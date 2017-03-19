
/**
 * CPSC 424, Fall 2015, Lab 6:  Light and Material in OpenGL 1.1.
 * This program shows a square "stage" with a variety of objects
 * arranged on it.  The objects use several shapes and materials
 * and include a wireframe object that is drawn with lighting
 * turned off.
 *
 * This program use GLU as well as GLUT, and it depends on polyhedron.c,
 * which requires the math library.  It can be compiled with
 *
 *        gcc -o lab6 lab6.c polyhedron.c -lGL -lglut -lGLU -lm
 */

#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdio.h>      // (Can be used for debugging messages, with printf().)
#include "polyhedron.h" // For access to the regular polyhedra from polyhedron.c.

// --------------------------- Data for some materials ---------------------------------------------------

/**
 * One of the rows of this array corresponds to a set of material properties.  Items 0 to 4 in a row
 * specify an ambient color; items 4 through 7, a diffuse color; items 8 through 11, a specular color;
 * and item 12, a specular exponent (shininess value).  The data is adapted from the table on the page
 * http://devernay.free.fr/cours/opengl/materials.html
 */
float materials[][13] = {
	{ /* "emerald" */   0.0215f, 0.1745f, 0.0215f, 1.0f, 0.07568f, 0.61424f, 0.07568f, 1.0f, 0.633f, 0.727811f, 0.633f, 1.0f, 0.6f*128 },//0
	{ /* "jade" */   0.135f, 0.2225f, 0.1575f, 1.0f, 0.54f, 0.89f, 0.63f, 1.0f, 0.316228f, 0.316228f, 0.316228f, 1.0f, 0.1f*128 },//1
	{ /* "obsidian" */   0.05375f, 0.05f, 0.06625f, 1.0f, 0.18275f, 0.17f, 0.22525f, 1.0f, 0.332741f, 0.328634f, 0.346435f, 1.0f, 0.3f*128 },//2
	{ /* "pearl" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.829f, 0.829f, 1.0f, 0.296648f, 0.296648f, 0.296648f, 1.0f, 0.088f*128 },//3
	{ /* "ruby" */   0.1745f, 0.01175f, 0.01175f, 1.0f, 0.61424f, 0.04136f, 0.04136f, 1.0f, 0.727811f, 0.626959f, 0.626959f, 1.0f, 0.6f*128 },//4
	{ /* "turquoise" */   0.1f, 0.18725f, 0.1745f, 1.0f, 0.396f, 0.74151f, 0.69102f, 1.0f, 0.297254f, 0.30829f, 0.306678f, 1.0f, 0.1f*128 },//5
	{ /* "brass" */   0.329412f, 0.223529f, 0.027451f, 1.0f, 0.780392f, 0.568627f, 0.113725f, 1.0f, 0.992157f, 0.941176f, 0.807843f, 1.0f, 0.21794872f*128 },//6
	{ /* "bronze" */   0.2125f, 0.1275f, 0.054f, 1.0f, 0.714f, 0.4284f, 0.18144f, 1.0f, 0.393548f, 0.271906f, 0.166721f, 1.0f, 0.2f*128 },//7
	{ /* "chrome" */   0.25f, 0.25f, 0.25f, 1.0f, 0.4f, 0.4f, 0.4f, 1.0f, 0.774597f, 0.774597f, 0.774597f, 1.0f, 0.6f*128 },//8
	{ /* "copper" */   0.19125f, 0.0735f, 0.0225f, 1.0f, 0.7038f, 0.27048f, 0.0828f, 1.0f, 0.256777f, 0.137622f, 0.086014f, 1.0f, 0.1f*128 },//9
	{ /* "gold" */   0.24725f, 0.1995f, 0.0745f, 1.0f, 0.75164f, 0.60648f, 0.22648f, 1.0f, 0.628281f, 0.555802f, 0.366065f, 1.0f, 0.4f*128 },//10
	{ /* "silver" */   0.19225f, 0.19225f, 0.19225f, 1.0f, 0.50754f, 0.50754f, 0.50754f, 1.0f, 0.508273f, 0.508273f, 0.508273f, 1.0f, 0.4f*128 },//11
	{ /* "cyan plastic" */   0.0f, 0.1f, 0.06f, 1.0f, 0.0f, 0.50980392f, 0.50980392f, 1.0f, 0.50196078f, 0.50196078f, 0.50196078f, 1.0f, .25f*128 },//12
	{ /* "green plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.1f, 0.35f, 0.1f, 1.0f, 0.45f, 0.55f, 0.45f, 1.0f, .25f*128 },//13
	{ /* "red plastic" */   0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.7f, 0.6f, 0.6f, 1.0f, .25f*128 },//14
	{ /* "cyan rubber" */   0.0f, 0.05f, 0.05f, 1.0f, 0.4f, 0.5f, 0.5f, 1.0f, 0.04f, 0.7f, 0.7f, 1.0f, .078125f*128 },//15
	{ /* "green rubber" */   0.0f, 0.05f, 0.0f, 1.0f, 0.4f, 0.5f, 0.4f, 1.0f, 0.04f, 0.7f, 0.04f, 1.0f, .078125f*128 },//16
	{ /* "red rubber" */   0.05f, 0.0f, 0.0f, 1.0f, 0.5f, 0.4f, 0.4f, 1.0f, 0.7f, 0.04f, 0.04f, 1.0f, .078125f*128 },//17
	{ /* "pearl plastic" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.8f, 0.8f, 1.0f, 0.3f, 0.3f, 0.3f, 1.0f, .25f*128 },//18
};


// ------------------------ OpenGL rendering and  initialization -----------------------

/**
 * The display method is called when the panel needs to be drawn.
 * Here, it draws a stage and some objects on the stage.
 */
 int deltaX = 0;
 int deltaY = 0;

 void drawPolyhedron(Polyhedron p){
	 //printf("%s\n", "reached before");
			int normalIdx = 0;
			int len = p.totalNumFaceVertices;
			//printf("Face vertices %d\n", len);
			size_t position = 0;
			//loop through face array
			//printf("%d\n", len);
			for (size_t i = 0; i < len; i++) {
				//printf("%lu\n",i );
				//printf("%s\n","reached inside" );
				//when a -1 is reached proceed
				//printf("%d\n", p.faces[i]);
				if (p.faces[i] == -1) {
					//printf("%s\n", "read -1");
					// draw vertex
					glPushMatrix();
					//printf("%s\n", "pushed matrix");
					glTranslated(5, 1,2);
					glBegin(GL_TRIANGLE_FAN);
					//printf("%s\n", "began triangle fan");
					if(p.faceColors){
						glColor3d(p.faceColors[3*normalIdx], p.faceColors[3*normalIdx+1], p.faceColors[3*normalIdx+2]);
					}
					glNormal3d(-p.normals[3*normalIdx], -p.normals[3*normalIdx + 1], -p.normals[3*normalIdx + 2]);
					//printf("%s\n", "coloured faces");
					for (size_t j = 0; j < (i-position); j++) {
							//printf("%s %lu\n","drawing" , (i-position));
							 glVertex3f(p.vertices[3*p.faces[j+position]], p.vertices[3*p.faces[j+position]+1], p.vertices[3*p.faces[j+position]+2]);
							 //printf("coordinates for %lu is (%lf,%lf,%lf)\n",(j+position), p.vertices[3*p.faces[j+position]], p.vertices[3*p.faces[j+position]+1], p.vertices[3*p.faces[j+position]+2]);
							 //glVertex3dv(p.vertices[3*p.faces[j+position]]);
					}
					//printf("%s\n","reached normals");
					//printf("Normal coords: (%lf,%lf,%lf)\n",p.normals[3*normalIdx],p.normals[3*normalIdx+1],p.normals[3*normalIdx+2] );
					glEnd();
					glPopMatrix();
					//initialize position to next element after -1
					position = i+1;
					normalIdx = normalIdx +1;
				}
			}

			//printf("%s\n", "reached after");

 }
void display() {
        // called whenever the display needs to be redrawn

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt( 0,8,40, 0,1,0, 0,1,0 );  // viewing transform

		//rotate depending on x and y changes
		glPushMatrix();
		//printf("%s%d\n", "The rotation degree is " , (deltaX*3 % 360));
		glRotatef((deltaX*0.1), 0, 1, 0);

    float gray[] = { 0.6f, 0.6f, 0.6f, 1 };
    float zero[] = { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    glPushMatrix();
    	glTranslatef(0,-1.5,0); // Move top of stage down to y = 0
    	glScalef(1, 0.05, 1); // Stage will be one unit thick,
			//glRotated(30, 0, 1, 0);
    	glutSolidCube(20);
    glPopMatrix();

		createPolyhedra();
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[5] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[5][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[5][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[5][12] );
		drawPolyhedron(dodecahedron);

		glPushMatrix();
		glTranslated(-10, 2, -1);
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[2] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[2][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[2][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[2][12] );
		drawPolyhedron(socerBall);
		glPopMatrix();

    glPushMatrix();
				glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[3] );
				glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[3][4] );
				glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[3][8] );
				glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[3][12] );
        glTranslatef(0,0.399,0); // translate so base of pot is on surface of stage
        glutSolidTeapot(2); // draw a solid teapot of size 2
    glPopMatrix();

		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[4][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[4][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[4][12] );
			glTranslatef(-8, -0.251, 0);
			glRotated(90, 0, 1, 0);
			glutSolidRhombicDodecahedron();
		glPopMatrix();

		glPushMatrix();
			glDisable(GL_LIGHTING);
			glTranslatef(6, -0.1,-5);
			glutWireIcosahedron();
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[18] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[18][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[18][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[18][12] );
			glTranslatef(4, -0.3, 0);
			glRotated(90, 1, 0, 0);
			glutSolidCone(1, 1.5, 32, 32);
		glPopMatrix();

		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[13] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[13][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[13][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[13][12] );
			glTranslatef(-6, 1, -4);
			glRotated(90, 1, 0, 0);
			glutSolidCylinder(1, 2, 32, 32);
		glPopMatrix();
		glPopMatrix();
    // TODO draw some shapes!


    glutSwapBuffers();  // (Required for double-buffered drawing, at the end of display().)
}

/**
 * initGL() is called just once, by main(), to do initialization of OpenGL state
 * and other global state. Here, it sets up a projection, configures some lighting,
 * and enables the depth test.  It also calls createPolyhedra(), whcih is defined
 * in the included file, polyhedron.h.
 */
void initGL() {
    createPolyhedra();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20, 2, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
		float col1[4] = { 0.72, 0.16, 0.26, 1};
		float col2[4] = { 0.41, 0, 0, 1 };
		float col3[4] = { 0.31, 0, 0, 1 };
		float position[4] = { 0, 0, 0, 1 };
		glLightfv(GL_LIGHT1, GL_POSITION, position);
		glLightfv( GL_LIGHT1, GL_DIFFUSE, col1 );
		glLightfv( GL_LIGHT1, GL_SPECULAR, col2 );
		glLightfv( GL_LIGHT1, GL_AMBIENT, col3 );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
    // TODO configure better lighting!
}  // end initGL()


// ------------------------------ mouse handling functions ----------------------------------

int dragging;        // 0 or 1 to indicate whether a drag operation is in progress
int dragButton;      // which button started the drag operation
int prevX, prevY;    // previous mouse position during drag

/*  mouseUpOrDown() is set up in main() to be called when the user presses or releases
 *  a button on the mouse.  The button paramter is one of the contants GLUT_LEFT_BUTTON,
 *  GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.  The buttonState is GLUT_UP or GLUT_DOWN and
 *  tells whether this is a mouse press or a mouse release event.  x and y give the
 *  mouse position in pixel coordinates, with (0,0) at the UPPER LEFT.
 */
void mouseUpOrDown(int button, int buttonState, int x, int y) {
       // called to respond to mouse press and mouse release events
   if (buttonState == GLUT_DOWN) {  // a mouse button was pressed
		 //printf("%s\n", "mouse pressed");
       if (dragging)
          return;  // Ignore a second button press during a draw operation.
       dragging = 1;
       dragButton = button;
       prevX = x;
       prevY = y;
   }
   else {  // a mouse button was released
       if ( ! dragging || button != dragButton )
           return; // this mouse release does not end a drag operation.
       dragging = 0;
   }
}

/*  mouseDragged() is set up in main() to be called when the user moves the mouse,
 *  but only when one or more mouse buttons are pressed.  x and y give the position
 *  of the mouse in pixel coordinates.
 */
void mouseDragged(int x, int y) {
	//printf("%s\n", "mouse dragged");
        // called to respond when the mouse moves during a drag
    if ( ! dragging )
        return;
    // TODO Do something when the mouse moves!
		deltaX = deltaX + (x - prevX);
		deltaY = deltaY +(y - prevY);
		//printf("%s%d\n", "the x change is ", deltaX);
    glutPostRedisplay();  // make OpenGL redraw the scene
    prevX = x;
    prevY = y;
}


// ----------------- main routine -------------------------------------------------

int main(int argc, char** argv) {
		//deltaX = 0;
		//deltaY = 0;
    glutInit(&argc, argv); // Allows processing of certain GLUT command line options
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  // Use double buffering and a depth buffer.
    glutInitWindowSize(1000,500);       // size of display area, in pixels
    glutInitWindowPosition(100,100);    // location in window coordinates
    glutCreateWindow("Stage");          // parameter is window title
    initGL();                           // do OpenGL initialization for the window
    glutDisplayFunc(display);           // call display() to draw the scene
    glutMouseFunc(mouseUpOrDown);       // call mouseUpOrDown() for mousedown and mouseup events
    glutMotionFunc(mouseDragged);       // call mouseDragged() when mouse moves, only during a drag gesture
    glutMainLoop(); // Run the event loop!  This function does not return.
    return 0;
}
