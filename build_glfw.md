## Сборка жарников glfw

1. Очистить ant
2. ```ant init```
3. ```ant compile-templates```
4. Сборка нативных либ 
 - Для сборки arm64: <br/>
```LWJGL_BUILD_ARCH=arm64 ant compile-native```
 - Для сборки amd: <br/>
```LWJGL_BUILD_ARCH=x64 ant compile-native```
 5. Заменяем  libglfw.so из папки glfwso под нужную архитектуру в папке ./bin/libs/native/linux/arm64/org/lwjgl/glfw
 6. ```LWJGL_BUILD_OFFLINE=true ant release```
 7. В папке ./bin/RELEASE/lwjgl-glfw появились жарники lwjgl-glfw.jar и lwjgl-glfw-natives-*platforma*.jar - с ними можно работать
