#ifndef LIBYM_MVN_MVNCONSTPARTSELECT_H
#define LIBYM_MVN_MVNCONSTPARTSELECT_H

/// @file libym_mvn/MvnConstPartSelect.h
/// @brief MvnConstPartSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConstPartSelect MvnConstPartSelect.h "MvnConstPartSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvnConstPartSelect :
  public MvnUnaryOp
{
  friend class MvnMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 範囲指定の MSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  msb() const;

  /// @brief 範囲指定の LSB を得る．
  /// @note type() が kConstPartSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  lsb() const;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] msb 範囲指定の MSB
  /// @param[in] lsb 範囲指定の LSB
  /// @param[in] bit_width 入力のビット幅
  MvnConstPartSelect(MvnModule* module,
		     ymuint msb,
		     ymuint lsb,
		     ymuint bit_width);

  /// @brief デストラクタ
  ~MvnConstPartSelect();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 範囲指定の MSB
  ymuint32 mMsb;

  // 範囲指定の LSB
  ymuint32 mLsb;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNCONSTPARTSELECT_H
