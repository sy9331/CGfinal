# �����ͼ��ѧ����ҵ
## Ŀ�꣺
### ����Ŀ�꣺��ȡ����Ⱦģ�͡����ý���ʽ�������ܲ������ģ��ϸ�ڣ���������ʹ���ܷ���۲�ģ�͡�������ͨ��ģ�ͣ����һ���򵥵���ʵ����ά��ʾ����
### ��չĿ�꣺
   �ܹ��ò�ͬ�ķ�ʽ���磺��¼�е�ͼ1~6��ȥ��Ⱦģ�ͣ����ӹ��տ��ƣ���Դ��λ�������ͣ��������������������磺����ϵ����ʾ����ʾ��Ϣ��ʾ����ͼ���õȵȣ�������������3D�Ľ���ʽ�༭ģ�͡�
	1�������ģʽ�£�������Ͳ���
	2���ڻ���ģʽ�£�������Ͳ���
	3���ڵ�ģʽ�£����������ͼ
	4�������ģʽ�£��򿪲��ʣ��ر�����
	5�������ģʽ�£��رղ��ʣ�������
	6�����ģʽ�£���һģ��
## ��Ŀ�ṹ��
    20250310
	������ README.md
	������ src��Դ����Ŀ¼��
	��   ������ main.cpp **����������**
	��   ������ interaction.cpp **��������**
	��   ������ parser1.cpp **ģ�ͽ���**
	|   ������ render.cpp **��Ⱦ����**
	������ Header��ͷ�ļ�Ŀ¼��
	��   ������ interaction.h **��������ͷ�ļ�**
	��   ������ parser1.h **ģ�ͽ���ͷ�ļ�**
	��   ������ render.h **��Ⱦ����ͷ�ļ�**
	������ assets����ԴĿ¼��
	��   ������ models **ģ���ļ�**
	��   ��   ������ luweiqi.txt ģ��1
	��   ������ textures **�����ļ�**
	��   ��   ������  
	������ glut.h
	������ glut32.lib
	������ glut32.dll
 ## ģ�����ݣ�
	 �����ļ����� 
	  �����ļ�1(�ַ���)
	  �����ļ�2(�ַ���)
	  �����ļ�3(�ַ���) 
	  .
	  .
	  .
	��������
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  �����ļ�����(int[1])//0��ʾ��
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  �����ļ�����(int[1])//0��ʾ��
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  �����ļ�����(int[1])//0��ʾ��
	  .
	  .
	��������
	  v1(float[3])
	  v2(float[3])
	  v3(float[3])
	  .
	  .
	��ͼ��������
	  t1(float[2])
	  t2(float[2])
	  t3(float[2])
	  .
	  .
	��������
	  n1(float[3])
	  n2(float[3])
	  n3(float[3])
	  .
	  .
	ģ�ͷ�������
	����ϵ��(float[3])
	  submodel1 ����������
	  ��������(int)//0��ʾ��
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    .
	    .
	  submodel2 ����������
	  ��������(int)//0��ʾ��
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    .
	    .
	  .
	  .
	��Ŀ���ƽ���
### 1. main.cpp (����������)
	main.cpp ���ǳ������ڵ㣬��Ҫ�����ʼ��GLUT�����ûص��������Լ�������ѭ����
	

**GLUT��ʼ����**
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); (˫���塢RGB��ɫ����Ȳ���)

**���ô��ڴ�С��λ�ã�**
	
	glutInitWindowSize(width, height); glutInitWindowPosition(x, y);

**�������ڣ�**
	
	glutCreateWindow("Computer Graphics Project");

