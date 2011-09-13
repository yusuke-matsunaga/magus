#ifndef LCLOGICMGR_H
#define LCLOGICMGR_H

/// @file LcLogicMgr.h
/// @brief LcLogicMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/TvFuncM.h"
#include "ym_utils/BioIO.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcLogicMgr LcLogicMgr.h "LcLogicMgr.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class LcLogicMgr
{
public:

  /// @brief コンストラクタ
  LcLogicMgr();

  /// @brief デストラクタ
  ~LcLogicMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 定数関数，リテラル関数を登録しておく
  ///
  /// 常に定数0，定数1，肯定リテラル，否定リテラルのグループ番号が
  /// 0, 1, 2, 3 になるようにする．
  void
  init();

  /// @brief セルを追加する．
  void
  add_cell(const Cell* cell);

  /// @brief f に対応する LcGroup を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  LcGroup*
  find_logic_group(const TvFunc& f);

  /// @brief f_list に対応する LcGroup を求める．
  /// @param[in] f_list 関数のリスト
  /// @note なければ新規に作る．
  LcGroup*
  find_logic_group(const vector<TvFunc>& f_list);

  /// @brief 内容をバイナリダンプする．
  /// @param[in] bos 出力先のストリーム
  void
  dump(BinO& bos) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理グループの数を返す．
  ymuint
  logic_group_num() const;

  /// @brief 論理グループを返す．
  /// @param[in] id 論理グループ番号 ( 0 <= id < logic_group_num() )
  const LcGroup*
  logic_group(ymuint id) const;

  /// @brief 論理クラスの数を返す．
  ymuint
  logic_class_num() const;

  /// @brief 論理クラスを返す．
  /// @param[in] id 論理クラス番号 ( 0 <= id < logic_class_num() )
  const LcClass*
  logic_class(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループのリスト
  // この配列上の位置とグループ番号は一致している．
  vector<LcGroup*> mLogicGroupList;

  // 多出力論理関数をキーとしてグループ番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mLogicGroupMap2;

  // クラスのリスト
  // この配列上の位置とクラス番号は一致している．
  vector<LcClass*> mLogicClassList;

  // 代表関数をキーとしてクラス番号を保持するハッシュ表
  hash_map<TvFuncM, ymuint> mLogicClassMap2;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCLOGICMGR_H
