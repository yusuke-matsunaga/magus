
/// @file StrListIDO_test.cc
/// @brief StrListIDO のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/StrListIDO.h"


BEGIN_NAMESPACE_YM

int
StrListIDO_test(int argc,
		const char** argv)
{
  vector<string> src_list;

  src_list.push_back(string("abcdefghijklmnopqrstuvwxyz"));
  src_list.push_back(string("ABCDEFGHJIKJLMNOPQRSTUVWXYZ"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));
  src_list.push_back(string("12345678901234567890"));

  StrListIDO ido(src_list);

  for ( ; ; ) {
    const ymuint BUFF_SIZE = 100;
    ymuint8 buff[BUFF_SIZE];
    ssize_t n = ido.read(buff, BUFF_SIZE);
    if ( n < 0 ) {
      cerr << "ERROR" << endl;
      return -1;
    }
    if ( n == 0 ) {
      break;
    }
    for (ymint i = 0; i < n; ++ i) {
      cout << static_cast<char>(buff[i]);
    }
  }

  return 0;
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  using namespace nsYm;

  return StrListIDO_test(argc, argv);
}
