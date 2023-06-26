//
// Created by kosti_0b5rpb8 on 20/6/2023.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef MYDNN_BASE_H
#define MYDNN_BASE_H
// Purpose: Base classes for NN nodes and links.

#include <cstdio>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Pattern.h"

using namespace std;

#define NODE_VALUE 0
#define LEARNING_RATE 1
#define NODE_ERROR 0
#define WEIGHT 0

class Base_Node;  // Forward declaration so links can use the base node type.
class Base_Link   // Base Neural-Network Link class.
{
private:
    static int ticket;

protected:
    int id;               // id number for link.
    double *value;        // Value(s) for link.
    Base_Node *in_node;   // Node instance link is coming from.
    Base_Node *out_node;  // Node instance link is going to.
    int value_size;

public:
    explicit Base_Link(int size = 1): id(++ticket), value_size(size) {
        value = value_size > 0 ? new double[value_size] : NULL;
        for (int i = 0; i < value_size; ++i)
            value[i] = 0.0;
        in_node = out_node = NULL;
    }

    ~Base_Link() { if (value_size > 0) delete[] value; }

    virtual void Save(ofstream &outfile) {
        outfile << id << endl;
        outfile << value_size;
        if (value) delete[]value;
        value = new double[value_size];
        for (int i = 0; i < value_size; ++i)
            outfile << " " << setprecision(18) << value[i];
        outfile << endl;
    }

    virtual void Load(ifstream &infile) {
        infile >> id;
        infile >> value_size;
        if (value) delete[]value;
        value = new double[value_size];
        for (int i = 0; i < value_size; ++i)
            infile >> value[i];
    }

    inline virtual double Get_Value(int id = WEIGHT) { return value[id]; }

    inline virtual void Set_Value(double new_val, int id = WEIGHT) { value[id] = new_val; }

    inline virtual void Set_In_Node(Base_Node *node, int id) { in_node = node; }

    inline virtual void Set_Out_Node(Base_Node *node, int id) { out_node = node; }

    inline virtual Base_Node *In_Node() { return in_node; }

    inline virtual Base_Node *Out_Node() { return out_node; }

    inline virtual char *Get_Name() {
        static char name[] = "BASE_LINK";
        return name;
    }

    inline virtual void Update_Weight(double new_val) { value[WEIGHT] += new_val; }

    inline int Get_ID() { return id; }

    inline virtual double In_Value(int mode = NODE_VALUE);

    inline virtual double Out_Value(int mode = NODE_VALUE);

    inline virtual double In_Error(int mode = NODE_ERROR);

    inline virtual double Out_Error(int mode = NODE_ERROR);

    inline virtual double Weighted_In_Value(int mode = NODE_VALUE);

    inline virtual double Weighted_Out_Value(int mode = NODE_VALUE);

    inline virtual double Weighted_In_Error(int mode = NODE_ERROR);

    inline virtual double Weighted_Out_Error(int mode = NODE_ERROR);

    inline virtual int Get_Set_Size() { return value_size; }

    inline virtual void Epoch(int code = 0) {}
};

int Base_Link::ticket = 1;
/*This static var is shared by all links derived from the base link class.
Its purpose is to give each link created from the base_link class a
unique identification number.*/


















// ------------------------------------//
// Iterator requirements and Iterator class
template<typename T> // make it work.
class Iterator {
public:
    Iterator(const Iterator &i) : impl(i.impl->clone()) {}

    ~Iterator() { delete impl; }

    T &operator*() { return impl->access(); }

    // prefix ++, i.e. ++i
    Iterator &operator++() {
        impl->advance();
        return *this;
    }

    // postfix ++, i.e. i++
    Iterator operator++(int) {
        Iterator result(*this);
        impl->advance();
        return result;
    }

    bool operator==(const Iterator &i) const {
        return impl->equal(*(i.impl));
    }

    bool operator!=(const Iterator &i) const {
        return !(*this == i);
    };

    class Impl {
    public:
        virtual ~Impl() {};

