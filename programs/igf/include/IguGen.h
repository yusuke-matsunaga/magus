#ifndef IGUGEN_H
#define IGUGEN_H

/// @file IguGen.h
/// @brief IguGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class IguGen IguGen.h "IguGen.h"
/// @brief IGU を作るクラス
//////////////////////////////////////////////////////////////////////
class IguGen
{
public:

  /// @brief コンストラクタ
  IguGen();

  /// @brief デストラクタ
  ~IguGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  /// @param[in] func_list 関数のリスト
  /// @param[out] g_list マッピングのリスト
  bool
  mapping(const vector<const FuncVect*>& func_list,
	  vector<vector<ymuint32> >& g_list);

  /// @brief collision free 分割を行う．
  /// @param[in] func_list 関数のリスト
  /// @param[out] block_map 分割を表す配列
  bool
  cf_partition(const vector<const FuncVect*>& func_list,
	       vector<ymuint>& block_map);

  /// @brief displace_decomposition を行う．
  bool
  displace_decomposition(const FuncVect* func1,
			 const FuncVect* func2,
			 vector<ymuint>& displace_map,
			 bool use_xor);

};

END_NAMESPACE_YM_IGF

#endif // IGUGEN_H
