#ifndef DEF_CYCTXT
#define DEF_CYCTXT

#include "Ctxt.h"

//The CyCtxt Class extend the Ctxt Class
class CyCtxt: public Ctxt {

 private:

	/******ATTRIBUTES******/
	long m_sizeOfPlaintext;

	/******PROTOTYPES OF PRIVATE METHODS******/

        
	/******COMPARISON OPERATORS OVERLOAD******/


	/******STREAM OPERATORS OVERLOAD******/
	friend std::ostream& operator<< (std::ostream& flux, CyCtxt const& cy);
	friend std::istream& operator<< (std::istream& in, CyCtxt& cy);
	


 public:

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	CyCtxt(FHEPubKey const& newPubKey, long sizeOfPlaintext=0, long newPtxtSpace=0): Ctxt(newPubKey, newPtxtSpace), m_sizeOfPlaintext(sizeOfPlaintext){}//Constructor


	/******DESTRUCTOR BY DEFAULT******/
	

	/******GETTERS******/
	long getm_sizeOfPlaintext() const;//Getter of attribute m_sizeOfPlaintext
	
	/******SETTERS******/
	void setm_sizeOfPlaintext(long sizeOfPlaintext);//Setter of attribute m_sizeOfPlaintext

       
	/******PROTOTYPES OF PUBLIC METHODS******/
	CyCtxt returnSquare() const;
	CyCtxt returnCube() const;

	/******PROTOTYPES OF PUBLIC METHODS: SHORTCUT OPERATORS OVERLOAD******/



};

	/******ARITHMETIC OPERATORS OVERLOAD******/
	CyCtxt operator+(CyCtxt const& cy1, CyCtxt const& cy2);
	CyCtxt operator-(CyCtxt const& cy1, CyCtxt const& cy2);
	CyCtxt operator*(CyCtxt const& cy1, CyCtxt const& cy2);


#endif
