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
        bkg_points.push_back(1-(bkg_slice_int/bkg_int));//y-coordinates are 1-passing/truth for different cuts for background
    }
    
    //TCanvas* c=new TCanvas();
    TGraph *roc = new TGraph(sig_points.size(), &sig_points[0], &bkg_points[0]);
    roc->SetLineColor(color);
    //roc1->SetTitle(vars1[j]);
    //roc2->SetTitle(vars2[j]);
    roc->SetName(name);
    return roc;
}
void plotGraphs(TGraph * graph_a, TGraph * graph_b, const char * name_a, const char * name_b, const char * header)
{
    TCanvas* can=new TCanvas();
    
    graph_a->Draw();
    graph_b->Draw("same");

    TLegend* leg = new TLegend(.3,.3,.5,.5);
    leg->SetHeader(header);
    leg->AddEntry(name_a,name_a);
    leg->AddEntry(name_b,name_b);
    leg->Draw("same");
    
    std::string var_a(name_a);//converting to strings to concatenate, then back
    std::string var_b(name_b);
    std::string header_str(header);
    std::string fname_str = header_str+"_"+var_a+"_blue_"+var_b+"_red_ROC.pdf";
    const char *fname_char = fname_str.c_str();
    can->Print(fname_char);
}
void plotHists(TH1F * hist_a, TH1F * hist_b, const char * name_a, const char * name_b, const char * header, int logscale=0)
{
    TCanvas* can=new TCanvas();

    hist_a->Draw();
    hist_b->Draw("same");

    if (logscale==1)
    {
        gPad->SetLogy();//gPad refers to the currently selected pad
    }

    TLegend* leg = new TLegend(.2,.6,.4,.8);
    leg->SetHeader(header);
    leg->AddEntry(name_a,name_a);
    leg->AddEntry(name_b,name_b);
    leg->Draw("same");
    
    std::string var_a(name_a);
    std::string var_b(name_b);
    std::string header_str(header);
    std::string fname_str = header_str+"_"+var_a+"_blue_"+var_b+"_red.pdf";
    const char *fname_char = fname_str.c_str();
    can->Print(fname_char);
}

void plots()
{

    const char *vars1[5] = {"SUEP_spher_1_ch","SUEP_aplan_1_ch","SUEP_FW2M_1_ch","SUEP_C_1_ch","SUEP_D_1_ch"};//the r=1 variables
    const char *vars2[5] = {"SUEP_spher_ch","SUEP_aplan_ch","SUEP_FW2M_ch","SUEP_C_ch","SUEP_D_ch"};//the r=2 variables
    //the variables are sphericity, aplanarity, 2nd fox wolfram moment, C parameter ,and D parameter

    TFile* bkg_file= new TFile("mar29outputs/outputs/backgrounds.root");
    std::string floc = "mar29outputs/outputs/";
    std::string signals[12] = {
        "SUEP-m1000-darkPho",
        "SUEP-m1000-darkPhoHad",
        "SUEP-m1000-generic",
        "SUEP-m125-darkPho",
        "SUEP-m125-darkPhoHad",
        "SUEP-m125-generic",
        "SUEP-m400-darkPho",
        "SUEP-m400-darkPhoHad",
        "SUEP-m400-generic",
        "SUEP-m750-darkPho",
        "SUEP-m750-darkPhoHad",
        "SUEP-m750-generic"
    };
    std::string ender = "+RunIIAutumn18-private+MINIAODSIM_IronMan.root";//the part at the end that is the same for every signal sample
    
    for (int k = 0; k<12; ++k)
    {
        std::string sig_filename_str = floc+signals[k]+ender;
        const char *sig_filename = sig_filename_str.c_str();
        cout << sig_filename; //debug
        TFile* sig_file= new TFile(sig_filename);

        for (int j = 0; j<5; ++j)
        {
            //TCanvas* can=new TCanvas();

            TH1F* bkg_hist_1=(TH1F*)bkg_file->Get(vars1[j]);
            TH1F* bkg_hist_2=(TH1F*)bkg_file->Get(vars2[j]);
            TH1F* sig_hist_1=(TH1F*)sig_file->Get(vars1[j]);
            TH1F* sig_hist_2=(TH1F*)sig_file->Get(vars2[j]);
            /*
            //plot histograms of r=1 and r=2 variables together:
            sig_hist_1->Draw();
            sig_hist_1->SetLineColor(4);
            sig_hist_2->Draw("same");
            sig_hist_2->SetLineColor(2);

            //converting to a string to concatenate, then converting back to save
            std::string var1(vars1[j]);
            std::string var2(vars2[j]);
            std::string fname_a = var1+"_blue_"+var2+"_red.pdf";
            const char *fname_b = fname_a.c_str();
            can->Print(fname_b);//save first plot
            */
            sig_hist_1->SetLineColor(4);
            sig_hist_2->SetLineColor(2);
            bkg_hist_1->SetLineColor(3);
            sig_hist_1->SetName(vars1[j]);
            sig_hist_2->SetName(vars2[j]);
            bkg_hist_1->SetName("background");

            //plot r=1 and r=2 variables together
            plotHists(sig_hist_1, sig_hist_2, vars1[j], vars2[j], signals[k].c_str());

            //plot ROC curves of r=1 and r=2 variables together:
            TGraph *roc1 = rocCurve(bkg_hist_1, sig_hist_1, 4, vars1[j]);
            TGraph *roc2 = rocCurve(bkg_hist_2, sig_hist_2, 2, vars2[j]);
            
            plotGraphs(roc1, roc2, vars1[j], vars2[j], signals[k].c_str());

            //plot r=1 variable and background together
            plotHists(bkg_hist_1, sig_hist_1, "background", vars1[j], signals[k].c_str(), 1);

            //plot r=2 variables against background in log scale:
            /*
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
            */
        }
    }
}