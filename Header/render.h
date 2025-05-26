#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // ����ģ�����ݽṹ
#include <glut.h>

// ��Ⱦģʽö��
enum RenderMode {
    FILL_MODE,
    LINE_MODE,
    POINT_MODE
};

// ȫ����Ⱦ״̬����
extern RenderMode g_currentRenderMode;
extern bool g_enableTexture;
extern bool g_enableMaterial;
extern bool g_displayCoordinates;
extern bool g_displayInfo;

// ������� (ʾ�����ɸ��ݽ���ģ���ʵ���������)
extern float g_cameraX, g_cameraY, g_cameraZ;
extern float g_lookAtX, g_lookAtY, g_lookAtZ;
extern float g_upX, g_upY, g_upZ;

// ��ת����
extern float g_rotateX, g_rotateY;

// ��������
void renderScene();
void changeSize(int w, int h);
void resetView();

// ��Ⱦģʽ�л�����
void toggleFillMode();
void toggleLineMode();
void togglePointMode();
void toggleTexture();
void toggleMaterial();
void toggleCoordinatesDisplay();
void toggleInfoDisplay();

// �����ı�����
void drawText(float x, float y, const char* text);

#endif // RENDER_H

