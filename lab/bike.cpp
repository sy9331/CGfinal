#include "glut.h"
#include <math.h>

GLfloat wheelAngle = 0.0f; // 车轮旋转角度
GLfloat bikePosition = -1.0f; // 自行车的水平位置
GLfloat speed = 2.0f; // 自行车的速度

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
    for (int i = 0; i < 8; i++) // 八边形有 8 个顶点
    {
		GLfloat angle = i * 3.14159f / 4; // 每个顶点的角度
		vertices[i][0] = x + size * cos(angle); // 计算 x 坐标
		vertices[i][1] = y + size * sin(angle); // 计算 y 坐标
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
        // 每个顶点与对面的顶点连线
        glVertex2f(vertices[i][0], vertices[i][1]);
        glVertex2f(vertices[(i + 4) % 8][0], vertices[(i + 4) % 8][1]);
    }
    glEnd();
}
void drawBike()
{
    // 使用矩阵栈管理变换

  // 后轮
    glPushMatrix();
    glTranslatef(-50, -50, 0); // 平移到后轮位置
    glRotatef(wheelAngle, 0, 0, 1); // 旋转后轮
    glColor3f(1.0, 1.0, 1.0);
    drawDiamondWheel(0, 0, 60); // 绘制后轮
    glPopMatrix();

    // 前轮
    glPushMatrix();
    glTranslatef(150, -50, 0); // 平移到前轮位置
    glRotatef(wheelAngle, 0, 0, 1); // 旋转前轮
    glColor3f(1.0, 1.0, 1.0);
    drawDiamondWheel(0, 0, 60); // 绘制前轮
    glPopMatrix();

    // 车架
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(-50, -50); glVertex2f(150, -50); // 后轮到前轮
    glVertex2f(50, -50); glVertex2f(50, 100); // 车座到车把
	glVertex2f(25, 100); glVertex2f(75, 100); 
    glEnd();

    // 踏板
    glPushMatrix();
    glTranslatef(50, -50, 0); // 平移到踏板中心
    glRotatef(wheelAngle, 0, 0, 1); // 旋转踏板
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(-20, 0); glVertex2f(20, 0); // 踏板横杆
    glVertex2f(0, -20); glVertex2f(0, 20); // 踏板竖杆
    glEnd();
    glPopMatrix();
}

// 显示函数
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(bikePosition, 300, 0); // 平移整个自行车
    drawBike();
    glPopMatrix();

    glutSwapBuffers();
}

// 动画更新
void update(int value) {
    wheelAngle -= speed * 2.0f; // 更新车轮旋转角度
    bikePosition += speed; // 更新自行车位置

    // 循环移动
    if (bikePosition > 800) {
        bikePosition = -200;
    }

    glutPostRedisplay(); // 重新绘制
    glutTimerFunc(16, update, 0); // 每16ms更新一次
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bicycle Animation with Matrix Stack");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0); // 启动动画更新
    glutMainLoop();

    return 0;
}