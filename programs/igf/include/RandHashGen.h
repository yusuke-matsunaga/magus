#ifndef RANDHASHGEN_H
#define RANDHASHGEN_H

/// @file RandHashGen.h
/// @brief RandHashGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class RandHashGen RandHashGen.h "RandHashGen.h"
/// @brief ランダムなハッシュ関数を作るクラス
//////////////////////////////////////////////////////////////////////
class RandHashGen
{
public:

  /// @brief コンストラクタ
  RandHashGen();

  /// @brief デストラクタ
  ~RandHashGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ランダムにハッシュ関数を作る．
  /// @param[in] input_num 入力数
  /// @param[in] output_num 出力数
  /// @param[in] max_degree
  InputFunc*
  gen_func(ymuint input_num,
	   ymuint output_num,
	   ymuint max_degree);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数生成機
  RandGen mRandGen;

};

END_NAMESPACE_YM_IGF

#endif // RANDHASHGEN_H
