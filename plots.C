//this macro makes some plots comparing sphericity tensor based variables with r=1 to those with r=2
TGraph * rocCurve(TH1F * bkg_hist, TH1F * sig_hist, int color,const char * name)
{
    int nbins = bkg_hist->GetNbinsX();

    float sig_int = sig_hist->Integral(0,nbins);
    float bkg_int = bkg_hist->Integral(0,nbins);

    std::vector<float> sig_points(nbins);
    std::vector<float> bkg_points(nbins);
 
    for (int i = 0; i<nbins; ++i)//
    {
        float sig_slice_int = sig_hist->Integral(i,nbins);//passing
        float bkg_slice_int = bkg_hist->Integral(i,nbins);

        sig_points.push_back(sig_slice_int/sig_int);//x-coordinates are passing/truth for different cuts for signal
        bkg_points.push_back(1-(bkg_slice_int/bkg_int));//y-coordinates are passing/truth for different cuts for background
    }
    
    //TCanvas* c=new TCanvas();
    TGraph *roc = new TGraph(sig_points.size(), &sig_points[0], &bkg_points[0]);
    roc->SetLineColor(color);
    //roc1->SetTitle(vars1[j]);
    //roc2->SetTitle(vars2[j]);
    roc->SetName(name);
    return roc;
}
void plotRocs(TGraph * roc_a, TGraph * roc_b, const char * name_a, const char * name_b)
{
    TCanvas* c=new TCanvas();
    
    roc_a->Draw();
    roc_b->Draw();

    TLegend* leg = new TLegend(.3,.3,.5,.5);
    leg->SetHeader("The Legend Title");
    leg->AddEntry(name_a,name_a);
    leg->AddEntry(name_b,name_b);
    leg->Draw();
    
    std::string var_a(name_a);
    std::string var_b(name_b);
    std::string fname_e = var_a+"_blue_"+var_b+"_red_ROC.pdf";
    const char *fname_f = fname_e.c_str();
    c->Print(fname_f);
}

void plots()
{
/*
gROOT->SetStyle("Plain");   // set plain TStyle
gStyle->SetOptStat(111111); // draw statistics on plots,
                            // (0) for no output
gStyle->SetOptFit(1111);    // draw fit results on plot,
                            // (0) for no ouput
gStyle->SetPalette(53);     // set color map
gStyle->SetOptTitle(0);     // suppress title box
*/

const char *vars1[5] = {"SUEP_spher_1_ch","SUEP_aplan_1_ch","SUEP_FW2M_1_ch","SUEP_C_1_ch","SUEP_D_1_ch"};//the r=1 variables
const char *vars2[5] = {"SUEP_spher_ch","SUEP_aplan_ch","SUEP_FW2M_ch","SUEP_C_ch","SUEP_D_ch"};//the r=2 variables

TFile* bkg_file= new TFile("outputs/backgrounds.root");
TFile* sig_file= new TFile("outputs/SUEP-m1000-darkPho+RunIIAutumn18-private+MINIAODSIM_noJetIdWrongWeight.root");

for (int j = 0; j<5; ++j)
{
    TCanvas* can=new TCanvas();

    TH1F* bkg_hist_1=(TH1F*)bkg_file->Get(vars1[j]);
    TH1F* bkg_hist_2=(TH1F*)bkg_file->Get(vars2[j]);
    TH1F* sig_hist_1=(TH1F*)sig_file->Get(vars1[j]);
    TH1F* sig_hist_2=(TH1F*)sig_file->Get(vars2[j]);

    sig_hist_1->Draw();
    sig_hist_1->SetLineColor(4);
    sig_hist_2->Draw("same");
    sig_hist_2->SetLineColor(2);



    //converting to a string to concatenate, then converting back to save
    std::string var1(vars1[j]);
    std::string var2(vars2[j]);
    std::string fname_a = var1+"_blue_"+var2+"_red.pdf";
    const char *fname_b = fname_a.c_str();
    can->Print(fname_b);

    TGraph *roc1 = rocCurve(bkg_hist_1, sig_hist_1, 4, vars1[j]);
    TGraph *roc2 = rocCurve(bkg_hist_2, sig_hist_2, 2, vars2[j]);
    
    plotRocs(roc1, roc2, vars1[j], vars2[j]);

    /*
    int nbins = bkg_hist_1->GetNbinsX();

    float sig_int_1 = sig_hist_1->Integral(0,nbins);
    float sig_int_2 = sig_hist_2->Integral(0,nbins);
    float bkg_int_1 = bkg_hist_1->Integral(0,nbins);
    float bkg_int_2 = bkg_hist_2->Integral(0,nbins);

    std::vector<float> sig_points_1(nbins);
    std::vector<float> sig_points_2(nbins);
    std::vector<float> bkg_points_1(nbins);
    std::vector<float> bkg_points_2(nbins);
 
    for (int i = 0; i<nbins; ++i)//
    {
        float sig_slice_int_1 = sig_hist_1->Integral(i,nbins);//passing
        float bkg_slice_int_1 = bkg_hist_1->Integral(i,nbins);
        float sig_slice_int_2 = sig_hist_2->Integral(i,nbins);
        float bkg_slice_int_2 = bkg_hist_2->Integral(i,nbins);

        sig_points_1.push_back(sig_slice_int_1/sig_int_1);//x-coordinates are passing/truth for different cuts for signal
        bkg_points_1.push_back(1-(bkg_slice_int_1/bkg_int_1));//y-coordinates are passing/truth for different cuts for background
        sig_points_2.push_back(sig_slice_int_2/sig_int_2);
        bkg_points_2.push_back(1-(bkg_slice_int_2/bkg_int_2));
    }
    
    //TCanvas* c=new TCanvas();
    TGraph *roc1 = new TGraph(sig_points_1.size(), &sig_points_1[0], &bkg_points_1[0]);
    TGraph *roc2 = new TGraph(sig_points_2.size(), &sig_points_2[0], &bkg_points_2[0]);
    roc1->SetLineColor(4);
    roc2->SetLineColor(2);
    //roc1->SetTitle(vars1[j]);
    //roc2->SetTitle(vars2[j]);
    roc1->SetName(vars1[j]);
    roc2->SetName(vars2[j]);
    roc1->Draw();
    roc2->SetTitle(vars2[j]);
    roc2->Draw("same");

    TLegend* leg = new TLegend(.3,.3,.5,.5);
    leg->SetHeader("The Legend Title");
    leg->AddEntry(vars1[j],vars1[j]);
    leg->AddEntry(vars2[j],vars2[j]);
    leg->Draw();
    
    std::string fname_e = var1+"_blue_"+var2+"_red_ROC.pdf";
    const char *fname_f = fname_e.c_str();
    can->Print(fname_f);
    */

    bkg_hist_1->Draw();
    gPad->SetLogy();//gPad refers to the currently selected pad
    sig_hist_1->Draw("same");
    sig_hist_1->SetLineColor(4);

    TLegend* l = new TLegend(.3,.3,.5,.5);
    l->SetHeader("The Legend Title");
    l->AddEntry(bkg_hist_1,"background");
    l->AddEntry(sig_hist_1,vars1[j]);
    l->Draw();

    std::string fname_c = var1+"_vs_bkg.pdf";
    const char *fname_d = fname_c.c_str();
    can->Print(fname_d);
    }
}