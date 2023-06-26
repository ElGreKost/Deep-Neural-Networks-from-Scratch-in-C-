//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//
//// Include the Gnuplot C++ interface
//#include "gnuplot-iostream.h"
//
//
//void plotCoordinates(const std::vector<std::pair<double, double>>& coordinates) {
//    // Create a Gnuplot object
//    Gnuplot gp;
//
//    // Create vectors to store x and y values separately
//    std::vector<double> xValues;
//    std::vector<double> yValues;
//
//    // Extract x and y values from the coordinates
//    for (const auto& coordinate : coordinates) {
//        xValues.push_back(coordinate.first);
//        yValues.push_back(coordinate.second);
//    }
//
//    // Send the x and y values to Gnuplot and plot them
//    gp << "plot '-' with points\n";
//    gp.send1d(boost::make_tuple(xValues, yValues));
//}
