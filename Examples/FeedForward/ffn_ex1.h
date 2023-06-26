//
// Created by kosti_0b5rpb8 on 23/6/2023.
//
#ifndef MYDNN_FFN_EX1_H
#define MYDNN_FFN_EX1_H

#include "backprop.h"

/// XOR
void FFN_EX1() {

    /// Create Training Set
    Pattern *data[4];
                        // id            sizes       input   output
                        // ----           ---       -------   ----
    data[0] = new Pattern(0, 2, 1, 0.0, 0.0, 0.0);
    data[1] = new Pattern(1, 2, 1, 0.0, 1.0, 1.0);
    data[2] = new Pattern(2, 2, 1, 1.0, 0.0, 1.0);
    data[3] = new Pattern(3, 2, 1, 1.0, 1.0, 0.0);

    // create backprop net
    Backprop_Network a(0.45, 0.9, 3, 2 ,3 ,1);

    // Train back prop
    long iteration = 0;
    int good = 0;
    double tolerance = 0.5;
    double total_error;

    while (good < 4) { // Train until all patterns are correct

        good = 0;
        total_error = 0.0;

        for (int i = 0; i < 4; i++) {
            a.Set_Value(data[i]->In(0), 0);   // Set input node values (x).
            a.Set_Value(data[i]->In(1), 1);   // Set input node values (y).

            a.Run();

            a.Set_Error(data[i]);
// there is a problem in learn delta appears to be always 0...
            a.Learn();

            if (fabs(a.Get_Value(0) - data[i] -> Out(0)) < tolerance) good++;

            total_error += fabs(a.Get_Error());
        }
        if (iteration % 1000 == 0) cout << iteration << ".    " << good << "/4"
                                        << "    Error: " << setprecision(15) << total_error << endl;
        iteration++;
    }

    // Save bp
    ofstream outfile("bp2.net");
    a.Save(outfile);
    outfile.close();

    // create new bp
    Backprop_Network b;

    // load
    ifstream infile ("bp2.net");
    b.Load(infile);
    infile.close();

    // run backprop
    for (int i = 0; i < 4; i++) {
        b.Set_Value(data[i]->In(0), 0);   // Set input node values (x).
        b.Set_Value(data[i]->In(1), 0);   // Set input node values (y).


        b.Run();

        int out = (int)(b.Get_Value(0) + 0.5); // Get Output from Network // round it to 0 or 1

        cout << "Pattern: " << setw(3) << i << "   Input: ("
             << data[i]->In(0) << "," << data[i]->In(1) << ")    Backprop: ("
             << out << ")    Actual: (" << data[i]->Out(0) << ")\n";
    }



}

#endif //MYDNN_FFN_EX1_H
