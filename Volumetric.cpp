// Include C++ headers
#include <iostream>
#include <string>
#include <vector>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader loading utilities and other
#include <common/shader.h>
#include <common/util.h>
#include <common/camera.h>
#include <common/model.h>
#include <common/texture.h>

//
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

using namespace std;
using namespace glm;


// Function prototypes
void initialize();
void mainLoop();
void free();
 
#define W_WIDTH 800
#define W_HEIGHT 600
#define TITLE "apallaktiki"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderScene(const Shader& shader);
void renderScene2(const Shader& shader);
void renderCube();
void renderQuad();
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool shadows = true;
bool shadowsKeyPressed = false;
GLFWwindow* window;
// camera
vec3 camerapos= vec3(1.0f, 2.5f, -3.3f);
float camyaw=75,campitch=-25;
Camera camera(camerapos, glm::vec3(0.0f, 1.0f, 0.0f), camyaw, campitch);
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float c_x = 0.0;
float c_z = 0.0;

vec3 start_lightPos = vec3(1.5, 1.5, 7.0);
vec3 volumetric_control = vec3(-1.0f, 0.0f, -3.0f);;
// meshes
unsigned int planeVAO;
float exposure = 1.0f;
GLuint  W_objVAO, D_objVAO, C_objVAO;
GLuint W_objVerticiesVBO, W_objUVVBO, W_objNormalsVBO, D_objVerticiesVBO, D_objUVVBO, D_objNormalsVBO, C_objVerticiesVBO, C_objUVVBO, C_objNormalsVBO;
std::vector<vec3> W_objVertices, W_objNormals, D_objVertices, D_objNormals, C_objVertices, C_objNormals;
std::vector<vec2> W_objUVs, D_objUVs, C_objUVs;


void free()
{

    glfwTerminate();
}



