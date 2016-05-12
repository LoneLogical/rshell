//base.h

#ifndef BASE_H
#define BASE_H

class Base {
    private:

    public:
        Base();
        ~Base();
        virtual bool execute() = 0;
};

class Command : public Base {
    private:
        char* args[];

    public:
        Command(char ** arr[]);
        ~Command();
        virtual bool execute();
};

class Exit : public Command {
    public:
        Exit(); 
        ~Exit();
        virtual bool execute();
};

class Connector : public Base {
    private:
        Base* lhs;
        Base* rhs;
    public:
        Connector();
        ~Connector();
        virtual Base* get_lhs(); 
        virtual Base* get_rhs(); 
        virtual Base* set_lhs(Base* left); 
        virtual Base* set_rhs(Base* right); 
        virtual bool execute() = 0;
};

class Semicolon : public Connector {
    public:
        Semicolon();
        ~Semicolon();
        virtual bool execute();
};

class Ampersand : public Connector {
    public:
        Ampersand();
        ~Ampersand();
        virtual bool execute();
};

class Verticalbars : public Connector {
    public:
        Verticalbars();
        ~Verticalbars();
        virtual bool execute();
};


#endif

