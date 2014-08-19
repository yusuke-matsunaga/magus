#ifndef SORTMGR_H
#define SORTMGR_H

/// @file SortMgr.h
/// @brief SortMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"
#include "YmLogic/smt_nsdef.h"
#include "YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SortElem;

//////////////////////////////////////////////////////////////////////
/// @class SortMgr SortMgr.h "SortMgr.h"
/// @brief SmtSortInfo を管理するクラス
///
/// 型名と要素リストをキーにして型インスタンスを保持する
//////////////////////////////////////////////////////////////////////
class SortMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] solver ソルバ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  SortMgr(Alloc& alloc,
	  SmtSolver& solver,
	  ymuint level,
	  SortMgr* parent_mgr);

  /// @brief デストラクタ
  ~SortMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型名を宣言する．
  /// @param[in] name_id 型名
  /// @param[in] param_num 引数の数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．すでに同名の型が登録されている
  bool
  declare_sort(const SmtId* name_id,
	       ymuint param_num);

  /// @breif パラメータ型のテンプレートを生成する．
  /// @param[in] param_id パラメータ番号
  const SortElem*
  make_param_sort_templ(ymuint param_id);

  /// @brief 型テンプレートを生成する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] elem_list 要素のリスト
  const SortElem*
  make_sort_templ(const SmtId* name_id,
		  const vector<const SortElem*>& elem_list = vector<const SortElem*>(0));

  /// @brief 型名を定義する．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] sort 登録する型テンプレート
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．すでに同名の型が登録されている
  bool
  define_sort(const SmtId* name_id,
	      const SortElem* sort);

  /// @brief 型を作る．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] param_list 要素の型のリスト
  /// @return 生成した型を返す．
  /// @note エラーが起こったら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の型が定義されていなかった．
  ///  - name_id という名の型のパラメータ数が param_list のサイズと異なった．
  tSmtSortId
  make_sort(const SmtId* name_id,
	    const vector<tSmtSortId>& param_list = vector<tSmtSortId>(0));


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型テンプレートを登録する．
  /// @param[in] name_id 型名
  /// @param[in] sort 登録する型テンプレート
  /// @param[in] param_num パラメータ数
  void
  reg_templ(const SmtId* name_id,
	    const SortElem* sort,
	    ymuint param_num);

  /// @brief 型テンプレートを探す．
  /// @param[in] name_id 型名
  /// @param[out] param_num パラメータ数
  /// @return name_id という名の型テンプレートを返す．
  /// @note なければ NULL を返す．
  const SortElem*
  find_templ(const SmtId* name_id,
	     ymuint& param_num);

  /// @brief 型を登録する．
  /// @param[in] name_id 型名
  /// @param[in] sort 登録する型
  void
  reg_sort(const SmtId* name_id,
	   const SmtSortInfo* sort);

  /// @brief 型を探す．
  /// @param[in] name_id 型名
  /// @param[in] elem_list 部品の型のリスト
  /// @return 登録されていなければ NULL を返す．
  const SmtSortInfo*
  find_sort(const SmtId* name_id,
	    const vector<tSmtSortId>& elem_list);

  /// @brief テンプレートから実際の型を作る．
  /// @param[in] sort_templ テンプレート
  /// @param[in] param_list パラメータリスト
  tSmtSortId
  replace_param(const SortElem* sort_templ,
		const vector<tSmtSortId>& param_list);

  /// @brief 型を作る．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] param_list 要素の型のリスト
  /// @return 生成した型を返す．
  tSmtSortId
  _make_sort(const SmtId* name_id,
	     const vector<tSmtSortId>& param_list = vector<tSmtSortId>(0));

  /// @brief 型テンプレート用のハッシュ表を拡大する．
  /// @param[in] req_size 新しいサイズ
  void
  expand_table1(ymuint req_size);

  /// @brief 型用のハッシュ表を拡大する．
  /// @param[in] req_size 新しいサイズ
  void
  expand_table2(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 型テンプレート用のハッシュ表の要素
  struct Cell1
  {
    // 名前を表す識別子
    const SmtId* mId;

    // 型
    const SortElem* mSort;

    // パラメータ数
    ymuint32 mParamNum;

    // 次の要素を指すリンクポインタ
    Cell1* mLink;

  };

  // 型用のハッシュ用のセル
  struct Cell2
  {
    // 名前を表す識別子
    const SmtId* mId;

    // 型
    const SmtSortInfo* mSort;

    // 次の要素を指すリンクポインタ
    Cell2* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  Alloc& mAlloc;

  // SMT ソルバ
  SmtSolver& mSolver;

  // スタックのレベル
  ymuint32 mLevel;

  // 親のマネージャ
  SortMgr* mParent;

  // 登録されている型テンプレートの数
  ymuint32 mNum1;

  // ハッシュ表のサイズ
  ymuint32 mTableSize1;

  // ハッシュ表
  Cell1** mHashTable1;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit1;

  // 登録されている宣言の数
  ymuint32 mNum2;

  // ハッシュ表のサイズ
  ymuint32 mTableSize2;

  // ハッシュ表
  Cell2** mHashTable2;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit2;

  // パラメータ型を保持しておく配列
  vector<const SortElem*> mParamArray;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SORTMGR_H
