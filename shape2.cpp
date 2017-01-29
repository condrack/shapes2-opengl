

/*
Taylor Condrack
Program 6
4230
04/21/16
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static int a, b, data,indv=0,width,height;
#define K 100
#define NV (K+1)*(K+1)
static float node[NV][3],node2[NV][3];
#define NT 2*100*100
static int ltri[NT][3];
static float trnormals[NT][1];
static int fscale=1;
static int xrot=0,yrot=0;
static int zoom=50;
static int e=10;
static bool toggle1=true,toggle2=true,toggle3=true;
static GLfloat mat_specular[] = {0.3 , 0.3 , 0.2, 1.0 };
static int x1=0,x2=1,y_1=0,y2=1,zmin=0,zmax=1;
void menu(int);
void popupmenu();
void biv();

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    biv();
}

//creates view volume and rectangle viewport
void reshape(int w, int h)
{
    width=w;
    height=h;
//creates a square viewport and keeps a square when resized
    if(w>h)
        glViewport ((w-h)/2, 0, (GLsizei) h, (GLsizei) h);
    else
        glViewport(0,(h-w)/2,(GLsizei) w, (GLsizei) w);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(zoom,(GLfloat)w/(GLfloat)h,1,20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-2.0);
    glRotatef(xrot,1.0,0.0,0.0);
    glRotatef(yrot,0.0,1.0,0.0);
    glTranslatef(-.5,-0.5,0.0);

}
float f(int x,int y)
{
    int z;
    z=0.5*exp(-.04*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2))) *cos(0.15*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2)));
    return z;

}
void norm(float node[])
{
    float l=0;

    for(int i=0;i<3;i++)
        l=l+(node[i]*node[i]);

    l=sqrt(l);

    for(int j=0;j<3;j++)
        node[j]=node[j]/l;
}

void biv()
{
    float y,x,z;
    float h = 1.0/K;
    for (int j=0; j<=K; j++) {
        y = j*h;
        for (int i=0; i<=K; i++) {
            x = i*h;
            z = 0.5*exp(-.04*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2))) *cos(0.15*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2)));
            node[indv][0] = x*fscale;
            node[indv][1] = y*fscale;
            node[indv][2] = z*fscale;
            indv++;
        }
    }
    int indt = 0;
    for (int j=1; j<=K; j++) {
        for (int i=1; i<=K; i++) {
            indv = j*(K+1) + i;
            ltri[indt][0] = indv-K-2;
            ltri[indt][1] = indv-K-1;
            ltri[indt][2] = indv;
            ltri[indt+1][0] = indv-K-2;
            ltri[indt+1][1] = indv;
            ltri[indt+1][2] = indv-1;
            indt += 2;
        }
    }

    for (indv=0; indv<=NV-1; indv++) {
        node2[indv][0] = 0;
        node2[indv][1] = 0;
        node2[indv][2] = 0;
    }

    int i1,i2,i3;
    float tn[3];
    for (indt=0; indt<=NT-1; indt++) {
        i1 = ltri[indt][0];
        i2 = ltri[indt][1];
        i3 = ltri[indt][2];
        tn[0] = (node[i2][1]-node[i1][1]) * (node[i3][2]-node[i1][2]) -
                (node[i2][2]-node[i1][2]) * (node[i3][1]-node[i1][1]);
        tn[1] = (node[i2][2]-node[i1][2]) * (node[i3][0]-node[i1][0]) -
                (node[i2][0]-node[i1][0]) * (node[i3][2]-node[i1][2]);
        tn[2] = (node[i2][0]-node[i1][0]) * (node[i3][1]-node[i1][1]) -
                (node[i2][1]-node[i1][1]) * (node[i3][0]-node[i1][0]);
        norm(tn);

        for(int i=0;i<3;i++)
            trnormals[i][0]=tn[i];

        for (int i=0; i<3; i++) {
            node2[i1][i] += tn[i];
            node2[i2][i] += tn[i];
            node2[i3][i] += tn[i];
        }
    }

    for (indv=0; indv<=NV-1; indv++)
        norm (node2[indv]);
}

void popupmenu()
{
    a= glutCreateMenu(menu);
    glutAddMenuEntry("rotate left 'l'",1);
    glutAddMenuEntry("rotate right'k'",2);
    glutAddMenuEntry("zoom '+'",3);
    glutAddMenuEntry("zoom '-'",4);
    glutAddMenuEntry("rotate up'x'",5);
    glutAddMenuEntry("rotate down'u'",6);
    glutAddMenuEntry("toggle display of bounding box'b'",7);
    glutAddMenuEntry("scale function by .5 'd'",8);
    glutAddMenuEntry("scale function by 2 'D'",9);
    glutAddMenuEntry("scale shininess by .5 'e'",10);
    glutAddMenuEntry("scale shininess by 2.0 'E'",11);
    glutAddMenuEntry("toggle wireframe/polygon-fill 'f'",12);
    glutAddMenuEntry("restore defaults 'r'",13);
    glutAddMenuEntry("toggle between flat and smooth shading 's'",14);
    glutAddMenuEntry("quit",15);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int in)
{
    if(in==0)
    {
        glutDestroyWindow(b);
        exit(0);
    }
    if(in==1)
    {
      yrot=(yrot+10)%360;
      glutPostRedisplay();
    }

    if(in==2)
    {
        yrot=(yrot-10)%360;
        glutPostRedisplay();
    }

    if(in==3)
    {
        zoom=zoom+1;
        glutPostRedisplay();
        reshape(width,height);
    }

    if(in==4)
    {
        zoom=zoom-1;
        glutPostRedisplay();
        reshape(width,height);
    }
    if(in==5)
    {
        xrot=(xrot-10)%360;
        glutPostRedisplay();
    }
    if(in==6)
    {
        xrot=(xrot+10)%360;
        glutPostRedisplay();
    }
    if(in==7)
    {
        if(toggle1==true)
            toggle1=false;
        else
            toggle1=true;
        glutPostRedisplay();
    }
    if(in==8)
    {
        fscale*=.5;
        glutPostRedisplay();
    }
    if(in==9)
    {
        fscale*=2.0;
        glutPostRedisplay();
    }
    if(in==10)
    {
        e=e*.5;
        glutPostRedisplay();
    }
    if(in==11)
    {
        e=e*2.0;
        glutPostRedisplay();
    }
    if(in==12)
    {
        if(toggle3==true)
            toggle3=false;
        else
            toggle3=true;
        glutPostRedisplay();
    }
    if(in==13)
    {
        e=10;
        toggle1=true;
        toggle2=true;
        toggle3=true;
        fscale=1;
        glutPostRedisplay();
    }
    if(in==14)
    {
        if(toggle2==true)
            toggle2=false;
        else
            toggle2=true;
        glutPostRedisplay();
    }
    if(in==15)
    {
        exit(0);
    }
    else
    {
        data=in;
    }
    reshape(width,height);
    glutPostRedisplay();
}
void keyboard(unsigned char key,int x,int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'l':
         yrot=(yrot+10)%360;
         glutPostRedisplay();
         break;
        case 'k':
         yrot=(yrot-10)%360;
         glutPostRedisplay();
         break;
        case '+':
         zoom=zoom+1;
         glutPostRedisplay();
         break;
        case '-':
         zoom=zoom-1;
         glutPostRedisplay();
         break;
        case 'u':
            xrot=(xrot+10)%360;
            glutPostRedisplay();
            break;
        case 'x':
            xrot=(xrot-10)%360;
            glutPostRedisplay();
            break;
        case 'b':
            if(toggle1==true)
                toggle1=false;
            else
                toggle1=true;
            glutPostRedisplay();
            break;
        case 'd':
            fscale*=.5;
            glutPostRedisplay();
            break;
        case 'D':
            fscale*=2.0;
            glutPostRedisplay();
            break;
        case 'e':
            e=e*.5;
            glutPostRedisplay();
            break;
        case 'E':
            e=e*2.0;
            glutPostRedisplay();
            break;
        case 'f':
            if(toggle3==true)
                toggle3=false;
            else
                toggle3=true;
            glutPostRedisplay();
            break;
        case 'r':
            e=10;
            toggle1=true;
            toggle2=true;
            toggle3=true;
            fscale=1.0;
            glutPostRedisplay();
            break;
        case 's':
            if(toggle2==true)
                toggle2=false;
            else
                toggle2=true;
            glutPostRedisplay();
            break;
      default:
         break;
    }
    reshape(width,height);
    glutPostRedisplay();
}
void display(void)
{
    popupmenu();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,e);

    if(toggle3==true)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glVertexPointer (3, GL_FLOAT, 0, node);
    glNormalPointer(GL_FLOAT, 0, node2);
    if(toggle2==false)
    {
        glShadeModel(GL_FLAT);
        glDrawElements(GL_TRIANGLES, NT*3, GL_UNSIGNED_INT, ltri);
    }
    else
    {
        glShadeModel(GL_SMOOTH);
        glDrawElements(GL_TRIANGLES, NT*3, GL_UNSIGNED_INT, ltri);
    }

    glPopMatrix();

    if(toggle1==true)
    {
        glTranslatef(0,0,.5);
        glColor3f (1.0, 1.0, 0.0);
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glBegin (GL_LINE_LOOP);
        glVertex3f (x1, y_1, -zmin);
        glVertex3f (x2, y_1, -zmin);
        glVertex3f (x2, y2, -zmin);
        glVertex3f (x1, y2, -zmin);
        glEnd ();

        glBegin (GL_LINE_LOOP);
        glVertex3f (x1, y_1, -zmax);
        glVertex3f (x2, y_1, -zmax);
        glVertex3f (x2, y2, -zmax);
        glVertex3f (x1, y2, -zmax);
        glEnd ();

        glBegin (GL_LINES);	/*  4 lines	*/
        glVertex3f (x1, y_1, -zmin);
        glVertex3f (x1, y_1, -zmax);
        glVertex3f (x1, y2, -zmin);
        glVertex3f (x1, y2, -zmax);
        glVertex3f (x2, y_1, -zmin);
        glVertex3f (x2, y_1, -zmax);
        glVertex3f (x2, y2, -zmin);
        glVertex3f (x2, y2, -zmax);
        glEnd ();
    }

    glutSwapBuffers();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    popupmenu();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
