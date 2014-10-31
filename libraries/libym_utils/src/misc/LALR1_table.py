# coding=UTF-8

from grammer import *

# LALR(1)構文解析表を作るモジュール
#
# - LALR(1) 項は (rule_id, pos, token) という3つ組で表す．

class LALR1_table :
    # コンストラクタ
    def __init__(self) :
        self.clear()


    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self._StateList = []

        # ACTION マップ
        self._ActionMap = []


    # @brief LALR(1)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()

        # LR0正準集を求める．
        lr0_state_list = LR0_state_list(grammer)

        # カーネル項の全リストを作る．
        kernel_list = []
        for terms in lr0_state_list :
            for term in terms :
                (rule_id, pos) = term
                if rule_id == grammer._StartRule or pos > 0 :
                    kernel_list.append(term)

        # 先読みの生成/伝搬を調べる．
        generated_token = []
        propagated_token = []
        for (rule_id, pos) in kernel_list :
            dummy = grammer._DummyToken
            exp_set = LR1_closure(grammer, [(rule_id, pos, dummy)])
            for (rule_id1, pos1, token1) in exp_set :
                (left, right) = grammer.id2rule(rule_id1)
                if pos1 < len(right) :
                    if token1 == dummy :
                        # 伝搬された先読み
                        src_id = grammer.term2id(rule_id, pos)
                        dst_id = grammer.term2id(rule_id1, pos1 + 1)
                        propagated_token.append( (src_id, dst_id) )
                    else :
                        # 生成された先読み
                        dst_id = grammer.term2id(rule_id1, pos1 + 1)
                        generated_token.append( (dst_id, token1) )

        # 先読みの伝搬を行う．
        la_token_list = []
        new_token_list = generated_token
        start_term_id = grammer.term2id(grammer._StartRule, 0)
        new_token_list.append( (start_term_id, 0) )
        while len(new_token_list) > 0 :
            cur_token_list = new_token_list
            new_token_list = []
            for (term_id, token) in cur_token_list :
                if (term_id, token) in la_token_list :
                    continue
                # term_id の先読みに token を追加
                la_token_list.append( (term_id, token) )
                # (term_id, token) の伝搬を調べる．
                for (src_id, dst_id) in propagated_token :
                    if src_id == term_id :
                        new_token_list.append( (dst_id, token) )

        # LALR(1) 正準集を作る．
        for lr0_state in lr0_state_list :
            lalr1_state = []
            for (rule_id, pos) in lr0_state :
                term_id = grammer.term2id(rule_id, pos)
                for (term_id1, token) in la_token_list :
                    if term_id1 == term_id :
                        lalr1_state.append( (rule_id, pos, token) )
            lalr1_state.sort()
            self._StateList.append(lalr1_state)

        self.print_states(grammer)

        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})
            for token_id in range(0, len(grammer._TokenList)) :
                if grammer._TerminalList[token_id] :
                    for (rule_id, pos, token) in state :
                        (left, right) = grammer.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            tmp_state = LR1_next_state(grammer, state, token_id)
                            LR1_print_terms(grammer, tmp_state)
                            print ""
                            next_id = self.state2id(tmp_state)
                            assert next_id != -1
                            if not self.set_action(state_id, token_id, 'shift', next_id) :
                                print "Error: not an LALR(1) SS"
                else :
                    tmp_state = LR1_next_state(grammer, state, token_id)
                    if len(tmp_state) > 0 :
                        next_id = self.state2id(tmp_state)
                        if not self.set_action(state_id, token_id, 'goto', next_id) :
                            print "Error: not an LALR(1) GOTO"
            for (rule_id, pos, token) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) == pos :
                    if left == grammer._StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LALR(1) ACCEPT"
                    else :
                        if not self.set_action(state_id, token, 'reduce', rule_id) :
                            print "Error: not an LALR(1) REDUCE"


    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self._ActionMap[state_id].has_key(token_id) :
            if self._ActionMap[state_id][token_id] != (action, action_id) :
                print "set_action(%d, %d, %s)" % (state_id, token_id, action)
                print "conflict with", self._ActionMap[state_id][token_id]
                return False
            else :
                return True
        else :
            self._ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief LALR(1)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self._StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            LR1_print_terms(grammer, state)
            print ""
            """
            for key in self._ActionMap[state_id].keys() :
                (action, action_id) = self._ActionMap[state_id][key]
                token = grammer.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
            print ''
            """

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state.sort()
        state_id = 0
        for state1 in self._StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1


if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    S = g.add_token('S')
    L = g.add_token('L')
    R = g.add_token('R')
    id = g.add_token('id')
    eq = g.add_token('=')
    star = g.add_token('*')

    g.add_rule(S, (L, eq, R))
    g.add_rule(S, (R, ))
    g.add_rule(L, (star, R))
    g.add_rule(L, (id, ))
    g.add_rule(R, (L, ))

    g.set_start(S)

    g.print_rules()

    g.print_tokens()

    lr1_tab = LALR1_table()

    lr1_tab.make_table(g)

    lr1_tab.print_states(g)
