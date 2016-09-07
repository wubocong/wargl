#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

vector<pair<GLdouble, GLdouble>> polygon, point;

void readPolygonFile() {
	fstream in;
	in.open("polygon.dat");
	if (in.fail()) {
		return;
	}
	int n;
	in >> n;
	for (int i = 0; i < n; i++) {
		GLdouble x, y;
		in >> x >> y;
		polygon.push_back(make_pair(x, y));
	}
}
void readPointFile() {
	fstream in;
	in.open("point.dat");
	if (in.fail()) {
		return;
	}
	int n;
	in >> n;
	for (int i = 0; i < n; i++) {
		GLdouble x, y;
		in >> x >> y;
		point.push_back(make_pair(x, y));
	}
}

void drawDot(pair<GLdouble, GLdouble> point, bool isIn) {
	glBegin(GL_POINTS);
	if (isIn) {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	else {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
	glVertex2f(point.first, point.second);
	glEnd();
}

void drawPolygon() {
	glColor3f(0.0f, 0.0f, 1.0f);
	int size = polygon.size();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < size; i++) {
		glVertex2f(polygon[i].first, polygon[i].second);
	}
	glEnd();
}

bool floatEqual(GLdouble f1, GLdouble f2) {
	bool equal = (fabs(f1 - f2) >= 0.0000001) ? false : true;
	return equal;
}

bool calcIn(pair<GLdouble, GLdouble> point) {
	int size = polygon.size();
	int count = 0;
	for (int i = 0; i < size; i++) {
		pair<GLdouble, GLdouble>point1 = polygon[i];
		pair<GLdouble, GLdouble>point2;
		if (floatEqual(point1.first, point.first) && floatEqual(point1.second, point.second)) {
			return true;
		}
		if (i == size - 1) {
			point2 = polygon[0];
		}
		else {
			point2 = polygon[i + 1];
		}
		if (floatEqual(point1.first, point2.first)) {
			if ((point.second < point1.second&&point.second >= point2.second) || (point.second >= point1.second&&point.second < point2.second))
			{
				if (floatEqual(point1.first, point.first)) {
					return true;
				}
				if (point1.first < point.first) {
					count++;
				}
			}
		}
		else {
			GLdouble k = (point2.second - point1.second) / (point2.first - point1.first);
			if (k == 0) {
				continue;
			}
			GLdouble b = point1.second - point1.first*k;
			GLdouble xCross = (point.second - b) / k;
			if (floatEqual(xCross, point.first)) {
				return true;
			}
			if ((xCross < point.first) && (point.second < point1.second || point.second < point2.second) && ((xCross >= point1.first&&xCross <= point2.first) || (xCross <= point1.first&&xCross >= point2.first))) {
				count++;
			}
		}
	}
	return count % 2;
}

void drawDots() {
	int size = point.size();
	for (int i = 0; i < size; i++) {
		drawDot(point[i], calcIn(point[i]));
	}
}

void myInit() {
	glLineWidth(2.0);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(6.0);
	readPolygonFile();
	readPointFile();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawPolygon();
	drawDots();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("War Opengl InPolygon");

	myInit();

	glutDisplayFunc(myDisplay);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glViewport(0, 0, 800, 600);

	glutMainLoop();

	return 0;
}