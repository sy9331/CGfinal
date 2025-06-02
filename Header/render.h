#ifndef RENDER_H
#define RENDER_H

 
#include "parser1.h"     // 包含模型数据结构
#include <glut.h>

// 定义投影模式
enum ProjectionMode {
    PERSPECTIVE_PROJECTION,
    ORTHOGRAPHIC_PROJECTION
};

//渲染模式
enum RenderMode {
     FILL_MODE,
     LINE_MODE,
     POINT_MODE
};
// 投影模式
extern ProjectionMode g_currentProjectionMode;

// 渲染模式
extern RenderMode g_currentRenderMode;

// 轨道相机变量
extern float g_orbitalDistance;
extern float g_orbitalYaw;
extern float g_orbitalPitch;
extern float g_targetX, g_targetY, g_targetZ; // 相机旋转/平移的目标点 (即模型中心)
extern float g_pitch;

// 其他场景状态
extern bool g_displayCoordinates;
extern bool g_displayInfo;
extern bool g_textureEnabled;
extern bool g_materialEnabled;
extern GLenum g_polygonMode;

// 函数声明
void renderScene();
void reshape(int w, int h);
void toggleProjectionMode();
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
void drawSimpleCube();

#endif // RENDER_H

