
/**************************************************************/
/****** Projeto de Computacao Grafica - 2012/2013        ******/
/******             -- Jogo do Galo --                   ******/
/****** Elaborado por:                                   ******/
/****** Alexandre Pinto                                  ******/
/****** Carlos Avim                                      ******/
/**************************************************************/

#include <GL/glut.h>
#include "glm.h"
#include "tictac.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "RgbImage.h"
#include "RgbImage.cpp"

#define COLOR1     0.651, 0.165, 0.165, 1.0   //reddish color
#define COLOR2     0.455, 0.608, 0.153, 1.0   //greenish color

GLuint  texture[2];
RgbImage imag;

int turn=1, end=0, game_status=0;                                   // auxiliar variables to the tic tac toe game
float rotationAngle = 0,rotationSky=0,degrees1, degrees2;           // rotation vars to the object dynamics
int display_X=0, display_board=0, display_O=0, display_grid=0;      // indexes of display lists
GLMmodel *x_model, *o_model, *board, *net;                                 // structures to the .obj models
GLint wScreen=800, hScreen=600;
GLint xMouse,yMouse;
int fog=0, night=0, light0=0, light1=0, light2=0, light3=0;

GLfloat  PI = 3.14159;
GLfloat  radialDist =50.0, theta=2.05,phi=1.05;             //spherical coordinates
GLfloat  initialPos[] ={-12.8,15.7,24.4};                  // initialPos: initial position of the observer finalPos: to where the obs is looking for
GLfloat  finalPos[] ={initialPos[0]-radialDist *cos(theta)*sin(phi),initialPos[1]-radialDist *cos(phi),initialPos[2]-radialDist *sin(theta)*sin(phi)};

GLfloat fogColor[] = {0.75, 0.75, 0.75, 1.0};

GLint grid[10][2] = {{0,0},                             //pieces coordinates
                    {-2,-2},{0,-2},{2,-2},
					{-2,0},{0,0},{2,0},
					{-2,2},{0,2},{2,2}};

GLint pieces[10];                                        // pieces
GLfloat scale[10];                                       //scale effects

int game[4][4];                                         // pieces played
GLfloat screen[10][4] = {{22,480,86,430},{87,480,152,430},{153,480,215,430},      // screen coordinates
                      {22,530,86,481},{87,530,152,481},{153,530,215,481},       
                      {22,580,86,531},{87,580,152,531},{153,580,215,531},
                      {315,530,550,455}};

GLfloat GlobalLight[4]={1.0,1.0,1.0,1.0};

GLfloat lightPos1[] = {0.0f, 5.0f, -3.0f, 1.0f};
GLfloat lightPos2[] = {-3.0f, 5.0f, 3.0f, 1.0f};
GLfloat lightPos3[] = {3.0f, 5.0f, 3.0f, 1.0f};
GLfloat lightPos4[] = {0.0f, -10.0f, -3.0f, 1.0f};

GLfloat lightDir1[] = {0.0f, 5.0f, 3.0f, 0.0f};
GLfloat lightDir2[] = {3.0f, 5.0f, -3.0f, 0.0f};
GLfloat lightDir3[] = {-3.0f, 5.0f, -3.0f, 0.0f};

GLfloat localAttCon =1.0;
GLfloat localAttLin =0.05;
GLfloat localAttQua =0.0;
GLfloat exponent   = 5.0;
GLfloat cutoff   = 45.0;

//turquoise material
GLfloat  turquoiseAmb []={ 0.1 ,0.18725 ,0.1745 };
GLfloat  turquoiseDif []={   0.396 ,0.74151 ,0.69102 };
GLfloat  turquoiseSpec []={ 0.297254 ,0.30829 ,0.306678 };
GLint   turquoiseCoef = 0.1 *128;

GLfloat  red[]={1.0,0.0,0.0,1.0};
GLfloat  green[]={0.0,1.0,0.0,1.0};
GLfloat  blue[]={0.0,0.0,1.0,1.0};
GLfloat  white[]={1.0,1.0,1.0,1.0};


