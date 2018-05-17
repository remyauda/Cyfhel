/* Copyright (C) 2012-2017 IBM Corp.
 * This program is Licensed under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. See accompanying LICENSE file.
 */
/* Test_extractDigits.cpp - extracting digits.
 *   For a plaintext space modulo a prime-power $p^e$, extracting
 *   the base-$p$ representation of an encrypted values.
 */
#include <NTL/ZZ.h>
NTL_CLIENT

#include <cstddef>
#include <sys/time.h>
#include <NTL/ZZX.h>
#include <NTL/BasicThreadPool.h>
#include <FHE.h>
#include <timing.h>
#include <EncryptedArray.h>
#include <NTL/lzz_pXFactoring.h>

#include <Cyfhel.h>
#include <CyCtxt.h>

#include <cassert>
#include <cstdio>


// Simple class to measure time for each method
class Timer
{
public:
    void start() { m_start = my_clock(); }
    void stop() { m_stop = my_clock(); }
    double elapsed_time() const {
        return m_stop - m_start;
    }

private:
    double m_start, m_stop;
    double my_clock() const {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
};

static void buildDigitPolynomial(ZZX& result, long p, long e);

void extractDigits(vector<CyCtxt>& digits, const CyCtxt& c, long r=0);

CyCtxt cypherBitsRemy(Cyfhel& cy, CyCtxt& x, CyCtxt& y, long p = 2, long r=0, long m=0, bool noPrint=false, bool dry=false);

// Compute a degree-p polynomial poly(x) s.t. for any t<e and integr z of the
// form z = z0 + p^t*z1 (with 0<=z0<p), we have poly(z) = z0 (mod p^{t+1}).
//
// We get poly(x) by interpolating a degree-(p-1) polynomial poly'(x)
// s.t. poly'(z0)=z0 - z0^p (mod p^e) for all 0<=z0<p, and then setting
// poly(x) = x^p + poly'(x).
static void buildDigitPolynomial(ZZX& result, long p, long e)
{
  if (p<2 || e<=1) return; // nothing to do
  FHE_TIMER_START;
  long p2e = power_long(p,e); // the integer p^e

  // Compute x - x^p (mod p^e), for x=0,1,...,p-1
  vec_long x(INIT_SIZE, p);
  vec_long y(INIT_SIZE, p);
  long bottom = -(p/2);
  for (long j=0; j<p; j++) {
    long z = bottom+j;
    x[j] = z;
    y[j] = z-PowerMod((z < 0 ? z + p2e : z), p, p2e);  // x - x^p (mod p^e)

    while (y[j] > p2e/2)         y[j] -= p2e;
    while (y[j] < -(p2e/2))      y[j] += p2e;
  }
  interpolateMod(result, x, y, p, e);
  assert(deg(result)<p); // interpolating p points, should get deg<=p-1
  SetCoeff(result, p);   // return result = x^p + poly'(x)
  //  cerr << "# digitExt mod "<<p<<"^"<<e<<"="<<result<<endl;
  FHE_TIMER_STOP;
}

// extractDigits assumes that the slots of *this contains integers mod p^r
// i.e., that only the free terms are nonzero. (If that assumptions does
// not hold then the result will not be a valid ciphertext anymore.)
//
// It returns in the slots of digits[j] the j'th-lowest gigits from the
// integers in the slots of the input. Namely, the i'th slot of digits[j]
// contains the j'th digit in the p-base expansion of the integer in the
// i'th slot of the *this. The plaintext space of digits[j] is mod p^{r-j},
// and all the digits are at the same level.
void extractDigits(vector<CyCtxt>& digits, const CyCtxt& c, long r)
{
  FHEcontext& context = (FHEcontext&) c.getContext();
  long rr = c.effectiveR();
  if (r<=0 || r>rr) r = rr; // how many digits to extract

  long p = context.zMStar.getP();
  ZZX x2p;
  if (p>3) {
    buildDigitPolynomial(x2p, p, r);
  }

  CyCtxt tmp(c.getPubKey(), c.getPtxtSpace());
  digits.resize(r, tmp);      // allocate space
  for (long i=0; i<r; i++) {
    tmp = c;
    for (long j=0; j<i; j++) {
      if (p==2) digits[j].square();
      else if (p==3) digits[j].cube();
      else polyEval(digits[j], x2p, digits[j]); // "in spirit" digits[j] = digits[j]^p
      tmp -= digits[j];
      tmp.divideByP();
    }
    digits[i] = tmp; // needed in the next round
  }
}



CyCtxt myGreaterThan(Cyfhel& cy, vector<CyCtxt> binary_x, vector<CyCtxt> binary_y, long nbits){

	vector<long> v1;
	v1.push_back(1);
	CyCtxt c1 = cy.encrypt(v1);

    CyCtxt init = (binary_x[1]*binary_y[1]+binary_x[1])+(binary_x[1]+binary_y[1]+c1)*(binary_x[0]*binary_y[0]+binary_x[0]);
	if(nbits>2){
		for(int i= 2; i<nbits;i++){
			init = (binary_x[i]*binary_y[i]+binary_x[i])+(binary_x[i]+binary_y[i]+c1)*init;
		}
	}
return init;
}

CyCtxt myGreaterThan2(vector<CyCtxt> binary_x, vector<CyCtxt> binary_y){
    return (binary_x[1]*binary_y[1]+binary_x[1])+(binary_x[1]+binary_y[1]+1)*(binary_x[0]*binary_y[0]+binary_x[0]);
}

CyCtxt myGreaterThan3(vector<CyCtxt> binary_x, vector<CyCtxt> binary_y){
    return (binary_x[2]*binary_y[2]+binary_x[2])+(binary_x[2]+binary_y[2]+1)*((binary_x[1]*binary_y[1]+binary_x[1])+(binary_x[1]+binary_y[1]+1)*(binary_x[0]*binary_y[0]+binary_x[0]));
}



CyCtxt cypherBitsRemy(Cyfhel& cy, CyCtxt& x, CyCtxt& y, long p, long r, long m, bool noPrint, bool dry){

  long p2r = power_long(p,r); // p^r

  vector<long> pDigits;
  pDigits = cy.decrypt(x);
  if (cy.getm_encryptedArraySize()<=20 && !noPrint)
    cout << "plaintext="<<pDigits<<endl;

  if (!noPrint)
    cout << "extracting "<<r<<" digits..." << std::flush;
  vector<CyCtxt> digits;
  extractDigits(digits, x);
  if (!noPrint)
    cout << " done\n" << std::flush;

  long pp = p2r;
  for (long i=0; i<(long)digits.size(); i++) {
    if (!digits[i].isCorrect()) {
      cout << " potential decryption error for "<<i<<"th digit ";
      CheckCtxt(digits[i], "");
      exit(0);
    }

  pDigits = cy.decrypt(digits[i]);
    pDigits.resize(1);
    if (cy.getm_encryptedArraySize()<=20 && !noPrint)
      cout << i << "th digit="<<pDigits<<endl;

    pp /= p;
  }
  cout << "digit extraction successful\n\n";


  // Here: digits is a vector of CyCtxt that contain encrypted bits of plaintext.


  //******************************

  vector<long> pDigits2;

  pDigits2 = cy.decrypt(y);
  if (cy.getm_encryptedArraySize()<=20 && !noPrint)
    cout << "plaintext="<<pDigits2<<endl;

  if (!noPrint)
    cout << "extracting "<<r<<" digits..." << std::flush;
  vector<CyCtxt> digits2;
  extractDigits(digits2, y);
  if (!noPrint)
    cout << " done\n" << std::flush;

  long pp2 = p2r;
  for (long i=0; i<(long)digits2.size(); i++) {
    if (!digits2[i].isCorrect()) {
      cout << " potential decryption error for "<<i<<"th digit ";
      CheckCtxt(digits2[i], "");
      exit(0);
    }

	// Here: digits is a vector of CyCtxt that contain encrypted bits of plaintext.

    pDigits2 = cy.decrypt(digits2[i]);
    pDigits2.resize(1);
    if (cy.getm_encryptedArraySize()<=20 && !noPrint)
      cout << i << "th digit="<<pDigits2<<endl;

    pp2 /= p;
  }
  cout << "digit extraction successful\n\n";

  // Here: digits2 is a vector of CyCtxt that contain encrypted bits of plaintext.

  //******************************

  Timer tMethod2;
  tMethod2.start();
  CyCtxt max = myGreaterThan(cy, digits, digits2, r);
  tMethod2.stop();

  return max;
}



static bool noPrint = false;

int main(int argc, char *argv[])
{
  ArgMapping amap;
  long x = 16;
  long y = 4;
  long p = 2;
  long r=0;
  long m = 0;
  bool dry = false;

  amap.arg("x", x, "value x");
  amap.arg("y", y, "value y");
  amap.arg("p", p, "plaintext base");
  amap.arg("r", r, "lifting");
  amap.arg("m", m, "the cyclotomic ring", "heuristic");
  amap.arg("noPrint", noPrint, "suppress printouts");
  amap.arg("dry", dry, "dry=1 for a dry-run");

  // get parameters from the command line
  amap.parse(argc, argv);

  if (p<2) exit(0);
  double lBound = (double)FHE_p2Bound;
  if (lBound>40.0) lBound=20.0;
  long bound = floor(lBound/log2((double)p));
  if (r<2 || r>bound) r = bound;
  //long p2r = power_long(p,r); // p^r

  long ll = NextPowerOfTwo(p);
  long L = r*ll*3 +2; // how many levels do we need
  m = p+1; // FindM(/*secparam=*/80, L, /*c=*/4, p, /*d=*/1, 0, m);
  setDryRun(dry);

  if (!noPrint) {
    if (dry) cout << "dry run: ";
    cout << "m="<<m<<", p="<<p<<", r="<<r<<", L="<<L<<endl;
  }

  /********************************************/

  Cyfhel cy(true, p, r, 4, 1, 128, 64, L, m);

  /********************************************/

  // Create a vector of random long that we want to know the max.
  vector<long> v1;
  for(int i=0; i<10 ; i++){
      long r1 = (rand() % 10) + 1;
      cout<<r1<<endl;
      v1.push_back(r1);
  }

  // Cypher each element of v1.
  vector<CyCtxt> c1;
  for(int i=0; i<10 ; i++){
      vector<long> tmp;
      tmp.push_back(v1[i]);
      c1.push_back(cy.encrypt(tmp));
  }

  vector<CyCtxt> vectc2;

  vector<long> v3;
  v3.push_back(1);

  cout << "step1"<<endl;
  vector<long> vtest;
  vector<long> vtest2;

  for(int i=0; i<10; i++){
      // c2 is initialize as Encrypt(1).
      CyCtxt c2 = cy.encrypt(v3);
      int j= 0;
      cout << "step2"<<endl;
      while(j!=10){
		  cout << "step3"<<endl;
          if(i!=j){
			 cout << "step4"<<endl;
             //c2 *= cypherBitsRemy(c1[i], c1[j], p, r, m, noPrint, dry);
			 CyCtxt maxij = cypherBitsRemy(cy, c1[i], c1[j], p, r, m, noPrint, dry);
			 cout << "step5"<<endl;
			 vtest = cy.decrypt(maxij);
			 vtest.resize(1);
			 cout <<"maxij = "<<vtest<<endl;
			 c2 *= maxij;
			 cout << "step6"<<endl;
			 vtest2 = cy.decrypt(c2);
			 cout <<"c2 = "<<vtest2<<endl;
              }
          j++;
          }
          vectc2.push_back(c2);
		  cout << "step7"<<endl;
  }

  vector<long> result;
  for(unsigned int i=0; i<vectc2.size(); i++){
    vector<long> tmp = cy.decrypt(vectc2[i]);
    long tmp2 = tmp[0];
    result.push_back(tmp2);
  }

  cout << "result="<<result<<endl;


  /*
  vector<long> result;
  result = cypherBits(x, y, p, r, m, noPrint, dry);
  result.resize(1);

  cout << "result="<<result<<endl;
  if(result[0]){
    cout << "x is greater than y."<<endl;
    cout << "max="<<x<<endl;
  }
  else{
cout << "y is greater than x."<<endl;
    cout << "max="<<y<<endl;
  }
  * */
  // Here: digits is a vector of CyCtxt that contain encrypted bits of plaintext.


} 
