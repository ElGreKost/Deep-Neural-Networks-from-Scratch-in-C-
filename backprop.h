//
// Created by kosti_0b5rpb8 on 23/6/2023.
//

#ifndef MYDNN_BACKPROP_H
#define MYDNN_BACKPROP_H

#include "adaline.h"

#define DELTA 1
#define MOMENTUM 2

class BP_Node : public Feed_Forward_Node
{
protected:
    virtual double Transfer_Function (double value) {return 1.0 / (1.0 + exp(-value));}

public:
    BP_Node(int v_size, int e_size) : Feed_Forward_Node(v_size, e_size) {}
    virtual char *Get_Name() {static char name[]="BP_NODE"; return name;}
};

class BP_Output_Node : public BP_Node
{
public:
    BP_Output_Node (double lr, double mt, int v_size=3, int e_size = 1)
        : BP_Node(v_size, e_size) {value[LEARNING_RATE] = lr; value[MOMENTUM] = mt;}
protected:
    /// Compute_Error assumes that the desired output has been loaded in the error[NODE_VALUE].
    virtual double Compute_Error (int mode = 0) {
        auto desired_output = error[NODE_ERROR];
        auto actual_output = value[NODE_VALUE];
        double sigmoid_derivative = value[NODE_VALUE] * (1.0 - value[NODE_VALUE]);

        return  sigmoid_derivative * (desired_output - actual_output);
    }

    virtual void Learn(int mode = 0) {
        double delta;
        error[NODE_ERROR] = Compute_Error();

        in_links.Reset_To_Head();
        Base_Link *link;
        int cnt = in_links.Count();

        for (int i = 0; i < cnt; i++) {
            link = in_links.Curr();
            delta = value[LEARNING_RATE] * error[NODE_ERROR] * link->In_Value(); // I don't get this link->In_Value().
            link->Update_Weight(delta);
            in_links.Next();
        }

        /* or with iterators: todo maybe make it work with iterators
         for (auto lnk : in_links)
             delta = value[LEARNING_RATE] * error[NODE_ERROR] * lnk.In_Value(),
             lnk.Update_Weight(delta); */

    }

    virtual char *Get_Name() {static char name[]="BP_OUTPUT_NODE"; return name;}
};

class BP_Middle_Node : public BP_Output_Node
{
public:
    BP_Middle_Node(double lr, double mt, int vSize=3, int eSize=1) : BP_Output_Node(lr, mt, vSize, eSize) {}
    virtual char *Get_Name() {static char name[]="BP_MIDDLE_NODE"; return name;}
protected:
    virtual double Compute_Error (int mode = 0) {
        double total = 0;
        out_links.Reset_To_Head();
        int cnt = out_links.Count();
        for (int i = 0; i < cnt; i++) {
            total += out_links.Curr()->Weighted_Out_Error();
            // Weighted_Out_Error = out_error * weight:curr->out
            out_links.Next();
        }
        // total = <out_err, weight>
        return value[NODE_VALUE] * (1.0 - value[NODE_VALUE]) * total;
    }
};

class BP_Link : public Base_Link
{
public:
    BP_Link(int size = 2) : Base_Link(size) {
        value[WEIGHT] = RANDOM(-1.0, 1.0);
        value[DELTA] = 0.0;     // init prev change to zero.
    }

    virtual void Save (ofstream &outfile) {
        outfile << setw(4) << id << " " << setprecision(18)
                << value[WEIGHT] << " " << setw(4)
                << In_Node()->Get_ID() << " "
                << setw(4) << Out_Node()->Get_ID() << endl;
    }

    virtual void Load (ifstream &infile) {
        infile >> id;
        infile >> value[WEIGHT];
        int dummy;
        infile >> dummy;
        infile >> dummy;
    }
    virtual char *Get_Name() {static char name[]="BP_LINK"; return name;}

    virtual void Update_Weight (double new_val) {
        double momentum = Out_Node()->Get_Value(MOMENTUM);
        value[WEIGHT] += new_val + (momentum * value[DELTA]);
        value[DELTA] = new_val;
    }
};

class Backprop_Network : public ADALINE_Network
{
protected:
    int num_layers;
    int first_middle_node;
    int first_output_node;
    int *node_cnt;
    double momentum_term;

