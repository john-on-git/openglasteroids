constexpr auto WINDOW_TITLE = "My First (Successful) OpenGL Project";
constexpr auto WINDOW_WIDTH = 1000;
constexpr auto WINDOW_HEIGHT = 1000;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLuint compileProgram(std::string fragShaderPath, std::string vertShaderPath);