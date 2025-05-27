#ifndef INTERACTION_H
#define INTERACTION_H

#include "render.h"      // 包含渲染状态和函数
#include <glut.h> // 或者 #include <GL/glut.h>
// 鼠标交互变量
extern int g_lastMouseX, g_lastMouseY;
extern bool g_isRotating;//用于轨道相机的旋转
extern bool g_isPanning; //用于自由漫游相机的旋转
extern bool g_isMovinggCamera;//用于自由漫游相机的平移

// 函数声明
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);

#endif