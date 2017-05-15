//By John Zhu and Henan Zhao
//VR Class Project
//04/06/2017

#include <omega.h>
#include <omegaGl.h>

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "svQDOT.h"
#include "svSplitArrow.h"
#include "svDirectArrow.h"
#include "svSummaryGlyph.h"
#include "svImage.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include "svOutline.h"
#include "svLegend.h"
#include "svQDOTImage.h"

using namespace omega;

#define CLUSTER_DIMENSION 7

using namespace __svl_lib;

//svQDOT *flow_field;
//svSplitArrow *splitglyph;
//svDirectArrow *directglyph;
//svOutline *outline;
//svImage *image;


typedef struct ConfigProperty{
	
	char *rawDir;
	char *rawFile;
	char *storeDir;
	
	svVector3 plane_center;
    svVector3 plane_vector;
    svScalar plane_distance;
	
	KmeansProperty step1_kmeansproperty;
	KmeansProperty step2_kmeansproperty;

	svScalarArray *magrange;
	
}; 
//configproperty;

int contourindex = 0;
bool newlist;

Trackball trackball;
GLfloat m[16];

double zmin;
double zmax;


float shiftx = 0;
float shifty = 0;
Vector3f rs;
Vector3f re;
bool middle_down = false;
double scale;
Vector3f startPos;

float analogLR = 0;
float analogUD = 0;
float analogL2 = 1;
bool button2Hold = false;
bool button5Hold = false;
bool button7Hold = false;
bool leftbuttonHold = false;
bool buttonUPHold = false;
bool buttonDNHold = false;
bool buttonLTHold = false;
bool buttonRTHold = false;

   svLegend *legend;
void InitLight()
{
  GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 20.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
}

//***************************
// GLUT callback functions
//****************************

class QDOTRenderPass:  public RenderPass
{
public:
    QDOTRenderPass(Renderer* client): RenderPass(client, "QDOTRenderPass") {}
    virtual void initialize();
    virtual void render(Renderer* client, const DrawContext& context);
   
    virtual void DetectHit();
    virtual void Generate();
private:
   svOutline *outline; 
   svQDOT *flow_field;
   svDirectArrow *directglyph;
   svSplitArrow *splitglyph;
   svQDOTImage *qdotimage;

   ConfigProperty configproperty;

      svVector3 myNormals[6];
      Vector4i myFaces[6];
      svVector3 myVertices[8];
      Color myFaceColors[6];

};

void init();

class QDOTApplication: public EngineModule
{
public:
    QDOTApplication(): EngineModule("QDOTApplication") {}

    virtual void initializeRenderer(Renderer* r) 
    {
	render=  new QDOTRenderPass(r);
        r->addRenderPass(render);
    }
    virtual void handleEvent(const Event& evt);
    virtual void update(const UpdateContext & context);
private: 
    QDOTRenderPass *render;
};

void QDOTRenderPass::Generate()
{

}

void QDOTRenderPass::DetectHit()
{

}

void QDOTRenderPass::render(Renderer* client, const DrawContext& context)
{
	//'cout<<context.task<<" "<<DrawContext::SceneDrawTask<<endl;
    if(context.task == DrawContext::SceneDrawTask)
    {
        client->getRenderer()->beginDraw3D(context);
        if(oglError) return;

       // if(newlist){   qdotimage->Generate(legend->GetBinIndex(), legend->GetBinPositions(),
         //             legend->GetBinCollapsed(), 0.25); newlist = false;}

 
        if(newlist)
        {

            directglyph->SetLayerShift(legend->GetBinPositions(),
                      legend->GetBinCollapsed(),legend->GetBinIndex());
             qdotimage->Generate(legend->GetBinIndex(), legend->GetBinPositions(),
                       legend->GetBinCollapsed(), 0.25);
            directglyph->Generate();
	    newlist = false;
        }

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
        glPushMatrix();

	glTranslatef(0,1.5,-6);
	glMultMatrixf(m); 
	glRotatef(-90,1,0,0);
    glTranslatef(shiftx, 0,shifty);
	glScalef(0.1,0.1,0.1);
	glScalef(scale,scale,scale);

        directglyph->DrawOutline();


        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT0);
        //glEnable(GL_TEXTURE_2D);
        //outline->DrawXYZ(splitglyph->GetLb(), splitglyph->GetRb());

        //glColor3f(1,0,0);
//glEnable(GL_LIGHTING);
//glEnable(GL_LIGHT0);
        directglyph->Render();
 	//glCallList(5);
 	    glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);

	legend->Render(1);
        //qdotimage->Render();

        glPopMatrix(); 


	//glDisable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	//glPushMatrix();
        //glTranslatef(0,2,-2);
