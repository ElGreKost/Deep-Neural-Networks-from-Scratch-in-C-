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
    virtual double Transfer_Function (double value) {return value < 0 ?  -1.0 : 1;}

public:
    // The first value is for the value the second for the learning rate.
    ADALINE_Node() : Feed_Forward_Node(2,1) {} \
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
    }
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

#endif //MYDNN_ADALINE_H
