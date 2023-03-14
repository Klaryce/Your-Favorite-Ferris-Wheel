#include<windows.h>
#include <GL/glu.h>
#include <QGLWidget>
#include <QTimer>
#include <iostream>
#include <math.h>
#include "SkyWheelWidget.h"

using namespace std;

#define PI 3.14159265

// Define the value of enlarged times
double enlarge_times = 10;

// Define the scales of components
double CIRCLE_R = 2.82843*enlarge_times;
double CIRCLE_OFF = 0.9*enlarge_times;
double HALF_CIRCLE_THICK = 0.1*enlarge_times;
double HALF_HOLDER_WIDTH = 0.1*enlarge_times, HALF_HOLDER_LENGTH = 1.414215*enlarge_times;
double HALF_AXIS_WIDTH = 0.1*enlarge_times, HALF_AXIS_LENGTH = 1.3*enlarge_times;
double HALF_SUPPORT_WIDTH = 0.15*enlarge_times, HALF_SUPPORT_LENGTH = 2.5*enlarge_times;
double HALF_CUBE_EDGE = 0.5*enlarge_times;
double HANG_R = 0.1*enlarge_times, HANG_LENGTH = 0.75*enlarge_times;
double HALF_EDGE_WIDTH = 0.5*enlarge_times;
double GLASS_WIDTH = HALF_CUBE_EDGE - 0.05*enlarge_times;
double GLASS_HEIGHT = 0.5*HALF_CUBE_EDGE - 0.05*enlarge_times;
double GLASS_OFF = HALF_CUBE_EDGE - GLASS_HEIGHT;
double HALF_UNDERWINDOW_HIGHT = HALF_CUBE_EDGE - GLASS_HEIGHT;
double UNDERWINDOW_OFF = HALF_CUBE_EDGE - HALF_UNDERWINDOW_HIGHT;
double HALF_KNOB_LENGTH = 0.05*enlarge_times, HALF_KNOB_WIDTH = 0.05*enlarge_times, KNOB_OFF = 0.1*enlarge_times;

// Setting up material properties
typedef struct materialStruct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} materialStruct;

// Frosted iron material
static materialStruct ironMaterials = {
  { 0.65, 0.65, 0.65, 1.0},
  { 0.85, 0.85, 0.85, 1.0},
  { 0.92, 0.92, 0.92, 1.0},
  90
};

// Red plastic material
static materialStruct redPlasticMaterials = {
  { 0.7, 0.0, 0.0, 1.0},
  { 0.8, 0.0, 0.0, 1.0},
  { 0.95, 0.87, 0.87, 1.0},
  70
};

// Yellow plastic material
static materialStruct yellowPlasticMaterials = {
  { 0.7, 0.7, 0.0, 1.0},
  { 0.8, 0.8, 0.0, 1.0},
  { 0.95, 0.95, 0.87, 1.0},
  70
};


// Blue plastic material
static materialStruct bluePlasticMaterials = {
  { 0.2, 0.2, 0.7, 1.0},
  { 0.3, 0.3, 0.8, 1.0},
  { 0.45, 0.45, 0.95, 1.0},
  70
};

// Green plastic material
static materialStruct greenPlasticMaterials = {
  { 0, 0.7, 0, 1.0},
  { 0, 0.8, 0, 1.0},
  { 0.87, 0.95, 0.87, 1.0},
  70
};

// White plastic material
static materialStruct whitePlasticMaterials = {
  { 0.75, 0.75, 0.75, 1.0},
  { 0.85, 0.85, 0.85, 1.0},
  { 0.96, 0.96, 0.96, 1.0},
  70
};

// Glass material
static materialStruct glassMaterials = {

    { 0.6, 0.6, 0.6, 1.0},
    { 0.62, 0.62, 0.62, 1.0},
    { 0.67, 0.67, 0.67, 1.0},
    120
};

// Material pointers which indicate the current materials
materialStruct* p_plastic = &redPlasticMaterials;
materialStruct* p_iron = &ironMaterials;
materialStruct* p_glass = &glassMaterials;

// constructor
SkyWheelWidget::SkyWheelWidget(QWidget *parent)
    : QGLWidget(parent){

    // Initialize values
    angle = 30;
    earth_isused = 0;
    marc_isused = 0;
    wood_isused = 0;
    isback = 0;
    earth_ID = 0;
    marc_ID = 0;
    bg_ID = 0;
    wood_ID = 0;
    white_ID = 0;
    xlook_init = 1;
    ylook_init = 1;
    zlook_init = 1;
    rlook_init = 2;
    light_x = 50;
    light_y = 50;
    light_z = 0;
    scale_x = 1;
    scale_y = 1;
    scale_z = 1;
    xlook = xlook_init;
    ylook = ylook_init;
    zlook = zlook_init;
    rlook = rlook_init;
    move_y = 0;
    initial_speed = 0.01;
    rotate_speed = initial_speed;
    connector = gluNewQuadric();
    big_circle = gluNewQuadric();
    inner_big_circle = gluNewQuadric();
    back_torus = gluNewQuadric();
    hang = gluNewQuadric();
    torus = gluNewQuadric();
    sphere_at_axis = gluNewQuadric();
    knob = gluNewQuadric();
    knob_circle = gluNewQuadric();

    // Initialize settings that if the gluQuadric object is allowed to bind a texture
    gluQuadricTexture(sphere_at_axis, GLU_TRUE);
    gluQuadricTexture(connector, GLU_FALSE);
    gluQuadricTexture(hang, GLU_FALSE);
    gluQuadricTexture(torus, GLU_FALSE);
    gluQuadricTexture(back_torus, GLU_FALSE);
    gluQuadricTexture(knob, GLU_FALSE);
    gluQuadricTexture(knob_circle, GLU_FALSE);
    gluQuadricTexture(inner_big_circle, GLU_FALSE);

    // Set the shade model as smooth
    glShadeModel(GL_SMOOTH);
}

