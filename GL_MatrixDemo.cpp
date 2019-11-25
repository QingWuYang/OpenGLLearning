//
//  GL_TextureDemo.cpp
//  caogangqian
//
//  Created by caogangqiang on 2019/11/21.
//  Copyright © 2019 caogangqiang. All rights reserved.
//

#include "GL_TextureDemo.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "GL_Shaer.hpp"

// 回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 关闭窗口
void processInput(GLFWwindow *window);

// settingsc窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 透明度大小
float transparencyValue = 0.2f;


int main(){
    
    // glfw: initialize and configure
    // 初始化GLFW，版本号设为3.3
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 适配mac
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    // glfw window creation
    // 创建窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 将窗口的上下文设置为当前线程的上下文
    glfwMakeContextCurrent(window);
    // 绑定窗口大小变化时候的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // glad: load all OpenGL function pointers
    // 加载GLAD
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //    // 纹理坐标
    //    float texCoords[] = {
    //        0.0f, 0.0f, // 左下角
    //        1.0f, 0.0f, // 右下角
    //        0.5f, 1.0f // 上中
    //    };
    
    // 旋转图片
    stbi_set_flip_vertically_on_load(true);
    // 加载图片
    int width, height, nrChannels, width2, height2, nrChannels2;
    unsigned char *data = stbi_load("/Users/caogangqiang/C++Project/GLFWTest/texture.jpg", &width, &height, &nrChannels, 0);
    unsigned char *data2 = stbi_load("/Users/caogangqiang/C++Project/GLFWTest/test2.jpg", &width2, &height2, &nrChannels2, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    // 创建纹理对象
    unsigned int texture1;
    glGenTextures(1, &texture1);
    
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    // 利用加载的图片生成一个纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // 创建纹理对象  ====2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    // 利用加载的图片生成一个纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // 设置纹理在xy方向上的环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // 纹理填充参数若是选择了GL_CLAMP_TO_BORDER，那还需要指定一个边缘颜色
    //    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // 设置纹理过滤模式，邻近过滤相当于突出像素点，线性过滤相当于减少颗粒感
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 设置多级渐远纹理的过滤模式(缩小有效果，放大没效果，所以不设置放大)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    // 释放图像内存
    stbi_image_free(data);
    stbi_image_free(data2);
    
    //
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // 标准化设备中的顶点数据
    // ------------------------------------------------------------------
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    // 顶点缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // 使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标
    glBindVertexArray(VAO);
    
    // 把之前定义的顶点数据复制到缓冲的内存中
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 告诉OpenGl如何解析顶点数据
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("/Users/caogangqiang/C++Project/caogangqian/MatrixVs.vs", "/Users/caogangqiang/C++Project/caogangqian/MatrixFs.fs"); // you can name your shader files however you like
    ourShader.use();
    
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
    ourShader.setInt("texture2", 1); // 或者使用着色器类设置
    
    
    
    
    // 循环输出每一帧
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 创建变换矩阵
        glm::mat4 trans = glm::mat4(1.0f);
        
        trans = glm::rotate(trans, /*glm::radians(90.0f)*/(float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));// 设置旋转角度随着时间变换
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
        
        
        //        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); // Switched the order
        //         trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f)); // Switched the order
        
        
        // 传递变换矩阵
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 激活纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader.setFloat("transparency", transparencyValue); // 或者使用着色器类设置
        
        ourShader.use();
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // second transformation
        // ---------------------
        glm::mat4 transform = glm::mat4(1.0f); // reset it to identity matrix
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleAmount = sin(glfwGetTime());
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value

        // now with the uniform matrix being replaced with new transformations, draw it again.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // 刷新缓冲，并且推出所有事件
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // 释放窗口资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//等待键盘输入s关闭程序 ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        transparencyValue += 0.0001f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        transparencyValue -= 0.0001f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//窗口大小改变的时候的回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

