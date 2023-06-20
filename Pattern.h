#ifndef SUPPORTCLASSES_H_
#define SUPPORTCLASSES_H_

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <stdarg.h>

class Pattern
{
private:
    double *in_set;         // PPointer to input pattern array
    double *out_set;        // pointer to output pattern array
    int id;                 // Pattern Identification Number
    int in_size, out_size;  // Input and Output pattern sizes

public:
    // Constructor 1: Define in/out sizes with no values initialize
    Pattern(int inSize, int outSize)
    : in_size(inSize), out_size(outSize), in_set(new double[inSize]), out_set(new double[outSize]) {}

    Pattern(int id, int inSize, int outSize, ...)
            : id(id), in_size(inSize), out_size(outSize), in_set(new double[inSize]), out_set(new double[outSize])
    {
        va_list v1; // takes as elements the arguments given as parameters after the id, parameter in constructor.
        va_start(v1, outSize); // Initialize va_list with the last named parameter before the ellipsis

        for (int i = 0; i < inSize; ++i)
            in_set[i] = va_arg(v1, double);

        for (int i = 0; i < outSize; ++i)
            out_set[i] = va_arg(v1, double);

        va_end(v1);
    }

    // Constructor 3: Define in/out sizes and init values using arrays of the input and output values
    Pattern(double *inSet, double *outSet, int id, int inSize, int outSize) : in_set(inSet), out_set(outSet), id(id),
                                                                              in_size(inSize), out_size(outSize) {
        for (int i = 0; i < in_size; ++i)
            in_set[i] = inSet[i];

        for (int i = 0; i < out_size; ++i)
            out_set[i] = outSet[i];
    }
    // Constructor 4: Define in/out sizes and init values by readin values from an input file
    Pattern(int inSize, int outSize, std::ifstream &infile)
            : in_size(inSize), out_size(outSize), in_set(new double[inSize]), out_set(new double[outSize]) {
        infile >> id;
        Load(infile);
    }

    ~Pattern() {delete in_set; delete out_set;}

    // Get Input Pattern array values:
    double In(int id) {return in_set[id];}

    // Get Output Pattern array values:
    double Out(int id) {return out_set[id];}

    void Set_In(int id, double value) {in_set[id] = value;}

    void Set_Out(int id, double value) {out_set[id] = value;}

    int getID() const { return id; }

    int InSize() const { return in_size; }

    int OutSize() const { return out_size; }

    void Save( std::ofstream & outfile) {
        outfile << id << "\t";
        for (int i = 0; i < in_size; ++i)
            outfile << in_set[i] << "\t";

        for (int i = 0; i < out_size; ++i) {
            outfile << out_set[i];
            if (i != out_size-1) outfile << '\t';
        }
        outfile << std::endl;
    }

    void Load(std::ifstream &infile) {
        for (int i = 0; i < in_size; ++i)
            infile >> in_set[i];

        for (int i = 0; i < out_size; ++i)
            infile >> out_set[i];

        char ch;
        ch=infile.peek();
        while (ch=='\n' || ch == EOF) {
            ch = infile.get();
            if (ch == EOF) break;
            ch=infile.peek();
        }
    }

    void Print() {
        std::cout << "ID: " << id << "  In: ";
        for (int i = 0; i < in_size; ++i)
            std::cout << in_set[i] << " ";
        std::cout << "    Out: ";
        for (int i = 0; i < out_size; ++i)
            std::cout << out_set[i] << " ";
        std::cout << std::endl;


    }
};


#endif // SUPPORTCLASSES_H_
