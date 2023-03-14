#include <windows.h>

#include <iostream>
#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QBoxLayout>
#include <QTimer>
#include "SkyWheelWindow.h"

// constructor
SkyWheelWindow::SkyWheelWindow(QWidget *parent)
    : QWidget(parent){

    creating(); // new

    this->setStyleSheet("background-color: #dfeeff"); // background color

    ptimer->start(0); // timer starts at 0

    setLabels(); // set properties of labels
    setButtons(); // set properties of buttons
    connection(); // connect signals and slots
    addElements(); // add widgets or layouts
}

// creat elements
void SkyWheelWindow::creating(){

    // create the window layout
    windowLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    // create the view widget
    cubeWidget = new SkyWheelWidget(this);

    // create buttons
    red_b = new QPushButton(this);
    yellow_b = new QPushButton(this);
    blue_b = new QPushButton(this);
    green_b = new QPushButton(this);
    white_b = new QPushButton(this);
    earth = new QPushButton(this);
    marc = new QPushButton(this);
    wood = new QPushButton(this);
    upper = new QPushButton(this);
    lower = new QPushButton(this);
    left = new QPushButton(this);
    right = new QPushButton(this);
    stop_rotate = new QPushButton(this);
    inverse_rotate = new QPushButton(this);
    smaller = new QPushButton(this);
    bigger = new QPushButton(this);
    look_back_side = new QPushButton(this);

    // create sliders
    angleSlider = new QSlider(Qt::Horizontal);
    speedSlider = new QSlider(Qt::Horizontal);
    xLookSlider = new QSlider(Qt::Horizontal);
    yLookSlider = new QSlider(Qt::Vertical);
    xLightSlider = new QSlider(Qt::Horizontal);
    yLightSlider = new QSlider(Qt::Horizontal);
    zLightSlider = new QSlider(Qt::Horizontal);
    sizeSlider = new QSlider(Qt::Horizontal);
    yMoveSlider = new QSlider(Qt::Vertical);
    lightIntensifySlider = new QSlider(Qt::Horizontal);

    // create labels
    color_l = new QLabel(this);
    look_horiz_l = new QLabel(this);
    light_pos_l = new QLabel(this);
    light_x_l = new QLabel(this);
    light_y_l = new QLabel(this);
    light_z_l = new QLabel(this);
    light_intense_l = new QLabel(this);
    zoom_l = new QLabel(this);
    texture_l = new QLabel(this);
    look_l = new QLabel(this);
    speed_l = new QLabel(this);
    rotate_byhand_l = new QLabel(this);

    // create a timer
    ptimer = new QTimer(this);

    // create layouts
    vertiSliderLayout =  new QHBoxLayout(this);
    colorLayout = new QHBoxLayout(this);
    textureLayout = new QHBoxLayout(this);
    perspectiveLayout = new QHBoxLayout(this);
    speedLayout = new QHBoxLayout(this);
    lightLayout = new QHBoxLayout(this);
    distanceLayout = new QHBoxLayout(this);
    horiLookLayout = new QHBoxLayout(this);
    intensityLayout = new QHBoxLayout(this);
    byHandLayout = new QHBoxLayout(this);
}