//	cout<<"render"<<endl;

       /*  for (int i = 0; i < 6; i++)
          {
              glBegin(GL_QUADS);

              glColor3fv(myFaceColors[i].data());
              glNormal3f(myNormals[i][0],myNormals[i][1],myNormals[i][2]);
              glVertex3f(myVertices[myFaces[i][0]][0],myVertices[myFaces[i][0]][1],myVertices[myFaces[i][0]][2]);
              glVertex3f(myVertices[myFaces[i][1]][0],myVertices[myFaces[i][1]][1],myVertices[myFaces[i][1]][2]);
              glVertex3f(myVertices[myFaces[i][2]][0],myVertices[myFaces[i][2]][1],myVertices[myFaces[i][2]][2]);
              glVertex3f(myVertices[myFaces[i][3]][0],myVertices[myFaces[i][3]][1],myVertices[myFaces[i][3]][2]);
              glEnd();
          }
         glPopMatrix();*/

		glColor3f(1,0,0);
		svVector3 rays;rays[0]=rs[0];rays[1]=rs[1];rays[2]=rs[2];
		svVector3 raye;raye[0]=re[0];raye[1]=re[1];raye[2]=re[2];
		svVector3 tpoints[11];
		svVector3 n1 = normalize(raye - rays);
		svVector3 n2 = svGetPerpendicularVector(n1);
		svScalar radius = 0.005;
		svVector3 head = rays + radius * n2;
		for(int i=0;i<11;i++)
		{								
			tpoints[i] = svGetRotatePoint(head, rays, n1,  (float)i * 36);
		}
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(raye[0], raye[1], raye[2]);
		for(int i=0;i<11;i++)
		{
			glVertex3f(tpoints[i][0], tpoints[i][1], tpoints[i][2]);
		}
		glEnd();

   
/*        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);

        directglyph->DrawOutline();
        splitglyph->DrawOutline();

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_TEXTURE_2D);
        outline->DrawXYZ(splitglyph->GetLb(), splitglyph->GetRb());

        glColor3f(1,0,0);
        splitglyph->Render();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glPopMatrix();
 */  
        if(oglError) return;
        client->getRenderer()->endDraw();
        if(oglError) return;
    }
  //  else if(context.task == DrawContext::OverlayDrawTask && context.eye == DrawContext::EyeCyclop)
   // {

   // }
}


//void init();
void QDOTRenderPass::initialize()//rbfname, char *cpname)
{
  RenderPass::initialize();
  newlist = false;

  flow_field = new svQDOT();
  svVector3 color;
  color[0]=1;color[1]=0;color[2]=0;
  legend = new svLegend(50, -3.5, 8.75, 0.1, color);
  svVector3 plane_center(0,0,0);
  svVector3 plane_vector(0,0,1);
  svScalar plane_distance=0.25;

    flow_field->SetVTK("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/", 
            "task2sample2",
                       "/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/",
                       "sort.txt", "format.txt", "density.txt",
                       plane_center,
                       plane_vector,
                       plane_distance);
    flow_field->New("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2/format.txt");

  zmin=0;
  zmax = 49;//flow_field->GetPlaneNum()-1;

  char *str = new char[400];

  directglyph  = new svDirectArrow(flow_field);
  directglyph->New(flow_field, 50);

  qdotimage = new svQDOTImage("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2");
  qdotimage->New(flow_field, 50);

  for(int i=0;i<50;i++)//
  {
              sprintf(str, "/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2/%d.txt",i);// configproperty.storeDir, configproperty.rawFile, i);
	          //cout<<str<<endl; //directglyph->SetData(str, i);
              directglyph->SetDataLayer(str,i );
              qdotimage->SetDataLayer(str, i);
  }

  directglyph->GenerateClusters("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2/cluster.txt");//configproperty.step1_kmeansproperty);
  qdotimage->GenerateClusters("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2/cluster.txt");
  qdotimage->SetDisplayList(30);
  //directglyph->SetColorByCluster();
              directglyph->SetLayerShift(legend->GetBinPositions(),
                           legend->GetBinCollapsed(),legend->GetBinIndex());
         int scaling = flow_field->GetScaling();
      directglyph->SetScaling(scaling);
 

  directglyph->Generate();
  qdotimage->SetImage("/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/tmp/task/task2sample2/");
  qdotimage->Generate(legend->GetBinIndex(), legend->GetBinPositions(),
                      legend->GetBinCollapsed(), 0.25);

  svInt list = 10;
  scale = 1;
  analogLR = 0;
  analogUD = 0;

 delete [] str;

}

