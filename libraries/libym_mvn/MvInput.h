#ifndef LIBYM_MVN_MVINPUT_H
#define LIBYM_MVN_MVINPUT_H

/// @file libym_mvn/MvInput.h
/// @brief MvInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvInput MvInput.h "MvInput.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvInput :
  public MvNode
{
  friend class MvMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 入力ノード/出力ノードの場合に位置を返す．
  /// @note type() が kInput と kOutput の時のみ意味を持つ．
  virtual
  ymuint
  pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] pos 位置
  /// @param[in] bit_width ビット幅
  MvInput(MvModule* module,
	  ymuint pos,
	  ymuint bit_width);

  /// @brief デストラクタ
  ~MvInput();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 位置
  ymuint32 mPos;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVINPUT_H
