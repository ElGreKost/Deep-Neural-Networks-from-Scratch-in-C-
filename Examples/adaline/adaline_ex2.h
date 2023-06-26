//
// Created by kosti_0b5rpb8 on 23/6/2023.
//

#ifndef MYDNN_ADALINE_EX2_H
#define MYDNN_ADALINE_EX2_H

#include "adaline.h"

void Adaline_Example_2() {
    srand(1);

    // Load training set
    Pattern *data[250];
    ifstream infile("lin2var.trn");
    if (infile.fail()) {
        cout << "Couldn't open pattern file...\n";
        exit(0);
    }
    for (int i = 0; i < 250; i++)
        data[i] = new Pattern(2, 1, infile);
    infile.close();

    // Create ADALINE
    ADALINE_Network a(2, 0.45); // Two input nodes(x,y). Learning rate = 0.45

    // Train ADALINE
    int iteration{0}, good{0};

    while (good < 250)  // Train until all patterns are good
    {
        good = 0;
        for (int i = 0; i < 250; i++) {
            a.Set_Value(data[i]->In(0), 0);   // Set input node values (x).
            a.Set_Value(data[i]->In(1), 1);   // Set input node values (y).

            a.Run(); // for adaline this just loads the inputs
            a.Get_Name();

            if (data[i]->Out(0) != a.Get_Value()) {  // If Network Produced and error, perform Learning function.
                a.Learn();
                break;
            } else good++;
        }
        cout << iteration++ << ",  " << good << "/250" << endl;
    }

    // Save ADALINE
    ofstream outfile("test.dta");
    a.Save(outfile);
    outfile.close();

    // Create and Load ADALINE
    ADALINE_Network b("test.dta");

    // Run ADALINE
    for (int i = 0; i < 250; i++) {
        b.Set_Value(data[i]->In(0), 0);   // Set input node values (x).
        b.Set_Value(data[i]->In(1), 1);   // Set input node values (y).

        b.Run();

        cout << setw(3) << i << "  Net: " << setprecision(7)
             << b.Get_Value() << "    Actual: " << setprecision(7)
             << data[i]->Out(0) << endl;
    }
}

#endif //MYDNN_ADALINE_EX2_H
