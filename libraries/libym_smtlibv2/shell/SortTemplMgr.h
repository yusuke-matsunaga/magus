#ifndef SORTTEMPLMGR_H
#define SORTTEMPLMGR_H

/// @file SortTemplMgr.h
/// @brief SortTemplMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SortElem;

//////////////////////////////////////////////////////////////////////
/// @class SortTemplMgr SortTemplMgr.h "SortTemplMgr.h"
/// @brief SortTempl を管理するクラス
//////////////////////////////////////////////////////////////////////
class SortTemplMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリアロケータ
  /// @param[in] level スタックレベル
  /// @param[in] parent_mgr 上位のマネージャ
  SortTemplMgr(Alloc& alloc,
	       ymuint level,
	       SortTemplMgr* parent_mgr);

  /// @brief デストラクタ
  ~SortTemplMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型名を登録する．
  /// @param[in] name_id 型名
  /// @param[in] param_num 引数の数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．すでに同名の型が登録されている
  bool
  reg_sort(const SmtId* name_id,
	   ymuint param_num);

  /// @brief alias を登録する．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] sort 登録する型テンプレート
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．すでに同名の型が登録されている
  bool
  reg_alias(const SmtId* name_id,
	    SortElem* sort);

  /// @brief 型テンプレートを探す．
  /// @param[in] name_id 型名を表す識別子
  /// @param[out] param_num パラメータ数
  /// @return 対象の型テンプレートを返す．
  /// @note 見つからなかったら NULL を返す．
  const SortElem*
  find_sort(const SmtId* name_id,
	    ymuint& param_num);

  /// @brief 単純な型を作る．
  /// @param[in] name_id 型名を表す識別子
  /// @return 生成した型を返す．
  /// @note エラーが起こったら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の型が定義されていなかった．
  ///  - name_id という名の型のパラメータ数が0ではなかった．
  const SortElem*
  make_sort(const SmtId* name_id);

  /// @brief 複合型を作る．
  /// @param[in] name_id 型名を表す識別子
  /// @param[in] param_list 要素の型のリスト
  /// @return 生成した型を返す．
  /// @note エラーが起こったら NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の型が定義されていなかった．
  ///  - name_id という名の型のパラメータ数が0ではなかった．
  const SortElem*
  make_sort(const SmtId* name_id,
	    const vector<const SortElem*>& param_list);

  /// @brief パラメータ型を作る．
  const SortElem*
  make_param_sort(ymuint param_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を登録する．
  /// @param[in] sort 登録する型テンプレート
  /// @param[in] param_num パラメータ数
  void
  reg_common(SortElem* sort,
	     ymuint param_num);

  /// @breif パラメータ型を生成する．
  /// @param[in] param_id パラメータ番号
  const SortElem*
  new_param_sort(ymuint param_id);

  /// @brief 単純な型を生成する．
  /// @param[in] name_id 名前を表す識別子
  const SortElem*
  new_simple_sort(const SmtId* name_id);

  /// @brief 複合型を生成する．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] elem_list 要素のリスト
  const SortElem*
  new_complex_sort(const SmtId* name_id,
		   const vector<const SortElem*>& elem_list);

  /// @brief ハッシュ表を拡大する．
  /// @param[in] req_size 新しいサイズ
  void
  expand_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表の要素
  struct Cell
  {
    // 型
    SortElem* mSort;

    // パラメータ数
    ymuint32 mParamNum;

    // 次の要素を指すリンクポインタ
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  Alloc& mAlloc;

  // スタックのレベル
  ymuint32 mLevel;

  // 親のマネージャ
  SortTemplMgr* mParent;

  // 登録されている型テンプレートの数
  ymuint32 mNum;

  // ハッシュ表のサイズ
  ymuint32 mTableSize;

  // ハッシュ表
  Cell** mHashTable;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // パラメータ用の配列
  vector<const SortElem*> mParamArray;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SORTTEMPLMGR_H
