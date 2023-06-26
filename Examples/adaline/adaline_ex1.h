//
// Created by kosti_0b5rpb8 on 23/6/2023.
//

#ifndef MYDNN_ADALINE_EX1_H
#define MYDNN_ADALINE_EX1_H
//
// Created by kosti_0b5rpb8 on 23/6/2023.
//
#include "adaline.h"
#include <utility>

void createData(string filename);

pair<Base_Node **, Base_Link **> CreateNetwork();

void TrainNetwork(Pattern *const *data, Base_Node *const *Node);
void Save(Base_Node *const *Node, Base_Link *const *Link, string filename);
void Load(ifstream &infile, Base_Node *const *Node, Base_Link *const *Link);
void Run(Pattern *const *data, Base_Node *const *Node);

/// Main Adaline
void AdalineExample() {
    createData("lin2var.trn");

    // Load Training Set
    Pattern *data[250];
    ifstream infile("lin2var.trn");
    for (int i = 0; i < 250; i++) data[i] = new Pattern(2, 1, infile);
    infile.close();

/// with create routine
//    auto Node_Link_pair = CreateNetwork();
//    auto Node = Node_Link_pair.first;
//    auto Link = Node_Link_pair.second;

/// without create routine
    Base_Node *Node[4];
    Base_Link *Link[3];

    Node[0] = new Input_Node;
    Node[1] = new Input_Node;
    Node[2] = new Bias_Node;
    Node[3] = new ADALINE_Node(0.45);

    Link[0] = new ADALINE_Link;
    Link[1] = new ADALINE_Link;
    Link[2] = new ADALINE_Link;

    Connect(Node[0], Node[3], Link[0]);
    Connect(Node[1], Node[3], Link[1]);
    Connect(Node[2], Node[3], Link[2]);


    // Train ADALINE
    TrainNetwork(data, Node);

    // Save ADALINE
    Save(Node, Link, "adaline1.net");

    // Load ADALINE
    Load(infile, Node, Link);

    // Run ADALINE
    Run(data, Node);
}

void Run(Pattern *const *data, Base_Node *const *Node) {
    for (int i = 0; i < 250; i++) {
        Node[0]->Set_Value(data[i]->In(0));
        Node[1]->Set_Value(data[i]->In(1));
        Node[3]->Run();

        cout << "Pattern: " << setw(3) << i << "Input: ("
             << data[i]->In(0) << ","
             << data[i]->In(1) << ")    ADALINE:"
             << setw(3) << Node[3]->Get_Value() << "    Actual:"
             << setw(3) << data[i]->Out(0) << endl;
    }
}
void Load(ifstream &infile, Base_Node *const *Node, Base_Link *const *Link) {
    infile.open("adaline1.net");
    for (int i = 0; i < 4; i++) Node[i]->Load(infile);
    for (int i = 0; i < 3; i++) Link[i]->Load(infile);
    infile.close();
}
void Save(Base_Node *const *Node, Base_Link *const *Link, string filename) {
    ofstream outfile(filename);

    for (int i = 0; i < 4; i++) Node[i]->Save(outfile);
    for (int i = 0; i < 3; i++) Link[i]->Save(outfile);
    outfile.close();
}
void TrainNetwork(Pattern *const *data, Base_Node *const *Node) {
    int iteration = 0;
    int good = 0;

    while (good < 250) {
        good = 0;
        for (int i = 0; i < 250; i++) {
            Node[0]->Set_Value(data[i]->In(0)),
                    Node[1]->Set_Value(data[i]->In(1)),
                    Node[3]->Run();

            if (data[i]->Out(0) != Node[3]->Get_Value()) // If wrong prediction learn
            {
                Node[3]->Learn();
                break;
            } else good++;
        }
        cout << iteration++ << ",  " << good << "/250" << endl;
    }
}
pair<Base_Node **, Base_Link **> CreateNetwork() {
    Base_Node** Node = new Base_Node*[4];
    Base_Link *Link[3];

    Node[0] = new Input_Node;
    Node[1] = new Input_Node;
    Node[2] = new Bias_Node;
    Node[3] = new ADALINE_Node(0.45);

    Link[0] = new ADALINE_Link;
    Link[1] = new ADALINE_Link;
    Link[2] = new ADALINE_Link;

    Connect(Node[0], Node[3], Link[0]);
    Connect(Node[1], Node[3], Link[1]);
    Connect(Node[2], Node[3], Link[2]);

    return make_pair(Node, Link);
}
void createData(string filename) {
    double x, y, y1, output;
    ofstream outfile(filename);

    for (int i = 0; i < 250; i++) {
        x = ((double) rand() / (double) RAND_MAX) * 2.0 - 1.0;
        y = ((double) rand() / (double) RAND_MAX) * 2.0 - 1.0;

        y1 = (-5 * x - 2) / -4;

        output = y < y1 ? 1 : -1;

        outfile << i << " " << x << " " << y << " " << "  " << output << endl;
    }
    outfile.close();
}


#endif //MYDNN_ADALINE_EX1_H
