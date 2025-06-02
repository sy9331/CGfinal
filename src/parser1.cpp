#include "../Header/parser1.h"
#include <glut.h>
Model g_model; 

#pragma pack(push, 1) 

struct BMPFileHeader {
    unsigned short file_type;    
    unsigned int file_size;      
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixel_offset;   
};

struct BMPInfoHeader {
    unsigned int header_size;    
    int width;                   
    int height;                  
    unsigned short planes;       
    unsigned short bit_count;    
    unsigned int compression;    
    unsigned int size_image;     
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    unsigned int colors_used;
    unsigned int colors_important;
};

#pragma pack(pop)

// 辅助函数：加载纹理
bool loadBMPTexture(const std::string& filepath, unsigned int& textureID) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open BMP file " << filepath << std::endl;
        return false;
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

    // 检查BMP文件有效性
    if (fileHeader.file_type != 0x4D42 || infoHeader.bit_count != 24) { 
        std::cerr << "Error: " << filepath << " is not a valid 24-bit BMP file." << std::endl;
        file.close();
        return false;
    }

    // 计算图像数据大小
    int row_padded_size = ((infoHeader.width * infoHeader.bit_count + 31) / 32) * 4;
    unsigned int imageSize = row_padded_size * std::abs(infoHeader.height); 

    std::vector<unsigned char> data(imageSize);

    // 移动文件指针到像素数据起始位置
    file.seekg(fileHeader.pixel_offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    file.close();

	// 生成纹理ID并绑定纹理
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.width, std::abs(infoHeader.height),
        0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data.data()); 

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, infoHeader.width, std::abs(infoHeader.height),
        GL_BGR_EXT, GL_UNSIGNED_BYTE, data.data());

    std::cout << "Texture " << filepath << " loaded successfully! ID: " << textureID << std::endl;
    return true;
}
// 加载模型函数
bool loadModel(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open model file " << filename << std::endl;
        return false;
    }

    // 清空旧数据
    g_model = Model();

    // 1. 读取纹理文件数量和文件名
    int textureCount;
    file >> textureCount;
    g_model.textureFileNames.resize(textureCount);
    for (int i = 0; i < textureCount; ++i) {
        file >> g_model.textureFileNames[i];
    }

    // 加载纹理
    g_model.textures.resize(textureCount);
    for (int i = 0; i < textureCount; ++i) {
        g_model.textures[i].path = g_model.textureFileNames[i];
        // 假设纹理文件在 assets/textures 目录下
        std::string fullPath = "assets/textures/" + g_model.textureFileNames[i];
        if (!loadBMPTexture(fullPath, g_model.textures[i].id)) {
            std::cerr << "Warning: Failed to load texture: " << fullPath << std::endl;
            // 继续加载，但此纹理ID可能为0
        }
    }


    // 2. 读取材质数量和材质数据
    int materialCount;
    file >> materialCount;
    g_model.materials.resize(materialCount);
    for (int i = 0; i < materialCount; ++i) {
        for (int j = 0; j < 4; ++j) file >> g_model.materials[i].ambient[j];
        for (int j = 0; j < 4; ++j) file >> g_model.materials[i].diffuse[j];
        for (int j = 0; j < 4; ++j) file >> g_model.materials[i].specular[j];
        for (int j = 0; j < 4; ++j) file >> g_model.materials[i].emission[j];
        file >> g_model.materials[i].shininess;
        file >> g_model.materials[i].textureIndex;
    }

    // 3. 读取顶点数量和顶点数据
    int vertexCount;
    file >> vertexCount;
    g_model.vertices.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        file >> g_model.vertices[i].x >> g_model.vertices[i].y >> g_model.vertices[i].z;
    }

    // 读取贴图坐标数量和贴图坐标数据
    int texCoordCount;
    file >> texCoordCount;
    g_model.texCoords.resize(texCoordCount);
    for (int i = 0; i < texCoordCount; ++i) {
        file >> g_model.texCoords[i].u >> g_model.texCoords[i].v;
    }

    // 读取法线数量和法线数据
    int normalCount;
    file >> normalCount;
    g_model.normals.resize(normalCount);
    for (int i = 0; i < normalCount; ++i) {
        file >> g_model.normals[i].x >> g_model.normals[i].y >> g_model.normals[i].z;
    }

    // 读取模型分组数量
    int subModelCount;
    file >> subModelCount;
    g_model.subModels.resize(subModelCount);

    // 读取缩放系数
    file >> g_model.scale[0] >> g_model.scale[1] >> g_model.scale[2];

    // 读取每个子模型的数据
    for (int i = 0; i < subModelCount; ++i) {
        int triangleCount;
        file >> triangleCount;
        file >> g_model.subModels[i].materialIndex;
        g_model.subModels[i].faces.resize(triangleCount);
        for (int j = 0; j < triangleCount; ++j) {
            for (int k = 0; k < 3; ++k) { 
                file >> g_model.subModels[i].faces[j].v_indices[k]
                    >> g_model.subModels[i].faces[j].t_indices[k]
                    >> g_model.subModels[i].faces[j].n_indices[k];
            }
        }
    }

    file.close();
    std::cout << "Model " << filename << " loaded successfully!" << std::endl;
    return true;
}