
/// @file NlProblem.cc
/// @brief NlProblem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlProblem.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlProblem::Builder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlProblem::Builder::Builder()
{
  mWidth = 0;
  mHeight = 0;
}

// @brief デストラクタ
NlProblem::Builder::~Builder()
{
}

// @brief 以前の内容をクリアする．
void
NlProblem::Builder::clear()
{
  mWidth = 0;
  mHeight = 0;
  mElemArray.clear();
}

// @brief 盤のサイズをセットする．
// @param[in] width 幅
// @param[in] height 高さ
//
// 以前の内容はクリアされる．
void
NlProblem::Builder::set_size(ymuint width,
			     ymuint height)
{
  clear();
  mWidth = width;
  mHeight = height;
}

// @brief 線分を追加する．
// @param[in] connection 線分
void
NlProblem::Builder::add_elem(const NlConnection& connection)
{
  ASSERT_COND( connection.start_point.x() < mWidth );
  ASSERT_COND( connection.start_point.y() < mHeight );
  ASSERT_COND( connection.end_point.x() < mWidth );
  ASSERT_COND( connection.end_point.y() < mHeight );
  mElemArray.push_back(connection);
}

// @brief 線分を追加する．
// @param[in] start_point 始点
// @param[in] end_point 終点
void
NlProblem::Builder::add_elem(const NlPoint& start_point,
			     const NlPoint& end_point)
{
  add_elem(NlConnection(start_point, end_point));
}

// @brief 盤の幅を返す．
ymuint
NlProblem::Builer::width() const
{
  return mWidth;
}

// @brief 盤の高さを返す．
ymuint
NlProblem::Builder::height() const
{
  return mHeight;
}

// @brief 線分の要素数を返す．
ymuint
NlProblem::Builder::elem_num() const
{
  return mElemArray.size();
}

// @brief 線分を返す．
// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
NlConnection
NlProblem::Builder::connection(ymuint idx) const
{
  ASSERT_COND( idx < elem_num() );
  return mElemArray[idx];
}


//////////////////////////////////////////////////////////////////////
// クラス NlProblem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] builder ビルダーオブジェクト
NlProblem::NlProblem(const Builder& builder)
{
  mWidth = builder.width();
  mHeight = builder.height();
  ymuint n = builder.elem_num();
  mElemArray.reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    mElemArray.push_back(builder.connection(i));
  }
}

// @brief デストラクタ
NlProblem::~NlProblem()
{
}

// @brief 盤の幅を返す．
ymuint
NlProbmlem::width() const
{
  return mWidth;
}

// @brief 盤の高さを返す．
ymuint
NlProblem::height() const
{
  return mHeight;
}

// @brief 線分の要素数を返す．
ymuint
NlProblem::elem_num() const
{
  return mElemArray.size();
}

// @brief 線分を返す．
// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
NlConnection
NlProblem::connection(ymuint idx) const
{
  ASSERT_COND( idx < elem_num() );
  return mElemArray[idx];
}

// @brief 線分の始点を返す．
// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
NlPoint
NlProblem::start_point(ymuint idx) const
{
  ASSERT_COND( idx < elem_num() );
  return mElemArray[idx].start_point();
}

// @brief 線分の終点を返す．
// @param[in] idx 要素番号 ( 0 <= idx < elem_num() )
NlPoint
NlProblem::end_point(ymuint idx) const
{
  ASSERT_COND( idx < elem_num() );
  return mElemArray[idx].end_point();
}

// @relates NlProblem
// @brief 問題の内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] problem 問題
void
print_problem(ostream& s,
	      const NlProblem& problem)
{
  s << "SIZE        " << problem.width() << "X" << problem.height() << endl
    << "LINE_NUM    " << problem.elem_num() << endl;
  for (ymuint i = 0; i < problem.elem_num() ++ i) {
    NlConnection con = problem.connection(i);
    s << "LINE#" << (i + 1) << " ("
      << con.start_point().x() << ", " << con.start_point().y() << ") - ("
      << con.end_point().x() << ", " << con.end_point().y() << ")" << endl;
  }
}

END_NAMESPACE_YM_NLINK
