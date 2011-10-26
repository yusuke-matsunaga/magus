#ifndef LIBYM_NPN_NPNCONF_H
#define LIBYM_NPN_NPNCONF_H

/// @file libym_npn/NpnConf.h
/// @brief NpnConf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/NpnMap.h"
#include "NpnBaseConf.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// @class NpnConf NpnConf.h "NpnConf.h"
/// @brief 1つのコンフィグレーションを表すクラス
/// 具体的には
///  - 出力の極性
///  - 入力の極性
///  - 入力の順列
/// を表す
/// ただし，上記の情報が未確定なものを含む可能性がある．
//////////////////////////////////////////////////////////////////////
class NpnConf
{
public:

  /// @brief 空のコンストラクタ
  /// @note 内容は不定
  NpnConf();

  /// @brief NpnBaseConf を指定したコンストラクタ
  NpnConf(const NpnBaseConf& base_conf);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  NpnConf(const NpnConf& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身への参照
  const NpnConf&
  operator=(const NpnConf& src);

  /// @brief 出力極性を固定するコピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  /// @param[in] pol 出力極性(-1/1)
  /// @note src.opol() == 0 でなければならない．
  NpnConf(const NpnConf& src,
	  int pol);

  /// @brief グループ g 内の c というクラスを切り出して独立したグループにする．
  /// @param[in] src コピー元のオブジェクト
  /// @param[in] g 切り出す対象のグループ番号
  /// @param[in] c 切り出す対象の等価入力クラス
  NpnConf(const NpnConf& src,
	  ymuint g,
	  ymuint c);

  /// @brief デストラクタ
  ~NpnConf();


public:

  /// @brief 対象の関数を得る．
  const TvFunc&
  func() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief 完全な正規形になっているとき true を返す．
  /// @param[in] g0 調べ始める最初のグループ番号
  bool
  is_resolved(ymuint g0 = 0) const;

  /// @brief 出力極性が確定のときに true を返す．
  bool
  is_opol_fixed() const;

  /// @brief 出力極性の割り当ての取得
  /// @return 出力極性の割り当て状態
  ///  - 0 : 未定
  ///  - 1 : 肯定
  ///  - 2 : 否定
  int
  opol() const;

  /// @brief 入力極性の取得
  /// @param[in] pos 入力番号
  /// @return 指定された入力の極性
  ///  - 0 : 未定
  ///  - 1 : 肯定
  ///  - 2 : 否定
  int
  ipol(ymuint pos) const;

  /// @brief 入力順序の取得
  /// @param[in] pos 入力位置
  /// @return pos 番めの入力番号
  ymuint
  iorder(ymuint pos) const;


public:

  /// @brief 等価入力クラス数の取得
  /// @return クラス数
  ymuint
  nc() const;

  /// @brief 等価入力クラスの取得
  /// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
  /// @return 指定された入力クラスの先頭番号＋極性
  pair<ymuint, int>
  input(ymuint cid) const;

  /// @brief 等価入力クラスの先頭番号の取得
  /// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
  /// @return 指定された入力クラスの先頭の入力番号インデックス
  ymuint
  ic_rep(ymuint cid) const;

  /// @brief 入力クラスの極性の割り当ての取得
  /// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
  /// @return pos に対応する入力の極性の割り当て状態
  ///  -  0 : 未定(肯定も否定もありうる)
  ///  -  1 : 肯定
  ///  -  2 : 否定
  int
  ic_pol(ymuint cid) const;

  /// @brief グループ数の取得
  /// @return グループ数
  ymuint
  group_num() const;

  /// @brief グループの先頭のクラス番号の取得
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  ymuint
  group_begin(ymuint gid) const;

  /// @brief グループの末尾のクラス番号取得
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @note 末尾とは最後の要素の次のクラス番号を指す．
  ymuint
  group_end(ymuint gid) const;

  /// @brief グループの要素数の取得
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @note group_end(gid) - group_begin(gid) に等しい．
  ymuint
  group_size(ymuint gid) const;


public:

  /// @brief 重み別 Walsh の 0次係数を返す．
  int
  walsh_w0(ymuint w) const;

  /// @brief Walsh の 2次係数を返す．
  int
  walsh_2(ymuint pos1,
	  ymuint pos2) const;

  /// @brief Walsh の 2次係数を返す (入力順序付き)．
  int
  walsh_2i(ymuint pos1,
	   ymuint pos2) const;

  /// @brief 内容を NpnMap にセットする．
  void
  set_map(NpnMap& map) const;


public:
  //////////////////////////////////////////////////////////////////////
  // NpnBaseConf の関数に対するラッパ
  //////////////////////////////////////////////////////////////////////

  // 等価入力クラスの要素数を返す．
  ymuint
  ic_num(ymuint rep) const;

  /// @brief 独立(無関係)な入力クラスの先頭番号を返す．
  ymuint
  indep_rep() const;

  /// @brief 独立な入力クラスの要素数を返す．
  ymuint
  indep_num() const;

  // 等価入力クラスの pos の次の要素を返す．
  ymuint
  ic_link(ymuint pos) const;

  // 等価入力クラスの bisym マークを返す．
  bool
  bisym(ymuint rep) const;

  /// @brief Walsh 係数を出力する．
  void
  dump_walsh(ostream& s) const;


public:

  /// @brief 入力クラスの極性の割り当ての設定
  /// @param[in] pos 入力クラス番号
  /// @param[in] val 割り当て状態
  /// @sa ic_pol
  void
  set_ic_pol(ymuint pos,
	     int val);

  /// @brief 入力グループの開始番号を追加する．
  /// @param[in] index 先頭の入力クラス番号
  void
  add_ig(ymuint index);

  /// @brief グループの細分化を行う．
  /// @param[in] g0 対象のグループ番号
  /// @param[in] cmp 2つの入力クラスの大小比較関数オブジェクト
  /// @return 増えたグループ数を返す．
  template <typename T>
  ymuint
  refine(ymuint g0,
	 T cmp);


public:

  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーを行う．
  void
  copy(const NpnConf& src);

  /// @brief 入力順序を正しくする．
  void
  validate_iorder() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 共通な情報
  const NpnBaseConf* mBaseConf;

  // 出力極性
  // 0 : 未定
  // 1 : 肯定
  // 2 : 否定
  ymuint8 mOpol;

  // 入力の極性
  // 0 : 未定
  // 1 : 肯定
  // 2 : 否定
  ymuint8 mIpols[TvFunc::kMaxNi];

  // 入力クラスの順列
  // インデックスはクラス番号で，
  // 各要素は下位2ビットで極性(0, 1, 2)，
  // 残りが入力配列へのインデックスを表す．
  ymuint32 mIcList[TvFunc::kMaxNi];

  // グループ数
  ymuint32 mGroupNum;

  // グループの先頭のクラス番号を納める配列．
  // i 番めのグループのクラス番号は
  // mGroupTop[i] 〜 mGroupTop[i + 1] - 1
  // に入っている．
  ymuint32 mGroupTop[TvFunc::kMaxNi + 1];

  // 入力の順序
  mutable
  ymuint32 mIorder[TvFunc::kMaxNi];

  // mIorder が正しいとき true となるフラグ
  mutable
  bool mIorderValid;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 対象の関数を得る．
inline
const TvFunc&
NpnConf::func() const
{
  return mBaseConf->func();
}

// @brief 入力数を得る．
inline
ymuint
NpnConf::ni() const
{
  return mBaseConf->ni();
}

// @brief 出力極性が確定のときに true を返す．
inline
bool
NpnConf::is_opol_fixed() const
{
  return mOpol != 0;
}

// 出力極性の割り当ての取得
inline
int
NpnConf::opol() const
{
  return mOpol;
}

// @brief 入力極性の取得
inline
int
NpnConf::ipol(ymuint pos) const
{
  return mIpols[pos];
}

// @brief 入力順序の取得
// @param[in] pos 入力位置
// @return pos 番めの入力番号
inline
ymuint
NpnConf::iorder(ymuint pos) const
{
  if ( !mIorderValid ) {
    validate_iorder();
  }
  return mIorder[pos];
}

// クラス数の取得
inline
ymuint
NpnConf::nc() const
{
  return mBaseConf->nc();;
}

// @brief 等価入力クラスの取得
// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
// @return 指定された入力クラスの先頭番号＋極性
inline
pair<ymuint, int>
NpnConf::input(ymuint cid) const
{
  int v = mIcList[cid];
  ymuint index = static_cast<ymuint>(v >> 2);
  int pol = v & 3;
  return make_pair(index, pol);
}

// @brief 等価入力クラスの先頭番号の取得
// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
// @return 指定された入力クラスの先頭の入力番号インデックス
inline
ymuint
NpnConf::ic_rep(ymuint cid) const
{
  int v = mIcList[cid];
  ymuint index = static_cast<ymuint>(v >> 2);
  return index;
}

// @brief 入力クラスの極性の割り当ての取得
// @param[in] cid 入力クラス番号 ( 0 <= cid < nc() )
// @return pos に対応する入力の極性の割り当て状態
//  -  0 : 未定(肯定も否定もありうる)
//  -  1 : 肯定
//  -  2 : 否定
inline
int
NpnConf::ic_pol(ymuint cid) const
{
  int v = mIcList[cid];
  int pol = v & 3;
  return pol;
}

// グループ数の取得
inline
ymuint
NpnConf::group_num() const
{
  return mGroupNum;
}

// @brief グループの先頭のクラス番号の取得
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
inline
ymuint
NpnConf::group_begin(ymuint gid) const
{
  return mGroupTop[gid];
}

// @brief グループの末尾のクラス番号取得
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @note 末尾とは最後の要素の次のクラス番号を指す．
inline
ymuint
NpnConf::group_end(ymuint gid) const
{
  return mGroupTop[gid + 1];
}

// @brief グループの要素数の取得
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @note group_end(gid) - group_begin(gid) に等しい．
inline
ymuint
NpnConf::group_size(ymuint gid) const
{
  return group_end(gid) - group_begin(gid);
}

#if 0

// 等価入力クラスの要素数を返す．
inline
ymuint
NpnConf::ic_num(ymuint rep) const
{
  return mBaseConf->ic_num(rep);
}

// @brief 独立(無関係)な入力クラスの先頭番号を返す．
inline
ymuint
NpnConf::indep_rep() const
{
  return mBaseConf->indep_rep();
}

// @brief 独立な入力クラスの要素数を返す．
inline
ymuint
NpnConf::indep_num() const
{
  return mBaseConf->indep_num();
}

// 等価入力クラスの pos の次の要素を返す．
inline
ymuint
NpnConf::ic_link(ymuint pos) const
{
  return mBaseConf->ic_link(pos);
}

// 等価入力クラスの bisym マークを返す．
inline
bool
NpnConf::bisym(ymuint rep) const
{
  return mBaseConf->bisym(rep);
}
#endif

#if 0
// @brief Walsh 係数を出力する．
inline
void
NpnConf::dump_walsh(ostream& s) const
{
  mBaseConf->dump_walsh(s);
}
#endif

// @brief Walsh の 2次係数を返す．
inline
int
NpnConf::walsh_2(ymuint pos1,
		 ymuint pos2) const
{
  int w2 = mBaseConf->walsh_2(pos1, pos2);
  bool oinv = (mOpol == 2);
  bool iinv1 = (mIpols[pos1] == 2);
  bool iinv2 = (mIpols[pos2] == 2);
  if ( oinv ^ iinv1 ^ iinv2 ) {
    w2 = -w2;
  }
  return w2;
}

// @brief Walsh の 2次係数を返す (入力順序付き)．
inline
int
NpnConf::walsh_2i(ymuint pos1,
		  ymuint pos2) const
{
  if ( !mIorderValid ) {
    validate_iorder();
  }
  ymuint ipos1 = mIorder[pos1];
  ymuint ipos2 = mIorder[pos2];
  return walsh_2(ipos1, ipos2);
}

// @brief 入力グループの開始番号を追加する．
// @param[in] index 先頭の入力クラス番号
inline
void
NpnConf::add_ig(ymuint index)
{
  mGroupTop[mGroupNum] = index;
  ++ mGroupNum;
  mGroupTop[mGroupNum] = nc();
}

// @brief グループの細分化を行う．
// @param[in] g0 対象のグループ番号
// @param[in] cmp 2つの入力クラスの大小比較関数オブジェクト
// @return 増えたグループ数を返す．
template <typename T>
inline
ymuint
NpnConf::refine(ymuint g0,
		T cmp)
{
  ymuint ng0 = group_num();
  ymuint s = group_begin(g0);
  ymuint e = group_end(g0);
  // 大小関数にしたがって整列させる．
  for (ymuint i = s + 1; i < e; ++ i) {
    int tmp1 = mIcList[i];
    ymuint pos1 = static_cast<ymuint>(tmp1 >> 2);
    ymuint j = i;
    for ( ; j > s; -- j) {
      int tmp2 = mIcList[j - 1];
      ymuint pos2 = static_cast<ymuint>(tmp2 >> 2);
      if ( cmp.gt(pos2, pos1) ) {
	break;
      }
      mIcList[j] = tmp2;
    }
    if ( j < i ) {
      mIcList[j] = tmp1;
      mIorderValid = false;
    }
  }
  // 等価なクラスをグループ化する．
  ymuint prev = ic_rep(s);
  for (ymuint i = s + 1; i < e; ++ i) {
    ymuint pos1 = ic_rep(i);
    if ( !cmp.eq(prev, pos1) ) {
      // 新しいグループを作る．
      for (ymuint g1 = mGroupNum; g1 > g0; -- g1) {
	mGroupTop[g1 + 1] = mGroupTop[g1];
      }
      ++ mGroupNum;
      ++ g0;
      mGroupTop[g0] = i;
      prev = pos1;
    }
  }
  return mGroupNum - ng0;
}

END_NAMESPACE_YM_NPN

#endif // YM_NPN_NPNCONF
