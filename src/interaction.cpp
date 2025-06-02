#include "../Header/interaction.h"
#include "../Header/render.h"
#include <cmath>
#include<glut.h>
// 鼠标交互变量定义
int g_lastMouseX, g_lastMouseY;
bool g_isRotating = false;
bool g_isPanning = false;
bool g_isMovingCamera = false; 

// 移动和旋转速度
const float CAMERA_SPEED = 0.1f;
const float ROTATION_SENSITIVITY = 0.2f; 
const float ORBITAL_ZOOM_SPEED = 0.1f;
const float FREE_LOOK_ZOOM_SPEED = 0.1f;
const float PAN_SPEED_FACTOR = 0.05f; 
const float CAMERA_ZOOM_SPEED = 0.2f;
const float CAMERA_PAN_SPEED = 0.1f;
const double M_PI = 3.14159265358979323846;

// 世界坐标系上向量 
const float WORLD_UP_X = 0.0f;
const float WORLD_UP_Y = 1.0f;
const float WORLD_UP_Z = 0.0f;

//辅助函数：获取相机当前的右向量
void getCameraRightVector(float& rx, float& ry, float& rz) {
    // 计算从相机位置到目标点的向量
    float yawRad = g_orbitalYaw * M_PI / 180.0f;
    float pitchRad = g_orbitalPitch * M_PI / 180.0f;
	// 计算相机位置
    float camX = g_orbitalDistance * cos(pitchRad) * sin(yawRad);
    float camY = g_orbitalDistance * sin(pitchRad);
    float camZ = g_orbitalDistance * cos(pitchRad) * cos(yawRad);
	// 计算当前前向向量
    float currentFrontX = -camX; 
    float currentFrontY = -camY;
    float currentFrontZ = -camZ;
    // 叉积计算右向量
    rx = currentFrontY * WORLD_UP_Z - currentFrontZ * WORLD_UP_Y;
    ry = currentFrontZ * WORLD_UP_X - currentFrontX * WORLD_UP_Z;
    rz = currentFrontX * WORLD_UP_Y - currentFrontY * WORLD_UP_X;
	// 归一化右向量
    float len = sqrt(rx * rx + ry * ry + rz * rz);
    if (len > 0.0001f) {
        rx /= len; ry /= len; rz /= len;
    }
}

// 辅助函数：获取相机当前的向上向量
void getCameraUpVector(float& ux, float& uy, float& uz) {
    float rx, ry, rz;
    getCameraRightVector(rx, ry, rz); 

    // 计算从相机位置到目标点的向量
    float yawRad = g_orbitalYaw * M_PI / 180.0f;
    float pitchRad = g_orbitalPitch * M_PI / 180.0f;

    float camX = g_orbitalDistance * cos(pitchRad) * sin(yawRad);
    float camY = g_orbitalDistance * sin(pitchRad);
    float camZ = g_orbitalDistance * cos(pitchRad) * cos(yawRad);

    float currentFrontX = -camX; 
    float currentFrontY = -camY;
    float currentFrontZ = -camZ;

    
    ux = ry * currentFrontZ - rz * currentFrontY;
    uy = rz * currentFrontX - rx * currentFrontZ;
    uz = rx * currentFrontY - ry * currentFrontX;

    float len = sqrt(ux * ux + uy * uy + uz * uz);
    if (len > 0.0001f) {
        ux /= len; uy /= len; uz /= len;
    }
}