        virtual Impl *clone() const = 0;

        virtual T &access() = 0;// ORIGINAL

        virtual void advance() = 0;

        virtual bool equal(const Impl &i) const = 0;
    };

    Iterator(Impl *i) : impl(i) {}

private:
    Impl *impl;
};

template<typename T>
class Iterable {
public:
    virtual Iterator<T> begin() = 0;

    virtual Iterator<T> end() = 0;
};

template<typename T>
class Visitor {
public:
    virtual ~Visitor() {}

    virtual void visit(T *x) = 0;

    virtual bool finished() const { return false; }
};

template<typename T>
class Visitable {
public:
    virtual void accept(Visitor<T> &visitor) = 0;
};

// DOUBLE LINKED LIST Support Class
template<typename T>
class LList : public Iterable<T>, public Visitable<T> {
    // todo not the classic impl because now the tail is connected to the head
    //  so I have to put a marker in the end for the iterator to know when to stop.
private:
    struct NODE {
        NODE *next, *prev;
        T *element;
        bool isTail;
    };

    NODE *head, *tail, *curr;
    int count;

    inline void Clear() {
        NODE *i = head, *temp;

        while (i) {
            temp = i;
            i = i->next;
            delete temp;
        }
        curr = head = tail = NULL;
        count = 0;
    }

public:
    explicit LList() { curr = head = tail = NULL, count = 0; }

    ~LList() { Clear(); }

    int Add_To_Tail(T *element) {
        curr = NULL;
        return Add_Node(element);
    }

    int Add_Node(T *element) {
        NODE *temp = new NODE;

        if (!temp)
            cout << "Unable to allocate Node..." << endl;

        temp->element = element;

        if (!temp) return 0; // is this error check?

        // Add to TAIL
        if (!curr) // if you don't specify where we add to the end.
        {
            /// for iterator
            temp->isTail = true;

            temp->prev = tail;
            temp->next = NULL;
            // if empty ? head=tail else temp is the new tail.
            if (tail == NULL)
            {
                head = temp;
                tail = temp;
            } else {
                tail -> isTail = false;
                tail -> next = temp;
                tail = temp;

            }

        }
            // Add as HEAD
        else if (curr == head) {
            temp->prev = NULL,
            temp->next = head;
            // if empty ? tail=head : temp is the new head.
            if (head == NULL) {
                head = temp;
                tail = temp;
            } else {
                head -> prev = temp;
                head = temp;
            }
        }
            // Add to Middle of List:
        else {
            temp->prev = curr->prev;
            temp->next = curr;
            curr->prev->next = temp;
            curr->prev = temp;
        }

        count++;
        return 1;
    }

    int Del_Node() {
        /* Delete current node
            We use curr->prev-next so that in the end we just say delete curr;...
        */
        if (!curr) return 0;    // if list is empty.
        delete curr->element; // free up space.

        if (curr == head) { // Delete head.
            head == tail ? tail = NULL : head->next->prev = NULL;
            head = curr->next;
        } else if (curr == tail) // Delete tail.
            tail->prev->next = NULL,
                    tail = curr->prev;
        else
            curr->next->prev = curr->prev,
                    curr->prev->next = curr->next;

        delete curr;
        curr = NULL;
        count--;
        return 1;
    }

    int Del(T *element) {
        /* Delete the first occurrence of element.*/
        if (!Find(element)) return 0;
        return Del_Node();
    }

    int Find(T *element) {
        NODE *temp = head;
        int cnt = 1; // we start counting from one.
        curr = NULL;
        while (temp) {
            if (temp->element == element) {
                curr = temp;
                return cnt;
            }
            cnt++,
                    temp = temp->next;
        }
        return 0;
    }

    inline int Count() const { return count; }

    inline void Reset_To_Head() { curr = head; }

    inline void Reset_To_Tail() { curr = tail; }

    inline T *Curr() { return !curr ? NULL : curr->element; }

    inline void Next() { curr = !curr->next ? head : curr->next; }