**�ص�����ע�᣺**
	
	��ʾ�ص��� glutDisplayFunc(renderScene); (���� render.cpp �е���Ⱦ����)
	
	���ڴ�С�ı�ص��� glutReshapeFunc(changeSize); (�����ڴ�С�ı�ʱ��͸������)
	
	��������ص��� glutKeyboardFunc(processNormalKeys); glutSpecialFunc(processSpecialKeys); (������ͨ��������������緽��������� interaction.cpp �еĺ���)
	
	����¼��ص��� glutMouseFunc(mouseButton); (���������)
	
	����ƶ��ص��� glutMotionFunc(mouseMove); (��������϶�������ģ����ת/ƽ��)
	
	���лص�����ѡ���� glutIdleFunc(update); (�����Ҫ������������£��������������)

**OpenGL״̬��ʼ����**
	
	������Ȳ��ԣ�glEnable(GL_DEPTH_TEST);
	
	���ù��գ�glEnable(GL_LIGHTING); (�����Ҫ����Ч��)
	
	���ù�Դ����ʼ��Դ����glEnable(GL_LIGHT0);
	
	���ñ����޳�����ѡ����glEnable(GL_CULL_FACE);
	
	����ƽ����ɫ��glShadeModel(GL_SMOOTH);
	
	���ñ�����ɫ��glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

**ģ�ͼ��أ�**
	
	�ڽ�����ѭ��ǰ������ parser1.cpp �еĺ�������ģ�����ݡ�

**������ѭ����**
	
	glutMainLoop();

### 2. parser1.cpp / parser1.h (ģ�ͽ���)

**loadModel(const char* filename)����:**
	
	ʹ���ļ�����ifstream���򿪲���ȡģ���ļ���
	
	��˳��������ݣ�
	 ��ȡ�����ļ�������Ȼ�����ж�ȡ�����ļ�����
	 ��ȡ����������Ȼ��ѭ����ȡÿ�����ʵ�ambient, diffuse, specular, emission, shininess, textureIndex��
	 ��ȡ����������Ȼ��ѭ����ȡÿ�������x, y, z���ꡣ
	 ��ȡ��ͼ����������Ȼ��ѭ����ȡÿ����ͼ�����u, v��
	 ��ȡ����������Ȼ��ѭ����ȡÿ�����ߵ�x, y, z���ꡣ
	 ��ȡģ�ͷ���������
	 ��ȡȫ������ϵ����
	ѭ����ȡÿ����ģ�͵������������Ͳ���������Ȼ��ѭ����ȡÿ�������ε�9��������vi, ti, ni����
	
	������أ� 
	 �ڶ�ȡ�����ļ�������Ҫ��������ͼƬ������OpenGL����ID���������Ҫ����Ŀ⣬���� SOIL2 �� FreeImage ����������ͼƬ��ʽ��BMP, PNG, JPG �ȣ������û�У�����Ҫ�Լ�ʵ��BMP�ȸ�ʽ�ļ򵥼��ء�
	
	������
	 ���ļ���ȡ�����м��뽡׳�Ĵ����������ļ������ڡ����ݸ�ʽ����ȡ�

### 3. render.cpp / render.h (��Ⱦ����)
 ���ģ����ʵ��ģ����ʾ����Ⱦģʽ�л��ĺ��ġ�
	ȫ����Ⱦ״̬������
	
  �� render.h �ж���һЩȫ�ֱ�����������Ⱦģʽ�����磺
	C++
	enum RenderMode { FILL, LINE, POINT };
	extern RenderMode currentRenderMode;
	extern bool enableTexture;
	extern bool enableMaterial;
	
  ��Ⱦ���� renderScene()��
	 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  �ӿڲ����� ���ݽ���ģ������λ�úͷ������� gluLookAt()��
  
  ����ϵ��ʾ����չĿ�꣩�� ��� displayCoordinateSystem Ϊ true������XYZ�ᡣ
  
  ģ����Ⱦѭ���� �������ص� Model �ṹ�壺
  
  Ӧ��ȫ������: glScalef(model.scale[0], model.scale[1], model.scale[2]);
  
  ����ÿ�� SubModel��
	
	��ȡ��ǰ��ģ�͵Ĳ��� Material currentMaterial = model.materials[subModel.materialIndex - 1]; (ע�������Ǵ�1��ʼ)
	
	�������ã� ��� enableMaterial Ϊ true �� subModel.materialIndex ��Ϊ0��ʹ�� glMaterialfv() ���ò������ԡ�
	
	����󶨣� ��� enableTexture Ϊ true �� currentMaterial.textureIndex ��Ϊ0���Լ���Ӧ�������Ѽ��أ��� glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, model.textures[currentMaterial.textureIndex - 1].id);
	
	������Ⱦģʽ�� ���� currentRenderMode �������� glPolygonMode()��
	GL_FILL (���ģʽ)
	GL_LINE (�߿�ģʽ)
	GL_POINT (��ģʽ)
  
  ������ģ���е�ÿ�� Face��
	
	glBegin(GL_TRIANGLES);
  
  ����ÿ�����㣺
	
	glNormal3fv(&model.normals[face.n_indices[i] - 1].x);
	glTexCoord2fv(&model.texCoords[face.t_indices[i] - 1].u); (�����������)
	glVertex3fv(&model.vertices[face.v_indices[i] - 1].x);
	glEnd();
	glDisable(GL_TEXTURE_2D); (��Ⱦ��һ����ģ�ͺ������������Ӱ����һ��)
	glutSwapBuffers();
	changeSize(int w, int h) ������
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

��Ⱦģʽ�л��߼���
	
	�� render.h ������һЩ�������� toggleFillMode(), toggleLineMode(), togglePointMode(), toggleTexture(), toggleMaterial()��
	��Щ�����޸� currentRenderMode, enableTexture, enableMaterial ��ȫ�ֱ�����Ȼ����� glutPostRedisplay(); �����ػ档

���տ��ƣ�
	
	ʵ�ֺ��������ù�Դ��λ�� (glLightfv(GL_LIGHT0, GL_POSITION, ...);) �����ͣ����Դ�������ȣ���
	����ͨ�����̻���꽻�����޸Ĺ�Դ���ԡ�
	��ʾ��Ϣ��ʾ����չĿ�꣩��
	����ʹ��GLUT��λͼ���� glRasterPos2f() �� glutBitmapCharacter() ����Ļ����ʾ��ǰ��Ⱦģʽ������������Ϣ��

### 4. interaction.cpp / interaction.h (��������)
���ģ�鸺�����û������룬���������λ�á�ģ��״̬�ȡ�
	���������
	�� interaction.h �ж��������ص�ȫ�ֱ�����
	C++
	
	extern float cameraX, cameraY, cameraZ;
	extern float lookAtX, lookAtY, lookAtZ;
	extern float angleX, angleY; // ���ڴ洢ģ�͵���ת�Ƕ�
	extern float zoomFactor; // ��������
	extern int lastMouseX, lastMouseY; // �洢�ϴ����λ��
	extern bool isRotating; // ����Ƿ�����ת
	extern bool isPanning; // ����Ƿ���ƽ��

**���̻ص�������**
	processNormalKeys(unsigned char key, int x, int y)��
	���磺
	'w', 'a', 's', 'd'��ǰ�������ƶ������
	'1', '2', '3'���л���Ⱦģʽ����䡢�߿򡢵㣩������ render.cpp �е��л�������
	't'���л������ء�
	'm'���л����ʿ��ء�
	'c'���л�����ϵ��ʾ��
	'r'��������ͼ���������ģ�ͻָ�����ʼλ�ã���
	'q'���˳�����
	'v': �л�ģʽ��
	
	processSpecialKeys(int key, int x, int y)��
	���磺GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT ��������΢�������ģ�͡�
	
