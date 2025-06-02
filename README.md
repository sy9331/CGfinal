# 计算机图形学大作业
## 目标：
### 基础目标：读取并渲染模型。利用交互式方法，能操纵浏览模型细节（交互），使得能方便观察模型。并最终通过模型，设计一个简单的真实感三维显示程序。
### 拓展目标：
   能够用不同的方式（如：附录中的图1~6）去渲染模型，增加光照控制（光源的位置与类型）及其他交互能力（例如：坐标系的显示，提示信息显示，视图重置等等），甚至类似于3D的交互式编辑模型。
	1、在填充模式下，打开纹理和材质
	2、在画线模式下，打开纹理和材质
	3、在点模式下，打开纹理和贴图
	4、在填充模式下，打开材质，关闭纹理
	5、在填充模式下，关闭材质，打开纹理
	6、填充模式下，另一模型
## 项目结构：
    20250310
	├── README.md
	├── src（源代码目录）
	│   ├── main.cpp **运行主函数**
	│   ├── interaction.cpp **交互处理**
	│   ├── parser1.cpp **模型解析**
	|   ├── render.cpp **渲染处理**
	├── Header（头文件目录）
	│   ├── interaction.h **交互处理头文件**
	│   ├── parser1.h **模型解析头文件**
	│   ├── render.h **渲染处理头文件**
	├── assets（资源目录）
	│   ├── models **模型文件**
	│   │   ├── luweiqi.txt 模型1
	│   ├── textures **纹理文件**
	│   │   ├──  
	├── glut.h
	├── glut32.lib
	├── glut32.dll
 ## 模型数据：
	 纹理文件数量 
	  纹理文件1(字符串)
	  纹理文件2(字符串)
	  纹理文件3(字符串) 
	  .
	  .
	  .
	材质数量
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  纹理文件索引(int[1])//0表示无
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  纹理文件索引(int[1])//0表示无
	  ambient(float[4])
	  diffuse(float[4])
	  specular(float[4]])
	  emission(float[4])
	  shininess(float[1])
	  纹理文件索引(int[1])//0表示无
	  .
	  .
	顶点数量
	  v1(float[3])
	  v2(float[3])
	  v3(float[3])
	  .
	  .
	贴图坐标数量
	  t1(float[2])
	  t2(float[2])
	  t3(float[2])
	  .
	  .
	法线数量
	  n1(float[3])
	  n2(float[3])
	  n3(float[3])
	  .
	  .
	模型分组数量
	缩放系数(float[3])
	  submodel1 三角形数量
	  材质索引(int)//0表示无
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    .
	    .
	  submodel2 三角形数量
	  材质索引(int)//0表示无
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    vi1 ti1 ni1 vi2 ti2 ni2 vi3 ti3 ni3 (unsigned int[9])
	    .
	    .
	  .
	  .
	项目完善建议
### 1. main.cpp (运行主函数)
	main.cpp 将是程序的入口点，主要负责初始化GLUT，设置回调函数，以及启动主循环。
	

**GLUT初始化：**
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); (双缓冲、RGB颜色、深度测试)

**设置窗口大小和位置：**
	
	glutInitWindowSize(width, height); glutInitWindowPosition(x, y);

**创建窗口：**
	
	glutCreateWindow("Computer Graphics Project");

**回调函数注册：**
	
	显示回调： glutDisplayFunc(renderScene); (调用 render.cpp 中的渲染函数)
	
	窗口大小改变回调： glutReshapeFunc(changeSize); (处理窗口大小改变时的透视设置)
	
	键盘输入回调： glutKeyboardFunc(processNormalKeys); glutSpecialFunc(processSpecialKeys); (处理普通键和特殊键，例如方向键，调用 interaction.cpp 中的函数)
	
	鼠标事件回调： glutMouseFunc(mouseButton); (处理鼠标点击)
	
	鼠标移动回调： glutMotionFunc(mouseMove); (处理鼠标拖动，用于模型旋转/平移)
	
	空闲回调（可选）： glutIdleFunc(update); (如果需要连续动画或更新，可以在这里添加)

**OpenGL状态初始化：**
	
	启用深度测试：glEnable(GL_DEPTH_TEST);
	
	启用光照：glEnable(GL_LIGHTING); (如果需要光照效果)
	
	设置光源（初始光源）：glEnable(GL_LIGHT0);
	
	设置背面剔除（可选）：glEnable(GL_CULL_FACE);
	
	设置平滑着色：glShadeModel(GL_SMOOTH);
	
	设置背景颜色：glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

