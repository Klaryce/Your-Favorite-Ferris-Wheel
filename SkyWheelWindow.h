#ifndef SKYWHEELWINDOW_H
#define SKYWHEELWINDOW_H

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "SkyWheelWidget.h"

class SkyWheelWindow: public QWidget{

    Q_OBJECT
    public:

    // constructor / destructor
    SkyWheelWindow(QWidget *parent);
    ~SkyWheelWindow();

    // layouts
    QBoxLayout *windowLayout;
    QHBoxLayout *colorLayout;
    QHBoxLayout *textureLayout;
    QHBoxLayout *perspectiveLayout;
    QHBoxLayout *vertiSliderLayout;
    QHBoxLayout *speedLayout;
    QHBoxLayout *lightLayout;
    QHBoxLayout *distanceLayout;
    QHBoxLayout *horiLookLayout;
    QHBoxLayout *intensityLayout;
    QHBoxLayout *byHandLayout;

    // labels
    QLabel *color_l;
    QLabel *look_horiz_l;
    QLabel *light_pos_l;
    QLabel *light_x_l;
    QLabel *light_y_l;
    QLabel *light_z_l;
    QLabel *light_intense_l;
    QLabel *zoom_l;
    QLabel *texture_l;
    QLabel *look_l;
    QLabel *speed_l;
    QLabel *rotate_byhand_l;

    // buttons
    QPushButton *red_b;
    QPushButton *yellow_b;
    QPushButton *green_b;
    QPushButton *blue_b;
    QPushButton *white_b;
    QPushButton *earth;
    QPushButton *marc;
    QPushButton *wood;
    QPushButton *upper;
    QPushButton *lower;
    QPushButton *left;
    QPushButton *right;
    QPushButton *stop_rotate;
    QPushButton *inverse_rotate;
    QPushButton *smaller;
    QPushButton *bigger;
    QPushButton *look_back_side;

    // beneath that, the main widget
    SkyWheelWidget *cubeWidget;

    // and a slider for the number of vertices
    QSlider *angleSlider;
    QSlider *speedSlider;
    QSlider *xLookSlider;
    QSlider *yLookSlider;
    QSlider *xLightSlider;
    QSlider *yLightSlider;
    QSlider *zLightSlider;
    QSlider *sizeSlider;
    QSlider *yMoveSlider;
    QSlider *lightIntensifySlider;

    QTimer* ptimer; // timer for animation

    void ResetInterface(); // resets all the interface elements
    void setButtonStyle(QPushButton *button); // set buttons style
    void setLabelStyle(QLabel *label); // set labels style
    void setLabels(); // set labels properties
    void setButtons(); // set buttons properties
    void addElements(); // add widgets and layouts
    void connection(); // connect signals and slots
    void creating(); // new objects

    signals:
    void sliderSignal(int *i); // signal to obtain the value to be set

    public slots:
    void setSizeSliderValue(); // update value of sizeSlider
};

#endif
