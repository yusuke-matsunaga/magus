
/// @file NlProblem.cc
/// @brief NlProblem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlProblem.h"
#include "NlScanner.h"
#include "YmUtils/FileIDO.h"


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
  ASSERT_COND( connection.start_point().x() < mWidth );
  ASSERT_COND( connection.start_point().y() < mHeight );
  ASSERT_COND( connection.end_point().x() < mWidth );
  ASSERT_COND( connection.end_point().y() < mHeight );

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
NlProblem::Builder::width() const
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
NlProblem::width() const
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

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
NlProblem::write(ostream& s) const
{
  s << "SIZE        " << width() << "X" << height() << endl
    << "LINE_NUM    " << elem_num() << endl;
  for (ymuint i = 0; i < elem_num(); ++ i) {
    NlConnection con = connection(i);
    s << "LINE#" << (i + 1) << " ("
      << con.start_point().x() << ", " << con.start_point().y() << ") - ("
      << con.end_point().x() << ", " << con.end_point().y() << ")" << endl;
  }
}

// @relates NlProblem
// @brief 問題を読み込む．
// @param[in] filename ファイル名
// @return 問題を返す．
NlProblem
read_problem(const string& filename)
{
  FileIDO ido;
  if ( !ido.open(filename) ) {
    cerr << filename << ": no such file" << endl;
    goto error;
  }

  {
    NlScanner scanner(ido);

    NlProblem::Builder builder;

    ymuint width;
    ymuint height;
    ymuint num;

    if ( !scanner.read_SIZE(width, height) ) {
      cerr << "read error: expecting 'SIZE'" << endl;
      goto error;
    }

    builder.set_size(width, height);

    if ( !scanner.read_LINE_NUM(num) ) {
      cerr << "read error: expecting 'LINE_NUM'" << endl;
      goto error;
    }

    for (ymuint i = 0; i < num; ++ i) {
      ymuint x1, y1, x2, y2;
      if ( !scanner.read_LINE(x1, y1, x2, y2) ) {
	cerr << "read error: expecting 'LINE#" << (i + 1) << "'" << endl;
	goto error;
      }
      builder.add_elem(NlPoint(x1, y1), NlPoint(x2, y2));
    }

    return NlProblem(builder);
  }

 error:
  return NlProblem(NlProblem::Builder());
}

// @relates NlProblem
// @brief 問題の内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] problem 問題
void
print_problem(ostream& s,
	      const NlProblem& problem)
{
  ymuint w = problem.width();
  ymuint h = problem.height();
  ymuint n = problem.elem_num();
  vector<ymuint> mark(w * h, 0);
  for (ymuint i = 0; i < n; ++ i) {
    NlConnection con = problem.connection(i);
    mark[con.start_point().x() * h + con.start_point().y()] = i + 1;
    mark[con.end_point().x() * h + con.end_point().y()] = i + 1;
  }
  cout << "   ";
  for (ymuint x = 0; x < w; ++ x) {
    cout << " " << setw(2) << x;
  }
  cout << endl;
  for (ymuint y = 0; y < h; ++ y) {
    cout << " " << setw(2) << y;
    for (ymuint x = 0; x < w; ++ x) {
      ymuint label = mark[x * h + y];
      if ( label > 0 ) {
	cout << " " << setw(2) << label;
      }
      else {
	cout << "   ";
      }
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_NLINK
