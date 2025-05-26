#include "glut.h"
#include <math.h>

GLfloat wheelAngle = 0.0f; // ������ת�Ƕ�
GLfloat bikePosition = -1.0f; // ���г���ˮƽλ��
GLfloat speed = 2.0f; // ���г����ٶ�

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat radius)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		GLfloat angle = i * 3.14159 / 180;
		glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
	}
	glEnd();
}
void drawDiamondWheel(GLfloat x, GLfloat y, GLfloat size)
{
	GLfloat vertices[8][2]; 
    for (int i = 0; i < 8; i++) // �˱����� 8 ������
    {
		GLfloat angle = i * 3.14159f / 4; // ÿ������ĽǶ�
		vertices[i][0] = x + size * cos(angle); // ���� x ����
		vertices[i][1] = y + size * sin(angle); // ���� y ����
    }
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 8; i++)
    {
        glVertex2f(vertices[i][0], vertices[i][1]);
    }
    glEnd();
    glBegin(GL_LINES);
    for (int i = 0; i < 8; i++)
    {
        // ÿ�����������Ķ�������
        glVertex2f(vertices[i][0], vertices[i][1]);
        glVertex2f(vertices[(i + 4) % 8][0], vertices[(i + 4) % 8][1]);
    }
    glEnd();
}
void drawBike()
{
    // ʹ�þ���ջ����任

  // ����
    glPushMatrix();
    glTranslatef(-50, -50, 0); // ƽ�Ƶ�����λ��
    glRotatef(wheelAngle, 0, 0, 1); // ��ת����
    glColor3f(1.0, 1.0, 1.0);
    drawDiamondWheel(0, 0, 60); // ���ƺ���
    glPopMatrix();

    // ǰ��
    glPushMatrix();
    glTranslatef(150, -50, 0); // ƽ�Ƶ�ǰ��λ��
    glRotatef(wheelAngle, 0, 0, 1); // ��תǰ��
    glColor3f(1.0, 1.0, 1.0);
    drawDiamondWheel(0, 0, 60); // ����ǰ��
    glPopMatrix();

    // ����
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(-50, -50); glVertex2f(150, -50); // ���ֵ�ǰ��
    glVertex2f(50, -50); glVertex2f(50, 100); // ����������
	glVertex2f(25, 100); glVertex2f(75, 100); 
    glEnd();

    // ̤��
    glPushMatrix();
    glTranslatef(50, -50, 0); // ƽ�Ƶ�̤������
    glRotatef(wheelAngle, 0, 0, 1); // ��ת̤��
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-20, 0); glVertex2f(20, 0); // ̤����
    glVertex2f(0, -20); glVertex2f(0, 20); // ̤������
    glEnd();
    glPopMatrix();
}

// ��ʾ����
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(bikePosition, 300, 0); // ƽ���������г�
    drawBike();
    glPopMatrix();

    glutSwapBuffers();
}

// ��������
void update(int value) {
    wheelAngle -= speed * 2.0f; // ���³�����ת�Ƕ�
    bikePosition += speed; // �������г�λ��

    // ѭ���ƶ�
    if (bikePosition > 800) {
        bikePosition = -200;
    }

    glutPostRedisplay(); // ���»���
    glutTimerFunc(16, update, 0); // ÿ16ms����һ��
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bicycle Animation with Matrix Stack");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0); // ������������
    glutMainLoop();

    return 0;
}