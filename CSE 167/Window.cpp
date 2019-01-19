#include <math.h>
#include "Window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";
//Cube * cube;
OBJObject* cur;
OBJObject* bunny;
OBJObject* dragon;
OBJObject* bear;
OBJObject* sphere;
OBJObject* point;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "/Users/apple/Desktop/CSE 167/CSE 167/shader.vert"
#define FRAGMENT_SHADER_PATH "/Users/apple/Desktop/CSE 167/CSE 167/shader.frag"


// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
int kb = 4;
int current = 1;
int changesize = 0;
int buttonsize = 0;
int scale = 1.5;
int pressed = 0;
float normal_coloring = 0;
float angle;
float smooth = 1;
float window_cutoff = -1;
int CHANGE_TO_NORMAL = 1;
int CHANGE_TO_LIGHT = 0;
int CHANGE_TO_BUNNY = 2;
int CHANGE_TO_DRAGON = 3;
int CHANGE_TO_BEAR = 4;
int SPHERE = 5;
int toggleobject = 0;
int togglepoint = 0;
int togglespot = 0;
glm::vec3 last_position;
glm::vec3 current_position;
glm::vec3 move;
glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
    bunny = new OBJObject("/Users/apple/Desktop/CSE 167/CSE 167/bunny.obj",0);
    dragon = new OBJObject("/Users/apple/Desktop/CSE 167/CSE 167/dragon.obj",0);
    bear = new OBJObject("/Users/apple/Desktop/CSE 167/CSE 167/bear.obj",0);
    sphere = new OBJObject("/Users/apple/Desktop/CSE 167/CSE 167/sphere.obj",2);
    point = new OBJObject("/Users/apple/Desktop/CSE 167/CSE 167/sphere.obj",1);
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(bunny);
    delete(dragon);
    delete(bear);
    delete(sphere);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
    bunny->update();
    dragon->update();
    bear->update();
    sphere->update();
    point->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
    sphere->draw(shaderProgram);
    point->draw(shaderProgram);
    if(kb == 2){
        if(changesize == -1){
            glPointSize(dragon->size = buttonsize - 0.5);
            buttonsize = dragon->size;
            changesize = 0;
        }
        else if(changesize == 1){
            glPointSize(dragon->size = buttonsize + 1.5);
            buttonsize = dragon->size;
            changesize = 0;
        }
        cur = dragon;
        dragon->draw(shaderProgram);
        
    }
    else if(kb == 3 ){
        if(changesize == -1){
            glPointSize(bear->size = buttonsize - 0.5);
            buttonsize = bear->size;
            changesize = 0;
        }
        else if(changesize == 1){
            glPointSize(bear->size = buttonsize + 1.5);
            buttonsize = bear->size;
            changesize = 0;
        }
        current = 2;
        cur = bear;
        bear->draw(shaderProgram);
        
    }
    else if(kb == 1){
        if(changesize == -1){
            glPointSize(bunny->size = buttonsize - 0.5);
            buttonsize = bunny->size;
            changesize = 0;
        }
        else if(changesize == 1){
            glPointSize(bunny->size = buttonsize + 1.5);
            buttonsize = bunny->size;
            changesize = 0;
        }
        cur = bunny;
        cur->upload(CHANGE_TO_BUNNY);
        bunny->draw(shaderProgram);
        
    }
    else{
        if(changesize == -1){
            glPointSize(bunny->size = buttonsize - 0.5);
            buttonsize = bunny->size;
            changesize = 0;
        }
        else if(changesize == 1){
            glPointSize(bunny->size = buttonsize + 1.5);
            buttonsize = bunny->size;
            changesize = 0;
        }
        cur = bunny;
        bunny->draw(shaderProgram);
        
    }
    // Render objects
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(yoffset+xoffset > 0 || changesize == 1){
        //it's positive, so it's zoom in
        scale = 1.5;
        cur->toWorld = glm::scale(cur->toWorld, glm::vec3(1.02f,1.02f,1.02f));
        changesize = 0;
    }
    else if(yoffset+xoffset<0 || changesize == -1){
        scale = -1.5;
        cur->toWorld = glm::scale(cur->toWorld, glm::vec3(0.98f,0.98f,0.98f));
        changesize = 0;
        //it's negative, so it's shrinking
    }
    
}
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        else{
            if(key == GLFW_KEY_N){
                if(normal_coloring == 0){
                    normal_coloring = 1;
                    cur->upload(CHANGE_TO_NORMAL);
                }
                else{
                    normal_coloring = 0;
                    cur->upload(CHANGE_TO_LIGHT);
                }
            }
            if(mods == GLFW_MOD_SHIFT && key == GLFW_KEY_E){
                changesize = 1;
                smooth = smooth *1.5f;
                sphere->smooth_update(smooth);
            }
            else if(key == GLFW_KEY_E){
                changesize = -1;
                smooth = smooth/1.5f;
                sphere->smooth_update(smooth);
            }
            if(mods == GLFW_MOD_SHIFT && key == GLFW_KEY_W){
                changesize = 1;
                window_cutoff = window_cutoff *1.01f;
                sphere->size_update(window_cutoff);
               
            }
            else if(key == GLFW_KEY_W){
                changesize = -1;
                window_cutoff = window_cutoff /1.01f;
                sphere->size_update(window_cutoff);
            }
            //not closing window, handling F1, F2, F3
            if(key == GLFW_KEY_F1){
                kb = 1;
                cur->upload(CHANGE_TO_BUNNY);
            }
            else if(key == GLFW_KEY_F2){
                kb = 2;
                cur->upload(CHANGE_TO_DRAGON);
            }
            else if(key == GLFW_KEY_F3){
                kb = 3;
                cur->upload(CHANGE_TO_BEAR);
            }
            if(key == GLFW_KEY_0){
                //enable or disable object rotation
                if(toggleobject == 0){
                    toggleobject = 1;
                }
                else{
                    toggleobject = 0;
                }
            }
            if(key == GLFW_KEY_1){
                //enable or disable point light rotation
                if(togglepoint == 0){
                    togglepoint = 1;
                }
                else{
                    togglepoint = 0;
                }
            }
            if(key == GLFW_KEY_2){
                //enable or disable spot light rotation
                if(togglespot == 0){
                    togglespot = 1;
                }
                else{
                    togglespot = 0;
                }
            }
        }
	}
}

