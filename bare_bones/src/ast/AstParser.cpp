/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "AstParser.hpp"
#include <iostream>
namespace ast
{

  //----------------------- delcare helper functions----------------------
  unique_ptr<AstNode> parseProgram(unique_ptr<SimpleSexpr> ss);

  unique_ptr<AstNode> parseStmt(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseAssign(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseIf(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseWhile(shared_ptr<SimpleSexpr> ss);

  unique_ptr<AstNode> parseBlock(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseLabledBlock(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseStmtBlock(shared_ptr<SimpleSexpr> ss);

  unique_ptr<AstNode> parseExpr(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseNumber(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseVariable(shared_ptr<SimpleSexpr> ss);
  unique_ptr<AstNode> parseBinOp(shared_ptr<SimpleSexpr> ss);
  unique_ptr<Error> err(shared_ptr<SimpleSexpr> ss, const string &message);
  //---------------------------------------------------------------------
  bool has_error{false};
  AstParser::AstParser(unique_ptr<SimpleSexpr> ss)
  {
    this->ss = move(ss);
  }

  pair<bool, unique_ptr<AstNode>> AstParser::parse()
  {
    unique_ptr<AstNode> result = parseProgram(move(this->ss));
    pair<bool, unique_ptr<AstNode>> p(has_error, move(result));
    has_error = false;
    return p;
  }

  unique_ptr<AstNode> parseProgram(unique_ptr<SimpleSexpr> ss)
  {
    int type = ss->type();
    if (SEQ == type)
    {
      // (parse -> stmt) ... (parse->expr)
      sexpr::Seq *seq = static_cast<sexpr::Seq *>(ss.get());
      list<shared_ptr<SimpleSexpr>> list = seq->getList();
      int len = list.size();
      if (len > 0)
      {
        shared_ptr<SimpleSexpr> last = list.back();
        list.pop_back();

        std::list<unique_ptr<AstNode>> stmts = std::list<unique_ptr<AstNode>>();
        for (auto &item : list)
        {
          unique_ptr<AstNode> stmt = parseStmt(item);
          stmts.push_back(move(stmt));
        }

        unique_ptr<AstNode> expr = parseExpr(move(last));
        return make_unique<Program>(move(stmts), move(expr));
      }
    }
    return err(move(ss), move("expect a program"));
  }

  // ------------------------Stmt------------------------------

  unique_ptr<AstNode> parseStmt(shared_ptr<SimpleSexpr> ss)
  {
    if (SEQ == ss->type())
    {
      sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
      list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();

      if (list.size() == 3)
      {
        shared_ptr<SimpleSexpr> first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_WHILE.compare(name_ptr->value()) == 0)
          {
            return parseWhile(ss);
          }
          else
          {
            return parseAssign(ss);
          }
        }
      }
      else if (list.size() == 4)
      {
        shared_ptr<SimpleSexpr> first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_IF.compare(name_ptr->value())==0)
          {
            return parseIf(ss);
          }
        }
      }
      else
      {
        return err(ss, move("Statement of Assign or If or While expected, example:(x = 1)"));
      }
      return err(ss, move("Statement of Assign or If or While expected, example:(x = 1)"));
    }
    else
    {
      return err(ss, move("an Statement expected, example:(x = 1)"));
    }
  }

  unique_ptr<AstNode> parseAssign(shared_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();
    unique_ptr<AstNode> var = parseVariable(list.front());
    list.pop_front();

    shared_ptr<SimpleSexpr> assign_sym = list.front();
    if (assign_sym->type() == NAME)
    {
      Name *name_ptr = static_cast<Name *>(assign_sym.get());
      if (KEY_WORD_ASSIGN.compare(name_ptr->value()) == 0)
      {
        list.pop_front();
        unique_ptr<AstNode> expr = parseExpr(list.front());
        return make_unique<Assign>(move(var), move(expr));
      }
    }
    return err(ss, move("an Assign statement expected, example:(x = y)"));
  }

  unique_ptr<AstNode> parseIf(shared_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();

    list.pop_front();
    unique_ptr<AstNode> test_expr = parseExpr(list.front());

    list.pop_front();
    unique_ptr<AstNode> then_block = parseBlock(list.front());
  
    list.pop_front();
    unique_ptr<AstNode> else_block = parseBlock(list.front());
    return make_unique<If>(move(test_expr), move(then_block), move(else_block));
  }

  unique_ptr<AstNode> parseWhile(shared_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();
    list.pop_front();
    unique_ptr<AstNode> test_expr = parseExpr(list.front());

    list.pop_front();
    unique_ptr<AstNode> block = parseBlock(list.front());
    return make_unique<While>(move(test_expr), move(block));
  }

  // ------------------------Expr------------------------------

  unique_ptr<AstNode> parseExpr(shared_ptr<SimpleSexpr> ss)
  {
    // seq

    if (sexpr::NUM == ss->type())
    {
      return parseNumber(ss);
    }
    else if (NAME == ss->type())
    {
      return parseVariable(ss);
    }
    else if (SEQ == ss->type())
    {
      return parseBinOp(ss);
    }
    else
    {
      // shoud nener happpen
      return err(ss, move("a BinOp expression expected, example:(+ x y)"));
    }
  }

  // ss must a Number
  unique_ptr<AstNode> parseNumber(shared_ptr<SimpleSexpr> ss)
  {
    sexpr::Number *num_ptr = static_cast<sexpr::Number *>(ss.get());
    float num = num_ptr->value();
    return make_unique<Number>(num);
  }

  // ss must an Name
  unique_ptr<AstNode> parseVariable(shared_ptr<SimpleSexpr> ss)
  {
    Name *name_ptr = static_cast<Name *>(ss.get());
    const std::string &name = name_ptr->value();
    return make_unique<Variable>(name);
  }

  // ss must an seq
  array<string_view, 3> BIN_OP{KEY_WORD_ADD, KEY_WORD_DIVID, KEY_WORD_COMPARE};
  unique_ptr<AstNode> parseBinOp(shared_ptr<SimpleSexpr> ss)
  {
    Seq *seq_ptr = static_cast<Seq *>(ss.get());
    list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();
    int len = list.size();
    if (len == 3)
    {
     
      shared_ptr<SimpleSexpr> first =  list.front();
      if (NAME == first->type())
      {
        unique_ptr<AstNode> left = parseVariable(first);
        
        list.pop_front();
        shared_ptr<SimpleSexpr> bin_op = list.front();
        Name *op_ptr = static_cast<Name *>(bin_op.get());

        bool is_bin_op = false;
        for (size_t i = 0; i < BIN_OP.size(); i++)
        {
          if (BIN_OP[i].compare(op_ptr->value()) == 0)
          {
            is_bin_op = true;
            break;
          }
        }

        if (!is_bin_op)
        {
          return err(ss, move("a BinOp expression expected, example:(+ x y)"));
        }
        list.pop_front();

        unique_ptr<AstNode> right = parseVariable(list.front());
        return make_unique<BinOp>(op_ptr->value(), move(left), move(right));
      }
    }
    return err(ss, move("a BinOp expression expected, example:(+ x y)"));
  }

  // ------------------------block------------------------------
  unique_ptr<AstNode> parseBlock(shared_ptr<SimpleSexpr> ss)
  {
    // seq
    if (SEQ == ss->type())
    {
      sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
      list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();
      int len = list.size();
      if (len >= 3)
      {
        shared_ptr<SimpleSexpr> first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_BLOCK.compare(name_ptr->value()) == 0)
          {
            return parseLabledBlock(ss);
          }
          else
          {
            return parseStmtBlock(ss);
          }
        }
        else
        {
          return parseStmtBlock(ss);
        }
      }
    }
    return err(ss, move("a block expected, example:(block (+ x y)) "));
  }

  unique_ptr<AstNode> parseLabledBlock(shared_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<shared_ptr<SimpleSexpr>> list = seq_ptr->getList();
    list.pop_front();

    std::list<unique_ptr<AstNode>> stmts = std::list<unique_ptr<AstNode>>();
    for (auto &item : list)
    {
      unique_ptr<AstNode> stmt = parseStmt(item);
      stmts.push_back(move(stmt));
    }
    if (stmts.size() >= 1)
    {
      return make_unique<LabledBlock>(move(stmts));
    }
    return err(ss, "a loabled block expected");
  }

  unique_ptr<AstNode> parseStmtBlock(shared_ptr<SimpleSexpr> ss)
  {
    return parseStmt(ss);
  }

  unique_ptr<Error> err(shared_ptr<SimpleSexpr> ss, const string &message)
  {
    has_error = true;
    //cout << "err:" << message << endl;
    return make_unique<Error>(ss, move(message));
  }
}