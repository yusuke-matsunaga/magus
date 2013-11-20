#ifndef SMTIDMGR_H
#define SMTIDMGR_H

/// @file SmtIdMgr.h
/// @brief SmtIdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibNode;
class SmtIdImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtIdMgr SmtIdMgr.h "SmtIdMgr.h"
/// @brief SmtId を管理するクラス
///
/// 識別子は <symbol> <numeral>* の形をとる．
/// 同じ表現が同じインスタンスになるようにハッシュ表を用いて管理する．
/// SmtId::id() は識別子のなかでユニークになるようにしているので
/// SmtId::id() を用いて同一かどうかのチェックが行える．
//////////////////////////////////////////////////////////////////////
class SmtIdMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  SmtIdMgr(Alloc& alloc);

  /// @brief デストラクタ
  ~SmtIdMgr();


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
  SmtIdImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTIDMGR_H