    /// change this method if we want to add residual or skip links
    virtual void Create_Network() {
        node = new Base_Node * [num_nodes];
        link = new Base_Link * [num_links];
        int curr = 0;

        for (int i = 0; i < node_cnt[0]; ++i)
            node[curr++] = new Input_Node;
        first_middle_node = curr;

        for (int i = 1; i < num_layers - 1; ++i)
            for (int j = 0; j < node_cnt[i]; ++j)
                node[curr++] = new BP_Middle_Node(learning_rate, momentum_term);
        first_output_node = curr;

        for (int i = 0; i < node_cnt[num_layers - 1]; ++i)
            node[curr++] = new BP_Output_Node(learning_rate, momentum_term);

        for (int i = 0; i < num_links; ++i) link[i] = new BP_Link;

        curr = 0;
        int layer1=0,layer2=first_middle_node;
        for (int i = 0; i < num_layers -1; ++i) {
            for (int j = 0; j < node_cnt[i+1]; j++)
                for (int k = 0; k < node_cnt[i]; k++)
                    Connect(node[layer1 + k], node[layer2 + j], link[curr++]);
            layer1 = layer2;
            layer2 += node_cnt[i+1];
        }

    }

public:
    Backprop_Network (char filename[]) : ADALINE_Network() {
        ifstream infile(filename);
        Load(infile);
        infile.close();
    }

    Backprop_Network(double lr, double mt, int layers, ...) : ADALINE_Network() {
        num_nodes = 0;
        num_links = 0;

        num_layers = layers;
        va_list v1;
        va_start(v1, layers);
        node_cnt = new int[layers];
        for (int i = 0; i < layers; i++) {
            node_cnt[i] = va_arg(v1, int);
            num_nodes += node_cnt[i];
            if (i > 0) num_links += node_cnt[i-1]*node_cnt[i]; // links between layers
        }
        va_end(v1);
        learning_rate = lr;
        momentum_term = mt;
        Create_Network();
    }

    Backprop_Network(double lr, double mt, int layers, int nodes[]) : ADALINE_Network() {
        num_nodes = 0;
        num_links = 0;

        num_layers = layers;
        node_cnt = new int[layers];
        for (int i = 0; i < layers; ++i) {
            node_cnt[i] = nodes[i];
            num_nodes += node_cnt[i];
            if (i > 0) num_links = node_cnt[i-1] * node_cnt[i]; // links between layers
        }
        learning_rate = lr;
        momentum_term = mt;

        Create_Network();
    }

    Backprop_Network() : ADALINE_Network() {node_cnt = NULL;}

    ~Backprop_Network() { if (node_cnt) delete []node_cnt;}

    double Get_Error (int id = 0) {return node[id + first_output_node]->Get_Error();}

    void Set_Error (double new_val, int id = 0) { node[id + first_output_node]->Set_Error(new_val);}

    void Set_Error (Pattern *output_pattern) {
        for (int i = 0; i < node_cnt[num_layers -1]; ++i)
            node[id + first_output_node]->Set_Error(output_pattern->Out(i));
    }

    double Get_Value(int id = 0) { return node[id + first_output_node]->Get_Value();}

    void Run(int mode = 0) {
        Load_Inputs();

        for (int i = first_middle_node; i < num_nodes; i++) // all nodes except inputs
            node[i]->Run();
    }

    void Learn(int mode = 0) {
        int cnt = out_links.Count(); // Get error from linked nodes if avail.
        if (cnt > 0) {
            out_links.Reset_To_Head();
            for (int i = 0; i < cnt; ++i) {
                node[i + first_output_node]->Set_Error(out_links.Curr()->Out_Error());
                out_links.Next();
            }
        }

        // changed from num_nodes - 1 to this (compatible with more than one output i think).
        for (int i = num_nodes - node_cnt[i-1]; i >= first_middle_node; i--)
            node[i] -> Learn();
    }

    void Save(ofstream &outfile) {
        outfile << id << endl;
        outfile << num_layers << endl;
        for (int i = 0; i < num_layers; i++)
            outfile << node_cnt[i] << endl;
        Save_Nodes_Links (outfile);
    }

    void Load(ifstream &infile) {
        infile >> id;
        infile >> num_layers;
        if (node_cnt) delete []node_cnt;
        node_cnt = new int[num_layers];
        for (int i = 0; i < num_layers; ++i)
            infile >> node_cnt[i];
        Load_Nodes_Links(infile);
    }
};

#endif //MYDNN_BACKPROP_H
