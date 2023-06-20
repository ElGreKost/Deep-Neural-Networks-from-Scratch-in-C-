#include <iostream>

#include "base.h"

int main() {
    Base_Node Node[6]; // we have 6 nodes available
    Base_Link Link[8]; // we have 8 links available

    Connect(Node[0],Node[2],Link[0]);
    Connect(Node[0],Node[3],Link[1]);
    Connect(Node[1],Node[2],Link[2]);
    Connect(Node[1],Node[3],Link[3]);
    Connect(Node[2],Node[4],Link[4]);
    Connect(Node[2],Node[5],Link[5]);
    Connect(Node[3],Node[4],Link[6]);
    Connect(Node[3],Node[5],Link[7]);

    ofstream outfile("Output.dta");

    for (auto & i : Node)
        i.Print(outfile);
    outfile.close();
}