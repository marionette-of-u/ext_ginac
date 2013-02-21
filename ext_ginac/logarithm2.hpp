#ifndef EXT_GINAC_LOGARITHM2_HPP
#define EXT_GINAC_LOGARITHM2_HPP

#include <ginac/ginac.h>

namespace ExtGiNaC{
    using namespace GiNaC;
    namespace detail{
        template<class Dummy = void>
        struct logarithm2_template{
            logarithm2_template() = delete;

            logarithm2_template(const ex &x_, const ex &y_) : x(x_), y(y_){}
            
            logarithm2_template(const logarithm2_template &other) : x(other.x), y(other.y){}

            logarithm2_template(logarithm2_template &&other) : x(other.x), y(other.y){}

            const char *get_class_name(){ return "log2"; }

            std::size_t nops() const{
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

            ex x, y;
        };

        inline bool operator ==(const logarithm2_template<> &lhs, const logarithm2_template<> &rhs){
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }

        inline bool operator <(const logarithm2_template<> &lhs, const logarithm2_template<> &rhs){
            return lhs.x < rhs.x ? true : lhs.y < rhs.y;
        }
    }
}

namespace GiNaC{
    using logarithm2 = structure<ExtGiNaC::detail::logarithm2_template<>>;

    template<>
    ex logarithm2::series(const relational &r, int order, unsigned int options) const{
        const ExtGiNaC::detail::logarithm2_template<> &obj = get_struct();
        return (log(obj.y) / log(obj.x)).series(r, order, options);
    }

    template<>
    ex logarithm2::derivative(const symbol &s) const{
        const ExtGiNaC::detail::logarithm2_template<> &obj = get_struct();
        return (log(obj.y) / log(obj.x)).diff(s);
    }

    template<>
    std::size_t logarithm2::nops() const{
        return get_struct().nops();
    }

    template<>
    ex logarithm2::op(std::size_t i) const{
        return get_struct().op(i);
    }

    template<>
    ex &logarithm2::let_op(std::size_t i){
        return get_struct().let_op(i);
    }

    template<>
    ex logarithm2::subs(const exmap &m, unsigned int options) const{
        const ExtGiNaC::detail::logarithm2_template<> &obj = get_struct();
        return logarithm2(ExtGiNaC::detail::logarithm2_template<>(obj.x.subs(m, options), obj.y.subs(m, options)));
    }

    template<>
    ex logarithm2::map(map_function &f) const{
        const ExtGiNaC::detail::logarithm2_template<> &obj = get_struct();
        return (log(obj.y) / log(obj.x)).map(f);
    }

    template<>
    void logarithm2::print(const print_context &c, unsigned int level) const{
        if(is_a<print_tree>(c)){
            inherited::print(c, level);
        }
        const ExtGiNaC::detail::logarithm2_template<> &obj = get_struct();
        const unsigned int prec = precedence();
        if(level >= prec){ c.s << "("; }
        c.s << "log(" << obj.x << ", " << obj.y << ")";
        if(level >= prec){ c.s << ")"; }
    }
}

namespace ExtGiNaC{
    inline logarithm2 log2(const ex &x, const ex &y){
        return logarithm2(detail::logarithm2_template<>(x, y));
    }

    template<class T, class U>
    inline logarithm2 log2(const T &x, const U &y){
        return logarithm2(detail::logarithm2_template<>(x, y));
    }
}

#endif
