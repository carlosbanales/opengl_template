#include <GL/glew.h>      // OpenGL extension wrangler
#include <GLFW/glfw3.h>   // GLFW for window and input handlin
#include <stdio.h>

// prototypes are placed on top so they can call the functions at the bottom
// used for the main loops
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawLine();

// Vertex Shader source code
// version 3.30 of opengl shading language
// declare an input variable (aPos) then convert the 2d position (aPos) into 4d vector
// and sets z to 0.0 and w to 1.0 cause opengl works in homogeneous corrdinates
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos; // 2D position
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0); // Convert to vec4 for gl_Position
    }
)";

// Fragment Shader source code
// 
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor; // Output color
    void main() {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set OpenGL version to 3.3 and core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1000, 800, "OpenGL Window", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current with out this there would be
    // no active context for the calling thread and the commands would
    // have no effect, if there are multiple windows you can switch
    // context to issue opengl commands to the appropriate window
    glfwMakeContextCurrent(window);

    // Initialize GLEW to load OpenGL functions
    // Used to load modern opengl functions that are not directly
    // available in older systems
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    // Set the viewport and window resize callback
    glViewport(0, 0, 1000, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Delete the shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data for a line
    GLfloat vertices[] = {
        -0.5f, 0.0f,  // Start point (left)
         0.5f, 0.0f   // End point (right)
    };

    // Generate and bind vertex array and buffer objects
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the line
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2); // Draw the line
        glBindVertexArray(0);

        // Swap buffers
        // this is a basic double buffer
        // TO-DO: research use cases for triple and quadruple buffering
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // clean up
    // Terminate GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    // Close the window when pressing escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Adjust the viewport when the window size changes
    glViewport(0, 0, width, height);
}
