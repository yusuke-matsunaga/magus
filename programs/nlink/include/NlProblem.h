#ifndef NLPROBLEM_H
#define NLPROBLEM_H

/// @file NlProblem.h
/// @brief NlProblem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "NlConnection.h"
#include "NlPoint.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlProblem NlProblem.h "NlProblem.h"
/// @brief number link の問題を表すクラス
//////////////////////////////////////////////////////////////////////
class NlProblem
{
public:
  //////////////////////////////////////////////////////////////////////
  /// brief 初期化用のクラス
  ///
  /// 内容は NlProblem と同じじゃないかと思われるかも知れないが，
  /// こうすることで NlProblem はコンストラクタ以外では内容を変更
  /// できなくなる．いわゆる builder デザインパタン
  //////////////////////////////////////////////////////////////////////
  class Builder
  {
  public:

    /// @brief コンストラクタ
    Builder();

    /// @brief デストラクタ
    ~Builder();

    /// @brief 以前の内容をクリアする．
    void
    clear();

    /// @brief 盤のサイズをセットする．
    /// @param[in] width 幅
    /// @param[in] height 高さ
    ///
    /// 以前の内容はクリアされる．
    void
    set_size(ymuint width,
	     ymuint height);

    /// @brief 線分を追加する．
    /// @param[in] connection 線分
    void
    add_elem(const NlConnection& connection);

    /// @brief 線分を追加する．
    /// @param[in] start_point 始点
    /// @param[in] end_point 終点
    void
    add_elem(const NlPoint& start_point,
	     const NlPoint& end_point);

    /// @brief 盤の幅を返す．
    ymuint
    width() const;

    /// @brief 盤の高さを返す．
    ymuint
    height() const;

    /// @brief 線分の要素数を返す．
    ymuint
    elem_num() const;

    /// @brief 線分を返す．
    /// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
    NlConnection
    connection(ymuint idx) const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 幅
    ymuint mWidth;

    // 高さ
    ymuint mHeight;

    // 線分
    vector<NlConnection> mElemArray;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] builder ビルダーオブジェクト
  NlProblem(const Builder& builder);

  /// @brief デストラクタ
  ~NlProblem();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 盤の幅を返す．
  ymuint
  width() const;

  /// @brief 盤の高さを返す．
  ymuint
  height() const;

  /// @brief 線分の要素数を返す．
  ymuint
  elem_num() const;

  /// @brief 線分を返す．
  /// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
  NlConnection
  connection(ymuint idx) const;

  /// @brief 線分の始点を返す．
  /// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
  NlPoint
  start_point(ymuint idx) const;

  /// @brief 線分の終点を返す．
  /// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
  NlPoint
  end_point(ymuint idx) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 線分
  vector<NlConnection> mElemArray;

};

/// @relates NlProblem
/// @brief 問題を読み込む．
/// @param[in] s 入力元のストリーム
/// @return 問題を返す．
NlProblem
read_problem(ostream& s);

/// @relates NlProblem
/// @brief 問題の内容を出力する
/// @param[in] s 出力先のストリーム
/// @param[in] problem 問題
void
print_problem(ostream& s,
	      const NlProblem& problem);

END_NAMESPACE_YM_NLINK

#endif // NLPROBLEM_H
