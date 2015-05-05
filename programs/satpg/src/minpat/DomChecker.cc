
/// @file DomChecker.cc
/// @brief DomChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DomChecker.h"

#include "FaultAnalyzer.h"

#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "DetOp.h"
#include "NodeSet.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

const bool verify_dom_check = false;
const bool print_dom_detail = false;

struct FaultLt
{
  FaultLt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.dom_cand_size(left) < mChecker.dom_cand_size(right);
  }

  DomChecker& mChecker;

};

struct FaultGt
{
  FaultGt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    if ( mChecker.single_cube(left) ) {
      if ( mChecker.single_cube(right) ) {
	return mChecker.dom_cand_size(left) > mChecker.dom_cand_size(right);
      }
      return true;
    }
    else if ( mChecker.single_cube(right) ) {
      return false;
    }
    return mChecker.dom_cand_size(left) > mChecker.dom_cand_size(right);
  }

  DomChecker& mChecker;

};

// 2つのリストが共通要素を持つとき true を返す．
// リストはソートされていると仮定する．
inline
bool
check_intersect(const vector<ymuint>& list1,
		const vector<ymuint>& list2)
{
  ymuint n1 = list1.size();
  ymuint n2 = list2.size();
  ymuint i1 = 0;
  ymuint i2 = 0;
  ymuint v1 = list1[i1];
  ymuint v2 = list2[i2];
  for ( ; ; ) {
    if ( v1 < v2 ) {
      ++ i1;
      if ( i1 >= n1 ) {
	return false;
      }
      v1 = list1[i1];
    }
    else if ( v1 > v2 ) {
      ++ i2;
      if ( i2 >= n2 ) {
	return false;
      }
      v2 = list2[i2];
    }
    else {
      return true;
    }
  }
  return false;
}

