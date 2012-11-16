#include <iostream>
#include <fstream>

int read1line(TString option="x=0.,y=0. &", double *xr, double *yr){
 Ssiz_t from = 0 ;
 TString act;
 TString act2;
 double xp, yp, x0, y0;

 int pair=0, cnt=0;
 //cout << "1:"<<option <<endl;
 option.Tokenize(act, from, " ");
 TSubString interest=act.SubString("&po", 0);
 if (interest.IsNull() ) { 
 //     cout<<"reject"<<endl; 
    return 0;
 }
 option.Tokenize(act, from, " ");
 
 option=act; from=0;
 //cout << "2:"<<option <<endl;
 do {
    if (!option.Tokenize(act, from, ",")) {
     // cout  << "finished parsing line params:"<< endl;
      break;
    }
   //cout << "ACT:"<<  act << " from:"<<from<<endl;

   TSubString xstring=act.SubString("x=", 0);
   if  (!xstring.IsNull() )  {                    // x string
             act.Tokenize(act2, 1, "="); xp=act2.Atof();
             if (pair!=2) pair=1;
    //         cout << "Xf is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString ystring=act.SubString("y=", 0);
   if  (!ystring.IsNull() )  {                    // y string
             act.Tokenize(act2, 1, "="); yp=act2.Atof();
   //          cout << "Xf is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString ntstring = act.SubString("nt=", 0);
   if  ( !ntstring.IsNull() )  {                    // arc string
    pair=2; //this should be read from the string
    //cout << "This will be an arc:"<< ntstring <<endl;
   }
   TSubString x0string = act.SubString("x0=", 0);
   if  ( !x0string.IsNull() )  {                    // x string
             act.Tokenize(act2, 2, "="); x0=act2.Atof();
    //         cout << "X0 is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }

   TSubString y0string = act.SubString("y0=", 0);
   if  ( !y0string.IsNull() )  {                    // y string
             act.Tokenize(act2, 2, "="); y0=act2.Atof();
   //          cout << "Y0 is:"<< act2<<"value:" <<act2.Atof() <<endl;
   }
 } while ( 1 );

   if (pair==1) {
       xr[0]=xp; yr[0]=yp;
  //     cout  <<"Xp:"<<xp<< "  Yp:"<<yp<<endl;
   }
   if (pair==2) {
       xr[0]=x0; yr[0]=y0;
       xr[1]=xp; yr[1]=yp;
   //    cout  <<"X0:"<<x0<< " Y0:"<<y0<<" Xp:"<<xp<< "  Yp:"<<yp<<endl;
   }
    return pair;
}// end of read1line

int sfishdraw (TString filename){
TCanvas *c1 = new TCanvas ("c1", "c1", 700, 700);
     gStyle->SetOptStat(0);
TH2F *hpf = new TH2F ("hpf", " :) " , 40, -10, 10, 40, -10, 10);
      hpf->Draw();
double x[9],y[9], xi=0.0, yi=0.0;
int ip, retval;
char aline[256];
ifstream fin;
         fin.open(filename);
         do {
           fin.getline(aline, 256);
          // cout << "R:"<<aline<<endl;
           retval=read1line(aline, x, y); 
           if (retval==1) {  // we received a line
             TLine *tl= new TLine(xi, yi, x[0], y[0]); tl->Draw();
             xi=x[0]; yi=y[0];
     //        cout << "did a Line"<<endl;
           }
           if (retval==2) {  // we received an arc
            double radius = sqrt( x[1]*x[1] + y[1]*y[1]);
            double thetai=180*atan(y[1]/x[1])/3.14159;
            double thetaf=180*atan((yi-y[0])/(xi-x[0]))/3.14159;
            if (thetaf==0) {thetaf=180;} 
            if (y[1]<0 && x[1]<=0) { 
                 thetaf-=180;
            } 
            if (y[1]<=0 && x[1]<0) { 
                 thetaf+=260;
                 thetai+=100;
            }
            xi=x[0]+x[1];
            yi=y[0]+y[1];
            TCrown *ar= new TCrown (x[0], y[0], radius, radius, thetai, thetaf); ar->SetLineColor(4); ar->Draw();
            //TArc *ar= new TArc (x[0], y[0], radius, thetai, thetaf); ar->Draw();
       //      cout << "did an Arc "<<endl;
           }
           c1->Update();
         } while (!fin.eof()) ;
}

