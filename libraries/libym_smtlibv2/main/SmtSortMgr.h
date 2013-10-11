#ifndef SMTSORTMGR_H
#define SMTSORTMGR_H

/// @file SmtSortMgr.h
/// @brief SmtSortMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;
class SmtSortImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtSortMgr SmtSortMgr.h "SmtSortMgr.h"
/// @brief SmtSort を管理するクラス
///
/// 内部には2つのハッシュ表がある．
/// 一つは型名をキーにして，型テンプレートとパラメータ数を保持するもの．
/// もう一つは型名と要素リストをキーにして型インスタンスを保持するもの．
//////////////////////////////////////////////////////////////////////
class SmtSortMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  SmtSortMgr(Alloc& alloc,
	     ymuint level,
	     SmtSortMgr* parent_mgr);

  /// @brief デストラクタ
  ~SmtSortMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型名を登録する．
  /// @param[in] name_id 型名
  /// @param[in] param_num 引数の数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名で異なる宣言がすでに登録されている．
  bool
  reg_sort(const SmtId* name_id,
	   ymuint param_num);

  /// @brief alias を登録する．
  /// @param[in] name_id 型名
  /// @param[in] param_num 引数の数
  /// @param[in] sort 登録する型
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名で異なる alias が登録されている．
  bool
  reg_alias(const SmtId* name_id,
	    ymuint param_num,
	    const SmtSort* sort);

  /// @brief SmtSort に変換する．
  /// @param[in] name 型名
  /// @param[in] elem_list 部品の型のリスト
  /// @return 引数に合致する型を返す．
  const SmtSort*
  make_sort(const SmtId* name_id,
	    const vector<const SmtSort*>& elem_list = vector<const SmtSort*>(0));

  /// @brief 型パラメータを作る．
  /// @param[in] pid パラメータ番号
  const SmtSort*
  make_param_sort(ymuint pid);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型宣言を探す．
  /// @param[in] name_id 型名
  /// @param[out] sort_tmpl 型テンプレート
  /// @param[out] param_num パラメータ数
  /// @retval true name_id とういう名の型テンプレートが見つかった．
  /// @retval false name_id という名の型テンプレートは登録されていなかった．
  bool
  find_sort_decl(const SmtId* name_id,
		 const SmtSort*& sort_tmpl,
		 ymuint& param_num);

  /// @brief 型を探す．
  /// @param[in] name_id 型名
  /// @param[in] elem_list 部品の型のリスト
  /// @return 登録されていなければ NULL を返す．
  const SmtSort*
  find_sort(const SmtId* name_id,
	    const vector<const SmtSort*>& elem_list);

  /// @brief テンプレートから実際の型を作る．
  /// @param[in] templ テンプレート
  /// @param[in] param_list パラメータリスト
  const SmtSort*
  replace_param(const SmtSort* templ,
		const vector<const SmtSort*>& param_list);

  /// @brief 単純な型を作る．
  /// @param[in] name_id 名前
  SmtSortImpl*
  new_simple_sort(const SmtId* name_id);

  /// @brief 複合型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素のリスト
  SmtSortImpl*
  new_complex_sort(const SmtId* name,
		   const vector<const SmtSort*>& elem_list);

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
  SmtSortMgr* mParent;

  // 名前をキーにして型テンプレートと引数の数を保持するハッシュ表
  hash_map<ymuint32, pair<const SmtSort*, ymuint32> > mHash;

  // 登録されている宣言の数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  SmtSortImpl** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTSORTMGR_H
