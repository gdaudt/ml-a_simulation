#include <GL/glut.h>
#include <FreeImage.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>
#include <unistd.h>

#include "GlutClass.h"

/////////////////////////////////////////////
///// CONSTRUCTOR & CLASS INSTANTIATION /////
/////////////////////////////////////////////

GlutClass::GlutClass(){
}

GlutClass* GlutClass::instance = 0; 

GlutClass* GlutClass::getInstance ()
{
    if (instance == 0){
        instance = new GlutClass;
    }
    return instance;
}

//////////////////////////
///// PUBLIC METHODS /////
//////////////////////////

void GlutClass::initialize()
{
    halfWindowSize_ = 0;
    x_aux_ = 0;
    y_aux_ = 0;
    glutWindowSize_ = 700;

    // Wait for the Simulation's initialization
    while(simulation_->initializationCompleted() == false){
        usleep(100000);
    }

    grid_ = simulation_->grid;
    maxHalfWindowSize_ = ((std::max(simulation_->grid->getMapWidth(), simulation_->grid->getMapHeight())/2)/10)*10;

    numViewModes_=3;
    viewMode_=0;

	int argc=0;char** argv=0;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (glutWindowSize_,glutWindowSize_);

    id_ = glutCreateWindow("Janela");
    frame_ = 0;

    timer.startCounting();

    glClearColor (1.0, 1.0, 1.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT);

    glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);     
}

void GlutClass::process()
{
    glutMainLoop();
}

void GlutClass::terminate()
{
    simulation_->finish();
}

void GlutClass::setSimulation(Simulation *s)
{
    simulation_=s;
}

///////////////////////////
///// PRIVATE METHODS /////
///////////////////////////

void GlutClass::render()
{
    if(simulation_->isRunning() == false){
        exit(0);
    }

    // Desenha MCL
    int h = grid_->getMapHeight();
    int w = grid_->getMapWidth();

    // Atualiza a região da janela
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if(w > h){
        glOrtho (0 - x_aux_ - halfWindowSize_,
                    w - x_aux_ + halfWindowSize_,
                    h+(w-h)/2 - y_aux_ + halfWindowSize_,
                    -(w-h)/2 - y_aux_ - halfWindowSize_,
                    -1, 50);
    }else{
        glOrtho (-(h-w)/2 - x_aux_ - halfWindowSize_,
                    w+(h-w)/2 - x_aux_ + halfWindowSize_,
                    h - y_aux_ + halfWindowSize_,
                    0 - y_aux_ - halfWindowSize_,
                    -1, 50);
    }
    glMatrixMode (GL_MODELVIEW);

    glClearColor(0.8, 0.8, 0.8, 1.0);
    glClear (GL_COLOR_BUFFER_BIT);

    grid_->draw(viewMode_);

    // if(viewMode_<2)
        simulation_->drawAgents();

    glutSwapBuffers();
    glutPostRedisplay();
}

void GlutClass::screenshot()
{

    std::stringstream ss;
    std::string imgName;
    ss << "Imgs/frame-" << std::setfill('0') << std::setw(6) << frame_ << ".png";
    ss >> imgName;

    int width = glutWindowSize_;
    int height = glutWindowSize_;

    // Make the BYTE array, factor of 3 because it's RBG.
    BYTE* pixels = new BYTE[ 3 * width * height];

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    BYTE aux;
    for(int p=0;p<3*width*height;p=p+3){
        aux=pixels[p+2];
        pixels[p+2]=pixels[p];
        pixels[p]=aux;
    }

    // Convert to FreeImage format & save to file
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0xFF0000, 0x0000FF, 0xFF0000, false);
    FreeImage_Save(FIF_PNG, image, imgName.c_str(), 0);

    // Free resources
    FreeImage_Unload(image);
    delete [] pixels;

    frame_++;
}

/////////////////////////////////////////////////////
///// STATIC FUNCTIONS PASSED AS GLUT CALLBACKS /////
/////////////////////////////////////////////////////

void GlutClass::display()
{
    instance->render();
}

void GlutClass::reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (-100,100,-100,100,0, 50);
    glMatrixMode (GL_MODELVIEW);

    glClearColor(0.8, 0.8, 0.8, 0);
    glClear (GL_COLOR_BUFFER_BIT);
}

void GlutClass::keyboard(unsigned char key, int x, int y)
{
    // key: the value of the pressed key

    switch(key) {
        case 27:
            instance->terminate();
            break;
        case 'v': //view mode
            instance->viewMode_++;
            if(instance->viewMode_ == instance->numViewModes_)
                instance->viewMode_ = 0;
            break;
        case 'b': //view mode
            instance->viewMode_--;
            if(instance->viewMode_ == -1)
                instance->viewMode_ = instance->numViewModes_-1;
            break;
        case 'w':
            instance->y_aux_ += 10;
            std::cout << "x_aux: " << instance->x_aux_ << " y_aux: " << instance->y_aux_ << " halfWindowSize:" << instance->halfWindowSize_ << std::endl;
            break;
        case 'd':
            instance->x_aux_ += 10;
            std::cout << "x_aux: " << instance->x_aux_ << " y_aux: " << instance->y_aux_ << " halfWindowSize:" << instance->halfWindowSize_ << std::endl;
            break;
        case 'a':
            instance->x_aux_ -= 10;
            std::cout << "x_aux: " << instance->x_aux_ << " y_aux: " << instance->y_aux_ << " halfWindowSize:" << instance->halfWindowSize_ << std::endl;
            break;
        case 's':
            instance->y_aux_ -= 10;
            std::cout << "x_aux: " << instance->x_aux_ << " y_aux: " << instance->y_aux_ << " halfWindowSize:" << instance->halfWindowSize_ << std::endl;
            break;
        case 'm':
            instance->screenshot();
            break;
        case '-':
            instance->halfWindowSize_ += 10;
            if(instance->halfWindowSize_ > instance->maxHalfWindowSize_)
                instance->halfWindowSize_ = instance->maxHalfWindowSize_;
            break;
        case '+': 
        case '=':
            instance->halfWindowSize_ -= 10;
            if(instance->halfWindowSize_ < -instance->maxHalfWindowSize_)
                instance->halfWindowSize_ = -instance->maxHalfWindowSize_;
            break;
        case ' ':
            instance->simulation_->stepByStep = !instance->simulation_->stepByStep;
            break;
        case 'n':
            instance->simulation_->runNextInStepByStepMode = true;
            break;
        default:
            break;
    }
}

void GlutClass::specialKeys(int key, int x, int y)
{
    // key: the value of the pressed key

    switch(key) {
        case GLUT_KEY_UP:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_DOWN:
            
            break;
        default:
            break;
    }

}

