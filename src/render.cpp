#include "../Header/render.h"
#include <string>
#include <cmath>
#include<math.h>

// ȫ����Ⱦ״̬��������
    RenderMode g_currentRenderMode = FILL_MODE;
    bool g_enableTexture = true;
    bool g_enableMaterial = true;
    bool g_displayCoordinates = false;
    bool g_displayInfo = true;
    
    // �����������
    float g_cameraPosX = 0.0f, g_cameraPosY = 0.0f, g_cameraPosZ = .0f;
    float g_cameraFrontX = 0.0f, g_cameraFrontY = 0.0f, g_cameraFrontZ = -1.0f; // ��ʼ��ǰ��Z�Ḻ����
    float g_cameraUpX = 0.0f, g_cameraUpY = 1.0f, g_cameraUpZ = 0.0f;

    float g_yaw = -90.0f;  
    float g_pitch = 0.0f;


	CameraMode g_currentCameraMode = ORBITAL_CAMERA; // Ĭ�Ϲ�����ģʽ
    float g_orbitalDistance = 5.0f;
    float g_orbitalYaw = 0.0f;   
    float g_orbitalPitch = 0.0f; 
    float g_targetX = 0.0f, g_targetY = 0.0f, g_targetZ = 0.0f; 
    const double M_PI = 3.14159265358979323846;
    
    // �����ı�����
    void drawText(float x, float y, const char* text) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
    
        // ���ù��պ���Ȳ��ԣ�ȷ���ı������ɼ�
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_2D);
    
        glColor3f(1.0f, 1.0f, 1.0f); // ��ɫ�ı�
        glRasterPos2f(x, y);
        for (const char* c = text; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    
        // �ָ�֮ǰ��״̬
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING); // ���֮ǰ�����˹���
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
    
    
    void renderScene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        // �������λ��
        if (g_currentCameraMode == ORBITAL_CAMERA) {
			glTranslated(-g_targetX, -g_targetY, -g_targetZ); 
			glRotatef(g_orbitalYaw, 0.0f, 1.0f, 0.0f); 
			glRotatef(g_orbitalPitch, 1.0f, 0.0f, 0.0f); 
			glTranslatef(0.0f, 0.0f, -g_orbitalDistance); 
        }
        else { // FREE_LOOK_CAMERA
            // �����������������λ�ú�ǰ����������
            // Ŀ��� = ���λ�� + ���ǰ������
            gluLookAt(g_cameraPosX, g_cameraPosY, g_cameraPosZ,
                g_cameraPosX + g_cameraFrontX, g_cameraPosY + g_cameraFrontY, g_cameraPosZ + g_cameraFrontZ,
                g_cameraUpX, g_cameraUpY, g_cameraUpZ);
        }

        // ��������ϵ (��չĿ��)
        if (g_displayCoordinates) {
            glDisable(GL_LIGHTING); // ���ù��ջ�������ϵ
            glLineWidth(2.0f);
            glBegin(GL_LINES);
            // X�� (��ɫ)
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);
            // Y�� (��ɫ)
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            // Z�� (��ɫ)
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 1.0f);
            glEnd();
            glLineWidth(1.0f); // �ָ�Ĭ���߿�
            glEnable(GL_LIGHTING); // �ָ�����
        }
    
    
        // Ӧ��ȫ������
        glScalef(g_model.scale[0], g_model.scale[1], g_model.scale[2]);
    
        // ������Ⱦģʽ���ö���λ��Ʒ�ʽ
        switch (g_currentRenderMode) {
        case FILL_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case LINE_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case POINT_MODE:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glPointSize(2.0f); // ��ģʽ�µĵ��С
            break;
        }
    
        // ��Ⱦģ��
        for (const auto& subModel : g_model.subModels) {
            // ���ò���
            if (g_enableMaterial && subModel.materialIndex > 0 && subModel.materialIndex <= g_model.materials.size()) {
                const Material& mat = g_model.materials[subModel.materialIndex - 1]; // ����-1
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat.ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular);
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat.emission);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat.shininess);
            }
            else {
                // ������ò��ʻ����������Ч��ʹ��Ĭ�ϲ���
                float default_color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, default_color);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
            }
    
    
            // ������
            if (g_enableTexture && subModel.materialIndex > 0 && subModel.materialIndex <= g_model.materials.size()) {
                const Material& mat = g_model.materials[subModel.materialIndex - 1];
                if (mat.textureIndex > 0 && mat.textureIndex <= g_model.textures.size() && g_model.textures[mat.textureIndex - 1].id != 0) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, g_model.textures[mat.textureIndex - 1].id);
                }
                else {
                    glDisable(GL_TEXTURE_2D); // û�����������������Ч����������
                }
            }
            else {
                glDisable(GL_TEXTURE_2D); // ���g_enableTextureΪfalse����������
            }
    
    
            glBegin(GL_TRIANGLES);
            for (const auto& face : subModel.faces) {
                for (int i = 0; i < 3; ++i) {
                    // ע��������Ҫ -1
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
    
            glDisable(GL_TEXTURE_2D); // ÿ����ģ����Ⱦ��������������Ӱ����һ��
        }
        // ��ʾ��ʾ��Ϣ (��չĿ��)
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
    
            // ������������Ӹ�����Ϣ���������λ�õ�
            char cameraInfo[200];
            if (g_currentCameraMode == ORBITAL_CAMERA) {
                sprintf_s(cameraInfo, sizeof(cameraInfo), "Camera Mode: Orbital (Dist: %.2f, Yaw: %.0f, Pitch: %.0f)",
                    g_orbitalDistance, g_orbitalYaw, g_orbitalPitch);
            }
            else {
                sprintf_s(cameraInfo, sizeof(cameraInfo), "Camera Mode: Free Look (Pos: %.2f,%.2f,%.2f | Yaw:%.0f, Pitch:%.0f)",
                    g_cameraPosX, g_cameraPosY, g_cameraPosZ, g_yaw, g_pitch);
            }
            drawText(10, glutGet(GLUT_WINDOW_HEIGHT) - 100, cameraInfo);
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
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_orbitalDistance = 5.0f;
            g_orbitalYaw = -90.0f;
            g_orbitalPitch = 0.0f;
            g_targetX = 0.0f; g_targetY = 0.0f; g_targetZ = 0.0f;
        }
        else {
            g_cameraPosX = 0.0f; g_cameraPosY = 0.0f; g_cameraPosZ = 5.0f;
            g_cameraFrontX = 0.0f; g_cameraFrontY = 0.0f; g_cameraFrontZ = -1.0f;
            g_cameraUpX = 0.0f; g_cameraUpY = 1.0f; g_cameraUpZ = 0.0f;
            g_yaw = -90.0f;
            g_pitch = 0.0f;
        }
        glutPostRedisplay();
    }
    
    void toggleFillMode() { g_currentRenderMode = FILL_MODE; glutPostRedisplay(); }
    void toggleLineMode() { g_currentRenderMode = LINE_MODE; glutPostRedisplay(); }
    void togglePointMode() { g_currentRenderMode = POINT_MODE; glutPostRedisplay(); }
    void toggleTexture() { g_enableTexture = !g_enableTexture; glutPostRedisplay(); }
    void toggleMaterial() { g_enableMaterial = !g_enableMaterial; glutPostRedisplay(); }
    void toggleCoordinatesDisplay() { g_displayCoordinates = !g_displayCoordinates; glutPostRedisplay(); }
    void toggleInfoDisplay() { g_displayInfo = !g_displayInfo; glutPostRedisplay(); }
    // ���ģʽ�л�����
    void toggleCameraMode() {
        if (g_currentCameraMode == ORBITAL_CAMERA) {
            g_currentCameraMode = FREE_LOOK_CAMERA;
            // �л�ģʽʱ���������������������λ�ýӽ���������λ�ã��Լ�������
            // �򵥵�ת��������������λ�ø��Ƹ������������
            float yawRad = g_orbitalYaw * M_PI / 180.0f;
            float pitchRad = g_orbitalPitch * M_PI / 180.0f;
            g_cameraPosX = g_targetX + g_orbitalDistance * cos(pitchRad) * sin(yawRad);
            g_cameraPosY = g_targetY + g_orbitalDistance * sin(pitchRad);
            g_cameraPosZ = g_targetZ + g_orbitalDistance * cos(pitchRad) * cos(yawRad);
            // ���ݹ������ĳ��򣬼����������������yaw��pitch
            // ����ֻ��һ�����ԵĹ��ƣ�����ȷ����Ҫʹ��atan2��
            // ���Ŀ�����ԭ�� (0,0,0)
            // �������������ǰ�������� (g_targetX - g_cameraPosX, g_targetY - g_cameraPosY, g_targetZ - g_cameraPosZ)
            // Ȼ���һ��������yaw��pitch
            float dx = g_targetX - g_cameraPosX;
            float dy = g_targetY - g_cameraPosY;
            float dz = g_targetZ - g_cameraPosZ;
            float len = sqrt(dx * dx + dy * dy + dz * dz);
            if (len > 0.0001f) {
                g_cameraFrontX = dx / len;
                g_cameraFrontY = dy / len;
                g_cameraFrontZ = dz / len;

                g_yaw = atan2(g_cameraFrontX, g_cameraFrontZ) * 180.0f / M_PI; // ע��OpenGL��Z�ᳯ��� atan2 �Ĳ���
                g_pitch = asin(g_cameraFrontY) * 180.0f / M_PI;
            }
        }
        else {
            g_currentCameraMode = ORBITAL_CAMERA;
            // �л��ع�����ʱ��ͨ���Ὣ�������Ĳ�������ΪĬ��ֵ���߸����������������λ������һ�����ĵ�
            // ����򵥵����ù�����
            resetView();
        }
        glutPostRedisplay();
    }