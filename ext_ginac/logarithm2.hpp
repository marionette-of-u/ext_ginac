#ifndef EXT_GINAC_LOGARITHM2_HPP
#define EXT_GINAC_LOGARITHM2_HPP

#include <ginac/ginac.h>
#include "ext_ginac.hpp"

namespace ExtGiNaC{
    using namespace GiNaC;

    namespace detail{
        template<class Dummy = void>
        class logarithm2 : public basic{
            GINAC_DECLARE_REGISTERED_CLASS(logarithm2, basic);

        public:
            logarithm2(const ex &x_, const ex &y_) : basic(), x(x_), y(y_){}
            logarithm2(const logarithm2 &other) : basic(other), x(other.x), y(other.y){}

        public:
            unsigned int precedence() const{ return basic::precedence(); }
            bool info(unsigned int inf) const{
                switch(inf){
                case info_flags::numeric:
                case info_flags::real:
                    return x.info(inf) && y.info(inf);

                case info_flags::algebraic:
                    return true;

                default:
                    break;
                }
                return false;
            }

            size_t nops() const{
                return 2;
            }

            ex op(std::size_t i) const{
                switch(i){
                case 0:
                    return x;

                case 1:
                    return y;

                default:
                    throw(std::range_error("logarithm2::op(): no such operand"));
                }
            }

            ex map(map_function &f) const{
                const ex &mapped_x = f(x), &mapped_y = f(y);
                if(!are_ex_trivially_equal(x, mapped_x) || !are_ex_trivially_equal(y, mapped_y)){
                    return (new logarithm2(mapped_x, mapped_y))->setflag(status_flags::dynallocated);
                }else{
                    return *this;
                }
            }

            bool is_polynomial(const ex &var) const{
                return false;
            }

            int degree(const ex&) const{
                return 0;
            }

            int ldegree(const ex&) const{
                return 0;
            }

            ex coeff(const ex&, int n = 1) const{
                return 1;
            }

            ex eval(int level = 0) const{
                if(level == 1 && (flags & status_flags::evaluated)){
                    return *this;
                }else if(level == -max_recursion_level){
                    throw(std::runtime_error("max recursion level reached"));
                }
                const ex
                    &e_x = level == 1 ? x : x.eval(level - 1),
                    &e_y = level == 1 ? y : y.eval(level - 1);
                // log(0, y) -> 0
                // log(x, 1) -> 0
                if(e_x.is_zero() || e_y == 1){
                    return 0;
                }
                // log(x, 0) -> -infinity
                if(e_y == 0){
                    throw(pole_error("logarithm2::eval(): log(0) = -infinity", 0));
                }
                // log(x, pow(y, p)) -> p * log(x, y)
                if(is_a<power>(e_y)){
                    return e_y.op(1) * logarithm2(e_x, e_y.op(0));
                }
                // log(x, y) * log(x', x) -> log(x', y)
                if(is_a<logarithm2>(e_x) && is_a<logarithm2>(e_y)){
                    return logarithm2(e_x.op(1), e_y.op(0));
                }
                // log(pow(x, a), y) -> ln(y) / (a * ln(x))
                if(is_a<power>(e_x)){
                    return log(e_y) / (e_x.op(1) * log(e_x.op(0)));
                }
                // raw form
                return this->hold();
            }

            ex evalf(int level = 0) const{
                return log(y).evalf() / log(x).evalf();
            }

            ex evalm() const{
                return (log(y) / log(x)).evalm();
            }

            ex series(const relational &s, int order, unsigned options = 0) const{
                return (log(y) / log(x)).series(s, order, options);
            }

            ex subs(const exmap &m, unsigned options = 0) const{
                const ex &e_x = x.subs(m, options);
                const ex &e_y = y.subs(m, options);
                if(!are_ex_trivially_equal(x, e_x) || !are_ex_trivially_equal(y, e_y)){
                    return logarithm2(e_x, e_y).subs_one_level(m, options);
                }
                if((options & subs_options::algebraic) == 0){
                    return subs_one_level(m, options);
                }
                for(const auto &item : m){
                    int nummatches = (std::numeric_limits<int>::max)();
                    exmap repls;
                    if(tryfactsubs(*this, item.first, nummatches, repls)){
                        ex
                            anum = item.second.subs(repls, subs_options::no_pattern),
                            aden = item.first.subs(repls, subs_options::no_pattern),
                            result = *this * logarithm2(anum / aden, nummatches);
                        return ex_to<basic>(result).subs_one_level(m, options);
                    }
                }
                return subs_one_level(m, options);
            }