// 普通键处理函数
void processNormalKeys(unsigned char key, int x, int y) {
    float rightX, rightY, rightZ;
    float upX, upY, upZ;
    float frontX, frontY, frontZ;
    getCameraRightVector(rightX, rightY, rightZ);
    rightX = frontY * WORLD_UP_Z - frontZ * WORLD_UP_Y;
    rightY = frontZ * WORLD_UP_X - frontX * WORLD_UP_Z;
	rightZ = frontX * WORLD_UP_Y - frontY * WORLD_UP_X;
    switch (key) {
    case 27: 
        std::exit(0);
        break;
    case '1':
        toggleFillMode();
        break;
    case '2':
        toggleLineMode();
        break;
    case '3':
        togglePointMode();
        break;
    case 't':
    case 'T':
        toggleTexture();
        break;
    case 'm':
    case 'M':
        toggleMaterial();
        break;
    case 'c':
    case 'C':
        toggleCoordinatesDisplay();
        break;
    case 'i':
    case 'I':
        toggleInfoDisplay();
        break;
    case 'r':
    case 'R':
        resetView();
        break;
    case 'p':
	case 'P':
		toggleProjectionMode();
		break;
    case 'w': case 'W': 
        g_orbitalDistance -= CAMERA_ZOOM_SPEED;
        if (g_currentProjectionMode == ORTHOGRAPHIC_PROJECTION) {
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        }
        std::cout << "DEBUG: W key - Zoom In. Orbital Distance: " << g_orbitalDistance << std::endl;
        break;
    case 's': case 'S': 
        g_orbitalDistance += CAMERA_ZOOM_SPEED;
        if (g_currentProjectionMode == ORTHOGRAPHIC_PROJECTION) {
            reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        }
        std::cout << "DEBUG: S key - Zoom Out. Orbital Distance: " << g_orbitalDistance << std::endl;
        break;
    case 'a': case 'A': // Pan Left (move target point left relative to camera's right vector)
        // 移动目标点，使其相对相机向左移动
        g_targetX -= rightX * CAMERA_PAN_SPEED;
        g_targetY -= rightY * CAMERA_PAN_SPEED;
        g_targetZ -= rightZ * CAMERA_PAN_SPEED;
        std::cout << "DEBUG: A key - Pan Left. Target: (" << g_targetX << ", " << g_targetY << ", " << g_targetZ << ")" << std::endl;
        break;
    case 'd': case 'D': // Pan Right (move target point right relative to camera's right vector)
        // 移动目标点，使其相对相机向右移动
        g_targetX += rightX * CAMERA_PAN_SPEED;
        g_targetY += rightY * CAMERA_PAN_SPEED;
        g_targetZ += rightZ * CAMERA_PAN_SPEED;
        std::cout << "DEBUG: D key - Pan Right. Target: (" << g_targetX << ", " << g_targetY << ", " << g_targetZ << ")" << std::endl;
        break;
        // --- End A and D key handling --- 
    
    default:
        break;
    }
    glutPostRedisplay(); 
}

//特殊键处理函数
void processSpecialKeys(int key, int x, int y) {
    
    glutPostRedisplay();
}

// 鼠标事件处理函数
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            g_isRotating = true; 
            g_lastMouseX = x;
            g_lastMouseY = y;
        }
        else {
            g_isRotating = false; 
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            g_isPanning = true;
            g_lastMouseX = x;
            g_lastMouseY = y;
        }
        else {
            g_isPanning = false;
        }
    }
    //好像glut不支持，所以就没做滚轮
    //// 鼠标滚轮滑动控制缩放
    //else if (button == 3) { 
    //    g_orbitalDistance -= ORBITAL_ZOOM_SPEED;
    //    if (g_orbitalDistance < 0.1f) g_orbitalDistance = 0.1f; // 限制最小距离
    //    glutPostRedisplay();
    //    if (g_currentProjectionMode == ORTHOGRAPHIC_PROJECTION) {
    //        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    //    }
    //}
    //else if (button == 4) { 
    //    g_orbitalDistance += ORBITAL_ZOOM_SPEED;
    //    glutPostRedisplay();
    //    if (g_currentProjectionMode == ORTHOGRAPHIC_PROJECTION) {
    //        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    //    }
    //}
}

//鼠标事件
void mouseMove(int x, int y) {
    if (!g_isRotating && !g_isPanning) {
        return;
    }

    float deltaX = (float)(x - g_lastMouseX);
    float deltaY = (float)(y - g_lastMouseY);

    if (g_isRotating) { 
        // 左键按下：绕模型旋转
        g_orbitalYaw -= deltaX * ROTATION_SENSITIVITY;
        g_orbitalPitch -= deltaY * ROTATION_SENSITIVITY;

        if (g_orbitalPitch > 89.0f) g_orbitalPitch = 89.0f;
        if (g_orbitalPitch < -89.0f) g_orbitalPitch = -89.0f;

    }
    else if (g_isPanning) { 
        // 中键按下：平移模型中心 
        float rx, ry, rz;
        getCameraRightVector(rx, ry, rz);
       
        float realUpX, realUpY, realUpZ;
        getCameraUpVector(realUpX, realUpY, realUpZ);

        // 平移目标点
        float panScale = g_orbitalDistance * PAN_SPEED_FACTOR;
        g_targetX -= rx * deltaX * panScale;
        g_targetY -= realUpY * deltaY * panScale;
        g_targetZ -= rz * deltaX * panScale; 
        g_targetX -= rx * deltaX * panScale;
        g_targetY += realUpY * deltaY * panScale; 
        g_targetZ -= rz * deltaX * panScale;

    }

    g_lastMouseX = x;
    g_lastMouseY = y;
    glutPostRedisplay();

    // 重新设置投影
    if (g_currentProjectionMode == ORTHOGRAPHIC_PROJECTION) {
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
}
