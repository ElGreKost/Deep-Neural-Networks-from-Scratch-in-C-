//
// Created by kosti_0b5rpb8 on 24/6/2023.
//

#ifndef MYDNN_FFN_EX1_SIMPLE_H
#define MYDNN_FFN_EX1_SIMPLE_H

#include "backprop.h"

void FFN_EX1_SIMPLE() { // todo seems like to find the problem i will have to first print the initial weights and check the handwriten results with the ones the net provides
    srand(1);

    /// Create Training Set
    Pattern *data[4];
    // id            sizes       input   output
    // ----           ---       -------   ----
    data[0] = new Pattern(0, 2, 1, 0.0, 0.0, 0.0);
    data[1] = new Pattern(1, 2, 1, 0.0, 1.0, 1.0);
    data[2] = new Pattern(2, 2, 1, 1.0, 0.0, 1.0);
    data[3] = new Pattern(3, 2, 1, 1.0, 1.0, 0.0);

    // Create Backprop Network
    Base_Link *Link[9];
    Base_Node *Node[6];

    Node[0] = new Input_Node;
    Node[1] = new Input_Node;
    Node[2] = new BP_Middle_Node(0.4, 0.9);
    Node[3] = new BP_Middle_Node(0.4, 0.9);
    Node[4] = new BP_Middle_Node(0.4, 0.9);
    Node[5] = new BP_Output_Node(0.4, 0.9);

    for (int i = 0; i < 9; ++i) Link[i] = new BP_Link();


    int curr = 0;
    for (int i = 2; i <= 4; i++)
        for (int j = 0; j <= 1; j++)
            Connect(Node[j], Node[i], Link[curr++]);

    for (int j = 2; j <=4; ++j)
        Connect(Node[j], Node[5], Link[curr++]);

    // Train back prop
    long iteration = 0;
    int good = 0;
    double tolerance = 0.4;
    double total_error;
    while (good < 4 and iteration < 10000) { // Train until all patterns are correct

        good = 0;
        total_error = 0.0;

        for (int i = 0; i < 4; i++) {

            Node[0]->Set_Value(data[i]->In(0), 0);   // Set input node values (x).
            Node[1]->Set_Value(data[i]->In(1), 1);   // Set input node values (y).

            for (int j = 2; j <= 5; ++j) // forward pass
            {
                Node[j]->Run();
            }

            Node[5]->Set_Error(data[i]->Out(0)); // Set the error values of the output to the desired ones


            for (int j = 5; j >= 2; --j)                // backward pass
                Node[j]->Learn();

            if (fabs(Node[5]->Get_Value() - data[i] -> Out(0)) < tolerance) good++;
            if (iteration < 5 or (30 < iteration and iteration < 35)) cout << iteration << ":  predicted: " << Node[5]->Get_Value() << "   actual: " << data[i] -> Out(0) << endl;

            double out_error = fabs(Node[5]->Get_Error());
            total_error += out_error;
        }


        if (iteration % 1000 == 0) cout << iteration << ".    " << good << "/4"
                                   << "    Error: " << setprecision(15) << total_error << endl;
        iteration++;
    }

}

#endif //MYDNN_FFN_EX1_SIMPLE_H
