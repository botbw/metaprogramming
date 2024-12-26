#include <type_traits>
#include <iostream>

#ifdef META_STATIC_ASSERT
#include <tuple>  // use std::tuple for static testing
#endif

namespace meta
{
    template <typename T>
    inline void print_type_name() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    /////////////////////// has type ////////////////////////////
    template <typename T>
    struct has_type
    {
        using type = T;
    };

    //////////////////////// static if ////////////////////////////
    template <bool COND, typename THEN, typename ELSE>
    struct static_if;

    template <typename THEN, typename ELSE>
    struct static_if<true, THEN, ELSE> : has_type<THEN>
    {
    };

    template <typename THEN, typename ELSE>
    struct static_if<false, THEN, ELSE> : has_type<ELSE>
    {
    };

    template <bool COND, typename THEN, typename ELSE>
    using static_if_t = typename static_if<COND, THEN, ELSE>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<int, static_if_t<true, int, char>>);
    static_assert(std::is_same_v<char, static_if_t<false, int, char>>);
#endif

    /////////////////////// empty ////////////////////////////

    template <typename>
    struct empty;

    template <template <typename...> class T_CLASS>
    struct empty<T_CLASS<>> : has_type<std::true_type>
    {
    };

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct empty<T_CLASS<T0, T1toN...>> : has_type<std::false_type>
    {
    };

    template <typename T>
    static constexpr bool empty_v = empty<T>::type::value;

#ifdef META_STATIC_ASSERT
    static_assert(empty_v<std::tuple<>>);
    static_assert(empty_v<std::tuple<int>> == false);
#endif

    //////////////////////// front //////////////////////////////
    template <typename>
    struct front;

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct front<T_CLASS<T0, T1toN...>>
    {
        using type = T0;
    };
    template <typename T>
    using front_t = typename front<T>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<int, front_t<std::tuple<int, bool, char>>>);
#endif

    //////////////////////// pop front //////////////////////////////

    template <typename>
    struct pop_front;

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct pop_front<T_CLASS<T0, T1toN...>> : has_type<T_CLASS<T1toN...>>
    {
    };

    template <typename T>
    using pop_front_t = typename pop_front<T>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<std::tuple<bool, char>, pop_front_t<std::tuple<int, bool, char>>>);
    static_assert(std::is_same_v<std::tuple<char>, pop_front_t<std::tuple<bool, char>>>);
    static_assert(std::is_same_v<std::tuple<>, pop_front_t<std::tuple<char>>>);
#endif
    /////////////////////// push front ///////////////////////////
    template <typename, typename>
    struct push_front;

    template <template <typename...> class T_CLASS, typename... T1toN, typename T0>
    struct push_front<T_CLASS<T1toN...>, T0>: has_type<T_CLASS<T0, T1toN...>> {};

    template <typename LIST, typename T0>
    using push_front_t = typename push_front<LIST, T0>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<std::tuple<int>, push_front_t<std::tuple<>, int>>);
    static_assert(std::is_same_v<std::tuple<char, float, int>, push_front_t<std::tuple<float, int>, char>>);
    static_assert(std::is_same_v<std::tuple<char, float, int>, push_front_t<std::tuple<float, int>, int>> == false);
#endif

    //////////////////////// back ////////////////////////////
    template <typename>
    struct back;

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct back<T_CLASS<T0, T1toN...>>: back<T_CLASS<T1toN...>> {};

    template <template <typename...> class T_CLASS, typename TN>
    struct back<T_CLASS<TN>>: has_type<TN> {};

    template <typename T>
    using back_t = typename back<T>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<int, back_t<std::tuple<float, char, int>>>);
    static_assert(std::is_same_v<float, back_t<std::tuple<float, char, int, int, int, float>>>);
#endif

    //////////////////////// pop back ////////////////////////
    template <typename>
    struct pop_back;

    template <typename, typename>
    struct pop_back_impl;

    template <template <typename...> class CUR, typename CUR_T0, typename... CUR_T1toN, template <typename...> class RET, typename... RET_T0toN>
    struct pop_back_impl<
        CUR<CUR_T0, CUR_T1toN...>,
        RET<RET_T0toN...>
    >: pop_back_impl<
        CUR<CUR_T1toN...>,
        RET<RET_T0toN..., CUR_T0>
    > {};

    template <template <typename...> class CUR, typename CUR_T0, template <typename...> class RET, typename... RET_T0toN>
    struct pop_back_impl<
        CUR<CUR_T0>,
        RET<RET_T0toN...>
    >: has_type<RET<RET_T0toN...>> {};

    template <template <typename...> class CUR, typename... Ts>
    struct pop_back<CUR<Ts...>>: pop_back_impl<CUR<Ts...>, CUR<>> {};

    template <typename T>
    using pop_back_t = typename pop_back<T>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<std::tuple<float, char>, pop_back_t<std::tuple<float, char, int>>>);
    static_assert(std::is_same_v<std::tuple<float, char, int>, pop_back_t<std::tuple<float, char, int, long>>>);
#endif

    //////////////////////// at //////////////////////////////
    template <typename, int>
    struct at;

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN, int N>
    struct at<T_CLASS<T0, T1toN...>, N>: at<T_CLASS<T1toN...>, N - 1> {};

    template <template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct at<T_CLASS<T0, T1toN...>, 0>: has_type<T0> {};

    template <typename T, int N>
    using at_t = typename at<T, N>::type;

#ifdef META_STATIC_ASSERT
    static_assert(std::is_same_v<int, at_t<std::tuple<int, float, int, char>, 0>>);
    static_assert(std::is_same_v<int, at_t<std::tuple<int, float, int, char>, 2>>);
#endif

    //////////////////////// contain type /////////////////////////
    template <typename, typename>
    struct contain_type;

    template <typename TYPE, template <typename...> class T_CLASS>
    struct contain_type<TYPE, T_CLASS<>>: std::false_type {};

    template <typename TYPE, template <typename...> class T_CLASS, typename T0, typename... T1toN>
    struct contain_type<TYPE, T_CLASS<T0, T1toN...>> : static_if<
                                                           /* COND */ std::is_same_v<TYPE, T0>,
                                                           /* THEN */ std::true_type,
                                                           /* ELSE */ typename contain_type<TYPE, T_CLASS<T1toN...>>::type
                                                        >
    {
    };

#ifdef META_STATIC_ASSERT
    static_assert(contain_type<int, std::tuple<int, bool, char>>::type::value);
    static_assert(contain_type<float, std::tuple<int, bool, char>>::type::value == false);
    static_assert(contain_type<int, std::tuple<>>::type::value == false);
#endif
}
