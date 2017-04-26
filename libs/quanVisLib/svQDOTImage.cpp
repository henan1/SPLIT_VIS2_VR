#include "svQDOTImage.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"
#include "svGlyph.h"
#include <cstdlib>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

namespace __svl_lib{

svQDOTImage::svQDOTImage(svChar *inf)
{
     //imageWidths = new svScalar2DArray[1];
     //imageSpacing = new svScalar2DArray[1];

     pSize[0] = 50;
     pSize[1] = 50;
     pSize[2] = 0.25;

     SetImage(inf);
}

void svQDOTImage::SetImage(svChar *inf)
{
     //cleanup();     
     imageWidths = new svScalar2DArray[seed_num];
     imageSpacing = new svScalar2DArray[seed_num];
     char *str = new char[400];
 
     for(int ii=0;ii<seed_num;ii++)
     {
	     sprintf(str, "%s/zlayer%d.txt", inf, ii);
	     ifstream infile(str);

	     int n;
	     infile>>n;

             svVector3 prep;
	     svVector3 p;
	     svScalarArray w, s;
	     for(int j=0;j<n;j++)
	     {
                prep = p;
                infile>>p[0]>>p[1]>>p[2];

                if(j==0)
                {
                    w.add(1); //hard code!!!!!!!!!!!!!!
                    s.add(p[0]);
                }    
                else if(fabs(prep[1] - p[1])<1e-8)
                {
                    w.add(1); //hard code!!!!!!!!!!!!!!!
                    s.add(0);
                }
                else if(fabs(prep[1] - p[1])>1e-8)
                {
                    s.add(pSize[0] - prep[0]);
                    imageWidths[ii].add(w);
                    imageSpacing[ii].add(s);
                    w.free();
                    s.free();
                    w.add(1);
                    s.add(p[0]);
                }
	    } 
 
            //j==n-1
            s.add(pSize[0] - p[0]);
            imageWidths[ii].add(w);
            imageSpacing[ii].add(s);
	    w.free();s.free();

       //    cout<<ii<<" "<<imageWidths[ii].size()<<endl;

	    infile.close();
     }
    // for(int ii=0;ii<seed_num;ii++) 
    //   cout<<imageWidths[ii].size()<<endl;
}

void svQDOTImage::Generate(MArray<svIntArray> bins, svVector3Array positions, 
              svIntArray collapsed, svScalar separation)//need to be fixed for future use: arbitrary planes
{
       if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
       glNewList(display_list, GL_COMPILE);

      // glDisable(GL_LIGHTING);
       //glDisable(GL_LIGHT0);

      // this->separation = separation;



       for(int i=0;i<collapsed.size();i++)
       {
           if(collapsed[i])
           {
       		svRasterArray color;
       		MArray<svScalar2DArray> width;
       		MArray<svScalar2DArray> spacing;
              for(int j=bins[i][0];j<=bins[i][bins[i].size()-1];j++)
              {
                  int count = 0;
                  svRaster ccc;
                  svScalar2DArray www;
                  svScalar2DArray sss;
			//cout<<j<<" "<<imageWidths.size()<<endl;
                  for(int ii=0;ii<imageWidths[j].size();ii++)
                  {
                        svVector3Array cc;
                        svScalarArray ww;
                        svScalarArray ss;
                    //    cout<<ii<<" "<<imageWidths[j][ii].size()<<endl;
			
                        for(int jj=0;jj<imageWidths[j][ii].size();jj++)
                        {
                             svVector3 c;
                             c[0] = glyphColors[j][count][0];
                             c[1] = glyphColors[j][count][1];
                             c[2] = glyphColors[j][count][2];
                             cc.add(c);
                             ww.add(imageWidths[j][ii][jj]);
                             ss.add(imageSpacing[j][ii][jj]);
                             count++;
                        }
                        ss.add(imageSpacing[j][ii][imageWidths[j][ii].size()]);
                        www.add(ww); //ww.free();
                        sss.add(ss); //ss.free();
                        ccc.add(cc); //cc.free();
                   }
                   width.add(www); //www.free();
                   spacing.add(sss); //sss.free();
                   color.add(ccc); //ccc.free();
              }
            
              svImage *image = new svImage(color, width, spacing,
                                             pSize[0],pSize[1], pSize[2],
                                             0,0,0,
                                             separation,0.5);

              glPushMatrix();
              glTranslatef(0,0, positions[i][2]);
              glRotatef(90,1,0,0);
              glScalef(0.01,0.01,0.01);
              image->Render(1);
              glPopMatrix();             

             /* delete image;
		

              for(int i=0;i<width.size();i++){
                 for(int j=0;j<width[i].size();j++){
                     width[i][j].free();
                     color[i][j].free();
                     spacing[i][j].free();
                 }
                 width[i].free();
                 color[i].free();
                 spacing[i].free();
              }  
              width.free();
              color.free();
              spacing.free();*/
           }
       }

	glEndList();
}

void svQDOTImage::Render()
{
     glCallList(display_list);
}

void svQDOTImage::cleanup()
{
    for(int i=0;i<seed_num;i++)
    {   for(int j=0;j<imageWidths[i].size();j++)
            imageWidths[i][j].free();
       imageWidths[i].free();
    }
    delete [] imageWidths;

    for(int i=0;i<seed_num;i++)
    {   for(int j=0;j<imageSpacing[i].size();j++)
            imageSpacing[i][j].free();
       imageSpacing[i].free();
    }
    delete [] imageSpacing;
}

}
