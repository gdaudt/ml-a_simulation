#ifndef __GLUTCLASS_H__
#define __GLUTCLASS_H__

#include "Simulation.h"
#include "Utils.h"

class GlutClass
{
    public:
        static GlutClass* getInstance();

        void initialize();
        void process();
        void terminate();

        void setSimulation(Simulation* r);

    private:
        GlutClass ();
        static GlutClass* instance;

        Simulation* simulation_;
        Grid* grid_;
        Timer timer;

        int glutWindowSize_;
        int frame_;
        int halfWindowSize_, maxHalfWindowSize_;
        int x_aux_, y_aux_;
        int viewMode_, numViewModes_;

        int id_;

	    void render();

        static void display();
        static void reshape(int w, int h);
        static void keyboard(unsigned char key, int x, int y);
        static void specialKeys(int key, int x, int y);

        void screenshot();

};

#endif /* __GLUT_H__ */


