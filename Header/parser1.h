#ifndef PARSER1_H
#define PARSER1_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// ��������
struct TextureData {
    unsigned int id; // OpenGL����ID
    std::string path; // �����ļ�·��
};

// ��������
struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
    int textureIndex; // 0��ʾ������ʵ��������Ҫ-1
};

// ��������
struct Vertex {
    float x, y, z;
};

// ��ͼ��������
struct TexCoord {
    float u, v;
};

// ��������
struct Normal {
    float x, y, z;
};

// ������������
struct Face {
    unsigned int v_indices[3]; // �������� (1-based)
    unsigned int t_indices[3]; // ��ͼ�������� (1-based)
    unsigned int n_indices[3]; // �������� (1-based)
};

// ��ģ������
struct SubModel {
    int materialIndex; // �������� (1-based)
    std::vector<Face> faces;
};

// ����ģ������
struct Model {
    std::vector<std::string> textureFileNames; // �����ļ����б�
    std::vector<TextureData> textures;         // ���غ����������
    std::vector<Material> materials;
    std::vector<Vertex> vertices;
    std::vector<TexCoord> texCoords;
    std::vector<Normal> normals;
    float scale[3]; // ����ϵ��
    std::vector<SubModel> subModels;
};

// ȫ��ģ�ͱ���
extern Model g_model;

// ��������
bool loadModel(const char* filename);
bool loadBMPTexture(const std::string& filepath, unsigned int& textureID);

#endif // PARSER1_H
