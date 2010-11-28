#ifndef LIBYM_MVN_VERILOG_SSAMGR_H
#define LIBYM_MVN_VERILOG_SSAMGR_H

/// @file libym_mvn/verilog/SsaMgr.h
/// @brief SsaMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_MVN

class DfgBlock;

//////////////////////////////////////////////////////////////////////
/// @class SsaMgr SsaMgr.h "SsaMgr.h"
/// @brief SSA のためのインスタンス番号生成を行うクラス
//////////////////////////////////////////////////////////////////////
class SsaMgr
{
public:

  /// @brief コンストラクタ
  SsaMgr();

  /// @brief デストラクタ
  ~SsaMgr();


public:

  /// @brief 内容を空にする
  void
  clear();

  /// @brief 新しい ID を割り当てる．
  ymuint
  new_id(const DfgBlock* block,
	 const VlDecl* decl);

  /// @brief ID 番号を取り出す．
  ymuint
  last_id(const DfgBlock* block,
	  const VlDecl* decl);

  /// @brief ID番号をキーにして対応する要素を返す．
  /// @param[in] id インスタンス番号
  const VlDecl*
  decl(ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    /// @brief コンストラクタ
    /// @param[in] block ブロック
    /// @param[in] decl 宣言要素
    Cell(const DfgBlock* block,
	 const VlDecl* decl) :
      mBlock(block),
      mDecl(decl)
    {
    }

    // ブロック
    const DfgBlock* mBlock;

    // 宣言要素
    const VlDecl* mDecl;

    // インスタンス番号
    ymuint32 mId;

    // ハッシュ表の中の次の要素を指すリンク
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルの領域を確保する．
  /// @param[in] size 必要なサイズ
  void
  alloc_table(ymuint size);

  /// @brief セルを探す．
  /// @param[in] block 親のブロック
  /// @param[in] decl 宣言要素
  Cell*
  find_cell(const DfgBlock* block,
	    const VlDecl* decl);

  /// @brief ハッシュ値を計算する．
  /// @param[in] block 親のブロック
  /// @param[in] decl 宣言要素
  static
  ymuint
  hash_func(const DfgBlock* block,
	    const VlDecl* decl);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の確保用アロケータ
  UnitAlloc mAlloc;

  // インスタンス番号をキーにして (block, decl) を格納する配列
  vector<pair<const DfgBlock*, const VlDecl*> > mDeclArray;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_VERILOG_SSAMGR_H