void defineTextures()
{
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    imag.LoadBmpFile("sand.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,imag.GetNumCols(),imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,imag.ImageData());

    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    imag.LoadBmpFile("sky.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,imag.GetNumCols(),imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,imag.ImageData());
}

void initNevoeiro(void){
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 100.0);
    //glFogf (GL_FOG_DENSITY, 0.35);
}

void initLights(void){
        //Ambient
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalLight);

    //light 1
    glLightfv(GL_LIGHT0, GL_POSITION,      lightPos1 );
    glLightfv(GL_LIGHT0, GL_AMBIENT,       red );
    glLightfv(GL_LIGHT0, GL_DIFFUSE,       red );
    glLightfv(GL_LIGHT0, GL_SPECULAR,      red );
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,lightDir1 );
    glLightf (GL_LIGHT0, GL_SPOT_EXPONENT ,exponent);
    glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,   cutoff);

    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION,   localAttLin) ;
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,localAttQua) ;

    //light 2
    glLightfv(GL_LIGHT1, GL_POSITION,      lightPos2 );
    glLightfv(GL_LIGHT1, GL_AMBIENT,       green );
    glLightfv(GL_LIGHT1, GL_DIFFUSE,       green );
    glLightfv(GL_LIGHT1, GL_SPECULAR,      green );
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,lightDir2 );
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT ,exponent);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,   cutoff);

    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION,   localAttLin) ;
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION,localAttQua) ;

    //light 3
    glLightfv(GL_LIGHT2, GL_POSITION,      lightPos3 );
    glLightfv(GL_LIGHT2, GL_AMBIENT,       blue );
    glLightfv(GL_LIGHT2, GL_DIFFUSE,       blue );
    glLightfv(GL_LIGHT2, GL_SPECULAR,      blue );
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION,lightDir3 );
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT ,exponent);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,   cutoff);

    glLightf (GL_LIGHT2, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT2, GL_LINEAR_ATTENUATION,   localAttLin) ;
    glLightf (GL_LIGHT2, GL_QUADRATIC_ATTENUATION,localAttQua) ;

    glLightfv(GL_LIGHT4, GL_POSITION,      lightPos4 );
    glLightfv(GL_LIGHT4, GL_AMBIENT,       white);

    glLightf (GL_LIGHT3, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT3, GL_LINEAR_ATTENUATION,   localAttLin) ;
    glLightf (GL_LIGHT3, GL_QUADRATIC_ATTENUATION,localAttQua) ;
}


void initMaterials(int material)
{
    switch (material){
        case 1: //turquoise
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  turquoiseAmb  );
            glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  turquoiseDif );
            glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, turquoiseSpec);
            glMateriali (GL_FRONT_AND_BACK,GL_SHININESS,turquoiseCoef);
        break;
    }
}

// draw 'x'
void draw_X(float x, float y, float z, int graus,int index)
{
    if(scale[index]<=1)
        scale[index]+=0.1;

    glPushMatrix();
    glColor4f(COLOR1);
    glTranslatef(x,y,z);
    glScalef(scale[index],scale[index],scale[index]);
    glRotatef(degrees1, 0.0, 1.0, 0.0);
    glCallList(display_X);
    glPopMatrix();
}

//draws a small 'x' in the lower left corner
void draw_X_map(float x, float y, float z)
{
    glPushMatrix();
    glColor4f(COLOR1);
    glTranslatef(x,y,z);
    glLineWidth(7.0f);
    glBegin(GL_LINES);
        glVertex3f(-0.5f,0.1f, 0.5f);
        glVertex3f(0.5f,0.1f, -0.5f);
        glVertex3f(-0.5f,0.1f, -0.5f);
        glVertex3f(0.5f,0.1f, 0.5f);
    glEnd();
    glPopMatrix();
}

// draws a 'O'
void draw_O(float x, float y, float z, int graus,int index)
{
    if(scale[index]<=1)
        scale[index]+=0.1;

    glPushMatrix();
    glColor4f(COLOR2);
    glTranslatef(x,y,z);
    glScalef(scale[index],scale[index],scale[index]);
    glRotatef(degrees2, 0.0, 1.0, 0.0);
    glCallList(display_O);
    glPopMatrix();
}

//draws a small 'o' in the lower left corner
void draw_O_map(float x, float y, float z)
{
    glPushMatrix();
    glColor4f(COLOR2);
    glTranslatef(x,y,z);
    glRotatef(90,1.0,0.0,0.0);
    glutWireTorus(0.05f,0.4f, 30, 30);
    glPopMatrix();
}

//draws a board
void draw_board(float x, float y, float z, int graus, float alpha)
{
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    initLights();
    initMaterials(1);

    glPushMatrix();
    //glColor4f(1.0f, 1.0f, 1.0f, alpha);
    glTranslatef(x,y,z);
    glRotatef(graus, 1.0, 0.0, 0.0);
    glCallList(display_board);
    glPopMatrix();

    glEnable(GL_COLOR_MATERIAL);
}

