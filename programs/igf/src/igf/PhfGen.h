#ifndef PHFGEN_H
#define PHFGEN_H

/// @file PhfGen.h
/// @brief PhfGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class InputFunc;

//////////////////////////////////////////////////////////////////////
/// @class PhfGen PhfGen.h "PhfGen.h"
/// @brief PHF を作るクラス
//////////////////////////////////////////////////////////////////////
class PhfGen
{
public:

  /// @brief コンストラクタ
  PhfGen();

  /// @brief デストラクタ
  ~PhfGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  bool
  mapping(const vector<RegVect*>& vector_list,
	  const InputFunc& f1,
	  const InputFunc& f2,
	  vector<ymuint32>& g1,
	  vector<ymuint32>& g2);

};

END_NAMESPACE_YM_IGF

#endif // PHFGEN_H