// set properties of labels
void SkyWheelWindow::setLabels(){

    color_l->setText("Color:"); // set text of the label
    setLabelStyle(color_l); // set style of the label
    color_l->setMinimumSize(72, color_l->height()); // minimum size of the label
    color_l->setMaximumSize(72, color_l->height()); // maximum size of the label

    look_horiz_l->setText("Look At(H):");
    setLabelStyle(look_horiz_l);
    look_horiz_l->setMinimumSize(120, look_horiz_l->height());
    look_horiz_l->setMaximumSize(120, look_horiz_l->height());

    light_pos_l->setText("Light Position:");
    setLabelStyle(light_pos_l);
    light_pos_l->setMinimumSize(150, light_pos_l->height());
    light_pos_l->setMaximumSize(150, light_pos_l->height());

    light_x_l->setText("X:");
    setLabelStyle(light_x_l);
    light_x_l->setMinimumSize(19, light_x_l->height());
    light_x_l->setMaximumSize(19, light_x_l->height());

    light_y_l->setText("Y:");
    setLabelStyle(light_y_l);
    light_y_l->setMinimumSize(18, light_y_l->height());
    light_y_l->setMaximumSize(18, light_y_l->height());

    light_z_l->setText("Z:");
    setLabelStyle(light_z_l);
    light_z_l->setMinimumSize(18, light_z_l->height());
    light_z_l->setMaximumSize(18, light_z_l->height());

    light_intense_l->setText("Light Intensity:");
    setLabelStyle(light_intense_l);
    light_intense_l->setMinimumSize(150, light_intense_l->height());
    light_intense_l->setMaximumSize(150, light_intense_l->height());

    zoom_l->setText("Zoom:");
    setLabelStyle(zoom_l);
    zoom_l->setMinimumSize(62, zoom_l->height());
    zoom_l->setMaximumSize(62, zoom_l->height());

    texture_l->setText("Texture:");
    setLabelStyle(texture_l);
    texture_l->setMinimumSize(78, texture_l->height());
    texture_l->setMaximumSize(78, texture_l->height());

    look_l->setText("Look At:");
    setLabelStyle(look_l);
    look_l->setMinimumSize(80, look_l->height());
    look_l->setMaximumSize(80, look_l->height());

    speed_l->setText("Speed:");
    setLabelStyle(speed_l);
    speed_l->setMinimumSize(70, speed_l->height());
    speed_l->setMaximumSize(70, speed_l->height());

    rotate_byhand_l->setText("Rotate By Hand:");
    setLabelStyle(rotate_byhand_l);
    rotate_byhand_l->setMinimumSize(166, rotate_byhand_l->height());
    rotate_byhand_l->setMaximumSize(166, rotate_byhand_l->height());
}

// set properties of buttons
void SkyWheelWindow::setButtons(){

    // set text of buttons
    red_b->setText("Red");
    yellow_b->setText("Yellow");
    blue_b->setText("Blue");
    green_b->setText("Green");
    white_b->setText("White");
    earth->setText("Earth");
    marc->setText("Marc");
    wood->setText("Wood");
    upper->setText("Up");
    lower->setText("Down");
    left->setText("Left");
    right->setText("Right");
    stop_rotate->setText("Stop");
    inverse_rotate->setText("Inverse");
    smaller->setText("-");
    bigger->setText("+");
    look_back_side->setText("Back Side");

    // set style of buttons
    setButtonStyle(red_b);
    setButtonStyle(yellow_b);
    setButtonStyle(blue_b);
    setButtonStyle(green_b);
    setButtonStyle(white_b);
    setButtonStyle(earth);
    setButtonStyle(marc);
    setButtonStyle(wood);
    setButtonStyle(upper);
    setButtonStyle(lower);
    setButtonStyle(left);
    setButtonStyle(right);
    setButtonStyle(stop_rotate);
    setButtonStyle(inverse_rotate);
    setButtonStyle(smaller);
    setButtonStyle(bigger);
    setButtonStyle(look_back_side);
}

// widgets and layout adding
void SkyWheelWindow::addElements(){

    // add widgets
    horiLookLayout->addWidget(look_horiz_l);
    horiLookLayout->addWidget(xLookSlider);
    horiLookLayout->addWidget(look_back_side);
    vertiSliderLayout->addWidget(cubeWidget);
    vertiSliderLayout->addWidget(yLookSlider);
    vertiSliderLayout->addWidget(yMoveSlider);
    colorLayout->addWidget(color_l);
    colorLayout->addWidget(red_b);
    colorLayout->addWidget(yellow_b);
    colorLayout->addWidget(blue_b);
    colorLayout->addWidget(green_b);
    colorLayout->addWidget(white_b);
    textureLayout->addWidget(texture_l);
    textureLayout->addWidget(earth);
    textureLayout->addWidget(marc);
    textureLayout->addWidget(wood);
    perspectiveLayout->addWidget(look_l);
    perspectiveLayout->addWidget(upper);
    perspectiveLayout->addWidget(lower);
    perspectiveLayout->addWidget(left);
    perspectiveLayout->addWidget(right);
    speedLayout->addWidget(speed_l);
    speedLayout->addWidget(speedSlider);
    speedLayout->addWidget(inverse_rotate);
    speedLayout->addWidget(stop_rotate);
    lightLayout->addWidget(light_pos_l);
    lightLayout->addWidget(light_x_l);
    lightLayout->addWidget(xLightSlider);
    lightLayout->addWidget(light_y_l);
    lightLayout->addWidget(yLightSlider);
    lightLayout->addWidget(light_z_l);
    lightLayout->addWidget(zLightSlider);
    distanceLayout->addWidget(zoom_l);
    distanceLayout->addWidget(smaller);
    distanceLayout->addWidget(sizeSlider);
    distanceLayout->addWidget(bigger);
    intensityLayout->addWidget(light_intense_l);
    intensityLayout->addWidget(lightIntensifySlider);
    byHandLayout->addWidget(rotate_byhand_l);
    byHandLayout->addWidget(angleSlider);

    // add layouts
    windowLayout->addLayout(vertiSliderLayout);
    windowLayout->addLayout(horiLookLayout);
    windowLayout->addLayout(distanceLayout);
    windowLayout->addLayout(lightLayout);
    windowLayout->addLayout(intensityLayout);
    windowLayout->addLayout(colorLayout);
    windowLayout->addLayout(textureLayout);
    windowLayout->addLayout(perspectiveLayout);
    windowLayout->addLayout(speedLayout);
    windowLayout->addLayout(byHandLayout);
}

