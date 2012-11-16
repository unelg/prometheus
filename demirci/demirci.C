#include <iostream>
#include <fstream>
#include <math.h>
#include <gsl/gsl_sf_bessel.h>
using namespace std;
int dump2file (int nbcells, double RFfreq, double Ein, double *V, double *A, double *phi, double *a, double *m, double *R, double *rho, double *cl ) ;

int main () {

// constants
double lightSpeed=299792458; // m/s
double protonMass=938.272;  // MeV

// input values --- please change the code to read them from an input file
double RFfreq=352.21; // MHZ
double Ein=25; // kV
double Vo=200; // kV
const int nbcells=612;
int Nbunch=61;
int Ninit=400;
int Nacc=nbcells-Nbunch-Ninit;

// calculated values
double lambda=lightSpeed/RFfreq/1E4; //cm
double cbeta0, cetot0, ttime;

// temporary variables
double cbeta, cetot, tmp, egain;

// needed quantities
double V[nbcells], phi[nbcells], a[nbcells],  m[nbcells], R[nbcells], rho[nbcells];
double beta[nbcells], A[nbcells], A10[nbcells], X[nbcells]; 
double cell_lenght[nbcells], etot[nbcells], k[nbcells];
double z[nbcells];
double B,EoT;

cout << "~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
cout << "Welcome to  Demirci v0.1" << endl;
cout << "~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
cout << "We operate at "<< RFfreq <<" MHz,   corresponding to lambda="<<lambda<< " cm."<<endl;
cetot0=Ein/1e3 + protonMass;
cbeta0=sqrt(1-pow(protonMass/cetot0,2));

cout << "The initial energy is "<< Ein<< " kV,  corresponding to beta=" <<cbeta0<<endl; 
 beta[0]=cbeta0;
 etot[0]=cetot0;
 cell_lenght[0]=beta[0]*lambda/2;
 z[0]=cell_lenght[0];
 rho[0]=0.351;
 R[0]=0.5*(m[0]+1)*a[0];
 egain=0;
 ttime=0;
 int nocells=0;
 ifstream fin;
 fin.open("dmr.in");
 for (int n=0; n<nbcells; n++) {
  fin>>V[n] >> phi[n] >> a[n]>>  m[n];
 // cout << "V="<<V[n]<<endl;
  V[n]=100.0; // force same voltage for a test
  if (n<=Nbunch) phi[n]=-90;
  phi[n]*=M_PI/180;  // conversion to radian
 }
cout << "Geometrical variables read"<<endl;


for (int n=1; n<nbcells; n++) {
 etot[n]=etot[n-1]+egain/1E3;
 beta[n]=sqrt(1-pow(protonMass/etot[n],2));
 cell_lenght[n]=beta[n]*lambda/2;
 rho[n]=rho[0];
 R[n]=0.5*(m[n]+1)*a[n];
 z[n]=z[n-1]+cell_lenght[n]; // in cm
 tmp=1E9*(cell_lenght[n]/100)/(beta[n]*lightSpeed); // in ns
 ttime+=tmp;
 k[n]=2*M_PI/(lambda*beta[n]); // cm
 A[n]=(m[n]*m[n]-1) / (m[n]*m[n]*gsl_sf_bessel_I0(k[n]*a[n]) + gsl_sf_bessel_I0(k[n]*a[n]*m[n]) );
 A10[n]=0.5*Vo*A[n]; //??
 X[n]=(gsl_sf_bessel_I0(k[n]*a[n]) + gsl_sf_bessel_I0(k[n]*a[n]*m[n]))/(m[n]*m[n]*gsl_sf_bessel_I0(k[n]*a[n]) + gsl_sf_bessel_I0(k[n]*a[n]*m[n]));
 nocells=n;
 B=(1/protonMass)*(V[n]/a[n])*(1/(RFfreq*RFfreq))*(1/a[n])*X[n]; // this is focusing strength
 EoT=A[n]*V[n]*k[n]/4; // this is accelerating efficiency
 
 if (etot[n] >(protonMass+1)) { cout << "1MeV reached in "<<n<<" cells."<<endl; break;}
 egain=0.25*M_PI*A[n]*V[n]*cos(phi[n])*gsl_sf_bessel_I0(k[n]*a[n]);
 //if (n<10 || n>600) cout << "n:"<<n<<" W:"<<egain<< " ak:"<< a[n]*k[n]<< " A:" << A[n]<< "  beta:"<<beta[n]<<endl;

 double U=0.5*Vo*( X[n]*a[n]*a[n]*cos(M_PI) + A[n]*gsl_sf_bessel_I0(k[n]*a[n])*cos(k[n]*z[n])  )*sin(ttime*2*M_PI*RFfreq/1E3 + phi[n]);

#ifdef __DEBUG__
 cout << "n:"<<n<<"  U="<<fabs(U) << " V="<<V[n]<< " T:"<<sin(ttime*2*M_PI*RFfreq/1E3 + phi[n])<< " time:"<<ttime<<endl;
 cout << "Focus:"<<X[n] << "   Accel:"<< A[n]*gsl_sf_bessel_I0(k[n]*a[n])*cos(k[n]*z[n])<<endl;
#endif

}
cout << "Final total energy reached:"<<etot[nocells]<< " MeV   which is a gain of:"<<etot[nocells]-protonMass<< " MeV"<<endl;
cout << "at a total length of "<< z[nocells]/100<<" m. in "<<ttime<< " ns." << endl; 

cout << "preparing ouputfile ... ";
int rv=dump2file (nocells, RFfreq, Ein, V, A, phi, a, m, R, rho, cell_lenght);
if (  !rv) {cout << "ok."; } else {cout <<"fails";}; cout << endl;  

return 0;
}

int dump2file (int nbcells, double RFfreq, double Ein, double *V, double *A, double *phi, double *a, double *m, double *R, double *rho, double *cl ) {
   ofstream fout;
   fout.open("dmr.out"); 

   fout << "RHO 0.85"<<endl;
   fout << "linac 1 "<< Ein/1E3<<" "<< RFfreq<<" 1.007276 1.0"<<
   fout << "trancell"<<endl;
   fout << "{ } Tank 1 Length= ll cm, "<<nbcells-1<<" cells"<<endl;
   fout << "Cell   cl      tl     curlt   curll"<<endl;

  for (int n=0; n<nbcells; n++) {
   fout << n<< "  "<< V[n]<< " ";
   fout << " 0.0  0.0   0.0  ";
   fout << A[n]<< "  " << phi[n]*180/M_PI<< "  "<< a[n]<< "  ";
   fout << m[n]<< "  " << R[n]<< "  "<< rho[n]<< "  ";
   fout <<" 0.0  " << cl[n]<< "  ";
   fout << " 0.0  0.0   0.0  "<<endl;
  }
  fout << "rfqout 1"<<endl;
  fout.close();
 return 0;
}
