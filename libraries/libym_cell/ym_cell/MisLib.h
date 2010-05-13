#ifndef YM_CELL_MISLIB_H
#define YM_CELL_MISLIB_H

/// @file ym_cell/MisLib.h
/// @brief MisLib のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MisLib.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.

#include <ym_cell/cell_nsdef.h>
#include <ym_lexp/LogExpr.h>
#include <ym_utils/ShString.h>


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MisLibDelayPara MisLib.h <ym_cell/MisLib.h>
/// @brief ディレイパラメータ
//////////////////////////////////////////////////////////////////////
struct MisLibDelayPara
{
  
  /// @brief 基本ディレイ
  double mBase;
  
  /// @brief ファンアウトの負荷によるディレイ
  double mFanout;
  
};


//////////////////////////////////////////////////////////////////////
/// @class MisLibPin MisLib.h <ym_cell/MisLib.h>
/// @brief 入力ピンに関する情報
//////////////////////////////////////////////////////////////////////
struct MisLibPin
{
  
  /// @brief ピン名
  ShString mName;
  
  /// @brief 位相
  int mPhase;
  
  /// @brief 入力負荷
  double mLoad;
  
  /// @brief ドライブできる最大負荷
  double mMaxLoad;
  
  /// @brief 0->1 の入力遷移の時のディレイパラメータ
  MisLibDelayPara mRise_delay;
  
  /// @brief 1->0 の入力遷移の時のディレイパラメータ
  MisLibDelayPara mFall_delay;
  
};


//////////////////////////////////////////////////////////////////////
/// @class MisLibGate MisLib.h <ym_cell/MisLib.h>
/// @brief ゲート(セル)に関する情報
//////////////////////////////////////////////////////////////////////
class MisLibGate
{
  friend class MisLib;
  
private:
  
  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  MisLibGate(const string& name,
	     double area,
	     const ShString& oname,
	     ymuint32 ni);

  /// @brief デストラクタ
  ~MisLibGate();
  

public:

  /// @brief 論理式を設定する．
  void
  set_expr(const LogExpr& expr);

  /// @brief ピンのオブジェクトを取り出す．
  MisLibPin*
  ipin(ymuint32 pos);
  

public:
  
  /// @brief 名前を取り出す．
  string
  name() const;

  /// @brief 面積を取り出す．
  double
  area() const;

  /// @brief 論理式を取り出す．
  const LogExpr&
  expr() const;

  /// @brief 入力ピン数を取り出す．
  ymuint32
  ni() const;

  /// @brief ピンのオブジェクトを取り出す．( const 版 )
  const MisLibPin*
  ipin(ymuint32 pos) const;
  
  /// @brief 入力ピン名を取り出す．
  string
  ipin_name(ymuint32 pos) const;

  /// @brief 出力ピン名を取り出す．
  string
  oname() const;

  /// @brief 内容を表示する．
  void
  dump(ostream& s) const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル名
  string mName;
  
  // 面積
  double mArea;
  
  // 機能を表す論理式(factored form)
  LogExpr mExpr;
  
  // ピンの情報を収める配列
  vector<MisLibPin> mPins;
  
  // 出力ピン名
  ShString mOName;
  
};


//////////////////////////////////////////////////////////////////////
/// @class MisLib MisLib.h <ym_cell/MisLib.h>
/// @brief セルライブラリ全体の情報を保持するクラス
//////////////////////////////////////////////////////////////////////
class MisLib
{
public:

  /// @brief コンストラクタ
  MisLib();

  /// @brief デストラクタ
  ~MisLib();
  

public:
  
  /// @brief ファイルを読み込む
  /// @param[in] file_name ファイル名
  /// @param[out] err_list エラーメッセージのリスト
  bool
  read(const string& file_name,
       list<string>& err_list);

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief セル数を得る．
  ymuint32
  num() const;
  
  /// @brief pos 番めのセルを取り出す．
  const MisLibGate*
  cell(ymuint32 pos) const;

  /// @brief セル名で検索してセルを得る．
  const MisLibGate*
  cell(const string& name) const;
  

public:
  
  /// @brief 新たなセルを加える．
  MisLibGate*
  new_gate(const string& name,
	   double area,
	   const ShString& oname,
	   ymuint32 ni);
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // セルを納めた配列
  vector<MisLibGate*> mGates;
  
  // セル名をキーにしてセルを納めたハッシュ表
  hash_map<string, MisLibGate*> mDict;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス MisLibGate
//////////////////////////////////////////////////////////////////////

// @brief 論理式を設定する．
inline
void
MisLibGate::set_expr(const LogExpr& expr)
{
  mExpr = expr;
}

// @brief 名前を取り出す．
inline
string
MisLibGate::name() const
{
  return mName;
}

// @brief 面積を取り出す．
inline
double
MisLibGate::area() const
{
  return mArea;
}

// @brief 論理式を取り出す．
inline
const LogExpr&
MisLibGate::expr() const
{
  return mExpr;
}

// @brief 入力ピン数を取り出す．
inline
ymuint32
MisLibGate::ni() const
{
  return mPins.size();;
}

// @brief ピンのオブジェクトを取り出す．
inline
MisLibPin*
MisLibGate::ipin(ymuint32 pos)
{
  return &mPins[pos];
}

// @brief ピンのオブジェクトを取り出す．( const 版 )
inline
const MisLibPin*
MisLibGate::ipin(ymuint32 pos) const
{
  return &mPins[pos];
}
  
// @brief 入力ピン名を取り出す．
inline
string
MisLibGate::ipin_name(ymuint32 pos) const
{
  return mPins[pos].mName;
}

// @brief 出力ピン名を取り出す．
inline
string
MisLibGate::oname() const
{
  return mOName;
}


//////////////////////////////////////////////////////////////////////
// クラス MisLib
//////////////////////////////////////////////////////////////////////

// @brief セル数を得る．
inline
ymuint32
MisLib::num() const
{
  return mGates.size();
}
  
// @brief pos 番めのセルを取り出す．
inline
const MisLibGate*
MisLib::cell(ymuint32 pos) const
{
  return mGates[pos];
}

// @brief セル名で検索してセルを得る．
inline
const MisLibGate*
MisLib::cell(const string& name) const
{
  hash_map<string, MisLibGate*>::const_iterator p = mDict.find(name);
  if ( p != mDict.end() ) {
    return p->second;
  }
  return NULL;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_MISLIB_H