// connect signals and slots
void SkyWheelWindow::connection(){

    // connect signals and slots of buttons
    connect(red_b, SIGNAL(released()), cubeWidget, SLOT(toRed()));
    connect(yellow_b, SIGNAL(released()), cubeWidget, SLOT(toYellow()));
    connect(blue_b, SIGNAL(released()), cubeWidget, SLOT(toBlue()));
    connect(green_b, SIGNAL(released()), cubeWidget, SLOT(toGreen()));
    connect(white_b, SIGNAL(released()), cubeWidget, SLOT(toWhite()));
    connect(earth, SIGNAL(released()), cubeWidget, SLOT(toEarth()));
    connect(marc, SIGNAL(released()), cubeWidget, SLOT(toMarc()));
    connect(wood, SIGNAL(released()), cubeWidget, SLOT(toWood()));
    connect(ptimer, SIGNAL(timeout()),  cubeWidget, SLOT(updateAngleWithTime()));
    connect(upper, SIGNAL(released()), cubeWidget, SLOT(moveUp()));
    connect(lower, SIGNAL(released()), cubeWidget, SLOT(moveDown()));
    connect(left, SIGNAL(released()), cubeWidget, SLOT(moveLeft()));
    connect(right, SIGNAL(released()), cubeWidget, SLOT(moveRight()));
    connect(stop_rotate, SIGNAL(pressed()), cubeWidget, SLOT(stopRotating()));
    connect(stop_rotate, SIGNAL(released()), cubeWidget, SLOT(continueRotating()));
    connect(inverse_rotate, SIGNAL(released()), cubeWidget, SLOT(inverseRotating()));
    connect(smaller, SIGNAL(released()), cubeWidget, SLOT(smaller()));
    connect(bigger, SIGNAL(released()), cubeWidget, SLOT(bigger()));
    connect(bigger, SIGNAL(released()), this, SLOT(setSizeSliderValue()));
    connect(smaller, SIGNAL(released()), this, SLOT(setSizeSliderValue()));
    connect(look_back_side, SIGNAL(released()), cubeWidget, SLOT(lookBackSide()));

    // connect signals and slots of sliders
    connect(angleSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateAngle(int)));
    connect(speedSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateSpeed(int)));
    connect(xLookSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateXLook(int)));
    connect(yLookSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateYLook(int)));
    connect(xLightSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateXLightPos(int)));
    connect(yLightSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateYLightPos(int)));
    connect(zLightSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateZLightPos(int)));
    connect(lightIntensifySlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updatelightIntensify(int)));
    connect(sizeSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateSize(int)));
    connect(this, SIGNAL(sliderSignal(int *)), cubeWidget, SLOT (returnSize(int *)));
    connect(yMoveSlider, SIGNAL(valueChanged(int)), cubeWidget, SLOT(updateYMove(int)));
}

// set style of labels
void SkyWheelWindow::setLabelStyle(QLabel *label){

    QFont font ("Arial", 14);
    label->setStyleSheet("color:#73a2d9;");
    label->setFont(font);
    label->setScaledContents(true);
}

