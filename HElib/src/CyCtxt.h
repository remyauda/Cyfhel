#ifndef DEF_CYCTXT
#define DEF_CYCTXT

#include "FHE.h"
#include "EncryptedArray.h"
#include "Ctxt.h"

//The CyCtxt Class extend the Ctxt Class
class CyCtxt: public Ctxt {

 private:

	/******ATTRIBUTES******/
	long m_sizeOfPlaintext;
	FHEPubKey *m_publicKey;// Public key of the public-secret key pair
	EncryptedArray *m_encryptedArray;// Array used for encryption
	long m_numberOfSlots;// Nº of slots in scheme

	/******PROTOTYPES OF PRIVATE METHODS******/
	CyCtxt encrypt(vector<long> &ptxt_vect) const;//Encryption
        
	/******COMPARISON OPERATORS OVERLOAD******/


	/******STREAM OPERATORS OVERLOAD******/
	friend std::ostream& operator<< (std::ostream& flux, CyCtxt const& cy);
	friend std::istream& operator>> (std::istream& in, CyCtxt& cy);
	


 public:

	/******CONSTRUCTOR BY DEFAULT******/


	/******CONSTRUCTOR WITH PARAMETERS******/
	CyCtxt(FHEPubKey const& newPubKey, long sizeOfPlaintext=0, long newPtxtSpace=0): Ctxt(newPubKey, newPtxtSpace), m_sizeOfPlaintext(sizeOfPlaintext){}//Constructor


	/******DESTRUCTOR BY DEFAULT******/
	

	/******GETTERS******/
	long getm_sizeOfPlaintext() const;//Getter of attribute m_sizeOfPlaintext
	
	/******SETTERS******/
	void setm_sizeOfPlaintext(long sizeOfPlaintext);//Setter of attribute m_sizeOfPlaintext

	void setm_publicKey(FHEPubKey *publicKey);//Setter of attribute m_publicKey

	void setm_encryptedArray(EncryptedArray *encryptedArray);//Setter of attribute m_encryptedArray

	void setm_numberOfSlots(long numberOfSlots);//Setter of attribute m_numberOfSlots

       
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