// destructor
SkyWheelWidget::~SkyWheelWidget()
{
  gluDeleteQuadric(sphere_at_axis);
  gluDeleteQuadric(connector);
  gluDeleteQuadric(hang);
  gluDeleteQuadric(torus);
  gluDeleteQuadric(knob);
  gluDeleteQuadric(back_torus);
  gluDeleteQuadric(knob_circle);
  gluDeleteQuadric(inner_big_circle);
}

// called when OpenGL context is set up
void SkyWheelWidget::initializeGL(){

    // load the background texture
    loadBG();
}


// called every time the widget is resized
void SkyWheelWidget::resizeGL(int w, int h){

    // set the viewport to the entire widget
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Create an orthogonal parallel viewport
    glOrtho(-8.0*enlarge_times, 8.0*enlarge_times, -8.0*enlarge_times, 8.0*enlarge_times, -8.0*enlarge_times, 8.0*enlarge_times);
}

void SkyWheelWidget::createCube(double x, double y, double z, double r){

    // Here are the normals, correctly calculated for the cube faces below
    GLfloat normals[][3] = { {1., 0. ,0.}, {-1., 0., 0.}, {0., 0., 1.}, {0., 0., -1.}, {0., 1., 0.}, {0., -1., 0.} };

    glNormal3fv(normals[0]); // face to the positive direction of x-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x+r, y-r, z+r); // bottom left vertex
      glTexCoord2f(1.0, 0.0);
      glVertex3f( x+r, y-r, z-r); // bottom right vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x+r, y+r, z-r); // top right vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x+r, y+r, z+r); // top left vertex
    glEnd();

    glNormal3fv(normals[5]); // face to the negative direction of y-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(1.0, 1.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x+r, y-r, z+r); // top right vertex
      glTexCoord2f(1.0, 0.0);
      glVertex3f( x+r, y-r, z-r); // bottom right vertex
      glTexCoord2f(0.0, 0.0);
      glVertex3f( x-r, y-r, z-r); // bottom left vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x-r, y-r, z+r); // top left vertex
    glEnd();

    glNormal3fv(normals[4]);// face to the positive direction of y-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(1.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x+r, y+r, z+r); // bottom right vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x+r, y+r, z-r); // top right vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x-r, y+r, z-r); // top left vertex
      glTexCoord2f(0.0, 0.0);
      glVertex3f( x-r, y+r, z+r); // bottom left vertex
    glEnd();

    glNormal3fv(normals[3]); // face to the negative direction of z-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(1.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x-r, y-r, z-r); // bottom right vertex
      glTexCoord2f(0.0, 0.0);
      glVertex3f( x+r, y-r, z-r); // bottom left vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x+r, y+r, z-r); // top left vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x-r, y+r, z-r); // top right vertex
    glEnd();

    glNormal3fv(normals[2]); // face to the positive direction of z-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(0.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x-r, y-r, z+r); // bottom left vertex
      glTexCoord2f(1.0, 0.0);
      glVertex3f( x+r, y-r, z+r); // bottom right vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x+r, y-UNDERWINDOW_OFF+HALF_UNDERWINDOW_HIGHT, z+r); // top right vertex (under the window)
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x-r, y-UNDERWINDOW_OFF+HALF_UNDERWINDOW_HIGHT, z+r); // top left vertex (under the window)
    glEnd();

    glNormal3fv(normals[1]); // face to the negative direction of x-axis
    glBegin(GL_POLYGON);
      glTexCoord2f(1.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x-r, y-r, z+r); // bottom right vertex
      glTexCoord2f(0.0, 0.0);
      glVertex3f( x-r, y-r, z-r); // bottom left vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x-r, y+r, z-r); // top left vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x-r, y+r, z+r); // top right vertex
    glEnd();
}

// create a cuboid
void SkyWheelWidget::createRec(double x, double y, double z, double xr, double yr, double zr){

    // Here are the normals, correctly calculated for the cuboid faces below
    GLfloat normals[][3] = { {1., 0. ,0.}, {-1., 0., 0.}, {0., 0., 1.}, {0., 0., -1.}, {0., 1., 0.}, {0., -1., 0.} };

    glNormal3fv(normals[0]); // face to the positive direction of x-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr, y-yr, z+zr); // bottom left vertex
      glVertex3f( x+xr, y-yr, z-zr); // bottom right vertex
      glVertex3f( x+xr, y+yr, z-zr); // top right vertex
      glVertex3f( x+xr, y+yr, z+zr); // top left vertex
    glEnd();

    glNormal3fv(normals[5]); // face to the negative direction of y-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr, y-yr, z+zr); // top right vertex
      glVertex3f( x+xr, y-yr, z-zr); // bottom right vertex
      glVertex3f( x-xr, y-yr, z-zr); // bottom left vertex
      glVertex3f( x-xr, y-yr, z+zr); // top left vertex
    glEnd();

    glNormal3fv(normals[4]); // face to the positive direction of y-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr, y+yr, z+zr); // bottom right vertex
      glVertex3f( x+xr, y+yr, z-zr); // top right vertex
      glVertex3f( x-xr, y+yr, z-zr); // top left vertex
      glVertex3f( x-xr, y+yr, z+zr); // bottom left vertex
    glEnd();

    glNormal3fv(normals[3]); // face to the negative direction of z-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr, y-yr, z-zr); // bottom right vertex
      glVertex3f( x+xr, y-yr, z-zr); // bottom left vertex
      glVertex3f( x+xr, y+yr, z-zr); // top left vertex
      glVertex3f( x-xr, y+yr, z-zr); // top right vertex
    glEnd();

    glNormal3fv(normals[2]); // face to the positive direction of z-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr, y-yr, z+zr); // bottom left vertex
      glVertex3f( x+xr, y-yr, z+zr); // bottom right vertex
      glVertex3f( x+xr, y+yr, z+zr); // top right vertex
      glVertex3f( x-xr, y+yr, z+zr); // top left vertex
    glEnd();

    glNormal3fv(normals[1]); // face to the negative direction of x-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr, y-yr, z+zr); // bottom right vertex
      glVertex3f( x-xr, y-yr, z-zr); // bottom left vertex
      glVertex3f( x-xr, y+yr, z-zr); // top left vertex
      glVertex3f( x-xr, y+yr, z+zr); // top right vertex
    glEnd();
}