    inline void Prev() { curr = !curr->prev ? tail : curr->prev; }

public:
    class ListIteratorImpl : public Iterator<T>::Impl {
    private:
        typedef typename Iterator<T>::Impl Impl;
    public:

        ~ListIteratorImpl() override {delete ptr; }

        Impl *clone() const {
            return new ListIteratorImpl(ptr);
        }


        // todo maybe when we access it because we want to return something
        //  returns the element of the elements
        //  lets say the value of the weight of the link and not the link node
        // we don't use dereference on ptr because of the Add_Node which also has * and not &.
        T &access() override { return *ptr->element;}

        void advance() override {ptr = ptr->next; }

        bool equal(const Impl &i) const override {
            return ptr == ((ListIteratorImpl *) &i)->ptr;     //ORIGINAL PAP
//            return ptr->isTail == ((ListIteratorImpl *) &i)->ptr->isTail; // me trying to see how to iter cycle
        }

    private:
        NODE *ptr;
        explicit ListIteratorImpl(NODE *p) : ptr(p) {}
        friend class LList;
    };

    Iterator<T> begin() override { return Iterator<T>(new ListIteratorImpl(head) ); }
    Iterator<T> end() override { return Iterator<T>(new ListIteratorImpl(nullptr)); }

public:
    void accept(Visitor<T> &visitor) override {
        for (NODE *p = head; p != NULL; p = p ->next) {
            if (visitor.finished()) return;
            auto to_visit = p->element;
            visitor.visit(to_visit);
        }
    }

};

template<typename T>
class PrintingVisitor : public Visitor<T> {
public:
    void visit(T *x) override { cout << x->Get_ID() << endl; }
};

//template <typename T>
//void print(Iterable<T> &c) {
//    for (auto &x : c)
//        cout << " " << x;
//}

template <typename T>
void print(Visitable<T> &c) {
    PrintingVisitor<T> v1;
    c.visit(v1);
}

template<typename T>
class SearchingVisitor : public Visitor<T> {
public:
    SearchingVisitor() : found(false) {}

    void visit(T *x) override {
        if (17.0 <= x->Get_Value() && x->Get_Value() <= 42.0) {
            found = true;
            value = x->Get_Value();
        }
    }

    bool finished() const override { return found; }

    bool getFound() const { return found; }

    int getValue() const { return value; }

private:
    bool found;
    int value;
};
















class Base_Node { // Base Neural-Network Node.
private:
    static int ticket;

protected:
    int id;             // Identification number.
    double *value;      // Value(s) stored by this node.
    int value_size;     // Number of Values stored by this node.
    double *error;      // Error value(s) stored by this node.
    int error_size;     // Number of Error values stored by this node.

    LList<Base_Link> in_links;     // List for input links.
    LList<Base_Link> out_links;    // List for output links.

public:
    explicit Base_Node(int v_size = 1, int e_size = 1) {
        id = ++ticket;

        if (v_size <= 0) value_size = 0, value = NULL;
        else value_size = v_size, value = new double[v_size];
        for (int i = 0; i < v_size; ++i) value[i] = 0.0;

        if (e_size <= 0) error_size = 0, error = NULL;
        else error_size = e_size, error = new double[e_size];
        for (int i = 0; i < e_size; ++i) error[i] = 0.0;
    }

    ~Base_Node() {
        if (value_size > 0) delete[] value;
        if (error_size > 0) delete[] error;
    }

    LList<Base_Link> *In_Links() { return &in_links; }

    LList<Base_Link> *Out_Links() { return &out_links; }

    virtual void Run(int mode = 0) {}

    virtual void Learn(int mode = 0) {}

    virtual void Epoch(int code = 0) {}


    virtual void Load(ifstream &infile) {
        infile >> id;

        infile >> value_size;
        if (value) delete[] value;
        value = new double[value_size];
        for (int i = 0; i < value_size; ++i) infile >> value[i];

        infile >> error_size;
        if (error) delete[] error;
        error = new double[error_size];
        for (int i = 0; i < error_size; ++i) infile >> error[i];
    }

