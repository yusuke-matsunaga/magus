#ifndef LIBYM_MVN_MVNCONSTBITSELECT_H
#define LIBYM_MVN_MVNCONSTBITSELECT_H

/// @file libym_mvn/MvnConstBitSelect.h
/// @brief MvnConstBitSelect のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUnaryOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConstBitSelect MvnConstBitSelect.h "MvnConstBitSelect.h"
/// @brief 定数ビット指定を表すノード
//////////////////////////////////////////////////////////////////////
class MvnConstBitSelect :
  public MvnUnaryOp
{
  friend class MvnMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief ビット位置を得る．
  /// @note type() が kConstBitSelect の時のみ意味を持つ．
  /// @note デフォルトの実装では 0 を返す．
  virtual
  ymuint
  bitpos() const;


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bitpos ビット位置
  /// @param[in] bit_width 入力のビット幅
  MvnConstBitSelect(MvnModule* module,
		    ymuint bitpos,
		    ymuint bit_width);

  /// @brief デストラクタ
  ~MvnConstBitSelect();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ビット位置
  ymuint32 mBitPos;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNCONSTBITSELECT_H
