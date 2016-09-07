#include <iostream>
#include <cmath>
#include <vector>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

const float PI = 3.141592653;
vector<pair<float, float>> oneEighthCircle;

void calcCircle(float radius) {
	for (float angle = 0; angle <= PI / 4; angle += 0.01) {
		oneEighthCircle.push_back(make_pair(radius*cos(angle), radius*sin(angle)));
	}
}

void myInit() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(2.0);
}

void drawCircle(float centerX, float centerY) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < oneEighthCircle.size(); i++) {
		glVertex2f(oneEighthCircle[i].first + centerX, oneEighthCircle[i].second + centerY);
	}
	for (int i = oneEighthCircle.size() - 1; i >= 0; i--) {
		glVertex2f(oneEighthCircle[i].second + centerX, oneEighthCircle[i].first + centerY);
	}
	for (int i = 0; i < oneEighthCircle.size(); i++) {
		glVertex2f(-oneEighthCircle[i].second + centerX, oneEighthCircle[i].first + centerY);
	}
	for (int i = oneEighthCircle.size() - 1; i >= 0; i--) {
		glVertex2f(-oneEighthCircle[i].first + centerX, oneEighthCircle[i].second + centerY);
	}
	for (int i = 0; i < oneEighthCircle.size(); i++) {
		glVertex2f(-oneEighthCircle[i].first + centerX, -oneEighthCircle[i].second + centerY);
	}
	for (int i = oneEighthCircle.size() - 1; i >= 0; i--) {
		glVertex2f(-oneEighthCircle[i].second + centerX, -oneEighthCircle[i].first + centerY);
	}
	for (int i = 0; i < oneEighthCircle.size(); i++) {
		glVertex2f(oneEighthCircle[i].second + centerX, -oneEighthCircle[i].first + centerY);
	}
	for (int i = oneEighthCircle.size() - 1; i >= 0; i--) {
		glVertex2f(oneEighthCircle[i].first + centerX, -oneEighthCircle[i].second + centerY);
	}
	glEnd();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawCircle(400.0, 300.0);
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("War Opengl Fight");

	calcCircle(100.0f);
	glutDisplayFunc(myDisplay);

	myInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glViewport(0, 0, 800, 600);

	glutMainLoop();

	return 0;
}