**模型加载：**
	
	在进入主循环前，调用 parser1.cpp 中的函数加载模型数据。

**进入主循环：**
	
	glutMainLoop();

### 2. parser1.cpp / parser1.h (模型解析)

**loadModel(const char* filename)函数:**
	
	使用文件流（ifstream）打开并读取模型文件。
	
	按顺序解析数据：
	 读取纹理文件数量，然后逐行读取纹理文件名。
	 读取材质数量，然后循环读取每个材质的ambient, diffuse, specular, emission, shininess, textureIndex。
	 读取顶点数量，然后循环读取每个顶点的x, y, z坐标。
	 读取贴图坐标数量，然后循环读取每个贴图坐标的u, v。
	 读取法线数量，然后循环读取每个法线的x, y, z坐标。
	 读取模型分组数量。
	 读取全局缩放系数。
	循环读取每个子模型的三角形数量和材质索引，然后循环读取每个三角形的9个索引（vi, ti, ni）。
	
	纹理加载： 
	 在读取纹理文件名后，需要加载纹理图片并生成OpenGL纹理ID。这可能需要额外的库，例如 SOIL2 或 FreeImage 来处理常见的图片格式（BMP, PNG, JPG 等）。如果没有，则需要自己实现BMP等格式的简单加载。
	
	错误处理：
	 在文件读取过程中加入健壮的错误处理，例如文件不存在、数据格式错误等。

### 3. render.cpp / render.h (渲染处理)
 这个模块是实现模型显示和渲染模式切换的核心。
	全局渲染状态变量：
	
  在 render.h 中定义一些全局变量来控制渲染模式，例如：
	C++
	enum RenderMode { FILL, LINE, POINT };
	extern RenderMode currentRenderMode;
	extern bool enableTexture;
	extern bool enableMaterial;
	
  渲染函数 renderScene()：
	 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  视口操作： 根据交互模块的相机位置和方向设置 gluLookAt()。
  
  坐标系显示（拓展目标）： 如果 displayCoordinateSystem 为 true，绘制XYZ轴。
  
  模型渲染循环： 遍历加载的 Model 结构体：
  
  应用全局缩放: glScalef(model.scale[0], model.scale[1], model.scale[2]);
  
  遍历每个 SubModel：
	
	获取当前子模型的材质 Material currentMaterial = model.materials[subModel.materialIndex - 1]; (注意索引是从1开始)
	
	材质设置： 如果 enableMaterial 为 true 且 subModel.materialIndex 不为0，使用 glMaterialfv() 设置材质属性。
	
	纹理绑定： 如果 enableTexture 为 true 且 currentMaterial.textureIndex 不为0，以及对应的纹理已加载，则 glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, model.textures[currentMaterial.textureIndex - 1].id);
	
	设置渲染模式： 根据 currentRenderMode 变量设置 glPolygonMode()：
	GL_FILL (填充模式)
	GL_LINE (线框模式)
	GL_POINT (点模式)
  
  遍历子模型中的每个 Face：
	
	glBegin(GL_TRIANGLES);
  
  对于每个顶点：
	
	glNormal3fv(&model.normals[face.n_indices[i] - 1].x);
	glTexCoord2fv(&model.texCoords[face.t_indices[i] - 1].u); (如果开启纹理)
	glVertex3fv(&model.vertices[face.v_indices[i] - 1].x);
	glEnd();
	glDisable(GL_TEXTURE_2D); (渲染完一个子模型后禁用纹理，避免影响下一个)
	glutSwapBuffers();
	changeSize(int w, int h) 函数：
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

渲染模式切换逻辑：
	
	在 render.h 中声明一些函数，如 toggleFillMode(), toggleLineMode(), togglePointMode(), toggleTexture(), toggleMaterial()。
	这些函数修改 currentRenderMode, enableTexture, enableMaterial 等全局变量，然后调用 glutPostRedisplay(); 触发重绘。

光照控制：
	
	实现函数来设置光源的位置 (glLightfv(GL_LIGHT0, GL_POSITION, ...);) 和类型（点光源、方向光等）。
	可以通过键盘或鼠标交互来修改光源属性。
	提示信息显示（拓展目标）：
	可以使用GLUT的位图字体 glRasterPos2f() 和 glutBitmapCharacter() 在屏幕上显示当前渲染模式、相机坐标等信息。

