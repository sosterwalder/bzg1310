//Get mouse position as an "unprojected" point in 3D space

#include <sys/time.h>

#include "vsr_cga3D.h"   
#include "vsr_GLVimpl.h"
#include "vsr_ncube.h"

using namespace vsr;
using namespace std;

#pragma GCC diagnostic ignored "-Wswitch"

static long timeGetTime( void )
{
    struct timeval now; //, res;

    gettimeofday(&now, 0);

    return (long)((now.tv_sec*1000) + (now.tv_usec/1000));
}

struct MyApp : App {    
    static const int DIM = 4;

    Pnt mouse;
    Lin ray;
    long to;
    float fTime;
    NCube<DIM> cube;
    vector<Vec> vecCube;
    Biv bivRotation;
    bool bDoAnimate;
    bool bDrawSpheresAsWireframe;
    bool bDrawCubeAsLines;
    enum CubeRenderModes { NORMAL, LINES };
    enum CubeRenderModes cubeRenderMode;
    double dFps;
    Window* pWindow;

    MyApp(Window * win ) : App(win) {
        to = timeGetTime();
        scene.camera.pos(0, 0, 4); 
        setupCube();
        cubeRenderMode = NORMAL;
        bDoAnimate = true;
        bDrawSpheresAsWireframe = false;
        bDrawCubeAsLines = false;
        pWindow = win;
    }

    ~MyApp() {
        if (pWindow) {
            delete pWindow;
        }
    }

    virtual void initGui() {
        gui(bDoAnimate, "Toggle animation");
        gui(bDrawSpheresAsWireframe, "Draw spheres wireframed");
        gui(bDrawCubeAsLines, "Draw cube using lines within GA");
    }

    void setupCube() {
        for (auto& i : cube.roots) {
            auto tmp = Proj<DIM>::Ortho<3>(i);
            vecCube.push_back(tmp);
        }
    }

    void getMouse(){
        auto tv = interface.vd().ray; 
        Vec z (tv[0], tv[1], tv[2] );
        auto tm = interface.mouse.projectMid;
        ray = Round::point( tm[0], tm[1], tm[2] ) ^ z ^ Inf(1); 
        mouse = Round::point( ray,  Ori(1) );  
    }

    void drawCube(Vec vecPosition, float fRadius) {
        for (auto edge : cube.edges) {
            gfx::Glyph::Line(
                vecCube[edge.a].rot(bivRotation),
                vecCube[edge.b].rot(bivRotation)
            );
        }
    }

    void drawGACube(Vec vecPosition, float fRadius) {
        Point bottomLeftFront = Ro::point(-fRadius, -fRadius, fRadius);
        Point bottomRightFront = Ro::point(fRadius, -fRadius, fRadius);

        Point topLeftFront = Ro::point(-fRadius, fRadius, fRadius);
        Point topRightFront = Ro::point(fRadius, fRadius, fRadius);

        Point bottomLeftBack = Ro::point(-fRadius, -fRadius, -fRadius);
        Point bottomRightBack = Ro::point(fRadius, -fRadius, -fRadius);

        Point topLeftBack = Ro::point(-fRadius, fRadius, -fRadius);
        Point topRightBack = Ro::point(fRadius, fRadius, -fRadius);

        Line lineTopBack = Fl::line(topLeftBack.rot(bivRotation), topRightBack.rot(bivRotation));
        Line lineLeftBack = Fl::line(topLeftBack.rot(bivRotation), bottomLeftBack.rot(bivRotation));
        Line lineRightBack = Fl::line(topRightBack.rot(bivRotation), bottomRightBack.rot(bivRotation));
        Line lineBottomBack = Fl::line(bottomLeftBack.rot(bivRotation), bottomRightBack.rot(bivRotation));

        Line lineTopLeft = Fl::line(topLeftBack.rot(bivRotation), topLeftFront.rot(bivRotation));
        Line lineTopRight = Fl::line(topRightBack.rot(bivRotation), topRightFront.rot(bivRotation));
        Line lineBottomLeft = Fl::line(bottomLeftBack.rot(bivRotation), bottomLeftFront.rot(bivRotation));
        Line lineBottomRight = Fl::line(bottomRightBack.rot(bivRotation), bottomRightFront.rot(bivRotation));

        Line lineTopFront = Fl::line(topLeftFront.rot(bivRotation), topRightFront.rot(bivRotation));
        Line lineLeftFront = Fl::line(topLeftFront.rot(bivRotation), bottomLeftFront.rot(bivRotation));
        Line lineRightFront = Fl::line(topRightFront.rot(bivRotation), bottomRightFront.rot(bivRotation));
        Line lineBottomFront = Fl::line(bottomLeftFront.rot(bivRotation), bottomRightFront.rot(bivRotation));

        Draw(lineTopBack);
        Draw(lineLeftBack);
        Draw(lineRightBack);
        Draw(lineBottomBack);

        Draw(lineTopLeft);
        Draw(lineTopRight);
        Draw(lineBottomLeft);
        Draw(lineBottomRight);

        Draw(lineTopFront);
        Draw(lineLeftFront);
        Draw(lineRightFront);
        Draw(lineBottomFront);
    }

    virtual void onDraw() {
        std::stringstream strTitle;
        strTitle << "Funkeeh stuff - FPS: ";
        strTitle << pWindow->fpsActual();
        pWindow->title(strTitle.str());

        float fSphereRadius = 0.2;

        if (bDrawSpheresAsWireframe) {
            fSphereRadius *= (-1);
        }

        if (bDoAnimate) {
            fTime = (timeGetTime() - to);
        }
        else {
            fTime = 0.0f;
        }

        getMouse();
        bivRotation = Biv(sinf(fTime * 0.001f), 0, cosf(fTime * 0.001f));

        
        // Individual rotation of the spheres...
        glRotatef(fTime * 0.1f, 1.0f, 0.0f, 1.0f);

        auto sbtl = Ro::sphere(Vec(-0.5, 0.5, -0.5).rot(bivRotation), fSphereRadius);
        Draw(sbtl);
        auto sftl = Ro::sphere(Vec(-0.5, 0.5, 0.5).rot(bivRotation), fSphereRadius);
        Draw(sftl);
        auto sbtr = Ro::sphere(Vec(0.5, 0.5, -0.5).rot(bivRotation), fSphereRadius);
        Draw(sbtr);
        auto sftr = Ro::sphere(Vec(0.5, 0.5, 0.5).rot(bivRotation), fSphereRadius);
        Draw(sftr);

        auto sbbl = Ro::sphere(Vec(-0.5, -0.5, -0.5).rot(bivRotation), fSphereRadius);
        Draw(sbbl);
        auto sfbl = Ro::sphere(Vec(-0.5, -0.5, 0.5).rot(bivRotation), fSphereRadius);
        Draw(sfbl);
        auto sbbr = Ro::sphere(Vec(0.5, -0.5, -0.5).rot(bivRotation), fSphereRadius);
        Draw(sbbr);
        auto sfbr = Ro::sphere(Vec(0.5, -0.5, 0.5).rot(bivRotation), fSphereRadius);
        Draw(sfbr);

        if (bDrawCubeAsLines) {
            drawGACube(Vec(0, 0, 0), 0.5f);
        }
        else {
            drawCube(Vec(0, 0, 0), 0.5f);
        }
    }
};


MyApp * app;

int main() {
    GLV glv(0, 0);
    
    Window * win = new Window(1024, 768, "Funkeeh stuff!", &glv);
    
    app = new MyApp(win); 
    app->initGui();

    glv << *app;

    Application::run();

    return 0;
}
