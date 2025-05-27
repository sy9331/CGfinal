#include "../Header/interaction.h"
#include <cmath>
#include<glut.h>
// 鼠标交互变量定义
int g_lastMouseX, g_lastMouseY;
bool g_isRotating = false;
bool g_isPanning = false;
bool g_isMovingCamera = false; 

// 移动和旋转速度
const float CAMERA_SPEED = 0.1f;
const float ROTATION_SENSITIVITY = 0.2f; // 鼠标旋转灵敏度
const float ORBITAL_ZOOM_SPEED = 0.1f;
const float FREE_LOOK_ZOOM_SPEED = 0.1f;
const float PAN_SPEED_FACTOR = 0.05f; // 平移速度因子
const double M_PI = 3.14159265358979323846;

// 辅助函数：更新自由漫游相机的前向向量
void updateFreeLookCameraVectors() {
    // 确保俯仰角在合理范围 (避免相机倒立)
    if (g_pitch > 89.0f) g_pitch = 89.0f;
    if (g_pitch < -89.0f) g_pitch = -89.0f;

    // 计算新的前向向量
    float yawRad = g_yaw * M_PI / 180.0f;
    float pitchRad = g_pitch * M_PI / 180.0f;

    g_cameraFrontX = cos(yawRad) * cos(pitchRad);
    g_cameraFrontY = sin(pitchRad);
    g_cameraFrontZ = sin(yawRad) * cos(pitchRad);
}

//辅助函数：获取相机当前的右向量
void getCameraRightVector(float& rightX, float& rightY, float& rightZ) {
    // 计算右向向量 (正交于前向和向上)
    if (g_currentCameraMode == FREE_LOOK_CAMERA) {
    rightX = g_cameraFrontY * g_cameraUpZ - g_cameraFrontZ * g_cameraUpY;
    rightY = g_cameraFrontZ * g_cameraUpX - g_cameraFrontX * g_cameraUpZ;
    rightZ = g_cameraFrontX * g_cameraUpY - g_cameraFrontY * g_cameraUpX;
}
    else{
     float camX, camY, camZ;
     float yawRad = g_orbitalYaw * M_PI / 180.0f;
     float pitchRad = g_orbitalPitch * M_PI / 180.0f;
     camX = g_targetX + g_orbitalDistance * sin(yawRad) * cos(pitchRad);
     camY = g_orbitalDistance * sin(pitchRad);
     camZ = g_orbitalDistance * cos(yawRad) * cos(pitchRad);
     // 从相机位置到目标点的向量 (-camX, -camY, -camZ)
     float currentFrontX = -camX;
     float currentFrontY = -camY;
     float currentFrontZ = -camZ;
     // 叉积计算右向量
     rightX = currentFrontY * g_cameraFrontZ - currentFrontZ * g_cameraFrontY;
     rightY = currentFrontZ * g_cameraFrontX - currentFrontX * g_cameraFrontZ;
     rightZ = currentFrontX * g_cameraFrontY - currentFrontY * g_cameraFrontX;
}
    float len = sqrt(rightX * rightX + rightY* rightY + rightZ * rightZ);
    if (len > 0.0001f) {
        rightX /= len; rightY /= len; rightZ /= len;
    }
}

