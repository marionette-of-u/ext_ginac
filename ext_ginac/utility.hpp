#ifndef EXT_GINAC_UTILITY_HPP
#define EXT_GINAC_UTILITY_HPP

#include <ginac/ginac.h>

namespace ExtGiNaC{
    using namespace GiNaC;
    namespace detail{
        inline bool tryfactsubs(const ex &origfactor, const ex &patternfactor, int &nummatches, exmap &repls){
            ex origbase;
            int origexponent, origexpsign;
            if(is_exactly_a<power>(origfactor) && origfactor.op(1).info(info_flags::integer)){
               origbase = origfactor.op(0);
                int expon = ex_to<numeric>(origfactor.op(1)).to_int();
                origexponent = expon > 0 ? expon : -expon;
                origexpsign = expon > 0 ? 1 : -1;
            }else{
                origbase = origfactor;
                origexponent = 1;
                origexpsign = 1;
            }
            ex patternbase;
            int patternexponent, patternexpsign;
            if(is_exactly_a<power>(patternfactor) && patternfactor.op(1).info(info_flags::integer)){
                patternbase = patternfactor.op(0);
                int expon = ex_to<numeric>(patternfactor.op(1)).to_int();
                if(expon > 0){
                    patternexponent = expon;
                    patternexpsign = 1;
                }else{
                    patternexponent = -expon;
                    patternexpsign = -1;
                }
            }else{
                patternbase = patternfactor;
                patternexponent = 1;
                patternexpsign = 1;
            }
            exmap saverepls = repls;
            if(origexponent < patternexponent || origexpsign != patternexpsign || !origbase.match(patternbase,saverepls)){
                return false;
            }
            repls = saverepls;
            int newnummatches = origexponent / patternexponent;
            if(newnummatches < nummatches){ nummatches = newnummatches; }
            return true;
        }
    }
}

#endif