// create a oblique parallelepiped
void SkyWheelWidget::createParaSix(GLfloat x, GLfloat y, GLfloat z, GLfloat xr, GLfloat yr, GLfloat zr, GLfloat xoff){

    // calculate normals of the sloping face
    GLfloat di = sqrt(pow(yr, 2) + pow(xoff, 2));
    GLfloat yr2 = yr / di;
    GLfloat xoff2 = xoff / di;

    GLfloat normals[][3] = { {yr2, -xoff2, 0.}, {-yr2, xoff2, 0.}, {0., 0., 1.}, {0., 0., -1.}, {0., 1., 0.}, {0., -1., 0.} };

    glNormal3fv(normals[0]); // face to the positive direction of x-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr-xoff, y-yr, z+zr); // bottom left
      glVertex3f( x+xr-xoff, y-yr, z-zr); // bottom riht
      glVertex3f( x+xr+xoff, y+yr, z-zr); // top left
      glVertex3f( x+xr+xoff, y+yr, z+zr); // top right
    glEnd();

    glNormal3fv(normals[5]); // face to the negative direction of y-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr-xoff, y-yr, z+zr);
      glVertex3f( x+xr-xoff, y-yr, z-zr);
      glVertex3f( x-xr-xoff, y-yr, z-zr);
      glVertex3f( x-xr-xoff, y-yr, z+zr);
    glEnd();

    glNormal3fv(normals[4]); // face to the positive direction of y-axis
    glBegin(GL_POLYGON);
      glVertex3f( x+xr+xoff, y+yr, z+zr);
      glVertex3f( x+xr+xoff, y+yr, z-zr);
      glVertex3f( x-xr+xoff, y+yr, z-zr);
      glVertex3f( x-xr+xoff, y+yr, z+zr);
    glEnd();

    glNormal3fv(normals[3]); // face to the negative direction of z-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr-xoff, y-yr, z-zr);
      glVertex3f( x+xr-xoff, y-yr, z-zr);
      glVertex3f( x+xr+xoff, y+yr, z-zr);
      glVertex3f( x-xr+xoff, y+yr, z-zr);
    glEnd();

    glNormal3fv(normals[2]); // face to the positive direction of z-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr-xoff, y-yr, z+zr);
      glVertex3f( x+xr-xoff, y-yr, z+zr);
      glVertex3f( x+xr+xoff, y+yr, z+zr);
      glVertex3f( x-xr+xoff, y+yr, z+zr);
    glEnd();

    glNormal3fv(normals[1]); // face to the negative direction of x-axis
    glBegin(GL_POLYGON);
      glVertex3f( x-xr-xoff, y-yr, z+zr);
      glVertex3f( x-xr-xoff, y-yr, z-zr);
      glVertex3f( x-xr+xoff, y+yr, z-zr);
      glVertex3f( x-xr+xoff, y+yr, z+zr);
    glEnd();
}

// rotate the coordinate system around z-axis
void SkyWheelWidget::rotateZ(double my_angle){

    glRotatef(my_angle, 0, 0, 1);
}

// calculate the x-coordinate after the coordinate (x, y) rotate "my_angle" degree
double SkyWheelWidget::vRotationTransformX(double x, double y, double my_angle){
    return x*cos(my_angle*(PI/180)) - y*sin(my_angle*(PI/180));
}

// calculate the x-coordinate after the coordinate (x, y) rotate "my_angle" degree
double SkyWheelWidget::vRotationTransformY(double x, double y, double my_angle){
    return x*sin(my_angle*(PI/180)) + y*cos(my_angle*(PI/180));
}

// slot to rotate the sky wheel by hand
void SkyWheelWidget::updateAngle(int i){
    self_rotate_angle = i;
    this->repaint();
}

// slot to rotate the sky wheel automatically
void SkyWheelWidget::updateAngleWithTime(){
    self_rotate_angle += rotate_speed;
    this->repaint();
}

// slot to set the speed of rotating
void SkyWheelWidget::updateSpeed(int i){
    rotate_speed = initial_speed + i*0.01;
}

// slot to stop rotating the sky wheel
void SkyWheelWidget::stopRotating(){
    store_speed = rotate_speed;
    rotate_speed = 0;
}

// slot to continue rotating the sky wheel after it is being stopped
void SkyWheelWidget::continueRotating(){
    rotate_speed = store_speed;
}

// slot to inverse the direction of rotating
void SkyWheelWidget::inverseRotating(){
    rotate_speed = -rotate_speed;
}

// slot to set the horizontal perspective
void SkyWheelWidget::updateXLook(int i){

    xlook = xlook_init + i*0.1; // the x-coordinate to look at is linear to the value of the slider
    zlook = sqrt(pow(rlook, 2) - pow(xlook, 2)); // calculate the z-coordinate to look at by Pythagorean theorem

    if(isback>0) // if the back side of the sky wheel is being looked at
        zlook = -zlook; // then zlook should be negative
}

// slot to set the vertical perspective
void SkyWheelWidget::updateYLook(int i){
    ylook  = ylook_init + i*0.1; // the y-coordinate to look at is linear to the value of the slider
}

// slot to look at another side
void SkyWheelWidget::lookBackSide(){
    zlook = -zlook; // change the side to look at (inverse z-coordinate
    if(isback == 0) // if the front side is being looked at
        isback = 1; // mark of looking at the back side
    else            // if the back side is being looked at
        isback = 0; // mark of looking at the front side
}

// slot to set the x-coordinate of the light
void SkyWheelWidget::updateXLightPos(int i){
    light_x = i;
}

// slot to set the y-coordinate of the light
void SkyWheelWidget::updateYLightPos(int i){
    light_y = i;
}

