#include <Wt/WApplication.h>
#include "WordleApplication.h"

int main(int argc, char** argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment& env) {
        return std::make_unique<WordleApplication>(env);
    });
}