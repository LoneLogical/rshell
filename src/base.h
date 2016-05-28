#ifndef BASE_H
#define BASE_H

class Base {
    private:

    public:
        Base();
        virtual bool execute() = 0;
};

class Command : public Base {
    protected:
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

class Test : public Command {
	private:
		/*
		struct stat{
			dev_t     st_dev;     
			ino_t     st_ino;     
			mode_t    st_mode;    
			nlink_t   st_nlink;   
			uid_t     st_uid;     
			gid_t     st_gid;     
		    dev_t     st_rdev;    
			off_t     st_size;    
			blksize_t st_blksize; 
			blkcnt_t  st_blocks;  
			time_t    st_atime; 
			time_t    st_mtime; 
			time_t    st_ctime;
		};
		*/
	public:
		Test(char** arr);
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
