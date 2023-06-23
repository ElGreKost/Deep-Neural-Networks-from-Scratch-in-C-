//
// Created by kosti_0b5rpb8 on 23/6/2023.
//

#ifndef MYDNN_ABSTRACT_NETWORK_H
#define MYDNN_ABSTRACT_NETWORK_H

#include <iostream>

#include "base.h"



void AbstractNNExample() {

    Base_Node Node[6]; // we have 6 nodes available
    Base_Link Link[8]; // we have 8 links available

    Connect(Node[0], Node[2], Link[0]);
    Connect(Node[0], Node[3], Link[1]);
    Connect(Node[1], Node[2], Link[2]);
    Connect(Node[1], Node[3], Link[3]);
    Connect(Node[2], Node[4], Link[4]);
    Connect(Node[2], Node[5], Link[5]);
    Connect(Node[3], Node[4], Link[6]);
    Connect(Node[3], Node[5], Link[7]);

    ofstream outfile("Link_output.dta");
    for (auto &i : Link)
        i.Save(outfile);
    outfile.close();

/*
    ofstream outfile("Output.dta");

    for (auto & i : Node)
        i.Print(outfile);
    outfile.close();
*/
//    /// iterator test
//    auto in = Node[2].In_Links();
//    cout << "now you can debug\n";
//    in->Reset_To_Head();
//    SearchingVisitor<Base_Link> v3;
//    in->accept(v3);
//    v3.getFound() ? cout << "Found: " << v3.getValue() << endl : cout << "Not found\n";

    // todo the problem is here because now in_list is pointer to LList and not LList
//    auto i = &in->begin();
//    cout << (*i).Get_ID() << endl;
//    i++;
//    cout << (*i).Get_ID();
//    for (auto i: in)
//        if (i.Get_Value() > 3)
//            cout << "I think I will cry\n";
}


/*
//    auto out = Node[2].Out_Links();
//    out->Reset_To_Tail();
//    in->Curr()->Set_Value(30.0);
//    cout << "l of size " << in->Count() << "Id: " << in->Curr()->Get_ID() << " Name: " <<  in->Curr()->Get_Name() << endl;
//    PrintingVisitor<Base_Link> v;
//    in->accept(v);
//
    // my iterators don't work....
*/

#endif //MYDNN_ABSTRACT_NETWORK_H
