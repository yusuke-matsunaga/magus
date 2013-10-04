#ifndef SMTINDEXID_H
#define SMTINDEXID_H

/// @file SmtIndexId.h
/// @brief SmtIndexId のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtIndexId SmtIndexId.h "SmtIndexId.h"
/// @brief インデックス付きの識別子を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtIndexId :
  public SmtId
{
  friend class SmtIdMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  SmtIndexId(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~SmtIndexId();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックスリストの要素数を返す．
  /// @note インデックスリストを持たない場合は 0 を返す．
  virtual
  ymuint
  index_size() const;

  /// @brief インデックスを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < index_size() )
  /// @note インデックスを持たない場合や pos が index_size() より大きい場合はエラー(アボート)となる．
  virtual
  ymint32
  index(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスの数
  ymuint32 mIndexNum;

  // インデックスの配列
  // 実際には必要なサイズを確保する．
  ymint32 mIndexList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTINDEXID_H
