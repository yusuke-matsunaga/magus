
/// @file igugen.cc
/// @brief igugen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "RegVect.h"
#include "Variable.h"
#include "utils/HeapTree.h"


BEGIN_NAMESPACE_YM_IGF

struct Lt
{
  bool
  operator()(const pair<ymuint, Variable*>& left,
	     const pair<ymuint, Variable*>& right)
  {
    return left.first > right.first;
  }
};


int
igugen(int argc,
       char** argv)
{
  IguGen ig;

  if ( argc != 2 ) {
    cerr << "USAGE: " << argv[0] << " <filename>" << endl;
    return 1;
  }

  ifstream ifs(argv[1]);
  if ( !ifs ) {
    cerr << "Could not open " << argv[1] << endl;
    return 1;
  }

  if ( !ig.read_data(ifs) ) {
    cerr << "Error in reading " << argv[1] << endl;
    return 1;
  }

  cout << "# of inputs:     " << ig.vect_size() << endl
       << "# of vectors:    " << ig.vect_list().size() << endl
       << "# of index bits: " << ig.index_size() << endl;

  HeapTree<pair<ymuint, Variable*>, Lt> var_set;
  const vector<const RegVect*>& v_list = ig.vect_list();
  ymuint ni = ig.vect_size();
  for (ymuint i = 0; i < ni; ++ i) {
    Variable* var1 = new Variable(i);
    ymuint n0 = 0;
    ymuint n1 = 0;
    for (vector<const RegVect*>::const_iterator p = v_list.begin();
	 p != v_list.end(); ++ p) {
      const RegVect* rv = *p;
      if ( var1->classify(rv) ) {
	++ n1;
      }
      else {
	++ n0;
      }
    }
    if ( n0 > 0 && n1 > 0 ) {
      ymuint n2 = n0 * n1;
      var_set.put(make_pair(n2, var1));
      cout << "Var#" << i << ": " << n0 << " x " << n1 << " = " << (n0 * n1) << endl;
    }
    else {
      delete var1;
    }
  }

  for ( ; ; ) {
    pair<ymuint, Variable*> p = var_set.getmin();
    ymuint n_old = p.first;
    Variable* var_old = p.second;
    cout << "min var = ";
    var_old->dump(cout);
    cout << " n = " << n_old << endl;
    const vector<ymuint>& vid_list0 = var_old->vid_list();
    vector<bool> used(ni, false);
    for (vector<ymuint>::const_iterator p = vid_list0.begin();
	 p != vid_list0.end(); ++ p) {
      used[*p] = true;
    }
    ymuint max_n = 0;
    Variable* max_var =NULL;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( used[i] ) {
	continue;
      }
      vector<ymuint> vid_list1(vid_list0);
      vid_list1.push_back(i);
      Variable* var1 = new Variable(vid_list1);
      ymuint n0 = 0;
      ymuint n1 = 0;
      for (vector<const RegVect*>::const_iterator p = v_list.begin();
	   p != v_list.end(); ++ p) {
	const RegVect* rv = *p;
	if ( var1->classify(rv) ) {
	  ++ n1;
	}
	else {
	  ++ n0;
	}
      }
      ymuint n2 = n0 * n1;
      if ( max_n < n2 ) {
	max_n = n2;
	delete max_var;
	max_var = var1;
      }
      else {
	delete var1;
      }
    }
    if ( max_n > n_old ) {
      var_set.popmin();
      var_set.put(make_pair(max_n, max_var));
      cout << "New Var:";
      max_var->dump(cout);
      cout << " n = " << max_n << endl;
    }
    else {
      break;
    }
  }

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     char** argv)
{
  return nsYm::nsIgf::igugen(argc, argv);
}