const TpgNode*
common_node(const TpgNode* node1,
	    const TpgNode* node2)
{
  ymuint id1 = node1->id();
  ymuint id2 = node2->id();
  for ( ; ; ) {
    if ( node1 == node2 ) {
      return node1;
    }
    if ( id1 < id2 ) {
      node1 = node1->imm_dom();
      if ( node1 == NULL ) {
	return NULL;
      }
      id1 = node1->id();
    }
    else if ( id1 > id2 ) {
      node2 = node2->imm_dom();
      if ( node2 == NULL ) {
	return NULL;
      }
      id2 = node2->id();
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス DomChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] analyzer 故障の情報を持つクラス
// @param[in] fsim 故障シミュレータ
// @param[in] tvmgr テストベクタのマネージャ
DomChecker::DomChecker(FaultAnalyzer& analyzer,
		       TvMgr& tvmgr,
		       Fsim& fsim) :
  mAnalyzer(analyzer),
  mTvMgr(tvmgr),
  mFsim(fsim)
{
  mVerbose = mAnalyzer.verbose();
  mMaxNodeId = mAnalyzer.max_node_id();
  mMaxFaultId = mAnalyzer.max_fault_id();
  mFaultDataArray.resize(mMaxFaultId);
  mDetFlag.resize(mMaxFaultId, 0UL);
  mDomFlag.resize(mMaxFaultId, false);

  for (ymuint i = 0; i < mMaxFaultId; ++ i) {
    mFaultDataArray[i].mDetCount = 0;
  }
}

// @brief デストラクタ
DomChecker::~DomChecker()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
DomChecker::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief 支配故障を求める．
void
DomChecker::get_dom_faults(const vector<ymuint>& src_fid_list,
			   vector<ymuint>& dom_fid_list)
{
  StopWatch local_timer;
  local_timer.start();

  mDomCheckCount = 0;
  mSmartDomCheck = 0;

  do_fsim(src_fid_list);

  USTime fsim_time = local_timer.time();

  ymuint fault_num = src_fid_list.size();
  vector<ymuint> tmp_list = src_fid_list;
  {
    // 故障を被支配故障数の少ない順に並べる．
    FaultLt comp(*this);
    sort(tmp_list.begin(), tmp_list.end(), comp);
  }

  // まず TFI に共通部分を持つ故障間の dominance check を行う．
  vector<ymuint> fault_list1;
  get_dom_faults1(tmp_list,  0, fault_list1);

  // 次に残った故障間の dominance check を行う．
  vector<ymuint> fault_list2;
  get_dom_faults1(fault_list1, 1, fault_list2);

  // 支配されていない故障を dom_fault_list に入れる．
  ymuint dom_fault_num = fault_list2.size();
  dom_fid_list.clear();
  dom_fid_list.reserve(dom_fault_num);
  ymuint single_num = 0;
  for (ymuint i = 0; i < dom_fault_num; ++ i) {
    ymuint f_id = fault_list2[i];
    dom_fid_list.push_back(f_id);

    const FaultInfo& fi = mAnalyzer.fault_info(f_id);
    if ( fi.single_cube() ) {
      ++ single_num;
    }
  }

  local_timer.stop();
  if ( mVerbose ) {
    DomStats sum;
    sum.clear();
    for (ymuint i = 0; i < 2; ++ i) {
      sum += mStats[i];
    }

    cout << "Total    " << setw(8) << src_fid_list.size()  << " original faults" << endl;
    cout << "Total    " << setw(8) << dom_fault_num   << " dominator faults" << endl;
    cout << "        (" << setw(8) << single_num      << ") single cube faults" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "         " << setw(8) << sum.mSingleSat << " sigle cube test" << endl;
    cout << "         " << setw(8) << sum.mNoDom     << " non-dominance" << endl;
    cout << "         " << setw(8) << sum.mSingleDom << " single dominance" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "         " << setw(8) << sum.mSat       << " dominance test" << endl;
    cout << "         " << setw(8) << sum.mDom       << " dominance" << endl;
    cout << "         " << setw(8) << mSmartDomCheck << " smart dom check (2)" << endl;
    cout << "         " << setw(8) << mPat << " patterns simulated" << endl;
    cout << "CPU time for dominance test" << local_timer.time() << endl
	 << "  CPU time (fsim)    " << fsim_time << endl;
    mAnalyzer.print_stats(cout);
  }
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
DomChecker::do_fsim(const vector<ymuint>& fid_list)
{
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  DetOp op;

  ymuint nf = fid_list.size();
  ymuint npat = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint fid = fid_list[i];
    const FaultInfo& fi = mAnalyzer.fault_info(fid);
    TestVector* tv = fi.testvector();
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      if ( mVerbose > 1 ) {
	cout << "\rFSIM: " << setw(6) << npat;
	cout.flush();
      }
      mFsim.ppsfp(cur_array, op);
      const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
      record_dom_cand(det_list);
      cur_array.clear();
      op.clear_det_list();
      npat += kPvBitLen;
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, op);
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    record_dom_cand(det_list);
    op.clear_det_list();
    npat += cur_array.size();
    cur_array.clear();
  }

  vector<TestVector*> cur_array2(kPvBitLen);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    TestVector* tv = mTvMgr.new_vector();
    cur_array2[i] = tv;
  }

  ymuint nochg = 0;
  for ( ; ; ) {
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array2[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array2, op);
    ymuint nchg = 0;
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    nchg += record_dom_cand(det_list);
    op.clear_det_list();
    npat += kPvBitLen;
    if ( nchg == 0 ) {
      ++ nochg;
      if ( nochg > 2 ) {
	break;
      }
    }
    else {
      nochg = 0;
    }
    if ( mVerbose > 1 ) {
      cout << "\rFSIM: " << setw(6) << npat;
      cout.flush();
    }
  }
  if ( mVerbose > 1 ) {
    cout << endl;
  }

  mPat = npat;

  // 乱数パタンは削除しておく．
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mTvMgr.delete_vector(cur_array2[i]);
  }

  // 構造的に独立な故障対を対象外にする．
  for (ymuint i1 = 0; i1 < nf; ++ i1) {
    ymuint f1_id = fid_list[i1];
    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& input_list1 = mAnalyzer.input_list(f1_id);

    vector<ymuint>& dst_list = fd1.mDomCandList1;
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < dst_list.size(); ++ rpos) {
      ymuint f2_id = dst_list[rpos];
      const vector<ymuint>& input_list2 = mAnalyzer.input_list(f2_id);
      bool intersect = check_intersect(input_list1, input_list2);
      if ( !intersect ) {
	// 共通部分を持たない故障は独立
	continue;
      }
      if ( wpos < rpos ) {
	dst_list[wpos] = f2_id;
      }
      ++ wpos;
    }
    if ( wpos < dst_list.size() ) {
      dst_list.erase(dst_list.begin() + wpos, dst_list.end());
    }
  }

  vector<ymuint> tmp_fid_list = fid_list;

  // 故障を被支配故障候補数の多い順に並べる．
  {
    // 正確にはさらに single cube condition の故障を先にする
    FaultGt comp(*this);
    sort(tmp_fid_list.begin(), tmp_fid_list.end(), comp);
  }

  // 被支配故障の候補から支配故障の候補を作る．
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint f1_id = tmp_fid_list[i];
    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& input_list1_2 = mAnalyzer.input_list2(f1_id);

    const vector<ymuint>& cand_list = fd1.mDomCandList1;
    for (ymuint j = 0; j < cand_list.size(); ++ j) {
      ymuint f2_id = cand_list[j];
      FaultData& fd2 = mFaultDataArray[f2_id];
      const vector<ymuint>& input_list2_2 = mAnalyzer.input_list2(f2_id);
      // TFI が共通部分をもつかどうかでリストを分ける．
      if ( check_intersect(input_list1_2, input_list2_2) ) {
	fd2.mDomCandList2[0].push_back(f1_id);
      }
      else {
	fd2.mDomCandList2[1].push_back(f1_id);
      }
    }
  }
}