// slot to set the z-coordinate of the light
void SkyWheelWidget::updateZLightPos(int i){
    light_z = i;
}

// slot to set the intensity of the light
void SkyWheelWidget::updatelightIntensify(int i){
    light_intense = i/20.0;
}

// set the size of the sky wheel
void SkyWheelWidget::updateSize(int i){
    scale_x = i*0.02; // size along x-coordinate
    scale_y = i*0.02; // size along y-coordinate
    scale_z = i*0.02; // size along z-coordinate
}

// return the value that the slider should be set to according to the size of the sky wheel
void SkyWheelWidget::returnSize(int *i){

    int tmp = scale_x / 0.02;
    *i = tmp;
}

// slot to increase size of the sky wheel
void SkyWheelWidget::bigger(){

    if (scale_x<=1.80){
        scale_x += 0.02;
        scale_y += 0.02;
        scale_z += 0.02;
    }
}

// slot to decrease size of the sky wheel
void SkyWheelWidget::smaller(){

    if (scale_x>=1.02 && scale_y>=1.02 && scale_z>=1.02){
        scale_x -= 0.02;
        scale_y -= 0.02;
        scale_z -= 0.02;
    }
}

// slot to move the sky wheel vertically
void SkyWheelWidget::updateYMove(int i){

    move_y = i*0.5;
}

// slot to paint the sky wheel to red
void SkyWheelWidget::toRed(){
    p_plastic = &redPlasticMaterials;
}

// slot to paint the sky wheel to yellow
void SkyWheelWidget::toYellow(){
    p_plastic = &yellowPlasticMaterials;
}

// slot to paint the sky wheel to blue
void SkyWheelWidget::toBlue(){
    p_plastic = &bluePlasticMaterials;
}

// slot to paint the sky wheel to green
void SkyWheelWidget::toGreen(){
    p_plastic = &greenPlasticMaterials;
}

// slot to paint the sky wheel to white
void SkyWheelWidget::toWhite(){
    p_plastic = &whitePlasticMaterials;
}

// slot to cover/remove the earth texture on the spheres on axis
void SkyWheelWidget::toEarth(){

    if(earth_isused > 0){               // if the earth texture is being used
        glDeleteTextures(1, &earth_ID); // delete it
        earth_isused = 0;               // mark it as unused
    }
    else{                               // if the earth texture is not being used
        loadEarth();                    // load earth texture
        glEnable(GL_TEXTURE_2D);        // allow 2D texture
        earth_isused = 1;               // mark it as used
    }
}

// slot to cover/remove the Marc texture on the boxes
void SkyWheelWidget::toMarc(){

    if(marc_isused > 0){                    // if the Marc texture is being used
        glDeleteTextures(1, &marc_ID);      // delete it
        marc_isused = 0;                    // mark it as unused
    }
    else{                                   // if the Marc texture is not being used
        loadMarc();                         // load Marc texture
        glEnable(GL_TEXTURE_2D);            // allow 2D texture
        if (wood_isused > 0){               // if the wood texture is being used
            glDeleteTextures(1, &wood_ID);  // delete the wood texture
            wood_isused = 0;                // mark the wood texture as unused
        }
        marc_isused = 1;                    // mark the Marc texture as used
    }
}

// slot to cover/remove the wood texture on the boxes
void SkyWheelWidget::toWood(){

    if(wood_isused > 0){                    // if the wood texture is being used
        glDeleteTextures(1, &wood_ID);      // delete it
        wood_isused = 0;                    // mark it as unused
    }
    else{                                   // if the wood texture is not being used
        loadWood();                         // load wood texture
        glEnable(GL_TEXTURE_2D);            // allow 2D texture
        if (marc_isused > 0){               // if the Marc texture is being used
            glDeleteTextures(1, &marc_ID);  // delete the Marc texture
            marc_isused = 0;                // mark the Marc texture as unused
        }
        wood_isused = 1;                    // mark the wood texture as used
    }
}

// slot to look at an upper position
void SkyWheelWidget::moveUp(){
    ylook += 0.1;
}

// slot to look at a lower position
void SkyWheelWidget::moveDown(){
    ylook -= 0.1;
}

// slot to look at further left position
void SkyWheelWidget::moveLeft(){
    xlook -= 0.1;
}

// slot to look at further right position
void SkyWheelWidget::moveRight(){
    xlook += 0.1;
}

// slot to create a piece of glass
void SkyWheelWidget::createGlass(double x, double y, double w, double h){

    glPushMatrix(); // store the current matrix

    GLfloat normals[][3] = { {0., 0., 1.} }; // create the normal

    glEnable(GL_BLEND);  // blend colors
    glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR); // set blend functions
    glDepthMask(GL_FALSE); // close the depth mask, allow the behind object to be seen

    glBindTexture(GL_TEXTURE_2D, white_ID); // bind the white texture to simulate the half-transparent window

    glNormal3fv(normals[0]); // the normal points to the positive direction of z-axis
    glBegin(GL_POLYGON); // begin to create a polygon
      glTexCoord2f(0.0, 0.0); // Mapping 2D texture coordinates to 3D spatial object faces
      glVertex3f( x-w, y-h, HALF_CUBE_EDGE+0.01); // bottom left vertex
      glTexCoord2f(1.0, 0.0);
      glVertex3f( x+w, y-h, HALF_CUBE_EDGE+0.01); // bottom right vertex
      glTexCoord2f(1.0, 1.0);
      glVertex3f( x+w, y+h, HALF_CUBE_EDGE+0.01); // top right vertex
      glTexCoord2f(0.0, 1.0);
      glVertex3f( x-w, y+h, HALF_CUBE_EDGE+0.01); // top left vertex
    glEnd();

    glBindTexture(GL_TEXTURE_2D, -1); // remove the texture

    glDepthMask(GL_TRUE); // open the depth mask
    glDisable(GL_BLEND); // stop blending colors

    glPopMatrix(); // restore the matrix
}

