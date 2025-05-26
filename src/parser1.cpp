#include "../Header/parser1.h"
#include <glut.h>
Model g_model; // ȫ��ģ��ʵ��

#pragma pack(push, 1) // ȷ���ṹ���Ա���ܴ����������ֽ�

struct BMPFileHeader {
    unsigned short file_type;    // �ļ����ͣ������� "BM" (0x4D42)
    unsigned int file_size;      // �ļ���С����λ�ֽ�
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int pixel_offset;   // ����������ʼλ��������ļ�ͷ��ƫ����
};

struct BMPInfoHeader {
    unsigned int header_size;    // ��Ϣͷ��С������BITMAPINFOHEADER��40�ֽ�
    int width;                   // ͼ����
    int height;                  // ͼ��߶�
    unsigned short planes;       // ƽ������������1
    unsigned short bit_count;    // ÿ����λ�� (1, 4, 8, 16, 24, 32)
    unsigned int compression;    // ѹ������
    unsigned int size_image;     // ͼ���С���������ݴ�С�������δѹ����Ϊ0
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    unsigned int colors_used;
    unsigned int colors_important;
};

#pragma pack(pop)

// ������������������
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

    // ���BMP�ļ���Ч��
    if (fileHeader.file_type != 0x4D42 || infoHeader.bit_count != 24) { // ���"BM"��24λ���ɫ
        std::cerr << "Error: " << filepath << " is not a valid 24-bit BMP file." << std::endl;
        file.close();
        return false;
    }

    // ����ͼ�����ݴ�С
    // BMP�����ֽ���������4�ı�����������ǣ���Ҫ���
    int row_padded_size = ((infoHeader.width * infoHeader.bit_count + 31) / 32) * 4;
    unsigned int imageSize = row_padded_size * std::abs(infoHeader.height); // �߶ȿ���Ϊ���� (����ͼ��)

    std::vector<unsigned char> data(imageSize);

    // �ƶ��ļ�ָ�뵽����������ʼλ��
    file.seekg(fileHeader.pixel_offset, std::ios::beg);
    file.read(reinterpret_cast<char*>(data.data()), imageSize);
    file.close();

    // BMPͨ����BGR˳��OpenGLͨ����ҪRGB��
    // ���������ǰ��ж�ȡ��ÿ�п�����Ҫ��䣬��������ֱ���ϴ���
    // ���������ʾ��ɫ���ԣ�������Ҫ���������B-Rͨ��������
    // ���磺 for (int i = 0; i < imageSize; i += 3) { std::swap(data[i], data[i+2]); }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // �����������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // �ϴ���������
    // ע�⣺BMPͨ����BGR˳�򣬵�OpenGLͨ����RGB��GLUTĬ�Ͽ��ܴ���BGR��
    // �����ɫ���ԣ����Խ�GL_BGR��ΪGL_RGB
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.width, std::abs(infoHeader.height),
        0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data.data()); // GL_BGR_EXT �� GL_RGB

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, infoHeader.width, std::abs(infoHeader.height),
        GL_BGR_EXT, GL_UNSIGNED_BYTE, data.data());

    std::cout << "Texture " << filepath << " loaded successfully! ID: " << textureID << std::endl;
    return true;
}

bool loadModel(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open model file " << filename << std::endl;
        return false;
    }

    // ��վ�����
    g_model = Model();

    // 1. ��ȡ�����ļ��������ļ���
    int textureCount;
    file >> textureCount;
    g_model.textureFileNames.resize(textureCount);
    for (int i = 0; i < textureCount; ++i) {
        file >> g_model.textureFileNames[i];
    }

    // ��������
    g_model.textures.resize(textureCount);
    for (int i = 0; i < textureCount; ++i) {
        g_model.textures[i].path = g_model.textureFileNames[i];
        // ���������ļ��� assets/textures Ŀ¼��
        std::string fullPath = "assets/textures/" + g_model.textureFileNames[i];
        if (!loadBMPTexture(fullPath, g_model.textures[i].id)) {
            std::cerr << "Warning: Failed to load texture: " << fullPath << std::endl;
            // �������أ���������ID����Ϊ0
        }
    }


    // 2. ��ȡ���������Ͳ�������
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

    // 3. ��ȡ���������Ͷ�������
    int vertexCount;
    file >> vertexCount;
    g_model.vertices.resize(vertexCount);
    for (int i = 0; i < vertexCount; ++i) {
        file >> g_model.vertices[i].x >> g_model.vertices[i].y >> g_model.vertices[i].z;
    }

    // ��ȡ��ͼ������������ͼ��������
    int texCoordCount;
    file >> texCoordCount;
    g_model.texCoords.resize(texCoordCount);
    for (int i = 0; i < texCoordCount; ++i) {
        file >> g_model.texCoords[i].u >> g_model.texCoords[i].v;
    }

    // ��ȡ���������ͷ�������
    int normalCount;
    file >> normalCount;
    g_model.normals.resize(normalCount);
    for (int i = 0; i < normalCount; ++i) {
        file >> g_model.normals[i].x >> g_model.normals[i].y >> g_model.normals[i].z;
    }

    // ��ȡģ�ͷ�������
    int subModelCount;
    file >> subModelCount;
    g_model.subModels.resize(subModelCount);

    // ��ȡ����ϵ��
    file >> g_model.scale[0] >> g_model.scale[1] >> g_model.scale[2];

    // ��ȡÿ����ģ�͵�����
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