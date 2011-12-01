#ifndef ITVLMGRIMPL_H
#define ITVLMGRIMPL_H

/// @file ItvlMgrImpl.h
/// @brief ItvlMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/Alloc.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM

class ItvlCell;

//////////////////////////////////////////////////////////////////////
/// @class ItvlMgrImpl ItvlMgrImpl.h "ItvlMgrImpl.h"
/// @brief ItvlMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class ItvlMgrImpl
{
public:

  /// @brief コンストラクタ
  ItvlMgrImpl();

  /// @brief デストラクタ
  ~ItvlMgrImpl();


public:

  /// @brief クリアする．
  void
  clear();

  /// @brief 全区間を表すセルを追加する．
  void
  add_allcell(ItvlCell*& root_ptr);

  /// @brief 使用可能な数字を得る．
  /// @note 内容は変化しない．
  /// @note 使用可能な区間がない場合(!!!)，-1を返す．
  int
  avail_num();

  /// @brief [d1, d2]という区間が使用可能などうか調べる．
  bool
  check(int d1,
	int d2);

  /// @brief 使用されている区間の最小値を求める．
  /// @note 全区間が未使用の場合は -1 を返す．
  int
  min_id();

  /// @brief 使用されている区間の最大値を求める．
  /// @note 全区間が未使用の場合は -1 を返す．
  int
  max_id();

  /// @brief d を使用可能な区間から削除する．
  void
  erase(int d);

  /// @brief [d1, d2] を使用可能な区間から削除する．
  void
  erase(int d1,
	int d2);

  /// @brief d を使用可能区間に追加する．
  void
  add(int d);

  /// @brief [d1, d2] を使用可能区間に追加する．
  void
  add(int d1,
      int d2);

  /// @brief 内部構造が正しいかチェックする．
  /// @note おかしい時は例外を投げる．
  void
  sanity_check() const;

  /// @brief 内容を表示する
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;

  /// @brief 木構造を表示する
  /// @param[in] s 出力ストリーム
  void
  print_tree(ostream& s) const;

  /// @brief バイナリファイルに書き出す．
  void
  dump(BinO& s) const;

  /// @brief バイナリファイルを読み込む．
  void
  restore(BinI& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief d を含む区間を求める．
  ItvlCell*
  find(int d,
       ItvlCell* ptr);

  /// @brief d よりも小さくもっとも右側にある区間を求める．
  ItvlCell*
  find_left(int d,
	    ItvlCell* ptr);

  /// @brief d よりも大きくもっとも左側にある区間を求める．
  ItvlCell*
  find_right(int d,
	     ItvlCell* ptr);

  /// @brief セルを追加するためのサブルーティン．
  bool
  add_cell(ItvlCell* cell,
	   ItvlCell*& ptr);

  /// @brief 左の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_left(ItvlCell*& ptr);

  /// @brief 右の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_right(ItvlCell*& ptr);

  /// @brief もっとも右にある節点の内容を cell にコピーして削除する．
  /// @return 木の高さが変化した時には true を返す．
  bool
  remove_right(ItvlCell* cell,
	       ItvlCell*& ptr);

  /// @brief delete のためのサブルーティン
  /// @note ptr を根とする部分木から cell を削除する．
  /// @return この部分木の高さが変わった時には true を返す．
  bool
  remove_cell(ItvlCell* cell,
	      ItvlCell*& ptr);

  /// @brief 新しいセルを確保する．
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  ItvlCell*
  new_cell(int start,
	   int end);

  /// @brief セルを削除する．
  void
  delete_cell(ItvlCell* cell);

  /// @brief sanity_check() の下請け関数
  int
  check_cell(ItvlCell* cell,
	     int& l,
	     int& r) const;

  /// @brief print() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  void
  print_cell(ostream& s,
	     ItvlCell* cell) const;

  /// @brief print_tree() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  /// @param[in] level レベル
  void
  print_tree_cell(ostream& s,
		  ItvlCell* cell,
		  int level) const;

  /// @brief dump() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  void
  dump_cell(BinO& s,
	    ItvlCell* cell) const;

  /// @brief restore() の下請け関数
  /// @param[in] s 入力元のストリーム
  /// @return 作成したセルを返す．
  ItvlCell*
  restore_cell(BinI& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  UnitAlloc mCellAlloc;

  // 根のポインタ
  ItvlCell* mRoot;

};

END_NAMESPACE_YM

#endif // ITVLMGRIMPL_H
