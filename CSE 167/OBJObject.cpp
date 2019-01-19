#include "OBJObject.h"
#include "Window.h"
#include <iostream>
#include <vector>

int type = 0;
glm::vec3 point_location;
glm::vec3 spot_location;
float light_smooth = 1.0f;
float light_spot = -1.0f;
int NORMAL = 1;
int LIGHT = 0;
int BUNNY = 2;
int DRAGON = 3;
int BEAR = 4;

OBJObject::OBJObject(const char* filepath, int type)
{
    toWorld = glm::mat4(1.0f);
    parse(filepath);
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &NBO);
   
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, (vertices.size()*sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
        3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
        GL_FLOAT, // What type these components are
        GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
        3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
        (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
        3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
        GL_FLOAT, // What type these components are
        GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
        3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
        (GLvoid*)0);
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    if(type == 1){
        //means that it is pointlight
        toWorld = glm::scale(toWorld, glm::vec3(0.1f,0.1f,0.1f));
        toWorld = glm::translate(toWorld, glm::vec3(70,0,0));
    }
    if(type == 2){
        //means that it is a sphere spotlitght
        toWorld = glm::scale(toWorld, glm::vec3(0.2f,0.2f,0.2f));
        toWorld = glm::translate(toWorld, glm::vec3(-50,0,0));
    }
}

