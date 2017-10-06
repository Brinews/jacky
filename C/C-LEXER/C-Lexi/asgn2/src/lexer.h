/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>
#include <map>

#include "inputbuf.h"

// Lexer modified for FIRST & FOLLOW project

typedef enum { END_OF_FILE = 0, ARROW, HASH, DOUBLEHASH, ID, ERROR } TokenType;

typedef enum { TERM, NONTERM } SymbolType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    SymbolType symbol_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

    // task 1
    void ParseGrammer();
    void MarkSymbols();
    void PrintSymbols();

    // task 2
    void MarkUseful();
    void PrintUsefulRule();

    // task 3
    void MarkFirst();
    void PrintFirstSet();

    // task 4
    void MarkFellow();
    void PrintFellowSet();

    // task 5
    void MarkPredictive();
    void PrintPredictive();

  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    // task 1
    std::vector<std::vector<Token> >rules;
    std::map<std::string, int> nonterms;//non-terminals hash<name, lineno>
    std::map<std::string, int> terms;

    std::vector<std::string> nonterm_seq; // order by appear sequence
    std::vector<std::string> term_seq;

    // task 2
    std::vector<bool> rule_useless;
    std::map<std::string, bool> nonterm_useless;

    typedef std::map<std::string, int>::value_type keyType;
    typedef std::map<std::string, bool>::value_type useType;
    
    void UpdateRuleUse(std::string start);

    // task 3
    std::map<std::string, std::vector<std::string> > first;

    typedef std::map<std::string, std::vector<std::string> >::value_type vecType;

    // copy first of keysrc to keydst with epislon or not
    int AddFirsts(std::string keydst, std::string keysrc, 
            bool withEpislon);
    // judge first of key contains ft or not
    bool ContainsFirst(std::string key, std::string ft);

    // task 4
    std::map<std::string, std::vector<std::string> > fellow;

    // add First(row.col) to fellow of key
    int AddFellows(std::string key, int row, int col);
    int CopyFellows(std::string keydst, std::vector<std::string> fws);
    // get First(row.col)
    std::vector<std::string> SetFirst(int row, int col);
    bool ContainsEpsilon(int row, int col);

    //task 5
    bool predictive;
    std::vector<std::vector<int> > ptable;

    int FindIndex(std::vector<std::string> &vec, std::string term);

    bool SkipSpace();
    Token ScanId();

};

#endif  //__LEXER__H__
