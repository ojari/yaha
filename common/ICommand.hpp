#pragma once
#include <string>

struct ICommand {
    ICommand(std::string flag, std::string description) : 
        _flag(flag),
        _description(description)
    {}
    
    virtual void execute() = 0;

    std::string description() {
        return _description;
    }
    
    std::string flag() {
        return _flag;
    }

    bool active = false;

protected:
    std::string _flag;
    std::string _description;
};
