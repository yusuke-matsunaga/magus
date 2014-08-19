#ifndef YMUTILS_STL_H
#define YMUTILS_STL_H

/// @file YmUtils/stl.h
/// @brief stl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <new>
#include <utility>
#include <string>
#include <vector>
#include <list>

#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#include <ext/algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#define HASH_NAMESPACE std
#elif __GNUC__ >= 3
#include <ext/algorithm>
#include <ext/functional>
#include <ext/hash_set>
#include <ext/hash_map>
#define HASH_NAMESPACE __gnu_cxx
#define unordered_set hash_set
#define unordered_map hash_map
#define constexpr const
#else
#include <algorithm>
#include <functional>
#include <hash_set>
#include <hash_map>
#define HASH_NAMESPACE std
#define unordered_set hash_set
#define unordered_map hash_map
#define constexpr const
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// std:: にある汎用的な名前を Oth の名前空間にインポートする
//////////////////////////////////////////////////////////////////////

using std::istream;
using std::ifstream;
using std::istringstream;
using std::ostream;
using std::ofstream;
using std::ostringstream;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::ios;
using std::setw;
using std::setprecision;
using std::setfill;
using std::hex;
using std::oct;
using std::dec;
using std::fixed;
using std::scientific;
using std::exception;
using std::bad_alloc;
using std::pair;
using std::make_pair;
using std::string;
using std::vector;
using std::list;

using HASH_NAMESPACE::hash;
using HASH_NAMESPACE::unordered_set;
using HASH_NAMESPACE::unordered_map;

END_NAMESPACE_YM


#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
BEGIN_NAMESPACE_HASH

// string/const string の hash_set/map 用のハッシュ関数クラス
// hash<string>, hash<const string>定義．
// SGI STL の string クラス (v3.1 以降)にはこの定義は不要．
#if !defined(__SGI_STL_STRING) && !defined(DEFINE_HASH_STRING)
#define DEFINE_HASH_STRING

template<typename __Key> struct hash;

template <>
struct
hash<std::string>
{
  size_t
  operator()(const std::string& __x) const
  {
    unsigned long __h = 0;
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;

    return size_t(__h);
  }
};

template <>
struct
hash<const std::string>
{
  size_t
  operator()(const std::string& __x) const {
    unsigned long __h = 0;
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;

    return size_t(__h);
  }
};

#endif // __SGI_STL_STRING

END_NAMESPACE_HASH
#endif

#endif // YMUTILS_STL_H