// draw hangs
void SkyWheelWidget::draw_hangs(){

    glPushMatrix();

    // set the materials
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient); // reflected light in the ambient light
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse); // reflected light in the diffuse light
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular); // reflected light in the specular light
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess); // specular gloss

    // rotate the coordinate system around x-axis to create cylinders vertically
    glRotatef(90, 1, 0, 0);

    // calculate the coordinates after revolution
    double xh1 = vRotationTransformX(0, -CIRCLE_R, -self_rotate_angle);
    double zh1 = vRotationTransformY(0, -CIRCLE_R, -self_rotate_angle);
    double xh2 = vRotationTransformX(0, 2*CIRCLE_R, -self_rotate_angle);
    double zh2 = vRotationTransformY(0, 2*CIRCLE_R, -self_rotate_angle);
    double xh3 = vRotationTransformX(-CIRCLE_R, -CIRCLE_R, -self_rotate_angle);
    double zh3 = vRotationTransformY(-CIRCLE_R, -CIRCLE_R, -self_rotate_angle);
    double xh4 = vRotationTransformX(2*CIRCLE_R, 0, -self_rotate_angle);
    double zh4 = vRotationTransformY(2*CIRCLE_R, 0, -self_rotate_angle);
    double xh5 = vRotationTransformX(-CIRCLE_R-2*enlarge_times, -2*enlarge_times, -self_rotate_angle);
    double zh5 = vRotationTransformY(-CIRCLE_R-2*enlarge_times, -2*enlarge_times, -self_rotate_angle);
    double xh6 = vRotationTransformX(0, 4*enlarge_times, -self_rotate_angle);
    double zh6 = vRotationTransformY(0, 4*enlarge_times, -self_rotate_angle);
    double xh7 = vRotationTransformX(4*enlarge_times, 0, -self_rotate_angle);
    double zh7 = vRotationTransformY(4*enlarge_times, 0, -self_rotate_angle);
    double xh8 = vRotationTransformX(0, -4*enlarge_times, -self_rotate_angle);
    double zh8 = vRotationTransformY(0, -4*enlarge_times, -self_rotate_angle);
    double xh9 = vRotationTransformX(-2*enlarge_times, 2*enlarge_times, -self_rotate_angle);
    double zh9 = vRotationTransformY(-2*enlarge_times, 2*enlarge_times, -self_rotate_angle);

    // move and create cylinders
    glTranslatef(xh1, 0., zh1);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh2, 0., zh2);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh3, 0.,zh3);
    gluCylinder(hang, HANG_R, HANG_R,0.5* HANG_LENGTH, 32, 1);

    glTranslatef(xh4, 0., zh4);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh5, 0., zh5);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh6, 0., zh6);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh7, 0., zh7);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh8, 0., zh8);
    gluCylinder(hang, HANG_R, HANG_R, 0.5*HANG_LENGTH, 32, 1);

    glTranslatef(xh9, 0., zh9);

    // rotate back the coordinate system
    glRotatef(-90, 1, 0, 0);

    glPopMatrix();
}

// draw knobs behind the boxes
void SkyWheelWidget::draw_knobs(){

    glPushMatrix();

    // set the materials of knobs
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess);

    // calculate the coordinates after revolution
    double xh1 = vRotationTransformX(0, -CIRCLE_R, self_rotate_angle);
    double zh1 = vRotationTransformY(0, -CIRCLE_R, self_rotate_angle);
    double xh2 = vRotationTransformX(0, 2*CIRCLE_R, self_rotate_angle);
    double zh2 = vRotationTransformY(0, 2*CIRCLE_R, self_rotate_angle);
    double xh3 = vRotationTransformX(-CIRCLE_R, -CIRCLE_R, self_rotate_angle);
    double zh3 = vRotationTransformY(-CIRCLE_R, -CIRCLE_R, self_rotate_angle);
    double xh4 = vRotationTransformX(2*CIRCLE_R, 0, self_rotate_angle);
    double zh4 = vRotationTransformY(2*CIRCLE_R, 0, self_rotate_angle);
    double xh5 = vRotationTransformX(-CIRCLE_R-2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double zh5 = vRotationTransformY(-CIRCLE_R-2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double xh6 = vRotationTransformX(0, 4*enlarge_times, self_rotate_angle);
    double zh6 = vRotationTransformY(0, 4*enlarge_times, self_rotate_angle);
    double xh7 = vRotationTransformX(4*enlarge_times, 0, self_rotate_angle);
    double zh7 = vRotationTransformY(4*enlarge_times, 0, self_rotate_angle);
    double xh8 = vRotationTransformX(0, -4*enlarge_times, self_rotate_angle);
    double zh8 = vRotationTransformY(0, -4*enlarge_times, self_rotate_angle);
    double xh9 = vRotationTransformX(-2*enlarge_times, 2*enlarge_times, self_rotate_angle);
    double zh9 = vRotationTransformY(-2*enlarge_times, 2*enlarge_times, self_rotate_angle);

    // inverse the direction of normals
    gluQuadricOrientation(knob_circle, GLU_INSIDE);

    // move and create knobs
    glTranslatef(xh1-HALF_CUBE_EDGE+KNOB_OFF+HALF_KNOB_WIDTH, zh1-HALF_CIRCLE_THICK*2-HANG_LENGTH, -HALF_CUBE_EDGE-HALF_KNOB_LENGTH);  // move the coordinate system
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1); // knob body
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1); // knob cover

    glTranslatef(xh2, zh2, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh3, zh3,0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh4, zh4, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh5, zh5, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh6, zh6, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh7, zh7, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh8, zh8, 0);
    gluCylinder(knob, HALF_KNOB_WIDTH, HALF_KNOB_WIDTH, HALF_KNOB_LENGTH, 32, 1);
    gluDisk(knob_circle, 0,HALF_KNOB_WIDTH, 32, 1);

    glTranslatef(xh9+HALF_CUBE_EDGE-KNOB_OFF-HALF_KNOB_WIDTH, zh9+HALF_CIRCLE_THICK*2+HANG_LENGTH, HALF_CUBE_EDGE+HALF_KNOB_LENGTH);

    glPopMatrix();
}

