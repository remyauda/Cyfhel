#ifndef DEF_CYCTXT
#define DEF_CYCTXT

#include "Ctxt.h"

//The CyCtxt Class extend the Ctxt Class
class CyCtxt: public Ctxt {

 private:

	/******ATTRIBUTES******/
	

	/******PROTOTYPES OF PRIVATE METHODS******/

        
	/******COMPARISON OPERATORS OVERLOAD******/


	/******STREAM OPERATORS OVERLOAD******/
	friend std::ostream& operator<< (std::ostream& flux, CyCtxt const& cy);
	friend std::istream& operator<< (std::istream& in, CyCtxt& cy);
	


 public:

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	CyCtxt(FHEPubKey const& newPubKey, long newPtxtSpace=0): Ctxt(newPubKey, newPtxtSpace){}//Constructor


	/******DESTRUCTOR BY DEFAULT******/
	

	/******GETTERS******/
	
	/******SETTERS******/
	

       
	/******PROTOTYPES OF PUBLIC METHODS******/
	CyCtxt square() const;
	CyCtxt cube() const;

	/******PROTOTYPES OF PUBLIC METHODS: SHORTCUT OPERATORS OVERLOAD******/



};

	/******ARITHMETIC OPERATORS OVERLOAD******/
	CyCtxt operator+(CyCtxt const& cy1, CyCtxt const& cy2);
	CyCtxt operator-(CyCtxt const& cy1, CyCtxt const& cy2);
	CyCtxt operator*(CyCtxt const& cy1, CyCtxt const& cy2);


#endif
