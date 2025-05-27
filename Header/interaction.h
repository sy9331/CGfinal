#ifndef INTERACTION_H
#define INTERACTION_H

#include "render.h"      // ������Ⱦ״̬�ͺ���
#include <glut.h> // ���� #include <GL/glut.h>
// ��꽻������
extern int g_lastMouseX, g_lastMouseY;
extern bool g_isRotating;//���ڹ���������ת
extern bool g_isPanning; //�������������������ת
extern bool g_isMovinggCamera;//�����������������ƽ��

// ��������
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMove(int x, int y);

#endif