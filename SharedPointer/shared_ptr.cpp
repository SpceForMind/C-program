
#include <iostream>

struct Expression
{
    virtual double evaluate() const = 0;
    virtual ~Expression()
    {}
};

struct Number : Expression
{
    double evaluate() const
    {
	return value;
    }
    Number(double value)
        : value(value)
    {}
private:
    double value;
};

struct BinaryOperation : Expression
{
    /*
      Здесь op это один из 4 символов: '+', '-', '*' или '/', соответствующих операциям,
      которые вам нужно реализовать.
     */
    double evaluate() const
    {
    		switch(op)
		{
			case '+':
				return left->evaluate() + right->evaluate();
				break;
			case '-':
				return left->evaluate() - right->evaluate();
				break;
			case '*': 
				return left->evaluate() * right->evaluate();
				break;
			case '/':
				return left->evaluate() / right->evaluate();
				break;
		}
    }
    BinaryOperation(Expression const * left, char op, Expression const * right)
        : left(left), op(op), right(right)
    { }
    ~BinaryOperation()
    {
    		delete left;
		delete right;
    }
private:
    Expression const * left;
    Expression const * right;
    char op;
};


struct SharedPtr
{
	explicit SharedPtr(Expression *ptr = 0) : ptr(ptr), count_link(0)
	{
		if(ptr!= 0)
			count_link = new size_t(1);
	}
	SharedPtr(SharedPtr const &sp)
	{
		if(sp.get()!= 0)
			++*sp.get_count();
		count_link = sp.get_count();
		ptr = sp.get();
	}
	SharedPtr & operator =(SharedPtr const &sp)
	{
		if(this!= &sp)
		{
			std::cout << "operator =" << std::endl;
			if(count_link && *count_link - 1 == 0)
				this->~SharedPtr();
			else if(count_link)
				--*count_link;
			ptr = sp.get();
			count_link = sp.get_count();
			if(ptr)
				++*count_link;
		}
		return *this;
	}
	~SharedPtr()
	{	
		std::cout << "~SharedPtr()" << std::endl;
		if(count_link && --*count_link == 0)
		{
			std::cout << "Delete ptr" << std::endl;
			delete ptr;
			delete count_link;
		}
	}	
	Expression *get() const
	{
		return ptr;
	}
	size_t *get_count() const
	{
		return count_link;
	}
	void reset(Expression * const ptr = 0)
	{
		if(ptr!= this->ptr)
		{
			if(count_link!= 0)
			{
				if(*count_link - 1 == 0)
					this->~SharedPtr();
				else
					--*count_link;
			}
			if(ptr!= 0)
				count_link = new size_t(1);
			else
				count_link = 0;		
			this->ptr = ptr;
		}
	}
    	Expression& operator *() const
	{
		return *ptr;
	}
	Expression* operator->() const
	{
		return ptr;
	}
private:
	Expression *ptr;
	size_t *count_link;
};


int main()
{
	Expression *num1 = new Number(10);
	Expression *num2 = new Number(20);
	Expression *num3 = new Number(30);
	SharedPtr sp1(0);
	sp1.reset(num2);
	std::cout << sp1.get_count() << std::endl;	

	return 0;
}
