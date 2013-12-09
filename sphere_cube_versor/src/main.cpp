//Get mouse position as an "unprojected" point in 3D space

#include <sys/time.h>

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_ncube.h"

using namespace vsr;
using namespace std;

static long timeGetTime( void )
{
    struct timeval now; //, res;

    gettimeofday(&now, 0);

    return (long)((now.tv_sec*1000) + (now.tv_usec/1000));
}

/*template< class B >
auto nrot( const B& b) -> decltype( B() + 1 ) {         
    VT c = sqrt(-(b.wt()));
    VT sc = -sin(c);

    if (c != 0) {
        sc /= c;
    }
   
    return (b*sc) + cos(c);
} */

struct MyApp : App {    

    Pnt mouse;
    Lin ray;
    long to;
    float fTime;


    MyApp(Window * win ) : App(win) {
        to = timeGetTime();
        scene.camera.pos(0, 0, 10); 
    }

    void getMouse(){
        auto tv = interface.vd().ray; 
        Vec z (tv[0], tv[1], tv[2] );
        auto tm = interface.mouse.projectMid;
        ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
        mouse = Round::point( ray,  Ori(1) );  
    }

    void drawCube(Vec vecPosition, float fRadius) {
        vector<Vec> proj;
        static const int DIM = 4;

        NCube<DIM> cube;

        auto& res = cube.roots; 

        for (auto& i : res) {
            // For rotation:
            //EGAMV<DIM, MV<24,12> > b(1,.01);
            //i = i.sp(nrot(b));
            
            auto tmp = Proj<DIM>::Ortho<3>(i);
            proj.push_back(tmp);
        }

        
        for (auto i : cube.edges) {
            gfx::Glyph::Line(proj[i.a], proj[i.b]);
        }
    }

    virtual void onDraw() {
        fTime = 0.001f * (timeGetTime() - to);
        getMouse();

        Point a = Ro::point(0, 0, 0);
        Draw(a);

        auto foo = Vec(-0.5, 0.5, -0.5);
        Biv rotX(fTime, 0, 0);

        auto sbtl = Ro::sphere(foo.rot(rotX), 0.2);
        Draw(sbtl);
        auto sftl = Ro::sphere(Vec(-0.5, 0.5, 0.5), 0.2);
        Draw(sftl);
        auto sbtr = Ro::sphere(Vec(0.5, 0.5, -0.5), 0.2);
        Draw(sbtr);
        auto sftr = Ro::sphere(Vec(0.5, 0.5, 0.5), 0.2);
        Draw(sftr);

        auto sbbl = Ro::sphere(Vec(-0.5, -0.5, -0.5), 0.2);
        Draw(sbbl);
        auto sfbl = Ro::sphere(Vec(-0.5, -0.5, 0.5), 0.2);
        Draw(sfbl);
        auto sbbr = Ro::sphere(Vec(0.5, -0.5, -0.5), 0.2);
        Draw(sbbr);
        auto sfbr = Ro::sphere(Vec(0.5, -0.5, 0.5), 0.2);
        Draw(sfbr);

        drawCube(Vec(0, 0, 0), 2.0f);
    }
};


MyApp * app;

int main() {
    GLV glv(0, 0);
    Window * win = new Window(500, 500, "Funkeeh stuff!", &glv);
    app = new MyApp(win); 
    glv << *app;

    Application::run();

    return 0;
}
