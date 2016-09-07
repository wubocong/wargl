#include <iostream>
#include <vector>
#include <fstream>
#include <glew\glew.h>
#include <freeglut\freeglut.h>

using namespace std;

vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  lines;
vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  dashedLines;
vector<pair<pair<GLdouble, GLdouble>, pair<GLdouble, GLdouble>>>  cutLines;
GLdouble top, r, bottom, l;

void readFile() {
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
		lines.push_back(make_pair(make_pair(x1, y1), make_pair(x2, y2)));
	}
}
void calcV(int lineOrder, bool isFirst, GLdouble vPos) {
	pair<double, double> &first = isFirst ? cutLines[lineOrder].first : cutLines[lineOrder].second;
	pair<double, double> &second = isFirst ? cutLines[lineOrder].second : cutLines[lineOrder].first;
	GLdouble x = first.first;
	GLdouble y = first.second;
	first.first -= (first.first - second.first)*(first.second - vPos) / (first.second - second.second);
	first.second = vPos;
	dashedLines.push_back(make_pair(first, make_pair(x, y)));
}
void calcH(int lineOrder, bool isFirst, GLdouble hPos) {
	pair<double, double> &first = isFirst ? cutLines[lineOrder].first : cutLines[lineOrder].second;
	pair<double, double> &second = isFirst ? cutLines[lineOrder].second : cutLines[lineOrder].first;
	GLdouble x = first.first;
	GLdouble y = first.second;
	first.second -= (first.second - second.second)*(first.first - hPos) / (first.first - second.first);
	first.first = hPos;
	dashedLines.push_back(make_pair(first, make_pair(x, y)));
}

void drawLine(pair<GLdouble, GLdouble> point1, pair<GLdouble, GLdouble> point2) {
	glBegin(GL_LINE_STRIP);
	glVertex2f(point1.first, point1.second);
	glVertex2f(point2.first, point2.second);
	glEnd();
}

void drawDashedLine(pair<GLdouble, GLdouble> point1, pair<GLdouble, GLdouble> point2) {
	//glPushAttrib(GL_ENABLE_BIT);
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(point1.first, point1.second);
	glVertex2f(point2.first, point2.second);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	//glPopAttrib();
}
void drawDashedLines() {
	int size = dashedLines.size();
	for (int i = 0; i < size; i++) {
		drawDashedLine(dashedLines[i].first, dashedLines[i].second);
	}
}
void drawLines() {
	int size = cutLines.size();
	for (int i = 0; i < size; i++) {
		drawLine(cutLines[i].first, cutLines[i].second);
	}
}
int generatePos(GLdouble x, GLdouble y) {
	int pos = 0;
	if (y > top) {
		pos++;
	}
	pos <<= 1;
	if (x > r) {
		pos++;
	}
	pos <<= 1;
	if (y < bottom) {
		pos++;
	}
	pos <<= 1;
	if (x < l) {
		pos++;
	}
	return pos;
}

void cutCalc() {
	int size = lines.size();
	cutLines.swap(lines);
	for (int i = 0; i < size; i++) {
		int pos1 = generatePos(cutLines[i].first.first, cutLines[i].first.second);
		int pos2 = generatePos(cutLines[i].second.first, cutLines[i].second.second);
		if ((pos1 ^ pos2) == 0) {
			continue;
		}
		if (pos1 & 8) {
			calcV(i, true, top);
			pos1 = generatePos(cutLines[i].first.first, cutLines[i].first.second);
		}
		else {
			if (pos2 & 8) {
				calcV(i, false, top);
				pos2 = generatePos(cutLines[i].second.first, cutLines[i].second.second);
			}
		}
		if ((pos1 ^ pos2) == 0) {
			continue;
		}
		if (pos1 & 4) {
			calcH(i, true, r);
			pos1 = generatePos(cutLines[i].first.first, cutLines[i].first.second);
		}
		else {
			if (pos2 & 4) {
				calcH(i, false, r);
				pos2 = generatePos(cutLines[i].second.first, cutLines[i].second.second);
			}
		}
		if ((pos1 ^ pos2) == 0) {
			continue;
		}
		if (pos1 & 2) {
			calcV(i, true, bottom);
			pos1 = generatePos(cutLines[i].first.first, cutLines[i].first.second);
		}
		else {
			if (pos2 & 2) {
				calcV(i, false, bottom);
				pos2 = generatePos(cutLines[i].second.first, cutLines[i].second.second);
			}
		}
		if ((pos1 ^ pos2) == 0) {
			continue;
		}
		if (pos1 & 1) {
			calcH(i, true, l);
			pos1 = generatePos(cutLines[i].first.first, cutLines[i].first.second);
		}
		else {
			if (pos2 & 1) {
				calcH(i, false, l);
				pos2 = generatePos(cutLines[i].second.first, cutLines[i].second.second);
			}
		}
	}
}

void drawRectangle() {
	glBegin(GL_LINE_LOOP);
	glVertex2f(l, bottom);
	glVertex2f(l, top);
	glVertex2f(r, top);
	glVertex2f(r, bottom);
	glEnd();
}


void myInit() {
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(2.0);
	r = 600;
	l = 200;
	top = 400;
	bottom = 200;
	readFile();
	cutCalc();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawRectangle();
	drawLines();
	drawDashedLines();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("War Opengl Cut");

	myInit();

	glutDisplayFunc(myDisplay);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
	glViewport(0, 0, 800, 600);

	glutMainLoop();

	return 0;
}