glm::vec3 Window::trackBallMapping(double x, double y){
    glm::vec3 v = glm::vec3(0,0,0);
    double d;
    v.x = (2.0*x-width)/width;
    v.y = (height-2.0*y)/height;
    
    v.z = 0.0;
    d = glm::length(v);
    d = (d<1.0) ? d : 1.0;
    v.z = sqrtf(1.001 - d*d);
    v = glm::normalize(v);
    return v;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(pressed == 0 && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        //glfwGetCursorPos(window, &x, &y);
        pressed = 1;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        pressed = 0;
    }
}
void Window::cursor_callback( GLFWwindow *window, double x, double y ){
    //only continue if the mouse is pressed
    if(pressed == 1){
        last_position = Window::trackBallMapping(x, y);
        pressed = -1;
    }
    else if(pressed == 0){
        //do nothing
        return;
    }
    current_position = Window::trackBallMapping(x, y);
    move = glm::cross(last_position, current_position);
    if(glm::length(current_position-last_position) < 0.01){
        return; //do nothing
    }
    angle = asin(glm::length(move));
    angle = angle/10;
    // here we move the obj
    if(toggleobject == 0){
        cur->toWorld = glm::rotate(cur->toWorld, angle, move);
    }
    if(togglespot == 1){
        glm::mat4 temp = sphere->toWorld;
        temp = glm::translate(temp, glm::vec3(50,0,0 ));
        temp = glm::rotate(temp, angle, move);
        temp = glm::translate(temp, glm::vec3(-50,0,0));
        sphere->toWorld = temp;
        glm::vec3 spot_position = glm::vec3(temp[3][0],temp[3][1],temp[3][2]);
        sphere->spot_update(spot_position);
    }
    if(togglepoint == 1){
        glm::mat4 temp = point->toWorld;
        temp = glm::translate(temp, glm::vec3(-70,0,0 ));
        temp = glm::rotate(temp, angle, move);
        temp = glm::translate(temp, glm::vec3(70,0,0));
        point->toWorld = temp;
        glm::vec3 point_position = glm::vec3(temp[3][0],temp[3][1],temp[3][2]);
        point->point_update(point_position);
    }
}

glm::vec3 Window::crossProduct(glm::vec3 a, glm::vec3 b){
    glm::vec3 p;
    p.x = a.y*b.z - a.z*b.y;
    p.y = a.x*b.z - a.z*b.x;
    p.z = a.x*b.y - a.y*b.x;
    return p;
}

