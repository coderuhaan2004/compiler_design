#include <bits/stdc++.h>
using namespace std;
// AST class for expressions
class ExprAST{
	public:
		virtual ~ExprAST() = default; //virtual destructor
};
// AST class for numeric literals
class NumberExprAST : public ExprAST{ //Inherited from ExprAST
	double val;

	public:
		NumberExprAST(double val) : val(val){}
};
// AST class for Variable expressions
class VariableExprAST : public ExprAST{
	string Name;
	public:
		VariableExprAST(const string &Name): Name(Name) {}
};
// AST class for binary expressions
class BinaryExprAST : public ExprAST {
	char Op;
	unique_ptr<ExprAST> LHS, RHS;
	public:
		BinaryExprAST(char Op, unique_ptr<ExprAST> LHS, unique_ptr<ExprAST> RHS) : Op(Op), LHS(move(LHS)), RHS(move(RHS)) {}
};
// AST class for function calls
class CallExprAST : public ExprAST {
	std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

	public:
  		CallExprAST(const string &Callee,vector<unique_ptr<ExprAST>> Args): Callee(Callee), Args(move(Args)) {}
};

class PrototypeAST {
	string Name;
	vector<string> Args;

	public:
	  	PrototypeAST(const string &Name, std::vector<std::string> Args): Name(Name), Args(move(Args)) {}

  	const string &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  
  	unique_ptr<PrototypeAST> Proto;
	unique_ptr<ExprAST> Body;

	public:
  		FunctionAST(unique_ptr<PrototypeAST> Proto, unique_ptr<ExprAST> Body) : Proto(move(Proto)), Body(move(Body)) {}
};

static unique_ptr<ExprAST> ParseNumberExpr() {
	auto Result = make_unique<NumberExprAST>(NumVal); //Create NumberExprAST 
	getNextToken(); //from the lexer
	return move(Result); 
}

static unique_ptr<ExprAST> ParseExpression() {
	string IdName = IdentifierStr; //Global variable
	getNextToken();

	if (CurTok != '(')
		return make_unique<VariableExprAST>(IdName);
	getNextToken(); //eat (
}









