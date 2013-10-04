#ifndef SMTIDIMPL_H
#define SMTIDIMPL_H

/// @file SmtIdImpl.h
/// @brief SmtIdImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtIdImpl SmtIdImpl.h "SmtIdImpl.h"
/// @brief SmtId の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtIdImpl :
  public SmtId
{
  friend class SmtIdMgr;

protected:

  /// @brief 単純な形のコンストラクタ
  /// @param[in] name 名前
  explicit
  SmtIdImpl(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~SmtIdImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 名前を返す．
  ShString
  name() const;

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

  // ID番号
  ymuint32 mId;

  // 名前
  ShString mName;

  // ハッシュ表のためのリンクポインタ
  SmtIdImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtIndexId SmtIdImpl.h "SmtIdImpl.h"
/// @brief インデックス付きの識別子を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtIndexId :
  public SmtIdImpl
{
  friend class SmtIdMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] index_num インデックスの数
  SmtIndexId(const ShString& name,
	     ymuint index_num);

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

#endif // YM_SMTLIBV2_SMTID_H