**���ص�������**
	mouseButton(int button, int state, int x, int y)��
	mouseMove(int x, int y)��
	��� isRotating Ϊ true������ (x - lastMouseX) �� (y - lastMouseY) ���� angleY �� angleX��Ȼ����� lastMouseX, lastMouseY ������ glutPostRedisplay();
	��� isPanning Ϊ true����������ƶ����������ģ�͵�ƽ������Ȼ����� glutPostRedisplay();

**��ͼ���ã�**
	ʵ�� resetView() �����������λ�á���ת�Ƕȡ��������ӻָ�����ʼֵ��

### 5. Header (ͷ�ļ�Ŀ¼)
	ȷ��ͷ�ļ���ֻ���������������б�Ҫ�İ�����ʿ��#ifndef, #define, #endif����
	
	interaction.h������ interaction.cpp ��ʹ�õ�ȫ�ֱ����ͺ�����
	parser1.h������ģ����ص����ݽṹ�� loadModel ������������
	render.h������ render.cpp ��ʹ�õ�ȫ����Ⱦ״̬��������Ⱦ������
### ��չĿ��ʵ��ϸ��
	��ͬ��Ⱦ��ʽ����䡢�߿򡢵㣩��
	ͨ�������¼����� '1', '2', '3'�����л� currentRenderMode ȫ�ֱ�����Ȼ���� renderScene() ��ʹ�� glPolygonMode(GL_FRONT_AND_BACK, currentRenderMode == FILL ? GL_FILL : (currentRenderMode == LINE ? GL_LINE : GL_POINT)); ���л���
	���տ��ƣ�
	������ render.cpp �ж���һ�� Light �ṹ�壬������Դλ�á���ɫ�����ԡ�
	ͨ�����̣����� 'L' ���л���Դ���ͣ�'WASD' ������Դλ�ã����޸Ĺ�Դ���ԣ�Ȼ����� glLightfv() ���¹�Դ��
	����ϵ��ʾ��
	�� renderScene() �У���ģ����Ⱦ֮ǰ������ģ����Ⱦ֮��ȡ������ϣ�������ڵ�����ʼ�տɼ��������������໥��ֱ���߶���ΪXYZ�ᡣ
	ʹ�� glDisable(GL_LIGHTING); �ڻ�������ϵǰ���ù��գ�����Ӱ������ɫ��
	��ʾ��Ϣ��ʾ��
	ʹ�� glRasterPos2f() �����ı�����λ�ã�Ȼ��ʹ�� glutBitmapCharacter() �� glutStrokeCharacter() �����ַ�����
	ע�⣬�ڻ����ı�֮ǰ����Ҫ��ͶӰ�����ģ����ͼ��������Ϊ2D����ͶӰ��������Ϻ��ٻָ�3D͸��ͶӰ��
	����3D����ʽ�༭ģ�ͣ�
	��Ḵ�Ӻܶ࣬������Ҫʵ��ʰȡ��Picking�����ܣ����������������Ļ���꣬ȷ���������ģ�͵��ĸ����֣����㡢�ߡ��棩��
	һ��ʰȡ��������ʵ�ֶ����϶������ƶ��ȹ��ܡ�����Ҫ������ļ��μ�������ݽṹ�������ڴ���ҵ��˵���������һ���߼���ս��
	ģ������ע������
	������1��ʼ�� ���ģ�������������� unsigned int[9]������ע���� //0��ʾ�ޣ�����ζ������ͨ���Ǵ�1��ʼ�ġ��ڴ����ж�ȡʱ����Ҫ��������ȥ1���ܶ�Ӧ��C++ std::vector ��0-based������
	����·���� ȷ�������ļ�·����ȷ��������ĳ�����Ȩ�޷�����Щ�ļ���
	�������� ���ģ�ͷǳ��󣬿���ʹ�� VBO (Vertex Buffer Object) �� VAO (Vertex Array Object) �������Ⱦ���ܣ�������ÿ����Ⱦ���ظ����ݶ������ݡ�����������ͨ����ҵ���ܲ��Ǳ���ġ�
	