// シミュレーション結果を用いて被支配故障の候補リストを作る．
//
// 基本的に f1 のビットベクタが f2 のビットベクタに含まれているときに
// f1 が f2 を支配している可能性がある．
// 複数回のシミュレーションでその候補を絞ってゆく．
// この関数は変更のあった故障の数を返す．
//
// 逆に支配故障のリストを直接作れないかと考えたが，この場合，
// 最初に候補となる故障は det_list に含まれない(ビットベクタが0)の故障
// を含むので効率のよいやり方が思いつかなかった．
ymuint
DomChecker::record_dom_cand(const vector<pair<ymuint, PackedVal> >& det_list)
{
  ymuint n = det_list.size();
  ymuint nchg = 0;

  // det_list の内容を mDetFlag に転写する．
  for (ymuint i = 0; i < n; ++ i) {
    ymuint f_id = det_list[i].first;
    PackedVal bv = det_list[i].second;
    mDetFlag[f_id] = bv;
  }

  // 新しくリストを作る時の作業用
  // 要素数の最大値を指定しておく．
  // reserve() を各故障に対してやってしまうとメモリが無駄になる．
  vector<ymuint> tmp_list;
  tmp_list.reserve(n);

  // 検出結果を用いて支配される故障の候補リストを作る．
  for (ymuint i1 = 0; i1 < n; ++ i1) {
    ymuint f1_id = det_list[i1].first;
    PackedVal bv1 = det_list[i1].second;
    FaultData& fd1 = mFaultDataArray[f1_id];
    if ( fd1.mDetCount == 0 ) {
      // 初めて検出された場合
      tmp_list.clear();
      for (ymuint i2 = 0; i2 < n; ++ i2) {
	if ( i2 == i1 ) {
	  continue;
	}
	ymuint f2_id = det_list[i2].first;
	PackedVal bv2 = det_list[i2].second;
	if ( (bv1 & bv2) == bv1 ) {
	  // bv1 が 1 の部分は bv2 も 1
	  tmp_list.push_back(f2_id);
	}
      }
      // 最後に要素数が確定してから mDomCandList1 にコピーする．
      fd1.mDomCandList1 = tmp_list;
    }
    else {
      // 二回目以降
      // dst_list の要素で bv1 を含まないものを落とす．
      vector<ymuint>& dst_list = fd1.mDomCandList1;
      ymuint n = dst_list.size();
      ymuint wpos = 0;
      for (ymuint rpos = 0; rpos < n; ++ rpos) {
	ymuint f2_id = dst_list[rpos];
	PackedVal bv2 = mDetFlag[f2_id];
	if ( (bv1 & bv2) == bv1 ) {
	  if ( wpos < rpos ) {
	    dst_list[wpos] = f2_id;
	  }
	  ++ wpos;
        }
      }
      if ( wpos < dst_list.size() ) {
	nchg += dst_list.size() - wpos;
	dst_list.erase(dst_list.begin() + wpos, dst_list.end());
      }
    }

    // 直接は使わないが，ヒューリスティックの評価値として
    // 検出回数を用いる．
    fd1.mDetCount += count_ones(bv1);
  }

  // mDetFlag をクリアしておく．
  for (ymuint i1 = 0; i1 < n; ++ i1) {
    ymuint f1_id = det_list[i1].first;
    mDetFlag[f1_id] = 0UL;
  }

  return nchg;
}