// draw 8 boxes
void SkyWheelWidget::draw_cubes(){

    glPushMatrix();

    // set the plastic material
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_plastic->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_plastic->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_plastic->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_plastic->shininess);

    // calculate the coordinates after revolution
    double x1 = vRotationTransformX(2*enlarge_times, 2*enlarge_times, self_rotate_angle);
    double y1 = vRotationTransformY(2*enlarge_times, 2*enlarge_times, self_rotate_angle);
    double x2 = vRotationTransformX(2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double y2 = vRotationTransformY(2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double x3 = vRotationTransformX(-2*enlarge_times, 2*enlarge_times, self_rotate_angle);
    double y3 = vRotationTransformY(-2*enlarge_times, 2*enlarge_times, self_rotate_angle);
    double x4 = vRotationTransformX(-2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double y4 = vRotationTransformY(-2*enlarge_times, -2*enlarge_times, self_rotate_angle);
    double x5 = vRotationTransformX(0*enlarge_times, 2.82843*enlarge_times, self_rotate_angle);
    double y5 = vRotationTransformY(0*enlarge_times, 2.82843*enlarge_times, self_rotate_angle);
    double x6 = vRotationTransformX(2.82843*enlarge_times, 0*enlarge_times, self_rotate_angle);
    double y6 = vRotationTransformY(2.82843*enlarge_times, 0*enlarge_times, self_rotate_angle);
    double x7 = vRotationTransformX(-2.82843*enlarge_times, 0*enlarge_times, self_rotate_angle);
    double y7 = vRotationTransformY(-2.82843*enlarge_times, 0*enlarge_times, self_rotate_angle);
    double x8 = vRotationTransformX(0*enlarge_times, -2.82843*enlarge_times, self_rotate_angle);
    double y8 = vRotationTransformY(0*enlarge_times, -2.82843*enlarge_times, self_rotate_angle);

    if(marc_isused > 0)                         // if the texture Marc is marked as used
        glBindTexture(GL_TEXTURE_2D, marc_ID);  // bind with Marc
    else                                        // if the texture Marc is not marked as used
        glBindTexture(GL_TEXTURE_2D, wood_ID);  // bind with the texture wood

    // create the eight cubes
    createCube(x1, y1-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x2, y2-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x3, y3-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x4, y4-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x5, y5-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x6, y6-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x7, y7-HANG_LENGTH, 0, HALF_EDGE_WIDTH);
    createCube(x8, y8-HANG_LENGTH, 0, HALF_EDGE_WIDTH);

    // set the glass material
    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_glass->ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_glass->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_glass->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_glass->shininess);

    loadWhite(); // load the texture white

    // create eight windows
    createGlass(x1, y1-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x2, y2-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x3, y3-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x4, y4-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x5, y5-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x6, y6-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x7, y7-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);
    createGlass(x8, y8-HANG_LENGTH+GLASS_OFF, HALF_EDGE_WIDTH, GLASS_HEIGHT);

    glPopMatrix();
}

// draw two wheels
void SkyWheelWidget::draw_wheels(){

    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient); // set the materials of wheels
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess);

    rotateZ(self_rotate_angle); // rotate the coordinate system due to the rotating sky wheel

    gluQuadricOrientation(back_torus, GLU_INSIDE); // change the direction of normals
    gluQuadricOrientation(inner_big_circle, GLU_INSIDE);

    // front circle
    glTranslatef(0., 0., CIRCLE_OFF); // move the coordinate system

    gluCylinder(big_circle,CIRCLE_R+HALF_CIRCLE_THICK,CIRCLE_R+HALF_CIRCLE_THICK,HALF_HOLDER_WIDTH*2,32,5); // outer cylinder
    gluCylinder(inner_big_circle,CIRCLE_R-HALF_CIRCLE_THICK,CIRCLE_R-HALF_CIRCLE_THICK,HALF_HOLDER_WIDTH*2,32,5); // inner cylinder

    gluDisk(back_torus, CIRCLE_R-HALF_CIRCLE_THICK, CIRCLE_R+HALF_CIRCLE_THICK, 50, 10); // back torus
    glTranslatef(0., 0., HALF_HOLDER_WIDTH*2); // move the coordinate system
    gluDisk(torus, CIRCLE_R-HALF_CIRCLE_THICK, CIRCLE_R+HALF_CIRCLE_THICK, 50, 10); // front torus

    glTranslatef(0., 0., -HALF_HOLDER_WIDTH*2); // move the coordinate system
    glTranslatef(0., 0., -CIRCLE_OFF); // move the coordinate system

    // back circle
    glTranslatef(0., 0., -CIRCLE_OFF-HALF_HOLDER_WIDTH*2); // move the coordinate system
    gluCylinder(big_circle,CIRCLE_R+HALF_CIRCLE_THICK,CIRCLE_R+HALF_CIRCLE_THICK,HALF_HOLDER_WIDTH*2,32,5); // outer cylinder
    gluCylinder(inner_big_circle, CIRCLE_R-HALF_CIRCLE_THICK,CIRCLE_R-HALF_CIRCLE_THICK,HALF_HOLDER_WIDTH*2,32,5); // inner cylinder

    gluDisk(back_torus, CIRCLE_R-HALF_CIRCLE_THICK, CIRCLE_R+HALF_CIRCLE_THICK, 50, 10); // back torus
    glTranslatef(0., 0., HALF_HOLDER_WIDTH*2); // move the coordinate system
    gluDisk(torus, CIRCLE_R-HALF_CIRCLE_THICK, CIRCLE_R+HALF_CIRCLE_THICK, 50, 10); // front torus
    glTranslatef(0., 0., CIRCLE_OFF); // move the coordinate system

    rotateZ(-self_rotate_angle); // rotate back the coordinate system

    glPopMatrix();
}

