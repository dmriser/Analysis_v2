
#include "pars.h"
#include "pars.cc"

void ec_sampling()
{
 
    // Get file
    TFile * f = TFile::Open("~/Documents/data/38472.root");
    TTree * h22;
    f->GetObject("h22",h22);
    
    const int N_SECT   = 6;
    
    // Physical Parameters
    double P_MIN  = 0.5;
    double P_MAX  = 2.80;
    int P_BINS    = 100;
    const int N_SLICES = P_BINS;
    double P_STEP = (P_MAX-P_MIN)/(N_SLICES-1);
    
    // Create histogram pointers & Fill, Define Fits
    TH2F * h2_ec_sampling[N_SECT];
    TH1D * h1_slice[N_SECT][N_SLICES];
    
    TF1 * f_gauss = new TF1("f_gauss","gaus",0.25,0.45);
    TF1 * f_mean_pol3[6];
    TF1 * f_sigma_pol3[6];
    
    TGraphErrors * g_mean[6];
    TGraphErrors * g_sigma[6];
    
    double mean[N_SLICES];
    double sigma[N_SLICES];
    double mean_e[N_SLICES];
    double sigma_e[N_SLICES];
    double x[N_SLICES];
    double dx[N_SLICES];
    
    // For passing to TGraph constructor
    for (int b=0; b<N_SLICES; b++) {x[b] = (double) b*P_STEP + P_MIN; dx[b] = 0.0;}
    
    // Load histogram for EC Sampling
    for (int s=0; s<N_SECT; s++)
    {
        h2_ec_sampling[s] = new TH2F(Form("h2_ec_sampling_%d",s),Form(" EC Sampling for Sector %d ",s+1),P_BINS,P_MIN,P_MAX,100,0.05,0.5);
        string name       = Form("etot/p:p >> h2_ec_sampling_%d",s);
        string cuts       = Form("q < 0 && ec_sect == %d",s+1);
        h22->Draw(name.c_str(),cuts.c_str());
    }
    
    // Do the fitting and slicing
    for (int s=0; s<N_SECT; s++)
    {
        for (int b=0; b<N_SLICES; b++)
        {
            string name       = Form("h1_slice_%d_%d",s,b);
            h1_slice[s][b]    = new TH1D(name.c_str(),name.c_str(),100,0.05,0.5);
            h2_ec_sampling[s] ->ProjectionY(name.c_str(),b+1,b+2);
            h1_slice[s][b]    ->Fit("f_gauss","RQ");
            
            mean[b]    = f_gauss->GetParameter(1);
            sigma[b]   = f_gauss->GetParameter(2);
            mean_e[b]  = f_gauss->GetParError(1);
            sigma_e[b] = f_gauss->GetParError(2);
        }
        
        f_mean_pol3[s]  = new TF1(Form("f_mean_pol3_%d",s),"pol3",0,N_SLICES);
        f_sigma_pol3[s] = new TF1(Form("f_sigma_pol3_%d",s),"pol3",0,N_SLICES);
        g_mean[s]       = new TGraphErrors(N_SLICES,x,mean,dx,mean_e);
        g_sigma[s]      = new TGraphErrors(N_SLICES,x,sigma,dx,sigma_e);
        g_mean[s]  ->Fit(Form("f_mean_pol3_%d",s),"RQ");
        g_sigma[s] ->Fit(Form("f_sigma_pol3_%d",s),"RQ");
    }
    
    cout << " Done! " << endl;

    // Outputting parameters to file. 
    epars pars;
    pars.load("test.dat");

    for (int s=0; s<6; s++)
      {
	pars.ECSSA[s] = f_sigma_pol3[s]->GetParameter(3);
	pars.ECSSB[s] = f_sigma_pol3[s]->GetParameter(2);
	pars.ECSSC[s] = f_sigma_pol3[s]->GetParameter(1);
	pars.ECSSD[s] = f_sigma_pol3[s]->GetParameter(0);

	pars.ECSMA[s] = f_mean_pol3[s]->GetParameter(3);
	pars.ECSMB[s] = f_mean_pol3[s]->GetParameter(2);
	pars.ECSMC[s] = f_mean_pol3[s]->GetParameter(1);
	pars.ECSMD[s] = f_mean_pol3[s]->GetParameter(0);

	pars.ECSNSIGMA[s] = 2.50;
      }
    
    pars.save("test_modded.dat");
    
    // Show results for mean on dist.
    TCanvas * c1 = new TCanvas("c1","",1200,800);
    c1->Divide(3,2);
    
    for (int c=0; c<N_SECT; c++)
    {
        c1->cd(c+1);
        h2_ec_sampling[c]->Draw("colz");
        g_mean[c]->Draw("PEsame");
    }
    
}
