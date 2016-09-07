#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

void drawDinosaur() {
	fstream in;
	in.open("dino.dat");
	if (in.fail()) {
		return;
	}
	GLint polys, lines;
	in >> polys;
	for (int i = 0; i < polys; i++) {
		in >> lines;
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < lines; j++) {
			GLint x, y;
			in >> x >> y;
			glVertex2i(x, y);
		}
		glEnd();
	}
}

void myInit() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(2.0);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			glViewport(i * 80, j * 60, 80, 60);
			drawDinosaur();
		}
	}
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("War Opengl Dinosaur");
	glutDisplayFunc(myDisplay);

	myInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glViewport(0, 0, 800, 600);

	glutMainLoop();
	return 0;
}