#ifndef __SV_QDOT_IMAGE_H
#define __SV_QDOT_IMAGE_H

#include "svGlyph.h"
#include "svImage.h"

namespace __svl_lib{

class svQDOTImage:public svGlyph{

   public:
     svQDOTImage(svChar *inf);

   virtual void SetImage(svChar *inf);

   virtual ~svQDOTImage(){cleanup();}

   virtual void Generate(MArray<svIntArray> bins, svVector3Array positions, 
              svIntArray collapsed, svScalar separation = 0.25);

   virtual void Render();

  protected:
   virtual void cleanup();

  private:
      svScalar2DArray *imageWidths;
      svScalar2DArray *imageSpacing;     
      svVector3 pSize;
      //svScalar separation; /* Space along x between each plane (in units of a plane) */     
};

}

#endif
