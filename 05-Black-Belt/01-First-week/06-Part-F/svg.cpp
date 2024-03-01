#include "svg.h"
#include "test_runner.h"

using namespace std;

namespace Svg {
std::ostream& operator<< (std::ostream& sout, const Rgb& rhs) {
    sout << "rgb(" << uint16_t(rhs.red) << "," << uint16_t(rhs.green) << "," << uint16_t(rhs.blue) << ")";
    return sout;
}

std::ostream& operator<< (std::ostream& sout, const Color& rhs) {
    std::visit([&sout](const auto& obj){sout << obj;}, rhs.color);
    return sout;
}

std::ostream& operator<< (std::ostream& sout, const Color::NoValue& rhs) {
    sout << "none";
    return sout;
}
}
