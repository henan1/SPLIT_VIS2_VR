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
// configproperty;

int contourindex = 0;
bool newlist=false;

Trackball trackball;

double zmin;
double zmax;

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
    void config(char *configfname, ConfigProperty &property);
    virtual void initialize();
    virtual void render(Renderer* client, const DrawContext& context);
   
    virtual void DetectHit();
    virtual void Generate();
private:
   svOutline *outline; 
   svQDOT *flow_field;
   svSplitArrow *splitglyph;
   svDirectArrow *directglyph;
 
   ConfigProperty configproperty;
};


class QDOTApplication: public EngineModule
{
public:
    QDOTApplication(): EngineModule("QDOTApplication") {}

    virtual void initializeRenderer(Renderer* r) 
    {
        r->addRenderPass(new QDOTRenderPass(r));
    }
 //   virtual void handleEvent(const Event& evt);
  //  virtual void update(const UpdateContext & context);
};

void QDOTRenderPass::Generate()
{

}

void QDOTRenderPass::DetectHit()
{

}

void  QDOTRenderPass::render(Renderer* client, const DrawContext& context)
{
    if(context.task == DrawContext::SceneDrawTask)
    {
        InitLight();

        client->getRenderer()->beginDraw3D(context);
        if(oglError) return;

        if(newlist) Generate();
        
/*        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

        GLfloat m[16];
        trackball.getMatrix().getValue(m);

        glPushMatrix();
        glMultMatrixf(m);

//        directglyph->DrawOutline();
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
    else if(context.task == DrawContext::OverlayDrawTask && context.eye == DrawContext::EyeCyclop)
    {

    }
}

void QDOTRenderPass::config(char *configfname, ConfigProperty &property)
{
	contourindex = 0;
    float store;

	ifstream infile(configfname);
    cout<<configfname<<endl;
	property.rawDir = new char[200];
	property.rawFile = new char[200];
	property.storeDir = new char[200];
	
	
	string tmp;
	/*-------------file names-----------------*/
	infile>>tmp;
	infile>>property.rawDir;
	infile>>tmp;
	infile>>property.rawFile;
	infile>>tmp;
	infile>>property.storeDir;cout<<property.storeDir<<endl;
	mkdir(property.storeDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  

    infile>>tmp;
	infile>>property.plane_center[0]>>property.plane_center[1]>>property.plane_center[2];
	infile>>tmp;
	infile>>property.plane_vector[0]>>property.plane_vector[1]>>property.plane_vector[2];
	infile>>tmp;
	infile>>property.plane_distance;
	/*--------------initialization of QDOT field------------*/
	char *qdot_format = new char[200];
	sprintf(qdot_format,"%s/%s/format.txt", property.storeDir, 
	            property.rawFile);
   cout<<qdot_format<<endl;
    flow_field->SetVTK(property.rawDir, property.rawFile,
	                   property.storeDir,
					   "sort.txt", "format.txt", "density.txt",
					   property.plane_center,
					   property.plane_vector,
					   property.plane_distance);
   cout<<qdot_format<<endl;
    flow_field->New(qdot_format);
   cout<<qdot_format<<endl;	
	delete [] qdot_format;
	
	/*--------------clustering------------------------------*/
	infile>>tmp; cerr<<tmp<<endl;
	infile>>tmp;

	property.magrange = new svScalarArray[2];
	
	/*-------------step 1-----------------------------------*/
    property.step1_kmeansproperty.dimension = 7;
    property.step1_kmeansproperty.isNormalize = true;
    property.step1_kmeansproperty.clusterLayer.free();
    property.step1_kmeansproperty.clusterWeight.free();
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step1_kmeansproperty.clusterLayer.add(-1);
    }   
  	infile>>tmp;// cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step1_kmeansproperty.clusterWeight.add(store);
	}	
	int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    bool isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step1_kmeansproperty.clusterLayer[i]= property.step1_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
    char *str = new char[400];
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  property.magrange[0].add(store);
	infile>>store;  property.magrange[0].add(store);
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;   property.step1_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step1_kmeansproperty.file2 = strdup(str);

    /*--------------------step 2-------------------------------------------*/
    property.step2_kmeansproperty.dimension = 7;
    property.step2_kmeansproperty.isNormalize = true;
property.step2_kmeansproperty.clusterLayer.free();
property.step2_kmeansproperty.clusterWeight.free();
    for(int i=0;i<flow_field->GetPlaneNum();i++)
    {
        property.step2_kmeansproperty.clusterLayer.add(-1);
    }  
        infile>>tmp;//cerr<<tmp<<endl; 
  	infile>>tmp; //cerr<<tmp<<endl;
	for(int i=0;i<CLUSTER_DIMENSION;i++)
	{
		infile>>store;
		property.step2_kmeansproperty.clusterWeight.add(store);
	}	
	//int index1, index2;
	infile>>tmp;//cerr<<tmp<<endl;
	infile>>index1;
	infile>>index2;
	for(int i=index1; i<=index2;i++)
	{
		property.step2_kmeansproperty.clusterLayer[i]++;
	}
	infile>>tmp; //cerr<<tmp<<endl;
	infile>>store;  
    isWhole = store;
    if(isWhole)
    {
                 for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]++;
                }
    }
    else
    {
            int count = 1;
             for(int i=index1; i<=index2;i++)
                {
                   property.step2_kmeansproperty.clusterLayer[i]= property.step2_kmeansproperty.clusterLayer[i]+count;
                   count++; 
                }
    }
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;  property.magrange[1].add(store);
	infile>>store;  property.magrange[1].add(store);
	infile>>tmp;// cerr<<tmp<<endl;
	infile>>store;   property.step2_kmeansproperty.numCluster = store;	
	sprintf(str, "%s/%s/input.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file1 = strdup(str);
	sprintf(str, "%s/%s/output.txt", property.storeDir, property.rawFile);
	property.step2_kmeansproperty.file2 = strdup(str);

   // zmin=0;
   // zmax = flow_field->GetPlaneNum()-1;

    delete [] str;	
	infile.close();

}
void QDOTRenderPass::initialize()//rbfname, char *cpname)
{
  outline = new svOutline();
  flow_field = new svQDOT();
  char *configfname = new char[200];
  sprintf(configfname, "/home/henan/Documents/OmegaLib/examples/SPLIT_VIS2/SplitTool/config");
  config(configfname, configproperty);
   
    zmin=0;
    zmax = flow_field->GetPlaneNum()-1;

  char *str = new char[400];

  splitglyph  = new svSplitArrow(flow_field);
  splitglyph->New(flow_field, flow_field->GetPlaneNum());
  directglyph = new svDirectArrow(flow_field);//cerr<<"done"<<endl;
  directglyph->New(flow_field, flow_field->GetPlaneNum());
  for(int i=0;i<flow_field->GetPlaneNum();i++)
  {
               sprintf(str, "%s/%s/%d.txt", configproperty.storeDir, configproperty.rawFile, i);
	           directglyph->SetData(str, i);
               splitglyph->SetData(str,i );
  }
  splitglyph->SetContourLabel();
  directglyph->SetContourLabel();

  directglyph->ResetCluster();
  directglyph->SetVisible(contourindex);
  directglyph->SetVisible(zmin, zmax);
  directglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
  directglyph->GenerateClusters(configproperty.step1_kmeansproperty);
  directglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
  directglyph->GenerateClusters(configproperty.step2_kmeansproperty);
  directglyph->SetColorByCluster();
  directglyph->Generate();

  splitglyph->ResetCluster();
  splitglyph->SetVisible(contourindex);
  splitglyph->SetVisible(zmin, zmax);
  splitglyph->SetROI(configproperty.magrange[0][0], configproperty.magrange[0][1]);
  splitglyph->GenerateClusters(configproperty.step1_kmeansproperty);
  splitglyph->SetROI(configproperty.magrange[1][0], configproperty.magrange[1][1]);
  splitglyph->GenerateClusters(configproperty.step2_kmeansproperty);
  splitglyph->SetColorByCluster();
  splitglyph->Generate();


  svInt list = 10;
  
  delete [] str;

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
  srand(12345);

   //glutInit(&argc, argv);
    Application<QDOTApplication> app("qdot");
        return omain(app, argc, argv);
  
}
