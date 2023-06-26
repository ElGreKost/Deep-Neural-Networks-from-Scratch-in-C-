//
// Created by kosti_0b5rpb8 on 26/6/2023.
//

#ifndef MYDNN_INHERITANCE_TESTS_H
#define MYDNN_INHERITANCE_TESTS_H

#include <iostream>

class A {
protected:
    virtual char id() {return 'A';}
public:
    virtual char only_in_A() { return id(); }
    virtual char *Get_Name() {char name[]="Parent"; return name;}
};

class B : public A {
protected:
    virtual char id() {return 'B';}
public:
    B () {};
    virtual char *Get_Name() {char name[]="Child"; return name;}
};

void inherit_test() {
    A *obj_list[1];
    obj_list[0] = new B;
    std::cout << obj_list[0]->only_in_A(); // well .... it works here correctly....

}

#endif //MYDNN_INHERITANCE_TESTS_H
