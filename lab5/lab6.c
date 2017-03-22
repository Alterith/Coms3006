
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
	{ /* "pearl plastic" */   0.25f, 0.20725f, 0.20725f, 1.0f, 1.0f, 0.8f, 0.8f, 1.0f, 0.3f, 0.3f, 0.3f, 1.0f, 0.01*128 },//18
};


// ------------------------ OpenGL rendering and  initialization -----------------------

/**
 * The display method is called when the panel needs to be drawn.
 * Here, it draws a stage and some objects on the stage.
 */

 //keep track of changes in mouse position
 int deltaX = 0;
 int deltaY = 0;

// draw polyhedron from polyhedron class
 void drawPolyhedron(Polyhedron p){
			int normalIdx = 0;
			int len = p.totalNumFaceVertices;
			size_t position = 0;
			//loop through face array
			for (size_t i = 0; i < len; i++) {
				//printf("%s\n","reached inside" );
				//when a -1 is reached proceed
				if (p.faces[i] == -1) {
					// draw vertex
					glPushMatrix();
					glBegin(GL_TRIANGLE_FAN);
					//if a colour face exists use it else ignore
					if(p.faceColors){
						glColor3d(p.faceColors[3*normalIdx], p.faceColors[3*normalIdx+1], p.faceColors[3*normalIdx+2]);
					}
					//initialize normal for face
					glNormal3d(-p.normals[3*normalIdx], -p.normals[3*normalIdx + 1], -p.normals[3*normalIdx + 2]);
					//printf("%s\n", "coloured faces");
					for (size_t j = 0; j < (i-position); j++) {
							//draw vertex
							 glVertex3f(p.vertices[3*p.faces[j+position]], p.vertices[3*p.faces[j+position]+1], p.vertices[3*p.faces[j+position]+2]);
					}
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

		//enable rotation of the stage
		glRotatef((deltaX*0.1), 0, 1, 0);
		glRotatef((deltaY*0.1), 1, 0, 0);

    float gray[] = { 0.6f, 0.6f, 0.6f, 1 };
    float zero[] = { 0, 0, 0, 1 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    glPushMatrix();
    	glTranslatef(0,-1.5,0); // Move top of stage down to y = 0
    	glScalef(1, 0.05, 1); // Stage will be one unit thick,
    	glutSolidCube(20);
    glPopMatrix();

		//call the function createPolyhedra to create the polyhedron'
		createPolyhedra();

		//draw a house at center
		glPushMatrix();
		glScaled(1.1, 1.3, 1.1);
		drawPolyhedron(house);
		glPopMatrix();

		//use the drawPolyhedron function to draw a truncatedRhombicDodecahedron and apply a material to it then translate it to a corner
		glPushMatrix();
		glTranslated(9, 0, 9);//translate to corner
		glScaled(0.75, 0.75, 0.75);// scale down
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[1] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[1][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[1][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[1][12] );
		drawPolyhedron(truncatedRhombicDodecahedron);//draw
		glPopMatrix();

		//use the drawPolyhedron function to draw an incorrectly spelled(XP) soccerball and apply a material to it then translate it to a corner
		glPushMatrix();
		glTranslated(-9, 0, 9);//translat eto corner
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[2] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[2][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[2][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[2][12] );
		drawPolyhedron(socerBall);//draw
		glPopMatrix();

		//use the drawPolyhedron function to draw a truncatedIcosahedron and apply a material to it then translate it to a corner
		glPushMatrix();
		glTranslated(-9, 0, -9);//translate to corner
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[3] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[3][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[3][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[3][12] );
		drawPolyhedron(truncatedIcosahedron);//draw
		glPopMatrix();

		//use the drawPolyhedron function to draw a stellatedOctahedron and apply a material to it then translate it to a corner
		glPushMatrix();
		glTranslated(9, -0.35, -9);//translate to corner
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[4][4] );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[4][8] );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[4][12] );
		drawPolyhedron(stellatedOctahedron);//draw
		glPopMatrix();


		//draw a teapot shrink it and place it upon the table
    glPushMatrix();
				glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[3] );
				glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[3][4] );
				glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[3][8] );
				glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[3][12] );
        glTranslatef(4,0,1.5); // translate so base of pot is on surface of stage
				glScaled(0.2, 0.2, 0.2);
        glutSolidTeapot(2); // draw a solid teapot of size 2
    glPopMatrix();


		//draw another glut object and place it at a corner
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[13] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[13][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[13][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[13][12] );
			glTranslatef(-9, -0.35, 0);
			glRotated(90, 0, 1, 0);
			glutSolidRhombicDodecahedron();
		glPopMatrix();


		//draw a wireframe icosahedron and plave at front of stage diable lighting for wireframe objects
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glTranslatef(0, -0.1,9);
			glutWireIcosahedron();
			glEnable(GL_LIGHTING);
		glPopMatrix();

		//draw a cone usling glut and sink into stage to make emulated table obsidian material
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[2] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[2][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[2][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[2][12] );
			glTranslatef(4, -0.3, 1.5);
			glRotated(90, 1, 0, 0);
			glutSolidCone(1, 1.5, 32, 32);
		glPopMatrix();


		//draw a simple glut cylinder material chrome
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[8] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[8][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[8][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[8][12] );
			glTranslatef(9, 1, 0);
			glRotated(90, 1, 0, 0);
			glutSolidCylinder(1, 2, 32, 32);
		glPopMatrix();

		//draw a window colour chrome
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[8] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[8][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[8][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[8][12] );
			glTranslatef(0, 0.75, 2.25);
			glScaled(2.5, 0.75, 0.001);
			glutSolidCube(1);
		glPopMatrix();

		//draw another window colour chrome
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[8] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[8][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[8][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[8][12] );
			glTranslatef(0, 0.75, -2.25);
			glScaled(2.5, 0.75, 0.001);
			glutSolidCube(1);
		glPopMatrix();

		//draw a door
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[2] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[2][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[2][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[2][12] );
			glTranslatef(2.25, -0.2, 0);
			glScaled(0.001, 1.5, 0.8);
			glutSolidCube(1);
		glPopMatrix();

		//draw door handle plastic material
		glPushMatrix();
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materials[17] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &materials[17][4] );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &materials[17][8] );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materials[17][12] );
			glTranslatef(2.25, -0.2, 0.15);
			glScaled(1, 1, 0.8);
			glutSolidSphere(0.1, 8, 8);
		glPopMatrix();


		glPopMatrix();


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
		float col11[4] = { 0.21, 0.43, 0.01, 1};
		float col12[4] = { 0.21, 0, 0, 1 };
		float col13[4] = { 0.16, 0, 0, 1 };
		float position1[4] = { 0, 0, 0, 1 };
		glLightfv(GL_LIGHT1, GL_POSITION, position1);
		glLightfv( GL_LIGHT1, GL_DIFFUSE, col11 );
		glLightfv( GL_LIGHT1, GL_SPECULAR, col12 );
		glLightfv( GL_LIGHT1, GL_AMBIENT, col13 );

		float col21[4] = { 0.31, 0.13, 0.71, 1};
		float col22[4] = { 0.1, 0, 0.3, 1 };
		float col23[4] = { 0.01, 0.14, 0.032, 1 };
		float position2[4] = { 1, 1, 0, 0 };
		glLightfv(GL_LIGHT2, GL_POSITION, position2);
		glLightfv( GL_LIGHT2, GL_DIFFUSE, col21 );
		glLightfv( GL_LIGHT2, GL_SPECULAR, col22 );
		glLightfv( GL_LIGHT2, GL_AMBIENT, col23 );


    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
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
		// track changes of x
		deltaX = deltaX + (x - prevX);

		//track changes of y limiting it between -40 and 40 change from 0 thus only tilting is applied
		if(deltaY>=40){
			if((y - prevY)<0){
				deltaY = deltaY + (y - prevY);
			}
		}else if(deltaY <= -40){
			if((y - prevY)>0){
				deltaY = deltaY + (y - prevY);
			}
		}else{
			deltaY = deltaY + (y - prevY);
		}
		printf("%s%d\n", "the x change is ", deltaY);
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
