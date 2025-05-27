#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // ����ģ�����ݽṹ
#include <glut.h>

//// ��Ⱦģʽö��
//enum RenderMode {
//    FILL_MODE,
//    LINE_MODE,
//    POINT_MODE
//};
//enum CameraMode {
//    ORBITAL_CAMERA, // Χ�����ĵ���ת
//    FREE_LOOK_CAMERA // ����ƽ�ƺ���ת
//};

// ȫ����Ⱦ״̬����
extern RenderMode g_currentRenderMode;
extern bool g_enableTexture;
extern bool g_enableMaterial;
extern bool g_displayCoordinates;
extern bool g_displayInfo;

// ������� 
extern CameraMode g_currentCameraMode;

extern float g_cameraPosX, g_cameraPosY, g_cameraPosZ;
extern float g_cameraFrontX, g_cameraFrontY, g_cameraFrontZ; 
extern float g_cameraUpX, g_cameraUpY, g_cameraUpZ; 

//�����������
extern float g_yaw;  
extern float g_pitch;
extern float g_roll;

//������
extern float g_orbitalDistance; 
extern float g_orbitalYaw;      
extern float g_orbitalPitch;  
extern float g_orbitalRoll;
extern float g_targetX, g_targetY, g_targetZ;


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

//���ģʽ�л�
void toggleCameraMode();


// �����ı�����
void drawText(float x, float y, const char* text);

#endif // RENDER_H

