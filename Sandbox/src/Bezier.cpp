#include "Bezier.h"


Bezier::Bezier():m_CameraController(1280.0f / 720.0f)
{

}

void Bezier::OnAttach()
{
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glShadeModel(GL_FLAT);
}

void Bezier::OnDetach()
{

}

void Bezier::OnUpdate(Vortex::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    Vortex::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Vortex::RenderCommand::Clear();

    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINE_STRIP);

    for(int i = 0; i <= 30; i++)
    {
        glEvalCoord1f((GLfloat)i / 30.0f);
    }
    glEnd();

    glPointSize(10.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++)
    {
        glVertex3fv(&ctrlpoints[i][0]);
    }
    glEnd();

    glTranslatef(-0.1f, 0.1f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 60; i++)
    {
        glEvalCoord1f((GLfloat)i / 60.0f);
    }
    glEnd();

}

void Bezier::OnImGuiRender()
{

}

void Bezier::OnEvent(Vortex::Event& e)
{

}
