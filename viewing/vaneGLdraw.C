#define dz 0.01;
//-------------------------------------------------------
TNode* basebox(float A, float B) {

   TBRIK *brik  = new TBRIK("BRIK","BRIK","void",A,B/2,16);
          brik->SetLineColor(1);
//  Build the geometry hierarchy
   TNode *node1 = new TNode("NODE1","NODE1","BRIK",0,B/2,446);
   node1->cd();
   return node1;
}

//-------------------------------------------------------
void addboxslice(float z, float x, float A, float B, float Y, float Z, TNode *mainnode, int N ) {
   TString trapn="TRAP";
   TString noden="NODE";
           trapn+=N;
           noden+=N;
   TTRAP *trap = new TTRAP(trapn, trapn, "void", dz/2,0,0, (Z+x)/2,A,Y,0, (Z+x)/2,A,Y,0    );
          trap->SetLineColor(2);

   mainnode->cd();
   TNode *node2  = new TNode(noden,noden,trapn, 0, B/2+Z/2 +x/2, 446/2-z/2); // what is the 0.5?
}

//-------------------------------------------------------
void adddomeslice(float z, float x, float rho, float B, float Z, TNode *mainnode, int N ) {
   TString tubsn="TUBS";
   TString noden="NODE";
           tubsn+=N;
           noden+=N;
 TTUBS *tubs = new TTUBS(tubsn,tubsn,"void",0,rho, dz/2,0,180);
        tubs->SetLineColor(4);
   mainnode->cd();
   TNode *node3  = new TNode(noden,noden,tubsn, 0, B/2+Z +x, 446/2-z/2);
}
//-------------------- main ---------------------------
int vaneGLdraw (char *fname="C12") {
 float A=1, B=6, Z=1, rho;
 float x,z;
 int N=0;
  TCanvas *c1=new TCanvas();
  TNode *mainnode = new TNode(); 
         mainnode = basebox(A, B);
 gSystem->Load("libGeom");
 gStyle->SetPalette(1, 0);
 ifstream ifn;
          ifn.open( fname );

   while ( ifn.good() && N<3102 ) {
    ifn >> z >> x >> rho;
    x+=1;
    //cout << "x:"<<x<< "   z:"<<z<< "  rho:"<<rho<<endl; 
    addboxslice(z, x, A, B, rho, Z, mainnode, N);
    adddomeslice(z, x, rho, B, Z, mainnode, N);
    N++;
   }
   ifn.close();
   mainnode->Draw("gl");
   c1->Update();

}
