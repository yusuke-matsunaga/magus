#ifndef IDIMPL_H
#define IDIMPL_H

/// @file IdImpl.h
/// @brief IdImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtId.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class IdImpl IdImpl.h "IdImpl.h"
/// @brief SmtId の実装クラス
//////////////////////////////////////////////////////////////////////
class IdImpl :
  public SmtId
{
  friend class IdMgr;

protected:

  /// @brief 単純な形のコンストラクタ
  /// @param[in] name 名前
  explicit
  IdImpl(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~IdImpl();


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
  ymuint32
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
  IdImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class IndexId IdImpl.h "IdImpl.h"
/// @brief インデックス付きの識別子を表すクラス
//////////////////////////////////////////////////////////////////////
class IndexId :
  public IdImpl
{
  friend class IdMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] index_num インデックスの数
  IndexId(const ShString& name,
	     ymuint index_num);

  /// @brief デストラクタ
  virtual
  ~IndexId();


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
  ymuint32
  index(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスの数
  ymuint32 mIndexNum;

  // インデックスの配列
  // 実際には必要なサイズを確保する．
  ymuint32 mIndexList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTID_H
