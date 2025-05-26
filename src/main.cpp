#include <iostream>
#include "../Header/parser1.h"
#include "../Header/render.h"
#include "../Header/interaction.h"
#include <glut.h> // ���� #include <GL/glut.h>
// ȫ�ֱ������� (��render.cpp��interaction.cpp��������)
// Model g_model; // ��parser1.cpp�ж��岢��ʼ��
// RenderMode g_currentRenderMode; // ��render.cpp�ж��岢��ʼ��
// bool g_enableTexture, g_enableMaterial, g_displayCoordinates, g_displayInfo; // render.cpp
// float g_cameraX, g_cameraY, g_cameraZ, g_lookAtX, g_lookAtY, g_lookAtZ, g_upX, g_upY, g_upZ; // render.cpp
// float g_rotateX, g_rotateY; // render.cpp
// int g_lastMouseX, g_lastMouseY; // interaction.cpp
// bool g_isRotating, g_isPanning; // interaction.cpp


void initOpenGL() {
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // ���ù���
    glEnable(GL_LIGHTING);
    // ���ù�Դ0
    glEnable(GL_LIGHT0);

    // ���ù�Դ0��λ�� (���Դ�����λ��)
    // ������������������Ϊ�̶�λ�� (���� [0, 0, 10, 1]) ���ƶ���Դ
    float light_position[] = { 0.0f, 0.0f, 5.0f, 1.0f }; // x, y, z, w (w=1.0f��ʾ���Դ, w=0.0f��ʾ�����)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // ���ù�Դ0����ɫ
    float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


    // ����ƽ����ɫ
    glShadeModel(GL_SMOOTH);

    // ���ñ�����ɫ
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // ���ɫ����
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Computer Graphics Project - Model Viewer");

    initOpenGL();

    // ����ģ��
    // ȷ����� luweiqi.txt �ļ��� assets/models Ŀ¼��
    if (!loadModel("assets/models/luweiqi.txt")) {
        std::cerr << "Failed to load model. Exiting." << std::endl;
        return 1;
    }

    // ע��ص�����
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    // glutIdleFunc(renderScene); // �����Ҫ������������������

    glutMainLoop();

    return 0;
}