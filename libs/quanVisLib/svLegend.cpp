#include <stdio.h>
#include <iostream>
#include <fstream>
#include "svLegend.h"
#include "svException.h"
#include "svUtil.h"
#include "svGlyph.h"
#include <cstdlib>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

namespace __svl_lib {
     svLegend::svLegend(svInt N, svScalar origin, svScalar end,
                        svScalar radius, svColor3 color) {
          init(N, origin, end, radius, color);
     }

     svLegend::svLegend(svInt N, svScalar origin, svScalar end, svVector3 color) {
          init(N, origin, end, (2*(end-origin) / (N*(N/2 + 1))) / 2, color);

     }

     svLegend::svLegend(svInt N, svScalar radius, svVector3 color) {
          init(N, 0, radius*N*(N/2 + 1), radius, color);
     }

     svLegend::~svLegend() {  }

     void svLegend::init(svInt N, svScalar origin, svScalar end,
                         svScalar radius, svColor3 color) {
          assert(N > 0);
          assert(end > origin);
          //assert(2*radius <= (2*(end-origin) / (N*(N/2 + 1))));
          assert(color[0] >= 0 && color[0] <= 1);
          assert(color[1] >= 0 && color[1] <= 1);
          assert(color[2] >= 0 && color[2] <= 1);

          this->N = N;
          this->origin = origin;
          this->end = end;
          this->radius = radius;
          this->color = color;

          svScalarArray *tPositions = new svScalarArray(N);
          svIntArray *tCollapsed = new svIntArray(N);
          svIntArray *tSelected = new svIntArray(N);
          this->positions = *tPositions;
          this->collapsed = *tCollapsed;
	  this->selected = *tSelected;

          delete tSelected;
          delete tPositions;
          delete tCollapsed;

          for (int n = 0; n < N; n++)
          {     this->collapsed[n] = SV_EXPANDED;
                this->selected[n] = false;}

          updatePositions();
     }

     void svLegend::Expand(svInt index) {
          assert(index >= 0 && index < N);

          collapsed[index] = SV_EXPANDED;
          //updatePositions();
     }

     void svLegend::Collapse(svInt index) {
          assert(index >= 0 && index < N);

          collapsed[index] = SV_COLLAPSED;
          //updatePositions();
     }

void svLegend::Move(svVector3 rayp1, svVector3 rayp2, int index)
{
	select.free();
	
        double td, dd;
        td = 9e+9;
        int j =-1;
        svVector3 p;
	for(int i=0;i<N;i++)
        {
	    selected[i] = false;
            p[0]=-9.75;p[1]=-9.75;p[2]=positions[i];
            //if(GetDot(p-rayp1, rayp2-rayp1)>=0.000001)
            //{
                 dd = PointtoLine(rayp1, rayp2, p);
                 if(dd < td)// && dd<1)
                 {
                       j= i ;td = dd;
                 }
            //}
        } 
        if(index>-1 && j > -1)
        {      
               if(j>index)
		{
			for(int i=index;i<=j;i++)
			{		
				select.add(i);
				selected[i] = true;
			}
		}
		else
		{
			for(int i=j;i<=index;i++)
			{		
				select.add(i);
				selected[i] = true;
			}
		}
        }      
}
int svLegend::Select(svVector3 rayp1, svVector3 rayp2)
{
        close = -1;

        double td, dd;
        td = 9e+9;
        int j =-1;
        svVector3 p;
	for(int i=0;i<N;i++)
        {
            p[0]=-9.75;p[1]=-9.75;p[2]=positions[i];
            if(GetDot(p-rayp1, rayp2-rayp1)>=0.000001)
            {
                 dd = PointtoLine(rayp1, rayp2, p);
                 if(dd < td && dd<2)
                 {
                       j= i ;td = dd;
                 }
            }
        }
	/*if(state == 1 && j>-1)
	{
		if(select.size() > 1)
		{
                        int jj = j - select[select.size()-1];
			if(abs(jj) > 1 && select[select.size()-1] > 0 && select[select.size()-1] < N-1) 
				j = select[select.size()-1] + jj;
		}
	}*/
	bool flag = false;
	/*for(int i=0;i<select.size();i++)
        {
		if(j == select[i])flag = true;
	}*/
        if(j>-1&&!flag)
        {      select.add(j);
               selected[j]=true;
        }
	//for(int i=0;i<select.size();i++)
        //{
		//cout<<select[i]<<" ";
	//}//cout<<endl;

	return j;
}

void  svLegend::Close(svVector3 rayp1, svVector3 rayp2)
{
        double td, dd;
        td = 9e+9;
        int j =-1;
        svVector3 p;
	for(int i=0;i<N;i++)
        {
            p[0]=-9.75;p[1]=-9.75;p[2]=positions[i];
            if(GetDot(p-rayp1, rayp2-rayp1)>=0.000001)
            {
                 dd = PointtoLine(rayp1, rayp2, p);
                 if(dd < td && dd<2)
                 {
                       j= i ;td = dd;
                 }
            }
        }
	/*if(state == 1 && j>-1)
	{
		if(select.size() > 1)
		{
                        int jj = j - select[select.size()-1];
			if(abs(jj) > 1 && select[select.size()-1] > 0 && select[select.size()-1] < N-1) 
				j = select[select.size()-1] + jj;
		}
	}*/
	bool flag = false;
	/*for(int i=0;i<select.size();i++)
        {
		if(j == select[i])flag = true;
	}*/
        if(j>-1&&!flag)
        {      
             close = j;
        }
	//for(int i=0;i<select.size();i++)
        //{
		//cout<<select[i]<<" ";
	//}//cout<<endl;

}

void svLegend::Process()
{
       for(int i=0;i<select.size();i++)
       {
             if(collapsed[select[i]])
             {
                  Expand(select[i]);
             }
             else
             {
                  Collapse(select[i]);
             }
       }

       updatePositions();
	   select.free();
       for(int i=0;i<N;i++)selected[i] = false;
}

