#include "Common/OpenGLHeaders.H"
#include "graphics/Drawable.H"
#include "graphics/Triangle.H"
#include "graphics/Rectangle.H"
#include "graphics/PhysicalObject.H"
#include "graphics/PhysicalTriangle.H"


#include <memory>
#include <vector>

#include <unistd.h>

constexpr auto WIDTH  = 800;
constexpr auto HEIGHT = 600;

int main(int argc, char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuck you Title", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    //glfwSetFrameBufferSizeCallback(window, )
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "Glew Initialization Failed" << std::endl;
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0, width, height);

    auto shaderProgram = std::make_shared<ShaderProgram>(VertexShader("graphics/shaders/vertex.vs"),
                                                         FragmentShader("graphics/shaders/fragment.fs"));

    PhysicalTriangle triangle{shaderProgram};
    triangle.scale(0.2);

    bool toggle = false;
    glViewport(0,0, width, height);
    while (1) {
        glfwPollEvents();
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        triangle.draw();
        Universe::update();

        glfwSwapBuffers(window);
    }
}
