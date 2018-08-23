#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <windows.h>
#include <Mmsystem.h>


//float ShipAng = 90;
int i;
float y = 0.1;
float Shipz = -5;
float WaitGirl = 0;
float GirlX = 0;
float keyX = 0;
float keyY = 0;
float GirlY = 0;
float GirlZ = 0;
float shipAng = 0;
float HandsGuy = 0;
float RJmp = 0;
float RJmp2 = 0;
float dressAng = 0;
float JArm = 0;
float JArm2 = 0;
float SecondJoint = 0;
float FirstJoint = 0;
bool dress = false;
bool going = true;
bool Up = true;
bool hands = false;
bool first = true;
bool Ice = false;
bool paused = false;
bool Jump = false;
bool clicked = false;
bool all = false;
bool Wood = false;
float SinkAng = 0;
float SinkPos = 0;
float WoodPos = -6;
float cam = 0;
float GuyPosY = 0;
float GuyPosX = 0;
bool down = true;
float GirlRot = 0;

float resShip[2] = { -5.5, 0 };
float p0[2] = { 0.01, 0.288 };
float p1[2] = { 0.115, 0.252 };
float p2[2] = { 0.101, 0.349 };
float p3[2] = { 0, 0.288 };
float t = 0;

float resIce[2] = { 3.5, 0 };
float I0[2] = { 0.687, 0.313 };
float I1[2] = { 0.613, 0.245 };
float I2[2] = { 0.650, 0.453 };
float I3[2] = { 0.658, 0.316 };
float I = 0;

float resJmp[2] = { 0, 0 };
float J0[2] = { 0.116, 0.292 };
float J1[2] = { 0.101, 0.445 };
float J2[2] = { 0.304, 0.475 };
float J3[2] = { 0.252, -1.5 };
float J = 0;

float resJmp2[2] = { 0, 0 };
float J22 = 0;

float resWood[2] = { 0, 0 };
float W0[2] = { 0.176, 0.209 };
float W1[2] = { 0.230, 0.138 };
float W2[2] = { 0.209, 0.288 };
float W3[2] = { 0.243, 0.206 };
float W = 0;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
void Timer(int value);
void TimerI(int value);
void TimerJ(int value);
void TimerW(int value);
void Anim();
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f &v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f &v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;
	
	Camera(float eyeX = 0.0f, float eyeY = 0.0f, float eyeZ = 2.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		if (clicked){
			eye.x = 2 * cos(DEG2RAD(cam));
			eye.z = 2 * sin(DEG2RAD(cam));
		}
		else{
			eye.x = 0;
			eye.z = 2;
		}
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
			);
	}
};

Camera camera;