     void svLegend::Render(svScalar alpha) {
         // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // glTranslatef(0, 0,origin);//origin, 0); /* TODO: Change this to glTranslatef(0, 0, origin); */

          for (int n = 0; n < N; n++) {
               svScalar prev = n == 0 ? origin : positions[n-1];
               svScalar delta = positions[n] > prev ?
                    fabs(prev-positions[n]) :
                    -fabs(prev-positions[n]);
               glPushMatrix();
               glTranslatef(-9.75, -9.75,positions[n]);//delta);//delta, 0); /* TODO: Change this to glTranslatef(0, 0, delta); */
//hard code!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
               if (collapsed[n] == SV_COLLAPSED)
                    glColor4f(1-color[0], 1-color[1], 1-color[2], alpha);
               else if (collapsed[n] == SV_EXPANDED)
                    glColor4f(color[0], color[1], color[2], alpha);
               else
                    assert(collapsed[n] == SV_COLLAPSED ||
                           collapsed[n] == SV_EXPANDED);
                if(selected[n] || n==close)
                     RenderCone(radius*2, radius/2., 4);
                else
                     RenderCone(radius, radius/2., 4);
               glPopMatrix();

               if(n%5==0 && n>0)
               {
               glPushMatrix();
               glTranslatef(-9.75-radius*4, -9.75-radius*4,(positions[n]+positions[n-1])/2.);
               glColor4f(1,1,1,1);
                     RenderCone(radius*4,radius/4.,4);
               glPopMatrix();
               }
                //char str[20];
               //sprintf(str, "%d", n);
               //glRasterPos3f(-25.1,-25.1,positions[n]);
               //for(int i=0;i<strlen(str);i++)
                     //glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
               //glutWireSphere(radius, SV_SPHERE_IT, SV_SPHERE_IT); /* TODO: Change this to glutSolidSphere(radius, SV_SPHERE_IT, SV_SPHERE_IT) */
          }


          //glFlush();
          //glutSwapBuffers();
     }

     void svLegend::updatePositions() {

          for(int i=0;i<binindex.size();i++) binindex[i].free();
          binindex.free();
          binpositions.free();
          bincollapsed.free();

	 // svIntArray collapsedbins;
          int countbins = 0;
          svScalar range = end-origin;
          svInt bins = 0;    /* Number of bins */
          svInt segments[N]; /* Number of spheres in each bin */

          for (int n = 0; n < N; n++) {
               if (collapsed[n] == SV_COLLAPSED) {
                    svInt c = 0;
                    svIntArray b;
                    b.add(n);
                    
                    for (int m = n+1; m < N; m++) {
                         if (collapsed[m] == SV_COLLAPSED && c<4) //hard code!!!!!!!!!
                         {
                                b.add(m);
                                c++;
                         }
                         else
                              break;
                    }

                    n += c;
                    binindex.add(b);b.free();
                    segments[bins] = c+1;
                    //collapsedbins.add(1);
		    countbins++;
                    bincollapsed.add(1);
               } else if (collapsed[n] == SV_EXPANDED) {
                    segments[bins] = 1;
                    svIntArray b; b.add(n); binindex.add(b);b.free();
                   // collapsedbins.add(0);
                    bincollapsed.add(0);
               } else {
                    assert(collapsed[n] == SV_COLLAPSED ||
                           collapsed[n] == SV_EXPANDED);
               }

               bins++;
          }
	  svScalar s = range/(N-1);
          svScalar delta1 = s * 5./2.;//half size
          svScalar delta2 = (range - delta1*countbins)/(bins - countbins);

          svScalar delta = range/bins; /* Size of each bin */
          svScalar cursor = origin;    /* Current position */
          svInt pos = 0;

          for (int b = 0; b < bins; b++) {

		if(bincollapsed[b]) delta = delta1;
		else delta = delta2;

               svScalar localCursor = cursor;           /* Current sub-position */
               svScalar localDelta = delta/segments[b]; /* Size of each sub-bin */

               svVector3 p;p[0]=0;p[1]=0;
               if(bincollapsed[b]) p[2] = localCursor;// + localDelta/2;
               else p[2] = localCursor + localDelta/2;

               binpositions.add(p);

               for (int i = 0; i < segments[b]; i++) {
                    svScalar localCenter = localCursor + localDelta/2;

                    positions[pos] = localCenter;
                    pos++;

                    localCursor += localDelta;
               }

               cursor += delta;
          }

	//for(int i=0;i<N;i++)cout<<positions[i]<<endl;
     }
}

