#ifndef PHF3GEN_H
#define PHF3GEN_H

/// @file Phf3Gen.h
/// @brief Phf3Gen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class InputFunc;

//////////////////////////////////////////////////////////////////////
/// @class Phf3Gen Phf3Gen.h "Phf3Gen.h"
/// @brief PHF を作るクラス
//////////////////////////////////////////////////////////////////////
class Phf3Gen
{
public:

  /// @brief コンストラクタ
  Phf3Gen();

  /// @brief デストラクタ
  ~Phf3Gen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  bool
  mapping(const vector<RegVect*>& vector_list,
	  const InputFunc& f1,
	  const InputFunc& f2,
	  const InputFunc& f3,
	  vector<ymuint32>& g1,
	  vector<ymuint32>& g2,
	  vector<ymuint32>& g3);

};

END_NAMESPACE_YM_IGF

#endif // PHF3GEN_H