// 辅助函数：获取相机当前的向上向量
void getCameraUpVector(float& uX, float& uY, float& uZ) {
    float rx, ry, rz;
    getCameraRightVector(rx, ry, rz);
	float currentFrontX, currentFrontY, currentFrontZ;
	if (g_currentCameraMode == FREE_LOOK_CAMERA) {
		currentFrontX = g_cameraFrontX;
		currentFrontY = g_cameraFrontY;
		currentFrontZ = g_cameraFrontZ;
    }
    else {
        float camX, camY, camZ;
        float yawRad = g_orbitalYaw * M_PI / 180.0f;
        float pitchRad = g_orbitalPitch * M_PI / 180.0f;

        camX = g_orbitalDistance * cos(pitchRad) * sin(yawRad);
        camY = g_orbitalDistance * sin(pitchRad);
        camZ = g_orbitalDistance * cos(pitchRad) * cos(yawRad);

        currentFrontX = -camX;
        currentFrontY = -camY;
        currentFrontZ = -camZ;
	}
    // 叉积计算向上向量
    uX = currentFrontY * rz - currentFrontZ * ry;
    uY = currentFrontZ * rx - currentFrontX * rz;
    uZ = currentFrontX * ry - currentFrontY * rx;

    float len = sqrt(uX * uX + uY * uY + uZ * uZ);
    if (len > 0.0001f) {
        uX /= len; uY /= len; uZ /= len;
    }
}
void processNormalKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC键退出
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
    case 'w': // 相机前进
    case 'W':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            g_cameraPosX += g_cameraFrontX * CAMERA_SPEED;
            g_cameraPosY += g_cameraFrontY * CAMERA_SPEED;
            g_cameraPosZ += g_cameraFrontZ * CAMERA_SPEED;
        }
        else { // 轨道相机：拉近距离
            g_orbitalDistance -= ORBITAL_ZOOM_SPEED;
            if (g_orbitalDistance < 0.1f) g_orbitalDistance = 0.1f; // 避免距离过近
        }
        break;
    case 's': // 相机后退
    case 'S':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            g_cameraPosX -= g_cameraFrontX * CAMERA_SPEED;
            g_cameraPosY -= g_cameraFrontY * CAMERA_SPEED;
            g_cameraPosZ -= g_cameraFrontZ * CAMERA_SPEED;
        }
        else { // 轨道相机：拉远距离
            g_orbitalDistance += ORBITAL_ZOOM_SPEED;
        }
        break;
    case 'a': // 相机左移
    case 'A':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            // 计算右向向量 (正交于前向和向上)
            // 简单的叉积: front x up
            float rightX = g_cameraFrontY * g_cameraUpZ - g_cameraFrontZ * g_cameraUpY;
            float rightY = g_cameraFrontZ * g_cameraUpX - g_cameraFrontX * g_cameraUpZ;
            float rightZ = g_cameraFrontX * g_cameraUpY - g_cameraFrontY * g_cameraUpX;
            // 归一化右向向量
            float len = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
            if (len > 0.0001f) {
                rightX /= len; rightY /= len; rightZ /= len;
            }

            g_cameraPosX -= rightX * CAMERA_SPEED;
            g_cameraPosY -= rightY * CAMERA_SPEED;
            g_cameraPosZ -= rightZ * CAMERA_SPEED;
        }
        break;
    case 'd': // 相机右移
    case 'D':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            float rightX = g_cameraFrontY * g_cameraUpZ - g_cameraFrontZ * g_cameraUpY;
            float rightY = g_cameraFrontZ * g_cameraUpX - g_cameraFrontX * g_cameraUpZ;
            float rightZ = g_cameraFrontX * g_cameraUpY - g_cameraFrontY * g_cameraUpX;
            float len = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
            if (len > 0.0001f) {
                rightX /= len; rightY /= len; rightZ /= len;
            }

            g_cameraPosX += rightX * CAMERA_SPEED;
            g_cameraPosY += rightY * CAMERA_SPEED;
            g_cameraPosZ += rightZ * CAMERA_SPEED;
        }
        break;
    case'q':
    case'Q':
        if (g_currentCameraMode == FREE_LOOK_CAMERA)
        {
			g_cameraPosY += CAMERA_SPEED; // 向上移动
        }
        break;
    case'e':
	case'E':
		if (g_currentCameraMode == FREE_LOOK_CAMERA)
		{
			g_cameraPosY -= CAMERA_SPEED; // 向下移动
		}
        break;
    default:
        break;
    }
    glutPostRedisplay(); // 触发重绘
}

void processSpecialKeys(int key, int x, int y) {
    //switch (key) {
    //case GLUT_KEY_UP:
    //    g_cameraY += 0.1f;
    //    break;
    //case GLUT_KEY_DOWN:
    //    g_cameraY -= 0.1f;
    //    break;
    //case GLUT_KEY_LEFT:
    //    g_cameraX -= 0.1f;
    //    break;
    //case GLUT_KEY_RIGHT:
    //    g_cameraX += 0.1f;
    //    break;
    //default:
    //    break;
    //}
    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            g_isRotating = true; // 激活旋转状态
            g_lastMouseX = x;
            g_lastMouseY = y;
        }
        else {
            g_isRotating = false; // 关闭旋转状态
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
    else if (button == 3) { // 鼠标滚轮向上 (放大)
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_orbitalDistance -= ORBITAL_ZOOM_SPEED;
            if (g_orbitalDistance < 0.1f) g_orbitalDistance = 0.1f;
        }
        else { // FREE_LOOK_CAMERA
            g_cameraPosX += g_cameraFrontX * FREE_LOOK_ZOOM_SPEED;
            g_cameraPosY += g_cameraFrontY * FREE_LOOK_ZOOM_SPEED;
            g_cameraPosZ += g_cameraFrontZ * FREE_LOOK_ZOOM_SPEED;
        }
        glutPostRedisplay();
    }
    else if (button == 4) { // 鼠标滚轮向下 (缩小)
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_orbitalDistance += ORBITAL_ZOOM_SPEED;
        }
        else { // FREE_LOOK_CAMERA
            g_cameraPosX -= g_cameraFrontX * FREE_LOOK_ZOOM_SPEED;
            g_cameraPosY -= g_cameraFrontY * FREE_LOOK_ZOOM_SPEED;
            g_cameraPosZ -= g_cameraFrontZ * FREE_LOOK_ZOOM_SPEED;
        }
        glutPostRedisplay();
    }
}

