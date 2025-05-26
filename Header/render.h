#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // 包含模型数据结构
#include <glut.h>

// 渲染模式枚举
enum RenderMode {
    FILL_MODE,
    LINE_MODE,
    POINT_MODE
};

// 全局渲染状态变量
extern RenderMode g_currentRenderMode;
extern bool g_enableTexture;
extern bool g_enableMaterial;
extern bool g_displayCoordinates;
extern bool g_displayInfo;

// 相机变量 (示例，可根据交互模块的实际需求调整)
extern float g_cameraX, g_cameraY, g_cameraZ;
extern float g_lookAtX, g_lookAtY, g_lookAtZ;
extern float g_upX, g_upY, g_upZ;

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

// 绘制文本函数
void drawText(float x, float y, const char* text);

#endif // RENDER_H

