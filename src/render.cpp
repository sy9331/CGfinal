#include "../Header/render.h"
#include <string>

// 全局渲染状态变量定义
    RenderMode g_currentRenderMode = FILL_MODE;
    bool g_enableTexture = true;
    bool g_enableMaterial = true;
    bool g_displayCoordinates = false;
    bool g_displayInfo = true;
    
    // 相机变量定义
    float g_cameraX = 0.0f, g_cameraY = 0.0f, g_cameraZ = 5.0f;
    float g_lookAtX = 0.0f, g_lookAtY = 0.0f, g_lookAtZ = 0.0f;
    float g_upX = 0.0f, g_upY = 1.0f, g_upZ = 0.0f;
    
    // 旋转变量定义
    float g_rotateX = 0.0f;
    float g_rotateY = 0.0f;
    
    // 绘制文本函数
    void drawText(float x, float y, const char* text) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    
        // 禁用光照和深度测试，确保文本清晰可见
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
    
        glColor3f(1.0f, 1.0f, 1.0f); // 白色文本
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    
        // 恢复之前的状态
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING); // 如果之前开启了光照
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    
    
    void renderScene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        // 设置相机位置
        gluLookAt(g_cameraX, g_cameraY, g_cameraZ,
            g_lookAtX, g_lookAtY, g_lookAtZ,
            g_upX, g_upY, g_upZ);
    
        // 应用模型旋转
        glRotatef(g_rotateX, 1.0f, 0.0f, 0.0f);
        glRotatef(g_rotateY, 0.0f, 1.0f, 0.0f);
    
        // 绘制坐标系 (拓展目标)
        if (g_displayCoordinates) {
            glDisable(GL_LIGHTING); // 禁用光照绘制坐标系
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            // X轴 (红色)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);
            // Y轴 (绿色)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            // Z轴 (蓝色)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 1.0f);
            glEnd();
            glLineWidth(1.0f); // 恢复默认线宽
            glEnable(GL_LIGHTING); // 恢复光照
        }
    
    
        // 应用全局缩放
        glScalef(g_model.scale[0], g_model.scale[1], g_model.scale[2]);
    
        // 根据渲染模式设置多边形绘制方式
        switch (g_currentRenderMode) {
        case FILL_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case LINE_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case POINT_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glPointSize(2.0f); // 点模式下的点大小
            break;
        }
    
        // 渲染模型
        for (const auto& subModel : g_model.subModels) {
            // 设置材质
            if (g_enableMaterial && subModel.materialIndex > 0 && subModel.materialIndex <= g_model.materials.size()) {
                const Material& mat = g_model.materials[subModel.materialIndex - 1]; // 索引-1
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat.emission);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
            }
            else {
                // 如果禁用材质或材质索引无效，使用默认材质
                float default_color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, default_color);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
            }
    
    
            // 绑定纹理
            if (g_enableTexture && subModel.materialIndex > 0 && subModel.materialIndex <= g_model.materials.size()) {
                const Material& mat = g_model.materials[subModel.materialIndex - 1];
                if (mat.textureIndex > 0 && mat.textureIndex <= g_model.textures.size() && g_model.textures[mat.textureIndex - 1].id != 0) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, g_model.textures[mat.textureIndex - 1].id);
                }
                else {
                    glDisable(GL_TEXTURE_2D); // 没有纹理或纹理索引无效，禁用纹理
                }
            }
            else {
                glDisable(GL_TEXTURE_2D); // 如果g_enableTexture为false，禁用纹理
            }
    
    
            glBegin(GL_TRIANGLES);
            for (const auto& face : subModel.faces) {
                for (int i = 0; i < 3; ++i) {
                    // 注意索引需要 -1
                    if (!g_model.normals.empty() && face.n_indices[i] > 0 && face.n_indices[i] <= g_model.normals.size()) {
                        glNormal3fv(&g_model.normals[face.n_indices[i] - 1].x);
                    }
                    if (!g_model.texCoords.empty() && face.t_indices[i] > 0 && face.t_indices[i] <= g_model.texCoords.size() && g_enableTexture) {
                        glTexCoord2fv(&g_model.texCoords[face.t_indices[i] - 1].u);
                    }
                    if (!g_model.vertices.empty() && face.v_indices[i] > 0 && face.v_indices[i] <= g_model.vertices.size()) {
                        glVertex3fv(&g_model.vertices[face.v_indices[i] - 1].x);
                    }
                }
            }
            glEnd();
    
            glDisable(GL_TEXTURE_2D); // 每个子模型渲染完后禁用纹理，避免影响下一个
        }
    
        // 显示提示信息 (拓展目标)
        if (g_displayInfo) {
            std::string mode_str;
            switch (g_currentRenderMode) {
            case FILL_MODE: mode_str = "Fill Mode"; break;
            case LINE_MODE: mode_str = "Line Mode"; break;
            case POINT_MODE: mode_str = "Point Mode"; break;
            }
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, ("Mode: " + mode_str).c_str());
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 40, (g_enableTexture ? "Texture: ON" : "Texture: OFF"));
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 60, (g_enableMaterial ? "Material: ON" : "Material: OFF"));
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 80, (g_displayCoordinates ? "Coords: ON" : "Coords: OFF"));
    
            // 可以在这里添加更多信息，例如相机位置等
            char cameraPos[100];
            sprintf_s(cameraPos, "Camera: (%.2f, %.2f, %.2f)", g_cameraX, g_cameraY, g_cameraZ);
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 100, cameraPos);
        }
    
    
        glutSwapBuffers();
    }
    
    void changeSize(int w, int h) {
        if (h == 0) h = 1;
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)w / (float)h, 0.1f, 1000.0f);
        glMatrixMode(GL_MODELVIEW);
    }
    
    void resetView() {
        g_cameraX = 0.0f;
        g_cameraY = 0.0f;
        g_cameraZ = 5.0f;
        g_lookAtX = 0.0f;
        g_lookAtY = 0.0f;
        g_lookAtZ = 0.0f;
        g_upX = 0.0f;
        g_upY = 1.0f;
        g_upZ = 0.0f;
        g_rotateX = 0.0f;
        g_rotateY = 0.0f;
        glutPostRedisplay();
    }
    
    void toggleFillMode() { g_currentRenderMode = FILL_MODE; glutPostRedisplay(); }
    void toggleLineMode() { g_currentRenderMode = LINE_MODE; glutPostRedisplay(); }
    void togglePointMode() { g_currentRenderMode = POINT_MODE; glutPostRedisplay(); }
    void toggleTexture() { g_enableTexture = !g_enableTexture; glutPostRedisplay(); }
    void toggleMaterial() { g_enableMaterial = !g_enableMaterial; glutPostRedisplay(); }
    void toggleCoordinatesDisplay() { g_displayCoordinates = !g_displayCoordinates; glutPostRedisplay(); }
    void toggleInfoDisplay() { g_displayInfo = !g_displayInfo; glutPostRedisplay(); }