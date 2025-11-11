/**
 * @author yangliang
 * @copyright Copyright (c) 2025 yangliang. All rights reserved.
 */

#include "AstParser.hpp"
#include <iostream>
namespace ast
{
  using namespace std;
  //----------------------- delcare helper functions----------------------
  variant<unique_ptr<Program>, unique_ptr<Error>> parseProgram(unique_ptr<SimpleSexpr> ss);

  variant<unique_ptr<Statment>, unique_ptr<Error>> parseStmt(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<Assign>, unique_ptr<Error>> parseAssign(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<If>, unique_ptr<Error>> parseIf(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<While>, unique_ptr<Error>> parseWhile(unique_ptr<SimpleSexpr> ss);

  variant<unique_ptr<Block>, unique_ptr<Error>> parseBlock(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<LabledBlock>, unique_ptr<Error>> parseLabledBlock(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<StmtBlock>, unique_ptr<Error>> parseStmtBlock(unique_ptr<SimpleSexpr> ss);

  variant<unique_ptr<Expr>, unique_ptr<Error>> parseExpr(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<Number>, unique_ptr<Error>> parseNumber(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<Variable>, unique_ptr<Error>> parseVariable(unique_ptr<SimpleSexpr> ss);
  variant<unique_ptr<BinOp>, unique_ptr<Error>> parseBinOp(unique_ptr<SimpleSexpr> ss);
  unique_ptr<Error> err(unique_ptr<SimpleSexpr> ss, unique_ptr<string> message);
  //---------------------------------------------------------------------
  bool has_error{false};
  AstParser::AstParser(unique_ptr<SimpleSexpr> ss)
  {
    this->ss = move(ss);
  }

  pair<bool, variant<unique_ptr<Program>, unique_ptr<Error>>> AstParser::parse()
  {
    return pair<bool, variant<unique_ptr<Program>, unique_ptr<Error>>>(has_error, parseProgram(move(this->ss)));
  }

  variant<unique_ptr<Program>, unique_ptr<Error>> parseProgram(unique_ptr<SimpleSexpr> ss)
  {
    int type = ss->type();
    if (SEQ == type)
    {
      // (parse -> stmt) ... (parse->expr)
      sexpr::Seq *seq = static_cast<sexpr::Seq *>(ss.get());
      list<unique_ptr<SimpleSexpr>> &list = seq->getList();
      int len = list.size();
      if (len > 0)
      {
        unique_ptr<SimpleSexpr> last = move(list.back());
        list.pop_back();

        std::list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts = std::list<variant<unique_ptr<Statment>, unique_ptr<Error>>>();
        for (auto &item : list)
        {
          variant<unique_ptr<Statment>, unique_ptr<Error>> stmt = parseStmt(move(item));
          stmts.push_back(move(stmt));
        }

        variant<unique_ptr<Expr>, unique_ptr<Error>> expr = parseExpr(move(last));
        return make_unique<Program>(move(stmts), move(expr));
      }
    }
    unique_ptr<string> msg = make_unique<string>("expect a program");
    return err(move(ss), move(msg));
  }

  // ------------------------Stmt------------------------------

  variant<unique_ptr<Statment>, unique_ptr<Error>> parseStmt(unique_ptr<SimpleSexpr> ss)
  {
    if (SEQ == ss->type())
    {
      sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
      list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();

      if (list.size() == 3)
      {
        unique_ptr<SimpleSexpr> &first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_WHILE.compare(name_ptr->value()) == 0)
          {
            variant<unique_ptr<While>, unique_ptr<Error>> v = parseWhile(move(ss));
            if (holds_alternative<unique_ptr<While>>(v))
            {
              return move(get<unique_ptr<While>>(v));
            }
            else
            {
              return move(get<unique_ptr<Error>>(v));
            }
          }
          else
          {
            variant<unique_ptr<Assign>, unique_ptr<Error>> v = parseAssign(move(ss));
            if (holds_alternative<unique_ptr<Assign>>(v))
            {
              return move(get<unique_ptr<Assign>>(v));
            }
            else
            {
              return move(get<unique_ptr<Error>>(v));
            }
          }
        }
      }
      else if (list.size() == 4)
      {
        unique_ptr<SimpleSexpr> &first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_IF.compare(name_ptr->value()) == 0)
          {
            variant<unique_ptr<If>, unique_ptr<Error>> v = parseIf(move(ss));
            if (holds_alternative<unique_ptr<If>>(v))
            {
              return move(get<unique_ptr<If>>(v));
            }
            else
            {
              return move(get<unique_ptr<Error>>(v));
            }
          }
        }
      }
      else
      {
        unique_ptr<string> msg = make_unique<string>("Statement of Assign or If or While expected, example:(x = 1)");
        return err(move(ss), move(msg));
      }
      unique_ptr<string> msg = make_unique<string>("Statement of Assign or If or While expected, example:(x = 1)");
      return err(move(ss), move(msg));
    }
    else
    {
      unique_ptr<string> msg = make_unique<string>("an Statement expected, example:(x = 1)");
      return err(move(ss), move(msg));
    }
  }

  variant<unique_ptr<Assign>, unique_ptr<Error>> parseAssign(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    unique_ptr<SimpleSexpr> &first = list.front();
    variant<unique_ptr<Variable>, unique_ptr<Error>> var = parseVariable(move(first));
    list.pop_front();
    unique_ptr<SimpleSexpr> &assign_sym = list.front();
    if (assign_sym->type() == NAME)
    {
      Name *name_ptr = static_cast<Name *>(assign_sym.get());
      if (KEY_WORD_ASSIGN.compare(name_ptr->value()) == 0)
      {
        list.pop_front();
        variant<unique_ptr<Expr>, unique_ptr<Error>> expr = parseExpr(move(list.front()));
        return make_unique<Assign>(move(var), move(expr));
      }
    }

    unique_ptr<string> msg = make_unique<string>("an Assign statement expected, example:(x = y)");
    return err(move(ss), move(msg));
  }

  variant<unique_ptr<If>, unique_ptr<Error>> parseIf(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();

    list.pop_front();
    variant<unique_ptr<Expr>, unique_ptr<Error>> test_expr = parseExpr(move(list.front()));
    list.pop_front();
    variant<unique_ptr<Block>, unique_ptr<Error>> then_block = parseBlock(move(list.front()));

    list.pop_front();
    variant<unique_ptr<Block>, unique_ptr<Error>> else_block = parseBlock(move(list.front()));
    return make_unique<If>(move(test_expr), move(then_block), move(else_block));
  }

  variant<unique_ptr<While>, unique_ptr<Error>> parseWhile(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    list.pop_front();
    variant<unique_ptr<Expr>, unique_ptr<Error>> test_expr = parseExpr(move(list.front()));

    list.pop_front();
    variant<unique_ptr<Block>, unique_ptr<Error>> block = parseBlock(move(list.front()));
    return make_unique<While>(move(test_expr), move(block));
  }

  // ------------------------Expr------------------------------

  variant<unique_ptr<Expr>, unique_ptr<Error>> parseExpr(unique_ptr<SimpleSexpr> ss)
  {
    // seq

    if (sexpr::NUM == ss->type())
    {
      variant<unique_ptr<Number>, unique_ptr<Error>> v = parseNumber(move(ss));
      if (holds_alternative<unique_ptr<Number>>(v))
      {
        return move(get<unique_ptr<Number>>(v));
      }
      else
      {
        return move(get<unique_ptr<Error>>(v));
      }
    }
    else if (NAME == ss->type())
    {
      variant<unique_ptr<Variable>, unique_ptr<Error>> v = parseVariable(move(ss));
      if (holds_alternative<unique_ptr<Variable>>(v))
      {
        return move(get<unique_ptr<Variable>>(v));
      }
      else
      {
        return move(get<unique_ptr<Error>>(v));
      }
    }
    else if (SEQ == ss->type())
    {
      variant<unique_ptr<BinOp>, unique_ptr<Error>> v = parseBinOp(move(ss));
      if (holds_alternative<unique_ptr<BinOp>>(v))
      {
        return move(get<unique_ptr<BinOp>>(v));
      }
      else
      {
        return move(get<unique_ptr<Error>>(v));
      }
    }
    else
    {
      // shoud nener happpen
      abort();
    }
  }

  // ss must a Number
  variant<unique_ptr<Number>, unique_ptr<Error>> parseNumber(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Number *num_ptr = static_cast<sexpr::Number *>(ss.get());
    float num = num_ptr->value();
    return make_unique<Number>(num);
  }

  // ss must an Name
  variant<unique_ptr<Variable>, unique_ptr<Error>> parseVariable(unique_ptr<SimpleSexpr> ss)
  {
    Name *name_ptr = static_cast<Name *>(ss.get());
    const std::string &name = name_ptr->value();
    return make_unique<Variable>(move(make_unique<string>(name)));
  }

  // ss must an seq
  array<string_view, 3> BIN_OP{KEY_WORD_ADD, KEY_WORD_DIVID, KEY_WORD_COMPARE};
  variant<unique_ptr<BinOp>, unique_ptr<Error>> parseBinOp(unique_ptr<SimpleSexpr> ss)
  {
    Seq *seq_ptr = static_cast<Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    int len = list.size();
    if (len == 3)
    {

      unique_ptr<SimpleSexpr> &first = list.front();
      if (NAME == first->type())
      {
        variant<unique_ptr<Variable>, unique_ptr<Error>> left = parseVariable(move(first));
        list.pop_front();
        unique_ptr<SimpleSexpr> bin_op = move(list.front());
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
          unique_ptr<string> msg = make_unique<string>("a BinOp expression expected, example:(x + y)");
          return err(move(ss), move(msg));
        }

        list.pop_front();
        unique_ptr<SimpleSexpr> &right_ss = list.front();
        variant<unique_ptr<Variable>, unique_ptr<Error>> right = parseVariable(move(right_ss));
        unique_ptr<string> op_str = make_unique<string>(op_ptr->value());
        return make_unique<BinOp>(move(op_str), move(left), move(right));
      }
    }
    unique_ptr<string> msg = make_unique<string>("a BinOp expression expected, example:(x + y)");
    return err(move(ss), move(msg));
  }

  // ------------------------block------------------------------
  variant<unique_ptr<Block>, unique_ptr<Error>> parseBlock(unique_ptr<SimpleSexpr> ss)
  {
    // seq
    if (SEQ == ss->type())
    {
      sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
      list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
      int len = list.size();
      if (len >= 3)
      {
        unique_ptr<SimpleSexpr> &first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_BLOCK.compare(name_ptr->value()) == 0)
          {
            variant<unique_ptr<LabledBlock>, unique_ptr<Error>> v = parseLabledBlock(move(ss));
            if (holds_alternative<unique_ptr<LabledBlock>>(v))
            {
              return move(get<unique_ptr<LabledBlock>>(v));
            }
            else
            {
              return move(get<unique_ptr<Error>>(v));
            }
          }
          else
          {
            variant<unique_ptr<StmtBlock>, unique_ptr<Error>> v = parseStmtBlock(move(ss));
            if (holds_alternative<unique_ptr<StmtBlock>>(v))
            {
              return move(get<unique_ptr<StmtBlock>>(v));
            }
            else
            {
              return move(get<unique_ptr<Error>>(v));
            }
          }
        }
      }
    }
    unique_ptr<string> msg = make_unique<string>("a block expected, example:(block (+ x y)) ");
    return err(move(ss), move(msg));
  }

