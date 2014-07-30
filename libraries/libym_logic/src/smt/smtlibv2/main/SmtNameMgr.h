#ifndef SMTNAMEMGR_H
#define SMTNAMEMGR_H

/// @file SmtNameMgr.h
/// @brief SmtNameMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_smtlibv2/SmtVarFun.h"
#include "ym_YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtVarFunImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtNameMgr SmtNameMgr.h "SmtNameMgr.h"
/// @brief SmtVarFun を管理するクラス
///
/// 関数/変数は名前によって区別され，同名の関数/変数は存在しない．
//////////////////////////////////////////////////////////////////////
class SmtNameMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  SmtNameMgr(Alloc& alloc,
	     ymuint level,
	     SmtNameMgr* parent_mgr);

  /// @brief デストラクタ
  ~SmtNameMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] sort 型
  /// @return 登録した変数を返す．
  /// @note エラーが起きたら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の定義が既に存在している．
  const SmtVarFun*
  reg_var(const SmtId* name_id,
	  const SmtSort* sort = NULL);

  /// @brief 宣言のみの関数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] input_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] fun_attr 属性
  /// @return 登録した関数を返す．
  /// @note エラーが起きたら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の関数がすでに存在している．
  ///  - input_list/output_sort 中のパラメータに中抜けがあった．
  const SmtVarFun*
  reg_fun(const SmtId* name_id,
	  const vector<const SmtSort*>& input_list,
	  const SmtSort* output_sort,
	  SmtVarFun::tFunAttr fun_attr = SmtVarFun::kNone);

  /// @brief 実体のある関数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] input_list 入力の型と変数のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体
  /// @return 登録した関数を返す．
  /// @note エラーが起きたら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の関数がすでに存在している．
  const SmtVarFun*
  reg_fun(const SmtId* name_id,
	  const vector<const SmtVarFun*>& input_list,
	  const SmtSort* output_sort,
	  const SmtTerm* body);

  /// @brief 名前からオブジェクトを探す．
  /// @param[in] name_id 名前を表す識別子
  /// @return name_id という名のオブジェクトを返す．
  /// @note 存在しなければ NULL を返す．
  const SmtVarFun*
  find_obj(const SmtId* name_id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief reg_fun() の下請け関数
  /// @param[in] obj 登録するオブジェクト
  void
  reg_sub(SmtVarFunImpl* obj);

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

  // スタックのレベル
  ymuint32 mLevel;

  // 親のマネージャ
  SmtNameMgr* mParent;

  // 登録されている関数の数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  SmtVarFunImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTNAMEMGR_H
