#include <iostream>

#include "tpbst.hpp"

int main() {
    TwoPhaseBST<int> tpbst;

    tpbst.print()
            .print("ceng213")
            .print("ceng213", "sec1")
            .insert("ceng213", "sec2", 26)
            .insert("ceng213", "sec1", 25)
            .insert("ceng213", "sec3", 24)
            .insert("ceng213", "sec4", 28)
            .insert("ceng477", "sec2", 50)
            .print()
            .print("ceng213")
            .print("ceng477")
            .print("ceng213", "sec1")
            .print("ceng477", "sec1")
            .remove("ceng213", "sec2")
            .remove("ceng477", "sec2")
            .print();

    std::cout << "Number of enrolled students in ceng213 - sec3 is " << *(tpbst.find("ceng213", "sec3")) << std::endl;

    return 0;
}