            bool has(const ex &other, unsigned options = 0) const{
                return (log(y) / log(x)).has(other, options);
            }

            ex normal(exmap &repl, exmap &rev_lookup, int level = 0) const{
                return (log(y) / log(x)).normal(level);
            }

            ex to_rational(exmap &repl) const{
                return (log(y) / log(x)).to_rational(repl);
            }

            ex to_polynomial(exmap &repl) const{
                return (log(y) / log(x)).to_polynomial(repl);
            }

            ex conjugate() const{
                return (log(y) / log(x)).conjugate();
            }

            ex real_part() const{
                return (log(y) / log(x)).real_part();
            }

            ex imag_part() const{
                return (log(y) / log(x)).imag_part();
            }

            void archive(archive_node &n) const{
                inherited::archive(n);
                n.add_ex("log_base", x);
                n.add_ex("log_x", y);
            }

            void read_archive(const archive_node &n, lst &syms){
                inherited::read_archive(n, syms);
                n.find_ex("log_base", x, syms);
                n.find_ex("log_x", y, syms);
            }

            const char *get_class_name(){ return "log2"; }

            ex &let_op(std::size_t i){
                switch(i){
                case 0:
                    return x;

                case 1:
                    return y;

                default:
                    throw(std::range_error("logarithm2::let_op(): no such operand"));
                }
            }

        protected:
            ex derivative(const symbol &s) const{
                return logarithm2(x, exp(1)) / y;
            }

            ex eval_ncmul(const exvector &v) const{
                return (log(y) / log(x)).eval_ncmul(v);
            }

            unsigned int return_type() const{
                return (log(y) / log(x)).return_type();
            }

            return_type_t return_type_tinfo() const{
                return (log(y) / log(x)).return_type_tinfo();
            }

            ex expand(unsigned options = 0) const{
                return (log(y) / log(x)).expand(options);
            }

        private:
            void print_logarithm2(const print_context &c, const char *openbrace, const char *closebrace, int level) const{
                if(precedence() <= level){ c.s << openbrace << "("; }
                c.s << "(";
                x.print(c, precedence());
                c.s << ", ";
                y.print(c, precedence());
                c.s << ")";
                if(precedence() <= level){ c.s << ")" << closebrace; }
            }

            template<class T>
            void do_print_generic(const T &c, unsigned int level) const{
                c.s << "log";
                print_logarithm2(c, "", "", level);
            }
        
        public:
            void do_print_dflt(const print_dflt &c, unsigned int level) const{
                do_print_generic(c, level);
            }

            void do_print_csrc(const print_csrc &c, unsigned int level) const{
                do_print_generic(c, level);
            }

            void do_print_latex(const print_latex &c, unsigned int level) const{
                c.s << "\\log";
                print_logarithm2(c, "{", "}", level);
            }

        private:
            ex x, y;
        };

        template<>
        int logarithm2<>::compare_same_type(const basic &other) const{
            const logarithm2 &o = static_cast<const logarithm2&>(other);
            return op(0) < o.op(0) ? -1 : op(0) > o.op(0) ? 1 : op(1) < o.op(1) ? -1 : op(1) > o.op(1) ? 1 : 0;
        }
    }

    using logarithm2 = detail::logarithm2<>;

    inline ex log2(const ex &x, const ex &y){
        return logarithm2(x, y);
    }
}

template<>
GINAC_IMPLEMENT_REGISTERED_CLASS_OPT_T(
    ExtGiNaC::logarithm2,
    basic,
    print_func<print_dflt>(&ExtGiNaC::logarithm2::do_print_dflt).
    print_func<print_latex>(&ExtGiNaC::logarithm2::do_print_latex).
    print_func<print_csrc>(&ExtGiNaC::logarithm2::do_print_csrc)
);

#endif