// draw 8 connectors and 8 spokes
void SkyWheelWidget::draw_connectors(){

    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient); // set the materials of wheels
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess);

    rotateZ(self_rotate_angle); // rotate the coordinate system due to the rotating sky wheel

    glPushMatrix();

    // iterate 8 times to create connectors and spokes in 8 directions
    for (int i=0;i<8;i++) {
      createRec(0, 0+HALF_HOLDER_LENGTH, CIRCLE_OFF, HALF_HOLDER_WIDTH, HALF_HOLDER_LENGTH, HALF_HOLDER_WIDTH); // front spoke
      createRec(0, 0+HALF_HOLDER_LENGTH, -CIRCLE_OFF, HALF_HOLDER_WIDTH, HALF_HOLDER_LENGTH, HALF_HOLDER_WIDTH); // back spoke
      glTranslatef(0., CIRCLE_R, -CIRCLE_OFF); // move to top behind
      gluCylinder(connector,0.1*enlarge_times,0.1*enlarge_times,2*CIRCLE_OFF,32,1); // connector
      glTranslatef(0., -CIRCLE_R, CIRCLE_OFF); // move back to the original position
      rotateZ(45); // rotate 45 degree around z-axis
    }

    glPopMatrix();

    rotateZ(-self_rotate_angle); // rotate back the coordinate system
    glPopMatrix();
}

// draw 6 supporters
void SkyWheelWidget::draw_supporters(){

    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient); // set the materials of supporters
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess);

    // supports in the behind
    createRec(0, 0-HALF_SUPPORT_LENGTH, -HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH); // middle
    createParaSix(-0.75*enlarge_times, 0-HALF_SUPPORT_LENGTH, -HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH, 0.75*enlarge_times); // right
    createParaSix(0.75*enlarge_times, 0-HALF_SUPPORT_LENGTH, -HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH, -0.75*enlarge_times); // left

    // supports in the front
    createRec(0, 0-HALF_SUPPORT_LENGTH, HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH); // middle
    createParaSix(-0.75*enlarge_times, 0-HALF_SUPPORT_LENGTH, HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH, 0.75*enlarge_times); // right
    createParaSix(0.75*enlarge_times, 0-HALF_SUPPORT_LENGTH, HALF_AXIS_LENGTH, HALF_SUPPORT_WIDTH, HALF_SUPPORT_LENGTH, HALF_SUPPORT_WIDTH, -0.75*enlarge_times); // left

    glPopMatrix();
}

// draw the axis and 2 spheres
void SkyWheelWidget::draw_spheres(){

    glPushMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT,    p_iron->ambient); // set the materials of the axis and spheres
    glMaterialfv(GL_FRONT, GL_DIFFUSE,    p_iron->diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,   p_iron->specular);
    glMaterialf(GL_FRONT, GL_SHININESS,   p_iron->shininess);

    rotateZ(self_rotate_angle); // rotate the coordinate system due to the rotating sky wheel

    createRec(0, 0, 0, HALF_AXIS_WIDTH, HALF_AXIS_WIDTH, HALF_AXIS_LENGTH+HALF_SUPPORT_WIDTH*2); // axis
    glTranslatef(0., 0., HALF_AXIS_LENGTH+HALF_SUPPORT_WIDTH*2); // move the coordinate system foreward
    glBindTexture(GL_TEXTURE_2D, earth_ID); // bind the texture
    gluSphere(sphere_at_axis, HALF_SUPPORT_WIDTH*1.65, 50, 5); // create the front sphere
    glTranslatef(0., 0., -2*(HALF_AXIS_LENGTH+HALF_SUPPORT_WIDTH*2)); // move the coordinate system backward
    gluSphere(sphere_at_axis, HALF_SUPPORT_WIDTH*1.65, 50, 5); // create the back sphere
    glTranslatef(0., 0., HALF_AXIS_LENGTH+HALF_SUPPORT_WIDTH*2); // move the coordinate system foreward

    rotateZ(-self_rotate_angle); // rotate the coordinate system back
    glPopMatrix();
}

// draw the sky wheel
void SkyWheelWidget::drawSkyWheel(){

    glTranslatef(0, 15+move_y, 0); // vertical position of the sky wheel

    draw_cubes(); // draw 8 boxes

    draw_hangs(); // draw 8 hangs

    draw_knobs(); // draw 8 knobs

    draw_wheels(); // draw 2 wheels

    draw_connectors(); // draw 8 connectors and 16 spokes

    draw_supporters(); // draw 6 supporters

    draw_spheres(); // draw 1 axis and 2 spheres
}

// called every time the widget needs painting
void SkyWheelWidget::paintGL(){

    // clear the widget
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //preparation of setting the background image
    // close the depth test
    glDisable(GL_DEPTH_TEST);

    // set the scale of the objects
    glScalef(scale_x, scale_y, scale_z);

    glPushMatrix();

    glMatrixMode(GL_PROJECTION); // load the projection matrix
    glLoadIdentity();

    // Create an orthogonal parallel viewport
    glOrtho(-9*enlarge_times, 9*enlarge_times, -9*enlarge_times, 9*enlarge_times, -9*enlarge_times, 9*enlarge_times);

    glMatrixMode(GL_MODELVIEW); // load the view matrix
    glLoadIdentity();

    glDisable(GL_LIGHTING); // close the light
    glEnable(GL_TEXTURE_2D); // enable 2D texture

    glBindTexture(GL_TEXTURE_2D, bg_ID); // bind background texture

    // create background
    glBegin(GL_QUADS);
      glTexCoord2d(0.0, 0.0); glVertex2d(-9.0*enlarge_times*scale_x, -9.0*enlarge_times*scale_x);
      glTexCoord2d(1.0, 0.0); glVertex2d(9.0*enlarge_times*scale_x, -9.0*enlarge_times*scale_y);
      glTexCoord2d(1.0, 1.0); glVertex2d(9.0*enlarge_times*scale_x, 9.0*enlarge_times*scale_y);
      glTexCoord2d(0.0, 1.0); glVertex2d(-9.0*enlarge_times*scale_x, 9.0*enlarge_times*scale_y);
    glEnd();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST); // enable the depth test
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    // set lighting
    GLfloat light_pos[] = {light_x, light_y, light_z, 1.}; // set the position of light. (Positional light)
    GLfloat ambientvalue = 0.5*light_intense; // intensity of ambient light is half of the diffuse and specular light
    GLfloat lightAmbient[4] = { ambientvalue, ambientvalue, ambientvalue, 1.0 }; // ambient light intensity
    GLfloat lightDiffuse[4] = { light_intense, light_intense, light_intense, 1.0 }; // diffuse light intensity
    GLfloat lightSpecular[4] = { light_intense, light_intense, light_intense, 1.0 }; // specular light intensity

    glEnable(GL_LIGHTING); // enable lighting
    glEnable(GL_LIGHT0); // enable light source zero
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); // set the position of light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient); // set the ambient light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); // set the diffuse light
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular); // set the specular light
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,180.); // point source spotlight cut-off angle

    glPopMatrix();

    // draw a sky wheel
    this->drawSkyWheel();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(xlook, ylook, zlook, 0.0,0.0,0.0, 0.0,1.0,0.0); // position to look at

    // flush to screen
    glFlush();
}

