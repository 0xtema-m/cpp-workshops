
namespace ohtuzh {

    template<typename Iterator, typename T>
    Iterator find(Iterator begin, Iterator end, const T& value) {
        for (auto it = begin; it != end; ++it) {
            if (*it == value) {
                return it;
            }
        }
        return end;
        // for (; begin != end && *begin != value; ++begin) {}
        // return begin;
    }

    template<typename Iterator, typename Functor>
    Iterator find_if(Iterator begin, Iterator end, Functor functor) {
        for (auto it = begin; it != end; ++it) {
            if (functor(*it)) {
                return it;
            }
        }
        return end;
        // for (; begin != end && !functor(*begin); ++begin) {}
        // return begin;
    }

    template<typename Iterator, typename T>
    T accumulate(Iterator begin, Iterator end, T init) {
        for (; begin != end; ++begin) {
            init += *begin;
        }
        return init;
    }

    template<typename Iterator, typename T, typename BinaryOperator>
    T accumulate(Iterator begin, Iterator end, T init, BinaryOperator oper) {
        for (; begin != end; ++begin) {
            init = oper(init, *begin);
        }
        return init;
    }

} // namespace ohtuzh