// set style of buttons
void SkyWheelWindow::setButtonStyle(QPushButton *button){

    button->setStyleSheet(
            // Normal state style
            "QPushButton{"
            "background-color:#a7caf2;"
            "border-style:outset;"
            "border-width:4px;"
            "border-radius:10px;"
            "border-color:#73a2d9;"
            "font: bold;"
            "font-family:'Arial';"
            "font-size: 24px;"
            "color:#ffffff;"
            "padding:5px;"
            "}"
            // Mouse press style
            "QPushButton:pressed{"
            "background-color:#9bc3f0;"
            "border-color: #73a2d9;"
            "border-style:inset;"
            "color:#ffffff;"
            "}"
            // Mouse hover style
            "QPushButton:hover{"
            "background-color:#9bc3f0;"
            "border-color:#73a2d9;"
            "color:#ffffff;"
            "}");
}

// destructor
SkyWheelWindow::~SkyWheelWindow(){

    // delete the view
    delete cubeWidget;

    // delete buttons
    delete red_b;
    delete yellow_b;
    delete blue_b;
    delete green_b;
    delete white_b;
    delete earth;
    delete marc;
    delete wood;
    delete upper;
    delete lower;
    delete left;
    delete right;
    delete stop_rotate;
    delete inverse_rotate;
    delete smaller;
    delete bigger;
    delete look_back_side;

    // delete sliders
    delete angleSlider;
    delete speedSlider;
    delete xLookSlider;
    delete yLookSlider;
    delete xLightSlider;
    delete yLightSlider;
    delete zLightSlider;
    delete sizeSlider;
    delete yMoveSlider;
    delete lightIntensifySlider;

    // delete labels
    delete color_l;
    delete look_horiz_l;
    delete light_pos_l;
    delete light_x_l;
    delete light_y_l;
    delete light_z_l;
    delete light_intense_l;
    delete zoom_l;
    delete texture_l;
    delete look_l;
    delete speed_l;
    delete rotate_byhand_l;

    // delete a timer
    delete ptimer;

    // delete layouts
    delete vertiSliderLayout;
    delete colorLayout;
    delete textureLayout;
    delete perspectiveLayout;
    delete speedLayout;
    delete lightLayout;
    delete distanceLayout;
    delete horiLookLayout;
    delete intensityLayout;
    delete byHandLayout;

    // delete the window layout
    delete windowLayout;
}

// resets all the interface elements
void SkyWheelWindow::ResetInterface(){

    angleSlider->setMinimum(0); // minimum value of the slider
    angleSlider->setMaximum(360); // maximum value of the slider
    angleSlider->setValue(0); // current value of the slider

    speedSlider->setMinimum(0);
    speedSlider->setMaximum(50);
    speedSlider->setValue(9);

    xLookSlider->setMinimum(-30);
    xLookSlider->setMaximum(10);
    xLookSlider->setValue(0);

    yLookSlider->setMinimum(-90);
    yLookSlider->setMaximum(30);
    yLookSlider->setValue(0);

    xLightSlider->setMinimum(-55);
    xLightSlider->setMaximum(55);
    xLightSlider->setValue(50);

    yLightSlider->setMinimum(-55);
    yLightSlider->setMaximum(55);
    yLightSlider->setValue(50);

    zLightSlider->setMinimum(-55);
    zLightSlider->setMaximum(55);
    zLightSlider->setValue(0);

    sizeSlider->setMinimum(50);
    sizeSlider->setMaximum(90);
    sizeSlider->setValue(50);

    yMoveSlider->setMinimum(-30);
    yMoveSlider->setMaximum(3);
    yMoveSlider->setValue(0);

    lightIntensifySlider->setMinimum(10);
    lightIntensifySlider->setMaximum(20);
    lightIntensifySlider->setValue(20);

    // now force refresh
    cubeWidget->update();
    update();
}

// slot to update value of the slider sizeSlider. It is invoked when button "bigger" or "smaller" is clicked.
void SkyWheelWindow::setSizeSliderValue(){

    int i = 1;
    emit sliderSignal(&i); // emit a signal to obtain the value which should be set to
    sizeSlider->setValue(i); // set the value
}