// load the texture earth
int SkyWheelWidget::loadEarth()
{
    QImage tex, buf;
    if(!buf.load("your path for textures/earth.ppm")) // if the image cannot be openned
    {
        qWarning("Cannot open the image..."); // print an error message
        return 0;
    }
    tex = convertToGLFormat(buf); // Convert Qt image format buf to opengl image format tex
    glGenTextures(1, &earth_ID); // Open a texture memory, the memory points to earth_ID
    glBindTexture(GL_TEXTURE_2D, earth_ID); // Bind the content of the created texture memory to the texture object GL_TEXTURE_2D.
    //After this code, any future operations on GL_TEXTURE_2D will correspond to the texture object it is bound to at the same time

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits()); // Start actually creating texture data

    // When the displayed texture is smaller than the one loaded in, the GL_LINEAR method is used to handle it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // When the displayed texture is larger than the loaded texture, the GL_LINEAR method is used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 0;
}

// load the texture Marc
int SkyWheelWidget::loadMarc()
{
    QImage tex, buf;
    if(!buf.load("your path for textures/marc.ppm")) // if the image cannot be openned
    {
        qWarning("Cannot open the image..."); // print an error message
        return 0;
    }
    tex = convertToGLFormat(buf); // Convert Qt image format buf to opengl image format tex
    glGenTextures(1, &marc_ID); // Open a texture memory, the memory points to earth_ID
    glBindTexture(GL_TEXTURE_2D, marc_ID); // Bind the content of the created texture memory to the texture object GL_TEXTURE_2D.
    //After this code, any future operations on GL_TEXTURE_2D will correspond to the texture object it is bound to at the same time

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits()); // Start actually creating texture data

    // When the displayed texture is smaller than the one loaded in, the GL_LINEAR method is used to handle it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // When the displayed texture is larger than the loaded texture, the GL_LINEAR method is used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 0;
}

// load the white texture
int SkyWheelWidget::loadWhite()
{
    QImage tex, buf;
    if(!buf.load("your path for textures/white.png")) // if the image cannot be openned
    {
        qWarning("Cannot open the image..."); // print an error message
        return 0;
    }
    tex = convertToGLFormat(buf); // Convert Qt image format buf to opengl image format tex
    glGenTextures(1, &white_ID); // Open a texture memory, the memory points to earth_ID
    glBindTexture(GL_TEXTURE_2D, white_ID); // Bind the content of the created texture memory to the texture object GL_TEXTURE_2D.
    //After this code, any future operations on GL_TEXTURE_2D will correspond to the texture object it is bound to at the same time

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits()); // Start actually creating texture

    // When the displayed texture is smaller than the one loaded in, the GL_LINEAR method is used to handle it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // When the displayed texture is larger than the loaded texture, the GL_LINEAR method is used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 0;
}

// load the background texture
int SkyWheelWidget::loadBG()
{
    QImage tex, buf;
    if(!buf.load("your path for textures/bgimage.jpg")) // if the image cannot be openned
    {
        qWarning("Cannot open the image..."); // print an error message
        return 0;
    }
    tex = convertToGLFormat(buf); // Convert Qt image format buf to opengl image format tex
    glGenTextures(1, &bg_ID); // Open a texture memory, the memory points to earth_ID
    glBindTexture(GL_TEXTURE_2D, bg_ID); // Bind the content of the created texture memory to the texture object GL_TEXTURE_2D.
    //After this code, any future operations on GL_TEXTURE_2D will correspond to the texture object it is bound to at the same time

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits()); // Start actually creating texture

    // When the displayed texture is smaller than the one loaded in, the GL_LINEAR method is used to handle it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // When the displayed texture is larger than the loaded texture, the GL_LINEAR method is used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 0;
}

// load the wood texture
int SkyWheelWidget::loadWood(){

    QImage tex, buf;
    if(!buf.load("your path for textures/wood.jpg")) // if the image cannot be openned ../my_cw2/textures/wood.jpg
    {
        qWarning("Cannot open the image..."); // print an error message
        return 0;
    }
    tex = convertToGLFormat(buf); // Convert Qt image format buf to opengl image format tex
    glGenTextures(1, &wood_ID); // Open a texture memory, the memory points to earth_ID
    glBindTexture(GL_TEXTURE_2D, wood_ID); // Bind the content of the created texture memory to the texture object GL_TEXTURE_2D.
    //After this code, any future operations on GL_TEXTURE_2D will correspond to the texture object it is bound to at the same time

    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits()); // Start actually creating texture

    // When the displayed texture is smaller than the one loaded in, the GL_LINEAR method is used to handle it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // When the displayed texture is larger than the loaded texture, the GL_LINEAR method is used.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return 0;
}
