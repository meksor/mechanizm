#include "gui/application.h"
#include <cxxmidi/file.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
    mechanizm::Application app(argc, argv);
    cxxmidi::File file("../midi/test.mid");

    for (auto tr=file.begin(); tr != file.end(); ++tr) {
        std::cout << "track" << "\n";

        for (auto ev=tr->begin(); ev != tr->end(); ++ev) {
            std::cout << "message" << " ";
            for (auto b=ev->begin(); b != ev->end(); ++b) {
                std::cout << *b << " ";
            }
            std::cout << ev->Dt() << "\n";
        }
        std::cout << "\n";
    }
    return app.exec();
}
