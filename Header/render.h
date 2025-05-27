#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // 包含模型数据结构
#include <glut.h>

//// 渲染模式枚举
//enum RenderMode {
//    FILL_MODE,
//    LINE_MODE,
//    POINT_MODE
//};
//enum CameraMode {
//    ORBITAL_CAMERA, // 围绕中心点旋转
//    FREE_LOOK_CAMERA // 自由平移和旋转
//};

// 全局渲染状态变量
extern RenderMode g_currentRenderMode;
extern bool g_enableTexture;
extern bool g_enableMaterial;
extern bool g_displayCoordinates;
extern bool g_displayInfo;

// 相机变量 
extern CameraMode g_currentCameraMode;

extern float g_cameraPosX, g_cameraPosY, g_cameraPosZ;
extern float g_cameraFrontX, g_cameraFrontY, g_cameraFrontZ; 
extern float g_cameraUpX, g_cameraUpY, g_cameraUpZ; 

//自由漫游相机
extern float g_yaw;  
extern float g_pitch;
extern float g_roll;

//轨道相机
extern float g_orbitalDistance; 
extern float g_orbitalYaw;      
extern float g_orbitalPitch;  
extern float g_orbitalRoll;
extern float g_targetX, g_targetY, g_targetZ;


// 旋转变量
extern float g_rotateX, g_rotateY;

// 函数声明
void renderScene();
void changeSize(int w, int h);
void resetView();

// 渲染模式切换函数
void toggleFillMode();
void toggleLineMode();
void togglePointMode();
void toggleTexture();
void toggleMaterial();
void toggleCoordinatesDisplay();
void toggleInfoDisplay();

//相机模式切换
void toggleCameraMode();


// 绘制文本函数
void drawText(float x, float y, const char* text);

#endif // RENDER_H