void drawWall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5 * thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawJackPart() {
	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
}
void drawJack() {
	glPushMatrix();
	drawJackPart();
	glRotated(90.0, 0, 1, 0);
	drawJackPart();
	glRotated(90.0, 1, 0, 0);
	drawJackPart();
	glPopMatrix();
}
void drawTable(double topWid, double topThick, double legThick, double legLen){
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95*topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -0.0f, 0.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(110, 1700 / 900, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}



void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}
void drawHair(){
	glColor3f(1, 1, 0);
	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex3f(0, 0.1, 0);
	glVertex3f(0.03, 0.1, 0);

	glVertex3f(0.03, 0.1, 0);
	glVertex3f(0.035, 0, 0);

	glVertex3f(0.035, 0, 0);
	glVertex3f(0.03, -0.1, 0);
	
	glEnd();
}
int* bezier(float t, int* p0, int* p1, int* p2, int* p3)
{
	int res[2];
	res[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
	res[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
	return res;
}

void pyramid()
{
	glBegin(GL_TRIANGLE_FAN);     	// draw triangle
	//glColor3f(1, 0, 0);
	glVertex3f(-50.0f, 30, 0.0f);
	//glColor3f(0, 1, 0);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	//glColor3f(0, 0, 1);
	glVertex3f(50.0f, -50.0f, 50.0f);
	//glColor3f(1, 0, 0);
	glVertex3f(50.0f, -50.0f, -50.0f);
	//glColor3f(0, 1, 0);
	glVertex3f(-50.0f, -50.0f, -50.0f);
	//glColor3f(0, 0, 1);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	glEnd();
	glBegin(GL_QUADS);                           	// draw square
	//glColor3f(1, 0, 0);
	glVertex3f(-50.0f, -50.0f, 50.0f);
	//glColor3f(0, 1, 0);
	glVertex3f(-50.0f, -50.0f, -50.0f);
	//glColor3f(0, 0, 1);
	glVertex3f(50.0f, -50.0f, -50.0f);
	//glColor3f(1, 0, 0);
	glVertex3f(50.0f, -50.0f, 50.0f);
	glEnd();

}
void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if (all)
		glTranslatef(resShip[0] - 9.5, resShip[1], Shipz);
	glPushMatrix();
	glTranslatef(keyX, keyY, 0);
	if (!all)
		glTranslatef(resShip[0]-9.5, resShip[1], Shipz);
	glRotatef(shipAng, 0, 1, 0);
	//Rose Main Charachter
	glPushMatrix();
	//glRotatef(WaitGirl, 0, 1, 0);
	glTranslatef(-GirlX, -GirlY, -GirlZ);
	glTranslatef(resJmp[0], resJmp[1], RJmp);
	glScalef(0.5, 0.5,0.5);
	glTranslatef(3.7, 0.25, 0.9);
	glRotatef(GirlRot, 0, 0, 1);
		//DressJupe
	glPushMatrix();

	glTranslatef(0, -0.1, 0);
	
	glPushMatrix();
	glColor3f(1, 0, 0); 
	glTranslatef(0, -1, 0);
	glRotatef(dressAng, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.45, 0.55, 25, 25);
	glPopMatrix();
		//DressTop
	glPushMatrix();
	glColor3f(1, 0, 0);
	
	glTranslatef(0, -0.3, 0);
	glRotatef(-dressAng, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.2, 0.5, 25, 25);
	glPopMatrix();
	
		//DressStrings
	glPushMatrix();
	glColor3f(0.474, 0, 0);
	glTranslatef(0.11, -0.27, 0);
	glRotated(84, 0, 1, 0);
	glRotated(9, 1, 0, 0);
	
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0.1, 0.14, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.474, 0, 0);
	glTranslatef(-0.1, -0.3, 0);
	glRotated(93, 0, 1, 0);
	glRotated(-9, 1, 0, 0);
	
	GLUquadric *quadObj2 = gluNewQuadric();
	gluDisk(quadObj2, 0.1, 0.14, 50, 50);
	glPopMatrix();
		//RoseHead
	glPushMatrix();
	glColor3f(0.917, 0.753, 0.525);
	glTranslatef(0, -0.02, 0);
	glutSolidSphere(0.08, 25, 25);
	glPopMatrix();

		//ARMS
	glPushMatrix();
	glColor3f(0.917, 0.753, 0.525);
			//1stJoint
	
	glPushMatrix();
	
	glTranslatef(-0.09, -0.27, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(30 - FirstJoint, 1, 0, 0);

	GLUquadric *quadObj3 = gluNewQuadric();
	gluCylinder(quadObj3,0.035,0.035,0.25, 25, 25);
	glPopMatrix();
			//2ndJoint
	glPushMatrix();
	glRotatef(-SecondJoint, 0, 0, 1);
	glTranslatef(-0.3, -0.6, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(150 - SecondJoint, 1, 0, 0);
	glRotatef(110, 1, 0, 0);

	GLUquadric *quadObj4 = gluNewQuadric();
	gluCylinder(quadObj4, 0.035, 0.035, 0.25, 25, 25);
	glPopMatrix();
			//1stJoint
	
	glPushMatrix();
	glTranslatef(0.09, -0.27, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(150 + FirstJoint, 1, 0, 0);
	
	GLUquadric *quadObj5 = gluNewQuadric();
	gluCylinder(quadObj5, 0.035, 0.035, 0.25, 25, 25);
	glPopMatrix();
			//2ndJoint

	glPushMatrix();
	glRotatef(SecondJoint, 0, 0, 1);
	glTranslatef(0.23, -0.66,0 );
	glRotatef(265, 0, 1, 0);
	glRotatef(150+SecondJoint, 1, 0, 0);
	glRotatef(110, 1, 0, 0);
	
	GLUquadric *quadObj6 = gluNewQuadric();
	gluCylinder(quadObj6, 0.035, 0.035, 0.25, 25, 25);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
		//Legs
			//FirstLeg
	glPushMatrix();
	glTranslatef(-0.1, -1.7, 0);
	glPushMatrix();
	glRotatef(269, 1, 0, 0);
	GLUquadric *quadObj7 = gluNewQuadric();
	gluCylinder(quadObj7, 0.045, 0.045, 0.55, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glScaled(0.2, 0.2, 0.2);
	glTranslatef(0, -0.3, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.5, 0.2);
	GLUquadric *quadObj9 = gluNewQuadric();
	gluSphere(quadObj9, 1.0, 10, 10);
	glPopMatrix();
	glPopMatrix();
			//SecondLeg
	glPushMatrix();
	glColor3f(0.917, 0.753, 0.525);
	glTranslatef(0.1, -1.7, 0);
	glPushMatrix();
	glRotatef(269, 1, 0, 0);
	GLUquadric *quadObj8 = gluNewQuadric();
	gluCylinder(quadObj8, 0.045, 0.045, 0.55, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glScaled(0.2, 0.2, 0.2);
	glTranslatef(0, -0.3, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.5, 0.2);
	GLUquadric *quadObj10 = gluNewQuadric();
	gluSphere(quadObj10, 1.0, 10, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//-----------------------------------------------------------------------------

	//Jack Main Character
	glPushMatrix();
	
	glTranslatef(resJmp2[0] - 0.3 - GuyPosX, resJmp2[1] -GuyPosY, RJmp2);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(4.8, 0, 0.4);
	//SuitPaints 1stLeg
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(-1.08, -1.4, 0);
	glRotatef(269, 1, 0, 0);
	GLUquadric *quadObj11 = gluNewQuadric();
	gluCylinder(quadObj11, 0.04, 0.065, 0.65, 25, 25);
	glPopMatrix();
	//Suit 1stShoe
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-1.09, -1.46, 0);
	glScalef(0.08, 0.08, 0.18);
	glRotatef(245, 1, 0, 0);
	glRotatef(250, 1, 0, 0);
	glutSolidTetrahedron();
	glPopMatrix();
	//SuitPaints 2ndLeg
	glPushMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glTranslatef(-0.95, -1.4, 0);
	glRotatef(269, 1, 0, 0);
	GLUquadric *quadObj12 = gluNewQuadric();
	gluCylinder(quadObj12, 0.04, 0.065, 0.65, 25, 25);
	glPopMatrix();
	//Suit 2ndShoe
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-0.95, -1.46, 0);
	glScalef(0.08, 0.08, 0.18);
	glRotatef(245, 1, 0, 0);
	glRotatef(250, 1, 0, 0);
	glutSolidTetrahedron();
	glPopMatrix();
	//SuitTop
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(-1, -0.3, 0);
	glRotated(16, 0, 1, 0);
	glScaled(1.2, 4.5, 1.2);
	glutSolidCube(0.2);
	glPopMatrix();

	//SuitCrava

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.958, -0.54, 0.12);
	GLUquadric *quadObj13 = gluNewQuadric();
	gluDisk(quadObj13, 0.01, 0.015, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.958, -0.36, 0.12);
	GLUquadric *quadObj14 = gluNewQuadric();
	gluDisk(quadObj14, 0.01, 0.015, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-0.94, 0.8, 0.115);

	glRotatef(21, 0, 1, 0);
	
	glRotatef(90, 0, 0, 1);
	glScalef(0.7, 0.7, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-1.6, 0.035, 0.12);

	glVertex3f(-0.95, -0.136, 0.12);
	glVertex3f(-0.95, 0.2, 0.12);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-0.93, 0.23, 0);
	glScalef(0.08, 0.08, 0.18);
	glRotatef(100, 1, 0, 0);
	glRotatef(250, 1, 0, 0);
	glutSolidTetrahedron();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-1.08, 0.23, 0);
	glScalef(0.08, 0.08, 0.18);
	glRotatef(145, 0, 0, 1);
	
	glRotatef(100, 1, 0, 0);
	glRotatef(250, 1, 0, 0);
	glutSolidTetrahedron();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(-0.97, 0.2, 0.09);
	glutSolidSphere(0.03, 25, 25);
	glPopMatrix();



	//JackHead

	glPushMatrix();
	glColor3f(0.917, 0.753, 0.525);
	glTranslatef(-1.02, 0.45, 0);
	glutSolidSphere(0.08, 25, 25);
	glPopMatrix();


	//ARMS
	glPushMatrix();
	glColor3f(0, 0, 0);
	glRotatef(-(JArm+HandsGuy), 1, 0, 0);
	//1stJoint

	glPushMatrix();
	glTranslatef(-1.1, 0.12, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(30, 1, 0, 0);

	GLUquadric *quadObj15 = gluNewQuadric();
	gluCylinder(quadObj15, 0.06, 0.06, 0.35, 25, 25);
	glPopMatrix();
	//2ndJoint

	glPushMatrix();
	glRotatef(-SecondJoint, 1, 0, 0);
	glTranslatef(-1.4, -0.04, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(120, 1, 0, 0);

	GLUquadric *quadObj16 = gluNewQuadric();
	gluCylinder(quadObj16, 0.04, 0.04, 0.45, 25, 25);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-(JArm2 + HandsGuy), 1, 0, 0);
	//1stJoint

	glPushMatrix();
	glTranslatef(-0.89, 0.09, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(150, 1, 0, 0);

	GLUquadric *quadObj17 = gluNewQuadric();
	gluCylinder(quadObj17, 0.06, 0.06, 0.25, 25, 25);
	glPopMatrix();
	//2ndJoint

	glPushMatrix();
	glRotatef(-SecondJoint, 1, 0, 0);
	glTranslatef(-0.75, -0.45, 0);
	glRotatef(265, 0, 1, 0);
	glRotatef(150, 1, 0, 0);
	glRotatef(110, 1, 0, 0);

	GLUquadric *quadObj18 = gluNewQuadric();
	gluCylinder(quadObj18, 0.04, 0.04, 0.45, 25, 25);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	//-------------------------------------------------------------------------------	//Titanic Ship
	glPushMatrix();
	glRotatef(SinkAng, 0, 0, 1);
	glTranslatef(-0.8 + SinkPos, 0, 0);
		//BackPyramid
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(1.2, -1.3, -1.2);
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glRotatef(42, 1, 0, 0);
	glRotatef(180, 0, 1, 0);
	glScaled(0.01, 0.01, 0.02);
	pyramid();
	glPopMatrix();

		//ShipSquare
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(1.9, -1.3, -0.45);
	glRotatef(42.5, 0, 1, 0);
	glScalef(9.8, 5, 5.3);
	glutSolidCube(0.2);
	glPopMatrix();

		//FrontPyramid
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(2.6, -1.3, 0.3);
	glRotatef(-90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(-42, 1, 0, 0);
	glScaled(0.01, 0.01, 0.02);
	pyramid();
	glPopMatrix();

		//ShipLevel1
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(1.9, -0.8, -0.45);
	glRotatef(42.5, 0, 1, 0);
	glScalef(8.7, 1, 4.7);
	glutSolidCube(0.2);
	glPopMatrix();

		//ShipLevel2
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(1.9, -0.7, -0.45);
	glRotatef(42.5, 0, 1, 0);
	glScalef(8.2, 1, 4.2);
	glutSolidCube(0.2);
	glPopMatrix();

		//Mad5ana1 Yellow
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(1.9, -0.3, -0.45);
	glRotatef(90, 1, 0, 0);
	GLUquadric *quadObj19 = gluNewQuadric();
	gluCylinder(quadObj19, 0.06, 0.06, 0.35, 25, 25);
	glPopMatrix();

		//Mad5ana1 Black
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(1.9, -0.2, -0.45);
	glRotatef(90, 1, 0, 0);
	GLUquadric *quadObj20 = gluNewQuadric();
	gluCylinder(quadObj20, 0.06, 0.06, 0.1, 25, 25);
	glPopMatrix();

		//Mad5ana2 Yellow
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(1.7, -0.3, -0.7);
	glRotatef(90, 1, 0, 0);
	GLUquadric *quadObj21 = gluNewQuadric();
	gluCylinder(quadObj21, 0.06, 0.06, 0.35, 25, 25);
	glPopMatrix();

		//Mad5ana2 Black
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(1.7, -0.2, -0.7);
	glRotatef(90, 1, 0, 0);
	GLUquadric *quadObj22 = gluNewQuadric();
	gluCylinder(quadObj22, 0.06, 0.06, 0.1, 25, 25);
	glPopMatrix();
		
		//Ship Beginning
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(2.75, -0.5, 0.5);
	glRotatef(90, 1, 0, 0);
	GLUquadric *quadObj23 = gluNewQuadric();
	gluCylinder(quadObj23, 0.01, 0.01, 0.3, 25, 25);
	glPopMatrix();


		//WINDOW
	glColor3f(1,1,1);
	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(1.42, -0.9, -0.75);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(1.2, -0.9, -0.76);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.98, -0.9, -0.77);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.76, -0.9, -0.78);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.54, -0.9, -0.79);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(1.42, -1.1, -0.75);
	glutSolidSphere(0.1,25,25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(1.2, -1.1, -0.76);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.98, -1.1, -0.77);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.76, -1.1, -0.78);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0.54, -1.1, -0.79);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();
	glPopMatrix();
	//----------------------------------------------------------------------------
	glPopMatrix();
	//WoodenDoor
	if (WoodPos <= -1){
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	}
	else{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
		
	glPushMatrix();
	if (all)
		glTranslatef(1.9, -1, -(Shipz));
	glColor3f(0.545, 0.27, 0.0745);
	glTranslatef(resWood[0], resWood[1], 0);
	glTranslatef(-1.6, -2 + WoodPos, 0);
	glRotatef(70, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	glPushMatrix();

	glTranslatef(0, 0.5, 0);
	glRotatef(120, 1, 0, 0);
	glScalef(8.7, 1, 4.7);
	glutSolidCube(0.2);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.85, 0.7, 0.18);
	glRotatef(120, 1, 0, 0);
	glScalef(8.7, 1, 5.7);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-0.8, 0.1, -0.2);
	glRotatef(80, 0, 0, 1);
	glScalef(0.2, 1, 0.2);
	glutSolidTetrahedron();
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-0.8, 0.95, 0.22);
	glRotatef(90, 0, 1, 0);

	GLUquadric *quadObj24 = gluNewQuadric();
	gluCylinder(quadObj24, 0.08, 0.08, 1.2, 25, 25);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//----------------------------------------------------------------------------------
	//Iceberg
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(9 + resIce[0], resIce[1] - 0.7, -0.95);

	glPushMatrix();
	glTranslatef(-2, 0, 0);
	glRotatef(166, 1, 1, 0);
	glScalef(0.4, 0.4, 0.4);
	glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.8, -0.2, -0.2);
	glRotatef(79, 0, 0, 1);
	glScalef(0.3, 0.3, 0.3);
	glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.3, -0.2, 0);
	glRotatef(70, 0, 1, 0);
	glScalef(0.3, 0.3, 0.3);
	glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, -0.4, 0);
	glRotatef(70, 0, 1, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.15, -0.4, 0);
	glScalef(0.2, 0.2, 0.2);
	glutSolidIcosahedron();
	glPopMatrix();
	glPopMatrix();
	//------------------------------------------------------------------





	//BckGround
		//sea
	glPushMatrix();
	glColor3f(0, 0.2705, 0.38039);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glTranslatef(0, -35.5, -7);

	glScalef(20, 20, 0);
	glutSolidSphere(2, 25, 25);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);

		//Moon
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 6.1, -8);
	glutSolidSphere(2, 25, 25);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
		//Starts
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(25, 50.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(45, 60.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(50, 55.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(70, 40.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(5, 75.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-30, 65.5, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-50, 70, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-70, 80, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-80, 25, -65);
	drawJack();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-18, 35, -65);
	drawJack();
	glPopMatrix();
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	//-------------------------------------------------
	glFlush();
}


void Anim() {
	if (!paused){

		if (clicked){
			cam += 0.1;
		}
		if (1.8 + (resShip[0] - 9.5) < -1){
			shipAng += 0.003;
			Shipz += 0.00085;

			if (t < 1.0){

				resShip[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
				resShip[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
				t += 0.001;

			}
			else{
				t = 0;
				p0[0] = p3[0];

				p1[0] = p3[0] + 0.115;
				p1[1] = p3[1] - 0.36;
				p3[0] = p0[0] + 1.5;

				p2[0] = p3[0] + 0.101;
				p2[1] = p3[1] + 0.61;
			}
		}
		else{
			if (!hands && !Ice && !Jump && !Wood)
				dress = true;
			if (t < 1.0){

				resShip[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
				resShip[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
				t += 0.001;

			}
			else{
				t = 0;
				p0[0] = p3[0];

				p1[0] = p3[0] + 0.115;
				p1[1] = p3[1] - 0.36;
				p3[0] = p0[0] + 0;

				p2[0] = p3[0] + 0.101;
				p2[1] = p3[1] + 0.61;
			}
		}
		if (dress == true){
			if (first){
				first = false;
				glutTimerFunc(5 * 1000, Timer, 0);

			}
			if (going){
				dressAng += 0.1;
				if (dressAng >= 10){
					going = !going;
				}
			}
			else{
				dressAng -= 0.1;
				if (dressAng <= -10){
					going = !going;
				}
			}
			if (Up){
				JArm += 1;
				if (JArm2 != 0)
					JArm2 -= 1;
				if (JArm >= 180){
					Up = !Up;
				}
			}
			else{
				JArm2 += 1;
				if (JArm != 0)
					JArm -= 1;
				if (JArm2 >= 180){
					Up = !Up;
				}
			}
		}
		if (hands){

			if (first){
				first = false;
				glutTimerFunc(5 * 1000, TimerI, 0);

			}
			JArm = 0;
			JArm2 = 0;
			dressAng = 0;
			if (FirstJoint < 30){
				FirstJoint += 0.01;
			}
			if (SecondJoint < 40){

				SecondJoint += 0.02;
			}

		}

		if (Ice){

			if (I < 1.0){

				resIce[0] = pow((1 - I), 3)*I0[0] + 3 * I*pow((1 - I), 2)*I1[0] + 3 * pow(I, 2)*(1 - I)*I2[0] + pow(I, 3)*I3[0];
				resIce[1] = pow((1 - I), 3)*I0[1] + 3 * I*pow((1 - I), 2)*I1[1] + 3 * pow(I, 2)*(1 - I)*I2[1] + pow(I, 3)*I3[1];
				I += 0.001;

			}
			else{
				I = 0;
				I0[0] = I3[0];

				I1[0] = I3[0] - 0.37;
				I1[1] = I3[1] - 0.71;
				I2[0] = I3[0] - 0.8;
				I2[1] = I3[1] + 0.137;
				I3[0] -= 0.29;


			}
		}

		if (6 + resIce[0] < 0 && Ice){
			if (first){
				first = false;
				glutTimerFunc(3 * 1000, TimerJ, 0);
			}
		}

		if (Jump){

			if (J < 1.0){

				resJmp[0] = pow((1 - J), 3)*J0[0] + 3 * J*pow((1 - J), 2)*J1[0] + 3 * pow(J, 2)*(1 - J)*J2[0] + pow(J, 3)*J3[0];
				resJmp[1] = pow((1 - J), 3)*J0[1] + 3 * J*pow((1 - J), 2)*J1[1] + 3 * pow(J, 2)*(1 - J)*J2[1] + pow(J, 3)*J3[1];
				J += 0.001;
				RJmp += 0.001;

			}
			else{
				if (J22 < 1.0){
					resJmp2[0] = pow((1 - J), 3)*J0[0] + 3 * J*pow((1 - J), 2)*J1[0] + 3 * pow(J, 2)*(1 - J)*J2[0] + pow(J, 3)*J3[0];
					resJmp2[1] = pow((1 - J), 3)*J0[1] + 3 * J*pow((1 - J), 2)*J1[1] + 3 * pow(J, 2)*(1 - J)*J2[1] + pow(J, 3)*J3[1];
					J22 += 0.001;
					RJmp2 += 0.001;
				}
				else{
					if (SinkAng < 90){
						SinkAng += 0.01;
					}
					else{
						SinkPos -= 0.001;
						if (WoodPos < 0.45){
							WoodPos += 0.001;
						}
						else{
							if (first == true){
								first = false;
								glutTimerFunc(1.5 * 1000, TimerW, 0);
							}
						}

					}

				}
			}

		}

		if (Wood){
			if ((resWood[0] - 1.6) < 0.7){
				if (GirlRot < 90){
					GirlRot += 0.03;

				}
				if (GirlX < 0.47)
					GirlX += 0.01;
				if (GirlY < 0.92)
					GirlY += 0.02;
				if (GirlZ < 0.22)
					GirlZ += 0.001;
				if (GuyPosX < 0.5)
					GuyPosX += 0.001;


				if (W < 1.0){

					resWood[0] = pow((1 - W), 3)*W0[0] + 3 * W*pow((1 - W), 2)*W1[0] + 3 * pow(W, 2)*(1 - W)*W2[0] + pow(W, 3)*W3[0];
					resWood[1] = pow((1 - W), 3)*W0[1] + 3 * W*pow((1 - W), 2)*W1[1] + 3 * pow(W, 2)*(1 - W)*W2[1] + pow(W, 3)*W3[1];
					W += 0.001;

				}
				else{
					W = 0;
					W0[0] = W3[0];

					W1[0] = W3[0] - 0.13;
					W1[1] = W3[1] - 0.68;
					W3[0] = W0[0] + 0.67;

					W2[0] = W3[0] - 0.34;
					W2[1] = W3[1] + 0.82;
				}
			}
			else{
				all = true;
				GuyPosY += 0.001;
				if (HandsGuy < 180)
					HandsGuy += 10;
			}
		}
	}
	glutPostRedisplay();
}

void Timer(int value) {
	dress = false;
	hands = true;
	first = true;
	glutPostRedisplay();

}
void TimerI(int value) {
	dress = false;
	hands = false;
	Ice = true;
	first = true;
	glutPostRedisplay();

}
void TimerJ(int value) {
	WaitGirl = 0;
	SecondJoint = 0;
	FirstJoint = 0;
	dress = false;
	hands = false;
	Ice = false;
	Jump = true;
	first = true;
	glutPostRedisplay();

}
void TimerW(int value) {
	dress = false;
	hands = false;
	Ice = false;
	Jump = false;
	Wood = true;
	first = true;
	glutPostRedisplay();

}
void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'r':
		camera.moveY(5.5);
		camera.moveZ(1.5);
		camera.rotateX(-90);
		break;
	case 'm':
		clicked = !clicked;
		break;
	case 'p':
		paused = !paused;
		break;
	case 'i':
		keyY += 0.01;
		break;
	case 'k':
		keyY -= 0.01;
		break;
	case 'l':
		keyX += 0.01;
		break;
	case 'j':
		keyX -= 0.01;
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1700, 900);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	PlaySound(TEXT("Titanic Theme Song • My Heart Will Go On • Celine Dion.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
