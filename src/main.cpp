#include "application.h"

int main(int argc, char *argv[]) {
    mechanizm::Application app(argc, argv);
    return app.exec();
}
