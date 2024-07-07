// Rylan Casanova and Dylan Olthoff
// CST-310
// 10/16/22
// Project 5: Render Your Scene With Primitives
// This is our work
// References used: MovingCamera.cpp

#include <stdlib.h> // standard definitions
#include <math.h> // math definitions
#include <stdio.h> // standard I/O

#include <GL/glut.h>

// Camera position
float x = 0.0, y = -5.0; // initially 5 units south of origin
float deltaMove = 0.0; // initially camera doesn't move

// Camera direction
float lx = 0.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)
{
	w = 1008;
	h = 756;
    float ratio =  ((float) w) / ((float) h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(45.0, ratio, 0.1, 100.0); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void)
{
	if (deltaMove) { // update camera position
		x += deltaMove * lx * 0.1;
		y += deltaMove * ly * 0.1;
	}
	glutPostRedisplay(); // redisplay everything
}

//----------------------------------------------------------------------
// Function to draw circles using GL_TRIANGLE_FAN primitive
//----------------------------------------------------------------------
void drawCircle(float x, float y, float z, float radius)
{
	float twicePi = 2.0f * M_PI;
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x, y, z);
		for (int i = 0; i <= 16; i++)
		{
			glVertex3f(
				x + (radius * cos(i *  twicePi / 16)),
			    y + (radius * sin(i * twicePi / 16)),
				z
			);
		}
	glEnd();
}

//----------------------------------------------------------------------
// Function to draw cylinders
//----------------------------------------------------------------------
void drawCylinder(float x, float y, float z, float radiusbot, float radiustop, float height)
{
	float twicePi = 2.0f * M_PI;
	glBegin(GL_QUADS);
		for (int i = 0; i <= 16; i++)
		{
			glVertex3f(
				x + (radiusbot * cos(i *  twicePi / 16)),
			    y + (radiusbot * sin(i * twicePi / 16)),
				z
			);
			glVertex3f(
				x + (radiustop * cos(i *  twicePi / 16)),
			    y + (radiustop * sin(i * twicePi / 16)),
				z + height
			);
			glVertex3f(
				x + (radiustop * cos((i+1) *  twicePi / 16)),
			    y + (radiustop * sin((i+1) * twicePi / 16)),
				z + height
			);
			glVertex3f(
				x + (radiusbot * cos((i+1) *  twicePi / 16)),
			    y + (radiusbot * sin((i+1) * twicePi / 16)),
				z
			);
		}
	glEnd();
}

