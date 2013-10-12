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
///
/// 関数は名前によって区別され，同名の関数は存在しない．
//////////////////////////////////////////////////////////////////////
class SmtFunMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  SmtFunMgr(Alloc& alloc,
	    ymuint level,
	    SmtFunMgr* parent_mgr);

  /// @brief デストラクタ
  ~SmtFunMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言のみの関数を登録する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] input_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] attr 属性
  /// @param[in] param_num パラメータの数
  /// @return 登録した関数を返す．
  /// @note エラーが起きたら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id と同名の関数がすでに存在している．
  const SmtFun*
  reg_fun(const SmtId* name_id,
	  const vector<const SmtSort*>& input_list,
	  const SmtSort* output_sort,
	  SmtFun::tAttr attr = SmtFun::kNone,
	  ymuint param_num = 0);

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
  const SmtFun*
  reg_fun(const SmtId* name_id,
	  const vector<SmtSortedVar>& input_list,
	  const SmtSort* output_sort,
	  const SmtTerm* body);

  /// @brief 関数を返す．
  /// @param[in] name_id 名前を表す識別子
  /// @return name_id という名の関数を返す．
  /// @note 存在しなければ NULL を返す．
  const SmtFun*
  find_fun(const SmtId* name_id) const;


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

  // スタックのレベル
  ymuint32 mLevel;

  // 親のマネージャ
  SmtFunMgr* mParent;

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
