#ifndef SMTFUNMGR_H
#define SMTFUNMGR_H

/// @file SmtFunMgr.h
/// @brief SmtFunMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_smtlibv2/SmtFun.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtFunImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtFunMgr SMtFunMgr.h "SmtFunMgr.h"
/// @brief SmtFun を管理するクラス
//////////////////////////////////////////////////////////////////////
class SmtFunMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  SmtFunMgr(Alloc& alloc);

  /// @brief デストラクタ
  ~SmtFunMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言のみの関数を登録する．
  /// @param[in] name 名前
  /// @param[in] sort 出力の型
  /// @param[in] input_list 入力の型のリスト
  /// @param[in] attr 属性
  /// @param[in] param_num パラメータの数
  /// @return 登録した関数を返す．
  const SmtFun*
  reg_fun(const SmtId* name,
	  const vector<const SmtSort*>& input_list,
	  const SmtSort* sort,
	  SmtFun::tAttr attr = SmtFun::kNone,
	  ymuint param_num = 0);

  /// @brief 実体のある関数を登録する．
  /// @param[in] name 名前
  /// @param[in] sort 出力の型
  /// @param[in] input_list 入力の型と変数のリスト
  /// @param[in] body 本体
  /// @return 登録した関数を返す．
  const SmtFun*
  reg_fun(const SmtId* name,
	  const vector<SmtSortedVar>& input_list,
	  const SmtSort* sort,
	  const SmtTerm* body);

  /// @brief 関数を返す．
  /// @param[in] name 名前
  const SmtFun*
  find_fun(const SmtId* name) const;


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

  // 登録されている関数の数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  SmtFunImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTFUNMGR_H
