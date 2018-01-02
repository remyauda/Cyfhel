#ifndef DEF_CYCTXT
#define DEF_CYCTXT

#include "Ctxt.h"

//The CyCtxt Class extend the Ctxt Class
class CyCtxt: public Ctxt {

 private:

    /******ATTRIBUTES******/
	
        


    /******PROTOTYPES OF PRIVATE METHODS******/
	


 public:

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	CyCtxt(FHEPubKey const& newPubKey, long newPtxtSpace=0): Ctxt(newPubKey, newPtxtSpace){}//Constructor


	/******DESTRUCTOR BY DEFAULT******/
	

    /******GETTERS******/
	
	/******SETTERS******/
	

       
    /******PROTOTYPES OF PUBLIC METHODS******/



	//------AUXILIARY------

    
	//------I/O------


        
    /******OPERATORS OVERLOAD******/


};

#endif
