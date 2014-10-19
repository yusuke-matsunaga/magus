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

#if defined(__clang__)
#include <algorithm>
#include <functional>
#elif __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
#include <ext/algorithm>
#include <functional>
#elif __GNUC__ >= 3
#include <ext/algorithm>
#include <ext/functional>
#define constexpr const
#else
#include <algorithm>
#include <functional>
#define constexpr const
#endif


//////////////////////////////////////////////////////////////////////
// std:: にある汎用的な名前を YmTools の名前空間にインポートする
//////////////////////////////////////////////////////////////////////
BEGIN_NAMESPACE_YM

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

END_NAMESPACE_YM

#endif // YMUTILS_STL_H