// @brief 支配故障を求める基本処理
// @param[in] src_list 対象の故障のリスト
// @param[in] idx 故障候補リストのインデックス
// @param[in] dst_list 支配されていない故障のリスト
void
DomChecker::get_dom_faults1(const vector<ymuint>& src_list,
			    ymuint idx,
			    vector<ymuint>& dst_list)
{
  DomStats& stats = mStats[idx];
  stats.clear();

  ymuint fault_num = src_list.size();
  ymuint cur_num = fault_num;
  for (ymuint i1 = 0; i1 < fault_num; ++ i1) {
    ymuint f1_id = src_list[i1];
    if ( mDomFlag[f1_id] ) {
      continue;
    }

    // f1 を支配する可能性のある故障番号のリスト
    FaultData& fd1 = mFaultDataArray[f1_id];
    const vector<ymuint>& cand_list = fd1.mDomCandList2[idx];
    if ( cand_list.empty() ) {
      continue;
    }

    if ( mVerbose > 1 ) {
      cout << "\rDOM" << (idx + 1) << ": " << setw(6) << i1 << " / " << setw(6) << fault_num
	   << " / " << setw(6) << cur_num;
      cout.flush();
    }

    const TpgFault* f1 = mAnalyzer.fault(f1_id);

    SatEngine engine(string(), string(), NULL);
    GvalCnf gval_cnf(mMaxNodeId);

    // f1 を検出しない CNF を作成
    FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
    engine.make_fval_cnf(fval_cnf, f1, mAnalyzer.node_set(f1_id), kVal0);

    for (ymuint i2 = 0; i2 < cand_list.size(); ++ i2) {
      ymuint f2_id = cand_list[i2];
      if ( mDomFlag[f2_id] ) {
	continue;
      }

      const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
      const TpgFault* f2 = fi2.fault();

      ++ stats.mSingleSat;

      // これが f2 の十分割当のもとで成り立ったら支配しない
      if ( engine.check_sat(gval_cnf, fi2.sufficient_assignment()) == kB3True ) {
	if ( print_dom_detail ) {
	  cout << "NODOM(1) " << f1_id << " " << f2_id << endl;
	}
	++ stats.mNoDom;
	if ( verify_dom_check ) {
	  bool check = mAnalyzer.check_dominance(f2_id, f1_id);
	  if ( check ) {
	    cout << "ERROR in check_dominance(" << f2 << ", " << f1 << ")[NODOM(1)]" << endl;
	    exit(1);
	  }
	}
	continue;
      }
      if ( fi2.single_cube() ) {
	// これ以上のチェックは必要ない．
	if ( print_dom_detail ) {
	  cout << "DOM(2) " << f1_id << " " << f2_id << endl;
	}
	mDomFlag[f1_id] = true;
	++ stats.mSingleDom;
	-- cur_num;
	if ( verify_dom_check ) {
	  bool check = mAnalyzer.check_dominance(f2_id, f1_id);
	  if ( !check ) {
	    cout << "ERROR in check_dominance(" << f2 << ", " << f1 << ")[DOM(2)]" << endl;
	    exit(1);
	  }
	}
	break;
      }

      // 実際にチェックを行う．
      ++ stats.mSat;
      if ( mAnalyzer.check_dominance(f2_id, f1_id) ) {
	if ( print_dom_detail ) {
	  cout << "DOM(3) " << f1_id << " " << f2_id << endl;
	}
	mDomFlag[f1_id] = true;
	++ stats.mDom;
	-- cur_num;
	break;
      }
      if ( print_dom_detail ) {
	cout << "NODOM(4) " << f1_id << " " << f2_id << endl;
      }
    }
  }

  // dom_flag がついた故障を落とす．
  dst_list.clear();
  dst_list.reserve(cur_num);
  for (ymuint i = 0; i < fault_num; ++ i) {
    ymuint f_id = src_list[i];
    if ( mDomFlag[f_id] ) {
      mAnalyzer.clear_fault_info(f_id);
    }
    else {
      dst_list.push_back(f_id);
    }
  }

  if ( mVerbose ) {
    stats.print(cout);
  }
}

// @brief シミュレーション時の検出パタン数を返す．
ymuint
DomChecker::det_count(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDetCount;
}

// @brief single cube condition かどうかを返す．
bool
DomChecker::single_cube(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mAnalyzer.fault_info(f_id).single_cube();
}

// @brief 非支配故障候補数を返す．
ymuint
DomChecker::dom_cand_size(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDomCandList1.size();
}

// @brief 支配故障候補数を返す．
ymuint
DomChecker::dom_cand2_size(ymuint f_id)
{
  ASSERT_COND( f_id < mMaxFaultId );
  return mFaultDataArray[f_id].mDomCandList2[0].size();
}

void
DomChecker::DomStats::clear()
{
  mSingleSat = 0;
  mNoDom = 0;
  mSingleDom = 0;
  mSat = 0;
  mDom = 0;
}

void
DomChecker::DomStats::print(ostream& s) const
{
  s << " ("
    << setw(6) << mNoDom << ", " << setw(6) << mSingleDom
    << ") / "
    << setw(6) << mSingleSat << " : "
    << setw(6) << mDom << " / " << setw(6) << mSat << endl;
}

const DomChecker::DomStats&
DomChecker::DomStats::operator+=(const DomStats& right)
{
  mSingleSat += right.mSingleSat;
  mNoDom += right.mNoDom;
  mSingleDom += right.mSingleDom;
  mSat += right.mSat;
  mDom += right.mDom;
  return *this;
}

END_NAMESPACE_YM_SATPG