void mainLoop()
{

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // build and compile shaders
    // -------------------------
    Shader shader("StandardShading.vertexshader",
        "StandardShading.fragmentshader");

    Shader simpleDepthShader("Shadow_mapping.vertexshader",
        "Shadow_mapping.fragmentshader", "Shadow_mapping.geometryshader");

    Shader simpleDepthShader2("Shadow_mapping2.vertexshader",
        "Shadow_mapping2.fragmentshader", "Shadow_mapping2.geometryshader");

    Shader hdrShader("hdr.vertexshader", "hdr.fragmentshader");

    Shader shaderBlur("blur.vertexshader", "blur.fragmentshader");

    Shader shaderbw("blackwhite.vertexshader", "blackwhite.fragmentshader");

    //load obj
    loadOBJWithTiny("tzamia.obj", W_objVertices, W_objUVs, W_objNormals);
    loadOBJWithTiny("spiti.obj", D_objVertices, D_objUVs, D_objNormals);
    loadOBJWithTiny("wood.obj", C_objVertices, C_objUVs, C_objNormals);
    // ------------------------------------------------------------------

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    //----------------------window------------------------------

    glGenVertexArrays(1, &W_objVAO);
    glBindVertexArray(W_objVAO);
    // vertex VBO
    glGenBuffers(1, &W_objVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, W_objVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, W_objVertices.size() * sizeof(glm::vec3),
        &W_objVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // normals VBO
    glGenBuffers(1, &W_objNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, W_objNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, W_objNormals.size() * sizeof(glm::vec3), &W_objNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // uvs VBO
    glGenBuffers(1, &W_objUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, W_objUVVBO);
    glBufferData(GL_ARRAY_BUFFER, W_objUVs.size() * sizeof(glm::vec2), &W_objUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    //----------------------domatio------------------------------

    glGenVertexArrays(1, &D_objVAO);
    glBindVertexArray(D_objVAO);

    // vertex VBO
    glGenBuffers(1, &D_objVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, D_objVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, D_objVertices.size() * sizeof(glm::vec3),
        &D_objVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // normals VBO
    glGenBuffers(1, &D_objNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, D_objNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, D_objNormals.size() * sizeof(glm::vec3), &D_objNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // uvs VBO
    glGenBuffers(1, &D_objUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, D_objUVVBO);
    glBufferData(GL_ARRAY_BUFFER, D_objUVs.size() * sizeof(glm::vec2), &D_objUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    //----------------------wood------------------------------

    glGenVertexArrays(1, &C_objVAO);
    glBindVertexArray(C_objVAO);

    // vertex VBO
    glGenBuffers(1, &C_objVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, C_objVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, C_objVertices.size() * sizeof(glm::vec3),
        &C_objVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // normals VBO
    glGenBuffers(1, &C_objNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, C_objNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, C_objNormals.size() * sizeof(glm::vec3), &C_objNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // uvs VBO
    glGenBuffers(1, &C_objUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, C_objUVVBO);
    glBufferData(GL_ARRAY_BUFFER, C_objUVs.size() * sizeof(glm::vec2), &C_objUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

   // ------------------------------------------------------------------
   //ftiaxnw to dapedo
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);




   // dimiourgw ton framebuffer gia tis skies me tin xrisi cubeMap
   // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // ------------------------------------------
    unsigned int depthMapFBO2;
    glGenFramebuffers(1, &depthMapFBO2);
    // create depth cubemap texture
    unsigned int depthCubemap2;
    glGenTextures(1, &depthCubemap2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap2);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO2);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap2, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // dimiourgw ton framebuffer gia to hdr
    // ------------------------------------
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // Framebuffer gia Volumetric
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }

    //Blak and White frambuffer
    //------------------------------------------------------------
    unsigned int bwFBO;
    unsigned int bwColorbuffer;
    glGenFramebuffers(1, &bwFBO);
    glGenTextures(1, &bwColorbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, bwFBO);
    glBindTexture(GL_TEXTURE_2D, bwColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bwColorbuffer, 0);
    // also check if framebuffers are complete (no need for depth buffer)
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
       std::cout << "Framebuffer not complete!" << std::endl;
    






    // pernaw ta textures stous shaders
    // --------------------
    shader.use();
    shader.setInt("depthMap", 1);
    // --------------------
    shaderBlur.use();
    shaderBlur.setInt("arxikocolor", 1);
    shaderBlur.setInt("bw", 2);
    shaderBlur.setInt("depthMap", 3);
    // --------------------
    hdrShader.use();
    hdrShader.setInt("hdrBuffer", 0);
    hdrShader.setInt("bloomBlur", 1);
    // --------------------
    shaderbw.use();
    shaderbw.setInt("scene", 0);
    shaderbw.setInt("depthMap", 1);
    shaderbw.setInt("depthMap2", 2);

    
    do
    {

        glm::vec3 lightPos(start_lightPos.x, start_lightPos.y , start_lightPos.z );

        //Ta xrisimopoiw sthn camera    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        // --------------------



        // render
        // --------------------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // create depth cubemap transformation matrices
        // -----------------------------------------------
        float near_plane = 1.0f;
        float far_plane = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // -----------------------------------------------

        std::vector<glm::mat4> shadowTransforms2;
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms2.push_back(shadowProj * glm::lookAt(camera.Position, camera.Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        // 1. kanw render tin skini moy ston depth buffer
        // --------------------------------
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        simpleDepthShader.use();
        for (unsigned int i = 0; i < 6; ++i)
            simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        simpleDepthShader.setFloat("far_plane", far_plane);
        simpleDepthShader.setVec3("lightPos", lightPos);
        renderScene2(simpleDepthShader);
        

        //----------------------------------


        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO2);
        glClear(GL_DEPTH_BUFFER_BIT);
        simpleDepthShader2.use();
        for (unsigned int i = 0; i < 6; ++i)
            simpleDepthShader2.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms2[i]);
        simpleDepthShader2.setFloat("far_plane", far_plane);
        simpleDepthShader2.setVec3("campos", camera.Position);
        renderScene(simpleDepthShader2);



        // 2. Kanw render tin skini mou kanonika kanotnas xrisi tou depth buffer
        // -------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set lighting uniforms
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("far_plane", far_plane);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        renderScene(shader);
        


        // 3. black and white render
        // -------------------------

        glBindFramebuffer(GL_FRAMEBUFFER, bwFBO); 
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderbw.use();
        shaderbw.setMat4("projection", projection);
        shaderbw.setMat4("view", view);
        shaderbw.setVec3("lightPos", lightPos);
        shaderbw.setVec3("viewPos", camera.Position);
        shaderbw.setInt("shadows", shadows); // enable/disable shadows by pressing 'SPACE'
        shaderbw.setFloat("far_plane", far_plane);
        shaderbw.setVec3("volumetric_control", volumetric_control);
  
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap2);

        renderScene(shaderbw);
        



        //Volumetric
        // --------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        float weight = 1;
        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        shaderBlur.use();

            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[1]);
            shaderBlur.setFloat("newweight", weight);
            shaderBlur.setVec3("lightPos", lightPos);
            shaderBlur.setMat4("projection", projection);
            shaderBlur.setMat4("view", view);
            shaderBlur.setVec3("camfront", camera.Front);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, bwColorbuffer);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
            renderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 5. Kanw render ton color buffer se ena 2D quad kai kanw tonemap ta HDR colors 
        // --------------------------------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[1]);
        hdrShader.setFloat("exposure", exposure);
        renderQuad();


        glfwSwapBuffers(window);
        glfwPollEvents();


    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
}

// renders the 3D scene
void renderScene(const Shader& shader)
{
   
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
    shader.setMat4("model", model);
    shader.setInt("usetexture", 0);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);



    //spiti
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setInt("usetexture", 1);
    glBindVertexArray(D_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, D_objVertices.size());

    //wood
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setInt("usetexture", 2);
    glBindVertexArray(C_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, C_objVertices.size());



    //cube (metakinisi tou cube meta arrows)
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f + c_x, 0.75f, -2.0f + c_z));
    model = glm::scale(model, glm::vec3(0.3f));
    shader.setInt("usetexture", 4);
    shader.setMat4("model", model);
    renderCube();


    //window
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setInt("usetexture", 3);
    glBindVertexArray(W_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, W_objVertices.size());


}


// renders the 3D scene --> xwris ta tzamia(gia na mporei na pernaei to fws) dioti otan evala cubemap gia ta shadows den pernouse to fws mesa apo ta parathyra
void renderScene2(const Shader& shader)
{

    // floor
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
    shader.setMat4("model", model);
    shader.setInt("usetexture", 0);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);



    //spiti
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setInt("usetexture", 1);
    glBindVertexArray(D_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, D_objVertices.size());


    //wood
    model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    shader.setInt("usetexture", 2);
    glBindVertexArray(C_objVAO);
    glDrawArrays(GL_TRIANGLES, 0, C_objVertices.size());

    //cube (metakinisi tou cube meta arrows)
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f + c_x, 0.75f, -2.0f + c_z));
    model = glm::scale(model, glm::vec3(0.3f));
    shader.setInt("usetexture", 4);
    shader.setMat4("model", model);
    renderCube();


}


// renderCube() renders a 1x1 3D cube in NDC.
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// renderQuad() renders a 1x1 XY quad in NDC
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


void initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
            " If you have an Intel GPU, they are not 3.3 compatible." +
            "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_FRAMEBUFFER_SRGB);


    // Log  
    logGLParameters();



}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
    {
        shadows = !shadows;
        shadowsKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        shadowsKeyPressed = false;
    }

    //cube (move cube position with arrows)
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        c_z += 0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        c_z -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        c_x += 0.1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        c_x -= 0.1;

    //volumetric control

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        start_lightPos=vec3(1.5,1.5,7.5);
        volumetric_control =vec3(-1.0f, -1.0f, -3.0f);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        start_lightPos = vec3(-2.5, 1.5, 7.5);
        volumetric_control = vec3(-1.0f, 1.0f, -3.0f);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        start_lightPos = vec3(0, 1.5, -12.0);
        volumetric_control = vec3(1.0f, 1.5f, -3.0f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
{
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
}

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}



int main(void)
{
    try
    {
        initialize();
        mainLoop();
        free();


    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }

    return 0;
}



