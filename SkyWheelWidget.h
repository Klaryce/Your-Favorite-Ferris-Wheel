#ifndef SKYWHEELWIDGET_H
#define SKYWHEELWIDGET_H

#include <QGLWidget>
#include <QObject>
#include <GL/glu.h>

using namespace std;

class SkyWheelWidget: public QGLWidget{
    Q_OBJECT
public:
    SkyWheelWidget(QWidget *parent);
    ~SkyWheelWidget();
    GLfloat angle;

public slots:

    // speed
    void updateAngle(int i);
    void updateAngleWithTime();
    void updateSpeed(int i);
    void stopRotating();
    void continueRotating();
    void inverseRotating();

    // light
    void updateXLightPos(int i);
    void updateYLightPos(int i);
    void updateZLightPos(int i);
    void updatelightIntensify(int);

    // look at
    void updateXLook(int i);
    void updateYLook(int i);
    void lookBackSide();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    // move
    void updateYMove(int i);

    // size
    void updateSize(int i);
    void returnSize(int *i);
    void smaller();
    void bigger();

    // material
    void toRed();
    void toYellow();
    void toBlue();
    void toGreen();
    void toWhite();

    // texture
    void toEarth();
    void toMarc();
    void toWood();

protected:
    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();

    // create objects using polygon statement
    void createCube(double x, double y, double z, double r);
    void createRec(double x, double y, double z, double xr, double yr, double zr);
    void createParaSix(GLfloat x, GLfloat y, GLfloat z, GLfloat xr, GLfloat yr, GLfloat zr, GLfloat xoff);

    // rotate the coordinate system around z-axis
    void rotateZ(double angle);

private:

    // calculate x and y coordinate after rotating
    double vRotationTransformX(double x, double y, double my_angle);
    double vRotationTransformY(double x, double y, double my_angle);

    // load textures
    int loadEarth();
    int loadMarc();
    int loadBG();
    int loadWood();
    int loadWhite();
    void createGlass(double x, double y, double w, double h);

    // draw the sky wheel
    void drawSkyWheel();
    void draw_hangs();
    void draw_cubes();
    void draw_wheels();
    void draw_connectors();
    void draw_supporters();
    void draw_spheres();
    void draw_knobs();

    // glu object
    GLUquadricObj *connector;
    GLUquadricObj *big_circle;
    GLUquadricObj *inner_big_circle;
    GLUquadricObj *back_torus;
    GLUquadricObj *hang;
    GLUquadricObj *torus;
    GLUquadricObj *sphere_at_axis;
    GLUquadric *knob;
    GLUquadric *knob_circle;

    GLuint earth_ID, marc_ID, bg_ID, wood_ID, white_ID;
    GLfloat light_x, light_y, light_z, light_intense;
    GLfloat scale_x, scale_y, scale_z, move_y;
    int earth_isused, marc_isused, wood_isused, isback;
    double rotate_speed, initial_speed, store_speed;
    double xlook_init, ylook_init, zlook_init, rlook_init, xlook, ylook, zlook, rlook;
    double self_rotate_angle;

}; // class GLPolygonWidget

#endif // SKYWHEELWIDGET_H
