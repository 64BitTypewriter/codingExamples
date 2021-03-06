With a TTree (such as "experiment.root"), what you want to do is the following:

     root experiment.root
     tree1->Scan() // For scanning
     tree1->Print() // For getting printing information

You can also:

    tree1->Draw("ebeam") // Or whatever branch is inside the tree
    tree1->Draw("ebeam:px") // To plot a 2D scatter plot
    tree1->Draw("ebeam:px", "chi2>0.5 && chi2<2", "LEGO") // To apply a cut

Finally, what you really want to do is:

    tree1->MakeClass("Analyze")

which produces "Analyze.C" and "Analyze.h". You can then modify them to do whatever you want.
Declaring histograms go in SETUP, in the LOOP you fill the histograms and after the loop you DRAW the histograms.

To run Analyze you must run the following commands:

		root
		.L Analyze.C
		Analyze t
		t.Loop()
   
You can find the type and name of the variables in the TTree by looking at Analyze.h.