  variant<unique_ptr<LabledBlock>, unique_ptr<Error>> parseLabledBlock(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    list.pop_front();

    std::list<variant<unique_ptr<Statment>, unique_ptr<Error>>> stmts = std::list<variant<unique_ptr<Statment>, unique_ptr<Error>>>();
    for (auto &item : list)
    {
      variant<unique_ptr<Statment>, unique_ptr<Error>> stmt = parseStmt(move(item));
      stmts.push_back(move(stmt));
    }
    if (stmts.size() >= 1)
    {
      return make_unique<LabledBlock>(move(stmts));
    }
    unique_ptr<string> msg = make_unique<string>("a labled block expected");
    return err(move(ss), move(msg));
  }

  variant<unique_ptr<StmtBlock>, unique_ptr<Error>> parseStmtBlock(unique_ptr<SimpleSexpr> ss)
  {
    variant<unique_ptr<Statment>, unique_ptr<Error>> res = parseStmt((move(ss)));
    variant<unique_ptr<StmtBlock>, unique_ptr<Error>> ret;
    if (holds_alternative<unique_ptr<Error>>(res))
    {
      return move(get<unique_ptr<Error>>(res));
    }
    unique_ptr<StmtBlock> stmt = make_unique<StmtBlock>(move(get<unique_ptr<Statment>>(res)));
    return stmt;
  }

  unique_ptr<Error> err(unique_ptr<SimpleSexpr> ss, unique_ptr<string> message)
  {
    has_error = true;
    return make_unique<Error>(move(ss), move(message));
  }
}