### 4. interaction.cpp / interaction.h (交互处理)
这个模块负责处理用户的输入，并更新相机位置、模型状态等。
	相机变量：
	在 interaction.h 中定义相机相关的全局变量：
	C++
	
	extern float cameraX, cameraY, cameraZ;
	extern float lookAtX, lookAtY, lookAtZ;
	extern float angleX, angleY; // 用于存储模型的旋转角度
	extern float zoomFactor; // 用于缩放
	extern int lastMouseX, lastMouseY; // 存储上次鼠标位置
	extern bool isRotating; // 标记是否在旋转
	extern bool isPanning; // 标记是否在平移

**键盘回调函数：**
	processNormalKeys(unsigned char key, int x, int y)：
	例如：
	'w', 'a', 's', 'd'：前后左右移动相机。
	'1', '2', '3'：切换渲染模式（填充、线框、点）。调用 render.cpp 中的切换函数。
	't'：切换纹理开关。
	'm'：切换材质开关。
	'c'：切换坐标系显示。
	'r'：重置视图（将相机和模型恢复到初始位置）。
	'q'：退出程序。
	'v': 切换模式。
	
	processSpecialKeys(int key, int x, int y)：
	例如：GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT 可以用于微调相机或模型。
	
**鼠标回调函数：**
	mouseButton(int button, int state, int x, int y)：
	mouseMove(int x, int y)：
	如果 isRotating 为 true：根据 (x - lastMouseX) 和 (y - lastMouseY) 更新 angleY 和 angleX。然后更新 lastMouseX, lastMouseY 并调用 glutPostRedisplay();
	如果 isPanning 为 true：根据鼠标移动更新相机或模型的平移量，然后调用 glutPostRedisplay();

**视图重置：**
	实现 resetView() 函数，将相机位置、旋转角度、缩放因子恢复到初始值。

### 5. Header (头文件目录)
	确保头文件中只包含声明，并且有必要的包含卫士（#ifndef, #define, #endif）。
	
	interaction.h：声明 interaction.cpp 中使用的全局变量和函数。
	parser1.h：定义模型相关的数据结构和 loadModel 函数的声明。
	render.h：声明 render.cpp 中使用的全局渲染状态变量和渲染函数。
### 拓展目标实现细节
	不同渲染方式（填充、线框、点）：
	通过键盘事件（如 '1', '2', '3'）来切换 currentRenderMode 全局变量，然后在 renderScene() 中使用 glPolygonMode(GL_FRONT_AND_BACK, currentRenderMode == FILL ? GL_FILL : (currentRenderMode == LINE ? GL_LINE : GL_POINT)); 来切换。
	光照控制：
	可以在 render.cpp 中定义一个 Light 结构体，包含光源位置、颜色等属性。
	通过键盘（例如 'L' 键切换光源类型，'WASD' 调整光源位置）来修改光源属性，然后调用 glLightfv() 更新光源。
	坐标系显示：
	在 renderScene() 中，在模型渲染之前（或者模型渲染之后，取决于你希望它被遮挡还是始终可见），绘制三条相互垂直的线段作为XYZ轴。
	使用 glDisable(GL_LIGHTING); 在绘制坐标系前禁用光照，避免影响其颜色。
	提示信息显示：
	使用 glRasterPos2f() 设置文本绘制位置，然后使用 glutBitmapCharacter() 或 glutStrokeCharacter() 绘制字符串。
	注意，在绘制文本之前，需要将投影矩阵和模型视图矩阵设置为2D正交投影，绘制完毕后再恢复3D透视投影。
	类似3D交互式编辑模型：
	这会复杂很多，可能需要实现拾取（Picking）功能，即根据鼠标点击的屏幕坐标，确定点击到了模型的哪个部分（顶点、边、面）。
	一旦拾取到，可以实现顶点拖动、面移动等功能。这需要更深入的几何计算和数据结构管理。对于大作业来说，这可能是一个高级挑战。
	模型数据注意事项
	索引从1开始： 你的模型数据中索引是 unsigned int[9]，并且注释是 //0表示无，这意味着索引通常是从1开始的。在代码中读取时，需要将索引减去1才能对应到C++ std::vector 的0-based索引。
	纹理路径： 确保纹理文件路径正确，并且你的程序有权限访问这些文件。
	数据量： 如果模型非常大，考虑使用 VBO (Vertex Buffer Object) 和 VAO (Vertex Array Object) 来提高渲染性能，而不是每次渲染都重复传递顶点数据。不过对于普通大作业可能不是必需的。
	