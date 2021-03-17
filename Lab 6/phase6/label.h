#ifndef LAB_6
#define LAB_6

# include <iostream>

class Label {
    static unsigned _counter;
    unsigned _number;

public:
    Label();
    unsigned number() const;
};

std::ostream &operator <<(std::ostream &ostr, const Label &label);

#endif /* LAB_6 */
