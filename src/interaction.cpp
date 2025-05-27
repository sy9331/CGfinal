#include "../Header/interaction.h"
#include <cmath>
#include<glut.h>
// ��꽻����������
int g_lastMouseX, g_lastMouseY;
bool g_isRotating = false;
bool g_isPanning = false;
bool g_isMovingCamera = false; 

// �ƶ�����ת�ٶ�
const float CAMERA_SPEED = 0.1f;
const float ROTATION_SENSITIVITY = 0.2f; // �����ת������
const float ORBITAL_ZOOM_SPEED = 0.1f;
const float FREE_LOOK_ZOOM_SPEED = 0.1f;
const float PAN_SPEED_FACTOR = 0.05f; // ƽ���ٶ�����
const double M_PI = 3.14159265358979323846;

// ���������������������������ǰ������
void updateFreeLookCameraVectors() {
    // ȷ���������ں���Χ (�����������)
    if (g_pitch > 89.0f) g_pitch = 89.0f;
    if (g_pitch < -89.0f) g_pitch = -89.0f;

    // �����µ�ǰ������
    float yawRad = g_yaw * M_PI / 180.0f;
    float pitchRad = g_pitch * M_PI / 180.0f;

    g_cameraFrontX = cos(yawRad) * cos(pitchRad);
    g_cameraFrontY = sin(pitchRad);
    g_cameraFrontZ = sin(yawRad) * cos(pitchRad);
}

//������������ȡ�����ǰ��������
void getCameraRightVector(float& rightX, float& rightY, float& rightZ) {
    // ������������ (������ǰ�������)
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
     // �����λ�õ�Ŀ�������� (-camX, -camY, -camZ)
     float currentFrontX = -camX;
     float currentFrontY = -camY;
     float currentFrontZ = -camZ;
     // �������������
     rightX = currentFrontY * g_cameraFrontZ - currentFrontZ * g_cameraFrontY;
     rightY = currentFrontZ * g_cameraFrontX - currentFrontX * g_cameraFrontZ;
     rightZ = currentFrontX * g_cameraFrontY - currentFrontY * g_cameraFrontX;
}
    float len = sqrt(rightX * rightX + rightY* rightY + rightZ * rightZ);
    if (len > 0.0001f) {
        rightX /= len; rightY /= len; rightZ /= len;
    }
}

