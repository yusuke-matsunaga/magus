#ifndef NLSOLUTION_H
#define NLSOLUTION_H

/// @file NlSolution.h
/// @brief NlSolution のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlSolution NlSolution.h "NlSolution.h"
/// @brief number link の解を表すクラス
//////////////////////////////////////////////////////////////////////
class NlSolution
{
public:

  /// @brief コンストラクタ
  NlSolution();

  /// @brief デストラクタ
  ~NlSolution();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを設定する．
  /// @param[in] width 幅
  /// @param[in] height 高さ
  void
  set_size(ymuint width,
	   ymuint height);

  /// @brief 初期化する．
  /// @param[in] problem 問題
  void
  init(const NlProblem& problem);

  /// @brief 幅を得る．
  ymuint
  width() const;

  /// @brief 高さを得る．
  ymuint
  height() const;

  /// @brief 内容を設定する．
  /// @param[in] x, y 座標
  /// @param[in] val 値
  void
  set(ymuint x,
      ymuint y,
      int val);

  /// @brief 内容を設定する．
  /// @param[in] point 座標
  /// @param[in] val 値
  void
  set(const NlPoint& point,
      int val);

  /// @brief 内容を読みだす．
  /// @param[in] x, y 座標
  int
  get(ymuint x,
      ymuint y) const;

  /// @brief 内容を読みだす．
  /// @param[in] point 座標
  int
  get(const NlPoint& point) const;

  /// @brief 解が妥当か検証する．
  ///
  /// 検証項目は
  /// - 各線分が連結しているか
  /// - 冗長な線分がないか
  /// - 閉路がないか
  bool
  verify() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  write(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief xy座標からインデックスを得る．
  ymuint
  xy_to_index(ymuint x,
	      ymuint y) const;

  /// @brief インデックスから xy 座標を得る．
  void
  index_to_xy(ymuint index,
	      ymuint& x,
	      ymuint& y) const;

  /// @brief NlPoint からインデックスを得る．
  ymuint
  point_to_index(const NlPoint& point) const;

  /// @brief インデックスから NlPoint を得る．
  NlPoint
  index_to_point(ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 盤の幅
  ymuint mWidth;

  // 盤の高さ
  ymuint mHeight;

  // 線分数
  ymuint mNum;

  // セルの配列
  // 線分番号 ( 1 - mNum ) が入る．
  // 未使用の場合には 0 が入る．
  // 端点の場合には - 線分番号が入る．
  vector<int> mCellArray;

};

/// @relates NlProblem
/// @brief 問題を読み込む．
/// @param[in] filename ファイル名
/// @return 問題を返す．
NlSolution
read_solution(const string& filename);

/// @relates NlSolution
/// @brief 解を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] solution 解
void
print_solution(ostream& s,
	       const NlSolution& solution);

END_NAMESPACE_YM_NLINK

#endif // NLSOLUTION_H
