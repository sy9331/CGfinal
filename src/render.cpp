#include "../Header/render.h"
#include <string>
#include <cmath>
#include<math.h>

// 全局渲染状态变量定义
    ProjectionMode g_currentProjectionMode = PERSPECTIVE_PROJECTION;
    RenderMode g_currentRenderMode = FILL_MODE;
    bool g_enableTexture = true;
    bool g_enableMaterial = true;
    bool g_displayCoordinates = false;
    bool g_displayInfo = true;
    
    // 相机变量定义
    float g_orbitalDistance = 5.0f;
    float g_orbitalYaw = 0.0f;   
    float g_orbitalPitch = 0.0f; 
    float g_targetX = 0.0f, g_targetY = 0.0f, g_targetZ = 0.0f; 
    const double M_PI = 3.14159265358979323846;
    
    // 其他场景状态变量 
    bool g_textureEnabled = false;
    bool g_materialEnabled = true;
    GLenum g_polygonMode = GL_FILL;

    // 绘制窗口左上方文本
    void drawText(float x, float y, const char* text) {
		// 保存当前矩阵状态
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        // 设置文本颜色和位置
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f); 
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
		//恢复状态
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING); 
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    //绘制坐标轴
    void drawCoordinates() {
        glBegin(GL_LINES);
        // X 
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        // Y 
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        // Z
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glEnd();
    }
    //渲染主循环
    void renderScene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPolygonMode(GL_FRONT_AND_BACK, g_polygonMode);
        // 设置相机位置
		glTranslatef(-g_targetX, -g_targetY, -g_targetZ);
		glRotatef(g_orbitalYaw, 0.0f, 1.0f, 0.0f); 
		glRotatef(g_orbitalPitch, 1.0f, 0.0f, 0.0f); 
		glTranslatef(0.0f, 0.0f, -g_orbitalDistance); 
        gluLookAt(0.0f, 0.0f, 0.0f,  
            0.0f, 0.0f, -1.0f, 
            0.0f, 1.0f, 0.0f);  
       
        // 绘制坐标系
        if (g_displayCoordinates) {
            drawCoordinates();
        }
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
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
                    glDisable(GL_TEXTURE_2D); 
                }
            }
            else {
                glDisable(GL_TEXTURE_2D); 
            }
    
			// 绘制子模型的三角形面
            glBegin(GL_TRIANGLES);
            for (const auto& face : subModel.faces) {
                for (int i = 0; i < 3; ++i) {
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

			// 解绑纹理
            glDisable(GL_TEXTURE_2D); 
        }
        
        // 显示提示信息
        if (g_displayInfo) {
            std::string mode_str;
            switch (g_currentRenderMode) {
            case FILL_MODE: mode_str = "Fill Mode"; break;
            case LINE_MODE: mode_str = "Line Mode"; break;
            case POINT_MODE: mode_str = "Point Mode"; break;
            }
			// 在窗口左上角显示当前渲染模式和其他信息
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 100, "Press A/D to zoom in or zoom out");
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, ("Mode:(Press 1/2/3) " + mode_str).c_str());
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 40, (g_enableTexture ? "Texture(Press:T): ON" : "Texture(Press:T): OFF"));
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 60, (g_enableMaterial ? "Material(Press:M): ON" : "Material(Press:M): OFF"));
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 80, (g_displayCoordinates ? "Coords(Press:C): ON" : "Coords(Press:C): OFF"));
            char cameraInfo[200];
            sprintf_s(cameraInfo, sizeof(cameraInfo), "Camera Mode: Orbital (Dist: %.2f, Yaw: %.0f, Pitch: %.0f)",
                    g_orbitalDistance, g_orbitalYaw, g_orbitalPitch);
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 100, cameraInfo);
			if (g_currentProjectionMode == PERSPECTIVE_PROJECTION) {
				drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 120, "Projection(press:P): Perspective");
			}
			else {
				drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 120, "Projection(press:P): Orthographic");
			}
        }
        glutSwapBuffers();
    }

	// 窗口大小变化和设置投影
    void reshape(int w, int h) {
        if (h == 0) h = 1;
        float ratio = (float)w / h;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (g_currentProjectionMode == PERSPECTIVE_PROJECTION) {
            gluPerspective(45.0f, ratio, 0.1f, 100.0f); 
        }
        else { 
            float orthoSize = g_orbitalDistance / 2.0f; 
            glOrtho(-orthoSize * ratio, orthoSize * ratio,
                -orthoSize, orthoSize,
                -100.0f, 100.0f); 
        }
        glViewport(0, 0, w, h);
        glutPostRedisplay();
    }

	// 重置视图函数
    void resetView() {
        g_orbitalDistance = 5.0f;
        g_orbitalYaw = -90.0f;
        g_orbitalPitch = 0.0f;
        g_targetX = 0.0f; g_targetY = 0.0f; g_targetZ = 0.0f;
        glutPostRedisplay();
    }
    
	// 渲染模式切换函数
    void toggleFillMode() { g_currentRenderMode = FILL_MODE;std::cout << "DEBUG: Polygon Mode: GL_FILL" << std::endl;  glutPostRedisplay(); }
    void toggleLineMode() { g_currentRenderMode = LINE_MODE;std::cout << "DEBUG: Polygon Mode: GL_LINE" << std::endl; glutPostRedisplay(); }
    void togglePointMode() { g_currentRenderMode = POINT_MODE;std::cout << "DEBUG: Polygon Mode: GL_POINT" << std::endl; glutPostRedisplay(); }
    void toggleTexture() { g_enableTexture = !g_enableTexture; glutPostRedisplay(); }
    void toggleMaterial() { g_enableMaterial = !g_enableMaterial; glutPostRedisplay(); }
    void toggleCoordinatesDisplay() { g_displayCoordinates = !g_displayCoordinates; glutPostRedisplay(); }
    void toggleInfoDisplay() { g_displayInfo = !g_displayInfo; glutPostRedisplay(); }
    
	// 切换投影模式函数
    void toggleProjectionMode() {
        if (g_currentProjectionMode == PERSPECTIVE_PROJECTION) {
            g_currentProjectionMode = ORTHOGRAPHIC_PROJECTION;
            std::cout << "DEBUG: Switched to ORTHOGRAPHIC_PROJECTION mode." << std::endl;
        }
        else {
            g_currentProjectionMode = PERSPECTIVE_PROJECTION;
            std::cout << "DEBUG: Switched to PERSPECTIVE_PROJECTION mode." << std::endl;
        }
        // 切换模式后，必须重新设置投影矩阵
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
  