//
// Created by kosti_0b5rpb8 on 20/6/2023.
//
// This header file contains some commonly used node types.

#include "base.h"

#ifndef MYDNN_COMMON_H
#define MYDNN_COMMON_H

/*The Input Node class is a generic Input Node. It can be used with most networks.*/
class Input_Node : public Base_Node
{
public:
    /* Default of one value set member (NODE_VALUE) and one error set member (NODE_ERROR).*/
    Input_Node(int size = 1) :Base_Node(size,size) {
        for (int i = 0; i < size; ++i) error[i] = 0.0, value[i] = 0.0;
    }
    virtual char *Get_Name() {static char name[] = "INPUT_NODE"; return name;}
};

/*The Bias Node Class is a node that always produces the same output. the bias Node's default output is 1.0*/
class Bias_Node: public Input_Node
{
public:
    Bias_Node(double bias = 1.0) : Input_Node(1) {value[0] = bias;}
    virtual void Set_Value(double value, int id=0) {} // disable Set_Value.
    virtual double Get_Value(int id = 0) {return value[0];}
    virtual char *Get_Name() {static char name[] = "BIAS_NODE"; return name;}
};

#endif //MYDNN_COMMON_H
