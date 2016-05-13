#ifndef BASE_H
#define BASE_H

class Base {
    private:

    public:
        Base();
        virtual bool execute() = 0;
};

class Command : public Base {
    private:
        char** args;
    public:
        Command(char** arr);
        virtual bool execute();
};

class Exit : public Command {
    public:
        Exit(char** arr); 
        virtual bool execute();
};

class Connector : public Base {
    private:
        Base* lhs;
        Base* rhs;
    public:
        Connector();
        virtual Base* get_lhs(); 
        virtual Base* get_rhs(); 
        virtual void set_lhs(Base* left); 
        virtual void set_rhs(Base* right); 
        virtual bool execute() = 0;
};

class Semicolon : public Connector {
    public:
        Semicolon();
        virtual bool execute();
};

class Ampersand : public Connector {
    public:
        Ampersand();
        virtual bool execute();
};

class Verticalbars : public Connector {
    public:
        Verticalbars();
        virtual bool execute();
};


#endif