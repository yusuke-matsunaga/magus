
/// @file RvMgr.cc
/// @brief RvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RvMgr.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス RvMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RvMgr::RvMgr()
{
  mVectSize = 0;
  mBlockSize = 0;
  mRvSize = 0;
  mAlloc = NULL;
}

// @brief デストラクタ
// @note このオブジェクトが確保したすべての RegVec が削除される．
RvMgr::~RvMgr()
{
  delete mAlloc;
}


BEGIN_NONAMESPACE

ymuint
read_num(const string& str,
	 ymuint& rpos)
{
  ymuint ans = 0;
  for ( ; ; ) {
    char c = str[rpos];
    ++ rpos;
    if ( c >= '0' && c <= '9' ) {
      ans = ans * 10 + static_cast<ymuint>(c - '0');
    }
    if ( c == ' ' || c == '\0' ) {
      return ans;
    }
  }
}

END_NONAMESPACE

// @brief データを読み込む．
// @param[in] s 読み込み元のストリーム演算子
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
RvMgr::read_data(istream& s)
{
  // 最初の行はベクタのサイズと要素数(残りの行数)
  string buf;
  getline(s, buf);

  ymuint rpos = 0;
  ymuint n = read_num(buf, rpos);
  ymuint k = read_num(buf, rpos);

  set_size(n);

  mVectList.clear();
  mVectList.reserve(k);

  for (ymuint i = 0; i < k; ++ i) {
    string buf;
    if ( !getline(s, buf) ) {
      cerr << "read error" << endl;
      return false;
    }
    if ( buf.size() != n ) {
      cerr << "data length error at line " << (i + 1) << endl;
      return false;
    }
    for (ymuint j = 0; j < n; ++ j) {
      char c = buf[j];
      if ( c != '0' && c != '1' ) {
	cerr << "illegal charactor at line "
	     << (i + 1) << ", column " << (j + 1) << endl;
	return false;
      }
    }
    RegVect* rv = new_vector(i + 1);
    for (ymuint j = 0; j < n; ++ j) {
      ymuint blk = j / 64;
      ymuint sft = j % 64;
      char c = buf[j];
      if ( c == '1' ) {
	rv->mBody[blk] |= (1ULL << sft);
      }
    }
    bool found = false;
    for (vector<RegVect*>::iterator p = mVectList.begin();
	 p != mVectList.end(); ++ p) {
      RegVect* rv1 = *p;
      bool diff = false;
      for (ymuint j = 0; j < mBlockSize; ++ j) {
	if ( rv1->mBody[j] != rv->mBody[j] ) {
	  diff = true;
	  break;
	}
      }
      if ( !diff ) {
	found = true;
	break;
      }
    }
    if ( found ) {
      // 重複したデータ
      delete_vector(rv);
    }
    else {
      mVectList.push_back(rv);
    }
  }

  return true;
}

// @brief インデックスのサイズを得る．
ymuint
RvMgr::index_size() const
{
  ymuint k = mVectList.size();
  ++ k;
  ymuint ans = 0;
  ymuint m = 1;
  while ( m < k ) {
    ++ ans;
    m <<= 1;
  }
  return ans;
}

// @brief ベクタのサイズを設定する．
// @note 付随するいくつかの処理を行う．
void
RvMgr::set_size(ymuint size)
{
  assert_cond( mVectSize == 0, __FILE__, __LINE__);
  mVectSize = size;
  mBlockSize = (size + 63) / 64;
  mRvSize = sizeof(RegVect) + 8 * (mBlockSize - 1);
  mAlloc = new UnitAlloc(mRvSize, 1024);
}

// @brief ベクタを作る．
// @param[in] index インデックス
RegVect*
RvMgr::new_vector(ymuint index)
{
  void* p = mAlloc->get_memory(mRvSize);
  RegVect* rv = new (p) RegVect(mVectSize, index);

  // 最初に0に初期化しておく．
  for (ymuint i = 0; i < mBlockSize; ++ i) {
    rv->mBody[i] = 0ULL;
  }

  return rv;
}

// @brief ベクタを削除する．
// @param[in] vec 削除対象のベクタ
void
RvMgr::delete_vector(RegVect* vec)
{
  mAlloc->put_memory(mRvSize, vec);
}

// @brief 内容を出力する．
void
RvMgr::dump(ostream& s) const
{
  for (vector<RegVect*>::const_iterator p = mVectList.begin();
       p != mVectList.end(); ++ p) {
    RegVect* rv = *p;
    rv->dump(s);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RegVect
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．
void
RegVect::dump(ostream& s) const
{
  s << setw(8) << index() << ": ";
  for (ymuint i = 0; i < size(); ++ i) {
    s << val(i);
  }
  s << endl;
}


END_NAMESPACE_YM_IGF
