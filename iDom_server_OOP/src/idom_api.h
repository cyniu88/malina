#ifndef IDOM_API_H
#define IDOM_API_H

class iDom_API
{

public:
    virtual ~iDom_API(){};
    virtual std::string dump() const = 0;
};

#endif // IDOM_API_H
