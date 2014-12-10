#ifndef YMSLVALUE_H
#define YMSLVALUE_H

/// @file YmslValue.h
/// @brief YmslValue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class 値を表す共用型
///
/// あんまり union は使いたくないんだけど
//////////////////////////////////////////////////////////////////////
union YmslValue {
  ymint64 int_value;
  double  float_value;
  YmslObj* obj_value;
};

END_NAMESPACE_YM_YMSL


#endif // YMSLVALUE_H
