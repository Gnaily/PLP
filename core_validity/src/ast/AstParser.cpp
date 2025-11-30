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
  variant<shared_ptr<Program>, Error> parseProgram(unique_ptr<SimpleSexpr> ss);

  bool isDecl(unique_ptr<SimpleSexpr>& ss);

  variant<Declare, Error> parseDecl(unique_ptr<SimpleSexpr> ss);

  variant<shared_ptr<Statment>, Error> parseStmt(unique_ptr<SimpleSexpr> ss);
  variant<Assign, Error> parseAssign(unique_ptr<SimpleSexpr> ss);
  variant<If, Error> parseIf(unique_ptr<SimpleSexpr> ss);
  variant<While, Error> parseWhile(unique_ptr<SimpleSexpr> ss);

  variant<shared_ptr<Block>, Error> parseBlock(unique_ptr<SimpleSexpr> ss);
  variant<LabledBlock, Error> parseLabledBlock(unique_ptr<SimpleSexpr> ss);
  variant<StmtBlock, Error> parseStmtBlock(unique_ptr<SimpleSexpr> ss);

  variant<shared_ptr<Expr>, Error> parseExpr(unique_ptr<SimpleSexpr> ss);
  variant<Number, Error> parseNumber(unique_ptr<SimpleSexpr> ss);
  variant<Variable, Error> parseVariable(unique_ptr<SimpleSexpr> ss);
  variant<BinOp, Error> parseBinOp(unique_ptr<SimpleSexpr> ss);
  Error err(unique_ptr<SimpleSexpr> ss, string message);
  //---------------------------------------------------------------------
  bool has_error{false};
  AstParser::AstParser(unique_ptr<SimpleSexpr> ss)
  {
    this->ss = move(ss);
  }

  pair<bool, variant<shared_ptr<Program>, Error>> AstParser::parse()
  {
    return pair<bool, variant<shared_ptr<Program>, Error>>(has_error, parseProgram(move(this->ss)));
  }

  variant<shared_ptr<Program>, Error> parseProgram(unique_ptr<SimpleSexpr> ss)
  {
    int type = ss->type();
    if (SEQ == type)
    {
      int type = ss->type();
      if (SEQ == type)
      {
        // parse(decl) ...parse(stmt)... (parse->expr)
        sexpr::Seq *seq = static_cast<sexpr::Seq *>(ss.get());
        std::list<unique_ptr<SimpleSexpr>> &list = seq->getList();
        int len = list.size();
        if (len > 0)
        {
          unique_ptr<SimpleSexpr> last = move(list.back());
          list.pop_back();
          std::list<variant<Declare, Error>> decls = std::list<variant<Declare, Error>>();
          std::list<variant<shared_ptr<Statment>, Error>> stmts = std::list<variant<shared_ptr<Statment>, Error>>();
          for (auto &item : list)
          {
            if(isDecl(item))
            {
              variant<Declare, Error> decl = parseDecl(move(item));
              decls.push_back(move(decl));
            }
            else
            {
              // TODO: should check if item is stmt before call parseStmt
              variant<shared_ptr<Statment>, Error> stmt = parseStmt(move(item));
              stmts.push_back(move(stmt));
            }
          }
          variant<shared_ptr<Expr>, Error> expr = parseExpr(move(last));
          return make_shared<Program>(move(decls),move(stmts), move(expr));
        }
      }
    }
    return err(move(ss), "expect a program");
  }

  bool isDecl(unique_ptr<SimpleSexpr> &ss)
  {
    if (SEQ == ss->type())
    {
      sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
      std::list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
      if (list.size() > 0)
      {
        unique_ptr<SimpleSexpr> &first = list.front();
        if (NAME == first->type())
        {
          Name *name_ptr = static_cast<Name *>(first.get());
          if (KEY_WORD_DEFINE.compare(name_ptr->value()) == 0)
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  variant<Declare, Error> parseDecl(unique_ptr<SimpleSexpr> ss)
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
          if (KEY_WORD_DEFINE.compare(name_ptr->value()) == 0)
          {
            list.pop_front();
            variant<Variable, Error> var = parseVariable(move(list.front()));
            list.pop_front();
            variant<shared_ptr<Expr>, Error> expr = parseExpr(move(list.front()));
            return Declare(move(var), move(expr));
          }
        }
      }
    }
    return err(move(ss), "an Declare expected, example:(def xyz  1)");
  }
  // ------------------------Stmt------------------------------

  variant<shared_ptr<Statment>, Error> parseStmt(unique_ptr<SimpleSexpr> ss)
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
            variant<While, Error> v = parseWhile(move(ss));
            if (holds_alternative<While>(v))
            {
              return make_shared<While>(move(get<While>(v)));
            }
            else
            {
              return move(get<Error>(v));
            }
          }
          else
          {
            variant<Assign, Error> v = parseAssign(move(ss));
            if (holds_alternative<Assign>(v))
            {
              return make_shared<Assign>(move(get<Assign>(v)));
            }
            else
            {
              return move(get<Error>(v));
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
            variant<If, Error> v = parseIf(move(ss));
            if (holds_alternative<If>(v))
            {
              return make_shared<If>(move(get<If>(v)));
            }
            else
            {
              return move(get<Error>(v));
            }
          }
        }
      }
      else
      {
        return err(move(ss), "Statement of Assign or If or While expected, example:(x = 1)");
      }
      return err(move(ss), "Statement of Assign or If or While expected, example:(x = 1)");
    }
    else
    {
      return err(move(ss), "an Statement expected, example:(x = 1)");
    }
  }

  variant<Assign, Error> parseAssign(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    unique_ptr<SimpleSexpr> &first = list.front();
    variant<Variable, Error> var = parseVariable(move(first));
    list.pop_front();
    unique_ptr<SimpleSexpr> &assign_sym = list.front();
    if (assign_sym->type() == NAME)
    {
      Name *name_ptr = static_cast<Name *>(assign_sym.get());
      if (KEY_WORD_ASSIGN.compare(name_ptr->value()) == 0)
      {
        list.pop_front();
        variant<shared_ptr<Expr>, Error> expr = parseExpr(move(list.front()));
        return Assign(move(var), move(expr));
      }
    }

    return err(move(ss), "an Assign statement expected, example:(x = y)");
  }

  variant<If, Error> parseIf(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();

    list.pop_front();
    variant<shared_ptr<Expr>, Error> test_expr = parseExpr(move(list.front()));
    list.pop_front();
    variant<shared_ptr<Block>, Error> then_block = parseBlock(move(list.front()));

    list.pop_front();
    variant<shared_ptr<Block>, Error> else_block = parseBlock(move(list.front()));
    return If(move(test_expr), move(then_block), move(else_block));
  }

  variant<While, Error> parseWhile(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    list.pop_front();
    variant<shared_ptr<Expr>, Error> test_expr = parseExpr(move(list.front()));

    list.pop_front();
    variant<shared_ptr<Block>, Error> block = parseBlock(move(list.front()));
    return While(move(test_expr), move(block));
  }

  // ------------------------Expr------------------------------

  variant<shared_ptr<Expr>, Error> parseExpr(unique_ptr<SimpleSexpr> ss)
  {
    // seq

    if (sexpr::NUM == ss->type())
    {
      variant<Number, Error> v = parseNumber(move(ss));
      if (holds_alternative<Number>(v))
      {
        return make_shared<Number>(get<Number>(v));
      }
      else
      {
        return move(get<Error>(v));
      }
    }
    else if (NAME == ss->type())
    {
      variant<Variable, Error> v = parseVariable(move(ss));
      if (holds_alternative<Variable>(v))
      {
        return make_shared<Variable>(get<Variable>(v));
      }
      else
      {
        return move(get<Error>(v));
      }
    }
    else if (SEQ == ss->type())
    {
      variant<BinOp, Error> v = parseBinOp(move(ss));
      if (holds_alternative<BinOp>(v))
      {
        return make_shared<BinOp>(move(get<BinOp>(v)));
      }
      else
      {
        return move(get<Error>(v));
      }
    }
    else
    {
      // shoud nener happpen
      abort();
    }
  }

  // ss must a Number
  variant<Number, Error> parseNumber(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Number *num_ptr = static_cast<sexpr::Number *>(ss.get());
    float num = num_ptr->value();
    return Number(num);
  }

  // ss must an Name
  variant<Variable, Error> parseVariable(unique_ptr<SimpleSexpr> ss)
  {
    Name *name_ptr = static_cast<Name *>(ss.get());
    return Variable(name_ptr->value());
  }

  // ss must an seq
  array<string_view, 3> BIN_OP{KEY_WORD_ADD, KEY_WORD_DIVID, KEY_WORD_COMPARE};
  variant<BinOp, Error> parseBinOp(unique_ptr<SimpleSexpr> ss)
  {
    Seq *seq_ptr = static_cast<Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    int len = list.size();
    if (len == 3)
    {

      unique_ptr<SimpleSexpr> &first = list.front();
      if (NAME == first->type())
      {
        variant<Variable, Error> left = parseVariable(move(first));
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
          return err(move(ss), "a BinOp expression expected, example:(x + y)");
        }

        list.pop_front();
        unique_ptr<SimpleSexpr> &right_ss = list.front();
        variant<Variable, Error> right = parseVariable(move(right_ss));
        return BinOp(op_ptr->value(), get<Variable>(left), get<Variable>(right));
      }
    }
    return err(move(ss), "a BinOp expression expected, example:(x + y)");
  }

  // ------------------------block------------------------------
  variant<shared_ptr<Block>, Error> parseBlock(unique_ptr<SimpleSexpr> ss)
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
            variant<LabledBlock, Error> v = parseLabledBlock(move(ss));
            if (holds_alternative<LabledBlock>(v))
            {
              return make_shared<LabledBlock>(move(get<LabledBlock>(v)));
            }
            else
            {
              return move(get<Error>(v));
            }
          }
          else
          {
            variant<StmtBlock, Error> v = parseStmtBlock(move(ss));
            if (holds_alternative<StmtBlock>(v))
            {
              return make_shared<StmtBlock>(move(get<StmtBlock>(v)));
            }
            else
            {
              return move(get<Error>(v));
            }
          }
        }
      }
    }
    return err(move(ss), "a block expected, example:(block (+ x y)) ");
  }

  variant<LabledBlock, Error> parseLabledBlock(unique_ptr<SimpleSexpr> ss)
  {
    sexpr::Seq *seq_ptr = static_cast<sexpr::Seq *>(ss.get());
    list<unique_ptr<SimpleSexpr>> &list = seq_ptr->getList();
    list.pop_front();
    std::list<variant<Declare, Error>> decls = std::list<variant<Declare, Error>>();
    std::list<variant<shared_ptr<Statment>, Error>> stmts = std::list<variant<shared_ptr<Statment>, Error>>();
    for (auto &item : list)
    {
      if (isDecl(item))
      {
        variant<Declare, Error> decl = parseDecl(move(item));
        decls.push_back(move(decl));
      }
      else
      {
        variant<shared_ptr<Statment>, Error> stmt = parseStmt(move(item));
        stmts.push_back(move(stmt));
      }
    }
    if (stmts.size() >= 1)
    {
      return LabledBlock(move(decls), move(stmts));
    }
    return err(move(ss), "a labled block expected");
  }

  variant<StmtBlock, Error> parseStmtBlock(unique_ptr<SimpleSexpr> ss)
  {
    variant<shared_ptr<Statment>, Error> res = parseStmt((move(ss)));
    if (holds_alternative<Error>(res))
    {
      return move(get<Error>(res));
    }
    return StmtBlock(get<shared_ptr<Statment>>(res));
  }

  Error err(unique_ptr<SimpleSexpr> ss, string message)
  {
    has_error = true;
    return Error(move(ss), message);
  }
}