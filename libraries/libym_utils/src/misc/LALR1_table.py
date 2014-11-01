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
        self._StateList = LR0_state_list(grammer)

        # カーネル項の全リストを作る．
        kernel_list = []
        for terms in self._StateList :
            for term in terms :
                (rule_id, pos) = term
                if rule_id == grammer._StartRule or pos > 0 :
                    kernel_list.append(term)

        # 先読みの生成/伝搬を調べる．
        generated_token = []
        propagated_token = []
        dummy = grammer._DummyToken
        for state in self._StateList :
            for (rule_id, pos) in state :
                if rule_id != grammer._StartRule and pos == 0 :
                    continue
                exp_set = LR1_closure(grammer, [(rule_id, pos, dummy)])
                for (rule_id1, pos1, token1) in exp_set :
                    (left, right) = grammer.id2rule(rule_id1)
                    if pos1 == len(right) :
                        continue
                    if token1 == dummy :
                        # 伝搬された先読み
                        token = right[pos1]
                        state2 = LR0_next_state(grammer, state, token)
                        for (rule_id2, pos2) in state2 :
                            if rule_id2 == rule_id1 and pos2 == pos1 :
                                propagated_token.append( (state, rule_id, pos, state2, rule_id2, pos2 + 1) )
                    else :
                        # 生成された先読み
                        token = right[pos1]
                        state2 = LR0_next_state(grammer, state, token)
                        for (rule_id2, pos2) in state2 :
                            if rule_id2 == rule_id1 and pos2 == pos1 :
                                generated_token.append( (state2, rule_id2, pos2 + 1, token1) )
        start_state = [(grammer._StartRule, 0)]
        generated_token.append( (start_state, grammer._StartRule, 0, 0) )

        print ""
        print "*** generated look-ahead token ***"
        for (state, rule_id, pos, token) in generated_token :
            print state, rule_id, pos, token
        print ""

        print ""
        print "*** propagation rule ***"
        for (state1, rule_id1, pos1, state2, rule_id2, pos2) in propagated_token :
            print state1, rule_id1, pos1, state2, rule_id2, pos2
        print ""

        # 先読みの伝搬を行う．
        la_token_list = []
        new_token_list = generated_token
        while len(new_token_list) > 0 :
            cur_token_list = new_token_list
            new_token_list = []
            for (state, rule_id, pos, token) in cur_token_list :
                if (state, rule_id, pos, token) in la_token_list :
                    continue
                # term_id の先読みに token を追加
                la_token_list.append( (state, rule_id, pos, token) )
                # (state, rule_id, pos, token) の伝搬を調べる．
                for (state1, rule_id1, pos1, state2, rule_id2, pos2) in propagated_token :
                    if state1 == state and rule_id1 == rule_id and pos1 == pos :
                        print "propagated: ", state2, rule_id2, pos2, token
                        print " from: ", state, rule_id, pos
                        new_token_list.append( (state2, rule_id2, pos2, token) )

        # LALR(1) 正準集を作る．
        # 実際には項番号をキーにした先読みトークンリストを作る．

        """
        self.print_states(grammer)
        term_id = 0
        for token_list in self._LaTokenList :
            print "Term#%d: " % term_id, token_list
            term_id += 1
        """

        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})
            for token_id in range(0, len(grammer._TokenList)) :
                if token_id == grammer._DummyToken :
                    continue
                if grammer._TerminalList[token_id] :
                    for (rule_id, pos) in state :
                        (left, right) = grammer.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            tmp_state = LR0_next_state(grammer, state, token_id)
                            next_id = self.state2id(tmp_state)
                            assert next_id != -1
                            if not self.set_action(state_id, token_id, 'shift', next_id, grammer) :
                                print "Error: not an LALR(1) SS"
                else :
                    tmp_state = LR0_next_state(grammer, state, token_id)
                    if len(tmp_state) > 0 :
                        next_id = self.state2id(tmp_state)
                        if not self.set_action(state_id, token_id, 'goto', next_id, grammer) :
                            print "Error: not an LALR(1) GOTO"
            for (rule_id, pos) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) == pos :
                    if left == grammer._StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0, grammer) :
                            print "Error: not an LALR(1) ACCEPT"
                    else :
                        for (state1, rule_id1, pos1, token1) in la_token_list :
                            state1_id = self.state2id(state1)
                            if state1_id == state and rule_id1 == rule_id and pos1 == pos :
                                if not self.set_action(state_id, token1, 'reduce', rule_id, grammer) :
                                    print "Error: not an LALR(1) REDUCE"


    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id, grammer) :
        print "set_action(%d, %s, %s)" % (state_id, grammer._TokenList[token_id], action)
        if self._ActionMap[state_id].has_key(token_id) :
            if self._ActionMap[state_id][token_id] != (action, action_id) :
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
            LR0_print_terms(grammer, state)
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
