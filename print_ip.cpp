#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <list>

using uchar = unsigned char;

namespace impl 
{
    template<typename T>
    std::enable_if_t<std::is_integral_v<T>,void>
    print_ip(T arg)
    {
        uchar* s = (uchar*) &arg + sizeof(arg)-1;
        for (size_t i = 0; i<sizeof(arg);i++)
        {
            if (i)
                printf(".");
            printf("%i",*(s-i));
        }
        printf("\n");
    }

    template<typename T>
    std::enable_if_t<std::is_convertible_v<T, std::string>,void>
    //std::enable_if_t<std::is_same<T, std::basic_string<typename T::value_type>>::value,void>
    print_ip(T arg)
    {
        std::string test;
        printf("%s",arg.c_str());
        printf("\n");
    }


    template<typename T>
    std::enable_if_t<std::is_same_v<T,typename std::vector<typename T::value_type>>||
                     std::is_same_v<T,typename std::list<typename T::value_type>>,void>
    print_ip(const T& arg)
    {
        std::string result;
        for (auto const &ip_part: arg)
        {
            if (!result.empty())
                result.append(".");
            result.append(std::to_string(ip_part));
        }
        printf("%s\n", result.c_str());
    }

    template<typename T, typename... Args,
    std::enable_if<std::is_same_v<T,Args> && ...>::type * = nullptr>
    print_ip(std::tuple<T,Args> arg)
    {
        int a = sizeof(arg);
        a = a+1;
        printf("tuple\n");
    }
}

template<typename T>
void print_ip(const T& arg)
{
    impl::print_ip(arg);
}



int main(/*int argc, char const *argv[]*/)
{
    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} ); // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0

    return 0;
}
