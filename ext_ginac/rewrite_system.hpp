#ifndef EXT_GINAC_REWRITE_SYSTEM_HPP
#define EXT_GINAC_REWRITE_SYSTEM_HPP

#include <ginac/ginac.h>
#include "logarithm2.hpp"

namespace ExtGiNaC{
    using namespace GiNaC;
    namespace detail{
        inline ex exp_log_reduce(const ex &expression){
            static const exmap equations = {
                {
                    pow(wild(0), wild(1)) * pow(wild(0), wild(2)),
                    pow(wild(0), wild(1) + wild(2))
                },
                {
                    pow(pow(wild(0), wild(1)), wild(2)),
                    pow(wild(0), wild(1) * wild(2))
                },
                {
                    pow(wild(0), wild(1)) / pow(wild(0), wild(2)),
                    pow(wild(0), wild(1) - wild(2))
                }
            };
            ex current = expression;
            for(; ; ){
                const ex temp = current.subs(equations, subs_options::algebraic);
                if(temp == current){ break; }
                current = temp;
            }
            return current;
        }
    }

    inline ex reduce(const ex &expression){
        symbol e;
        return detail::exp_log_reduce(expression.subs(exp(wild(0)) == pow(e, wild(0)))).subs(pow(e, wild(0)) == exp(wild(0)));
    }
}

#endif

