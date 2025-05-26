#ifndef PARSER1_H
#define PARSER1_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

// 纹理数据
struct TextureData {
    unsigned int id; // OpenGL纹理ID
    std::string path; // 纹理文件路径
};

// 材质数据
struct Material {
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
    int textureIndex; // 0表示无纹理，实际索引需要-1
};

// 顶点数据
struct Vertex {
    float x, y, z;
};

// 贴图坐标数据
struct TexCoord {
    float u, v;
};

// 法线数据
struct Normal {
    float x, y, z;
};

// 三角形面数据
struct Face {
    unsigned int v_indices[3]; // 顶点索引 (1-based)
    unsigned int t_indices[3]; // 贴图坐标索引 (1-based)
    unsigned int n_indices[3]; // 法线索引 (1-based)
};

// 子模型数据
struct SubModel {
    int materialIndex; // 材质索引 (1-based)
    std::vector<Face> faces;
};

// 完整模型数据
struct Model {
    std::vector<std::string> textureFileNames; // 纹理文件名列表
    std::vector<TextureData> textures;         // 加载后的纹理数据
    std::vector<Material> materials;
    std::vector<Vertex> vertices;
    std::vector<TexCoord> texCoords;
    std::vector<Normal> normals;
    float scale[3]; // 缩放系数
    std::vector<SubModel> subModels;
};

// 全局模型变量
extern Model g_model;

// 函数声明
bool loadModel(const char* filename);
bool loadBMPTexture(const std::string& filepath, unsigned int& textureID);

#endif // PARSER1_H
