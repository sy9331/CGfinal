#include <iostream>
#include "../Header/parser1.h"
#include "../Header/render.h"
#include "../Header/interaction.h"
#include <glut.h> 

void initOpenGL() {
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 启用光照
    glEnable(GL_LIGHTING);
    // 启用光源0
    glEnable(GL_LIGHT0);

    // 设置光源0的位置 (点光源在相机位置)
    // 根据你的需求可以设置为固定位置 (例如 [0, 0, 10, 1]) 或移动光源
    float light_position[] = { 0.0f, 0.0f, 5.0f, 1.0f }; // x, y, z, w (w=1.0f表示点光源, w=0.0f表示方向光)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // 设置光源0的颜色
    float light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


    // 设置平滑着色
    glShadeModel(GL_SMOOTH);

    // 设置背景颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 深灰色背景
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Computer Graphics Project - Model Viewer");

    initOpenGL();

    // 加载模型
    if (!loadModel("assets/models/luweiqi.txt")) {
        std::cerr << "Failed to load model. Exiting." << std::endl;
        return 1;
    }

    // 注册回调函数
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
 

    glutMainLoop();

    return 0;
}