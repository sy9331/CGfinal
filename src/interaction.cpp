#include "../Header/interaction.h"
#include <cmath>
#include<glut.h>
// 鼠标交互变量定义
int g_lastMouseX, g_lastMouseY;
bool g_isRotating = false;
bool g_isPanning = false;

// 简单的平移步长
const float PAN_SPEED = 0.01f;
// 缩放步长
const float ZOOM_SPEED = 0.1f;

void processNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC键退出
        exit(0);
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
    case 'w': // 相机前进
    case 'W':
        g_cameraZ -= 0.1f;
        break;
    case 's': // 相机后退
    case 'S':
        g_cameraZ += 0.1f;
        break;
    case 'a': // 相机左移
    case 'A':
        g_cameraX -= 0.1f;
        break;
    case 'd': // 相机右移
    case 'D':
        g_cameraX += 0.1f;
        break;
    default:
        break;
    }
    glutPostRedisplay(); // 触发重绘
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        g_cameraY += 0.1f;
        break;
    case GLUT_KEY_DOWN:
        g_cameraY -= 0.1f;
        break;
    case GLUT_KEY_LEFT:
        g_cameraX -= 0.1f;
        break;
    case GLUT_KEY_RIGHT:
        g_cameraX += 0.1f;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

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
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            g_isPanning = true;
            g_lastMouseX = x;
            g_lastMouseY = y;
        }
        else {
            g_isPanning = false;
        }
    }
    else if (button == 3) { // 鼠标滚轮向上 (放大)
        g_cameraZ -= ZOOM_SPEED;
        glutPostRedisplay();
    }
    else if (button == 4) { // 鼠标滚轮向下 (缩小)
        g_cameraZ += ZOOM_SPEED;
        glutPostRedisplay();
    }
}

void mouseMove(int x, int y) {
    if (g_isRotating) {
        float deltaX = (float)(x - g_lastMouseX);
        float deltaY = (float)(y - g_lastMouseY);

        g_rotateY += deltaX * 0.5f; // 鼠标水平移动控制Y轴旋转
        g_rotateX += deltaY * 0.5f; // 鼠标垂直移动控制X轴旋转

        // 限制X轴旋转角度，避免模型翻转
        if (g_rotateX > 90.0f) g_rotateX = 90.0f;
        if (g_rotateX < -90.0f) g_rotateX = -90.0f;

        g_lastMouseX = x;
        g_lastMouseY = y;
        glutPostRedisplay();
    }
    else if (g_isPanning) {
        // 平移相机
        float deltaX = (float)(x - g_lastMouseX) * PAN_SPEED;
        float deltaY = (float)(y - g_lastMouseY) * PAN_SPEED;

        // 简单的屏幕空间平移
        g_cameraX -= deltaX;
        g_lookAtX -= deltaX;
        g_cameraY += deltaY; // Y轴向上
        g_lookAtY += deltaY;

        g_lastMouseX = x;
        g_lastMouseY = y;
        glutPostRedisplay();
    }
}