OBJObject::~OBJObject(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void OBJObject::parse(const char *filepath)
{
    //parse the whole file
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float r=0,g=0,b=0;  // vertex color
    int t1=0, t2=0, t3=0; //triangle
    int c1,c2;    // characters read from file
    c2 = 1;
    //std::cout<< filepath <<"\n";
    fp = fopen(filepath, "rb");
    if (fp==NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }
    while ((c1 = fgetc(fp)) != EOF){
        //then there are several cases, one is vn, the other is v and the other is #
        if(c1=='v'){
            c2 = fgetc(fp);
            //std:: cout<<(char)c1<<(char)c2<<" ";
            if( c2 == 'n'){
                //case that is vn
                fscanf(fp," %f %f %f\n", &x, &y, &z);
                //it takes glm::vec3
                normals.push_back(glm::vec3(x,y,z));
                //std::cout <<"x y z: "<< x <<" "<<y << " "<<z<<std::endl;
            }
            else if (c1 == 'v' && c2 == ' '){
                //case that is v
                fscanf(fp,"%f %f %f %f %f %f\n", &x, &y, &z, &r, &g, &b);
                vertices.push_back(glm::vec3(x,y,z));
                //std::cout <<"x y z: "<< x <<" "<<y<<" "<<z<<std::endl;
            }
            else{
                while(c2 !='\n'){ // skip a line
                    c2 = fgetc(fp);
                    // ignore the rest
                }
            }
        }
        else if(c1=='f'){
            fscanf(fp," %d//%d %d//%d %d//%d\n", &t1, &t1, &t2, &t2, &t3, &t3);
            indices.push_back(t1-1);
            indices.push_back(t2-1);
            indices.push_back(t3-1);
        }
        else{
            //case that we ignore the whole line
            while(c2 !='\n'){ // skip a line
                c2 = fgetc(fp);
                // ignore the rest
            }
        }
    }
    fclose(fp);
    float xmax = vertices[0].x, ymax = vertices[0].y, zmax = vertices[0].z;
    float xmin = vertices[0].x, ymin = vertices[0].y, zmin = vertices[0].z;
    // here are all the maximum
    for (unsigned int i = 0; i < vertices.size(); ++i) {
        if(vertices[i].x< xmin){
            xmin = vertices[i].x;
        }
        else if (vertices[i].x > xmax){
            xmax = vertices[i].x;
        }
        if(vertices[i].y< ymin){
            ymin = vertices[i].y;
        }
        else if (vertices[i].y > ymax){
            ymax = vertices[i].y;
        }
        if(vertices[i].z< zmin){
            zmin = vertices[i].z;
        }
        else if (vertices[i].z > zmax){
            zmax = vertices[i].z;
        }
    }
    //calculate the middle point
    float middleX = (xmin+xmax)/2.0;
    float middleY = (ymin+ymax)/2.0;
    float middleZ = (zmin+zmax)/2.0;
    float x_shift = 0 - middleX;
    float y_shift = 0 - middleY;
    float z_shift = 0 - middleZ;
    
    //need to calculate a constant
    float max = xmin+xmax+x_shift;
    if(max < (ymin+ymax+y_shift)){
        max = ymin+ymax+y_shift;
    }
    if(max<zmin+zmax+z_shift){
        max =zmin+zmax+z_shift;
    }
    if(max == 0){
        max = 0.9*xmax;
        if(max < 0.9*ymax){
            max = 0.9*ymax;
        }
        if(max <0.9*zmax){
            max = 0.9*zmax;
        }
    }
    float zoom = 6/max;
    int i = 0;
    for(i = 0; i< vertices.size(); i++){
        vertices[i].x =zoom*(vertices[i].x +x_shift);
        vertices[i].y = zoom*(vertices[i].y+y_shift);
        vertices[i].z = zoom*(vertices[i].z+z_shift);
    }
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw(GLuint shaderProgram)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 model = Window::V;
    glm::mat4 view = toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    GLuint uModel = glGetUniformLocation(shaderProgram, "model");
    GLuint uView = glGetUniformLocation(shaderProgram, "view");
    uPointPosition = glGetUniformLocation(shaderProgram, "pointLight.position");
    uPointColor = glGetUniformLocation(shaderProgram, "pointLight.color");
    uAmbient = glGetUniformLocation(shaderProgram, "material.ambient");
    uDiffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
    uSpecular  = glGetUniformLocation(shaderProgram, "material.specular");
    uShininess  = glGetUniformLocation(shaderProgram, "material.shininess");
    uColoring = glGetUniformLocation(shaderProgram, "normal_coloring");
    uObjcolor = glGetUniformLocation(shaderProgram, "Objcolor");
    uSpotColor = glGetUniformLocation(shaderProgram, "spotLight.color");
    uSpotPosition = glGetUniformLocation(shaderProgram, "spotLight.position");
    uSpotCutoff = glGetUniformLocation(shaderProgram, "spotLight.cutoff");
    uSpotExponent = glGetUniformLocation(shaderProgram, "spotLight.exponent");
    uSpotDirection = glGetUniformLocation(shaderProgram, "spotLight.direction");
    uviewPos = glGetUniformLocation(shaderProgram, "viewPos");
   
    // Now send these values to the shader program;
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &view[0][0]);
    
    //send more
    glm::vec3 pointcolor = glm::vec3(1.f,1.0f,0.0f);
    glm::vec3 pointposition = point_location;
    glm::vec3 spotcolor = glm::vec3(0.f,0.f,1.f);
    glm::vec3 spotposition = spot_location;
    //std::cout << pointposition.x <<" "<< pointposition.y<<" "<<pointposition.z <<std::endl;
    glm::vec3 spotdirection = -spot_location;
    glm::vec3 viewposition = glm::vec3(0.0f, 0.0f, 20.0f);
    glUniform1f(uColoring, type);
    glUniform1f(uSpotCutoff, glm::cos(glm::radians(12.5f)) /light_spot);
    glUniform1f(uSpotExponent, light_smooth);
    
    glUniform3fv(uPointColor, 1, &pointcolor[0]);
    glUniform3fv(uPointPosition, 1, &pointposition[0]);
    glUniform3fv(uSpotColor, 1, &spotcolor[0]);
    glUniform3fv(uSpotPosition, 1, &spotposition[0]);
    glUniform3fv(uSpotDirection, 1, &spotdirection[0]);
    glUniform3fv(uviewPos, 1, &viewposition[0]);
    
    //depending on the type, we will perform different tasks
    glm::vec3 ambient = glm::vec3(0.8f,0.8f,0.0f);
    glm::vec3 diffuse = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 specular = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 objcolor = glm::vec3(0.3f,0.3f,0.3f);
    if(type == BUNNY){ //no diffuse and high specular
        ambient = glm::vec3(0.9f,0.9f,0.9f);
        diffuse = glm::vec3(0.0f,0.0f,0.0f);
        specular = glm::vec3(1.0f,1.0f,1.0f);
        objcolor = glm::vec3(0.3f,0.3f,0.3f);
        glUniform1f(uShininess, 1.0f);
    }
    else if(type == DRAGON){//no shininess  and only diffuse speculation
        ambient = glm::vec3(0.0f,0.0f,0.0f);
        diffuse = glm::vec3(1.0f,1.0f,1.0f);
        specular = glm::vec3(0.0f,0.0f,0.0f);
        objcolor = glm::vec3(0.3f,0.3f,0.3f);
        glUniform1f(uShininess, 0.0f);//green
    }
    else if(type == BEAR){ //high diffuse and high specular
        ambient = glm::vec3(0.1f,0.1f,0.1f);
        diffuse = glm::vec3(1.0f,1.0f,1.0f);
        specular = glm::vec3(1.0f,1.0f,1.0f);
        objcolor = glm::vec3(0.3f,0.3f,0.3f);
        glUniform1f(uShininess, 1.0f);//blue
    }
    glUniform3fv(uAmbient,1, &ambient[0]);
    glUniform3fv(uDiffuse,1, &diffuse[0]);
    glUniform3fv(uSpecular,1, &specular[0]);
    glUniform3fv(uObjcolor,1, &objcolor[0]);
    // the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::update()
{
    //spin(1.0f);
}
void OBJObject::point_update(glm::vec3 a){
    point_location = a;
}
void OBJObject::spot_update(glm::vec3 b){
    spot_location = b;
}
void OBJObject::smooth_update(float c){
    light_smooth = c;
}
void OBJObject::size_update(float c){
    light_spot = c;
}
void OBJObject::upload(int info){
    type = info;
}
/*
void OBJObject::spin(float deg)
{
    // If you haven't figured it out from the last project, this is how you fix spin's behavior
    toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}
*/

