#ifndef EXT_GINAC_REWRITE_SYSTEM_HPP
#define EXT_GINAC_REWRITE_SYSTEM_HPP

#include <ginac/ginac.h>
#include "logarithm2.hpp"

namespace ExtGiNaC{
    using namespace GiNaC;
    namespace detail{
        inline ex reduce_proc(const ex &expression){
            static const exmap equations = {
                // exp & log2 & pow
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
                },
                {
                    pow(wild(0), log2(wild(0), wild(1))),
                    wild(1)
                },
                {
                    pow(wild(0), log2(wild(0), wild(1))),
                    wild(1)
                },
                {
                    log2(wild(0), wild(1)) * log2(wild(2), wild(0)),
                    log2(wild(2), wild(1))
                },
                {
                    log2(wild(0), wild(1)) / log2(wild(0), wild(2)),
                    log2(wild(2), wild(1))
                },
                {
                    1 / log2(wild(0), wild(1)),
                    log2(wild(1), wild(0))
                },
                {
                    log2(wild(0), wild(1)) + log2(wild(0), wild(2)),
                    log2(wild(0), wild(1) * wild(2))
                },
                {
                    log2(wild(0), wild(1)) - log2(wild(0), wild(2)),
                    log2(wild(0), wild(1) / wild(2))
                }
            };
            ex current = expression;
            for(; ; ){
                ex temp = current.subs(equations, subs_options::algebraic);
                if(temp == current){ break; }
                current = temp;
            }
            return current;
        }
    }

    inline ex reduce(const ex &expression){
        static const symbol e;
        static const ex p = exp(wild(0)) == pow(e, wild(0));
        static const lst q = {
            pow(e, wild(0)) == exp(wild(0)),
            e == exp(1)
        };
        return detail::reduce_proc(expression.subs(p)).subs(q);
    }
}

#endif