// ������������ȡ�����ǰ����������
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
    // ���������������
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
    case 27: // ESC���˳�
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
    case 'w': // ���ǰ��
    case 'W':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            g_cameraPosX += g_cameraFrontX * CAMERA_SPEED;
            g_cameraPosY += g_cameraFrontY * CAMERA_SPEED;
            g_cameraPosZ += g_cameraFrontZ * CAMERA_SPEED;
        }
        else { // ����������������
            g_orbitalDistance -= ORBITAL_ZOOM_SPEED;
            if (g_orbitalDistance < 0.1f) g_orbitalDistance = 0.1f; // ����������
        }
        break;
    case 's': // �������
    case 'S':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            g_cameraPosX -= g_cameraFrontX * CAMERA_SPEED;
            g_cameraPosY -= g_cameraFrontY * CAMERA_SPEED;
            g_cameraPosZ -= g_cameraFrontZ * CAMERA_SPEED;
        }
        else { // ����������Զ����
            g_orbitalDistance += ORBITAL_ZOOM_SPEED;
        }
        break;
    case 'a': // �������
    case 'A':
        if (g_currentCameraMode == FREE_LOOK_CAMERA) {
            // ������������ (������ǰ�������)
            // �򵥵Ĳ��: front x up
            float rightX = g_cameraFrontY * g_cameraUpZ - g_cameraFrontZ * g_cameraUpY;
            float rightY = g_cameraFrontZ * g_cameraUpX - g_cameraFrontX * g_cameraUpZ;
            float rightZ = g_cameraFrontX * g_cameraUpY - g_cameraFrontY * g_cameraUpX;
            // ��һ����������
            float len = sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
            if (len > 0.0001f) {
                rightX /= len; rightY /= len; rightZ /= len;
            }

            g_cameraPosX -= rightX * CAMERA_SPEED;
            g_cameraPosY -= rightY * CAMERA_SPEED;
            g_cameraPosZ -= rightZ * CAMERA_SPEED;
        }
        break;
    case 'd': // �������
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
			g_cameraPosY += CAMERA_SPEED; // �����ƶ�
        }
        break;
    case'e':
	case'E':
		if (g_currentCameraMode == FREE_LOOK_CAMERA)
		{
			g_cameraPosY -= CAMERA_SPEED; // �����ƶ�
		}
        break;
    default:
        break;
    }
    glutPostRedisplay(); // �����ػ�
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
            g_isRotating = true; // ������ת״̬
            g_lastMouseX = x;
            g_lastMouseY = y;
        }
        else {
            g_isRotating = false; // �ر���ת״̬
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
    else if (button == 3) { // ���������� (�Ŵ�)
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
    else if (button == 4) { // ���������� (��С)
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
    // ���û����갴�������£�ֱ�ӷ���
    if (!g_isRotating && !g_isPanning) {
        return;
    }

    float deltaX = (float)(x - g_lastMouseX);
    float deltaY = (float)(y - g_lastMouseY);

    if (g_isRotating) { // �Ҽ���ת
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_orbitalYaw -= deltaX * ROTATION_SENSITIVITY;
            g_orbitalPitch -= deltaY * ROTATION_SENSITIVITY; // ��������ƶ�������������

            // ���Ƹ����ǣ����ⷭת
            if (g_orbitalPitch > 89.0f) g_orbitalPitch = 89.0f;
            if (g_orbitalPitch < -89.0f) g_orbitalPitch = -89.0f;
        }
        else { // FREE_LOOK_CAMERA
            g_yaw += deltaX * ROTATION_SENSITIVITY;
            g_pitch -= deltaY * ROTATION_SENSITIVITY; // ��������ƶ�������������
            updateFreeLookCameraVectors(); // ����ǰ������
        }
    }
    else if (g_isPanning) { // �м�ƽ��
        if (!g_isRotating && !g_isPanning) {
            return;
        }

        float deltaX = (float)(x - g_lastMouseX);
        float deltaY = (float)(y - g_lastMouseY);

        if (g_isRotating) { // �Ҽ����£�����������ģ����ת
            if (g_currentCameraMode == ORBITAL_CAMERA) {
                // **�����޸ģ����� g_orbitalYaw �� g_orbitalPitch �ķ���**

                // ��������϶� (deltaX > 0):
                // �����ϣ��ģ�͸о�������ת���������������ģ��ת������ô g_orbitalYaw ���ӡ�
                // g_orbitalYaw += deltaX * ROTATION_SENSITIVITY; // ��������

                // �����ϣ��ģ�͸о�������ת���������������ģ��ת������ô g_orbitalYaw ���١�
                g_orbitalYaw -= deltaX * ROTATION_SENSITIVITY; // **���������ͨ��������ֱ��**

                // ��������϶� (deltaY < 0):
                // �����ϣ��ģ�͸о�������ת����������������ӣ�����ô g_orbitalPitch ��С��
                g_orbitalPitch -= deltaY * ROTATION_SENSITIVITY; // **���������ͨ��������ֱ��**

                // �����ϣ��ģ�͸о�������ת������������¸��ӣ�����ô g_orbitalPitch ���ӡ�
                // g_orbitalPitch += deltaY * ROTATION_SENSITIVITY;


                // ���Ƹ����ǣ����ⷭת
                if (g_orbitalPitch > 89.0f) g_orbitalPitch = 89.0f;
                if (g_orbitalPitch < -89.0f) g_orbitalPitch = -89.0f;

            }
            else { // FREE_LOOK_CAMERA: �����ӽ���ת (���ֲ��䣬�������Ҫ΢�� yaw/pitch ����)
                g_yaw += deltaX * ROTATION_SENSITIVITY;
                g_pitch -= deltaY * ROTATION_SENSITIVITY;
                updateFreeLookCameraVectors();
            }
        }
        else if (g_isPanning) { // �м����£�ƽ�� (���ֲ��䣬ȷ���ٶȺ���)
            float rightX, rightY, rz;
            getCameraRightVector(rightX, rightY, rz);

            float realUpX, realUpY, realUpZ;
            getCameraUpVector(realUpX, realUpY, realUpZ);

            // ƽ���ٶ����ӣ������������أ�ʹԶ��ƽ���ָ�һ��
            float panScale = g_orbitalDistance * PAN_SPEED_FACTOR; // �����������

            if (g_currentCameraMode == ORBITAL_CAMERA) {
                // ƽ��Ŀ���
                g_targetX -= rightX * deltaX * panScale;
                g_targetY -= realUpY * deltaY * panScale;
                g_targetZ -= rz * deltaX * panScale; // ������ƽ��ʱ Z ��Ҳ������Ӱ��
            }
            else { // FREE_LOOK_CAMERA: ƽ���������
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