//----------------------------------------------------------------------
// Draw the entire scene
//
// We first update the camera location based on its distance from the
// origin and its direction.
//----------------------------------------------------------------------
void renderScene(void)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int i, j;

	// Clear color and depth buffers
	glClearColor(0, 0, 0, 1.0); // sky color is black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the camera centered at (x,y,z) and looking along directional
	// vector (lx, ly, 0), with the z-axis pointing up
	gluLookAt(
			x,      y,      2.7,
			x + lx, y + ly, 2.3,
			0.0,    0.0,    1.0);

	// Floor
	glColor3f(0.66, 0.5, 0.24);
	glBegin(GL_QUADS);
		glVertex3f(-2.5, -5, 0.0);
		glVertex3f(-2.5,  0, 0.0);
		glVertex3f( 4,  0, 0.0);
		glVertex3f( 4, -5, 0.0);
	glEnd();

	// Walls
	glColor3f(0.85,0.83,0.83);
	glBegin(GL_QUADS);
		glVertex3f(-2.5, 0, 0); // back wall
		glVertex3f( 4, 0, 0);
		glVertex3f( 4, 0, 4);
		glVertex3f(-2.5, 0, 4);

		glColor3f(0.8,0.77,0.77);
		glVertex3f(-2.5, 0, 0); // left wall
		glVertex3f(-2.5, -5, 0);
		glVertex3f(-2.5, -5, 4);
		glVertex3f(-2.5, 0, 4);
	glEnd();

	// Wall Trim
	glColor3f(0.62,0.46,0.3);
	glBegin(GL_QUADS);
		glVertex3f(-2.49, -0.01, 0);
		glVertex3f( 4, -0.01, 0);
		glVertex3f( 4, -0.01, 0.2);
		glVertex3f(-2.49, -0.01, 0.2);

		glColor3f(0.6,0.46,0.28);
		glVertex3f(-2.49, -0.01, 0);
		glVertex3f(-2.49, -5, 0);
		glVertex3f(-2.49, -5, 0.2);
		glVertex3f(-2.49, -0.01, 0.2);
	glEnd();

	// table shadows
	glColor4f(0.22,0.175,0.05,0.9);
	glBegin(GL_QUADS);
		glVertex3f(-1.0, 0, 0.01);
		glVertex3f( 2, 0, 0.01);
		glVertex3f( 1.3, -1.6, 0.01);
		glVertex3f(-1.3, -1.6, 0.01);

		glVertex3f(1.25, -1.4, 0.01);
		glVertex3f( 1.15, -1.4, 0.01);
		glVertex3f( 1.1, -1.81, 0.01);
		glVertex3f(1.2, -1.81, 0.01);

		glVertex3f(-1.25, -1.4, 0.01);
		glVertex3f( -1.15, -1.4, 0.01);
		glVertex3f( -1.1, -1.81, 0.01);
		glVertex3f(-1.2, -1.81, 0.01);
	glEnd();

	// wall and trim shadows
	glColor3f(0.5,0.38,0.23);
	glBegin(GL_QUADS);
		glVertex3f(-1.0, -0.02, 0);
		glVertex3f( 2, -0.02, 0);
		glVertex3f( 2, -0.02, 0.2);
		glVertex3f(-1.0, -0.02, 0.2);

		glColor3f(0.7,0.7,0.7);
		glVertex3f(-1.0, -0.02, 0.2);
		glVertex3f( 2, -0.02, 0.2);
		glVertex3f( 1.3, -0.02, 1.24);
		glVertex3f(-1.3, -0.02, 1.24);
	glEnd();


	// Tabletop
	glColor3f(0.27,0.175,0.05);
	glBegin(GL_QUADS);
		glVertex3f(-1.3, 0, 1.3);
		glVertex3f( 1.3, 0, 1.3);
		glVertex3f( 1.3, -2, 1.3);
		glVertex3f(-1.3, -2, 1.3);

		glColor3f(0.24,0.145,0.02);
		glVertex3f(-1.3, -2, 1.3);
		glVertex3f( 1.3, -2, 1.3);
		glVertex3f( 1.3, -2, 1.25);
		glVertex3f(-1.3, -2, 1.25);


		glVertex3f(-1.3, 0, 1.25);
		glVertex3f( 1.3, 0, 1.25);
		glVertex3f( 1.3, -2, 1.25);
		glVertex3f(-1.3, -2, 1.25);
	glEnd();


	// Table legs
	glColor3f(0.03,0.03,0.03);
	glBegin(GL_QUADS);
		// back left leg
		glVertex3f(-1.2, -0.1, 1.29);
		glVertex3f(-1.2, -0.2, 1.29);
		glVertex3f(-1.2, -0.2, 0.01);
		glVertex3f(-1.2, -0.1, 0.01);

		glVertex3f(-1.1, -0.1, 1.29);
		glVertex3f(-1.1, -0.2, 1.29);
		glVertex3f(-1.1, -0.2, 0.01);
		glVertex3f(-1.1, -0.1, 0.01);

		glVertex3f(-1.2, -0.1, 1.29);
		glVertex3f(-1.1, -0.1, 1.29);
		glVertex3f(-1.1, -0.1, 0.01);
		glVertex3f(-1.2, -0.1, 0.01);

		glVertex3f(-1.2, -0.2, 1.29);
		glVertex3f(-1.1, -0.2, 1.29);
		glVertex3f(-1.1, -0.2, 0.01);
		glVertex3f(-1.2, -0.2, 0.01);

		// front left leg
		glVertex3f(-1.2, -1.9, 1.29);
		glVertex3f(-1.2, -1.8, 1.29);
		glVertex3f(-1.2, -1.8, 0.01);
		glVertex3f(-1.2, -1.9, 0.01);

		glVertex3f(-1.1, -1.9, 1.29);
		glVertex3f(-1.1, -1.8, 1.29);
		glVertex3f(-1.1, -1.8, 0.01);
		glVertex3f(-1.1, -1.9, 0.01);

		glVertex3f(-1.2, -1.9, 1.29);
		glVertex3f(-1.1, -1.9, 1.29);
		glVertex3f(-1.1, -1.9, 0.01);
		glVertex3f(-1.2, -1.9, 0.01);

		glVertex3f(-1.2, -1.8, 1.29);
		glVertex3f(-1.1, -1.8, 1.29);
		glVertex3f(-1.1, -1.8, 0.01);
		glVertex3f(-1.2, -1.8, 0.01);

		// back right leg
		glVertex3f(1.2, -0.1, 1.29);
		glVertex3f(1.2, -0.2, 1.29);
		glVertex3f(1.2, -0.2, 0.01);
		glVertex3f(1.2, -0.1, 0.01);

		glVertex3f(1.1, -0.1, 1.29);
		glVertex3f(1.1, -0.2, 1.29);
		glVertex3f(1.1, -0.2, 0.01);
		glVertex3f(1.1, -0.1, 0.01);

		glVertex3f(1.2, -0.1, 1.29);
		glVertex3f(1.1, -0.1, 1.29);
		glVertex3f(1.1, -0.1, 0.01);
		glVertex3f(1.2, -0.1, 0.01);

		glVertex3f(1.2, -0.2, 1.29);
		glVertex3f(1.1, -0.2, 1.29);
		glVertex3f(1.1, -0.2, 0.01);
		glVertex3f(1.2, -0.2, 0.01);

		// front right leg
		glVertex3f(1.2, -1.9, 1.29);
		glVertex3f(1.2, -1.8, 1.29);
		glVertex3f(1.2, -1.8, 0.01);
		glVertex3f(1.2, -1.9, 0.01);

		glVertex3f(1.1, -1.9, 1.29);
		glVertex3f(1.1, -1.8, 1.29);
		glVertex3f(1.1, -1.8, 0.01);
		glVertex3f(1.1, -1.9, 0.01);

		glVertex3f(1.2, -1.9, 1.29);
		glVertex3f(1.1, -1.9, 1.29);
		glVertex3f(1.1, -1.9, 0.01);
		glVertex3f(1.2, -1.9, 0.01);

		glVertex3f(1.2, -1.8, 1.29);
		glVertex3f(1.1, -1.8, 1.29);
		glVertex3f(1.1, -1.8, 0.01);
		glVertex3f(1.2, -1.8, 0.01);
	glEnd();

	glColor3f(0.03,0.03,0.03);
	glBegin(GL_QUADS); // right side chair
		// front left leg
		glVertex3f(.75, -1.45, .7);
		glVertex3f(.75, -1.4, .7);
		glVertex3f(.75, -1.4, 0.01);
		glVertex3f(.75, -1.45, 0.01);

		glVertex3f(.7, -1.45, .7);
		glVertex3f(.7, -1.4, .7);
		glVertex3f(.7, -1.4, 0.01);
		glVertex3f(.7, -1.45, 0.01);

		glVertex3f(.7, -1.45, .7);
		glVertex3f(.7, -1.45, .7);
		glVertex3f(.7, -1.45, 0.01);
		glVertex3f(.75, -1.45, 0.01);

		glVertex3f(.75, -1.4, .7);
		glVertex3f(.7, -1.4, .7);
		glVertex3f(.7, -1.4, 0.01);
		glVertex3f(.75, -1.4, 0.01);

		// front right leg
		glVertex3f(1.55, -1.45, 1.1);
		glVertex3f(1.55, -1.4, 1.1);
		glVertex3f(1.55, -1.4, 0.01);
		glVertex3f(1.55, -1.45, 0.01);

		glVertex3f(1.5, -1.45, 1.1);
		glVertex3f(1.5, -1.4, 1.1);
		glVertex3f(1.5, -1.4, 0.01);
		glVertex3f(1.5, -1.45, 0.01);

		glVertex3f(1.5, -1.45, 1.1);
		glVertex3f(1.5, -1.45, 1.1);
		glVertex3f(1.5, -1.45, 0.01);
		glVertex3f(1.55, -1.45, 0.01);

		glVertex3f(1.55, -1.4, 1.1);
		glVertex3f(1.5, -1.4, 1.1);
		glVertex3f(1.5, -1.4, 0.01);
		glVertex3f(1.55, -1.4, 0.01);

		// top right leg
		glVertex3f(1.55, -1.45, 1.8); // right
		glVertex3f(1.55, -1.4, 1.8);
		glVertex3f(1.55, -1.4, 1.1);
		glVertex3f(1.55, -1.45, 1.1);

		glVertex3f(1.5, -1.45, 1.8); // left
		glVertex3f(1.5, -1.4, 1.8);
		glVertex3f(1.5, -1.4, 1.1);
		glVertex3f(1.5, -1.45, 1.1);

		glVertex3f(1.5, -1.45, 1.8);
		glVertex3f(1.5, -1.45, 1.8); // front
		glVertex3f(1.5, -1.45, 1.1);
		glVertex3f(1.55, -1.45, 1.1);

		glVertex3f(1.55, -1.4, 1.8);
		glVertex3f(1.5, -1.4, 1.8); // back
		glVertex3f(1.5, -1.4, 1.1);
		glVertex3f(1.55, -1.4, 1.1);

		// top back post
		glVertex3f(1.55, -.45, 1.7); // right
		glVertex3f(1.55, -1.4, 1.7);
		glVertex3f(1.55, -1.4, 1.5);
		glVertex3f(1.55, -.45, 1.5);

		glVertex3f(1.5, -.45, 1.7); // left
		glVertex3f(1.5, -1.4, 1.7);
		glVertex3f(1.5, -1.4, 1.5);
		glVertex3f(1.5, -.45, 1.5);

		glVertex3f(1.5, -.45, 1.7);
		glVertex3f(1.5, -.45, 1.7); // front
		glVertex3f(1.5, -.45, 1.5);
		glVertex3f(1.55, -.45, 1.5);

		glVertex3f(1.55, -1.4, 1.7);
		glVertex3f(1.5, -1.4, 1.7); // back
		glVertex3f(1.5, -1.4, 1.5);
		glVertex3f(1.55, -1.4, 1.5);

		glVertex3f(1.5, -.45, 1.7); // top
		glVertex3f(1.5, -1.4, 1.7);
		glVertex3f(1.555, -1.4, 1.7);
		glVertex3f(1.55, -.45, 1.7);

		// top back post 2
		glVertex3f(1.55, -.45, 1.3); // right
		glVertex3f(1.55, -1.4, 1.3);
		glVertex3f(1.55, -1.4, 1.2);
		glVertex3f(1.55, -.45, 1.2);

		glVertex3f(1.5, -.45, 1.3); // left
		glVertex3f(1.5, -1.4, 1.3);
		glVertex3f(1.5, -1.4, 1.2);
		glVertex3f(1.5, -.45, 1.2);

		glVertex3f(1.5, -.45, 1.3);
		glVertex3f(1.5, -.45, 1.3); // front
		glVertex3f(1.5, -.45, 1.2);
		glVertex3f(1.55, -.45, 1.2);

		glVertex3f(1.55, -1.4, 1.3);
		glVertex3f(1.5, -1.4, 1.3); // back
		glVertex3f(1.5, -1.4, 1.2);
		glVertex3f(1.55, -1.4, 1.2);

		glVertex3f(1.5, -.45, 1.3); // top
		glVertex3f(1.5, -1.4, 1.3);
		glVertex3f(1.555, -1.4, 1.3);
		glVertex3f(1.55, -.45, 1.3);

		// top back post 3
		glColor3f(0.03,0.03,0.03);
		glVertex3f(1.55, -.45, 1); // right
		glVertex3f(1.55, -1.4, 1);
		glVertex3f(1.55, -1.4, .9);
		glVertex3f(1.55, -.45, .9);

		glVertex3f(1.5, -.45, 1); // left
		glVertex3f(1.5, -1.4, 1);
		glVertex3f(1.5, -1.4, .9);
		glVertex3f(1.5, -.45, .9);

		glVertex3f(1.5, -.45, 1);
		glVertex3f(1.5, -.45, 1); // front
		glVertex3f(1.5, -.45, .9);
		glVertex3f(1.55, -.45, .9);

		glVertex3f(1.55, -1.4, 1);
		glVertex3f(1.5, -1.4, 1); // back
		glVertex3f(1.5, -1.4, .9);
		glVertex3f(1.55, -1.4, .9);

		glVertex3f(1.5, -.45, 1); // top
		glVertex3f(1.5, -1.4, 1);
		glVertex3f(1.55, -1.4, 1);
		glVertex3f(1.55, -.45, 1);

		// bottom support1
		glVertex3f(1.55, -.45, .5); // right
		glVertex3f(1.55, -1.4, .5);
		glVertex3f(1.55, -1.4, .45);
		glVertex3f(1.55, -.45, .45);

		glVertex3f(1.5, -.45, .5); // left
		glVertex3f(1.5, -1.4, .5);
		glVertex3f(1.5, -1.4, .45);
		glVertex3f(1.5, -.45, .45);

		glVertex3f(1.5, -.45, .5);
		glVertex3f(1.5, -.45, .5); // front
		glVertex3f(1.5, -.45, .45);
		glVertex3f(1.55, -.45, .45);

		glVertex3f(1.55, -1.4, .5);
		glVertex3f(1.5, -1.4, .5); // back
		glVertex3f(1.5, -1.4, .45);
		glVertex3f(1.55, -1.4, .45);

		glVertex3f(1.5, -.45, .5); // top
		glVertex3f(1.5, -1.4, .5);
		glVertex3f(1.55, -1.4, .5);
		glVertex3f(1.55, -.45, .5);

		// bottom support1
		glVertex3f(.75, -.45, .5); // right
		glVertex3f(.75, -1.4, .5);
		glVertex3f(.75, -1.4, .45);
		glVertex3f(.75, -.45, .45);

		glVertex3f(.7, -.45, .5); // left
		glVertex3f(.7, -1.4, .5);
		glVertex3f(.7, -1.4, .45);
		glVertex3f(.7, -.45, .45);

		glVertex3f(.7, -.45, .5);
		glVertex3f(.7, -.45, .5); // front
		glVertex3f(.7, -.45, .45);
		glVertex3f(.75, -.45, .45);

		glVertex3f(.75, -1.4, .5);
		glVertex3f(.7, -1.4, .5); // back
		glVertex3f(.7, -1.4, .45);
		glVertex3f(.75, -1.4, .45);

		glVertex3f(.7, -.45, .5); // top
		glVertex3f(.7, -1.4, .5);
		glVertex3f(.75, -1.4, .5);
		glVertex3f(.75, -.45, .5);

		// bottom support3
		glVertex3f(1.5, -.45, .5); // right
		glVertex3f(1.5, -.5, .5);
		glVertex3f(1.5, -.5, .45);
		glVertex3f(1.5, -.45, .45);

		glVertex3f(.75, -.45, .5); // left
		glVertex3f(.75, -.5, .5);
		glVertex3f(.75, -.5, .45);
		glVertex3f(.75, -.45, .45);

		glVertex3f(.75, -.45, .5);
		glVertex3f(.75, -.45, .5); // front
		glVertex3f(.75, -.45, .45);
		glVertex3f(1.5, -.45, .45);

		glVertex3f(1.5, -.5, .5);
		glVertex3f(.75, -.5, .5); // back
		glVertex3f(.75, -.5, .45);
		glVertex3f(1.5, -.5, .45);

		glVertex3f(.75, -.45, .5); // top
		glVertex3f(.75, -.5, .5);
		glVertex3f(1.5, -.5, .5);
		glVertex3f(1.5, -.45, .5);

		// bottom support3
		glVertex3f(1.5, -1.45, .5); // right
		glVertex3f(1.5, -1.5, .5);
		glVertex3f(1.5, -1.5, .45);
		glVertex3f(1.5, -1.45, .45);

		glVertex3f(.75, -1.45, .5); // left
		glVertex3f(.75, -1.5, .5);
		glVertex3f(.75, -1.5, .45);
		glVertex3f(.75, -1.45, .45);

		glVertex3f(.75, -1.45, .5);
		glVertex3f(.75, -1.45, .5); // front
		glVertex3f(.75, -1.45, .45);
		glVertex3f(1.5, -1.45, .45);

		glVertex3f(1.5, -1.5, .5);
		glVertex3f(.75, -1.5, .5); // back
		glVertex3f(.75, -1.5, .45);
		glVertex3f(1.5, -1.5, .45);

		glVertex3f(.75, -1.45, .5); // top
		glVertex3f(.75, -1.5, .5);
		glVertex3f(1.5, -1.5, .5);
		glVertex3f(1.5, -1.45, .5);

		glVertex3f(.75, -1.45, .5); // bottom
		glVertex3f(.75, -1.5, .5);
		glVertex3f(1.5, -1.5, .5);
		glVertex3f(1.5, -1.45, .5);


		// front back left leg
		glVertex3f(.75, -.45, .7);
		glVertex3f(.75, -.4, .7);
		glVertex3f(.75, -.4, 0.01);
		glVertex3f(.75, -.45, 0.01);

		glVertex3f(.7, -.45, .7);
		glVertex3f(.7, -.4, .7);
		glVertex3f(.7, -.4, 0.01);
		glVertex3f(.7, -.45, 0.01);

		glVertex3f(.7, -.45, .7);
		glVertex3f(.7, -.45, .7);
		glVertex3f(.7, -.45, 0.01);
		glVertex3f(.75, -.45, 0.01);

		glVertex3f(.75, -.4, .7);
		glVertex3f(.7, -.4, .7);
		glVertex3f(.7, -.4, 0.01);
		glVertex3f(.75, -.4, 0.01);

		// front back right leg top
		glVertex3f(1.55, -.45, .7);
		glVertex3f(1.55, -.4, .7);
		glVertex3f(1.55, -.4, 1.1);
		glVertex3f(1.55, -.45, 1.1);

		glVertex3f(1.5, -.45, .7);
		glVertex3f(1.5, -.4, .7);
		glVertex3f(1.5, -.4, 1.1);
		glVertex3f(1.5, -.45, 1.1);

		glVertex3f(1.5, -.45, .7);
		glVertex3f(1.5, -.45, .7);
		glVertex3f(1.5, -.45, 1.1);
		glVertex3f(1.55, -.45, 1.1);

		glVertex3f(1.55, -.4, .7);
		glVertex3f(1.5, -.4, .7);
		glVertex3f(1.5, -.4, 1.1);
		glVertex3f(1.55, -.4, 1.1);

		// front back right leg
		glVertex3f(1.55, -.45, 1.8);
		glVertex3f(1.55, -.4, 1.8);
		glVertex3f(1.55, -.4, .01);
		glVertex3f(1.55, -.45, .01);

		glVertex3f(1.5, -.45, 1.8);
		glVertex3f(1.5, -.4, 1.8);
		glVertex3f(1.5, -.4, .01);
		glVertex3f(1.5, -.45, .01);

		glVertex3f(1.5, -.45, 1.8);
		glVertex3f(1.5, -.45, 1.8);
		glVertex3f(1.5, -.45, .01);
		glVertex3f(1.55, -.45, .01);

		glVertex3f(1.55, -.4, 1.8);
		glVertex3f(1.5, -.4, 1.8);
		glVertex3f(1.5, -.4, 0.01);
		glVertex3f(1.55, -.4, 0.01);

		// cushion
		glColor3f(0.27,0.135,0.05);
		glVertex3f(.7, -.4, .75);  // left side
		glVertex3f(.7, -1.45, .75);
		glVertex3f(.7, -1.45, 0.7);
		glVertex3f(.7, -.4, 0.7);

		glVertex3f(1.5, -1.45, .75);
		glVertex3f(1.5, -.4, 0.75);
		glVertex3f(.7, -.4, .75); // top
		glVertex3f(.7, -1.45, 0.75);

		glVertex3f(1.5, -1.45, .7);
		glVertex3f(1.5, -.4, 0.7);
		glVertex3f(.7, -.4, .7); // bottom
		glVertex3f(.7, -1.45, 0.7);



		glVertex3f(1.5, -.4, .75); // right side
		glVertex3f(1.5, -1.45, .75);
		glVertex3f(1.5, -1.45, 0.7);
		glVertex3f(1.5, -.4, 0.7);

		glVertex3f(1.5, -.4, 0.75);
		glVertex3f(.7, -.4, .75); // back face
		glVertex3f(.7, -.4, .7);
		glVertex3f(1.5, -.4, 0.7);

		glVertex3f(1.5, -1.45, 0.75);
		glVertex3f(.7, -1.45, .75); // front face
		glVertex3f(.7, -1.45, .7);
		glVertex3f(1.5, -1.45, 0.7);


	glBegin(GL_QUADS); // left side chair
		// front left leg
		glColor3f(0.03,0.03,0.03);
		glVertex3f(-.75, -1.45, .7);
		glVertex3f(-.75, -1.4, .7);
		glVertex3f(-.75, -1.4, 0.01);
		glVertex3f(-.75, -1.45, 0.01);

		glVertex3f(-.7, -1.45, .7);
		glVertex3f(-.7, -1.4, .7);
		glVertex3f(-.7, -1.4, 0.01);
		glVertex3f(-.7, -1.45, 0.01);

		glVertex3f(-.7, -1.45, .7);
		glVertex3f(-.7, -1.45, .7);
		glVertex3f(-.7, -1.45, 0.01);
		glVertex3f(-.75, -1.45, 0.01);

		glVertex3f(-.75, -1.4, .7);
		glVertex3f(-.7, -1.4, .7);
		glVertex3f(-.7, -1.4, 0.01);
		glVertex3f(-.75, -1.4, 0.01);



		// front right leg
		glVertex3f(-1.55, -1.45, 1.1);
		glVertex3f(-1.55, -1.4, 1.1);
		glVertex3f(-1.55, -1.4, 0.01);
		glVertex3f(-1.55, -1.45, 0.01);

		glVertex3f(-1.5, -1.45, 1.1);
		glVertex3f(-1.5, -1.4, 1.1);
		glVertex3f(-1.5, -1.4, 0.01);
		glVertex3f(-1.5, -1.45, 0.01);

		glVertex3f(-1.5, -1.45, 1.1);
		glVertex3f(-1.5, -1.45, 1.1);
		glVertex3f(-1.5, -1.45, 0.01);
		glVertex3f(-1.55, -1.45, 0.01);

		glVertex3f(-1.55, -1.4, 1.1);
		glVertex3f(-1.5, -1.4, 1.1);
		glVertex3f(-1.5, -1.4, 0.01);
		glVertex3f(-1.55, -1.4, 0.01);

		// top right leg
		glVertex3f(-1.55, -1.45, 1.8); // right
		glVertex3f(-1.55, -1.4, 1.8);
		glVertex3f(-1.55, -1.4, 1.1);
		glVertex3f(-1.55, -1.45, 1.1);

		glVertex3f(-1.5, -1.45, 1.8); // left
		glVertex3f(-1.5, -1.4, 1.8);
		glVertex3f(-1.5, -1.4, 1.1);
		glVertex3f(-1.5, -1.45, 1.1);

		glVertex3f(-1.5, -1.45, 1.8);
		glVertex3f(-1.5, -1.45, 1.8); // front
		glVertex3f(-1.5, -1.45, 1.1);
		glVertex3f(-1.55, -1.45, 1.1);

		glVertex3f(-1.55, -1.4, 1.8);
		glVertex3f(-1.5, -1.4, 1.8); // back
		glVertex3f(-1.5, -1.4, 1.1);
		glVertex3f(-1.55, -1.4, 1.1);

		// top back post
		glVertex3f(-1.55, -.45, 1.7); // right
		glVertex3f(-1.55, -1.4, 1.7);
		glVertex3f(-1.55, -1.4, 1.5);
		glVertex3f(-1.55, -.45, 1.5);

		glVertex3f(-1.5, -.45, 1.7); // left
		glVertex3f(-1.5, -1.4, 1.7);
		glVertex3f(-1.5, -1.4, 1.5);
		glVertex3f(-1.5, -.45, 1.5);

		glVertex3f(-1.5, -.45, 1.7);
		glVertex3f(-1.5, -.45, 1.7); // front
		glVertex3f(-1.5, -.45, 1.5);
		glVertex3f(-1.55, -.45, 1.5);

		glVertex3f(-1.55, -1.4, 1.7);
		glVertex3f(-1.5, -1.4, 1.7); // back
		glVertex3f(-1.5, -1.4, 1.5);
		glVertex3f(-1.55, -1.4, 1.5);

		glVertex3f(-1.5, -.45, 1.7); // top
		glVertex3f(-1.5, -1.4, 1.7);
		glVertex3f(-1.555, -1.4, 1.7);
		glVertex3f(-1.55, -.45, 1.7);

		// top back post 2
		glVertex3f(-1.55, -.45, 1.3); // right
		glVertex3f(-1.55, -1.4, 1.3);
		glVertex3f(-1.55, -1.4, 1.2);
		glVertex3f(-1.55, -.45, 1.2);

		glVertex3f(-1.5, -.45, 1.3); // left
		glVertex3f(-1.5, -1.4, 1.3);
		glVertex3f(-1.5, -1.4, 1.2);
		glVertex3f(-1.5, -.45, 1.2);

		glVertex3f(-1.5, -.45, 1.3);
		glVertex3f(-1.5, -.45, 1.3); // front
		glVertex3f(-1.5, -.45, 1.2);
		glVertex3f(-1.55, -.45, 1.2);

		glVertex3f(-1.55, -1.4, 1.3);
		glVertex3f(-1.5, -1.4, 1.3); // back
		glVertex3f(-1.5, -1.4, 1.2);
		glVertex3f(-1.55, -1.4, 1.2);

		glVertex3f(-1.5, -.45, 1.3); // top
		glVertex3f(-1.5, -1.4, 1.3);
		glVertex3f(-1.555, -1.4, 1.3);
		glVertex3f(-1.55, -.45, 1.3);

		// top back post 3
		glColor3f(0.03,0.03,0.03);
		glVertex3f(-1.55, -.45, 1); // right
		glVertex3f(-1.55, -1.4, 1);
		glVertex3f(-1.55, -1.4, .9);
		glVertex3f(-1.55, -.45, .9);

		glVertex3f(-1.5, -.45, 1); // left
		glVertex3f(-1.5, -1.4, 1);
		glVertex3f(-1.5, -1.4, .9);
		glVertex3f(-1.5, -.45, .9);

		glVertex3f(-1.5, -.45, 1);
		glVertex3f(-1.5, -.45, 1); // front
		glVertex3f(-1.5, -.45, .9);
		glVertex3f(-1.55, -.45, .9);

		glVertex3f(-1.55, -1.4, 1);
		glVertex3f(-1.5, -1.4, 1); // back
		glVertex3f(-1.5, -1.4, .9);
		glVertex3f(-1.55, -1.4, .9);

		glVertex3f(-1.5, -.45, 1); // top
		glVertex3f(-1.5, -1.4, 1);
		glVertex3f(-1.55, -1.4, 1);
		glVertex3f(-1.55, -.45, 1);

		// bottom support1
		glVertex3f(-1.55, -.45, .5); // right
		glVertex3f(-1.55, -1.4, .5);
		glVertex3f(-1.55, -1.4, .45);
		glVertex3f(-1.55, -.45, .45);

		glVertex3f(-1.5, -.45, .5); // left
		glVertex3f(-1.5, -1.4, .5);
		glVertex3f(-1.5, -1.4, .45);
		glVertex3f(-1.5, -.45, .45);

		glVertex3f(-1.5, -.45, .5);
		glVertex3f(-1.5, -.45, .5); // front
		glVertex3f(-1.5, -.45, .45);
		glVertex3f(-1.55, -.45, .45);

		glVertex3f(-1.55, -1.4, .5);
		glVertex3f(-1.5, -1.4, .5); // back
		glVertex3f(-1.5, -1.4, .45);
		glVertex3f(-1.55, -1.4, .45);

		glVertex3f(-1.5, -.45, .5); // top
		glVertex3f(-1.5, -1.4, .5);
		glVertex3f(-1.55, -1.4, .5);
		glVertex3f(-1.55, -.45, .5);

		// bottom support1
		glVertex3f(-.75, -.45, .5); // right
		glVertex3f(-.75, -1.4, .5);
		glVertex3f(-.75, -1.4, .45);
		glVertex3f(-.75, -.45, .45);

		glVertex3f(-.7, -.45, .5); // left
		glVertex3f(-.7, -1.4, .5);
		glVertex3f(-.7, -1.4, .45);
		glVertex3f(-.7, -.45, .45);

		glVertex3f(-.7, -.45, .5);
		glVertex3f(-.7, -.45, .5); // front
		glVertex3f(-.7, -.45, .45);
		glVertex3f(-.75, -.45, .45);

		glVertex3f(-.75, -1.4, .5);
		glVertex3f(-.7, -1.4, .5); // back
		glVertex3f(-.7, -1.4, .45);
		glVertex3f(-.75, -1.4, .45);

		glVertex3f(-.7, -.45, .5); // top
		glVertex3f(-.7, -1.4, .5);
		glVertex3f(-.75, -1.4, .5);
		glVertex3f(-.75, -.45, .5);

		// bottom support3
		glVertex3f(-1.5, -.45, .5); // right
		glVertex3f(-1.5, -.5, .5);
		glVertex3f(-1.5, -.5, .45);
		glVertex3f(-1.5, -.45, .45);

		glVertex3f(-.75, -.45, .5); // left
		glVertex3f(-.75, -.5, .5);
		glVertex3f(-.75, -.5, .45);
		glVertex3f(-.75, -.45, .45);

		glVertex3f(-.75, -.45, .5);
		glVertex3f(-.75, -.45, .5); // front
		glVertex3f(-.75, -.45, .45);
		glVertex3f(-1.5, -.45, .45);

		glVertex3f(-1.5, -.5, .5);
		glVertex3f(-.75, -.5, .5); // back
		glVertex3f(-.75, -.5, .45);
		glVertex3f(-1.5, -.5, .45);

		glVertex3f(-.75, -.45, .5); // top
		glVertex3f(-.75, -.5, .5);
		glVertex3f(-1.5, -.5, .5);
		glVertex3f(-1.5, -.45, .5);

		// bottom support3
		glVertex3f(-1.5, -1.45, .5); // right
		glVertex3f(-1.5, -1.5, .5);
		glVertex3f(-1.5, -1.5, .45);
		glVertex3f(-1.5, -1.45, .45);

		glVertex3f(-.75, -1.45, .5); // left
		glVertex3f(-.75, -1.5, .5);
		glVertex3f(-.75, -1.5, .45);
		glVertex3f(-.75, -1.45, .45);

		glVertex3f(-.75, -1.45, .5);
		glVertex3f(-.75, -1.45, .5); // front
		glVertex3f(-.75, -1.45, .45);
		glVertex3f(-1.5, -1.45, .45);

		glVertex3f(-1.5, -1.5, .5);
		glVertex3f(-.75, -1.5, .5); // back
		glVertex3f(-.75, -1.5, .45);
		glVertex3f(-1.5, -1.5, .45);

		glVertex3f(-.75, -1.45, .5); // top
		glVertex3f(-.75, -1.5, .5);
		glVertex3f(-1.5, -1.5, .5);
		glVertex3f(-1.5, -1.45, .5);

		glVertex3f(-.75, -1.45, .5); // bottom
		glVertex3f(-.75, -1.5, .5);
		glVertex3f(-1.5, -1.5, .5);
		glVertex3f(-1.5, -1.45, .5);


		// front back left leg
		glVertex3f(-.75, -.45, .7);
		glVertex3f(-.75, -.4, .7);
		glVertex3f(-.75, -.4, 0.01);
		glVertex3f(-.75, -.45, 0.01);

		glVertex3f(-.7, -.45, .7);
		glVertex3f(-.7, -.4, .7);
		glVertex3f(-.7, -.4, 0.01);
		glVertex3f(-.7, -.45, 0.01);

		glVertex3f(-.7, -.45, .7);
		glVertex3f(-.7, -.45, .7);
		glVertex3f(-.7, -.45, 0.01);
		glVertex3f(-.75, -.45, 0.01);

		glVertex3f(-.75, -.4, .7);
		glVertex3f(-.7, -.4, .7);
		glVertex3f(-.7, -.4, 0.01);
		glVertex3f(-.75, -.4, 0.01);

		// front back right leg top
		glVertex3f(-1.55, -.45, .7);
		glVertex3f(-1.55, -.4, .7);
		glVertex3f(-1.55, -.4, 1.1);
		glVertex3f(-1.55, -.45, 1.1);

		glVertex3f(-1.5, -.45, .7);
		glVertex3f(-1.5, -.4, .7);
		glVertex3f(-1.5, -.4, 1.1);
		glVertex3f(-1.5, -.45, 1.1);

		glVertex3f(-1.5, -.45, .7);
		glVertex3f(-1.5, -.45, .7);
		glVertex3f(-1.5, -.45, 1.1);
		glVertex3f(-1.55, -.45, 1.1);

		glVertex3f(-1.55, -.4, .7);
		glVertex3f(-1.5, -.4, .7);
		glVertex3f(-1.5, -.4, 1.1);
		glVertex3f(-1.55, -.4, 1.1);

		// front back right leg
		glVertex3f(-1.55, -.45, 1.8);
		glVertex3f(-1.55, -.4, 1.8);
		glVertex3f(-1.55, -.4, .01);
		glVertex3f(-1.55, -.45, .01);

		glVertex3f(-1.5, -.45, 1.8);
		glVertex3f(-1.5, -.4, 1.8);
		glVertex3f(-1.5, -.4, .01);
		glVertex3f(-1.5, -.45, .01);

		glVertex3f(-1.5, -.45, 1.8);
		glVertex3f(-1.5, -.45, 1.8);
		glVertex3f(-1.5, -.45, .01);
		glVertex3f(-1.55, -.45, .01);

		glVertex3f(-1.55, -.4, 1.8);
		glVertex3f(-1.5, -.4, 1.8);
		glVertex3f(-1.5, -.4, 0.01);
		glVertex3f(-1.55, -.4, 0.01);

		// cushion
		glColor3f(0.27,0.135,0.05);
		glVertex3f(-.7, -.4, .75);  // left side
		glVertex3f(-.7, -1.45, .75);
		glVertex3f(-.7, -1.45, 0.7);
		glVertex3f(-.7, -.4, 0.7);

		glVertex3f(-1.5, -1.45, .75);
		glVertex3f(-1.5, -.4, 0.75);
		glVertex3f(-.7, -.4, .75); // top
		glVertex3f(-.7, -1.45, 0.75);

		glVertex3f(-1.5, -1.45, .7);
		glVertex3f(-1.5, -.4, 0.7);
		glVertex3f(-.7, -.4, .7); // bottom
		glVertex3f(-.7, -1.45, 0.7);

		glVertex3f(-1.5, -.4, .75); // right side
		glVertex3f(-1.5, -1.45, .75);
		glVertex3f(-1.5, -1.45, 0.7);
		glVertex3f(-1.5, -.4, 0.7);

		glVertex3f(-1.5, -.4, 0.75);
		glVertex3f(-.7, -.4, .75); // back face
		glVertex3f(-.7, -.4, .7);
		glVertex3f(-1.5, -.4, 0.7);

		glVertex3f(-1.5, -1.45, 0.75);
		glVertex3f(-.7, -1.45, .75); // front face
		glVertex3f(-.7, -1.45, .7);
		glVertex3f(-1.5, -1.45, 0.7);
	glEnd();

	// left plate
	glColor3f(0.8, 0.8, 0.8);
	drawCircle(-0.9, -1, 1.31, 0.3);

	// right plate
	glColor3f(0.86, 0.86, 0.86);
	drawCircle(0.9, -1, 1.31, 0.27);

	// paper towels
	glColor3f (0.05, 0.05, 0.05);
	drawCircle(0.1, -0.25, 1.31, 0.23); // bottom of paper towel stand
	drawCylinder(0.1, -0.25, 1.32, 0.025, 0.025, 0.79); // pole on towel stand
	drawCircle(0.1, -0.25, 2.11, 0.025);
	glColor3f(0.9, 0.9, 0.9);
	drawCylinder(0.1, -0.25, 1.32, 0.18, 0.18, 0.7); // paper towel roll
	drawCircle(0.1, -0.25, 2.02, 0.18);
	glColor3f(0.27,0.175,0.05);
	drawCircle(0.1, -0.25, 2.02, 0.05);
	glColor3f(0.85,0.85,0.85);
	glBegin(GL_QUADS);
		glVertex3f(-0.08, -0.25, 1.32);
		glVertex3f(-0.08, -0.25, 2.02);
		glVertex3f(-0.02, -0.58, 2.02);
		glVertex3f(-0.02, -0.58, 1.32);
	glEnd();


	// red cup
	glColor3f(0.83, 0, 0);
	drawCylinder(-0.5, -1.3, 1.31, 0.08, 0.12, 0.45);
	glColor3f(0.78, 0, 0);
	drawCylinder(-0.5, -1.3, 1.32, 0.079, 0.119, 0.45);

	// pink cup
	glColor3f(0.83, 0.4, 0.75);
	drawCylinder(0.58, -0.74, 1.31, 0.08, 0.12, 0.45);
	glColor3f(0.86, 0.43, 0.78);
	drawCylinder(0.58, -0.74, 1.32, 0.079, 0.119, 0.45);

	// knife (left side of table)
	glColor3f(0.65, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1.18, -1.45, 1.31); // handle
		glVertex3f(-0.87, -1.45, 1.31);
		glVertex3f(-0.87, -1.48, 1.31);
		glVertex3f(-1.18, -1.48, 1.31);

		glColor3f(0.8, 0.8, 0.8);
		glVertex3f(-0.87, -1.45, 1.31); // knife blade
		glVertex3f(-0.6, -1.46, 1.31);
		glVertex3f(-0.6, -1.48, 1.31);
		glVertex3f(-0.87, -1.48, 1.31);
	glEnd();



	// spoon (left side of table)
	glColor3f(0.65, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(-1.15, -1.53, 1.31); // handle
		glVertex3f(-0.9, -1.53, 1.31);
		glVertex3f(-0.9, -1.56, 1.31);
		glVertex3f(-1.15, -1.56, 1.31);

		glColor3f(0.8, 0.8, 0.8);
		glVertex3f(-0.9, -1.53, 1.31); // spoon extension
		glVertex3f(-0.85, -1.53, 1.31);
		glVertex3f(-0.85, -1.56, 1.31);
		glVertex3f(-0.9, -1.56, 1.31);
	glEnd();
	drawCircle(-0.8, -1.545, 1.31, 0.05); // spoon head


	// fork (right side of table)
	glColor3f(0.65, 0, 0);
	glBegin(GL_QUADS);
		glVertex3f(1.18, -1.4, 1.31); // handle
		glVertex3f(0.87, -1.4, 1.31);
		glVertex3f(0.87, -1.43, 1.31);
		glVertex3f(1.18, -1.43, 1.31);

		glColor3f(0.8, 0.8, 0.8);
		glVertex3f(0.87, -1.4, 1.31); // fork extension
		glVertex3f(0.82, -1.4, 1.31);
		glVertex3f(0.82, -1.43, 1.31);
		glVertex3f(0.87, -1.43, 1.31);

		glVertex3f(0.82, -1.37, 1.31);
		glVertex3f(0.8, -1.37, 1.31);
		glVertex3f(0.8, -1.46, 1.31);
		glVertex3f(0.82, -1.46, 1.31);

		glVertex3f(0.8, -1.37, 1.31); // fork head
		glVertex3f(0.7, -1.37, 1.31);
		glVertex3f(0.7, -1.39, 1.31);
		glVertex3f(0.8, -1.39, 1.31);

		glVertex3f(0.8, -1.4, 1.31);
		glVertex3f(0.7, -1.4, 1.31);
		glVertex3f(0.7, -1.43, 1.31);
		glVertex3f(0.8, -1.43, 1.31);

		glVertex3f(0.8, -1.44, 1.31);
		glVertex3f(0.7, -1.44, 1.31);
		glVertex3f(0.7, -1.46, 1.31);
		glVertex3f(0.8, -1.46, 1.31);
	glEnd();

	glutSwapBuffers(); // Make it all visible
}


//----------------------------------------------------------------------
// Process mouse drag events
//
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
void mouseMove(int x, int y)
{
	if (isDragging) { // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = -sin(angle + deltaAngle);
		ly = cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			angle += deltaAngle; // update camera turning angle
			isDragging = 0; // no longer dragging
		}
	}
}

//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	printf("\n\
------------------------------------------------------------------------------\n\
  Rylan Casanova + Dylan Olthoff \n\
  CST-310 \n\
  Project 5\n\
  \n\
  \n\
  Drag the mouse left and right while holding left-click to move the camera\n\
------------------------------------------------------------------------------\n");

	// general initializations
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1008, 756);
	glutCreateWindow("Project 5 - Our Scene");

	// register callbacks
	glutReshapeFunc(changeSize); // window reshape callback
	glutDisplayFunc(renderScene); // (re)display callback
	glutIdleFunc(update); // incremental update
	glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
	glutMouseFunc(mouseButton); // process mouse button push/release
	glutMotionFunc(mouseMove); // process mouse dragging motion

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0; // this is just to keep the compiler happy
}