// draws a grid
void draw_grid(float x, float y, float z, int graus)
{
    glPushMatrix();
    glColor4f(0.0f, 0.0f, 0.0f, 1.0);
    glTranslatef(x,y,z);
    glRotatef(graus, 1.0, 0.0, 0.0);
    glCallList(display_grid);
    glPopMatrix();
}

// print text to the screen
void print( float x, float z, char *st, int tipo)
{
    int l,i;

    l=strlen(st);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0);
    glRasterPos3f(x, -1, z);

    if(tipo ==1) {
        for( i=0; i < l; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
        }
    }

    else {
        for( i=0; i < l; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, st[i]);
        }
    }

    glPopMatrix();

}

void handleKeyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        // move forward
        case 'w':
                initialPos[0]+=(finalPos[0]-initialPos[0])/radialDist ;
                finalPos[0]+=(finalPos[0]-initialPos[0])/radialDist ;
                initialPos[1]+= (finalPos[1]-initialPos[1])/radialDist ;
                finalPos[1]+= (finalPos[1]-initialPos[1])/radialDist ;
                initialPos[2]+=(finalPos[2]-initialPos[2])/radialDist ;
                finalPos[2]+=(finalPos[2]-initialPos[2])/radialDist ;
            break;
        // move backwards
        case 's':
                initialPos[0]-=(finalPos[0]-initialPos[0])/radialDist ;
                finalPos[0]-=(finalPos[0]-initialPos[0])/radialDist ;
                initialPos[1]-= (finalPos[1]-initialPos[1])/radialDist ;
                finalPos[1]-= (finalPos[1]-initialPos[1])/radialDist ;
                initialPos[2]-=(finalPos[2]-initialPos[2])/radialDist ;
                finalPos[2]-=(finalPos[2]-initialPos[2])/radialDist ;
            break;
        // fog
        case 'f' :
            if(fog==0) {
                glEnable(GL_FOG);
                fog=1;
            }
            else {
                glDisable(GL_FOG);
                fog=0;
            }
            break;
        //--------------------------- Day/night
        case 'n':
        case 'N':
            night=!night;
            if (night)
            {
                GlobalLight[0]=0.1;   GlobalLight[1]=0.1;   GlobalLight[2]=0.1;
                fogColor[0]=0.1;    fogColor[1]=0.1;    fogColor[2]=0.1;
                glFogfv(GL_FOG_COLOR, fogColor);
            }
            else
            {
                GlobalLight[0]=1.0;   GlobalLight[1]=1.0;   GlobalLight[2]=1.0;
                fogColor[0]=0.75;    fogColor[1]=0.75;    fogColor[2]=0.75;
                glFogfv(GL_FOG_COLOR, fogColor);
            }
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GlobalLight);
            glutPostRedisplay();
            break;
        // light 1
        case '1':
            light0=(light0+1)%2;
            if(light0==1 && night==1){
                glEnable(GL_LIGHT0);
            }
            else {
                glDisable(GL_LIGHT0);
            }
        break;
        // light 2
        case '2':
            light1=(light1+1)%2;
            if(light1==1 && night==1){
                glEnable(GL_LIGHT1);
            }
            else {
                glDisable(GL_LIGHT1);
            }
        break;
        // light 3
        case '3':
            light2=(light2+1)%2;
            if(light2==1 && night==1) {
                glEnable(GL_LIGHT2);
            }
            else {
                glDisable(GL_LIGHT2);
            }
        break;
        case 27:
            exit(0);
    }

}

void NotASCIIkeys(int key, int x, int y)
{
    // phi angle
    if(key == GLUT_KEY_UP) {
        phi+=0.05;
    }
    if(key == GLUT_KEY_DOWN) {
        phi-=0.05;
    }
    // theta angle
    if(key == GLUT_KEY_LEFT) {
        theta-=0.05;
    }
    if(key == GLUT_KEY_RIGHT) {
        theta+=0.05;
    }
    finalPos[0] =initialPos[0]-radialDist *cos(theta)*sin(phi);
    finalPos[1] =initialPos[1]-radialDist *cos(phi);
    finalPos[2] =initialPos[2]-radialDist *sin(theta)*sin(phi);
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    initNevoeiro();

    initLights();
    glEnable(GL_LIGHTING);

    glShadeModel(GL_SMOOTH);
    defineTextures();
    glEnable(GL_TEXTURE_2D);

    // reboot the game:
    for(int i=1;i<=9;i++) {
        pieces[i] = 0;
        scale[i] = 0;
    }
    turn=1, end=0, game_status=0;
    rotationAngle = degrees1 = degrees2 = 0;
    fog=0;
}

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

