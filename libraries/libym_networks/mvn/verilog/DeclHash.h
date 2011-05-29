#ifndef LIBYM_MVN_VERILOG_DECLHASH_H
#define LIBYM_MVN_VERILOG_DECLHASH_H

/// @file libym_networks/verilog/DeclHash.h
/// @brief DeclHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class DeclHash DeclHash.h "DeclHash.h"
/// @brief VlDecl とID番号の対応付けを行うハッシュ表
//////////////////////////////////////////////////////////////////////
class DeclHash
{
public:

  /// @brief コンストラクタ
  DeclHash();

  /// @brief デストラクタ
  ~DeclHash();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief ID番号を得る．
  /// @param[in] decl 宣言要素
  /// @return ID番号
  /// @note 登録されていなかった場合には新しい番号を割り当てる．
  ymuint
  get_id(const VlDecl* decl);

  /// @brief ID番号を得る．
  /// @param[in] decl 配列型宣言要素
  /// @param[in] offset オフセット
  /// @return ID番号
  /// @note 登録されていなかった場合には新しい番号を割り当てる．
  ymuint
  get_id(const VlDeclArray* decl,
	 ymuint offset);

  /// @brief ID番号の最大値 + 1を返す．
  ymuint
  max_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 宣言要素
    const VlObj* mDecl;

    // ID番号
    ymuint32 mId;

    // 次の要素を指すリンク
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を登録する．
  void
  put_cell(const VlObj* decl,
	   ymuint id);

  /// @brief Cell を探す．
  Cell*
  find_cell(const VlObj* decl) const;

  /// @brief テーブルの領域を確保する．
  /// @param[in] size 必要なサイズ
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ値を計算する．
  ymuint
  hash_func(const VlObj* decl) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の確保用アロケータ
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

  // 次に割り当て可能な ID 番号
  ymuint32 mNextId;

};

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_DECLHASH_H