void mouseMove(int x, int y) {
    // 如果没有鼠标按键被按下，直接返回
    if (!g_isRotating && !g_isPanning) {
        return;
    }

    float deltaX = (float)(x - g_lastMouseX);
    float deltaY = (float)(y - g_lastMouseY);

    if (g_isRotating) { // 右键旋转
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_orbitalYaw -= deltaX * ROTATION_SENSITIVITY;
            g_orbitalPitch -= deltaY * ROTATION_SENSITIVITY; // 鼠标向上移动，俯仰角增大

            // 限制俯仰角，避免翻转
            if (g_orbitalPitch > 89.0f) g_orbitalPitch = 89.0f;
            if (g_orbitalPitch < -89.0f) g_orbitalPitch = -89.0f;
        }
        else { // FREE_LOOK_CAMERA
            g_yaw += deltaX * ROTATION_SENSITIVITY;
            g_pitch -= deltaY * ROTATION_SENSITIVITY; // 鼠标向上移动，俯仰角增大
            updateFreeLookCameraVectors(); // 更新前向向量
        }
    }
    else if (g_isPanning) { // 中键平移
        if (!g_isRotating && !g_isPanning) {
            return;
        }

        float deltaX = (float)(x - g_lastMouseX);
        float deltaY = (float)(y - g_lastMouseY);

        if (g_isRotating) { // 右键按下：现在用于绕模型旋转
            if (g_currentCameraMode == ORBITAL_CAMERA) {
                // **核心修改：调整 g_orbitalYaw 和 g_orbitalPitch 的符号**

                // 鼠标向右拖动 (deltaX > 0):
                // 如果你希望模型感觉“向左转”（即相机向右绕模型转），那么 g_orbitalYaw 增加。
                // g_orbitalYaw += deltaX * ROTATION_SENSITIVITY; // 常见设置

                // 如果你希望模型感觉“向右转”（即相机向左绕模型转），那么 g_orbitalYaw 减少。
                g_orbitalYaw -= deltaX * ROTATION_SENSITIVITY; // **尝试这个，通常更符合直觉**

                // 鼠标向上拖动 (deltaY < 0):
                // 如果你希望模型感觉“向上转”（即相机向上仰视），那么 g_orbitalPitch 减小。
                g_orbitalPitch -= deltaY * ROTATION_SENSITIVITY; // **尝试这个，通常更符合直觉**

                // 如果你希望模型感觉“向下转”（即相机向下俯视），那么 g_orbitalPitch 增加。
                // g_orbitalPitch += deltaY * ROTATION_SENSITIVITY;


                // 限制俯仰角，避免翻转
                if (g_orbitalPitch > 89.0f) g_orbitalPitch = 89.0f;
                if (g_orbitalPitch < -89.0f) g_orbitalPitch = -89.0f;

            }
            else { // FREE_LOOK_CAMERA: 自由视角旋转 (保持不变，或根据需要微调 yaw/pitch 符号)
                g_yaw += deltaX * ROTATION_SENSITIVITY;
                g_pitch -= deltaY * ROTATION_SENSITIVITY;
                updateFreeLookCameraVectors();
            }
        }
        else if (g_isPanning) { // 中键按下：平移 (保持不变，确保速度合理)
            float rightX, rightY, rz;
            getCameraRightVector(rightX, rightY, rz);

            float realUpX, realUpY, realUpZ;
            getCameraUpVector(realUpX, realUpY, realUpZ);

            // 平移速度因子，与相机距离相关，使远近平移手感一致
            float panScale = g_orbitalDistance * PAN_SPEED_FACTOR; // 调整这个因子

            if (g_currentCameraMode == ORBITAL_CAMERA) {
                // 平移目标点
                g_targetX -= rightX * deltaX * panScale;
                g_targetY -= realUpY * deltaY * panScale;
                g_targetZ -= rz * deltaX * panScale; // 修正：平移时 Z 轴也可能受影响
            }
            else { // FREE_LOOK_CAMERA: 平移相机自身
                g_cameraPosX -= rightX * deltaX * panScale;
                g_cameraPosY -= realUpY * deltaY * panScale;
                g_cameraPosZ -= rz * deltaX * panScale;
            }
        }

        g_lastMouseX = x;
        g_lastMouseY = y;
        glutPostRedisplay();
    }

    g_lastMouseX = x;
    g_lastMouseY = y;
    glutPostRedisplay();
}