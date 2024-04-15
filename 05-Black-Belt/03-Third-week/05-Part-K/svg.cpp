#include "svg.h"
#include "test_runner.h"

using namespace std;

namespace Svg {
std::ostream& operator<< (std::ostream& sout, const Rgb& rhs) {
    sout << "rgb(" << rhs.red << "," << rhs.green << "," << rhs.blue << ")";
    return sout;
}

std::ostream& operator<< (std::ostream& sout, const Rgba& rhs) {
    sout << "rgba(" << rhs.red << "," << rhs.green << "," << rhs.blue << "," << rhs.alpha << ")";
    return sout;
}

std::ostream& operator<< (std::ostream& sout, const Color::NoValue& rhs) {
    sout << "none";
    return sout;
}

std::ostream& operator<< (std::ostream& sout, const Color& rhs) {
    std::visit([&sout](const auto& obj){sout << obj;}, rhs.color);
    return sout;
}

}
