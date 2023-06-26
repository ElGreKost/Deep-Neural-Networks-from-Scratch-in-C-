//
// Created by kosti_0b5rpb8 on 22/6/2023.
//
# include "common.h"
# include "base.h"

#ifndef MYDNN_ADALINE_H
#define MYDNN_ADALINE_H

// I'm just a soul whose intentions are good. Ohh Lord please don't let me be misunderstood.
class ADALINE_Node : public Feed_Forward_Node // Adaline processing node
{
protected:
    virtual double Transfer_Function (double value) override {return value < 0 ?  -1.0 : 1;}

public:
    // The first value is for the value the second for the learning rate.
    ADALINE_Node() : Feed_Forward_Node(2,1) {}
    ADALINE_Node (float lr): Feed_Forward_Node(2,1) {value[LEARNING_RATE] = lr;}
    virtual void Learn (int mode) {
        error[NODE_ERROR] = value[NODE_VALUE] * -2.0; // Compute error todo understand -2.0
        Base_Link *link;
        in_links.Reset_To_Head();
        double delta;
        for (int i = 0; i < in_links.Count(); ++i) { // Traverse input links.
            link = in_links.Curr();
            delta = value[LEARNING_RATE] * error[NODE_ERROR] * link->In_Value();
            link ->Update_Weight(delta);
            in_links.Next();
        }
        // todo try this to test iterator here that we have to pointer but the real in_list
//        for (auto lnk : in_links) {
//            delta = value[LEARNING_RATE] * error[NODE_ERROR] * lnk.In_Value();
//            lnk.Update_Weight(delta);
//        }
    }
    virtual char *Get_Name() {static char name[]="ADALINE_NODE"; return name;}
};

class ADALINE_Link : public Base_Link // Link for ADALINE Node
{
public:
    ADALINE_Link() : Base_Link() {value[WEIGHT] = RANDOM(-1.0, 1.0);}
    virtual void Save (ofstream &outfile){
        outfile << setw(4) << id << " " << setprecision(18)
        << value[WEIGHT] << " " << setw(4) << In_Node()->Get_ID() << " "
        << setw(4) << Out_Node()->Get_ID() << endl;
    }
    virtual void Load (ifstream &infile) {
        infile >> id;
        infile >> value[WEIGHT];
        int dummy;
        infile >> dummy;
        infile >> dummy;
    }
    virtual char *Get_Name() {static char name[] = "ADALINE_LINK"; return name;}
};


class ADALINE_Network : public Base_Network
{
protected:
    virtual void Create_Network() {
        node = new Base_Node * [num_nodes];
        link = new Base_Link * [num_links];

        for (int i = 0; i < num_nodes - 2; i++)
            node[i] = new Input_Node;

        node[num_nodes - 2] = new Bias_Node;

        node[num_nodes - 1] = new ADALINE_Node(learning_rate);

        for (int i = 0; i < num_links; i++) link[i] = new ADALINE_Link;

        for (int i = 0; i < num_links; i++) Connect(node[i], node[num_nodes-1], link[i]);
    }

    // my changed Load_Inputs that worked fine in ADALINE but in bp not so good
    virtual void Load_Inputs() { // the value of the input nodes is the same with the data input values
        auto adaline_inlist = node[num_nodes-1]->In_Links();
        int cnt = adaline_inlist->Count();   // if network node has any input links.
        if (cnt > 0)                         // load the connected node's input values into the input layer.
        {
            adaline_inlist->Reset_To_Head();
            for (int i = 0; i < cnt; i++) {
                // Set the value of the link (weight) to be the same as the input_node's value which is the input.
                Set_Value(adaline_inlist->Curr()->In_Value(), i);
                adaline_inlist->Next();
            }
        }
    }

    double learning_rate;
public:
    ADALINE_Network() :Base_Network() {}
    ADALINE_Network(char filename[]) :Base_Network(){
        ifstream infile(filename);
        Load(infile);
        infile.close();
    }
    ADALINE_Network(int size) :Base_Network() { // size is referencing the number of input nodes
        num_nodes = size + 2; // +1 because of bias node +1 because of ADALINE NODE
        num_links = size + 1;
        learning_rate = 0;
        Create_Network();
    }
    ADALINE_Network(int size, double lr):Base_Network() {
        num_nodes = size + 2;
        num_links = size + 1;
        learning_rate = lr;
        Create_Network();
    }

    virtual double Get_Value (int id = 0) { return node[num_nodes - 1] ->Get_Value();}
    virtual void Set_Value (double new_val, int id = 0) {node[id]->Set_Value(new_val);}
    virtual void Save (ofstream &outfile) {
        outfile << id << endl;
        Save_Nodes_Links(outfile);
    }
    virtual void Load (ifstream &infile) {
        infile >> id;
        Load_Nodes_Links(infile);
    }
    virtual void Run (int mode = 0) {
        Load_Inputs();
        node[num_nodes-1]->Run();
    }
    virtual void Learn (int mode = 0) {
        node[num_nodes - 1]->Learn();
    }
    virtual char *Get_Name() {static char name[]="ADALINE_Network"; return name;}
};
#endif //MYDNN_ADALINE_H
