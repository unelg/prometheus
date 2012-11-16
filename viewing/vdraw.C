int vdraw ( char *fname="C12", int ncell=3102){

   gROOT->Reset();
   c1 = new TCanvas("c1","PolyLine3D & PolyMarker3D Window",200,10,700,500);

   // create a pad
   p1 = new TPad("p1","p1",0.05,0.02,0.95,0.82,4,2,1);
   p1->Draw();
   p1->cd();

   // creating a view
   view = TView::CreateView(1);
   view->SetRange(0.0,0,446,5, 10,478);

   float A=1,B=4,C, Y=0.75,Z=1, rho;
   float x,z;
   int N=0;
   TPolyLine3D *pl3d1 = new TPolyLine3D(9*ncell);

   ifstream ifn;
            ifn.open( fname );

   while ( ifn.good() && N<3102*9) {
    ifn >> z >> x >> rho;
    x+=1;
    //cout << "x:"<<x<< "   z:"<<z<< "  rho:"<<rho<<endl; 
    pl3d1->SetPoint(N,  0, 0,         z); N++;
    pl3d1->SetPoint(N, -A, 0,         z); N++;
    pl3d1->SetPoint(N, -A, B+x,       z); N++;
    pl3d1->SetPoint(N, -Y, B+x+Z,     z); N++;
    pl3d1->SetPoint(N,  0, B+x+Z+rho, z); N++;
    pl3d1->SetPoint(N,  Y, B+x+Z,     z); N++;
    pl3d1->SetPoint(N,  A, B+x,       z); N++;
    pl3d1->SetPoint(N,  A, 0,         z); N++;
    pl3d1->SetPoint(N,  0, 0,         z); N++;
   }
   ifn.close();

   pl3d1->SetLineWidth(1);
   pl3d1->SetLineColor(5);

   pl3d1->Draw();
}
