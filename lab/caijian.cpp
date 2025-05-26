#include<glut.h>


#define Xmin 100
#define Xmax 400
#define Ymin 100
#define Ymax 400

int clickcount = 0;
float x1, y1, x2, y2;

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

int computeOutCode(float x, float y)
{
	int code = INSIDE;
	if(x<Xmin)
		code |= LEFT;
	else if (x > Xmax)
		code |= RIGHT;
	if (y < Ymin)
		code |= BOTTOM;
	else if (y > Ymax)
		code |= TOP;
	return code;
}

bool cohenSutherlandClip(float& x0, float& y0, float& x1, float& y1)
{
	int outcode0 = computeOutCode(x0, y0);
	int outcode1 = computeOutCode(x1, y1);
	bool accept = false;
	while (true)
	{
		if (!(outcode0 | outcode1)) // 线段在窗口内
		{
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) // 线段在窗口外
			break;
		else
		{
			float x, y;
			int outcodeOut = outcode0 ? outcode0 : outcode1; // 选择一个外部点
			if (outcodeOut & TOP) // 线段在上边界外
			{
				x = x0 + (x1 - x0) * (Ymax - y0) / (y1 - y0);
				y = Ymax;
			}
			else if (outcodeOut & BOTTOM) // 线段在下边界外
			{
				x = x0 + (x1 - x0) * (Ymin - y0) / (y1 - y0);
				y = Ymin;
			}
			else if (outcodeOut & RIGHT) // 线段在右边界外
			{
				y = y0 + (y1 - y0) * (Xmax - x0) / (x1 - x0);
				x = Xmax;
			}
			else if (outcodeOut & LEFT) // 线段在左边界外
			{
				y = y0 + (y1 - y0) * (Xmin - x0) / (x1 - x0);
				x = Xmin;
			}
			if (outcodeOut == outcode0)
			{
				x0 = x; y0 = y; // 更新第一个端点
				outcode0 = computeOutCode(x0, y0);
			}
			else
			{
				x1 = x; y1 = y; // 更新第二个端点
				outcode1 = computeOutCode(x1, y1);
			}
		}
	}
	return accept;
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(Xmin, Ymin);
	glVertex2f(Xmax, Ymin);
	glVertex2f(Xmax, Ymax);
	glVertex2f(Xmin, Ymax);
	glEnd();
	if (clickcount == 2)
	{
		float clippedX1 = x1, clippedY1 = y1;
		float clippedX2 = x2, clippedY2 = y2;
		if (cohenSutherlandClip(clippedX1, clippedY1, clippedX2, clippedY2))
		{
			glColor3f(0.0, 1.0, 0.0);
			glBegin(GL_LINES);
			glVertex2f(clippedX1, clippedY1);
			glVertex2f(clippedX2, clippedY2);
			glEnd();
		}
	}
	glutSwapBuffers(); // 替换glFlush()为glutSwapBuffers()
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (clickcount == 0) {
			x1 = x;
			y1 = 600 - y; // OpenGL坐标系y轴向上
			clickcount++;
		}
		else if (clickcount == 1) {
			x2 = x;
			y2 = 600 - y; // OpenGL坐标系y轴向上
			clickcount++;
		}
		else {
			clickcount = 0; // 重置点击计数
		}
		glutPostRedisplay();
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0,1.0); // 设置背景颜色为白色
	glColor3f(0.0, 0.0, 0.0); // 设置绘制颜色为黑色
	glPointSize(5.0); // 设置点的大小
	glLineWidth(2.0); // 设置线宽
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600); // 设置坐标系范围
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
    glutCreateWindow("cut");

    init();
    return 0;
}
