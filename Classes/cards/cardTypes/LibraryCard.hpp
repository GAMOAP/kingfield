//
//  LibraryCard.hpp
//  kingfield-mobile
//
//  Created by alexandre gimeno on 17/07/2019.
//

#ifndef LibraryCard_hpp
#define LibraryCard_hpp

#include "Card.hpp"

class LibraryCard : public Card
{
public:
    static LibraryCard* setCard(int number, std::string board);
    
    bool virtual init(int number);
    
    bool virtual initDisplay();
    
    virtual void setSelect();
    virtual void setUnselect();
    
    void setPage(int pageNbr);
    int getPage();
    
private:
    void setLibraryCardSelect();
    
protected:
    int m_page;
    
    const std::vector<std::vector<int>> m_libraryNumber = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};
    
    cocos2d::Sequence* m_rotSeq = nullptr;
};

#endif /* LibraryCard_hpp */
