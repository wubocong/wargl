#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

pair<GLdouble, GLdouble> operator-(const pair<GLdouble, GLdouble> &point1, const pair<GLdouble, GLdouble> &point2) {
	return make_pair(point1.first - point2.first, point1.second - point2.second);
}

pair<GLdouble, GLdouble> operator+(const pair<GLdouble, GLdouble> &point1, const pair<GLdouble, GLdouble> &point2) {
	return make_pair(point1.first + point2.first, point1.second + point2.second);
}

pair<GLdouble, GLdouble> operator*(const pair<GLdouble, GLdouble> &point1, const GLdouble &factor) {
	return make_pair(point1.first*factor, point1.second*factor);
}

pair<GLdouble, GLdouble> operator*(const GLdouble &factor, const pair<GLdouble, GLdouble> &point1) {
	return make_pair(point1.first*factor, point1.second*factor);
}

pair<GLdouble, GLdouble> operator/(const pair<GLdouble, GLdouble> &point1, const GLdouble &factor) {
	return make_pair(point1.first / factor, point1.second / factor);
}

vector<pair<GLdouble, GLdouble>> polygonPoint;
vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  lines, polygonLines;
vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  dashedLines;
vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  cutLines;

void readLineFile() {
	fstream in;
	in.open("line.dat");
	if (in.fail()) {
		return;
	}
	int n;
	in >> n;
	for (int i = 0; i < n; i++) {
		GLdouble x1, y1, x2, y2;
		in >> x1 >> y1 >> x2 >> y2;
		pair<GLdouble, GLdouble> p1 = make_pair(x1, y1);
		pair<GLdouble, GLdouble> p2 = make_pair(x2, y2);
		lines.push_back(make_pair(p1, p2 - p1));
	}
}

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
		polygonPoint.push_back(make_pair(x, y));
		if (i > 0) {
			polygonLines.push_back(make_pair(polygonPoint[i - 1], polygonPoint[i] - polygonPoint[i - 1]));
		}
	}
	polygonLines.push_back(make_pair(polygonPoint[n - 1], polygonPoint[0] - polygonPoint[n - 1]));
}

void drawLine(pair<GLdouble, GLdouble> point1, pair<GLdouble, GLdouble> point2) {
	glBegin(GL_LINE_STRIP);
	glVertex2f(point1.first, point1.second);
	glVertex2f(point2.first, point2.second);
	glEnd();
}

void drawDashedLine(pair<GLdouble, GLdouble> point1, pair<GLdouble, GLdouble> point2) {
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(point1.first, point1.second);
	glVertex2f(point2.first, point2.second);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

void drawDashedLines() {
	glColor3f(1.0f, 0.0f, 0.0f);
	int size = dashedLines.size();
	for (int i = 0; i < size; i++) {
		drawDashedLine(dashedLines[i].first, dashedLines[i].second);
	}
}

void drawLines() {
	glColor3f(0.0f, 0.0f, 1.0f);
	int size = cutLines.size();
	for (int i = 0; i < size; i++) {
		drawLine(cutLines[i].first, cutLines[i].second);
	}
}

void drawPolygon() {
	glColor3f(0.0f, 0.0f, 1.0f);
	int size = polygonPoint.size();
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < size; i++) {
		glVertex2f(polygonPoint[i].first, polygonPoint[i].second);
	}
	glEnd();
}

GLdouble secondOrderDeter(GLdouble a, GLdouble b, GLdouble c, GLdouble d) {
	return a*d - b*c;
}

pair<GLdouble, GLdouble> calcT(pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>  line1, pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>  line2) {
	GLdouble x1 = line1.first.first;
	GLdouble y1 = line1.first.second;
	GLdouble dx1 = line1.second.first;
	GLdouble dy1 = line1.second.second;
	GLdouble x2 = line2.first.first;
	GLdouble y2 = line2.first.second;
	GLdouble dx2 = line2.second.first;
	GLdouble dy2 = line2.second.second;
	return make_pair((secondOrderDeter(x2, y2, dx2, dy2) - secondOrderDeter(x1, y1, dx2, dy2)) / secondOrderDeter(dx1, dy1, dx2, dy2), (secondOrderDeter(x1, y1, dx1, dy1) - secondOrderDeter(x2, y2, dx1, dy1)) / secondOrderDeter(dx2, dy2, dx1, dy1));
}

bool floatEqual(GLdouble f1, GLdouble f2) {
	bool equal = (fabs(f1 - f2) >= 0.0000001) ? false : true;
	return equal;
}

void calcIn() {
	int polygonSize = polygonLines.size();
	int lineSize = lines.size();
	for (int i = 0; i < lineSize; i++) {
		vector<GLdouble> t;
		int count = 0;
		t.push_back(0);
		t.push_back(1);
		for (int j = 0; j < polygonSize; j++) {
			if (floatEqual(lines[i].second.first*polygonLines[j].second.second, lines[i].second.second*polygonLines[j].second.first)) {
				continue;
			}
			pair<GLdouble, GLdouble> result = calcT(lines[i], polygonLines[j]);

			int next;
			if (j == polygonSize - 1) {
				next = 0;
			}
			else {
				next = j + 1;
			}

			// count internal vertices once
			if ((result.second > 0 && result.second < 1) || (floatEqual(result.second, 0) && polygonPoint[j].second > polygonPoint[next].second) || (floatEqual(result.second, 1) && polygonPoint[next].second > polygonPoint[j].second)) {
				if (!floatEqual(result.first, 0) && result.first > 0) {
					count++;
					if (result.first < 1 && !floatEqual(result.first, 1)) {
						t.push_back(result.first);
					}
				}
			}
		}
		sort(t.begin(), t.end());
		bool in = count % 2;
		for (int k = 0; k < t.size() - 1; k++) {
			// ignore external vertices
			if (!floatEqual(t[k], t[k + 1])) {
				if (in) {
					cutLines.push_back(make_pair(lines[i].first + lines[i].second*t[k], lines[i].first + lines[i].second*t[k + 1]));
				}
				else {
					dashedLines.push_back(make_pair(lines[i].first + lines[i].second*t[k], lines[i].first + lines[i].second*t[k + 1]));
				}
			}
			in = !in;
		}
		t.clear();
	}
}

void myInit() {
	glLineWidth(2.0);
	readPolygonFile();
	readLineFile();
	calcIn();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawPolygon();
	drawLines();
	drawDashedLines();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("WarGL PolygonCutLine");

	myInit();

	glutDisplayFunc(myDisplay);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glViewport(0, 0, 800, 600);

	glutMainLoop();

	return 0;
}