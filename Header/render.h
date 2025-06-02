#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // ����ģ�����ݽṹ
#include <glut.h>

// ����ͶӰģʽ
enum ProjectionMode {
    PERSPECTIVE_PROJECTION,
    ORTHOGRAPHIC_PROJECTION
};

//��Ⱦģʽ
enum RenderMode {
     FILL_MODE,
     LINE_MODE,
     POINT_MODE
};
// ͶӰģʽ
extern ProjectionMode g_currentProjectionMode;

// ��Ⱦģʽ
extern RenderMode g_currentRenderMode;

// ����������
extern float g_orbitalDistance;
extern float g_orbitalYaw;
extern float g_orbitalPitch;
extern float g_targetX, g_targetY, g_targetZ; // �����ת/ƽ�Ƶ�Ŀ��� (��ģ������)
extern float g_pitch;

// ��������״̬
extern bool g_displayCoordinates;
extern bool g_displayInfo;
extern bool g_textureEnabled;
extern bool g_materialEnabled;
extern GLenum g_polygonMode;

// ��������
void renderScene();
void reshape(int w, int h);
void toggleProjectionMode();
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
void drawSimpleCube();

#endif // RENDER_H

