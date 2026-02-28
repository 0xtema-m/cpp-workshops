#include <iostream>
#include <type_traits>

template<typename T>
struct TWrapper {
    T value;
};

template<>
struct TWrapper<void> {};

template<typename T>
struct is_void_wrapper : std::false_type {};

template<>
struct is_void_wrapper<TWrapper<void>> : std::true_type {};

template<typename T>
struct is_wrapper : std::false_type {};

template<typename T>
struct is_wrapper<TWrapper<T>> : std::true_type {};

template<size_t idx, typename T, typename... Ts, size_t... ids>
auto get_non_void_ids(std::integer_sequence<size_t, ids...>) {
    if constexpr (!is_void_wrapper<T>::value) {
        if constexpr (sizeof...(Ts) == 0) {
            return std::integer_sequence<size_t, ids..., idx>();
        } else {
            return get_non_void_ids<idx + 1, Ts...>(
                std::integer_sequence<size_t, ids..., idx>{}
            );
        }
    } else {
        if constexpr (sizeof...(Ts) == 0) {
            return std::integer_sequence<size_t, ids...>();
        } else {
            return get_non_void_ids<idx + 1, Ts...>(
                std::integer_sequence<size_t, ids...>{}
            );
        }
    }
}

template<size_t curr, size_t end, typename Front, typename... Args>
auto get_arg_impl(Front&& front, Args&&... args) {
    if constexpr (curr == end) {
        if constexpr (is_wrapper<std::decay_t<Front>>::value) {
            return std::forward<Front>(front).value;
        } else {
            return std::forward<Front>(front);
        }
    } else {
        return get_arg_impl<curr + 1, end>(std::forward<Args>(args)...);
    }
}

template<size_t idx, typename... Args>
auto get_arg(Args&&... args) {
    return get_arg_impl<0, idx>(args...);
}

template<typename Call, size_t... ids, typename... Args>
auto func_impl(Call call, std::integer_sequence<size_t, ids...>, Args&&... args) {
    return call(
        get_arg<ids>(std::forward<Args>(args)...)...
    );
}

template<typename Call, typename... Args>
auto func(Call call, Args&&... args) {
    constexpr size_t sz = 1;
    using non_void_ids = decltype(
        get_non_void_ids<0, std::decay_t<Args>...>(
            std::integer_sequence<size_t>{}
        )
    );
    return func_impl(call, non_void_ids{}, std::forward<Args>(args)...);
}

double add(int a, double b) {
    return a + b;
}

int main() {
    using TVoid = TWrapper<void>;
    std::cout << func(add, TVoid{}, TWrapper<int>{1}, TVoid{}, 2.5, TVoid{});
}
