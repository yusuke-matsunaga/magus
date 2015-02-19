#ifndef VSMVALUE_H
#define VSMVALUE_H

/// @file VsmValue.h
/// @brief VsmValue のヘッダファイル
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
union VsmValue {
  Ymsl_INT    int_value;
  Ymsl_FLOAT  float_value;
  Ymsl_OBJPTR obj_value;
};

END_NAMESPACE_YM_YMSL


#endif // VSMVALUE_H
