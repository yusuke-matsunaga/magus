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
  /// @param[in] func_list 関数のリスト
  /// @param[out] g_list マッピングのリスト
  bool
  mapping(const vector<const FuncVect*>& func_list,
	  vector<vector<ymuint32>* >& g_list);

  /// @brief 分割を求める．
  /// @param[in] func_list 関数のリスト
  /// @param[out] block_map 個々の入力ごとのブロック番号を入れる配列
  ymuint
  split(const vector<const FuncVect*>& func_list,
	vector<ymuint>& block_map);

};

END_NAMESPACE_YM_IGF

#endif // PHFGEN_H