int selected_widget;
svVector3 newray[2];
void setPickingray()
{
        trackball.getMatrix().getValue(m);

    GLfloat invert_tb[16];
    if(!invert_matrix(m, invert_tb)) {

    }

	glTranslatef(0,1.5,-6);
	glMultMatrixf(m); 
	glRotatef(-90,1,0,0);
	glScalef(0.1,0.1,0.1);
	glScalef(scale,scale,scale);


    svVector3 ray1, ray2;
/*ray1[0] = rs[0]*10/scale;
ray1[1] = (-rs[2] - 6)*10/scale;
ray1[2] = (rs[1] - 1.5)*10/scale;
ray2[0] = re[0]*10/scale;
ray2[1] = (-re[2] - 6)*10/scale;
ray2[2] = (re[1] - 1.5)*10/scale;*/
    ray1[0] = rs[0];
    ray1[1] = rs[1] - 1.5;
    ray1[2] = rs[2] + 6;
    ray2[0] = re[0];
    ray2[1] = re[1] - 1.5;
    ray2[2] = re[2] + 6;

    newray[0][0] = ray1[0] * invert_tb[0] +
            ray1[1] * invert_tb[4] +
            ray1[2] * invert_tb[8] +
            invert_tb[12];
    newray[0][1] = ray1[0] * invert_tb[1] +
            ray1[1] * invert_tb[5] +
            ray1[2] * invert_tb[9] +
            invert_tb[13];
    newray[0][2] = ray1[0] * invert_tb[2] +
            ray1[1] * invert_tb[6] +
            ray1[2] * invert_tb[10] +
            invert_tb[14];

    newray[1][0] = ray2[0] * invert_tb[0] +
            (ray2[1]) * invert_tb[4] +
            (ray2[2]) * invert_tb[8] +
            invert_tb[12];
    newray[1][1] = (ray2[0]) * invert_tb[1] +
            (ray2[1]) * invert_tb[5] +
            (ray2[2]) * invert_tb[9] +
            invert_tb[13];
    newray[1][2] = (ray2[0])  * invert_tb[2] +
            (ray2[1])  * invert_tb[6] +
            (ray2[2])  * invert_tb[10] +
            invert_tb[14];
    
    ray1 = newray[0];
    ray2 = newray[1];
 
    newray[0][0] = (ray1[0]-shiftx)*10/scale;
    newray[0][1] = (-ray1[2])*10/scale;
    newray[0][2] = (ray1[1]-shifty)*10/scale;
    newray[1][0] = (ray2[0]-shiftx)*10/scale;
    newray[1][1] = (-ray2[2])*10/scale;
    newray[1][2] = (ray2[1]-shifty)*10/scale;
 
}
void QDOTApplication::handleEvent(const Event & evt)
{

    if(evt.getServiceType() == Service::Wand)
    {
         DisplaySystem* ds = SystemManager::instance()->getDisplaySystem();
         Ray r;
         bool res = ds->getViewRayFromEvent(evt, r);
        //cout<<res<<endl;
         rs = r.getOrigin();
         //cout<<rs[0]<<" "<<rs[1]<<" "<<rs[2]<<endl;
         re = r. getDirection();
         re[0] = re[0]*5 + rs[0];
         re[1] = re[1]*5 + rs[1];
         re[2] = re[2]*5 + rs[2];

         //
         if(!leftbuttonHold && (evt.getType() != Event::Zoom))
         {
         //cout<<re[0]<<" "<<re[1]<<" "<<re[2]<<endl;
	//	cout<<evt.getServiceType()<<endl;
	    analogLR = -evt.getAxis(0)+analogLR;
	    analogUD = -evt.getAxis(1)+analogUD;
	    float factor = 40;
	    if(evt.isButtonDown(Event::Button7))
	    {
		button7Hold = true;
	    }
            if(analogL2 > 0.1)
               analogL2 = analogL2 + evt.getAxis(4);
	    if(evt.isButtonUp(Event::Button7))
	    {
		button7Hold = false;
	    }

	    if(evt.isButtonDown(Event::Button5))
	    {

		button5Hold = true;
	    }
	    if(evt.isButtonUp(Event::Button5))
	   {
		button5Hold = false;
	   }

                setPickingray();

            if(!button2Hold)
                legend->Close(newray[0], newray[1]);
            if(evt.isButtonDown(Event::Button6))
            {
                scale = 1;
                shiftx=0;shifty=0;
                trackball.reset();
            }
	    if(evt.isButtonDown(Event::Button2))
	    {
		button2Hold = true; 


		//cout<<newray[1][0]<<" "<<newray[1][1]<<" "<<newray[1][2]<<endl;
		selected_widget=legend->Select(newray[0], newray[1]);//svVector3 rayp1, svVector3 rayp2, svBool state);
	    }
	    if(evt.isButtonUp(Event::Button2))
	   {
		/*svVector3 newray[2];
		newray[0][0] = rs[0]*10/scale;
		newray[0][1] = (-rs[2] - 6)*10/scale;
		newray[0][2] = (rs[1] - 1.5)*10/scale;
		newray[1][0] = re[0]*10/scale;
		newray[1][1] = (-re[2] - 6)*10/scale;
		newray[1][2] = (re[1] - 1.5)*10/scale;*/
		//cout<<newray[1][0]<<" "<<newray[1][1]<<" "<<newray[1][2]<<endl;
		legend->Move(newray[0], newray[1], selected_widget);//svVector3 rayp1, svVector3 rayp2, svBool state);
		button2Hold = false;
		legend->Process();
                newlist=true;
	   }	
	   if(button2Hold)
	   {
		/*svVector3 newray[2];
		newray[0][0] = rs[0]*10/scale;
		newray[0][1] = (-rs[2] - 6)*10/scale;
		newray[0][2] = (rs[1] - 1.5)*10/scale;
		newray[1][0] = re[0]*10/scale;
		newray[1][1] = (-re[2] - 6)*10/scale;
		newray[1][2] = (re[1] - 1.5)*10/scale;*/
                //setPickingray();
		legend->Move(newray[0], newray[1], selected_widget);//svVector3 rayp1, svVector3 rayp2, svBool state);
	   }   
	    // Panning Inputs
	    if(evt.isButtonDown(Event::ButtonUp)){
		buttonUPHold = true;}
	    if(evt.isButtonUp(Event::ButtonUp)){
		buttonUPHold = false;
		}
	    if(evt.isButtonDown(Event::ButtonDown)){
		buttonDNHold = true;}
	    if(evt.isButtonUp(Event::ButtonDown)){
		buttonDNHold = false;
		}
	    if(evt.isButtonDown(Event::ButtonLeft)){
		buttonLTHold = true;}
	    if(evt.isButtonUp(Event::ButtonLeft)){
		buttonLTHold = false;
		}
	    if(evt.isButtonDown(Event::ButtonRight)){
		buttonRTHold = true;}
	    if(evt.isButtonUp(Event::ButtonRight)){
		buttonRTHold = false;
		}

        float lr = evt.getAxis(0);
        float ud = evt.getAxis(1);
	//cout<<lr<<" "<<ud<<endl;
	    if((lr *ud) > 0.001 || (lr*ud) < -0.001 )
	    {
	           trackball.mouseMotion(analogLR * factor,  analogUD  * factor);
	    }
	    else if(!button5Hold && !button7Hold &&!button2Hold)
		   trackball.mouseDown(0, analogLR  * factor,  analogUD  * factor);
         }
    }
    else if(evt.getServiceType() == Service::Pointer)
    {
	///cout<<"pointer"<<endl;
	long s=0x00000000;
	Vector3f v = evt.getPosition();
	double x = v[0];
	double y = v[1];
        if(evt.isButtonDown(Event::Left))//Event::Click  && Widget::getSource<Button>(e) == mybutton)
	{
		trackball.mouseDown(0, x, y);
		leftbuttonHold = true;
		//cout<<x<<" "<<y<<endl; analogUD 
		//ttt++;
	}
	if(evt.isButtonUp(Event::Left))//Event::Click  && Widget::getSource<Button>(e) == mybutton)
	{
		trackball.mouseUp(s, x, y);
		trackball.getRotateMatrix().getValue(m);
		leftbuttonHold = false;	
	}
 	if(evt.isFlagSet(Event::Left))
	{
		trackball.mouseMotion(x, y);
		//cout<<x<<" "<<y<<endl;
	}
	if(evt.getType() == Event::Zoom)
	{
		double zoom = evt.getExtraDataInt(0);
		double s = 0.9;
		if(zoom > 0) s = 1.1;
		scale = scale * s;
	}
    }

}

void QDOTApplication::update(const UpdateContext & context)
{
	//trackball.getMatrix().getValue(m);
        trackball.getMatrix().getValue(m);
	
	if(button7Hold)
	{
		scale =  scale*1.05;
	}
	if(button5Hold)
	{
		scale = scale/1.05;
	}
	if(buttonUPHold){
		shifty = shifty - 0.05;
	}
	if(buttonDNHold){
		shifty = shifty + 0.05;
	}
	if(buttonLTHold){
		shiftx = shiftx + 0.05;
	}
	if(buttonRTHold){
		shiftx = shiftx - 0.05;
	}

}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
//  srand(12345);
  //  init();
   //glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH|GLUT_DOUBLE);
  // glutInit(&argc, argv);
   //glutInitWindowSize(1000,1000);
   //glutCreateWindow("QDOT");
   //glutInitWindowPosition(0,0);

    Application<QDOTApplication> app("qdot");
        return omain(app, argc, argv);
  
}
