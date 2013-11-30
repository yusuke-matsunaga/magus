#ifndef GBMAND_H
#define GBMAND_H

/// @file GbmAnd.h
/// @brief GbmAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmAnd GbmAnd.h "GbmAnd.h"
/// @brief ANDノードを表す GbmNode の派生クラス
//////////////////////////////////////////////////////////////////////
class GbmAnd :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid 変数番号
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン1のハンドル
  GbmAnd(ymuint id,
	 VarId vid,
	 GbmNodeHandle input0,
	 GbmNodeHandle input1);

  /// @brief デストラクタ
  virtual
  ~GbmAnd();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ANDノードの時 true を返す．
  virtual
  bool
  is_and() const;

  /// @brief ファンイン数を返す．
  /// @note 外部入力ノードの場合は常に0
  /// @note AND ノードの場合は常に2
  virtual
  ymuint
  fanin_num() const;

  /// @brief ファンインのハンドルを返す．
  /// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
  virtual
  GbmNodeHandle
  fanin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンイン0のハンドル
  GbmNodeHandle mFanin0;

  // ファンイン1のハンドル
  GbmNodeHandle mFanin1;

};

END_NAMESPACE_YM_GBM

#endif // GBMAND_H
