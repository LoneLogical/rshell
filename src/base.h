#ifndef BASE_H
#define BASE_H

class Base {
    private:

    public:
        Base();
        virtual bool execute(int, int) = 0;
        virtual bool check_type() = 0;
        virtual char** get_info() = 0;
        virtual void set_lhs(Base*) = 0;
        virtual void set_rhs(Base*) = 0;
};

class Command : public Base {
    protected:
        char** args;
    public:
        Command(char**);
        virtual bool execute(int, int);
        virtual bool check_type();
        virtual char** get_info();
        virtual void set_lhs(Base*);
        virtual void set_rhs(Base*);
};

class Exit : public Command {
    public:
        Exit(char**); 
        virtual bool execute(int, int);
};

class Test : public Command {
	public:
		Test(char**);
		virtual bool execute(int, int);
};

class Connector : public Base {
    private:
        Base* lhs;
        Base* rhs;
    public:
        Connector();
        virtual Base* get_lhs(); 
        virtual Base* get_rhs(); 
        virtual void set_lhs(Base*); 
        virtual void set_rhs(Base*); 
        virtual bool check_type();
        virtual char** get_info();
        virtual bool execute(int, int) = 0;
};

class Semicolon : public Connector {
    public:
        Semicolon();
        virtual bool execute(int, int);
};

class Ampersand : public Connector {
    public:
        Ampersand();
        virtual bool execute(int, int);
};

class Verticalbars : public Connector {
    public:
        Verticalbars();
        virtual bool execute(int, int);
};

class InputRedirect : public Connector {
    public:
        InputRedirect();
        virtual bool execute(int, int);
};

class OutputRedirect : public Connector {
    public:
        OutputRedirect();
        virtual bool execute(int, int); 
};

class OutputAppend : public Connector {
    public:
        OutputAppend();
        virtual bool execute(int, int);
};

class Pipe : public Connector {
    public:
        Pipe();
        virtual bool execute(int, int);
};

#endif
