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
        std::cout << "-------- logarithm2" << std::endl;

        symbol x("x"), y("y");
        symbol a("a"), b("b");

        ex e = log2(x, y);

        //std::cout << latex;
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

    // reduce test
    {
        std::cout << "-------- reduce" << std::endl;

        symbol x("x"), y("y"), z("z");
        symbol a("a"), b("b"), c("c");

        {
            ex p[] = {
                //pass
                pow(a, log2(a, x)),
                // !!
                log2(a, x) + log2(a, y),
                // !!
                log2(a, x) - log2(a, y),
                // pass
                log2(a, pow(x, b)),
                // pass
                log2(a, 1 / x),
                // !!
                log2(a, x) * log2(b, a),
                // !!
                log2(b, x) / log2(b, a),
                // !!
                1 / log2(x, a),
                // pass
                log2(1 / a, x)
            };
            for(int i = 0, i_ = sizeof(p) / sizeof(*p); i < i_; ++i){
                std::cout << (i + 1) << ") " << p[i] << " = " << reduce(p[i]) << std::endl;
            }
        }

        std::cout << (pow(exp(Pi), I) + 1) << " = " << reduce(pow(exp(Pi), I) + 1) << std::endl;
    }

    return 0;
}


