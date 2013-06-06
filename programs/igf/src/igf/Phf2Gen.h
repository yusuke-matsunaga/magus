#ifndef PHF2GEN_H
#define PHF2GEN_H

/// @file Phf2Gen.h
/// @brief Phf2Gen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class InputFunc;

//////////////////////////////////////////////////////////////////////
/// @class Phf2Gen Phf2Gen.h "Phf2Gen.h"
/// @brief PHF を作るクラス
//////////////////////////////////////////////////////////////////////
class Phf2Gen
{
public:

  /// @brief コンストラクタ
  Phf2Gen();

  /// @brief デストラクタ
  ~Phf2Gen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  bool
  mapping(const vector<const RegVect*>& vector_list,
	  const InputFunc& f1,
	  const InputFunc& f2,
	  vector<ymuint32>& g1,
	  vector<ymuint32>& g2);

};

END_NAMESPACE_YM_IGF

#endif // PHF2GEN_H