// timer used for the object dynamics
void update(int value)
{
    rotationSky+=0.05;
    rotationAngle += 5.0f;
    if (rotationAngle>360)
        rotationAngle = 0;
    if( rotationSky> 360)
        rotationSky = 0;

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

void drawScene()
{
	glClearStencil(0);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// *************************************** viewport 1 - lower left corner *********************
    glEnable(GL_LIGHT4);                                // illuminates the lower left corner
    glViewport (20, 20, wScreen/4, hScreen/4);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho (-3.0,3.0, 3.0,-3.0, 3.0,-3.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,-2.0,0.0, 0.0,0.0,0.0, 0.0,0.0,1.0);

	// draws objects
	draw_board(0.0f, 0.0f, 0.0f, 0, 1.0f);
    draw_grid(0.0f, 0.0f, 0.0f, 0);

    for(int i=1;i<=9;i++) {
        if(pieces[i] == 1) {
            draw_X_map(grid[i][0],0.0f,grid[i][1]);
        }
        else if(pieces[i] == 2) {
            draw_O_map(grid[i][0],0.0f,grid[i][1]);
        }
    }

    //******************************************** viewport  2 - main ********************************
    glViewport (0, 0, wScreen, hScreen);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(15.0, wScreen/hScreen, 0.1, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(initialPos[0], initialPos[1], initialPos[2], finalPos[0], finalPos[1], finalPos[2], 0, 1, 0);


    glDisable(GL_LIGHT4);
    glColor4f(1.0,1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[1]);

    //draws a surrounding sphere with a sky texture applied
    glPushMatrix();
        GLUquadricObj*y=gluNewQuadric();
        glRotatef (180,1,0,0);
        glRotatef (rotationSky,0,1,0);
        glTranslatef(0,10,0);
        gluQuadricDrawStyle(y,GLU_FILL);
        gluQuadricNormals(y,GLU_SMOOTH);
        gluQuadricTexture(y,GL_TRUE);
        gluSphere(y,100,200,200);
        gluDeleteQuadric(y);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //draws the floor plan and applies a snd texture to it
    glColor4f(1.0,1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glPushMatrix();
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f,0.0f); glVertex3i(100,-5,100);
            glTexCoord2f(10.0f,0.0f); glVertex3i(100,-5,-100);
            glTexCoord2f(10.0f,10.0f); glVertex3i(-100,-5,-100);
            glTexCoord2f(0.0f,10.0f); glVertex3i(-100,-5,100);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glColor4f(1.0,1.0,1.0,1.0);

    //draws spheres that are used to indicate the location of the lamps
    if(light0 && night){
        glPushMatrix();
            glColor3f(1.0,0.0,0.0);
            glTranslatef(0.0,5.0,-3.0);
            glutSolidSphere(0.2f, 100, 100);
        glPopMatrix();
    }
    if(light1 && night){
        glPushMatrix();
            glColor3f(0.0f,1.0f,0.0f);
            glTranslatef(-3.0,5.0,3.0);
            glutSolidSphere(0.2f, 100, 100);
        glPopMatrix();
    }
    if(light2 && night){
        glPushMatrix();
            glColor3f(0.0f,0.0f,1.0f);
            glTranslatef(3.0,5.0,3.0);
            glutSolidSphere(0.2f, 100, 100);
        glPopMatrix();
    }

    // draws game pieces
    for(int i=1;i<=9;i++) {
        if(pieces[i] == 1) {
            draw_X(grid[i][0],0.0f,grid[i][1],0,i);
        }
        else if(pieces[i] == 2) {
            draw_O(grid[i][0],0.0f,grid[i][1],0,i);
        }
    }

    //draws a board
    glEnable(GL_STENCIL_TEST);
    glColorMask(0, 0, 0, 0);
    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    draw_board(0.0f, 0.0f, 0.0f, 0, 1.0f);
    draw_grid(0.0f, 0.0f, 0.0f, 0);

    glColorMask(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // draws reflected pieces
    glPushMatrix();
        glScalef(1, -1, 1);
        for(int i=1;i<=9;i++) {
            if(pieces[i] == 1 && initialPos[1]>0) {
                draw_X(grid[i][0],0.0f,grid[i][1],0,i);
            }
            else if(pieces[i] == 2 && initialPos[1]>0) {
                draw_O(grid[i][0],0.0f,grid[i][1],0,i);
            }
    }
    glPopMatrix();

    // draws tranparent board
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    draw_board(0.0f, 0.0f, 0.0f, 0, 0.8f);
    draw_grid(0.0f, 0.0f, 0.0f, 0);
    glDisable(GL_BLEND);

    //fills auxiliar matrix with game pieces
    int aux=1;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            game[i][j]=pieces[aux++];
        }
    }
    //check game state: 0: still on; 1- x won; 2- o won; 3 - it's a draw
    game_status=check_game(game);


    // ******************** VIEWPORT 3 - Prints messages to the user about the state of the game *************
    glEnable(GL_LIGHT4);
    if (game_status > 0) {
        end=1;

        glViewport (150, 20, wScreen/2, hScreen/4);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho (-3.0,3.0, 3.0,-3.0, 3.0,-3.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0.0,1.0,0.0, 0.0,0.0,0.0, 0.0,0.0,-1.0);

        // desenha objeto
         glPushMatrix();
        glColor4f(1.0f, 0.0f, 0.0f, 1.0);
        glBegin(GL_QUADS);
            glVertex3f(-0.5f,0.0f,-1.0f);
            glVertex3f(3.0f,0.0f,-1.0f);
            glVertex3f(3.0f,0.0f,2.0f);
            glVertex3f(-0.5f,0.0f,2.0f);
        glEnd();

        if(game_status == 1) {
            char msg[]="PLAYER 1 WON!!";
            print(-0.2,0.5,msg,1);
            degrees1 = rotationAngle;
        }
        else if(game_status == 2) {
            char msg[]="PLAYER 2 WON!!";
            print(-0.2,0.5,msg,1);
            degrees2 = rotationAngle;
        }
        else if(game_status == 3) {
            char msg[]="DRAW!";
            print(0.7,0.5,msg,1);
        }
        char msg[]="Click here to play again";
        print(0.2,-0.4,msg,2);

    }

    glutSwapBuffers();
}

//check user mouse coordinates
int checkCoord(int xMouse, int yMouse) {

    if (!end) {
        for(int i=0; i<9; i++) {
             if(xMouse>screen[i][0] && xMouse<screen[i][2] && yMouse>screen[i][3] && yMouse<screen[i][1]) {
                return i+1;
             }
        }
    }
    else {
       if(xMouse>screen[9][0] && xMouse<screen[9][2] && yMouse>screen[9][3] && yMouse<screen[9][1]) {
            return 1;
        }
    }
    return 0;
}

//change turn when the user clicks in the lower left board
void mouseClick( int button, int buttonState, int x, int y ){

    int pos;

    if (buttonState == GLUT_DOWN) {
        pos=checkCoord(xMouse, yMouse);

        // the game isn't over yet
        if (!end) {
            if(pos > 0 && pieces[pos] == 0) {
                pieces[pos] = turn;
                turn = 3-turn;
            }
        }
        // if it's over, reboot the game
        else {
            if(pos==1) {
                initRendering();
            }
        }
    }
}


void getMouseCoordinates(int x,int y){
    xMouse=x;
    yMouse=y;
}


int main(int argc, char *argv[])
{
    char filename[10];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(wScreen, hScreen);
    glutCreateWindow("Tik Tak Toe");
    glutPassiveMotionFunc(getMouseCoordinates);

    initRendering();
    glutDisplayFunc(drawScene);
    glutTimerFunc(25, update, 0);
    glutKeyboardFunc(handleKeyPress);
    glutSpecialFunc(NotASCIIkeys);
    glutMouseFunc(mouseClick);
    glutReshapeFunc(NULL);

    //load object models 
    x_model = (GLMmodel*)malloc(sizeof(GLMmodel));
    strcpy(filename,"x.obj");
    x_model = glmReadOBJ(filename);

    display_X=glGenLists(1);
    glNewList(display_X,GL_COMPILE);
        glmDraw(x_model, GLM_SMOOTH);
    glEndList();

    o_model = (GLMmodel*)malloc(sizeof(GLMmodel));
    strcpy(filename,"O.obj");
    o_model = glmReadOBJ(filename);

    display_O=glGenLists(1);
    glNewList(display_O,GL_COMPILE);
        glmDraw(o_model, GLM_SMOOTH);
    glEndList();

    board = (GLMmodel*)malloc(sizeof(GLMmodel));
    strcpy(filename,"board.obj");
    board = glmReadOBJ(filename);

    display_board=glGenLists(1);
    glNewList(display_board,GL_COMPILE);
        glmDraw(board, GLM_SMOOTH);
    glEndList();


    net = (GLMmodel*)malloc(sizeof(GLMmodel));
    strcpy(filename,"grid.obj");
    net = glmReadOBJ(filename);

    display_grid=glGenLists(1);
    glNewList(display_grid,GL_COMPILE);
        glmDraw(net, GLM_SMOOTH);
    glEndList();

    glutMainLoop();

    return 0;
}
