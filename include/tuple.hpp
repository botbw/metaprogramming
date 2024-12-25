#include <type_traits>

#ifdef META_STATIC_ASSERT
#include <tuple>
#endif

namespace meta
{

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
