#include "model.h"
#include <fstream>
#include <iostream>
#include<glut.h>

std::vector<Vertex> vertices;
std::vector<unsigned int> indices;

bool loadModel(const std::string& filename) {
    std::ifstream file(filename);  
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    int numVertices = 0, numTriangles = 0;
    file >> numVertices >> numTriangles; 
    // 读取顶点数据
    for (int i = 0; i < numVertices; ++i) {
        float x, y, z;
        file >> x >> y >> z;
        vertices.push_back({ x, y, z });
    }
    // 读取三角形索引数据
    for (int i = 0; i < numTriangles; ++i) {
        int flag, i1, i2, i3;
        file >> flag >> i1 >> i2 >> i3;    // 读取 3 + 三个索引
        if (flag != 3) {
            std::cerr << "错误：三角形标志不是3。" << std::endl;
            return false;
        }
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }
    file.close(); // 关闭文件
    return true;
}
// draw function
void drawModel() {
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < indices.size(); ++i) {
        Vertex& v = vertices[indices[i]];
        glVertex3f(v.x, v.y, v.z);
    }
    glEnd();
}
float rotateX = 0.0f, rotateY = 0.0f;
float zoom = 1.0f;
int lastX, lastY;
bool mouseDown = false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glScalef(zoom, zoom, zoom);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);

    drawModel();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        mouseDown = (state == GLUT_DOWN);
        lastX = x;
        lastY = y;
    }
}

void motion(int x, int y) {
    if (mouseDown) {
        rotateY += (x - lastX);
        rotateX += (y - lastY);
        lastX = x;
        lastY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == '+') zoom *= 1.1f;
    if (key == '-') zoom *= 0.9f;
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    if (!loadModel("what.txt")) {
        std::cerr << "模型加载失败！" << std::endl;
        return -1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("网格模型观察");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

