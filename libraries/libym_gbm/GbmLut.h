#ifndef GBMLUT_H
#define GBMLUT_H

/// @file GbmLut.h
/// @brief GbmLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
/// @class GbmLut GbmLut.h "GbmLut.h"
/// @brief LUTノードを表す GbmNode の派生クラス
//////////////////////////////////////////////////////////////////////
class GbmLut :
  public GbmNodeImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid 変数番号
  /// @param[in] inputs 入力のハンドルのリスト
  /// @param[in] tv_list 真理値ベクタの変数番号のリスト
  GbmLut(ymuint id,
	 VarId vid,
	 const vector<GbmNodeHandle>& inputs,
	 const vector<VarId>& tv_list);

  /// @brief デストラクタ
  virtual
  ~GbmLut();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief LUTノードの時 true を返す．
  virtual
  bool
  is_lut() const;

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

  // ファンインのハンドル
  vector<GbmNodeHandle> mFanins;

  // 真理値ベクタの変数番号
  vector<VarId> mTvList;

};

END_NAMESPACE_YM_GBM

#endif // GBMLUT_H
