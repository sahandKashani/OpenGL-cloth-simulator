//=============================================================================
//
//   Exercise code for Introduction to Computer Graphics
//   LGG - EPFL
//
//=============================================================================
//=============================================================================
//
//   CLASS Light
//
//=============================================================================


#ifndef LIGHT_HH
#define LIGHT_HH


//== INCLUDES =================================================================

#include "Object3D.h"

//== CLASS DEFINITION =========================================================

/*
     Directional Light
*/

class Light3D : public Object3D {

    public: //------------------------------------------------ public methods
    
        Light3D(): Object3D() {}
        virtual ~Light3D() {}
        
    private: //------------------------------------------------------- private data
    
};


//=============================================================================
#endif // LIGHT_HH defined
//=============================================================================
