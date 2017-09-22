#include <iostream>

void crypto(std::string &toEncrypt, std::string key, bool encrypted)
{
    if (!encrypted){
        std::cout << "nie robie"<< std::endl;
          return;
      }
    unsigned int keySize = key.size();

    //std::cout << toEncrypt <<std::endl;
    for (unsigned int i = 0; i < toEncrypt.size (); i++)
    {
        toEncrypt[i] ^= key[keySize];
           std::cout << (int) toEncrypt[i]<<" ";

        if (keySize==0){
            keySize = key.size();
        }
        else{
            --keySize;
        }
    }
    std::cout<<""<<std::endl;
}

int main()
{
    std::string m = "054499609";
    std::string key = "054499609";

    std::cout << "msg: " << m <<std::endl;
    crypto(m,key,true);
    std::cout << "zakodowane: " << m <<" size: "<<m.size()<<std::endl;
    crypto(m,key,true);
    std::cout << "odkodowane: " << m <<" size: "<<m.size()<<std::endl;
    return 0;
}
