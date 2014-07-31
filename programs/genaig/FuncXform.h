#ifndef FUNCXORM_H
#define FUNCXORM_h

/// @file FuncXform.h
/// @brief FuncXform のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FuncXform FuncXform.h "FuncXform.h"
/// @brief 関数変換を表すクラス
//////////////////////////////////////////////////////////////////////
struct FuncXform
{

  /// @brief コンストラクタ
  FuncXform(ymuint32 fv,
	    ymuint8 p0,
	    ymuint8 p1,
	    ymuint8 p2,
	    ymuint8 p3) :
    mVector(fv)
  {
    mPerm[0] = p0;
    mPerm[1] = p1;
    mPerm[2] = p2;
    mPerm[3] = p3;
  }

  /// @brief コンストラクタ
  FuncXform(ymuint32 fv,
	    ymuint8 p0,
	    ymuint8 p1,
	    ymuint8 p2,
	    ymuint8 p3,
	    ymuint8 p4) :
    mVector(fv)
  {
    mPerm[0] = p0;
    mPerm[1] = p1;
    mPerm[2] = p2;
    mPerm[3] = p3;
    mPerm[4] = p4;
  }

  // 変換先の関数ベクタ
  ymuint32 mVector;

  // 変換方法
  // 最初の ni 個が順列を表す．
  // 最後のビット列が入出力の反転ビットを表す．
  ymuint8 mPerm[5];

};

END_NAMESPACE_YM

#endif // FUNCXORM_H
