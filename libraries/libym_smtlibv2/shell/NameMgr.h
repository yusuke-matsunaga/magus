#ifndef NAMEMGR_H
#define NAMEMGR_H

/// @file NameMgr.h
/// @brief NameMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_logic/SmtFun.h"
#include "ym_logic/SmtVar.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;
class NameObj;
class NameObjImpl;

//////////////////////////////////////////////////////////////////////
/// @class NameMgr NameMgr.h "NameMgr.h"
/// @brief SmtVar/SmtFun を管理するクラス
///
/// 関数/変数は名前によって区別され，同名の関数/変数は存在しない．
//////////////////////////////////////////////////////////////////////
class NameMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  NameMgr(Alloc& alloc,
	  ymuint level,
	  NameMgr* parent_mgr);

  /// @brief デストラクタ
  ~NameMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] fun_type 関数の型
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の関数がすでに存在している．
  bool
  reg_builtin_fun(const SmtId* name_id,
		  tSmtFun fun_type);

  /// @brief 関数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] fun 関数
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の関数がすでに存在している．
  bool
  reg_fun(const SmtId* name_id,
	  const SmtFun* fun);

  /// @brief 変数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] var 変数
  /// @retval true 処理が成功した．
  /// @retval false 処理が失敗した．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の定義が既に存在している．
  bool
  reg_var(const SmtId* name_id,
	  const SmtVar* var);

  /// @brief 名前からオブジェクトを探す．
  /// @param[in] name_id 名前を表す識別子
  /// @return name_id という名のオブジェクトを返す．
  /// @note 存在しなければ NULL を返す．
  const NameObj*
  find_obj(const SmtId* name_id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief reg_fun() の下請け関数
  /// @param[in] obj 登録するオブジェクト
  void
  reg_sub(NameObjImpl* obj);

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
  NameMgr* mParent;

  // 登録されている関数の数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  NameObjImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // NAMEMGR_H
