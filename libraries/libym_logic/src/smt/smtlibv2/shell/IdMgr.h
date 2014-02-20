#ifndef IDMGR_H
#define IDMGR_H

/// @file IdMgr.h
/// @brief IdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"
#include "utils/ShString.h"
#include "utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;
class IdImpl;

//////////////////////////////////////////////////////////////////////
/// @class IdMgr IdMgr.h "IdMgr.h"
/// @brief SmtId を管理するクラス
///
/// 識別子は <symbol> <numeral>* の形をとる．
/// 同じ表現が同じインスタンスになるようにハッシュ表を用いて管理する．
/// SmtId::id() は識別子のなかでユニークになるようにしているので
/// SmtId::id() を用いて同一かどうかのチェックが行える．
//////////////////////////////////////////////////////////////////////
class IdMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  IdMgr(Alloc& alloc);

  /// @brief デストラクタ
  ~IdMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 識別子を返す．
  /// @param[in] name 名前
  /// @param[in] index_list インデックスリスト
  const SmtId*
  make_id(const ShString& name,
	  const vector<ymuint32>& index_list = vector<ymuint32>(0));


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を拡大する．
  /// @param[in] req_size 新しいサイズ
  void
  expand_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  Alloc& mAlloc;

  // 登録されている要素数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  IdImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // IDMGR_H
