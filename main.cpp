#include <typeinfo>
#include <cxxabi.h>
#include <cstdlib>
class demangle{
private:
    char *realname;

public:
    demangle(const std::type_info &ti){
        int status = 0;
        realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
    }

    demangle(const demangle&) = delete;
    demangle &operator =(const demangle&) = delete;
    ~demangle(){
        std::free(realname);
    }

    operator const char *() const{
        return realname;
    }
};


#include <iostream>
#include "ext_ginac/ext_ginac.hpp"

int main(){
    using namespace ExtGiNaC;

    // logarithm2 test
    {
        symbol x("x"), y("y");
        symbol a("a"), b("b");
        ex e = log2(x, y);
        std::cout << latex;
        std::cout << e << std::endl;
        std::cout << e.subs(lst(x == 2, y == 1024)).eval() << std::endl;
        std::cout << e.subs(lst(x == 2, y == 1024)).evalf() << std::endl;
        std::cout << e.op(0) << std::endl;
        std::cout << e.op(1) << std::endl;
        std::cout << e.series(y == a, 4) << std::endl;
        std::cout << e.series(x == b, 4) << std::endl;
        std::cout << e.diff(y) << std::endl;
        std::cout << e.diff(x) << std::endl;
    }

    // alternate_form test
    {
        symbol x("x"), y("y"), z("z");
        auto f = [&z](const ex &expression){
            return pow(z, expression);
        };

        symbol a("a"), b("b"), c("c");
        ex p = exp(pow(a, b)) / exp(pow(a, c));
        ex q = pow(p * exp(pow(a, c)), p * exp(pow(b, c)));

        std::cout << q << " = " << reduce(q) << std::endl;
        std::cout << (pow(exp(Pi), I) + 1) << " = " << reduce(pow(exp(Pi), I) + 1) << std::endl;
    }

    return 0;
}


