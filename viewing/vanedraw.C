//-------------------------------------------------------
TEveBox* basebox(float A, float B) {
   TEveManager::Create();
   TEveBox* b = new TEveBox;
   b->SetMainColor(kCyan);
   b->SetMainTransparency(0);

   b->SetVertex(0, -A, 0, 446 );
   b->SetVertex(1, -A, 0, 478 );
   b->SetVertex(2, +A, 0, 478 );
   b->SetVertex(3, +A, 0, 446 );
   b->SetVertex(4, -A, B, 446);
   b->SetVertex(5, -A, B, 478);
   b->SetVertex(6, +A, B, 478);
   b->SetVertex(7, +A, B, 446);

   gEve->AddElement(b);
   gEve->Redraw3D(kTRUE);
}

//-------------------------------------------------------
TEveBox* addboxslice(float z, float x, float A, float B, float Y, float Z ) {
   TEveBox* b = new TEveBox;
   b->SetMainColor(kCyan);
   b->SetLineColor(kYellow);
   b->SetMainTransparency(0);
   b->SetVertex(0, -A, B, z );
   b->SetVertex(1, -A, B, z+dz );
   b->SetVertex(2, +A, B, z+dz );
   b->SetVertex(3, +A, B, z );
   b->SetVertex(4, -Y, Z, z);
   b->SetVertex(5, -Y, Z, z+dz);
   b->SetVertex(6, +Y, Z, z+dz);
   b->SetVertex(7, +Y, Z, z);

  // TEveRGBAPalette* pal = new TEveRGBAPalette(0, 130);
  // b->SetPalette(pal);
   gEve->AddElement(b);
}

//-------------------------------------------------------
TEveBox* adddomeslice(float z, float center, float rho ) {
   TGeoSphere* b = new 	TGeoSphere(0, rho, 0, 90, 0, 180);
   gEve->AddElement(b);
}
//-------------------- main ---------------------------
int vanedraw (char *fname="C12") {
 float A=1, B=4, Y=0.75,Z=1, rho;
 float x,z;
 int N=0;
#define dz 0.01;
 basebox(A, B);
 gSystem->Load("libGeom");
 gStyle->SetPalette(1, 0);
 ifstream ifn;
          ifn.open( fname );

   while ( ifn.good() && N<3102 ) {
    ifn >> z >> x >> rho;
    x+=1;
    //cout << "x:"<<x<< "   z:"<<z<< "  rho:"<<rho<<endl; 
    addboxslice(z, x, A, B, Y, B+x+Z);
    N++;
   }
   ifn.close();
   gEve->Redraw3D(kTRUE);

}