    virtual void Save(ofstream &outfile) {
        outfile << setw(4) << id << endl;
        outfile << value_size;  // store value set.
        for (int i = 0; i < value_size; ++i) outfile << " " << setprecision(18) << value[i];
        outfile << endl;
        outfile << error_size;
        for (int i = 0; i < error_size; ++i) outfile << " " << setprecision(18) << error[i];
        outfile << endl;
    }

    inline virtual double Get_Value(int id = NODE_VALUE) { return value[id]; }

    inline virtual void Set_Value(double new_val, int id = NODE_VALUE) { value[id] = new_val; }

    inline virtual double Get_Error(int id = NODE_ERROR) { return error[id]; }

    inline virtual void Set_Error(double new_val, int id = NODE_ERROR) { error[id] = new_val; }

    inline int Get_ID(){ return id; }

    inline virtual char *Get_Name() {
        static char name[] = "BASE_NODE";
        return name;
    }

    void Create_Link_To(Base_Node &to_node, Base_Link *link) {
        out_links.Add_To_Tail(link);
        to_node.In_Links()->Add_To_Tail(link);
        link->Set_In_Node(this, id);
        link->Set_Out_Node(&to_node, to_node.Get_ID());
    }

    virtual void Print(ofstream &out) {
        out << "Node Id: " << id << "    Node Name: " << Get_Name() << endl;
        out << "Value set: ";
        for (int i = 0; i < value_size; ++i) out << value[i] << " ";
        out << endl;
        out << "Error set: ";
        for (int i = 0; i < error_size; ++i) out << error[i] << " ";
        out << endl;

        in_links.Reset_To_Head();
        for (int i = 0; i < in_links.Count(); ++i) {
            out << "    In Link ID : " << in_links.Curr()->Get_ID()
                << "    Link Name: " << in_links.Curr()->Get_Name()
                << "    Source Node: " << in_links.Curr()->In_Node()->Get_ID()
                << "    Value Set: ";
            for (int j = 0; j < in_links.Curr()->Get_Set_Size(); ++j)
                out << in_links.Curr()->Get_Value(j) << " ";
            out << endl;
            in_links.Next();
        }

        out_links.Reset_To_Head();
        for (int i = 0; i < out_links.Count(); ++i) {
            out << "    Out Link ID : " << out_links.Curr()->Get_ID()
                << "    Link Name: " << out_links.Curr()->Get_Name()
                << "    Source Node: " << out_links.Curr()->In_Node()->Get_ID()
                << "    Value Set: ";
            for (int j = 0; j < out_links.Curr()->Get_Set_Size(); ++j)
                out << out_links.Curr()->Get_Value(j) << " ";
            out << endl;
            out_links.Next();
        }
        out << endl;
    }

    friend void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link *link);

    friend void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link &link);

    friend void Connect(Base_Node *from_node, Base_Node *to_node, Base_Link *link);

    friend int Disconnect(Base_Node *from_node, Base_Node *to_node);

    friend double RANDOM(double lower_bound, double upper_bound);
};

int Base_Node::ticket = -1;

void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link *link) {
    from_node.Create_Link_To(to_node, link);
}

void Connect(Base_Node &from_node, Base_Node &to_node, Base_Link &link) {
    from_node.Create_Link_To(to_node, &link);
}

void Connect(Base_Node *from_node, Base_Node *to_node, Base_Link *link) {
    from_node->Create_Link_To(*to_node, link);
}

int Disconnect(Base_Node *from_node, Base_Node *to_node) {
    LList<Base_Link> *out_links = from_node->Out_Links();
    int flag = 0;
    out_links->Reset_To_Head();
    for (int i = 0; i < out_links->Count(); ++i) { // for each output link
        if (out_links->Curr()->Out_Node() == to_node) {
            flag = 1;
            break;
        }
        out_links->Next();
    }
    if (flag == 1) // link exists, delete it from both nodes.
    {
        out_links->Curr()->Out_Node()->In_Links()->Del(out_links->Curr());
        out_links->Del_Node();
        return 1;
    } else return 0;
}

