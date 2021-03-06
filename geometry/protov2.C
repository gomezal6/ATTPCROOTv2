// Author Y. Ayyad (RCNP Osaka Univ.)
// ATTPC Prototype vanilla macro to generate the pad geometry
// 27 - 03 -2015
// Note: This macro must be run in compiled mode i.e.: .L protov2.C+




#if defined(__CINT__) && !defined(__MAKECINT__)
{
   
   std::cout<<"protov2.C has to be run in compiled mode ... doing this for you."<<std::endl;
   gSystem->CompileMacro("protov2.C");
   protov2();
}
#else

#include "TCanvas.h"
#include "TH2Poly.h"
#include "TMath.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include <iostream>
#include "TFile.h"
#include <fstream>


void protov2(){
    
    

    static Int_t n_strips = 32;
	std::cout<<" Prototype micromegas geometry "<<std::endl;
	Float_t x_r1[128][1820]={0.0};
	Float_t x_r2[128][1820]={0.0};
        Float_t y_r1[128][1820]={0.0};
	Float_t y_r2[128][1820]={0.0};

	Float_t x_r1c[128]={0.0};
	Float_t x_r2c[128]={0.0};
	Float_t y_r1c[128]={0.0};
	Float_t y_r2c[128]={0.0};

	Float_t r1=0.0;
	Float_t r2=0.0;
	Float_t r1p=0.0;
	Float_t r2p=0.0;
	Float_t phi=0.0;
    Float_t rc=0.0;
    Float_t x_rc[3600]={0.0};
    Float_t y_rc[3600]={0.0};
	Int_t i, bin;
	Float_t LL = 1.0;//mm
	Float_t SS = 0.5;//mm
	Float_t DD = 0.25; //mm

  Int_t bin_num[257]={0};
    
    TMultiGraph *mg1[128];
    TMultiGraph *mg2[128];
    TMultiGraph *mgc;
    TGraph *sec1[128];
    TGraph *sec2[128];
    TGraph *secc;
    TGraph *sec_center;
    TGraph *sec_center_2;
    
   // std::stringstream sector1_name;
    Char_t sector1_name[256];
    Char_t sector2_name[256];
    
    TH2Poly *h2p = new TH2Poly();
    h2p->SetName("ATTPC_Proto");
    h2p->SetTitle("ATTPC_Proto");
    
    //TCanvas *c1 = new TCanvas("c1", "c1");
    //c1->Draw();
    
    TFile *file = new TFile ("proto_geo_hires.root","RECREATE");
   
     ofstream map;
     map.open ("map.dat");
     


    
    std::cout<<" Building Up-Right sector "<<std::flush;

  for(Int_t k=0;k<n_strips;k++){
      
      if(k<5) LL = 1.0;
       else LL=3.2;
    
	for(Int_t j=0;j<900;j++){
		
        if(j%900==0) std::cout<<"."<<std::flush;
		phi = (j/10.0)*(TMath::Pi()/180.0);
		if(k<5) r1= 1.34 + k*(LL+SS+DD+DD) ; // mm
     		   else r1= 1.34 + k*(LL+SS+DD+DD) -11.0; //Take the offset into account
       		 r2= r1 + LL + SS +DD;
		r1p = r1 + LL - (phi*(LL-SS)/(TMath::Pi()/2.0));
		x_r1[k][j] = r1 * TMath::Cos(phi);
		y_r1[k][j] = r1 * TMath::Sin(phi);
		x_r2[k][j] = r2 * TMath::Cos(phi);
		y_r2[k][j] = r2 * TMath::Sin(phi);

                if(j==450){
			 x_r1c[k] = x_r1[k][j] + (LL/2.0*TMath::Cos(phi));
			 y_r1c[k] = y_r1[k][j];
			 x_r2c[k] = x_r2[k][j];
			 y_r2c[k] = y_r2[k][j] - (LL/2.0*TMath::Sin(phi));
			 x_r1c[k+32] = x_r1c[k];
			 y_r1c[k+32] = -y_r1c[k];
			 x_r2c[k+32] = x_r2c[k];
			 y_r2c[k+32] = -y_r2c[k];
			 x_r1c[k+64] = -x_r1c[k];
			 y_r1c[k+64] = y_r1c[k];
			 x_r2c[k+64] = -x_r2c[k];
			 y_r2c[k+64] = y_r2c[k];
			 x_r1c[k+96] = -x_r1c[k];
			 y_r1c[k+96] = -y_r1c[k];
			 x_r2c[k+96] = -x_r2c[k];
			 y_r2c[k+96] = -y_r2c[k];
			 

		}    
        //Dirty way to force the gap
       // y_r1[k][j] += DD/2.0;
       // y_r2[k][j] += DD/2.0;
		
	}
	
	  
	for(Int_t j=0;j<10;j++) {
		
		x_r1[k][900+j] = x_r1[k][899];
		y_r1[k][900+j] = SS*(j/10.0) + y_r1[k][899];
		x_r2[k][900+j] = x_r2[k][899];
		y_r2[k][900+j] = y_r2[k][899] - LL*(j/10.0);
        
        //Dirty way to force the gap
       // y_r1[k][900+j] += DD/2.0;
       // y_r2[k][900+j] += DD/2.0;
		
	}
	
	
	for(Int_t j=0;j<900;j++){
        
		
		phi = (j/10.0)*(TMath::Pi()/180.0); 
        r1p = r1 + LL - (phi*(LL-SS)/(TMath::Pi()/2.0));
		r2p = r2 - SS - (phi*(LL-SS)/(TMath::Pi()/2.0));
		x_r1[k][1809-j] = r1p * TMath::Cos(phi);
		y_r1[k][1809-j] = r1p * TMath::Sin(phi);
		x_r2[k][1809-j] = r2p * TMath::Cos(phi);
		y_r2[k][1809-j] = r2p * TMath::Sin(phi);
        
        //Dirty way to force the gap
       // y_r1[k][1809-j] += DD/2.0;
       // y_r2[k][1809-j] += DD/2.0;
 
    }
	
	
	for(Int_t j=0;j<10;j++) {
		
		x_r1[k][1810+j] = x_r1[k][1809]- LL*(j/10.0);
		y_r1[k][1810+j] = y_r1[k][1809];
		x_r2[k][1810+j] = SS*(j/10.0) + x_r2[k][1809];
		y_r2[k][1810+j] = y_r2[k][1809];
        
        //Dirty way to force the gap
       // y_r1[k][1810+j] += DD/2.0;
       // y_r2[k][1810+j] += DD/2.0;

		
	} 
	
	x_r1[k][1819]=x_r1[k][0];
	y_r1[k][1819]=y_r1[k][0];
	x_r2[k][1819]=x_r2[k][0];
	y_r2[k][1819]=y_r2[k][0];
      
      
    
      

      
  
       sprintf(sector1_name,"sector1_%d",k);
       sprintf(sector2_name,"sector2_%d",k);
      // cout<<endl;
      // cout<<" Sector 1 name : "<<sector1_name<<endl;
      // cout<<" Coordinates - X : "<< x_r1c[k]<<"  Y :"<< y_r1c[k]<<endl;
      // cout<<" Sector 2 name : "<<sector2_name<<endl;
      // cout<<" Coordinates - X : "<< x_r2c[k]<<"  Y :"<< y_r2c[k]<<endl;
      //sector1_name<<"sector1_"<<k;
  // std::cout<<sector1_name.str()<<std::endl;
  //map<<sector1_name<<"	"<<x_r1c[k]<<"	"<<y_r1c[k]<<std::endl;
	//map<<sector2_name<<"	"<<x_r2c[k]<<"	"<<y_r2c[k]<<std::endl;
        //map<<endl;
	//map << "Writing this to a file.\n";
    
    mg1[k] = new TMultiGraph(sector1_name,sector1_name);
    sec1[k]= new TGraph(1820,x_r1[k],y_r1[k]);
    sec_center = new TGraph(128,x_r1c,y_r1c);
    sec_center ->SetMarkerSize(0.4);
    sec_center ->SetMarkerStyle(20);
    sec_center ->SetMarkerColor(kRed);
    sec_center_2 = new TGraph(128,x_r2c,y_r2c);
    sec_center_2 ->SetMarkerSize(0.4);
    sec_center_2 ->SetMarkerStyle(20);
    sec_center_2 ->SetMarkerColor(kGreen);
    mg1[k]->Add(sec1[k]);
    mg1[k]->Write();
    
    
    
    mg2[k] = new TMultiGraph(sector2_name,sector2_name);
    sec2[k] = new TGraph(1820,x_r2[k],y_r2[k]);
    mg2[k]->Add(sec2[k]);
    mg2[k]->Write();
    
      // sector1_name.str("");
     // sector1_name.clear();
     // c1->cd();
      //mg2[k]->Draw("ALP");
      //mg1[k]->Draw("LP");

    //  sec2[k]->Draw("ALP");
    //  sec1[k]->Draw("LP");
    //  sec_center->Draw("P");       

      // Filling the TH2Poly
    //  bin = h2p->AddBin(mg1[k]);
    //  bin = h2p->AddBin(mg2[k]);
    bin_num[2*k] = h2p->AddBin(mg1[k]);
    bin_num[2*k+1] = h2p->AddBin(mg2[k]);

  map<<sector1_name<<" "<<x_r1c[k]<<"  "<<y_r1c[k]<<" "<<bin_num[2*k]<<std::endl;
  map<<sector2_name<<"  "<<x_r2c[k]<<"  "<<y_r2c[k]<<" "<<bin_num[2*k+1]<<std::endl;

  } //K Loop - Number of strips in one section
    
    std::cout<<std::endl;
     std::cout<<" Building Down-Right sector "<<std::flush;

       for(Int_t k=32;k<64;k++){
        for(Int_t j=0;j<1820;j++){
            
            if(j%1800==0) std::cout<<"."<<std::flush;
            x_r1[k][j] = x_r1[k-32][j];
            y_r1[k][j] = -y_r1[k-32][j];
            x_r2[k][j] = x_r2[k-32][j];
            y_r2[k][j] = -y_r2[k-32][j];

		
          
            
        
        }
           sprintf(sector1_name,"sector1_%d",k);
           sprintf(sector2_name,"sector2_%d",k);
	  // map<<sector1_name<<"	"<<x_r1c[k]<<"	"<<y_r1c[k]<<std::endl;
	  // map<<sector2_name<<"	"<<x_r2c[k]<<"	"<<y_r2c[k]<<std::endl;
	   
           
           mg1[k] = new TMultiGraph(sector1_name,sector1_name);
           sec1[k]= new TGraph(1820,x_r1[k],y_r1[k]);
           mg1[k]->Add(sec1[k]);
           mg1[k]->Write();
           
           
           mg2[k] = new TMultiGraph(sector2_name,sector2_name);
           sec2[k] = new TGraph(1820,x_r2[k],y_r2[k]);
           mg2[k]->Add(sec2[k]);
           mg2[k]->Write();

           
           //bin = h2p->AddBin(mg1[k]);
           //bin = h2p->AddBin(mg2[k]);
           bin_num[2*k] = h2p->AddBin(mg1[k]);
           bin_num[2*k+1]  = h2p->AddBin(mg2[k]);

            map<<sector1_name<<" "<<x_r1c[k]<<"  "<<y_r1c[k]<<" "<<bin_num[2*k]<<std::endl;
            map<<sector2_name<<" "<<x_r2c[k]<<"  "<<y_r2c[k]<<" "<<bin_num[2*k+1]<<std::endl;
       }
    
    std::cout<<std::endl;
    std::cout<<" Building Up-Left sector "<<std::flush;
    
    for(Int_t k=64;k<96;k++){
        for(Int_t j=0;j<1820;j++){
            
            if(j%1800==0) std::cout<<"."<<std::flush;
            x_r1[k][j] = -x_r1[k-64][j];
            y_r1[k][j] = y_r1[k-64][j];
            x_r2[k][j] = -x_r2[k-64][j];
            y_r2[k][j] = y_r2[k-64][j];
            
            
            
        }
        sprintf(sector1_name,"sector1_%d",k);
        sprintf(sector2_name,"sector2_%d",k);
        //map<<sector1_name<<"	"<<x_r1c[k]<<"	"<<y_r1c[k]<<std::endl;
      //	map<<sector2_name<<"	"<<x_r2c[k]<<"	"<<y_r2c[k]<<std::endl;
        
        mg1[k] = new TMultiGraph(sector1_name,sector1_name);
        sec1[k]= new TGraph(1820,x_r1[k],y_r1[k]);
        mg1[k]->Add(sec1[k]);
        mg1[k]->Write();
    
        
        mg2[k] = new TMultiGraph(sector2_name,sector2_name);
        sec2[k] = new TGraph(1820,x_r2[k],y_r2[k]);
        mg2[k]->Add(sec2[k]);
        mg2[k]->Write();
        
        
        //bin = h2p->AddBin(mg1[k]);
        //bin = h2p->AddBin(mg2[k]);
        bin_num[2*k] = h2p->AddBin(mg1[k]);
        bin_num[2*k+1]  = h2p->AddBin(mg2[k]);
         map<<sector1_name<<" "<<x_r1c[k]<<"  "<<y_r1c[k]<<" "<<bin_num[2*k]<<std::endl;
          map<<sector2_name<<" "<<x_r2c[k]<<"  "<<y_r2c[k]<<" "<<bin_num[2*k+1]<<std::endl;
    }
    
    std::cout<<std::endl;
    std::cout<<" Building Down-Left sector "<<std::flush;
    
    for(Int_t k=96;k<128;k++){
        for(Int_t j=0;j<1820;j++){
            
            if(j%1800==0) std::cout<<"."<<std::flush;
            x_r1[k][j] = -x_r1[k-96][j];
            y_r1[k][j] = -y_r1[k-96][j];
            x_r2[k][j] = -x_r2[k-96][j];
            y_r2[k][j] = -y_r2[k-96][j];
            
            
            
        }
        sprintf(sector1_name,"sector1_%d",k);
        sprintf(sector2_name,"sector2_%d",k);
     //   map<<sector1_name<<"	"<<x_r1c[k]<<"	"<<y_r1c[k]<<std::endl;
//	map<<sector2_name<<"	"<<x_r2c[k]<<"	"<<y_r2c[k]<<std::endl;
        
        mg1[k] = new TMultiGraph(sector1_name,sector1_name);
        sec1[k]= new TGraph(1820,x_r1[k],y_r1[k]);
        mg1[k]->Add(sec1[k]);
        mg1[k]->Write();
        
        
        mg2[k] = new TMultiGraph(sector2_name,sector2_name);
        sec2[k] = new TGraph(1820,x_r2[k],y_r2[k]);
        mg2[k]->Add(sec2[k]);
        mg2[k]->Write();
        
        
       
       // mg2[k]->Draw("ALP");
       // mg1[k]->Draw("LP");
        
        //bin = h2p->AddBin(mg1[k]);
        //bin = h2p->AddBin(mg2[k]);
        bin_num[2*k] = h2p->AddBin(mg1[k]);
        bin_num[2*k+1]  = h2p->AddBin(mg2[k]);
         map<<sector1_name<<" "<<x_r1c[k]<<"  "<<y_r1c[k]<<" "<<bin_num[2*k]<<std::endl;
            map<<sector2_name<<" "<<x_r2c[k]<<"  "<<y_r2c[k]<<" "<<bin_num[2*k+1]<<std::endl;
        
    }
    

    // Central Pad
   rc= 1.34-DD;
    
     for(Int_t j=0;j<3600;j++){
         
         phi = (j/10.0)*(TMath::Pi()/180.0);
         x_rc[j] = rc * TMath::Cos(phi);
         y_rc[j] = rc * TMath::Sin(phi);
         
     }
    
    mgc = new TMultiGraph("sector_center","sector_center");
    secc= new TGraph(3600,x_rc,y_rc);
    mgc->Add(secc);
   // bin = h2p->AddBin(mgc);
    bin_num[256] = h2p->AddBin(mgc);
          
    mgc->Write();

   // Float_t px,py;
   /* std::cout<<" Generating random events ... "<<std::endl;
    for (Int_t i = 0; i < 50000; i++) {
        gRandom->Rannor(px,py);
        h2p->Fill(10*px,10*py);
        
        //std::cout<<" px : "<<px*30.0<<" py : "<<py*30.0<<std::endl;
    }*/
     /*   for (Int_t i = 0; i <256; i++) {

          std::cout<<"bin["<<i<<"] = "<<bin_num[i]<<std::endl;

        }*/


	TCanvas *c2 = new TCanvas("c2", "c2");
	c2->Draw();
	//h2p->Fill(1.5,1,10);
	h2p->Draw("Lcolz");
    //h2p->Draw("Lscat");
        sec_center->Draw("P");
	sec_center_2->Draw("P");
        h2p->Write();
    file->Close();
    	map.close();
      
	
}
#endif
