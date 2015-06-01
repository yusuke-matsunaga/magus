
/// @file NlSolution.cc
/// @brief NlSolution の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlSolution.h"
#include "NlProblem.h"
#include "NlScanner.h"
#include "YmUtils/FileIDO.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlSolution
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NlSolution::NlSolution()
{
  mWidth = 0;
  mHeight = 0;
  mNum = 0;
}

// @brief デストラクタ
NlSolution::~NlSolution()
{
}

// @brief サイズを設定する．
// @param[in] width 幅
// @param[in] height 高さ
void
NlSolution::set_size(ymuint width,
		     ymuint height)
{
  mWidth = width;
  mHeight = height;

  ymuint n = mWidth * mHeight;
  mCellArray.clear();
  mCellArray.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mCellArray[i] = 0;
  }
}

// @brief 初期化する．
// @param[in] problem 問題
void
NlSolution::init(const NlProblem& problem)
{
  set_size(problem.width(), problem.height());

  mNum = problem.elem_num();
  for (ymuint i = 0; i < mNum; ++ i) {
    NlConnection con = problem.connection(i);
    set(con.start_point(), -(i + 1));
    set(con.end_point(), -(i + 1));
  }
}

// @brief 幅を得る．
ymuint
NlSolution::width() const
{
  return mWidth;
}

// @brief 高さを得る．
ymuint
NlSolution::height() const
{
  return mHeight;
}

// @brief 内容を設定する．
// @param[in] x, y 座標
// @param[in] val 値
void
NlSolution::set(ymuint x,
		ymuint y,
		int val)
{
  mCellArray[xy_to_index(x, y)] = val;
}

// @brief 内容を設定する．
// @param[in] point 座標
// @param[in] val 値
void
NlSolution::set(const NlPoint& point,
		int val)
{
  mCellArray[point_to_index(point)] = val;
}

// @brief 内容を読みだす．
// @param[in] x, y 座標
int
NlSolution::get(ymuint x,
		ymuint y) const
{
  return mCellArray[xy_to_index(x, y)];
}

// @brief 内容を読みだす．
// @param[in] point 座標
int
NlSolution::get(const NlPoint& point) const
{
  return mCellArray[point_to_index(point)];
}

// @brief 解が妥当か検証する．
//
// 検証項目は
// - 各線分が連結しているか
// - 冗長な線分がないか
// - 閉路がないか
bool
NlSolution::verify() const
{
  // 各セルに対して隣接セルの番号を調べ，
  // - 端点なら，同じ番号が1つ
  // - 端点以外なら同じ番号2つ
  // あることを確認する．
  bool error = false;
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      int val = get(x, y);
      if ( val == 0 ) {
	cerr << NlPoint(x, y) << ": no value" << endl;
	error = true;
	continue;
      }
      bool terminal = false;
      if ( val < 0 ) {
	val = - val;
	terminal = true;
      }
      // val の値を持つセルの個数を数える．
      ymuint count = 0;
      if ( x > 0 ) {
	int val1 = get(x - 1, y);
	if ( val1 == val || - val1 == val ) {
	  ++ count;
	}
      }
      if ( x < mWidth - 1 ) {
	int val1 = get(x + 1, y);
	if ( val1 == val || - val1 == val ) {
	  ++ count;
	}
      }
      if ( y > 0 ) {
	int val1 = get(x, y - 1);
	if ( val1 == val || - val1 == val ) {
	  ++ count;
	}
      }
      if ( y < mHeight - 1 ) {
	int val1 = get(x, y + 1);
	if ( val1 == val || - val1 == val ) {
	  ++ count;
	}
      }
      if ( terminal ) {
	if ( count != 1 ) {
	  cerr << NlPoint(x, y) << ": count != 1" << endl;
	  error = true;
	}
      }
      else {
	if ( count != 2 ) {
	  cerr << NlPoint(x, y) << ": count != 2" << endl;
	  error = true;
	}
      }
    }
  }

  return !error;
}

// @brief xy座標からインデックスを得る．
ymuint
NlSolution::xy_to_index(ymuint x,
			ymuint y) const
{
  ASSERT_COND( x >= 0 );
  ASSERT_COND( x < mWidth );
  ASSERT_COND( y >= 0 );
  ASSERT_COND( y < mHeight );

  return x * mHeight + y;
}

// @brief インデックスから xy 座標を得る．
void
NlSolution::index_to_xy(ymuint index,
			ymuint& x,
			ymuint& y) const
{
  ASSERT_COND( index >= 0 );
  ASSERT_COND( index < (mWidth * mHeight) );

  x = index / mHeight;
  y = index % mHeight;
}

/// @brief NlPoint からインデックスを得る．
ymuint
NlSolution::point_to_index(const NlPoint& point) const
{
  return xy_to_index(point.x(), point.y());
}

// @brief インデックスから NlPoint を得る．
NlPoint
NlSolution::index_to_point(ymuint index) const
{
  ymuint x;
  ymuint y;
  index_to_xy(index, x, y);
  return NlPoint(x, y);
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
NlSolution::write(ostream& s) const
{
  s << "SIZE " << width() << "X" << height() << endl;
  ios::fmtflags fsave = s.flags();
  s.fill('0');
  for (ymuint y = 0; y < height(); ++ y) {
    const char* comma = "";
    for (ymuint x = 0; x < width(); ++ x) {
      int val = get(x, y);
      if ( val < 0 ) {
	val = - val;
      }
      s << comma << setw(2) << val;
      comma = ",";
    }
    s << endl;
  }
  s.flags(fsave);
}

// @relates NlProblem
// @brief 問題を読み込む．
// @param[in] filename ファイル名
// @return 問題を返す．
NlSolution
read_solution(const string& filename)
{
  FileIDO ido;
  if ( !ido.open(filename) ) {
    cerr << filename << ": no such file" << endl;
    goto error;
  }

  {
    NlScanner scanner(ido);

    ymuint width;
    ymuint height;

    if ( !scanner.read_SIZE(width, height) ) {
      cerr << "read error: expecting 'SIZE'" << endl;
      goto error;
    }

    NlSolution solution;

    solution.set_size(width, height);

    for (ymuint y = 0; y < height; ++ y) {
      vector<ymuint> num_array(width);
      if ( !scanner.read_SOLUTION(width, num_array) ) {
	goto error;
      }
      for (ymuint x = 0; x < width; ++ x) {
	solution.set(x, y, num_array[x]);
      }
    }

    return solution;
  }

 error:
  return NlSolution();
}

// @relates NlSolution
// @brief 解を出力する．
// @param[in] s 出力先のストリーム
// @param[in] solution 解
void
print_solution(ostream& s,
	       const NlSolution& solution)
{
  ymuint w = solution.width();
  ymuint h = solution.height();
  for (ymuint y = 0; y < h; ++ y) {
    for (ymuint x = 0; x < w; ++ x) {
      int val = solution.get(x, y);
      if ( val < 0 ) {
	val = - val;
	cout << "*";
      }
      else {
	cout << " ";
      }
      cout << setw(2) << val;
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_NLINK