double RANDOM(double lower_bound, double upper_bound) {
    return ((double) (rand() % RAND_MAX) / (double) RAND_MAX) *
           (upper_bound - lower_bound) + lower_bound;
}


// ----------------------------------- //
/// Base Link Definitions that need to be defined after the Base_Node class
/// because they reference specific Base_Node Members

double Base_Link::In_Value(int mode) { return in_node->Get_Value(mode); }

double Base_Link::Out_Value(int mode) { return out_node->Get_Value(mode); }

double Base_Link::In_Error(int mode) { return in_node->Get_Error(mode); }

double Base_Link::Out_Error(int mode) { return out_node->Get_Error(mode); }

double Base_Link::Weighted_In_Value(int mode) { return in_node->Get_Value(mode) * value[WEIGHT]; }

double Base_Link::Weighted_Out_Value(int mode) { return out_node->Get_Value(mode) * value[WEIGHT]; }

double Base_Link::Weighted_In_Error(int mode) { return in_node->Get_Error(mode) * value[WEIGHT]; }

double Base_Link::Weighted_Out_Error(int mode) { return out_node->Get_Error(mode) * value[WEIGHT]; }

/*This derived class provides a generic freed-forward neural-network node
* which can be used by ADALINEs and Backprop networks.*/
class Feed_Forward_Node : public Base_Node {
protected:
    virtual double Transfer_Function(double value) { return value; }

public:
    Feed_Forward_Node(int v_size = 1, int e_size = 1) : Base_Node(v_size, e_size) {}

    virtual void Run(int mode = 0) {
        in_links.Reset_To_Head();
        double total = 0.0;
        int cnt = in_links.Count();
        for (int i = 0; i < cnt; ++i) // for each node's input link
            total += in_links.Curr()->Weighted_In_Value(),
                    in_links.Next();
        value[mode] = Transfer_Function(total);
    }

    virtual char *Get_Name() {
        static char name[] = "FEED_FORWARD_NODE";
        return name;
    }
};


class Base_Network : public Base_Node   // Base Network Node
{
protected:
    int num_nodes;              // Number of nodes in Network
    int num_links;              // Number of links in Network
    Base_Node **node;          // Array of base nodes
    Base_Link **link;          // array of base links

    virtual void Create_Network() {}

    virtual void Load_Inputs() {}

    virtual void Save_Nodes_Links(ofstream &outfile) {
        outfile << num_nodes << endl;
        outfile << num_links << endl;
        for (int i = 0; i < num_nodes; ++i) node[i]->Save(outfile);
        for (int i = 0; i < num_links; ++i) link[i]->Save(outfile);
    }

    virtual void Load_Nodes_Links(ifstream &infile) {
        infile >> num_nodes;
        infile >> num_links;
        Create_Network();
        for (int i = 0; i < num_nodes; ++i) node[i]->Load(infile); // Load all Nodes.
        for (int i = 0; i < num_links; ++i) link[i]->Load(infile); // Load all Links.
    }

public:
    Base_Network() : Base_Node(0.0), num_nodes(0), num_links(0), node(NULL), link(NULL) {}

    ~Base_Network() {
        if (node) {
            for (int i = 0; i < num_nodes; ++i) delete node[i]; // free all nodes.
            for (int i = 0; i < num_links; ++i) delete link[i]; // free all links.
            delete[]node;
            delete[]link;
        }
    }

    virtual void Epoch(int code = 0) {
        for (int i = 0; i < num_nodes; ++i) node[i]->Epoch(code);
        for (int i = 0; i < num_links; ++i) link[i]->Epoch(code);
    }

    virtual void Print(ofstream &outfile) { for (int i = 0; i < num_nodes; ++i) node[i]->Print(outfile); }

    virtual char *Get_Name() {
        static char name[] = "BASE_NETWORK";
        return name;
    }
};

#endif //MYDNN_BASE_H

#pragma clang diagnostic pop