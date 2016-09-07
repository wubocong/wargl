#include <iostream>
#include <vector>
#include <fstream>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

const GLdouble ww = 800, wh = 600;
GLdouble w = 800, h = 600;
vector<vector<pair<GLint, GLint>>> dinosaur;

void drawDinosaur() {
	for (int i = 0; i < dinosaur.size(); i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < dinosaur[i].size(); j++) {
			glVertex2i(dinosaur[i][j].first, dinosaur[i][j].second);
		}
		glEnd();
	}
}

void readFile() {
	fstream in;
	in.open("dino.dat");
	if (in.fail()) {
		return;
	}
	GLint polys, lines;
	in >> polys;
	for (int i = 0; i < polys; i++) {
		in >> lines;
		vector<pair<GLint, GLint>> poly;
		for (int j = 0; j < lines; j++) {
			GLint x, y;
			in >> x >> y;
			poly.push_back(make_pair(x, y));
		}
		dinosaur.push_back(poly);
	}
}

void setWorldWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);
}

void myInit() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(2.0);
	readFile();
}

void myDisplay() {
	setWorldWindow(0, w, 0, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawDinosaur();
	glFlush();
	w *= 0.95;
	h *= 0.95;
	Sleep(100);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("War Opengl Dinosaur");
	glutDisplayFunc(myDisplay);

	myInit();

	setWorldWindow(0, w, 0, h);
	glViewport(0, 0, 800, 600);

	glutMainLoop();

	return 0;
}